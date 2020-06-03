#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "dr_meter.h"
#include "audio_rms.h"
#include "block_analyser.h"
#include "print_dr_meter.h"

// constants according to DR standard
static const double DR_LOUD_FRACTION = 0.2;

static double decibels(double value)
{
    return 20. * log10(value);
}

static void init_dr_meter(dr_meter_t* dr_meter)
{
    dr_meter->_ana_blocks = 0;
    dr_meter->_ana_samples = 0;
    dr_meter->peaks = calloc(dr_meter->channels, sizeof(double));
    dr_meter->second_peaks = calloc(dr_meter->channels, sizeof(double));
    dr_meter->sum2 = malloc(dr_meter->channels * sizeof(*dr_meter->sum2));// allocate dr_meter->channels pointers
    for(unsigned cha = 0; cha < dr_meter->channels; ++cha)
        dr_meter->sum2[cha] = malloc(dr_meter->blocks * sizeof(double));//allocate dr_meter->blocks 1D arrays
}

dr_meter_t make_dr_meter(unsigned channels, unsigned blocks)
{
    dr_meter_t dr_meter = {.channels = channels, .blocks = blocks};
    init_dr_meter(&dr_meter);
    return dr_meter;
}

static int filled(dr_meter_t* dr_meter)
{
    return dr_meter->_ana_blocks > 0 && dr_meter->_ana_samples > 0;
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

static void fill_channel(dr_meter_t* dr_meter, block_analyser_t* analyser, unsigned channel_index)
{
    fill_dr_peaks(dr_meter, analyser, channel_index);
    fill_sum2(dr_meter, analyser, channel_index);
}

void fill_dr_meter(dr_meter_t* dr_meter, block_analyser_t* analyser)
{
    assert((dr_meter->_ana_blocks < dr_meter->blocks));
    assert((analyser->channels == dr_meter->channels));
    for(unsigned channel_index = 0; channel_index < dr_meter->channels; ++channel_index)
        fill_channel(dr_meter, analyser, channel_index);
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

static double get_sum2(dr_meter_t* dr_meter, unsigned channel, unsigned last_block)
{
    double sum2 = 0.;
    for(unsigned block_index = 0; block_index < last_block; ++block_index)
        sum2 += dr_meter->sum2[channel][block_index];
    return sum2;
}

static double get_tot_sum2(dr_meter_t* dr_meter, unsigned channel)
{
    return get_sum2(dr_meter, channel, dr_meter->_ana_blocks);
}

static double get_sum2_quantile(dr_meter_t* dr_meter, unsigned channel, double quantile)
{
    unsigned last_block = quantile * dr_meter->_ana_blocks;
    if(!last_block) last_block = 1; //not enough blocks analysed
    return get_sum2(dr_meter, channel, last_block);
}


double get_rms_dr_meter(dr_meter_t* dr_meter, unsigned channel)
{
    double sum2 = get_tot_sum2(dr_meter, channel);
    return get_audio_rms(sum2, dr_meter->_ana_samples);
}

static double get_dr_dr_meter(dr_meter_t* dr_meter, unsigned channel)
{
    sort_sum2(dr_meter, channel);
    double loud_sum2 = get_sum2_quantile(dr_meter, channel, DR_LOUD_FRACTION);
    double loud_rms = get_audio_rms(loud_sum2, DR_LOUD_FRACTION * dr_meter->_ana_samples);
    return decibels(dr_meter->second_peaks[channel] / loud_rms);
}

static dr_stats_t get_dr_stats_filled(dr_meter_t* dr_meter, unsigned channel)
{
    double dr = get_dr_dr_meter(dr_meter, channel);
    double second_peak = dr_meter->second_peaks[channel];
    double rms = get_rms_dr_meter(dr_meter, channel);
    return make_dr_stats(dr, decibels(second_peak), decibels(rms));
}

dr_stats_t get_dr_stats_dr_meter(dr_meter_t* dr_meter, unsigned channel)
{
    if(filled(dr_meter)) return get_dr_stats_filled(dr_meter, channel);
    else return make_dr_stats(0., -INFINITY, -INFINITY);
}

dr_stats_t get_avg_dr_stats_dr_meter(dr_meter_t* dr_meter)
{
    dr_stats_t result = make_dr_stats(0., -INFINITY, -INFINITY);
    if(filled(dr_meter))
    {
        result.rms = 0.;
        for(unsigned cha = 0; cha < dr_meter->channels; ++cha)
        {
            dr_stats_t current = get_dr_stats_filled(dr_meter, cha);
            if(current.second_peak > result.second_peak) result.second_peak = current.second_peak;
            result.rms += current.rms;
            result.dr += current.dr;
        }
        result.rms /= dr_meter->channels;
        result.dr /= dr_meter->channels;
    }
    return result;
}

void free_dr_meter(dr_meter_t* dr_meter)
{
    free(dr_meter->peaks);
    free(dr_meter->second_peaks);
    free(dr_meter->sum2);
}

void print_dr_meter(dr_meter_t* dr_meter, FILE* output)
{
    print_dr_meter_impl(dr_meter, output);
}
