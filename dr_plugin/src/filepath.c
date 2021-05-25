#include "filepath.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const char* get_filename(const char* path)
{
    const char* filename = strrchr(path, '/');
    return filename ? filename+1 : path;
}

int sprint_dirname(char* dirname, const char* path)
{
    const char* filename = strrchr(path, '/');
    if(filename)
    {
        size_t dir_length = filename - path + 1;
        return snprintf(dirname, dir_length, "%s", path);
    }
    else return sprintf(dirname, "%s", path);
}
