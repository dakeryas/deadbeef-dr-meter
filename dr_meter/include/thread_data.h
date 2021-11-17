/**************************************************************************
* @file thread_data.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 04 Jun 2020
* @brief Fancier array of thread_datum for all selected tracks
* ************************************************************************/

#ifndef THREAD_DATA_H
#define THREAD_DATA_H

#include <pthread.h>
#include "thread_datum.h"

struct selection_s;
struct dr_stats_s;

struct thread_data_s
{
    unsigned items; //length of 'data', i.e. numbef of tracks
    thread_datum_t* data;
};

typedef struct thread_data_s thread_data_t;

thread_data_t make_thread_data(struct selection_s* selection);//must free_thread_data struct
thread_data_t* create_thread_data(struct selection_s* selection);//must free_thread_data struct and free pointer
struct dr_stats_s* get_dr_stats(thread_data_t* thread_data, unsigned item_id);
void free_thread_data(thread_data_t* thread_data);

#endif /* THREAD_DATA_H */
