#include <stdlib.h>
#include "thread_data.h"
#include "thread_datum.h"
#include "selection.h"

static unsigned number_of_batches(selection_t* selection, unsigned max_threads)
{
    unsigned batches = selection->items_count / max_threads;
    if(selection->items_count % max_threads) batches += 1;
    return batches;
}
static void init_thread_data(thread_data_t* thread_data)
{
    thread_data->data = malloc(thread_data->items * sizeof(thread_datum_t));
}

thread_data_t make_thread_data(selection_t* selection, unsigned max_threads)
{
    thread_data_t thread_data =
    {.threads = max_threads, .batches = number_of_batches(selection, max_threads), .items = selection->items_count};
    init_thread_data(&thread_data);
    for(int k = 0; k < selection->items_count ; ++k)
        thread_data.data[k].item = selection->items[k];
    return thread_data;
}

dr_stats_t* get_dr_stats(thread_data_t* thread_data, unsigned item_id)
{
    return &thread_data->data[item_id].dr_stats;
}

void free_thread_data(thread_data_t* thread_data)
{
    free(thread_data->data);
}

