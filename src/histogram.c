#include <assert.h>
#include <string.h>
#include "histogram.h"

static void init_hist(histogram_t* hist)
{
    memset(hist->counts, 0, DR_HIST_SIZE * sizeof(*hist->counts));
    hist->bin_width = (hist->max - hist->min) / DR_HIST_SIZE;
}

void init_hist_mm(histogram_t* hist, double min, double max)
{
    hist->min = min;
    hist->max = max;
    init_hist(hist);
}

static unsigned bin_hist(histogram_t* hist, double value)
{
    return (value - hist->min) / hist->bin_width;
}

static hist_res bin_low_edge(histogram_t* hist, unsigned bin_index)
{
    return hist->min + bin_index * hist->bin_width;
}

static hist_res bin_up_edge(histogram_t* hist, unsigned bin_index)
{
    return bin_low_edge(hist, bin_index + 1);
}

void fill_hist(histogram_t* hist, double value)
{
    unsigned index = bin_hist(hist, value);
    assert((index < DR_HIST_SIZE));
    ++(hist->counts[index]);
}

static void divide_hist(histogram_t* hist, double value)
{
    assert((value != 0));
    for(unsigned k = 0; k < DR_HIST_SIZE; ++k)
        hist->counts[k] /= value;
}

static void assert_valid_bins(unsigned first_bin, unsigned last_bin)
{
    assert((first_bin >= 0 && first_bin <= DR_HIST_SIZE));
    assert((last_bin >= 0 && last_bin <= DR_HIST_SIZE && last_bin >= first_bin));
}

double integrate_hist(histogram_t* hist, unsigned first_bin, unsigned last_bin)
{
    assert_valid_bins(first_bin, last_bin);
    double sum = 0.;
    for(unsigned k = first_bin; k < last_bin; ++k)
        sum += hist->counts[k];
    return sum;
}

void normalise_hist(histogram_t* hist)
{
    double integral = integrate_hist(hist, 0, DR_HIST_SIZE);
    divide_hist(hist, integral);
}

void print_hist(histogram_t* hist, FILE* output)
{
    for(unsigned k = 0; k < DR_HIST_SIZE; ++k)
        fprintf(output, "%f ,\t%f :\t%i\n", bin_low_edge(hist, k), bin_up_edge(hist, k), hist->counts[k]);
}
