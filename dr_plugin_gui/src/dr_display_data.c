#include <stdlib.h>
#include <gtk/gtk.h>
#include "dr_display_data.h"

static unsigned get_log_size(unsigned selected_items)
{
    const unsigned item_length = 40 + 5 + 5 + 1 + 3 + 80 + 2;//DR info, space, duration, space, track number, title, newline
    return 135 + 48 + 5 * 80 + selected_items * item_length + 21 + 23;
}

static unsigned allocate_log(dr_display_data_t** display_data, unsigned selected_items)
{
    unsigned log_size = get_log_size(selected_items);
    (*display_data)->log = malloc(log_size);
    if((*display_data)->log) return log_size;
    else return 0;
}

dr_display_data_t* create_dr_display_data(unsigned selected_items)
{
    dr_display_data_t* display_data = malloc(sizeof(*display_data));
    unsigned log_size = allocate_log(&display_data, selected_items);
    if(!log_size)
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
