/**************************************************************************
* @file dr_display_data.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 17 Aug 2021
* @brief Struct to save the log data for the "save log" callback
* ************************************************************************/

#ifndef DR_PLUGIN_GUI_DR_DISPLAY_DATA_H
#define DR_PLUGIN_GUI_DR_DISPLAY_DATA_H

#include <gdk/gdk.h>
#include "sprint_dr_log.h"

struct dr_display_data_s
{
    unsigned items;
    char* log;
    unsigned log_length;
    GtkDialog* dialog;
};

typedef struct dr_display_data_s dr_display_data_t;

dr_display_data_t* create_dr_display_data(GtkWindow* parent, GdkWindowTypeHint window_hint, unsigned selected_items);
void fill_log_dr_display(dr_display_data_t* self, sprint_dr_log_t sprint_dr_log, const struct tagged_dr_data_s* tagged_dr_data);
void show_dr_dialog_dr_display(dr_display_data_t* self);
void free_dr_display_data_members(dr_display_data_t* self);
void free_dr_display_data(dr_display_data_t* self);

#endif /* DR_PLUGIN_GUI_DR_DISPLAY_DATA_H */
