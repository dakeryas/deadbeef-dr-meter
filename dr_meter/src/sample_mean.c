#include "sample_mean.h"

sample_mean_t make_sample_mean()
{
    sample_mean_t sample_mean;
    reset_sample_mean(&sample_mean);
    return sample_mean;
}

void reset_sample_mean(sample_mean_t* self)
{
    self->sum = 0.;
    self->samples = 0;
}

void observe_sample_mean(sample_mean_t* self, double sample_value)
{
    self->sum += sample_value;
    ++self->samples;
}

double get_sample_mean(sample_mean_t* self)
{
    return self->sum / self->samples;
}
