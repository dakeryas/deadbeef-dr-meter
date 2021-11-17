/**************************************************************************
* @file thread_runner.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 04 Jun 2020
* @brief Handles thread creation and fills thread_data, works as a pool
* ************************************************************************/

#ifndef THREAD_RUNNER_H
#define THREAD_RUNNER_H

#include <pthread.h>

struct thread_datum_s;
struct thread_data_s;

typedef void (*datum_work_t)(struct thread_datum_s* thread_datum);

struct thread_runner_s
{
    struct thread_data_s* thread_data;
    unsigned threads;
    pthread_t* pids;
    pthread_mutex_t mutex;
    unsigned next_data_id; //id of next thread_datum to process with datum_work
    datum_work_t datum_work; //function to apply to each thread_datum
};

typedef struct thread_runner_s thread_runner_t;

thread_runner_t make_thread_runner(struct thread_data_s* thread_data, unsigned threads);
void run_work(thread_runner_t* runner, const datum_work_t datum_work);
void free_thread_runner(thread_runner_t* runner);

#endif /* THREAD_RUNNER_H */
