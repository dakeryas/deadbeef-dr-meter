#include <stdlib.h>
#include <math.h>
#include <gtk/gtk.h>
#include "dr_display_data.h"
#include "mono_text.h"
#include "save_button.h"

static unsigned get_header_footer_size(unsigned selected_items)
{
    const unsigned header_length = 135 + 48 + 5 * 80;
    const unsigned footer_length = 21 + 23;
    const unsigned number_of_albums = selected_items;//would need to check tags otherwise
    return (header_length + footer_length) * number_of_albums;
}

static unsigned get_log_size(unsigned selected_items)
{
    const unsigned item_length = 40 + 5 + 5 + 1 + 3 + 80 + 2;//DR info, space, duration, space, track number, title, newline
    return selected_items * item_length + get_header_footer_size(selected_items);
}

static void update_log_size(dr_display_data_t* self, unsigned selected_items)
{
    free(self->log);
    self->log_length = 0;
    self->log = malloc(get_log_size(selected_items));
}

static void init_log(dr_display_data_t* self, unsigned selected_items)
{
    self->log = NULL;
    update_log_size(self, selected_items);
}

static void free_display_data_cb(GtkDialog* unused, gpointer data)
{
    (void) unused;
    dr_display_data_t* display_data = (dr_display_data_t*)data;
    free_dr_display_data(display_data);
}

static void create_dr_dialog(dr_display_data_t* self, GtkWindow* parent, GdkWindowTypeHint window_hint)
{
    self->dialog = GTK_DIALOG(gtk_dialog_new_with_buttons("Dynamic Range", parent, GTK_DIALOG_DESTROY_WITH_PARENT, NULL, NULL));
    gtk_widget_set_size_request(GTK_WIDGET(self->dialog), 600, 420);
    gtk_container_set_border_width(GTK_CONTAINER(self->dialog), 5);
    gtk_window_set_position(GTK_WINDOW(self->dialog), GTK_WIN_POS_MOUSE);
    gtk_window_set_type_hint(GTK_WINDOW(self->dialog), window_hint);
    gtk_window_set_transient_for(GTK_WINDOW(self->dialog), parent);
}

dr_display_data_t* create_dr_display_data(GtkWindow* parent, GdkWindowTypeHint window_hint, unsigned selected_items)
{
    dr_display_data_t* self = malloc(sizeof(*self));
    init_log(self, selected_items);
    create_dr_dialog(self, parent, window_hint);
    g_signal_connect(self->dialog, "destroy", G_CALLBACK(free_display_data_cb), self);
    if(!self->log)
    {
        fprintf(stderr, "Failed allocating DR log buffer\n");
        free_dr_display_data(self);
        self = NULL;
    }
    return self;
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

void show_dr_dialog(dr_display_data_t* self)
{
    add_scrolled_mono_text(self->dialog, self->log, self->log_length);
    add_save_button(self->dialog, self);
    gtk_widget_show_all(GTK_WIDGET(self->dialog));
}

void free_dr_display_data_members(dr_display_data_t* self)
{
    gtk_widget_destroy(GTK_WIDGET(self->dialog));
    free(self->log);
}

void free_dr_display_data(dr_display_data_t* self)
{
    free_dr_display_data_members(self);
    free(self);
}
