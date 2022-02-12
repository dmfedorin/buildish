#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

void loginfo(const char *msg);

void warning(const char *msg);

void error(const char *msg);

#endif