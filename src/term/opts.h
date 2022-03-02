#ifndef OPTS_H_INCLUDED
#define OPTS_H_INCLUDED

#include "../util/arraylist.h"

#include <stdbool.h>
#include <string.h>

void initopts(struct arraylist *opts);

void cleanopts(struct arraylist *opts);

void getopts(struct arraylist *opts, int argc, const char *argv[]);

bool optscont(const struct arraylist *opts, const char *opt);

#endif