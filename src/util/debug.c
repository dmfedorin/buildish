#include "util/debug.h"

#include <stdio.h>
#include <stdlib.h>

void log_info(const char *msg)
{
        printf("log: %s\n", msg);
}

void warning(const char *msg)
{
        printf("warn: %s\n", msg);
}

void error(const char *msg)
{
        printf("err: %s\n", msg);
        exit(-1);
}