#ifndef OPTS_H_165737
#define OPTS_H_165737

#include "util/arraylist.h"
#include <stdbool.h>

void init_opts(struct array_list *opts);

void clean_opts(struct array_list *opts);

void get_opts(struct array_list *opts, int argc, const char *argv[]);

bool opts_contain(const struct array_list *opts, const char *opt);

#endif