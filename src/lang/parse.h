#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include "lex.h"

#include "../util/util.h"
#include "../util/debug.h"
#include "../util/arraylist.h"

#define LANG_PARSE_ERROR_UNEXPECTED "unexpected token"
#define LANG_PARSE_ERROR_UNHANDLED  "unhandled token encountered"

/*
add node types by order of how they use eachother

root is at the top, as it contains everything in the ast
procedure is above block, as it can contain blocks but blocks cant contain it
block is above command, as commands appear inside of blocks but not vice versa

node types of equal order should be next to each other

in parse.c, define the functions for parsing node types in the reverse order
*/

enum lang_ast_node_type {
        LANG_AST_NODE_TYPE_ROOT,
        LANG_AST_NODE_TYPE_PROCEDURE,
        LANG_AST_NODE_TYPE_BLOCK,
        LANG_AST_NODE_TYPE_COMMAND,
        LANG_AST_NODE_TYPE_CALL,
};

struct lang_ast_node {
        enum lang_ast_node_type type;
        struct utl_array_list toks, children;
};

void lang_init_ast(struct lang_ast_node *root);

void lang_clean_ast(struct lang_ast_node *root);

void lang_parse(struct lang_ast_node *root,
                const struct utl_array_list *toks);

void lang_print_ast(const struct lang_ast_node *root);

#endif