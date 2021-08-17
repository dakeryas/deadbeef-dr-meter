/**************************************************************************
* @file save_button.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 17 Aug 2021
* @brief Define a "Save" button and connect it to a callback relying on
* the dialog and log of run_data
* ************************************************************************/

#ifndef DR_METER_SAVE_BUTTON_H
#define DR_METER_SAVE_BUTTON_H

struct dr_run_data_s;
struct GtkWidget;

GtkWidget* create_save_button(struct dr_run_data_s* run_data);

#endif /* DR_METER_SAVE_BUTTON_H */
