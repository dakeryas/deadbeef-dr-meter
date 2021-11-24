/**************************************************************************
* @file mono_text.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 23 Nov 2021
* @brief Create text view with monospace font from text buffer
* ************************************************************************/

#ifndef DR_PLUGIN_GUI_MONO_TEXT_H
#define DR_PLUGIN_GUI_MONO_TEXT_H

struct _GtkTextView;

struct _GtkTextView* create_mono_text(char* log_buffer, unsigned log_length);

#endif /* DR_PLUGIN_GUI_MONO_TEXT_H */
