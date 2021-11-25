/**************************************************************************
* @file duration.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 14 Jun 2020
* @brief struct to split time in seconds into hours, minutes, seconds
* ************************************************************************/

#ifndef DR_PLUGIN_DURATION_H
#define DR_PLUGIN_DURATION_H

struct duration_s
{
    unsigned hours;
    unsigned minutes;
    unsigned seconds;
};

typedef struct duration_s duration_t;

duration_t make_duration(double length_seconds);
int sprint_duration(const duration_t* dur, char* buffer);

#endif /* DR_PLUGIN_DURATION_H */
