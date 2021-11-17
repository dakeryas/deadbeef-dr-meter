#include <stdlib.h>
#include "tagged_dr_data.h"
#include "selection.h"

static void update_data_size(tagged_dr_data_t* self)
{
    free_tagged_dr_data(self);
    self->data = malloc(self->items * sizeof(tagged_dr_datum_t));
}

static void copy_data(tagged_dr_data_t* self, const selection_t* selection)
{
    self->items = selection->items_count;
    update_data_size(self);
    for(int k = 0; k < selection->items_count ; ++k)
        self->data[k].item = selection->items[k];
}

tagged_dr_data_t make_tagged_dr_data(const selection_t* selection)
{
    tagged_dr_data_t tagged_dr_data;
    copy_data(&tagged_dr_data, selection);
    return tagged_dr_data;
}

tagged_dr_data_t* create_tagged_dr_data(const selection_t* selection)
{
    tagged_dr_data_t* tagged_dr_data = malloc(sizeof(*tagged_dr_data));
    copy_data(tagged_dr_data, selection);
    return tagged_dr_data;
}

dr_stats_t* get_dr_stats(tagged_dr_data_t* self, unsigned item_id)
{
    return &self->data[item_id].dr_stats;
}

void free_tagged_dr_data(tagged_dr_data_t* self)
{
    free(self->data);
}
