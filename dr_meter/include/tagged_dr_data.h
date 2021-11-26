/**************************************************************************
* @file tagged_dr_data.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 04 Jun 2020
* @brief Fancier array of tagged_dr_datum for all selected tracks
* ************************************************************************/

#ifndef DR_METER_TAGGED_DR_DATA_H
#define DR_METER_TAGGED_DR_DATA_H

#include <pthread.h>
#include "tagged_dr_datum.h"

struct selection_s;
struct dr_stats_s;

struct tagged_dr_data_s
{
    unsigned items; //length of 'data', i.e. numbef of tracks
    tagged_dr_datum_t* data;
};

typedef struct tagged_dr_data_s tagged_dr_data_t;

tagged_dr_data_t make_tagged_dr_data(const struct selection_s* selection);//must free_tagged_dr_data_members
tagged_dr_data_t* create_tagged_dr_data(const struct selection_s* selection);//must free_tagged_dr_data
struct dr_stats_s* get_dr_stats(tagged_dr_data_t* tagged_dr_data, unsigned item_id);
void free_tagged_dr_data_members(tagged_dr_data_t* tagged_dr_data);
void free_tagged_dr_data(tagged_dr_data_t* tagged_dr_data);

#endif /* DR_METER_TAGGED_DR_DATA_H */
