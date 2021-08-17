#include <stdlib.h>
#include <gtk/gtk.h>
#include "dr_run_data.h"

void free_dr_run_data(dr_run_data_t* self)
{
    free(self->log);
}
