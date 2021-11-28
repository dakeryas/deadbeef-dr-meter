/**************************************************************************
* @file compute_dr.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 28 Nov 2021
* @brief Type definition for function filling tagged data with DR stats
* ************************************************************************/

#ifndef DR_PLUGIN_COMPUTE_DR_H
#define DR_PLUGIN_COMPUTE_DR_H

struct tagged_dr_data_s;

typedef int (*compute_dr_t)(struct tagged_dr_data_s* tagged_dr_data);

#endif /* DR_PLUGIN_COMPUTE_DR_H */
