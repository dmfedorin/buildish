#include "procs.h"

static inline __attribute__ ((always_inline))
const struct tok *procnodename(const struct astnode *node)
{
        return getalelem(&node->toks, 0);
}

// requires all procedure nodes to be children of the root node
void findprocs(struct arraylist *procs, const struct astnode *root)
{
        for (uint32_t i = 0; i < root->children.size; i++) {
                const struct astnode *node = getalelem(&root->children, i);

                if (node->type != ANT_PROCEDURE)
                        continue;

                struct proc proc = {
                        .name = procnodename(node)->value,
                        .node = node
                };

                addalelem(procs, &proc);
        }
}

const struct astnode *getprocnode(const struct arraylist *procs,
                                  const char *name)
{
        for (uint32_t i = 0; i < procs->size; i++) {
                const struct proc *proc = getalelem(procs, i);

                if (strcmp(proc->name, name) == 0)
                        return proc->node;
        }

        return NULL;
}