/**************************************************************************
* @file dr_display_data.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 17 Aug 2021
* @brief Struct to save the log data for the "save log" callback
* ************************************************************************/

#ifndef DR_METER_GUI_DR_DISPLAY_DATA_H
#define DR_METER_GUI_DR_DISPLAY_DATA_H

#include <gdk/gdk.h>

struct dr_display_data_s
{
    unsigned items;
    char* log;
    unsigned log_length;
    GtkDialog* dialog;
};

typedef struct dr_display_data_s dr_display_data_t;

dr_display_data_t* create_dr_display_data(GtkWindow* parent, GdkWindowTypeHint window_hint, unsigned selected_items);
void show_dr_dialog(dr_display_data_t* self);
void free_dr_display_data_members(dr_display_data_t* self);
void free_dr_display_data(dr_display_data_t* self);

#endif /* DR_METER_GUI_DR_DISPLAY_DATA_H */
