#include <gtk/gtk.h>
#include <glib.h>

#include <deadbeef/deadbeef.h>
#include <deadbeef/gtkui_api.h>

#include "dr_meter_plugin_gui.h"
#include "dr_meter_plugin.h"
#include "thread_data.h"
#include "selection.h"

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

static GtkWindow* create_dr_dialog()
{
    GtkWindow* dialog = GTK_WINDOW(gtk_dialog_new());
    gtk_widget_set_size_request(GTK_WIDGET(dialog), 600, 400);
    gtk_window_set_title(dialog, "Dynamic Range");
    gtk_window_set_position(dialog, GTK_WIN_POS_MOUSE);
    gtk_window_set_modal(dialog, TRUE);
    gtk_window_set_destroy_with_parent((dialog), TRUE);
    gtk_window_set_type_hint(dialog, GDK_WINDOW_TYPE_HINT_DIALOG);
    return dialog;
}

static GtkLabel* create_selectable_label(const char* log_buffer)
{
    GtkLabel* label = GTK_LABEL(gtk_label_new(log_buffer));
    gtk_label_set_selectable(label, TRUE);
    return label;
}

static int show_dr_dialog(const char* log_buffer)
{
    GtkWindow* main_window = GTK_WINDOW(gtk_ui_plugin->get_mainwin());
    GtkWindow* dialog = create_dr_dialog();
    gtk_window_set_transient_for(dialog, main_window);

    GtkLabel* log_label = create_selectable_label(log_buffer);
    GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_add(GTK_CONTAINER(content_area), GTK_WIDGET(log_label));

    gtk_widget_show_all(GTK_WIDGET(dialog));
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

static gboolean run_meter_job(void* data)
{
    int context =(intptr_t)data;
    if(context == DDB_ACTION_CTX_SELECTION)
    {
        selection_t selection;
        retrieve_current_selection(&selection);
        thread_data_t thread_data = make_thread_data(&selection);
        dr_meter_plugin->compute_dr(&thread_data);
        const unsigned item_length = 40 + 5 + 5 + 1 + 3 + 80 + 2;//DR info, space, duration, space, track number, title, newline
        char buffer[135 + 48 + 5 * 80 + thread_data.items * item_length + 21 + 23];
        dr_meter_plugin->sprint_dr_log(&thread_data, buffer);
        show_dr_dialog(buffer);
        free_thread_data(&thread_data);
        unreference_selection(&selection);
    }
    return FALSE;
}

static int run_meter(DB_plugin_action_t* action, ddb_action_context_t context)
{
    (void) action;
    gdk_threads_add_idle(run_meter_job, (void *)(intptr_t)context);
    return 0;
}

DB_plugin_action_t* dr_meter_gui_get_actions(DB_playItem_t* item)
{
    (void) item;
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
    dr_meter_plugin = (dr_meter_plugin_t *)ddb_api->plug_get_for_id ("dr_meter");
    if(!gtk_ui_plugin) return -1;
    else if(!dr_meter_plugin)
    {
        fprintf(stderr, "dr_meter_gui: main dr_meter plugin not found\n");
        return -1;
    }
    else return 0;
}

#if GTK_CHECK_VERSION(3,0,0)
DB_plugin_t* ddb_dr_meter_gtk3_load(DB_functions_t* api)
#else
DB_plugin_t* ddb_dr_meter_gtk2_load(DB_functions_t* api)
#endif
{
    ddb_api = api;
    static DB_misc_t dr_plugin = {
        .plugin.type            = DB_PLUGIN_MISC,
        .plugin.api_vmajor      = 1,
        .plugin.api_vminor      = 10,
        .plugin.version_major   = 0,
        .plugin.version_minor   = 1,
#if GTK_CHECK_VERSION(3,0,0)
        .plugin.name            = "Dynamic Range Meter GTK3",
#else
        .plugin.name            = "Dynamic Range Meter GTK2",
#endif
        .plugin.descr           = "Displays Dynamic Range",
        .plugin.copyright       =
        "Copyright (C) 2021 Valérian Sibille <mrdakeryas@gmail.com>\n"
        ,
        .plugin.website         = "https://github.com/dakeryas/deadbeef-dr-meter.git",
        .plugin.start           = dr_meter_gui_start,
        .plugin.connect         = dr_meter_gui_connect,
        .plugin.get_actions     = dr_meter_gui_get_actions,
    };
    return &dr_plugin.plugin;
}
