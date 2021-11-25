/**************************************************************************
* @file print_dr_meter.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 03 Jun 2020
* @brief print implementation for struct dr_meter_s
* ************************************************************************/

#ifndef DR_METER_PRINT_DR_METER_H
#define DR_METER_PRINT_DR_METER_H

#include <stdio.h>

struct dr_meter_s;

void print_dr_meter_impl(struct dr_meter_s* dr_meter, FILE* output);

#endif /* DR_METER_PRINT_DR_METER_H */
