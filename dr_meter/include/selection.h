/**************************************************************************
* @file selection.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 04 Jun 2020
* @brief Hold selected playlist items
* ************************************************************************/

#ifndef SELECTION_H
#define SELECTION_H

struct selection_s
{
    void** items;
    int items_count;
};

typedef struct selection_s selection_t;

#endif /* SELECTION_H */
