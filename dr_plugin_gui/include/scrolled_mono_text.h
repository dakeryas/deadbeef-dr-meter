/**************************************************************************
* @file scrolled_mono_text.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 24 Nov 2021
* @brief
* ************************************************************************/

#ifndef DR_PLUGIN_GUI_SCROLLED_MONO_TEXT_H
#define DR_PLUGIN_GUI_SCROLLED_MONO_TEXT_H

struct _GtkDialog;
struct _GtkScrolledWindow;

struct _GtkScrolledWindow* add_scrolled_mono_text(struct _GtkDialog* dialog, char* text, unsigned text_length);

#endif /* DR_PLUGIN_GUI_SCROLLED_MONO_TEXT_H */
