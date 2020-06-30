#include <stdlib.h>
#include "thread_data.h"
#include "selection.h"

static void init_thread_data(thread_data_t* self)
{
    self->data = malloc(self->items * sizeof(thread_datum_t));
}

thread_data_t make_thread_data(selection_t* selection)
{
    thread_data_t thread_data = {.items = selection->items_count};
    init_thread_data(&thread_data);
    for(int k = 0; k < selection->items_count ; ++k)
        thread_data.data[k].item = selection->items[k];
    return thread_data;
}

dr_stats_t* get_dr_stats(thread_data_t* self, unsigned item_id)
{
    return &self->data[item_id].dr_stats;
}

void free_thread_data(thread_data_t* self)
{
    free(self->data);
}
