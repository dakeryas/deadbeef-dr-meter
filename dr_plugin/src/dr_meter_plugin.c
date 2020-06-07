#include <stdlib.h>
#include <unistd.h>

#include "dr_meter_plugin.h"
#include "block_analyser.h"
#include "dr_meter.h"
#include "thread_data.h"
#include "thread_runner.h"
#include "selection.h"

// constants according to DR standard
static const unsigned DR_BLOCK_DURATION = 3;

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
    if(decoded_bytes ==  buffer_size)
        analyse_block(analyser, buffer, buffer_size, fileinfo->fmt.channels, fileinfo->fmt.bps, fileinfo->fmt.samplerate);
    //else end of file
}

static unsigned number_of_blocks(DB_playItem_t* item)
{
    return  ddb_api->pl_get_item_duration(item) / DR_BLOCK_DURATION;
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

int dr_meter_start()
{
    return 0;
}

int dr_meter_stop()
{
    return 0;
}

DB_plugin_t* ddb_dr_meter_load(DB_functions_t* api)
{
    ddb_api = api;
    static dr_meter_plugin_t dr_meter_plugin = {
        .info.plugin.type            = DB_PLUGIN_MISC,
        .info.plugin.api_vmajor      = 1,
        .info.plugin.api_vminor      = 10,
        .info.plugin.version_major   = 0,
        .info.plugin.version_minor   = 1,
        .info.plugin.id              = "dr_meter",
        .info.plugin.name            = "Dynamic Range Meter",
        .info.plugin.descr           = "Measure Dynamic Range",
        .info.plugin.copyright       =
        "Copyright (C) 2020 Valérian Sibille <vsibille@mit.edu>\n"
        ,
        .info.plugin.website         = "https://github.com/dakeryas/deadbeef-dr-meter.git",
        .info.plugin.start           = dr_meter_start,
        .info.plugin.stop            = dr_meter_stop,
        .compute_dr                  = compute_dr_impl,
    };
    return (DB_plugin_t*)&dr_meter_plugin;
}
