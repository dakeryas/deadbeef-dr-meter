#include <stdlib.h>
#include "thread_runner.h"
#include "thread_data.h"
#include "thread_datum.h"

static unsigned number_of_batches(thread_data_t* thread_data, unsigned max_threads)
{
    unsigned batches = thread_data->items / max_threads;
    if(thread_data->items % max_threads) batches += 1;
    return batches;
}

thread_runner_t make_thread_runner(thread_data_t* thread_data, unsigned max_threads)
{
    thread_runner_t runner =
    {.threads = max_threads, .batches = number_of_batches(thread_data, max_threads), .thread_data = thread_data};
    runner.pids = malloc(runner.threads * sizeof(pthread_t));
    return runner;
}

static unsigned first_item_id(thread_runner_t* this, unsigned batch_id)
{
    return batch_id * this->threads;
}

static unsigned last_item_id(thread_runner_t* this, unsigned batch_id)
{
    unsigned next = batch_id + 1;
    return next == this->batches ? this->thread_data->items : next * this->threads;
}

static void join_threads(thread_runner_t* this, int spawned_threads)
{
    for(int j = 0; j < spawned_threads; ++j)
        pthread_join(this->pids[j], NULL);
}

static void run_batch(thread_runner_t* this, thread_worker_t worker, unsigned batch_id)
{
    unsigned first_item = first_item_id(this, batch_id);
    unsigned last_item = last_item_id(this, batch_id);
    for(int k = first_item; k < last_item; ++k)
        pthread_create(&this->pids[k % this->threads], NULL, worker, (void*)&this->thread_data->data[k]);
    join_threads(this, last_item - first_item);
}

void run_batches(thread_runner_t* this, thread_worker_t worker)
{
    for(unsigned batch_id = 0; batch_id < this->batches; ++batch_id)
        run_batch(this, worker, batch_id);
}

void free_thread_runner(thread_runner_t* this)
{
    free(this->pids);
}
