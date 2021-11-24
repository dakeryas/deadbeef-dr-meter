#include <gtk/gtk.h>
#include <glib.h>
#include <stdlib.h>
#include "dialogue.h"
#include "dr_display_data.h"
#include "mono_text.h"
#include "save_button.h"

static void free_display_data(GtkDialog* unused, gpointer data)
{
    (void) unused;
    dr_display_data_t* display_data = (dr_display_data_t*)data;
    free_dr_display_data(display_data);
    free(display_data);
}

static GtkDialog* create_dr_dialog(GtkWindow* parent, GdkWindowTypeHint window_hint)
{
    GtkDialog* dialog = GTK_DIALOG(gtk_dialog_new_with_buttons("Dynamic Range", parent, GTK_DIALOG_DESTROY_WITH_PARENT, NULL, NULL));
    gtk_widget_set_size_request(GTK_WIDGET(dialog), 600, 420);
    gtk_container_set_border_width(GTK_CONTAINER(dialog), 5);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_MOUSE);
    gtk_window_set_type_hint(GTK_WINDOW(dialog), window_hint);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), parent);
    return dialog;
}

static GtkScrolledWindow* add_scrolled_mono_text(GtkDialog* dialog, char* text, unsigned text_length)
{
    GtkScrolledWindow* scrolled_window = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new(NULL, NULL));
    GtkTextView* text_view = create_mono_text(text, text_length);
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window), -1, 370);
    gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(text_view));
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(dialog)), GTK_WIDGET(scrolled_window));
    return scrolled_window;
}

int show_dr_dialog(dr_display_data_t* display_data, GtkWindow* main_window)
{
    display_data->dr_dialog = create_dr_dialog(main_window, display_data->window_hint);
    g_signal_connect(display_data->dr_dialog, "destroy", G_CALLBACK(free_display_data), display_data);
    add_scrolled_mono_text(display_data->dr_dialog, display_data->log, display_data->log_length);
    add_save_button(display_data->dr_dialog, display_data);
    gtk_widget_show_all(GTK_WIDGET(display_data->dr_dialog));
    return 0;
}
