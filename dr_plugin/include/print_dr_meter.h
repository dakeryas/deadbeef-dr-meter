/**************************************************************************
* @file print_dr_meter.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 03 Jun 2020
* @brief print implementation for struct dr_meter_s
* ************************************************************************/

#ifndef PRINT_DR_METER_H
#define PRINT_DR_METER_H

#include <stdio.h>

struct dr_meter_s;

void print_dr_meter_impl(struct dr_meter_s* dr_meter, FILE* output);

#endif /* PRINT_DR_METER_H */
