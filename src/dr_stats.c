#include "dr_stats.h"
#include "math.h"

dr_stats_t make_dr_stats(double dr, double second_peak, double rms)
{
    dr_stats_t object = {.dr = dr, .second_peak = second_peak, .rms = rms};
    return object;
}

dr_stats_t make_zero_dr_stats()
{
    return make_dr_stats(0., -INFINITY, -INFINITY);
}

void print_dr_stats(dr_stats_t* dr_stats, FILE* output)
{
    fprintf(output, "%.3f\t%.3f\t%.3f\n", dr_stats->dr, dr_stats->second_peak, dr_stats->rms);
}
