#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "dr_meter_plugin.h"
#include "block_analyser.h"
#include "dr_meter.h"
#include "tagged_dr_data.h"
#include "thread_runner.h"
#include "selection.h"
#include "duration.h"
#include "dr_log_printer.h"
#include "filepath.h"

// constants according to DR standard
static const unsigned DR_BLOCK_DURATION = 3;
#define DEFAULT_DR_FORMAT "DR%-2.0f %10.2f dB %8.2f dB"

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

static DB_fileinfo_t* get_fileinfo(DB_playItem_t* item)
{
    DB_fileinfo_t* fileinfo = NULL;
    DB_decoder_t* decoder = get_decoder(item);
    if(decoder)
    {
        fileinfo = decoder->open(DDB_DECODER_HINT_RAW_SIGNAL);
        decoder->init(fileinfo, item);
    }
    return fileinfo;
}

static void free_fileinfo(DB_fileinfo_t* fileinfo)
{
    if(fileinfo->plugin) fileinfo->plugin->free(fileinfo);
}

static void decode_analyse_block(char* buffer, int buffer_size, DB_fileinfo_t* fileinfo, block_analyser_t* analyser)
{
    int decoded_bytes = fileinfo->plugin->read(fileinfo, buffer, buffer_size);
    analyse_block(analyser, buffer, decoded_bytes, fileinfo->fmt.channels, fileinfo->fmt.bps);
}

static unsigned number_of_blocks(DB_playItem_t* item)
{
    double duration = ddb_api->pl_get_item_duration(item);
    unsigned nu_blocks = duration / DR_BLOCK_DURATION;
    if(duration > nu_blocks * DR_BLOCK_DURATION) nu_blocks += 1;
    return nu_blocks;
}

static unsigned get_number_of_bytes(unsigned duration, const DB_fileinfo_t* fileinfo)
{
    return duration * fileinfo->fmt.samplerate * fileinfo->fmt.channels * (fileinfo->fmt.bps >> 3);
}

static unsigned allocate_buffer(char** buffer, unsigned duration, const DB_fileinfo_t* fileinfo)
{
    unsigned buffer_size = get_number_of_bytes(duration, fileinfo);
    *buffer = malloc(buffer_size);
    if(*buffer) return buffer_size;
    else return 0;
}

static void process_item(DB_playItem_t* item, DB_fileinfo_t* fileinfo, dr_stats_t* dr_stats)
{
    char* buffer = NULL;
    unsigned buffer_size = allocate_buffer(&buffer, DR_BLOCK_DURATION, fileinfo);
    if(buffer_size)
    {
        block_analyser_t analyser = make_block_analyser(fileinfo->fmt.channels);
        unsigned nu_blocks = number_of_blocks(item);
        dr_meter_t dr_meter = make_dr_meter(fileinfo->fmt.channels, nu_blocks);
        for(unsigned block_index = 0 ; block_index < nu_blocks; ++block_index)
        {
            decode_analyse_block(buffer, buffer_size, fileinfo, &analyser);
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
    DB_fileinfo_t* fileinfo = get_fileinfo(selection_item);
    if(fileinfo)
    {
        process_item(selection_item, fileinfo, dr_stats);
        free_fileinfo(fileinfo);
    }
}

static void datum_work(tagged_dr_datum_t* datum)
{
    compute_single_dr((DB_playItem_t*)datum->item, &datum->dr_stats);
}

static int get_number_of_threads()
{
    int number_of_threads = ddb_api->conf_get_int("dr_meter.threads", 0);
    if(number_of_threads <= 0) number_of_threads = 1;//against negative user value
    return number_of_threads;
}

static int compute_dr_impl(tagged_dr_data_t* tagged_dr_data)
{
    thread_runner_t thread_runner = make_thread_runner(tagged_dr_data, get_number_of_threads());
    run_work(&thread_runner, datum_work);
    free_thread_runner(&thread_runner);
    return 0;
}

static const char* get_track_filename(DB_playItem_t* item)
{
    ddb_api->pl_lock();
    const char* path = ddb_api->pl_find_meta_raw(item, ":URI");
    ddb_api->pl_unlock();
    return get_filename(path);
}

static unsigned sprint_filename(DB_playItem_t* item, char* begin)
{
    char* end = begin;
    const char* filename = get_track_filename(item);
    end += sprintf(end, " %.83s", filename);
    return end - begin;
}

static unsigned sprint_tagged_track_info(DB_playItem_t* item, int track_number, char* begin)
{
    char* end = begin;
    ddb_api->pl_lock();
    const char* title = ddb_api->pl_find_meta_raw(item, "title");
    ddb_api->pl_unlock();
    end += sprintf(end, " %02d-%-.80s", track_number, title);
    return end - begin;
}

static unsigned sprint_track_info(track_t* track, char* begin)
{
    DB_playItem_t* item = (DB_playItem_t*)track;
    char* end = begin;
    duration_t duration = make_duration(ddb_api->pl_get_item_duration(item));
    end += sprint_duration(&duration, end);
    int track_number = ddb_api->pl_find_meta_int(item, "track", 0);
    if(track_number) end += sprint_tagged_track_info(item, track_number, end);
    else end += sprint_filename(item, end);
    return end - begin;
}

static unsigned sprint_folder_info(DB_playItem_t* item, char* begin)
{
    char* end = begin;
    ddb_api->pl_lock();
    const char* path = ddb_api->pl_find_meta_raw(item, ":URI");
    ddb_api->pl_unlock();
    char dirname[120];
    sprint_dirname(dirname, path);
    end += sprintf(end, "Analyzed folder: %.120s", dirname);
    return end - begin;
}

static unsigned sprint_album_info(track_t* track, char* begin)
{
    DB_playItem_t* item = (DB_playItem_t*)track;
    char* end = begin;
    ddb_api->pl_lock();
    const char* artist = ddb_api->pl_find_meta_raw(item, "artist");
    const char* album = ddb_api->pl_find_meta_raw(item, "album");
    ddb_api->pl_unlock();
    if(!artist && !album) end += sprint_folder_info(item, end);
    else end += sprintf(end, "Analyzed: %.40s / %.80s", artist, album);
    return end - begin;
}

static unsigned sprint_codec_info(track_t* track, char* begin)
{
    DB_playItem_t* item = (DB_playItem_t*)track;
    char* end = begin;
    DB_fileinfo_t* fileinfo = get_fileinfo(item);
    if(fileinfo)
    {
        end += sprintf(end, "Samplerate:        %d Hz\n", fileinfo->fmt.samplerate);
        end += sprintf(end, "Channels:          %d\n", fileinfo->fmt.channels);
        end += sprintf(end, "Bits per sample:   %d\n", fileinfo->fmt.bps);
        end += sprintf(end, "Decoder:           %s", fileinfo->plugin->plugin.name);
        free_fileinfo(fileinfo);
    }
    return end - begin;
}

static int same_album(track_t* track1, track_t* track2)
{
    DB_playItem_t* item1 = (DB_playItem_t*) track1;
    DB_playItem_t* item2 = (DB_playItem_t*) track2;
    ddb_api->pl_lock();
    const char* artist1 = ddb_api->pl_find_meta_raw(item1, "artist");
    const char* album1 = ddb_api->pl_find_meta_raw(item1, "album");
    const char* artist2 = ddb_api->pl_find_meta_raw(item2, "artist");
    const char* album2 = ddb_api->pl_find_meta_raw(item2, "album");
    ddb_api->pl_unlock();
    int found_all = artist1 && album1 && artist2 && album2;
    return found_all && !(strcmp(artist1, artist2) || strcmp(album1, album2));
}

unsigned sprint_dr_log_impl(const tagged_dr_data_t* tagged_dr_data, char* buffer)
{
    char dr_format[32];
    ddb_api->conf_get_str("dr_meter.format", DEFAULT_DR_FORMAT, dr_format, sizeof(dr_format));
    dr_log_printer_t log_printer = {
        .dr_format = dr_format,
        .line_length = 80,
        .sprint_track_info = sprint_track_info,
        .sprint_album_info = sprint_album_info,
        .sprint_codec_info = sprint_codec_info,
        .same_album = same_album,
    };
    return sprint_log_dr_log_printer(&log_printer, tagged_dr_data, buffer);
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
    "property \"(DR, Peak, RMS) printf formatting\" entry dr_meter.format \"" DEFAULT_DR_FORMAT "\";\n"
    "property \"Number of threads\" entry dr_meter.threads 4;\n"
;

DB_plugin_t* ddb_dr_meter_load(DB_functions_t* api)
{
    ddb_api = api;
    static dr_meter_plugin_t dr_meter_plugin = {
        .info.plugin.type            = DB_PLUGIN_MISC,
        .info.plugin.api_vmajor      = 1,
        .info.plugin.api_vminor      = 10,
        .info.plugin.version_major   = 1,
        .info.plugin.version_minor   = 1,
        .info.plugin.id              = "dr_meter",
        .info.plugin.name            = "Dynamic Range Meter",
        .info.plugin.descr           = "Prints Dynamic Range log",
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
