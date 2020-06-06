/**************************************************************************
* @file dr_stats.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 03 Jun 2020
* @brief Data structure to hold DR numbers
* ************************************************************************/

#ifndef DR_STATS_H
#define DR_STATS_H

#include <stdio.h>

struct dr_stats_s
{
    double dr;
    double peak;
    double rms;
};

typedef struct dr_stats_s dr_stats_t;

dr_stats_t make_dr_stats(double dr, double peak, double rms);
void zero_dr_stats(dr_stats_t* dr_stats);
dr_stats_t make_zero_dr_stats();
dr_stats_t make_dB_dr_stats(dr_stats_t* dr_stats);
int print_dr_stats(dr_stats_t* dr_stats, FILE* output);
int print_buffer_dr_stats(dr_stats_t* dr_stats, char* buffer);

#endif /* DR_STATS_H */
