#ifndef DR_METER_H
#define DR_METER_H

#include <stdio.h>

struct block_analyser_s;

typedef struct
{
    unsigned channels;
    unsigned blocks; // expected blocks to analyse
    double* peaks; // maximum per channel
    double* second_peaks; // second maximum per channel
    double** sum2; // sum of squared sample values per block per channel
    unsigned _ana_blocks; // analysed blocks
    unsigned _ana_samples; // analysed samples
} dr_meter_t;

void init_dr_meter(dr_meter_t* dr_meter);
void fill_dr_meter(dr_meter_t* dr_meter, struct block_analyser_s* analyser);
double get_rms_dr_meter(dr_meter_t* dr_meter, unsigned channel);
double get_dr_dr_meter(dr_meter_t* dr_meter, unsigned channel);
void free_dr_meter(dr_meter_t* dr_meter);
void print_dr_meter(dr_meter_t* dr_meter, FILE* output);

#endif /* DR_METER_H */
