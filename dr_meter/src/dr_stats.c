#include "dr_stats.h"
#include "math.h"
#include "decibels.h"

static const char* DR_FORMAT = "DR%-2.0f %7.2f dB %9.2f dB";
static const int DR_FORMAT_LENGTH = 28; //> than actual DR_FORMAT length

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
    return sprintf(buffer, DR_FORMAT, rint(stats.dr), stats.peak, stats.rms);
}

int print_dr_stats(dr_stats_t* dr_stats, FILE* output)
{
    char buffer[DR_FORMAT_LENGTH];
    int written_bytes = print_buffer_dr_stats(dr_stats, buffer);
    if(written_bytes > DR_FORMAT_LENGTH)
        fprintf(stderr, "print_dr_stats: %i bytes in %i-wide buffer!\n", written_bytes, DR_FORMAT_LENGTH);
    return fprintf(output, "%s", buffer);
}

int printl_dr_stats(dr_stats_t* dr_stats, FILE* output)
{
    int written_bytes = print_dr_stats(dr_stats, output);
    return written_bytes + fprintf(output, "\n");
}
