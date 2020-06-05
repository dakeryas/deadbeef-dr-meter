#include <math.h>

double get_audio_rms(double sum2, unsigned number_of_samples)
{
    return sqrt(2 * sum2 / number_of_samples);
}
