#include <stdlib.h>
#include <time.h>
#include <gtk/gtk.h>

#include <deadbeef/deadbeef.h>
#include <deadbeef/gtkui_api.h>

#include "dr_meter_plugin_gui.h"
#include "dr_meter_plugin.h"
#include "tagged_dr_data.h"
#include "selection.h"
#include "dr_display_data.h"

DB_functions_t* ddb_api;
dr_meter_plugin_t* dr_meter_plugin;
ddb_gtkui_t* gtk_ui_plugin;

int dr_meter_gui_start()
{
    return 0;
}

static void unreference_selection(selection_t* selection)
{
    for(int k = 0; k < selection->items_count ; ++k)
        ddb_api->pl_item_unref(selection->items[k]);
    free(selection->items);
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

static GdkWindowTypeHint get_window_hint()
{
    int focus_dialogue = ddb_api->conf_get_int("dr_meter_gui.focus_dialogue", 0);
    return focus_dialogue ? GDK_WINDOW_TYPE_HINT_DIALOG : GDK_WINDOW_TYPE_HINT_UTILITY;
}

static int show_dr_dialog(void* display_data)
{
    show_dr_dialog_dr_display((dr_display_data_t*)display_data);
    return 0;
}

static void display_dr_results(sprint_dr_log_t sprint_dr_log, const tagged_dr_data_t* tagged_dr_data)
{
    dr_display_data_t* display_data = create_dr_display_data(GTK_WINDOW(gtk_ui_plugin->get_mainwin()), get_window_hint(), tagged_dr_data->items);
    if(display_data)
    {
        fill_log_dr_display(display_data, sprint_dr_log, tagged_dr_data);
        g_idle_add(show_dr_dialog, display_data);
    }
}

static void* meter_job(void* data)
{
    int context = (intptr_t)data;
    if(context == DDB_ACTION_CTX_SELECTION)
    {
        selection_t selection;
        retrieve_current_selection(&selection);
        tagged_dr_data_t tagged_dr_data = make_tagged_dr_data(&selection);
        struct timespec before;
        clock_gettime(CLOCK_MONOTONIC, &before);
        dr_meter_plugin->compute_dr(&tagged_dr_data);
        struct timespec after;
        clock_gettime(CLOCK_MONOTONIC, &after);
        struct timespec duration = {
            .tv_sec = after.tv_sec - before.tv_sec,
            .tv_nsec = after.tv_nsec - before.tv_nsec,
        };
        fprintf(stderr, "compute_dr ran in %f seconds\n", duration.tv_sec + 1e-9*duration.tv_nsec);
        display_dr_results(dr_meter_plugin->sprint_dr_log, &tagged_dr_data);
        free_tagged_dr_data_members(&tagged_dr_data);
        unreference_selection(&selection);
    }
    return NULL;
}

static int run_meter(DB_plugin_action_t* self, ddb_action_context_t context)
{
    (void) self;
    pthread_t pid;
    pthread_create(&pid, NULL, meter_job, (void*)(intptr_t)context);
    pthread_detach(pid);
    return 0;
}

DB_plugin_action_t* dr_meter_gui_get_actions(DB_playItem_t* unused)
{
    (void) unused;
    static DB_plugin_action_t dr_meter_action = {
        .title = "Dynamic Range",
        .name = "compute_dynamic_range",
        .flags = DB_ACTION_MULTIPLE_TRACKS | DB_ACTION_SINGLE_TRACK | DB_ACTION_ADD_MENU,
        .callback2 = run_meter,
        .next = NULL
    };
    return &dr_meter_action;
}

int dr_meter_gui_connect()
{
    gtk_ui_plugin =(ddb_gtkui_t*)ddb_api->plug_get_for_id(DDB_GTKUI_PLUGIN_ID);
    dr_meter_plugin = (dr_meter_plugin_t*)ddb_api->plug_get_for_id ("dr_meter");
    if(!gtk_ui_plugin) return -1;
    else if(!dr_meter_plugin)
    {
        fprintf(stderr, "dr_meter_gui: main dr_meter plugin not found\n");
        return -1;
    }
    else return 0;
}

static const char settings_dialog[] =
    "property \"Suggest desktop focus DR dialogue\" checkbox dr_meter_gui.focus_dialogue 0;\n"
;


#if GTK_CHECK_VERSION(3,0,0)
#define DR_METER_GUI_PLUGIN_ID "dynamic-range-meter-gtk3"
DB_plugin_t* ddb_dr_meter_gtk3_load(DB_functions_t* api)
#else
#define DR_METER_GUI_PLUGIN_ID "dynamic-range-meter-gtk2"
DB_plugin_t* ddb_dr_meter_gtk2_load(DB_functions_t* api)
#endif
{
    ddb_api = api;
    static DB_misc_t dr_plugin = {
        .plugin.type            = DB_PLUGIN_MISC,
        .plugin.api_vmajor      = 1,
        .plugin.api_vminor      = 10,
        .plugin.version_major   = 1,
        .plugin.version_minor   = 2,
        .plugin.id              = DR_METER_GUI_PLUGIN_ID,
        .plugin.name            = "Dynamic Range Meter GUI",
        .plugin.descr           = "Shows Dynamic Range log",
        .plugin.copyright       =
        "Copyright (C) 2021 Valérian Sibille <mrdakeryas@gmail.com>\n"
        ,
        .plugin.website         = "https://github.com/dakeryas/deadbeef-dr-meter.git",
        .plugin.start           = dr_meter_gui_start,
        .plugin.connect         = dr_meter_gui_connect,
        .plugin.get_actions     = dr_meter_gui_get_actions,
        .plugin.configdialog    = settings_dialog,
    };
    return &dr_plugin.plugin;
}
