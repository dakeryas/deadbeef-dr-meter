#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdio.h>

enum{DR_HIST_SIZE = 10000};
typedef float hist_res;

typedef struct
{
    hist_res min;
    hist_res max;
    unsigned counts[DR_HIST_SIZE];
    hist_res bin_width;
} histogram_t;

void init_hist_mm(histogram_t* hist, double min, double max);
void fill_hist(histogram_t* hist, double value);
double integrate_hist(histogram_t* hist, unsigned first_bin, unsigned last_bin);
void normalise_hist(histogram_t* hist);
void print_hist(histogram_t* hist, FILE* output);

#endif /* HISTOGRAM_H */
