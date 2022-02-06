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