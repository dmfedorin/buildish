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
void find_procs(struct array_list *procs, const struct ast_node *root);

const struct ast_node *get_proc_node(const struct array_list *procs,
                                     const char *name);

#endif