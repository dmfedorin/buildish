#include "debug.h"

void loginfo(const char *msg)
{
        printf("LOG: \"%s\"\n", msg);
}

void warning(const char *msg)
{
        printf("WARNING: \"%s\"", msg);
}

void error(const char *msg)
{
        printf("ERROR: \"%s\"\n", msg);
        exit(-1);
}