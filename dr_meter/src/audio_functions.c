#include "audio_functions.h"
#include "math.h"

double decibels(double value)
{
    return 20. * log10(value);
}

double get_audio_rms(double sum2, unsigned number_of_samples)
{
    return sqrt(2 * sum2 / number_of_samples);
}

double true_peak(double peak_value)
{
    return fabs(peak_value) < 1. ? peak_value : (peak_value < 0 ? -1. : 1.);
}
