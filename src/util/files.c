#include "util/files.h"

#include "util/debug.h"
#include <stdio.h>

#define ERROR_NO_FILE "attempting to open nonexistant file"

int file_size(const char *filepath)
{
        FILE *fp = fopen(filepath, "rb");
        if (fp == NULL)
                error(ERROR_NO_FILE);
                
        fseek(fp, 0, SEEK_END);
        int size = ftell(fp);

        fseek(fp, 0, SEEK_SET);
        fclose(fp);
        return size;
}

void read_file(char *dst, const char *filepath)
{
        FILE *fp = fopen(filepath, "rb");
        if (fp == NULL)
                error(ERROR_NO_FILE);
        fread(dst, 1, file_size(filepath), fp);
        fclose(fp);
}

const char *file_ext(const char *filename)
{
        const char *c = filename;
        while (*c != '.' && *c != '\0')
                ++c;
        return c;
}