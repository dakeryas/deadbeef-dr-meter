#include "dr_stats.h"
#include "math.h"
#include "decibels.h"

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

void print_dr_stats(dr_stats_t* dr_stats, FILE* output)
{
    fprintf(output, "%.3f\t%.3f\t%.3f\n", dr_stats->dr, decibels(dr_stats->peak), decibels(dr_stats->rms));
}
