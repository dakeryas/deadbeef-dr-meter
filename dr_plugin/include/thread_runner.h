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

struct thread_runner_s
{
    unsigned threads;
    unsigned batches;
    pthread_t* pids;
    struct thread_data_s* thread_data;
};

typedef struct thread_runner_s thread_runner_t;

typedef void* (*thread_worker_t)(void* thread_datum);

thread_runner_t make_thread_runner(struct thread_data_s* thread_data, unsigned max_threads);
void run_batches(thread_runner_t* runner, thread_worker_t worker);
void free_thread_runner(thread_runner_t* runner);

#endif /* THREAD_RUNNER_H */
