#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "block_analyser.h"
#include "audio_functions.h"
#include "channel_interpreter.h"

static unsigned size_analyser_array(block_analyser_t* self)
{
    return self->channels * sizeof(block_res);
}

static void init_block_analyser(block_analyser_t* self)
{
    self->peak = malloc(size_analyser_array(self));
    self->sum2 = malloc(size_analyser_array(self));
}

block_analyser_t make_block_analyser(unsigned channels)
{
    block_analyser_t analyser = {.channels = channels};
    init_block_analyser(&analyser);
    return analyser;
}

static void reset_block_analyser(block_analyser_t* self)
{
    memset(self->peak, 0., size_analyser_array(self));
    memset(self->sum2, 0., size_analyser_array(self));
    self->samples = 0;
}

static void update_peak(block_analyser_t* self, double sample_value, int channel_index)
{
    double value = fabs(sample_value);
    if(value >self->peak[channel_index]) self->peak[channel_index] = value;
}

static void update_moment2(block_analyser_t* self, double sample_value, int channel_index)
{
    self->sum2[channel_index] += sample_value * sample_value;
}

static void analyse_channel_value(block_analyser_t* self, double sample_value, int channel_index)
{
    update_peak(self, sample_value, channel_index);
    update_moment2(self, sample_value, channel_index);
}

static void analyse_sample(block_analyser_t* self, char* sample_begin, int nu_channels, int bytes_per_channel)
{
    for(int channel_index = 0; channel_index < nu_channels; ++channel_index)
    {
        char* channel_begin = sample_begin + channel_index * bytes_per_channel;
        double channel_value = get_normalised_channel_value(channel_begin, bytes_per_channel);
        analyse_channel_value(self, channel_value, channel_index);
    }
    ++self->samples;
}

void analyse_block(block_analyser_t* self, char* bytes_begin, unsigned read_bytes, int nu_channels, int bits_per_channel)
{
    assert((self->channels == (unsigned)nu_channels));
    reset_block_analyser(self);
    int bytes_per_channel = bits_per_channel >> 3;
    int bytes_per_sample = nu_channels * bytes_per_channel;
    unsigned nu_samples = read_bytes / bytes_per_sample;
    for(size_t sample_index = 0; sample_index < nu_samples; ++sample_index)
    {
        char* sample_begin = bytes_begin + sample_index * bytes_per_sample;
        analyse_sample(self, sample_begin, nu_channels, bytes_per_channel);
    }
}

int filled_block_analyser(block_analyser_t* self)
{
    return self->samples > 0;
}

double get_avg_sum_squares(block_analyser_t* self, unsigned channel)
{
    return self->sum2[channel] / self->samples;
}

double get_rms_analyser(block_analyser_t* self, unsigned channel)
{
    return get_audio_rms(self->sum2[channel], self->samples);
}

void free_block_analyser(block_analyser_t* self)
{
    free(self->peak);
    free(self->sum2);
}

void print_block_analyser(block_analyser_t* self, FILE* output)
{
    if(filled_block_analyser(self))
    {
        for(unsigned cha = 0; cha < self->channels; ++cha)
            fprintf(output, "ch%i: %f %f\t", cha, self->peak[cha], get_rms_analyser(self, cha));
    }
    fprintf(output, "%i sp\n", self->samples);
}
