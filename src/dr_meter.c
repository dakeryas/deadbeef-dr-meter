#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "dr_meter.h"
#include "audio_rms.h"
#include "block_analyser.h"

// constants according to DR standard
static const double DR_LOUD_FRACTION = 0.2;

static double decibels(double value)
{
    return 20. * log10(value);
}

void init_dr_meter(dr_meter_t* dr_meter)
{
    dr_meter->_ana_blocks = 0;
    dr_meter->_ana_samples = 0;
    dr_meter->peaks = calloc(dr_meter->channels, sizeof(double));
    dr_meter->second_peaks = calloc(dr_meter->channels, sizeof(double));
    dr_meter->sum2 = malloc(dr_meter->channels * sizeof(*dr_meter->sum2));// allocate dr_meter->channels pointers
    for(unsigned cha = 0; cha < dr_meter->channels; ++cha)
        dr_meter->sum2[cha] = malloc(dr_meter->blocks * sizeof(double));//allocate dr_meter->blocks 1D arrays
}

static void fill_dr_peaks(dr_meter_t* dr_meter, block_analyser_t* analyser, unsigned channel_index)
{
    double block_peak = analyser->peak[channel_index];
    double* first = &dr_meter->peaks[channel_index];
    double* second = &dr_meter->second_peaks[channel_index];
    if(block_peak > *first)
    {
        *second = *first;
        *first = block_peak;
    }
    else if(block_peak > *second) *second = block_peak;
}

static void fill_sum2(dr_meter_t* dr_meter, block_analyser_t* analyser, unsigned channel_index)
{
    dr_meter->sum2[channel_index][dr_meter->_ana_blocks] = analyser->sum2[channel_index];
}

static void fill_dr_meter_channel(dr_meter_t* dr_meter, block_analyser_t* analyser, unsigned channel_index)
{
    fill_dr_peaks(dr_meter, analyser, channel_index);
    fill_sum2(dr_meter, analyser, channel_index);
}

void fill_dr_meter(dr_meter_t* dr_meter, block_analyser_t* analyser)
{
    assert((dr_meter->_ana_blocks < dr_meter->blocks));
    assert((analyser->channels == dr_meter->channels));
    for(unsigned channel_index = 0; channel_index < dr_meter->channels; ++channel_index)
        fill_dr_meter_channel(dr_meter, analyser, channel_index);
    dr_meter->_ana_samples += analyser->samples;
    ++dr_meter->_ana_blocks;
}

static int reverse_comp_samples(const void* s1, const void* s2)
{

    double diff = *(const double*)s1 - *(const double*)s2;//abs(diff) may be smaller than 1
    if(diff > 0) return -1;
    else if(diff < 0) return 1;
    else return 0;
}

static void sort_sum2(dr_meter_t* dr_meter, unsigned channel)
{
    qsort(dr_meter->sum2[channel], dr_meter->_ana_blocks, sizeof(double), reverse_comp_samples);
}

static void sort_all_sum2(dr_meter_t* dr_meter)
{
    for(unsigned channel_index = 0; channel_index < dr_meter->channels; ++channel_index)
        sort_sum2(dr_meter, channel_index);
}

static double get_sum2_dr_meter(dr_meter_t* dr_meter, unsigned channel, unsigned last_block)
{
    double sum2 = 0.;
    for(unsigned block_index = 0; block_index < last_block; ++block_index)
        sum2 += dr_meter->sum2[channel][block_index];
    return sum2;
}

static double get_tot_sum2_dr_meter(dr_meter_t* dr_meter, unsigned channel)
{
    return get_sum2_dr_meter(dr_meter, channel, dr_meter->_ana_blocks);
}

double get_rms_dr_meter(dr_meter_t* dr_meter, unsigned channel)
{
    double sum2 = get_tot_sum2_dr_meter(dr_meter, channel);
    return get_audio_rms(sum2, dr_meter->_ana_samples);
}

double get_dr_dr_meter(dr_meter_t* dr_meter, unsigned channel)
{
    sort_sum2(dr_meter, channel);
    unsigned last_block = DR_LOUD_FRACTION * dr_meter->_ana_blocks;
    double loud_fraction_sum2 = get_sum2_dr_meter(dr_meter, channel, last_block);
    double loud_fraction_rms = sqrt(loud_fraction_sum2 / (DR_LOUD_FRACTION * dr_meter->_ana_samples));
    return decibels(dr_meter->second_peaks[channel] / loud_fraction_rms);
}

void free_dr_meter(dr_meter_t* dr_meter)
{
    free(dr_meter->peaks);
    free(dr_meter->second_peaks);
    free(dr_meter->sum2);
}
