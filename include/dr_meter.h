#ifndef DR_METER_H
#define DR_METER_H

#include "histogram.h"

struct block_analyser_s;

typedef struct
{
    double min_hist;
    double max_hist;
    unsigned channels;
    histogram_t* peaks_hist;
    histogram_t* rms_hist;
} dr_meter_t;

void init_dr_meter(dr_meter_t* dr_meter);
void fill_dr_meter(dr_meter_t* dr_meter, struct block_analyser_s* analyser);
void free_dr_meter(dr_meter_t* dr_meter);

#endif /* DR_METER_H */
