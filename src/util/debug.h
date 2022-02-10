#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

void utl_log(const char *msg);

void utl_warning(const char *msg);

void utl_error(const char *msg);

#endif