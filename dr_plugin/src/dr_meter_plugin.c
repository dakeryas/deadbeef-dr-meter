#include <stdlib.h>
#include <unistd.h>

#include "dr_meter_plugin.h"
#include "block_analyser.h"
#include "dr_meter.h"
#include "thread_data.h"
#include "thread_runner.h"
#include "selection.h"
#include "duration.h"
#include "dr_log_printer.h"
#include "filepath.h"

// constants according to DR standard
static const unsigned DR_BLOCK_DURATION = 3;
#define DEFAULT_DR_FORMAT "DR%-2.0f %7.2f dB %9.2f dB"

DB_functions_t* ddb_api;

static DB_decoder_t* get_decoder(DB_playItem_t* selection_item)
{
    DB_decoder_t* decoder = NULL;
    const char* decoder_id = ddb_api->pl_find_meta(selection_item, ":DECODER");
    if(decoder_id)
    {
        ddb_api->pl_lock();
        decoder = (DB_decoder_t*)ddb_api->plug_get_for_id(decoder_id);
        ddb_api->pl_unlock();
    }
    else fprintf(stderr, "Can't find decoder for selected item\n");
    return decoder;
}

static void decode_analyse_block(char* buffer, int buffer_size, DB_fileinfo_t* fileinfo, DB_decoder_t* decoder, block_analyser_t* analyser)
{
    int decoded_bytes = decoder->read(fileinfo, buffer, buffer_size);
    analyse_block(analyser, buffer, decoded_bytes, fileinfo->fmt.channels, fileinfo->fmt.bps);
}

static unsigned number_of_blocks(DB_playItem_t* item)
{
    double duration = ddb_api->pl_get_item_duration(item);
    unsigned nu_blocks = duration / DR_BLOCK_DURATION;
    if(duration > nu_blocks * DR_BLOCK_DURATION) nu_blocks += 1;
    return nu_blocks;
}

static int get_number_of_bytes(unsigned duration, DB_fileinfo_t* fileinfo)
{
    return duration * fileinfo->fmt.samplerate * fileinfo->fmt.channels * (fileinfo->fmt.bps >> 3);
}

static int allocate_buffer(char** buffer, unsigned duration, DB_fileinfo_t* fileinfo)
{
    int buffer_size = get_number_of_bytes(duration, fileinfo);
    *buffer = malloc(buffer_size);
    if(buffer) return buffer_size;
    else return 0;
}

static void process_item(DB_playItem_t* item, DB_fileinfo_t* fileinfo, DB_decoder_t* decoder, dr_stats_t* dr_stats)
{
    decoder->init(fileinfo, item);
    char* buffer = NULL;
    int buffer_size = allocate_buffer(&buffer, DR_BLOCK_DURATION, fileinfo);
    if(buffer_size)
    {
        block_analyser_t analyser = make_block_analyser(fileinfo->fmt.channels);
        unsigned nu_blocks = number_of_blocks(item);
        dr_meter_t dr_meter = make_dr_meter(fileinfo->fmt.channels, nu_blocks);
        for(unsigned block_index = 0 ; block_index < nu_blocks; ++block_index)
        {
            decode_analyse_block(buffer, buffer_size, fileinfo, decoder, &analyser);
            fill_dr_meter(&dr_meter, &analyser);
        }
        fill_avg_dr_stats_dr_meter(&dr_meter, dr_stats);
        free_block_analyser(&analyser);
        free_dr_meter(&dr_meter);
    }
    free(buffer);
}

static void compute_single_dr(DB_playItem_t* selection_item, dr_stats_t* dr_stats)
{
    DB_decoder_t* decoder = get_decoder(selection_item);
    if(decoder)
    {
        DB_fileinfo_t* fileinfo = decoder->open(DDB_DECODER_HINT_RAW_SIGNAL);
        if(fileinfo) process_item(selection_item, fileinfo, decoder, dr_stats);
        decoder->free(fileinfo);
    }
}

static void* thread_worker(void* thread_datum)
{
    thread_datum_t* datum = (thread_datum_t*) thread_datum;
    compute_single_dr(datum->item, &datum->dr_stats);
    return NULL;
}

static int compute_dr_impl(thread_data_t* thread_data)
{
    int nu_threads = sysconf(_SC_NPROCESSORS_ONLN) - 1;
    if(nu_threads <= 0) nu_threads = 1;
    thread_runner_t thread_runner = make_thread_runner(thread_data, nu_threads);
    run_batches(&thread_runner, thread_worker);
    free_thread_runner(&thread_runner);
    return 0;
}

static const char* get_track_filename(void* track)
{
    DB_playItem_t* item = track;
    ddb_api->pl_lock();
    const char* path = ddb_api->pl_find_meta_raw(item, ":URI");
    ddb_api->pl_unlock();
    return get_filename(path);
}

static unsigned sprint_filename(void* track, char* begin)
{
    char* end = begin;
    const char* filename = get_track_filename(track);
    end += sprintf(end, "  %.83s", filename);
    return end - begin;
}

static unsigned sprint_tagged_track_info(void* track, int track_number, char* begin)
{
    DB_playItem_t* item = track;
    char* end = begin;
    ddb_api->pl_lock();
    const char* title = ddb_api->pl_find_meta_raw(item, "title");
    ddb_api->pl_unlock();
    end += sprintf(end, "  %02d-%-.80s", track_number, title);
    return end - begin;
}

static unsigned sprint_track_info(void* track, char* begin)
{
    DB_playItem_t* item = track;
    char* end = begin;
    duration_t duration = make_duration(ddb_api->pl_get_item_duration(item));
    end += sprint_duration(&duration, end);
    int track_number = ddb_api->pl_find_meta_int(item, "track", 0);
    if(track_number) end += sprint_tagged_track_info(item, track_number, end);
    else end += sprint_filename(item, end);
    return end - begin;
}

static unsigned sprint_folder_info(void* track, char* begin)
{
    DB_playItem_t* item = track;
    char* end = begin;
    ddb_api->pl_lock();
    const char* path = ddb_api->pl_find_meta_raw(item, ":URI");
    ddb_api->pl_unlock();
    char dirname[120];
    sprint_dirname(dirname, path);
    end += sprintf(end, "Analysed folder: %.120s", dirname);
    return end - begin;
}

static unsigned sprint_album_info(void* track, char* begin)
{
    DB_playItem_t* item = track;
    char* end = begin;
    ddb_api->pl_lock();
    const char* artist = ddb_api->pl_find_meta_raw(item, "artist");
    const char* album = ddb_api->pl_find_meta_raw(item, "album");
    ddb_api->pl_unlock();
    if(!artist && !album) end += sprint_folder_info(item, end);
    else end += sprintf(end, "Analysed: %.40s / %.80s", artist, album);
    return end - begin;
}

unsigned sprint_dr_log_impl(thread_data_t* thread_data, char* buffer)
{
    char dr_format[28];
    ddb_api->conf_get_str("dr_meter.format", DEFAULT_DR_FORMAT, dr_format, sizeof(dr_format));
    dr_log_printer_t log_printer = {.dr_format = dr_format, .sprint_track_info = sprint_track_info, .sprint_album_info = sprint_album_info};
    return sprint_log_dr_log_printer(&log_printer, thread_data, buffer);
}

int dr_meter_start()
{
    return 0;
}

int dr_meter_stop()
{
    return 0;
}

static const char settings_dialog[] =
    "property \"DR Peak RMS printf formatting\" entry dr_meter.format \"" DEFAULT_DR_FORMAT "\";\n"
;

DB_plugin_t* ddb_dr_meter_load(DB_functions_t* api)
{
    ddb_api = api;
    static dr_meter_plugin_t dr_meter_plugin = {
        .info.plugin.type            = DB_PLUGIN_MISC,
        .info.plugin.api_vmajor      = 1,
        .info.plugin.api_vminor      = 10,
        .info.plugin.version_major   = 0,
        .info.plugin.version_minor   = 2,
        .info.plugin.id              = "dr_meter",
        .info.plugin.name            = "Dynamic Range Meter",
        .info.plugin.descr           = "Measure Dynamic Range",
        .info.plugin.copyright       =
        "Copyright (C) 2021 Valérian Sibille <mrdakeryas@gmail.com>\n"
        ,
        .info.plugin.website         = "https://github.com/dakeryas/deadbeef-dr-meter.git",
        .info.plugin.start           = dr_meter_start,
        .info.plugin.stop            = dr_meter_stop,
        .info.plugin.configdialog    = settings_dialog,
        .compute_dr                  = compute_dr_impl,
        .sprint_dr_log               = sprint_dr_log_impl,
    };
    return (DB_plugin_t*)&dr_meter_plugin;
}
