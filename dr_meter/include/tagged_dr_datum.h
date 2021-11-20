/**************************************************************************
* @file tagged_dr_datum.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 04 Jun 2020
* @brief Hold track input and DR stats output for threads
* ************************************************************************/

#ifndef TAGGED_DR_DATUM_H
#define TAGGED_DR_DATUM_H

#include "dr_stats.h"

struct track_s;
typedef struct track_s track_t;

struct tagged_dr_datum_s
{
    track_t* item;
    dr_stats_t dr_stats;
};

typedef struct tagged_dr_datum_s tagged_dr_datum_t;

#endif /* TAGGED_DR_DATUM_H */
