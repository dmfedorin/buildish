#include "term/opts.h"

#include <string.h>

void init_opts(struct array_list *opts)
{
        init_array_list(opts, sizeof(char *));
}

void clean_opts(struct array_list *opts)
{
        clean_array_list(opts);
}

void get_opts(struct array_list *opts, int argc, const char *argv[])
{
        for (int i = 1; i < argc; i++)
                add_array_list_elem(opts, argv[i]);
}

bool opts_contain(const struct array_list *opts, const char *opt)
{
        for (int i = 0; i < opts->size; i++) {
                if (strcmp(opt, array_list_elem(opts, i)) == 0)
                        return true;
        }

        return false;
}