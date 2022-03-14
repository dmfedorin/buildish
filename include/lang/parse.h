#ifndef PARSE_H_iym1j7lg387ffg46iykif8ozfxfet5ub
#define PARSE_H_iym1j7lg387ffg46iykif8ozfxfet5ub

#include "util/arraylist.h"
#include "lang/lex.h"

/* add node types by order of how they use eachother
 *
 * root is at the top, as it contains everything in the ast
 * procedure is above block, as it can contain blocks but blocks cant contain
 * it. block is above command, as commands appear inside of blocks but not
 * vice versa
 * 
 * node types of equal order should be next to each other
 * 
 * in parse.c, define the functions for parsing node types in the reverse
 * order
 */

enum ast_node_type {
        AST_NODE_TYPE_ROOT,
        AST_NODE_TYPE_PROC,
        AST_NODE_TYPE_BLOCK,
        AST_NODE_TYPE_ALLCMD,
        AST_NODE_TYPE_CMD,
        AST_NODE_TYPE_CALL,
        AST_NODE_TYPE_LOG,
};

struct ast_node {
        enum ast_node_type type;
        struct array_list toks, children;
};

const struct ast_node *ast_node_child(const struct ast_node *node, int ind);
const struct token *ast_node_token(const struct ast_node *node, int ind);
void init_ast(struct ast_node *root);
void clean_ast(struct ast_node *root);

/* the ast will be written onto the root ast node */
void parse(struct ast_node *root, const struct array_list *toks);

void print_ast(const struct ast_node *root);

#endif