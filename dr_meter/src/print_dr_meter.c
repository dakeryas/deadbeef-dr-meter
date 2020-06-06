#include "print_dr_meter.h"
#include "dr_meter.h"

static void print_integers(dr_meter_t* dr_meter, FILE* output)
{
    fprintf(output, "channels        : %i\n", dr_meter->channels);
    fprintf(output, "expected blocks : %i\n", dr_meter->blocks);
    fprintf(output, "analysed blocks : %i\n", dr_meter->_ana_blocks);
    fprintf(output, "analysed samples: %i\n", dr_meter->_ana_samples);
}

static void print_channel_values(const char* header, double* values, unsigned channels, FILE* output)
{
    fprintf(output, "%s:\t", header);
    if(values == NULL)
    {
        for(unsigned cha = 0; cha < channels; ++cha)
            fprintf(output, "%8i\t", cha);
    }
    else
    {
        for(unsigned cha = 0; cha < channels; ++cha)
            fprintf(output, "%f\t", values[cha]);
    }
    fprintf(output, "\n");
}

static void print_sum2(dr_meter_t* dr_meter, FILE* output)
{
    for(unsigned block = 0; block < dr_meter->_ana_blocks; ++block)
    {
        fprintf(output, "sum2 %4i:\t", block);
        for(unsigned cha = 0; cha < dr_meter->channels; ++cha)
            fprintf(output, "%8.2f\t", dr_meter->sum2[cha][block]);
        fprintf(output, "\n");
    }
}

void print_dr_meter_impl(dr_meter_t* dr_meter, FILE* output)
{
    print_integers(dr_meter, output);
    print_channel_values("ch       ", NULL, dr_meter->channels, output);
    print_channel_values("1st peaks", dr_meter->peaks, dr_meter->channels, output);
    print_channel_values("2nd peaks", dr_meter->second_peaks, dr_meter->channels, output);
    print_sum2(dr_meter, output);
}
