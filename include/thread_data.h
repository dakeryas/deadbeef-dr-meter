/**************************************************************************
* @file thread_data.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 04 Jun 2020
* @brief Fancier array of thread_datum for all selected tracks
* ************************************************************************/

#ifndef THREAD_DATA_H
#define THREAD_DATA_H

#include <pthread.h>

struct thread_datum_s;
struct selection_s;
struct dr_stats_s;

struct thread_data_s
{
    unsigned items;
    struct thread_datum_s* data;
};

typedef struct thread_data_s thread_data_t;

thread_data_t make_thread_data(struct selection_s* selection);
struct dr_stats_s* get_dr_stats(thread_data_t* thread_data, unsigned item_id);
void free_thread_data(thread_data_t* thread_data);

#endif /* THREAD_DATA_H */
