#include "parse.h"

#define ERRUNEXPECTED "unexpected token"
#define ERRUNHANDLED  "unhandled token encountered"

void initast(struct astnode *root)
{
        root->type = ANT_ROOT;

        inital(&root->toks, sizeof(struct tok));
        inital(&root->children, sizeof(struct astnode));
}

static void cleannode(struct astnode *node)
{
        for (int i = 0; i < node->children.size; i++) {
                struct astnode *children = node->children.data;
                cleannode(&children[i]);
        }

        cleanal(&node->toks);
        cleanal(&node->children);
}

void cleanast(struct astnode *root)
{
        cleannode(root);
}

static inline const struct tok *prevtok(const struct arraylist *toks,
                                        int *tokind)
{
        (*tokind)--;
        return getalelem(toks, *tokind);
}

static inline const struct tok *curtok(const struct arraylist *toks,
                                       int tokind)
{
        return getalelem(toks, tokind);
}

static inline const struct tok *nexttok(const struct arraylist *toks,
                                        int *tokind)
{
        (*tokind)++;
        return getalelem(toks, *tokind);
}

static inline const struct tok *peektok(const struct arraylist *toks,
                                        int tokind)
{
        return getalelem(toks, tokind + 1);
}

static inline const void parseerr(const struct tok *tok, const char *msg)
{
        printf("[L:%d] ", tok->line);
        error(ERRUNEXPECTED);
}

static inline const void expect(const struct arraylist *toks, int *tokind,
                                enum toktype type)
{
        if (nexttok(toks, tokind)->type != type)
                parseerr(curtok(toks, *tokind), ERRUNEXPECTED);
}

static struct astnode *addchild(struct astnode *parent, enum astnodetype type)
{
        struct astnode child = {
                .type = type,
        };

        inital(&child.toks, sizeof(struct tok));
        inital(&child.children, sizeof(struct astnode));
        addalelem(&parent->children, &child);

        struct astnode *children = parent->children.data;

        return &children[parent->children.size - 1];
}

void addtok(struct astnode *node, const struct tok *tok)
{
        addalelem(&node->toks, tok);
}

static void parselog(struct astnode *parent, const struct arraylist *toks,
                     int *tokind)
{
        struct astnode *node = addchild(parent, ANT_LOG);

        expect(toks, tokind, TT_LPAREN);
        expect(toks, tokind, TT_STRLITERAL);

        addtok(node, curtok(toks, *tokind));

        expect(toks, tokind, TT_RPAREN);
}

static void parsecall(struct astnode *parent, const struct arraylist *toks,
                      int *tokind)
{
        struct astnode *node = addchild(parent, ANT_CALL);

        expect(toks, tokind, TT_LPAREN);
        expect(toks, tokind, TT_IDENTIFIER);

        addtok(node, curtok(toks, *tokind));

        expect(toks, tokind, TT_RPAREN);
}

static void parsecmd(struct astnode *parent, const struct arraylist *toks,
                     int *tokind)
{
        struct astnode *node = addchild(parent, ANT_CMD);

        expect(toks, tokind, TT_LPAREN);
        expect(toks, tokind, TT_STRLITERAL);

        addtok(node, curtok(toks, *tokind));
        
        expect(toks, tokind, TT_RPAREN);
}

static void parseallcmd(struct astnode *parent, const struct arraylist *toks,
                        int *tokind)
{
        struct astnode *node = addchild(parent, ANT_ALLCMD);

        expect(toks, tokind, TT_LPAREN);
        expect(toks, tokind, TT_STRLITERAL);
        
        // directory
        addtok(node, curtok(toks, *tokind));
        
        expect(toks, tokind, TT_COMMA);
        expect(toks, tokind, TT_STRLITERAL);
        
        // command
        addtok(node, curtok(toks, *tokind));
        
        expect(toks, tokind, TT_RPAREN);
}

static void parseblock(struct astnode *parent, const struct arraylist *toks,
                       int *tokind)
{
        struct astnode *node = addchild(parent, ANT_BLOCK);

        while (nexttok(toks, tokind)->type != TT_RBRACE) {
                switch (curtok(toks, *tokind)->type) {
                case TT_LBRACE:
                        parseblock(node, toks, tokind);
                        break;

                case TT_PERCENTAGE:
                        parseallcmd(node, toks, tokind);
                        break;

                case TT_DOLLAR:
                        parsecmd(node, toks, tokind);
                        break;

                case TT_RANGLE:
                        parsecall(node, toks, tokind);
                        break;

                case TT_EXCLAMATION:
                        parselog(node, toks, tokind);
                        break;

                default:
                        parseerr(curtok(toks, *tokind), ERRUNHANDLED);
                }

                expect(toks, tokind, TT_SEMICOLON);
        }
}

static void parseproc(struct astnode *parent, const struct arraylist *toks,
                      int *tokind)
{
        struct astnode *node = addchild(parent, ANT_PROC);

        expect(toks, tokind, TT_IDENTIFIER);

        addtok(node, curtok(toks, *tokind));
        
        expect(toks, tokind, TT_LBRACE);
        parseblock(node, toks, tokind);
}

void parse(struct astnode *root, const struct arraylist *toks)
{
        int tokind = 0;

        while (nexttok(toks, &tokind)->type != TT_EOF) {
                switch (curtok(toks, tokind)->type) {
                case TT_AT:
                        parseproc(root, toks, &tokind);
                        break;

                default:
                        parseerr(curtok(toks, tokind), ERRUNHANDLED);
                }
        }
}

static const char *anttostr(enum astnodetype type)
{
        switch (type) {
        case ANT_ROOT:   return "root";
        case ANT_PROC:   return "procedure";
        case ANT_BLOCK:  return "block";
        case ANT_ALLCMD: return "allcommand";
        case ANT_CMD:    return "command";
        case ANT_CALL:   return "call";
        case ANT_LOG:    return "log";

        default: return "unknown";
        }
}

static void printnode(const struct astnode *node, int depth)
{
        for (int i = 0; i < depth; i++)
                printf("\t");

        printf("| %s [", anttostr(node->type));

        for (int i = 0; i < node->toks.size; i++) {
                const struct tok *tok = getalelem(&node->toks, i);
                printf("%s, ", tok->value);
        }

        printf("]\n");

        for (int i = 0; i < node->children.size; i++) {
                const struct astnode *child = getalelem(&node->children, i);
                printnode(child, depth + 1);
        }
}

void printast(const struct astnode *root)
{
        printnode(root, 0);
}