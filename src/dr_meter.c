#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "dr_meter.h"
#include "block_analyser.h"

static double decibels(double value)
{
    return 20. * log10(value);
}

void init_dr_meter(dr_meter_t* dr_meter)
{
    dr_meter->peaks_hist = malloc(dr_meter->channels * sizeof(histogram_t));
    dr_meter->rms_hist = malloc(dr_meter->channels * sizeof(histogram_t));
    for(unsigned channel_index = 0; channel_index < dr_meter->channels ; ++channel_index)
    {
        init_hist_mm(&dr_meter->peaks_hist[channel_index], dr_meter->min_hist, dr_meter->max_hist);
        init_hist_mm(&dr_meter->rms_hist[channel_index], dr_meter->min_hist, dr_meter->max_hist);
    }
}

static void fill_dr_peaks(dr_meter_t* dr_meter, block_analyser_t* analyser, unsigned channel_index)
{
    float peak_dB =  decibels(analyser->peak[channel_index]);
    fill_hist(&dr_meter->peaks_hist[channel_index], peak_dB);
}

static void fill_dr_rms(dr_meter_t* dr_meter, block_analyser_t* analyser, unsigned channel_index)
{
    float rms_dB = decibels(get_analyser_rms(analyser, channel_index));
    fill_hist(&dr_meter->rms_hist[channel_index], rms_dB);
}

static void fill_dr_meter_channel(dr_meter_t* dr_meter, block_analyser_t* analyser, unsigned channel_index)
{
    fill_dr_peaks(dr_meter, analyser, channel_index);
    fill_dr_rms(dr_meter, analyser, channel_index);
}

void fill_dr_meter(dr_meter_t* dr_meter, block_analyser_t* analyser)
{
    assert((analyser->channels == dr_meter->channels));
    for(unsigned channel_index = 0; channel_index < dr_meter->channels; ++channel_index)
        fill_dr_meter_channel(dr_meter, analyser, channel_index);
}

void free_dr_meter(dr_meter_t* dr_meter)
{
    free(dr_meter->peaks_hist);
    free(dr_meter->rms_hist);
}
