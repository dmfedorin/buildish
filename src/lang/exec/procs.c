#include "procs.h"

static inline __attribute__ ((always_inline))
const struct tok *get_proc_node_name(const struct ast_node *node)
{
        return get_array_list_elem(&node->children, 0);
}

// requires all procedure nodes to be children of the root node
void find_procs(struct array_list *procs, const struct ast_node *root)
{
        for (uint32_t i = 0; i < root->children.size; i++) {
                const struct ast_node *node
                        = get_array_list_elem(&root->children, i);

                if (node->type != AST_NODE_TYPE_PROCEDURE)
                        continue;

                struct proc proc = {
                        .name = get_proc_node_name(node)->value,
                        .node = node
                };

                add_array_list_elem(procs, &proc);
        }
}

const struct ast_node *get_proc_node(const struct array_list *procs,
                                     const char *name)
{
        for (uint32_t i = 0; i < procs->size; i++) {
                const struct proc *proc = get_array_list_elem(procs, i);

                if (strcmp(proc->name, name) == 0)
                        return proc->node;
        }

        return NULL;
}