#include <gtk/gtk.h>
#include <glib.h>
#include <stdlib.h>
#include "dialogue.h"
#include "dr_display_data.h"
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
    gtk_container_set_border_width(GTK_CONTAINER(dialog), 6);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_MOUSE);
    gtk_window_set_type_hint(GTK_WINDOW(dialog), window_hint);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), parent);
    return dialog;
}

static void set_monospace(GtkTextView* text)
{
#if GTK_CHECK_VERSION(3,0,0)
    GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(text));
    GtkCssProvider *provider = gtk_css_provider_new();
    char css[] = "textview {font-family: \"monospace\";}";
    gtk_css_provider_load_from_data(provider, css, sizeof(css), NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
#else
    PangoFontDescription *descr = pango_font_description_from_string("monospace");
    gtk_widget_modify_font(GTK_WIDGET(text), descr);
    pango_font_description_free(descr);
#endif
}

static GtkTextView* create_mono_text(char* log_buffer, unsigned log_length)
{
    GtkTextView* text = GTK_TEXT_VIEW(gtk_text_view_new());
    gtk_text_buffer_set_text(gtk_text_view_get_buffer(text), log_buffer, log_length);
    set_monospace(text);
    gtk_text_view_set_editable(text, FALSE);
    return text;
}

int show_dr_dialog(dr_display_data_t* display_data, GtkWindow* main_window)
{
    display_data->dr_dialog = create_dr_dialog(main_window, display_data->window_hint);
    g_signal_connect(display_data->dr_dialog, "destroy", G_CALLBACK(free_display_data), display_data);
    GtkTextView* log_text = create_mono_text(display_data->log, display_data->log_length);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(display_data->dr_dialog)), GTK_WIDGET(log_text)));
    add_save_button(display_data->dr_dialog, display_data);
    gtk_widget_show_all(GTK_WIDGET(display_data->dr_dialog));
    return 0;
}
