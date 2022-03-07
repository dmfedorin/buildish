#include "term/opts.h"

#include <string.h>

void initopts(struct arraylist *opts)
{
        inital(opts, sizeof(char *));
}

void cleanopts(struct arraylist *opts)
{
        cleanal(opts);
}

void getopts(struct arraylist *opts, int argc, const char *argv[])
{
        for (int i = 1; i < argc; i++)
                addalelem(opts, argv[i]);
}

bool optscont(const struct arraylist *opts, const char *opt)
{
        for (int i = 0; i < opts->size; i++) {
                if (strcmp(opt, getalelem(opts, i)) == 0)
                        return true;
        }

        return false;
}