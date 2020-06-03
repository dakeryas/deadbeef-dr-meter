/**************************************************************************
* @file dr_stats.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 03 Jun 2020
* @brief Data structure to hold DR numbers
* ************************************************************************/

#ifndef DR_STATS_H
#define DR_STATS_H

struct dr_stats_s
{
    double dr;
    double second_peak;
    double rms;
};

typedef struct dr_stats_s dr_stats_t;

dr_stats_t make_dr_stats(double dr, double second_peak, double rms);
dr_stats_t make_zero_dr_stats();

#endif /* DR_STATS_H */
