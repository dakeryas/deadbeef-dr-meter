#ifndef BLOCK_ANALYSER_H
#define BLOCK_ANALYSER_H

#include <stdio.h>

typedef double block_res;

struct block_analyser_s
{
    unsigned channels;
    block_res* peak;
    block_res* sum2;
    unsigned samples;
};

typedef struct block_analyser_s block_analyser_t;

block_analyser_t make_block_analyser(unsigned channels);
void analyse_block(block_analyser_t* analyser, char* bytes_begin, unsigned read_bytes, int nu_channels, int bits_per_channel, int sample_rate);
int filled_block_analyser(block_analyser_t* analyser);
double get_avg_sum_squares(block_analyser_t* analyser, unsigned channel);
double get_rms_analyser(block_analyser_t* analyser, unsigned channel);
void free_block_analyser(block_analyser_t* analyser);
void print_block_analyser(block_analyser_t* analyser, FILE* output);

#endif /* BLOCK_ANALYSER_H */
