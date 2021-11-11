/**************************************************************************
* @file thread_runner.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 04 Jun 2020
* @brief Handles thread creation and fills thread_data
* ************************************************************************/

#ifndef THREAD_RUNNER_H
#define THREAD_RUNNER_H

#include <pthread.h>

struct thread_data_s;

typedef void* (*thread_worker_t)(void* thread_datum);

struct thread_runner_s
{
    unsigned threads;
    struct thread_data_s* thread_data;
    thread_worker_t thread_worker; //function to apply to each thread_datum
    pthread_mutex_t mutex;
    pthread_cond_t no_item_left;
    unsigned next_data_id; //id of next thread_datum to process with thread_worker
};

typedef struct thread_runner_s thread_runner_t;


thread_runner_t make_thread_runner(struct thread_data_s* thread_data, unsigned threads);
void run_worker(thread_runner_t* runner, thread_worker_t worker);
void free_thread_runner(thread_runner_t* runner);

#endif /* THREAD_RUNNER_H */
