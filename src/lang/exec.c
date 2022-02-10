#include "exec.h"

static void execute_call(const struct ast_node *node)
{
}

static void execute_command(const struct ast_node *node)
{
}

static void execute_block(const struct ast_node *node)
{
        for (uint32_t i = 0; i < node->children.size; i++) {
                const struct ast_node *child
                        = get_array_list_elem(&node->children, i);

                switch (child->type) {
                case AST_NODE_TYPE_BLOCK:
                        execute_block(child);
                        break;

                case AST_NODE_TYPE_COMMAND:
                        execute_command(child);
                        break;

                case AST_NODE_TYPE_CALL:
                        execute_call(child);
                        break;
                }
        }
}

static void execute_procedure(const struct ast_node *node)
{
        // the first child of a procedure is a block
        execute_block(get_array_list_elem(&node->children, 0));
}

void execute(const struct ast_node *root)
{
}