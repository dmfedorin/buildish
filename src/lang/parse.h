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

enum astnodetype {
        ANT_ROOT,
        ANT_PROC,
        ANT_BLOCK,
        ANT_ALLCMD,
        ANT_CMD,
        ANT_CALL,
        ANT_LOG,
};

struct astnode {
        enum astnodetype type;
        struct arraylist toks, children;
};

const struct astnode *getchild(const struct astnode *node, int ind);

const struct tok *gettok(const struct astnode *node, int ind);

void initast(struct astnode *root);

void cleanast(struct astnode *root);

void parse(struct astnode *root, const struct arraylist *toks);

void printast(const struct astnode *root);

#endif