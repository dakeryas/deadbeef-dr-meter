/**************************************************************************
* @file sample_mean.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 19 Nov 2021
* @brief Help compute sample mean from new values
* ************************************************************************/

#ifndef DR_METER_SAMPLE_MEAN_H
#define DR_METER_SAMPLE_MEAN_H

struct sample_mean_s
{
    double sum;
    unsigned samples;
};

typedef struct sample_mean_s sample_mean_t;

sample_mean_t make_sample_mean(void);
void reset_sample_mean(sample_mean_t* self);
void observe_sample_mean(sample_mean_t* self, double sample_value);
double get_sample_mean(sample_mean_t* self);

#endif /* DR_METER_SAMPLE_MEAN_H */
