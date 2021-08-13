#include "dr_stats.h"
#include "math.h"
#include "audio_functions.h"

dr_stats_t make_dr_stats(double dr, double peak, double rms)
{
    dr_stats_t object = {.dr = dr, .peak = peak, .rms = rms};
    return object;
}

void zero_dr_stats(dr_stats_t* dr_stats)
{
    dr_stats->dr = 0.;
    dr_stats->peak = 0.;
    dr_stats->rms = 0.;
}

dr_stats_t make_zero_dr_stats()
{
    dr_stats_t dr_stats;
    zero_dr_stats(&dr_stats);
    return dr_stats;
}

dr_stats_t make_dB_dr_stats(dr_stats_t* dr_stats)
{
    return make_dr_stats(dr_stats->dr, decibels(dr_stats->peak), decibels(dr_stats->rms));
}
