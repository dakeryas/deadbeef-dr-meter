#include <stdlib.h>
#include <math.h>
#include <gtk/gtk.h>
#include "dr_display_data.h"

static unsigned get_log_size(unsigned selected_items)
{
    const unsigned item_length = 40 + 5 + 5 + 1 + 3 + 80 + 2;//DR info, space, duration, space, track number, title, newline
    const unsigned header_length = 135 + 48 + 5 * 80;
    const unsigned footer_length = 21 + 23;
    return selected_items * item_length + lrint((header_length + footer_length) * selected_items / 8.) ;
}

static void update_log_size(dr_display_data_t* self, unsigned selected_items)
{
    free(self->log);
    self->log = malloc(get_log_size(selected_items));
}

dr_display_data_t* create_dr_display_data(unsigned selected_items, GdkWindowTypeHint window_hint)
{
    dr_display_data_t* display_data = malloc(sizeof(*display_data));
    display_data->window_hint = window_hint;
    display_data->log = NULL;
    update_log_size(display_data, selected_items);
    if(!display_data->log)
    {
        fprintf(stderr, "Failed allocating DR log buffer\n");
        free(display_data);
        display_data = NULL;
    }
    return display_data;
}

void free_dr_display_data(dr_display_data_t* self)
{
    free(self->log);
}
