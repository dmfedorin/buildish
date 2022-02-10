#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED

#include <stdint.h>
#include <stdio.h>

uint64_t get_file_size(const char *filepath);

void read_file(char *dst, const char *filepath);

#endif