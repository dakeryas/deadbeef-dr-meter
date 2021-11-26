/**************************************************************************
* @file filepath.h
* @author Valérian Sibille <vsibille@mit.edu>
* @date 25 May 2021
* @brief Dumb function to extract a relative path
* ************************************************************************/

#ifndef DR_PLUGIN_FILEPATH_H
#define DR_PLUGIN_FILEPATH_H

const char* get_filename(const char* path);
unsigned get_folder_length(const char* path);

#endif /* DR_PLUGIN_FILEPATH_H */
