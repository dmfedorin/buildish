#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED

#include <stdint.h>
#include <stdio.h>

int filesize(const char *filepath);

void rdfile(char *dst, const char *filepath);

#endif