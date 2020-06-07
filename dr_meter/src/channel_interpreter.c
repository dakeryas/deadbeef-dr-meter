#include "channel_interpreter.h"
#include <math.h>

double max_signed_channel_value(int bits_per_channel)
{
    switch(bits_per_channel)
    {
        case 8: return (1 << (8 - 1)) - 1;
        case 16: return (1 << (16 - 1)) - 1;
        case 24: return (1 << (24 - 1)) - 1;
        default: return pow(2, bits_per_channel - 1) - 1;
    }
}

static double double_channel_value(int32_t value, int bits_per_channel)
{
    return value / max_signed_channel_value(bits_per_channel);
}

int32_t get_int_channel_value(char* channel_begin, int bytes_per_channel)
{
    switch(bytes_per_channel)
    {
        case 1:
            return channel_begin[0];
        case 2:
            return *(int16_t*)channel_begin;
        case 3:
            return (unsigned char)channel_begin[0] | (unsigned char)channel_begin[1]<<8 | channel_begin[2]<<16;
        case 4:
        default:
            return *(int32_t*)channel_begin;
    }
}

double get_normalised_channel_value(char* channel_begin, int bytes_per_channel)
{
    if(bytes_per_channel < 4) //int for resonnable listeners
    {
        int32_t channel_value = get_int_channel_value(channel_begin, bytes_per_channel);
        return double_channel_value(channel_value, bytes_per_channel << 3);
    }
    else return *(float*)channel_begin;
}
