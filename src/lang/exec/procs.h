#ifndef PROCS_H_INCLUDED
#define PROCS_H_INCLUDED

#include "../lex.h"
#include "../parse.h"
#include "../../util/arraylist.h"

#include <stdint.h>
#include <string.h>

struct proc {
        const char *name;
        const struct ast_node *node;
};

// requires all procedure nodes to be children of the root node
void findprocs(struct arraylist *procs, const struct astnode *root);

const struct astnode *getprocnode(const struct arraylist *procs,
                                  const char *name);

#endif