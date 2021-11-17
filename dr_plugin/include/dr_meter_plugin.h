/**************************************************************************
* @file dr_meter_plugin.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 05 Jun 2020
* @brief Plugin has generic DeaDBeeF plugin info and compute_dr 'method'
* ************************************************************************/

#ifndef DR_METER_PLUGIN_H
#define DR_METER_PLUGIN_H

#include <deadbeef/deadbeef.h>

struct thread_data_s;

typedef struct
{
    DB_misc_t info;
    int (*compute_dr)(struct thread_data_s* thread_data, unsigned max_threads);
    unsigned (*sprint_dr_log)(const struct thread_data_s* thread_data, char* buffer);
} dr_meter_plugin_t;

#endif /* DR_METER_PLUGIN_H */
