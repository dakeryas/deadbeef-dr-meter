#include <stdlib.h>
#include "thread_runner.h"
#include "thread_data.h"

thread_runner_t make_thread_runner(thread_data_t* thread_data, unsigned threads)
{
    thread_runner_t runner =
    {.threads = threads, .thread_data = thread_data, .next_data_id = 0};
    pthread_mutex_init(&runner.mutex, NULL);
    pthread_cond_init(&runner.no_item_left, NULL);
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

static unsigned get_next_data_id(thread_runner_t* self)
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

static void* pool_worker(void* pool_arg)
{
    thread_runner_t* pool = (thread_runner_t*)pool_arg;
    while(1)
    {
        pthread_mutex_lock(&pool->mutex);
        unsigned current_data_id = get_next_data_id(pool);
        pthread_mutex_unlock(&pool->mutex);
        if(is_data_id_valid(pool, current_data_id))
            pool->thread_worker(thread_data(pool, current_data_id));//should the shared thead_worker be copied?
        else pthread_cond_signal(&pool->no_item_left);
    }
    return NULL;
}

static void create_pool_threads(thread_runner_t* self)
{
    for(unsigned k = 0; k < self->threads; ++k)
    {
        pthread_t current_pid;
        pthread_create(&current_pid, NULL, &pool_worker, (void*)self);
        pthread_detach(current_pid);
    }
}

static void wait_work_end(thread_runner_t* self)
{
    pthread_mutex_lock(&self->mutex);
    while(is_next_data_id_valid(self))
        pthread_cond_wait(&self->no_item_left, &self->mutex);
    pthread_mutex_unlock(&self->mutex);
}

void run_worker(thread_runner_t* self, thread_worker_t worker)
{
    if(worker)
    {
        self->thread_worker = worker;
        create_pool_threads(self);
        wait_work_end(self);
    }
}

void free_thread_runner(thread_runner_t* self)
{
    pthread_mutex_destroy(&self->mutex);
    pthread_cond_destroy(&self->no_item_left);
}
