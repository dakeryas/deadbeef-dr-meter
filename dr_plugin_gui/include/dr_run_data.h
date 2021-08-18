/**************************************************************************
* @file dr_run_data.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 17 Aug 2021
* @brief Struct to save the log data for the "save log" callback
* ************************************************************************/

#ifndef DR_METER_DR_RUN_DATA_H
#define DR_METER_DR_RUN_DATA_H

struct GtkWindow;

struct dr_run_data_s
{
    GtkWindow* dr_dialog;
    char* log;
};

typedef struct dr_run_data_s dr_run_data_t;

void free_dr_run_data(struct dr_run_data_s* self);

#endif /* DR_METER_DR_RUN_DATA_H */
