#include "files.h"

uint64_t utl_get_file_size(const char *filepath)
{
        FILE *fp = fopen(filepath, "rb");

        if (fp == NULL)
                perror("file error: ");

        fseek(fp, 0, SEEK_END);

        uint64_t size = ftell(fp);

        fseek(fp, 0, SEEK_SET);
        fclose(fp);

        return size;
}

void utl_read_file(char *dst, const char *filepath)
{
        FILE *fp = fopen(filepath, "rb");
        
        if (fp == NULL)
                perror("file error: ");

        fread(dst, 1, utl_get_file_size(filepath), fp);
        fclose(fp);
}