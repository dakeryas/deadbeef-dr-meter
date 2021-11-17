#include <stdlib.h>
#include "thread_runner.h"
#include "tagged_dr_data.h"

static unsigned get_number_of_threads(const tagged_dr_data_t* tagged_dr_data, unsigned threads)
{
    return threads < tagged_dr_data->items ? threads : tagged_dr_data->items;
}

thread_runner_t make_thread_runner(tagged_dr_data_t* tagged_dr_data, unsigned threads)
{
    thread_runner_t runner =
    {.tagged_dr_data = tagged_dr_data, .threads = get_number_of_threads(tagged_dr_data, threads)};
    runner.pids = malloc(runner.threads * sizeof(pthread_t));
    pthread_mutex_init(&runner.mutex, NULL);
    return runner;
}

static unsigned work_items(thread_runner_t* self)
{
    return self->tagged_dr_data->items;
}

static void reset_next_data_id(thread_runner_t* self)
{
    self->next_data_id = 0;
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
    return self->next_data_id++;
}

static tagged_dr_datum_t* tagged_dr_data(thread_runner_t* self, unsigned data_id)
{
    return &self->tagged_dr_data->data[data_id];
}

static void* thread_work(void* runner_arg)
{
    thread_runner_t* runner = (thread_runner_t*)runner_arg;
    while(is_next_data_id_valid(runner))
    {
        pthread_mutex_lock(&runner->mutex);
        unsigned current_data_id = pop_next_data_id(runner);
        pthread_mutex_unlock(&runner->mutex);
        if(is_data_id_valid(runner, current_data_id))
            runner->datum_work(tagged_dr_data(runner, current_data_id));
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

void run_work(thread_runner_t* self, const datum_work_t datum_work)
{
    if(datum_work)
    {
        self->datum_work = datum_work;
        reset_next_data_id(self);
        create_pool_threads(self);
        join_pool_threads(self);
    }
}

void free_thread_runner(thread_runner_t* self)
{
    free(self->pids);
    pthread_mutex_destroy(&self->mutex);
}
