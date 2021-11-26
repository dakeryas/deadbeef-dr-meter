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
    if(filename) return filename - path + 1;
    else return strlen(path);
}
