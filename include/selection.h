/**************************************************************************
* @file selection.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 04 Jun 2020
* @brief
* ************************************************************************/

#ifndef SELECTION_H
#define SELECTION_H

struct DB_playItem_s;

struct selection_s
{
    struct DB_playItem_s **items;
    int items_count;
};

typedef struct selection_s selection_t;

#endif /* SELECTION_H */
