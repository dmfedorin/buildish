#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include "lex.h"

#include "../util/util.h"
#include "../util/debug.h"
#include "../util/arraylist.h"

/*
add node types by order of how they use eachother

root is at the top, as it contains everything in the ast
procedure is above block, as it can contain blocks but blocks cant contain it
block is above command, as commands appear inside of blocks but not vice versa

node types of equal order should be next to each other

in parse.c, define the functions for parsing node types in the reverse order
*/

enum ast_node_type {
        AST_NODE_TYPE_ROOT,
        AST_NODE_TYPE_PROCEDURE,
        AST_NODE_TYPE_BLOCK,
        AST_NODE_TYPE_COMMAND,
        AST_NODE_TYPE_CALL,
};

struct ast_node {
        enum ast_node_type type;
        struct array_list toks, children;
};

void init_ast(struct ast_node *root);

void clean_ast(struct ast_node *root);

void parse(struct ast_node *root, const struct array_list *toks);

void print_ast(const struct ast_node *root);

#endif