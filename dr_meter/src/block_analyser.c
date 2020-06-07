#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "block_analyser.h"
#include "audio_rms.h"
#include "channel_interpreter.h"

static unsigned size_analyser_array(block_analyser_t* this)
{
    return this->channels * sizeof(block_res);
}

static void init_block_analyser(block_analyser_t* this)
{
    this->peak = malloc(size_analyser_array(this));
    this->sum2 = malloc(size_analyser_array(this));
}

block_analyser_t make_block_analyser(unsigned channels)
{
    block_analyser_t analyser = {.channels = channels};
    init_block_analyser(&analyser);
    return analyser;
}

static void reset_block_analyser(block_analyser_t* this)
{
    memset(this->peak, 0., size_analyser_array(this));
    memset(this->sum2, 0., size_analyser_array(this));
    this->samples = 0;
}

static void update_peak(block_analyser_t* this, double sample_value, int channel_index)
{
    double value = fabs(sample_value);
    if(value >this->peak[channel_index]) this->peak[channel_index] = value;
}

static void update_moment2(block_analyser_t* this, double sample_value, int channel_index)
{
    this->sum2[channel_index] += sample_value * sample_value;
}

static void analyse_channel_value(block_analyser_t* this, double sample_value, int channel_index)
{
    update_peak(this, sample_value, channel_index);
    update_moment2(this, sample_value, channel_index);
}

static void analyse_sample(block_analyser_t* this, char* sample_begin, int nu_channels, int bytes_per_channel)
{
    for(int channel_index = 0; channel_index < nu_channels; ++channel_index)
    {
        char* channel_begin = sample_begin + channel_index * bytes_per_channel;
        double channel_value = get_normalised_channel_value(channel_begin, bytes_per_channel);
        analyse_channel_value(this, channel_value, channel_index);
    }
    ++this->samples;
}

void analyse_block(block_analyser_t* this, char* bytes_begin, unsigned read_bytes, int nu_channels, int bits_per_channel, int sample_rate)
{
    reset_block_analyser(this);
    int bytes_per_channel = bits_per_channel >> 3;
    int bytes_per_sample = nu_channels * bytes_per_channel;
    unsigned nu_samples = read_bytes / bytes_per_sample;

    for(size_t sample_index = 0; sample_index < nu_samples; ++sample_index)
    {
        char* sample_begin = bytes_begin + sample_index * bytes_per_sample;
        analyse_sample(this, sample_begin, nu_channels, bytes_per_channel);
    }
}

int filled_block_analyser(block_analyser_t* this)
{
    return this->samples > 0;
}

double get_rms_analyser(block_analyser_t* this, unsigned channel)
{
    if(filled(this)) return get_audio_rms(this->sum2[channel], this->samples);
    else return 0.;
}

void free_block_analyser(block_analyser_t* this)
{
    free(this->peak);
    free(this->sum2);
}

void print_block_analyser(block_analyser_t* this, FILE* output)
{
    for(unsigned cha = 0; cha < this->channels; ++cha)
        fprintf(output, "ch%i: %f %f\t", cha, this->peak[cha], get_rms_analyser(this, cha));
    fprintf(output, "%i sp\n", this->samples);
}
