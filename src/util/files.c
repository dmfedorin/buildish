#include "util/files.h"

#include "util/debug.h"
#include <stdio.h>

#define ERRNOFILE "attempting to open nonexistant file"

int filesize(const char *filepath)
{
        FILE *fp = fopen(filepath, "rb");

        if (fp == NULL)
                error(ERRNOFILE);

        fseek(fp, 0, SEEK_END);

        int size = ftell(fp);

        fseek(fp, 0, SEEK_SET);
        fclose(fp);

        return size;
}

void readfile(char *dst, const char *filepath)
{
        FILE *fp = fopen(filepath, "rb");
        
        if (fp == NULL)
                error(ERRNOFILE);

        fread(dst, 1, filesize(filepath), fp);
        fclose(fp);
}

const char *fileext(const char *filename)
{
        const char *c = filename;

        while (*c != '.' && *c != '\0')
                c++;

        return c;
}