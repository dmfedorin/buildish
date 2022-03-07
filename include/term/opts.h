#ifndef OPTSH_165737
#define OPTSH_165737

#include "util/arraylist.h"
#include <stdbool.h>

void initopts(struct arraylist *opts);

void cleanopts(struct arraylist *opts);

void getopts(struct arraylist *opts, int argc, const char *argv[]);

bool optscont(const struct arraylist *opts, const char *opt);

#endif