#include "dr_stats.h"

dr_stats_t make_dr_stats(double dr, double second_peak, double rms)
{
    dr_stats_t object = {.dr = dr, .second_peak = second_peak, .rms = rms};
    return object;
}
