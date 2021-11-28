/**************************************************************************
* @file dr_meter_plugin.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 05 Jun 2020
* @brief Plugin has generic DeaDBeeF plugin info and compute_dr 'method'
* ************************************************************************/

#ifndef DR_PLUGIN_DR_METER_PLUGIN_H
#define DR_PLUGIN_DR_METER_PLUGIN_H

#include <deadbeef/deadbeef.h>
#include "compute_dr.h"
#include "sprint_dr_log.h"

struct dr_meter_plugin_s
{
    DB_misc_t info;
    compute_dr_t compute_dr;
    sprint_dr_log_t sprint_dr_log;
};

typedef struct dr_meter_plugin_s dr_meter_plugin_t;

#endif /* DR_PLUGIN_DR_METER_PLUGIN_H */
