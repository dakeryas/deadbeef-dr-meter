/**************************************************************************
* @file dr_meter_plugin.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 05 Jun 2020
* @brief Plugin has generic DeaDBeeF plugin info and compute_dr 'method'
* ************************************************************************/

#ifndef DR_PLUGIN_DR_METER_PLUGIN_H
#define DR_PLUGIN_DR_METER_PLUGIN_H

#include <deadbeef/deadbeef.h>

struct tagged_dr_data_s;

struct dr_meter_plugin_s
{
    DB_misc_t info;
    int (*compute_dr)(struct tagged_dr_data_s* tagged_dr_data);
    unsigned (*sprint_dr_log)(const struct tagged_dr_data_s* tagged_dr_data, char* buffer);
};

typedef struct dr_meter_plugin_s dr_meter_plugin_t;

#endif /* DR_PLUGIN_DR_METER_PLUGIN_H */
