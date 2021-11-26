#include "filepath.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const char* get_filename(const char* path)
{
    const char* filename = strrchr(path, '/');
    return filename ? filename+1 : path;
}

unsigned get_folder_length(const char* path)
{
    const char* filename = strrchr(path, '/');
    if(filename) return filename - path;
    else return strlen(path) - 1;
}

unsigned sprint_dirname(const char* path, char* begin)
{
    unsigned folder_length = get_folder_length(path);
    strncpy(begin, path, folder_length);
    return folder_length;
}
