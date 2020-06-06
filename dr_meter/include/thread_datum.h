/**************************************************************************
* @file thread_datum.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 04 Jun 2020
* @brief Hold track input and DR stats output for threads
* ************************************************************************/

#ifndef THREAD_DATUM_H
#define THREAD_DATUM_H

#include "dr_stats.h"

struct thread_datum_s
{
    void* item;
    dr_stats_t dr_stats;
};

typedef struct thread_datum_s thread_datum_t;

#endif /* THREAD_DATUM_H */
