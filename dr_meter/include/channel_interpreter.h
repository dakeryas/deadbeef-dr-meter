#ifndef CHANNEL_INTERPRETER_H
#define CHANNEL_INTERPRETER_H

#include <stdint.h>

int32_t get_int_channel_value(char* channel_begin, int bytes_per_channel);
double get_normalised_channel_value(char* channel_begin, int bytes_per_channel);

#endif /* CHANNEL_INTERPRETER_H */
