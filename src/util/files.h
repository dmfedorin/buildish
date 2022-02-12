#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED

#include <stdint.h>
#include <stdio.h>

uint64_t filesize(const char *filepath);

void rdfile(char *dst, const char *filepath);

#endif