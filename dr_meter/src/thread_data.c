#include <stdlib.h>
#include "thread_data.h"
#include "selection.h"

static void update_data_size(thread_data_t* self)
{
    free_thread_data(self);
    self->data = malloc(self->items * sizeof(thread_datum_t));
}

static void copy_data(thread_data_t* self, const selection_t* selection)
{
    self->items = selection->items_count;
    update_data_size(self);
    for(int k = 0; k < selection->items_count ; ++k)
        self->data[k].item = selection->items[k];
}

thread_data_t make_thread_data(const selection_t* selection)
{
    thread_data_t thread_data;
    copy_data(&thread_data, selection);
    return thread_data;
}

thread_data_t* create_thread_data(const selection_t* selection)
{
    thread_data_t* thread_data = malloc(sizeof(*thread_data));
    copy_data(thread_data, selection);
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
