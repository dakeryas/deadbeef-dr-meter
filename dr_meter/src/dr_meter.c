#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "dr_meter.h"
#include "audio_functions.h"
#include "block_analyser.h"
#include "print_dr_meter.h"

// constants according to DR standard
static const double DR_LOUD_FRACTION = 0.2;

static void init_dr_meter(dr_meter_t* self)
{
    self->_ana_blocks = 0;
    self->_ana_samples = 0;
    self->peaks = calloc(self->channels, sizeof(double));
    self->second_peaks = calloc(self->channels, sizeof(double));
    self->sum2 = malloc(self->channels * sizeof(*self->sum2));// allocate self->channels pointers
    for(unsigned cha = 0; cha < self->channels; ++cha)
        self->sum2[cha] = malloc(self->blocks * sizeof(double));//allocate self->blocks 1D arrays
}

dr_meter_t make_dr_meter(unsigned channels, unsigned blocks)
{
    dr_meter_t dr_meter = {.channels = channels, .blocks = blocks};
    init_dr_meter(&dr_meter);
    return dr_meter;
}

static int filled(dr_meter_t* self)
{
    return self->_ana_blocks > 0 && self->_ana_samples > 0;
}

static void fill_dr_peaks(dr_meter_t* self, block_analyser_t* analyser, unsigned channel_id)
{
    double block_peak = analyser->peak[channel_id];
    double* first = &self->peaks[channel_id];
    double* second = &self->second_peaks[channel_id];
    if(block_peak > *first)
    {
        *second = *first;
        *first = block_peak;
    }
    else if(block_peak > *second) *second = block_peak;
}

static void fill_sum2(dr_meter_t* self, block_analyser_t* analyser, unsigned channel_id)
{
    self->sum2[channel_id][self->_ana_blocks] = get_avg_sum_squares(analyser, channel_id);
}

static void fill_channel(dr_meter_t* self, block_analyser_t* analyser, unsigned channel_id)
{
    fill_dr_peaks(self, analyser, channel_id);
    fill_sum2(self, analyser, channel_id);
}

static void fill_channels(dr_meter_t* self, block_analyser_t* analyser)
{
    for(unsigned channel_id = 0; channel_id < self->channels; ++channel_id)
        fill_channel(self, analyser, channel_id);
}

static void fill_dr_meter_nocheck(dr_meter_t* self, block_analyser_t* analyser)
{
    fill_channels(self, analyser);
    self->_ana_samples += analyser->samples;
    ++self->_ana_blocks;
}

void fill_dr_meter(dr_meter_t* self, block_analyser_t* analyser)
{
    assert((self->_ana_blocks < self->blocks));
    assert((analyser->channels == self->channels));
    if(filled_block_analyser(analyser)) fill_dr_meter_nocheck(self, analyser);
}

static int reverse_comp_samples(const void* s1, const void* s2)
{

    double diff = *(const double*)s1 - *(const double*)s2;//abs(diff) may be smaller than 1
    if(diff > 0) return -1;
    else if(diff < 0) return 1;
    else return 0;
}

static void sort_sum2(dr_meter_t* self, unsigned channel)
{
    qsort(self->sum2[channel], self->_ana_blocks, sizeof(double), reverse_comp_samples);
}

static double get_sum2(dr_meter_t* self, unsigned channel, unsigned last_block)
{
    double sum2 = 0.;
    for(unsigned block_index = 0; block_index < last_block; ++block_index)
        sum2 += self->sum2[channel][block_index];
    return sum2;
}

static double get_tot_sum2(dr_meter_t* self, unsigned channel)
{
    return get_sum2(self, channel, self->_ana_blocks);
}

static double get_sum2_quantile(dr_meter_t* self, unsigned channel, double quantile)
{
    unsigned last_block = quantile * self->_ana_blocks;
    if(!last_block) last_block = 1; //not enough blocks analysed
    return get_sum2(self, channel, last_block);
}


double get_rms_dr_meter(dr_meter_t* self, unsigned channel)
{
    double sum2 = get_tot_sum2(self, channel);
    return get_audio_rms(sum2, self->_ana_blocks);
}

static double get_dr_dr_meter(dr_meter_t* self, unsigned channel)
{
    sort_sum2(self, channel);
    double loud_sum2 = get_sum2_quantile(self, channel, DR_LOUD_FRACTION);
    double loud_rms = get_audio_rms(loud_sum2, DR_LOUD_FRACTION * self->_ana_blocks);
    double second_max = true_peak(self->second_peaks[channel]);
    return decibels(second_max / loud_rms);
}

static dr_stats_t get_dr_stats_filled(dr_meter_t* self, unsigned channel)
{
    double dr = get_dr_dr_meter(self, channel);
    double peak = true_peak(self->peaks[channel]);
    double rms = get_rms_dr_meter(self, channel);
    return make_dr_stats(dr, peak, rms);
}

dr_stats_t get_dr_stats_dr_meter(dr_meter_t* self, unsigned channel)
{
    if(filled(self)) return get_dr_stats_filled(self, channel);
    else return make_zero_dr_stats();
}

void fill_avg_dr_stats_dr_meter(dr_meter_t* self, dr_stats_t* result)
{
    zero_dr_stats(result);
    if(filled(self))
    {
        for(unsigned cha = 0; cha < self->channels; ++cha)
        {
            dr_stats_t current = get_dr_stats_filled(self, cha);
            if(current.peak > result->peak) result->peak = current.peak;
            result->rms += current.rms;
            result->dr += current.dr;
        }
        result->rms /= self->channels;
        result->dr /= self->channels;
    }
}

dr_stats_t get_avg_dr_stats_dr_meter(dr_meter_t* self)
{
    dr_stats_t result;
    fill_avg_dr_stats_dr_meter(self, &result);
    return result;
}

void free_dr_meter(dr_meter_t* self)
{
    free(self->peaks);
    free(self->second_peaks);
    for(unsigned cha = 0; cha < self->channels; ++cha)
        free(self->sum2[cha]);
    free(self->sum2);
}

void print_dr_meter(dr_meter_t* self, FILE* output)
{
    print_dr_meter_impl(self, output);
}
