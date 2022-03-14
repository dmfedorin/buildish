#ifndef OPTS_H_q8ocnmd9rzn7jtufhrav3p3n6ga8emwk
#define OPTS_H_q8ocnmd9rzn7jtufhrav3p3n6ga8emwk

#include "util/arraylist.h"
#include <stdbool.h>

void init_opts(struct array_list *opts);
void clean_opts(struct array_list *opts);
void get_opts(struct array_list *opts, int argc, const char *argv[]);
bool opts_contain(const struct array_list *opts, const char *opt);

#endif