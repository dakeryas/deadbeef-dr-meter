/**************************************************************************
* @file sprint_dr_log.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 28 Nov 2021
* @brief Type definition for function printing DR log to buffer from
* tagged DR data
* ************************************************************************/

#ifndef DR_PLUGIN_SPRINT_DR_LOG_H
#define DR_PLUGIN_SPRINT_DR_LOG_H

struct tagged_dr_data_s;

typedef unsigned (*sprint_dr_log_t)(const struct tagged_dr_data_s*, char*);

#endif /* DR_PLUGIN_SPRINT_DR_LOG_H */
