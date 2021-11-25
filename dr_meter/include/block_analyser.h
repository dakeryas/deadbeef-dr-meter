/**************************************************************************
* @file block_analyser.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 09 Jun 2020
* @brief Analyse bytes as audio samples to save peak (max) and second
* moment (sum of samples squared) for each channel
* ************************************************************************/

#ifndef DR_METER_BLOCK_ANALYSER_H
#define DR_METER_BLOCK_ANALYSER_H

#include <stdio.h>

typedef double block_res;

struct block_analyser_s
{
    unsigned channels;
    block_res* peak; // max of all (absolute value) samples per channel
    block_res* sum2; // sum of squared samples per channel
    unsigned samples; // number of samples analysed
};

typedef struct block_analyser_s block_analyser_t;

block_analyser_t make_block_analyser(unsigned channels);
void analyse_block(block_analyser_t* analyser, char* bytes_begin, unsigned read_bytes, int nu_channels, int bits_per_channel);
int filled_block_analyser(const block_analyser_t* analyser);
double get_avg_sum_squares(const block_analyser_t* analyser, unsigned channel);
double get_rms_analyser(const block_analyser_t* analyser, unsigned channel);
void free_block_analyser(block_analyser_t* analyser);
void print_block_analyser(const block_analyser_t* analyser, FILE* output);

#endif /* DR_METER_BLOCK_ANALYSER_H */
