/**************************************************************************
* @file selection.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 04 Jun 2020
* @brief Hold selected playlist items
* ************************************************************************/

#ifndef DR_METER_SELECTION_H
#define DR_METER_SELECTION_H

struct selection_s
{
    void** items;
    int items_count;
};

typedef struct selection_s selection_t;

#endif /* DR_METER_SELECTION_H */
