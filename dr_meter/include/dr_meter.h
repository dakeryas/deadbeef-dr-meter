/**************************************************************************
* @file dr_meter.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 09 Jun 2020
* @brief Object to save loudness stats of analysed audio blocks
* ************************************************************************/

#ifndef DR_METER_H
#define DR_METER_H

#include <stdio.h>
#include "dr_stats.h"

struct block_analyser_s;

struct dr_meter_s
{
    unsigned channels;
    unsigned blocks; // expected blocks to analyse
    double* peaks; // maximum per channel
    double* second_peaks; // second maximum per channel
    double** sum2; // sum of squared sample values per block per channel
    unsigned _ana_blocks; // analysed blocks
    unsigned _ana_samples; // analysed samples
};

typedef struct dr_meter_s dr_meter_t;

dr_meter_t make_dr_meter(unsigned channels, unsigned blocks);
void fill_dr_meter(dr_meter_t* dr_meter, const struct block_analyser_s* analyser);
double get_rms_dr_meter(dr_meter_t* dr_meter, unsigned channel);
dr_stats_t get_dr_stats_dr_meter(dr_meter_t* dr_meter, unsigned channel);
void fill_avg_dr_stats_dr_meter(dr_meter_t* dr_meter, dr_stats_t* dr_stats);
dr_stats_t get_avg_dr_stats_dr_meter(dr_meter_t* dr_meter);
void free_dr_meter(dr_meter_t* dr_meter);
void print_dr_meter(dr_meter_t* dr_meter, FILE* output);

#endif /* DR_METER_H */
