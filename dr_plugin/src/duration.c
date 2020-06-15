#include "duration.h"
#include <math.h>
#include  <stdio.h>

duration_t make_duration(double track_length)
{
    unsigned length = rint(track_length);
    duration_t duration =
    {.hours = length / 3600, .minutes = (length % 3600) / 60, .seconds = (length % 3600) % 60};
    return duration;
}

int sprint_duration(duration_t* dur, char* buffer)
{
    char* format = "%.0s%2d:%02d";
    if(dur->hours) format = "%2d:%02d:%02d";
    return sprintf(buffer, format, dur->hours, dur->minutes, dur->seconds);
}
