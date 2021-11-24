/**************************************************************************
* @file dialogue.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 17 Aug 2021
* @brief Build the DR dialogue to display the DR log and save it to disk
* ************************************************************************/

#ifndef DR_PLUGIN_GUI_DIALOGUE_H
#define DR_PLUGIN_GUI_DIALOGUE_H

struct GtkWindow;
struct dr_display_data_s;

int show_dr_dialog(struct dr_display_data_s* display_data, GtkWindow* main_window);

#endif /* DR_PLUGIN_GUI_DIALOGUE_H */
