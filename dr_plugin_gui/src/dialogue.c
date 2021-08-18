#include <gtk/gtk.h>
#include <glib.h>
#include <stdlib.h>
#include "dialogue.h"
#include "dr_display_data.h"
#include "save_button.h"

static void free_display_data(GtkWindow*, gpointer data)
{
    dr_display_data_t* display_data = (dr_display_data_t*)data;
    free_dr_display_data(display_data);
    free(display_data);
}

static GtkWindow* create_dr_dialog(GtkWindow* parent)
{
    GtkWindow* dialog = GTK_WINDOW(gtk_dialog_new());
    gtk_widget_set_size_request(GTK_WIDGET(dialog), 600, 420);
    gtk_window_set_title(dialog, "Dynamic Range");
    gtk_window_set_position(dialog, GTK_WIN_POS_MOUSE);
    gtk_window_set_modal(dialog, TRUE);
    gtk_window_set_destroy_with_parent((dialog), TRUE);
    gtk_window_set_type_hint(dialog, GDK_WINDOW_TYPE_HINT_DIALOG);
    gtk_window_set_transient_for(dialog, parent);
    return dialog;
}

static GtkLabel* create_selectable_label(const char* log_buffer)
{
    GtkLabel* label = GTK_LABEL(gtk_label_new(log_buffer));
    gtk_label_set_selectable(label, TRUE);
    return label;
}

int show_dr_dialog(dr_display_data_t* display_data, GtkWindow* main_window)
{
    display_data->dr_dialog = create_dr_dialog(main_window);
    g_signal_connect(display_data->dr_dialog, "destroy", G_CALLBACK(free_display_data), display_data);
    GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(display_data->dr_dialog));
    GtkLabel* log_label = create_selectable_label(display_data->log);
    gtk_container_add(GTK_CONTAINER(content_area), GTK_WIDGET(log_label));
    GtkWidget* save_button = create_save_button(display_data);
    gtk_container_add(GTK_CONTAINER(content_area), save_button);
    gtk_widget_show_all(GTK_WIDGET(display_data->dr_dialog));
    return 0;
}