#include "procs.h"

// requires all procedure nodes to be children of the root node
void findprocs(struct arraylist *procs, const struct astnode *root)
{
        for (int i = 0; i < root->children.size; i++) {
                const struct astnode *node = getalelem(&root->children, i);

                if (node->type != ANT_PROC)
                        continue;

                const struct tok *name = getalelem(&node->toks, 0);

                struct proc proc = {
                        .name = name->value, .node = node,
                };

                addalelem(procs, &proc);
        }
}

const struct astnode *getprocnode(const struct arraylist *procs,
                                  const char *name)
{
        for (int i = 0; i < procs->size; i++) {
                const struct proc *proc = getalelem(procs, i);

                if (strcmp(proc->name, name) == 0)
                        return proc->node;
        }

        return NULL;
}