#include "filepath.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const char* get_filename(const char* path)
{
    const char* filename = strrchr(path, '/');
    return filename ? filename+1 : path;
}

int snprint_dirname(char* dirname, unsigned dirname_size, const char* path)
{
    const char* filename = strrchr(path, '/');
    if(filename)
    {
        size_t dir_length = filename - path + 1;
        size_t length = dir_length < dirname_size ? dir_length : dirname_size;
        return snprintf(dirname, length, "%s", path);
    }
    else return sprintf(dirname, "%s", path);
}
