#include "decibels.h"
#include "math.h"

double decibels(double value)
{
    return 20. * log10(value);
}
