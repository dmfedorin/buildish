#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include "lex.h"

#include "../util/arraylist.h"

enum lang_ast_node_type {
        LANG_AST_NODE_TYPE_ROOT,
};

struct lang_ast_node {
        enum lang_ast_node_type type;
        struct utl_array_list toks, children;
};

void lang_init_ast(struct lang_ast_node *root);

void lang_clean_ast(struct lang_ast_node *root);

void lang_print_ast(struct lang_ast_node *root);

#endif