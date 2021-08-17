/**************************************************************************
* @file dialogue.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 17 Aug 2021
* @brief Build the DR dialogue to display the DR log and save it to disk
* ************************************************************************/

#ifndef DR_METER_DIALOGUE_H
#define DR_METER_DIALOGUE_H

struct GtkWindow;
struct GtkLabel;
struct dr_run_data_s;

int show_dr_dialog(struct dr_run_data_s* run_data, GtkWindow* main_window);

#endif /* DR_METER_DIALOGUE_H */
