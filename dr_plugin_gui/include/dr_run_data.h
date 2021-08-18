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

dr_run_data_t* create_dr_run_data(unsigned selected_items);
void free_dr_run_data(dr_run_data_t* self);

#endif /* DR_METER_DR_RUN_DATA_H */
