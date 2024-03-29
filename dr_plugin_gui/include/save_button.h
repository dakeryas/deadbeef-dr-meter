/**************************************************************************
* @file save_button.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 17 Aug 2021
* @brief Define a "Save" button and connect it to a callback relying on
* the dialog and log of display_data
* ************************************************************************/

#ifndef DR_PLUGIN_GUI_SAVE_BUTTON_H
#define DR_PLUGIN_GUI_SAVE_BUTTON_H

struct dr_display_data_s;
struct GtkWidget;

GtkWidget* add_save_button(struct dr_display_data_s* display_data, const char* button_label);

#endif /* DR_PLUGIN_GUI_SAVE_BUTTON_H */
