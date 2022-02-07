#include "parse.h"

void lang_init_ast(struct lang_ast_node *root)
{
        root->type = LANG_AST_NODE_TYPE_ROOT;

        utl_init_array_list(&root->toks, sizeof(struct lang_tok));
        utl_init_array_list(&root->children, sizeof(struct lang_ast_node));
}

void lang_clean_ast(struct lang_ast_node *root)
{
        // TODO: implement ast cleaning
}

static const char *to_string(enum lang_ast_node_type type)
{
        switch (type) {
        case LANG_AST_NODE_TYPE_ROOT: return "LANG_AST_NODE_TYPE_ROOT";

        default: return "UNKNOWN";
        }
}

static void print_node(struct lang_ast_node *node, uint32_t depth)
{
        for (uint32_t i = 0; i < depth; i++)
                printf("\t");

        printf("| %s [", to_string(node->type));

        for (uint32_t i = 0; i < node->toks.size; i++) {
                struct lang_tok *tok = utl_get_array_list_elem(&node->toks,
                                                               i);

                printf("%s, ", tok->value);
        }

        printf("]\n");

        for (uint32_t i = 0; i < node->children.size; i++) {
                struct lang_ast_node *node
                        = utl_get_array_list_elem(&node->children, i);

                print_node(node, depth + 1);
        }
}

void lang_print_ast(struct lang_ast_node *root)
{
        print_node(root, 0);
}