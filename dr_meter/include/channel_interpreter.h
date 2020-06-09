/**************************************************************************
* @file channel_interpreter.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 09 Jun 2020
* @brief Turn single-channel single-sample audio bytes into floating
* point between -1 and 1 depending on guessed-format according to
* provided sample size (in bytes).
* ************************************************************************/

#ifndef CHANNEL_INTERPRETER_H
#define CHANNEL_INTERPRETER_H

#include <stdint.h>

int32_t get_int_channel_value(char* channel_begin, int bytes_per_channel);
double get_normalised_channel_value(char* channel_begin, int bytes_per_channel);

#endif /* CHANNEL_INTERPRETER_H */
