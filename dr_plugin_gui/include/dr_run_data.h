/**************************************************************************
* @file dr_run_data.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 17 Aug 2021
* @brief Struct to save the log data for the "save log" callback
* ************************************************************************/

#ifndef DR_RUN_DATA_H
#define DR_RUN_DATA_H

struct GtkWindow;

struct dr_run_data_s
{
    int ddb_context;
    GtkWindow* dr_dialog;
    char* log;
};

typedef struct dr_run_data_s dr_run_data_t;

#endif /* DR_RUN_DATA_H */
