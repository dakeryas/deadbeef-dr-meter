/**************************************************************************
* @file dr_meter_plugin_gui.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 05 Jun 2020
* @brief GUI part of the DR meter plugin
* ************************************************************************/

#ifndef DR_METER_PLUGIN_GUI_H
#define DR_METER_PLUGIN_GUI_H

int dr_meter_gui_start();
struct DB_plugin_action_s* dr_meter_gui_get_actions(struct DB_playItem_s* item);
int dr_meter_gui_connect(); // connect this to DDB GTK and computing dr_meter plugins

#endif /* DR_METER_PLUGIN_GUI_H */
