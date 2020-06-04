#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "dr_meter.h"
#include "audio_rms.h"
#include "block_analyser.h"
#include "print_dr_meter.h"
#include "decibels.h"

// constants according to DR standard
static const double DR_LOUD_FRACTION = 0.2;

static void init_dr_meter(dr_meter_t* this)
{
    this->_ana_blocks = 0;
    this->_ana_samples = 0;
    this->peaks = calloc(this->channels, sizeof(double));
    this->second_peaks = calloc(this->channels, sizeof(double));
    this->sum2 = malloc(this->channels * sizeof(*this->sum2));// allocate this->channels pointers
    for(unsigned cha = 0; cha < this->channels; ++cha)
        this->sum2[cha] = malloc(this->blocks * sizeof(double));//allocate this->blocks 1D arrays
}

dr_meter_t make_dr_meter(unsigned channels, unsigned blocks)
{
    dr_meter_t dr_meter = {.channels = channels, .blocks = blocks};
    init_dr_meter(&dr_meter);
    return dr_meter;
}

static int filled(dr_meter_t* this)
{
    return this->_ana_blocks > 0 && this->_ana_samples > 0;
}

static void fill_dr_peaks(dr_meter_t* this, block_analyser_t* analyser, unsigned channel_index)
{
    double block_peak = analyser->peak[channel_index];
    double* first = &this->peaks[channel_index];
    double* second = &this->second_peaks[channel_index];
    if(block_peak > *first)
    {
        *second = *first;
        *first = block_peak;
    }
    else if(block_peak > *second) *second = block_peak;
}

static void fill_sum2(dr_meter_t* this, block_analyser_t* analyser, unsigned channel_index)
{
    this->sum2[channel_index][this->_ana_blocks] = analyser->sum2[channel_index];
}

static void fill_channel(dr_meter_t* this, block_analyser_t* analyser, unsigned channel_index)
{
    fill_dr_peaks(this, analyser, channel_index);
    fill_sum2(this, analyser, channel_index);
}

void fill_dr_meter(dr_meter_t* this, block_analyser_t* analyser)
{
    assert((this->_ana_blocks < this->blocks));
    assert((analyser->channels == this->channels));
    for(unsigned channel_index = 0; channel_index < this->channels; ++channel_index)
        fill_channel(this, analyser, channel_index);
    this->_ana_samples += analyser->samples;
    ++this->_ana_blocks;
}

static int reverse_comp_samples(const void* s1, const void* s2)
{

    double diff = *(const double*)s1 - *(const double*)s2;//abs(diff) may be smaller than 1
    if(diff > 0) return -1;
    else if(diff < 0) return 1;
    else return 0;
}

static void sort_sum2(dr_meter_t* this, unsigned channel)
{
    qsort(this->sum2[channel], this->_ana_blocks, sizeof(double), reverse_comp_samples);
}

static double get_sum2(dr_meter_t* this, unsigned channel, unsigned last_block)
{
    double sum2 = 0.;
    for(unsigned block_index = 0; block_index < last_block; ++block_index)
        sum2 += this->sum2[channel][block_index];
    return sum2;
}

static double get_tot_sum2(dr_meter_t* this, unsigned channel)
{
    return get_sum2(this, channel, this->_ana_blocks);
}

static double get_sum2_quantile(dr_meter_t* this, unsigned channel, double quantile)
{
    unsigned last_block = quantile * this->_ana_blocks;
    if(!last_block) last_block = 1; //not enough blocks analysed
    return get_sum2(this, channel, last_block);
}


double get_rms_dr_meter(dr_meter_t* this, unsigned channel)
{
    double sum2 = get_tot_sum2(this, channel);
    return get_audio_rms(sum2, this->_ana_samples);
}

static double get_dr_dr_meter(dr_meter_t* this, unsigned channel)
{
    sort_sum2(this, channel);
    double loud_sum2 = get_sum2_quantile(this, channel, DR_LOUD_FRACTION);
    double loud_rms = get_audio_rms(loud_sum2, DR_LOUD_FRACTION * this->_ana_samples);
    return decibels(this->second_peaks[channel] / loud_rms);
}

static dr_stats_t get_dr_stats_filled(dr_meter_t* this, unsigned channel)
{
    double dr = get_dr_dr_meter(this, channel);
    double peak = this->peaks[channel];
    double rms = get_rms_dr_meter(this, channel);
    return make_dr_stats(dr, peak, rms);
}

dr_stats_t get_dr_stats_dr_meter(dr_meter_t* this, unsigned channel)
{
    if(filled(this)) return get_dr_stats_filled(this, channel);
    else return make_zero_dr_stats();
}

void fill_avg_dr_stats_dr_meter(dr_meter_t* this, dr_stats_t* result)
{
    zero_dr_stats(result);
    if(filled(this))
    {
        for(unsigned cha = 0; cha < this->channels; ++cha)
        {
            dr_stats_t current = get_dr_stats_filled(this, cha);
            if(current.peak > result->peak) result->peak = current.peak;
            result->rms += current.rms;
            result->dr += current.dr;
        }
        result->rms /= this->channels;
        result->dr /= this->channels;
    }
}

dr_stats_t get_avg_dr_stats_dr_meter(dr_meter_t* this)
{
    dr_stats_t result;
    fill_avg_dr_stats_dr_meter(this, &result);
    return result;
}

void free_dr_meter(dr_meter_t* this)
{
    free(this->peaks);
    free(this->second_peaks);
    free(this->sum2);
}

void print_dr_meter(dr_meter_t* this, FILE* output)
{
    print_dr_meter_impl(this, output);
}
