#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "block_analyser.h"
#include "channel_interpreter.h"

static unsigned size_analyser_array(block_analyser_t* analyser)
{
    return analyser->channels * sizeof(block_res);
}

void init_block_analyser(block_analyser_t* analyser)
{
    analyser->peak = malloc(size_analyser_array(analyser));
    analyser->sum2 = malloc(size_analyser_array(analyser));
}

static void reset_block_analyser(block_analyser_t* analyser)
{
    memset(analyser->peak, 0., size_analyser_array(analyser));
    memset(analyser->sum2, 0., size_analyser_array(analyser));
    analyser->samples = 0;
}

static void update_peak(block_analyser_t* analyser, double sample_value, int channel_index)
{
    double value = fabs(sample_value);
    if(value >analyser->peak[channel_index]) analyser->peak[channel_index] = value;
}

static void update_moment2(block_analyser_t* analyser, double sample_value, int channel_index)
{
    analyser->sum2[channel_index] += sample_value * sample_value;
}

static void analyse_channel_value(block_analyser_t* analyser, double sample_value, int channel_index)
{
    update_peak(analyser, sample_value, channel_index);
    update_moment2(analyser, sample_value, channel_index);
}

static void analyse_sample(block_analyser_t* analyser, char* sample_begin, int nu_channels, int bytes_per_channel)
{
    for(int channel_index = 0; channel_index < nu_channels; ++channel_index)
    {
        char* channel_begin = sample_begin + channel_index * bytes_per_channel;
        double channel_value = get_normalised_channel_value(channel_begin, bytes_per_channel);
        analyse_channel_value(analyser, channel_value, channel_index);
    }
    ++analyser->samples;
}

void analyse_block(block_analyser_t* analyser, char* bytes_begin, unsigned buffer_size, int nu_channels, int bits_per_channel, int sample_rate)
{
    reset_block_analyser(analyser);
    int bytes_per_channel = bits_per_channel >> 3;
    int bytes_per_sample = nu_channels * bytes_per_channel;
    unsigned nu_samples = buffer_size / bytes_per_sample;

    for(size_t sample_index = 0; sample_index < nu_samples; ++sample_index)
    {
        char* sample_begin = bytes_begin + sample_index * bytes_per_sample;
        analyse_sample(analyser, sample_begin, nu_channels, bytes_per_channel);
    }
}


double get_analyser_rms(block_analyser_t* analyser, unsigned channel)
{
    return sqrt(2 * analyser->sum2[channel] / analyser->samples); //DR assumes 0 mean
}

void free_block_analyser(block_analyser_t* analyser)
{
    free(analyser->peak);
    free(analyser->sum2);
}

void  print_block_analyser(block_analyser_t* analyser, FILE* output)
{
    for(unsigned cha = 0; cha < analyser->channels; ++cha)
        fprintf(output, "ch%i: %f %f\t", cha, analyser->peak[cha], get_analyser_rms(analyser, cha));
    fprintf(output, "%i sp\n", analyser->samples);
}
