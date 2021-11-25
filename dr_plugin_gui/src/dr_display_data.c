#include <stdlib.h>
#include <math.h>
#include <gtk/gtk.h>
#include "dr_display_data.h"
#include "scrolled_mono_text.h"
#include "save_button.h"

static unsigned get_header_footer_size(unsigned selected_items)
{
    const unsigned header_length = 135 + 48 + 5 * 80;
    const unsigned footer_length = 22 + 24 + 28 + 21 + 22 + 24;
    const unsigned number_of_albums = selected_items;//would need to check tags otherwise
    return (header_length + footer_length) * number_of_albums;
}

static unsigned item_length()
{
    return 40 + 5 + 5 + 1 + 3 + 80 + 2;//DR info, space, duration, space, track number, title, newline
}

static unsigned get_log_size(unsigned selected_items)
{
    return selected_items * item_length() + get_header_footer_size(selected_items);
}

static void update_log_size(dr_display_data_t* self)
{
    free(self->log);
    self->log_length = 0;
    self->log = malloc(get_log_size(self->items));
}

static void init_log(dr_display_data_t* self)
{
    self->log = NULL;
    update_log_size(self);
}

static void free_display_data_cb(GtkDialog* unused, gpointer data)
{
    (void) unused;
    dr_display_data_t* display_data = (dr_display_data_t*)data;
    free_dr_display_data(display_data);
}

static void set_dr_dialog_properties(GtkWindow* dialog, GtkWindow* parent, GdkWindowTypeHint window_hint)
{
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_MOUSE);
    gtk_window_set_type_hint(GTK_WINDOW(dialog), window_hint);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), parent);
}

static void create_dr_dialog(dr_display_data_t* self, GtkWindow* parent, GdkWindowTypeHint window_hint)
{
    self->dialog = GTK_DIALOG(gtk_dialog_new_with_buttons("Dynamic Range", parent, GTK_DIALOG_DESTROY_WITH_PARENT, NULL, NULL));
    set_dr_dialog_properties(GTK_WINDOW(self->dialog), parent, window_hint);
    add_save_button(self, "Save DR log");
    g_signal_connect(self->dialog, "destroy", G_CALLBACK(free_display_data_cb), self);
}

dr_display_data_t* create_dr_display_data(GtkWindow* parent, GdkWindowTypeHint window_hint, unsigned selected_items)
{
    dr_display_data_t* self = malloc(sizeof(*self));
    self->items = selected_items;
    init_log(self);
    create_dr_dialog(self, parent, window_hint);
    if(!self->log)
    {
        fprintf(stderr, "Failed allocating DR log buffer\n");
        free_dr_display_data(self);
        self = NULL;
    }
    return self;
}

static unsigned guess_needed_dr_dialog_height(dr_display_data_t* self)
{
    unsigned number_of_albums = self->items < 18 ? 1 : lround(self->items / 10.);
    return 287 * number_of_albums + 15 * self->items;
}

static void adapt_dialog_height(dr_display_data_t* self)
{
    unsigned height = guess_needed_dr_dialog_height(self);
    const unsigned max_height = 900;
    if(height > max_height) height = max_height;
    gtk_window_set_default_size(GTK_WINDOW(self->dialog), 600, height);
}

void show_dr_dialog(dr_display_data_t* self)
{
    add_scrolled_mono_text(self->dialog, self->log, self->log_length);
    adapt_dialog_height(self);
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
