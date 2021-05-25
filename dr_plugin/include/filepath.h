/**************************************************************************
* @file filepath.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 25 May 2021
* @brief Dumb function to extract a relative path
* ************************************************************************/

#ifndef DR_METER_FILEPATH_H
#define DR_METER_FILEPATH_H

const char* get_filename(const char* path);
int sprint_dirname(char* dirname, const char* path);

#endif /* DR_METER_FILEPATH_H */
