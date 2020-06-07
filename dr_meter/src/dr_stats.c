#include "dr_stats.h"
#include "math.h"
#include "decibels.h"

static const char* DR_FORMAT = "DR %-8.3f %7.2f dB %9.2f dB";

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

int print_buffer_dr_stats(dr_stats_t* dr_stats, char* buffer)
{
    dr_stats_t stats = make_dB_dr_stats(dr_stats);
    return sprintf(buffer, DR_FORMAT, stats.dr, stats.peak, stats.rms);
}

int print_dr_stats(dr_stats_t* dr_stats, FILE* output)
{
    dr_stats_t stats = make_dB_dr_stats(dr_stats);
    return fprintf(output, DR_FORMAT, stats.dr, stats.peak, stats.rms);
}

int printl_dr_stats(dr_stats_t* dr_stats, FILE* output)
{
    int written_bytes = print_dr_stats(dr_stats, output);
    return written_bytes + fprintf(output, "\n");
}
