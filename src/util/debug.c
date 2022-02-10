#include "debug.h"

void utl_log(const char *msg)
{
        printf("LOG: \"%s\"\n", msg);
}

void utl_warning(const char *msg)
{
        printf("WARNING: \"%s\"", msg);
}

void utl_error(const char *msg)
{
        printf("ERROR: \"%s\"\n", msg);
        exit(-1);
}