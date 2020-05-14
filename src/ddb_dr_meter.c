#include <gtk/gtk.h>
#include <glib.h>

#include <deadbeef/deadbeef.h>
#include <deadbeef/gtkui_api.h>

#include <assert.h>
#include <math.h>

#include "block_analyser.h"
#include "dr_meter.h"

// constants according to DR standard
const static float DR_MIN_DB = -100.;
const static float DR_MAX_DB = 0.;
static const unsigned DR_BLOCK_DURATION = 3;

static DB_functions_t* ddb_api;
static ddb_gtkui_t* gtk_ui_plugin;

typedef struct{
    DB_playItem_t **items;
    int items_count;
} selection_t;

typedef struct{
    ddb_gtkui_widget_t* gtk_widget;
} widget_dr_meter_t;

int dr_meter_start()
{
    return 0;
}

static void unreference_selection(selection_t* selection)
{
    for(int k = 0; k < selection->items_count ; ++k)
        ddb_api->pl_item_unref(selection->items[k]);
}

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

static void process_item(DB_playItem_t* item, DB_fileinfo_t* fileinfo, DB_decoder_t* decoder)
{
    decoder->init(fileinfo, item);
    char* buffer = NULL;
    int buffer_size = allocate_buffer(&buffer, DR_BLOCK_DURATION, fileinfo);
    if(buffer_size)
    {
        block_analyser_t analyser = {.channels = fileinfo->fmt.channels};
        init_block_analyser(&analyser);
        dr_meter_t dr_meter = {.min_hist = DR_MIN_DB, .max_hist = DR_MAX_DB, .channels = fileinfo->fmt.channels};
        init_dr_meter(&dr_meter);
        for(unsigned block_index = 0 ; block_index < number_of_blocks(item); ++block_index)
        {
            decode_analyse_block(buffer, buffer_size, fileinfo, decoder, &analyser);
            fill_dr_meter(&dr_meter, &analyser);
        }
    }
    free(buffer);
}

static int compute_single_dr(DB_playItem_t* selection_item)
{
    DB_decoder_t* decoder = get_decoder(selection_item);
    if(decoder)
    {
        DB_fileinfo_t* fileinfo = decoder->open(DDB_DECODER_HINT_RAW_SIGNAL);
        if(fileinfo) process_item(selection_item, fileinfo, decoder);
    }
    return 0;
}

static int compute_dr(selection_t* selection)
{
    for(int k = 0; k < selection->items_count ; ++k)
        compute_single_dr(selection->items[k]);
    return 0;
}

static void copy_selected_from_main_playlist(selection_t* selection)
{
    unsigned n = -1;
    DB_playItem_t* item = ddb_api->pl_get_first(PL_MAIN);
    while(item)
    {
        if(ddb_api->pl_is_selected(item))
        {
            ddb_api->pl_item_ref(item);
            selection->items[++n] = item;
        }
        DB_playItem_t* next = ddb_api->pl_get_next(item, PL_MAIN);
        ddb_api->pl_item_unref(item);
        item = next;
    }
}

static void copy_selected(ddb_playlist_t* playlist, selection_t* selection)
{
    selection->items_count = ddb_api->plt_getselcount(playlist);
    if(selection->items_count > 0)
    {
        selection->items = malloc(sizeof(DB_playItem_t*) * selection->items_count);
        if(selection->items) copy_selected_from_main_playlist(selection);
    }
}

static void retrieve_current_selection(selection_t* selection)
{
    ddb_api->pl_lock();
    ddb_playlist_t* playlist = ddb_api->plt_get_curr();
    if(playlist) copy_selected(playlist, selection);
    ddb_api->pl_unlock();
}

static gpointer compute_dr_callback(void* data)
{
    int context =(intptr_t)data;
    if(context == DDB_ACTION_CTX_SELECTION)
    {
        selection_t current_selection;
        retrieve_current_selection(&current_selection);
        compute_dr(&current_selection);
        unreference_selection(&current_selection);
    }
    return NULL;
}

static int dr_meter_compute(DB_plugin_action_t* action, int context)
{
    g_thread_new("dr_meter", compute_dr_callback,(void *)(intptr_t)context);
    return 0;
}

DB_plugin_action_t* dr_meter_get_actions(DB_playItem_t* item)
{
    static DB_plugin_action_t dr_meter_action = {
        .title = "Dynamic Range",
        .name = "compute_dynamic_range",
        .flags = DB_ACTION_MULTIPLE_TRACKS | DB_ACTION_SINGLE_TRACK | DB_ACTION_ADD_MENU,
        .callback2 = dr_meter_compute,
        .next = NULL
    };
    return &dr_meter_action;
}

int dr_meter_connect()
{
    gtk_ui_plugin =(ddb_gtkui_t*)ddb_api->plug_get_for_id(DDB_GTKUI_PLUGIN_ID);
    if(gtk_ui_plugin) return 0;
    else return -1;
}

DB_plugin_t* ddb_dr_meter_load(DB_functions_t* api)
{
    ddb_api = api;
    static DB_misc_t dr_plugin = {
        .plugin.type            = DB_PLUGIN_MISC,
        .plugin.api_vmajor      = 1,
        .plugin.api_vminor      = 10,
        .plugin.version_major   = 0,
        .plugin.version_minor   = 1,
        .plugin.id              = "dr_meter",
        .plugin.name            = "Dynamic Range Meter",
        .plugin.descr           = "Measure Dynamic Range",
        .plugin.copyright       =
        "Copyright(C)2020 Valérian Sibille <vsibille@mit.edu>\n"
        ,
        .plugin.website         = "",
        .plugin.start           = dr_meter_start,
        .plugin.connect         = dr_meter_connect,
        .plugin.get_actions     = dr_meter_get_actions,
    };
    return &dr_plugin.plugin;
}
