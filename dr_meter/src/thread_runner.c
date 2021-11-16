#include <stdlib.h>
#include "thread_runner.h"
#include "thread_data.h"

thread_runner_t make_thread_runner(thread_data_t* thread_data, unsigned threads)
{
    thread_runner_t runner =
    {.thread_data = thread_data, .threads = threads, .next_data_id = 0};
    runner.pids = malloc(runner.threads * sizeof(pthread_t));
    pthread_mutex_init(&runner.mutex, NULL);
    return runner;
}

static unsigned work_items(thread_runner_t* self)
{
    return self->thread_data->items;
}

static int is_data_id_valid(thread_runner_t* self, unsigned data_id)
{
    return data_id < work_items(self);
}

static int is_next_data_id_valid(thread_runner_t* self)
{
    return is_data_id_valid(self, self->next_data_id);
}

static unsigned pop_next_data_id(thread_runner_t* self)
{
    unsigned current_data_id = work_items(self);
    if(is_next_data_id_valid(self))
    {
        current_data_id = self->next_data_id;
        ++self->next_data_id;
    }
    return current_data_id;
}

static thread_datum_t* thread_data(thread_runner_t* self, unsigned data_id)
{
    return &self->thread_data->data[data_id];
}

static void* thread_work(void* pool_arg)
{
    thread_runner_t* pool = (thread_runner_t*)pool_arg;
    while(is_next_data_id_valid(pool))
    {
        pthread_mutex_lock(&pool->mutex);
        unsigned current_data_id = pop_next_data_id(pool);
        pthread_mutex_unlock(&pool->mutex);
        if(is_data_id_valid(pool, current_data_id))
            pool->datum_work(thread_data(pool, current_data_id));
    }
    return NULL;
}

static void create_pool_threads(thread_runner_t* self)
{
    for(unsigned k = 0; k < self->threads; ++k)
        pthread_create(&self->pids[k], NULL, &thread_work, (void*)self);
}

static void join_pool_threads(thread_runner_t* self)
{
    for(unsigned k = 0; k < self->threads; ++k)
        pthread_join(self->pids[k], NULL);
}

void run_work(thread_runner_t* self, datum_work_t datum_work)
{
    if(datum_work)
    {
        self->datum_work = datum_work;
        create_pool_threads(self);
        join_pool_threads(self);
    }
}

void free_thread_runner(thread_runner_t* self)
{
    free(self->pids);
    pthread_mutex_destroy(&self->mutex);
}
