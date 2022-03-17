#include "lang/parse.h"

#include "util/debug.h"
#include <stdio.h>

#define ERROR_UNEXPECTED_TOKEN "unexpected token"
#define ERROR_UNHANDLED_TOKEN "unhandled token encountered"

const struct ast_node *ast_node_child(const struct ast_node *node, int ind)
{
        return array_list_elem(&node->children, ind);
}

const struct token *ast_node_token(const struct ast_node *node, int ind)
{
        return array_list_elem(&node->toks, ind);
}

void init_ast(struct ast_node *root)
{
        root->type = AST_NODE_TYPE_ROOT;
        init_array_list(&root->toks, sizeof(struct token));
        init_array_list(&root->children, sizeof(struct ast_node));
}

static void clean_node(struct ast_node *node)
{
        for (int i = 0; i < node->children.size; ++i) {
                struct ast_node *children = node->children.data;
                clean_node(&children[i]);
        }
        clean_array_list(&node->toks);
        clean_array_list(&node->children);
}

void clean_ast(struct ast_node *root)
{
        clean_node(root);
}

static inline const struct token *prev_token(const struct array_list *toks,
                                             int *tokind)
{
        --*tokind;
        return array_list_elem(toks, *tokind);
}

static inline const struct token *current_token(const struct array_list *toks,
                                                int tokind)
{
        return array_list_elem(toks, tokind);
}

static inline const struct token *next_token(const struct array_list *toks,
                                             int *tokind)
{
        ++*tokind;
        return array_list_elem(toks, *tokind);
}

static inline const struct token *peek_token(const struct array_list *toks,
                                             int tokind)
{
        return array_list_elem(toks, tokind + 1);
}

static inline const void parser_error(const struct token *tok,
                                      const char *msg)
{
        printf("[L:%d] ", tok->line);
        error(ERROR_UNEXPECTED_TOKEN);
}

static inline const void expect(const struct array_list *toks, int *tokind,
                                enum token_type type)
{
        if (next_token(toks, tokind)->type != type) {
                parser_error(current_token(toks, *tokind),
                             ERROR_UNEXPECTED_TOKEN);
        }
}

static struct ast_node *add_ast_node_child(struct ast_node *parent,
                                           enum ast_node_type type)
{
        struct ast_node child = {
                .type = type,
        };
        init_array_list(&child.toks, sizeof(struct token));
        init_array_list(&child.children, sizeof(struct ast_node));
        add_array_list_elem(&parent->children, &child);
        struct ast_node *children = parent->children.data;
        return &children[parent->children.size - 1];
}

static inline void add_ast_node_token(struct ast_node *node,
                                      const struct token *tok)
{
        add_array_list_elem(&node->toks, tok);
}

/* extracts every meaningful token between parentheses onto a node */
static void extract_paren_tokens(struct ast_node *node,
                                 const struct array_list *toks, int *tokind)
{
        while (next_token(toks, tokind)->type != TOKEN_TYPE_RPAREN) {
                switch (current_token(toks, *tokind)->type) {
                case TOKEN_TYPE_IDENTIFIER:
                case TOKEN_TYPE_STRLITERAL:
                case TOKEN_TYPE_NUMLITERAL:
                        add_ast_node_token(node,
                                           current_token(toks, *tokind));
                        break;
                default:
                        parser_error(current_token(toks, *tokind),
                                    ERROR_UNHANDLED_TOKEN);
                        break;
                }
                /* unconditionally expecting would force an unnecessary comma
                 * to be required after the last token
                 */
                if (peek_token(toks, *tokind)->type != TOKEN_TYPE_RPAREN)
                        expect(toks, tokind, TOKEN_TYPE_COMMA);
        }
}

static inline void parse_log(struct ast_node *parent,
                             const struct array_list *toks, int *tokind)
{
        struct ast_node *node = add_ast_node_child(parent, AST_NODE_TYPE_LOG);
        expect(toks, tokind, TOKEN_TYPE_LPAREN);
        extract_paren_tokens(node, toks, tokind);
}

static inline void parse_call(struct ast_node *parent,
                              const struct array_list *toks, int *tokind)
{
        struct ast_node *node = add_ast_node_child(parent,
                                                   AST_NODE_TYPE_CALL);
        expect(toks, tokind, TOKEN_TYPE_LPAREN);
        extract_paren_tokens(node, toks, tokind);
}

static inline void parse_cmd(struct ast_node *parent,
                             const struct array_list *toks, int *tokind)
{
        struct ast_node *node = add_ast_node_child(parent, AST_NODE_TYPE_CMD);
        expect(toks, tokind, TOKEN_TYPE_LPAREN);
        extract_paren_tokens(node, toks, tokind);
}

static void parse_allcmd(struct ast_node *parent,
                         const struct array_list *toks, int *tokind)
{
        struct ast_node *node = add_ast_node_child(parent,
                                                   AST_NODE_TYPE_ALLCMD);
        expect(toks, tokind, TOKEN_TYPE_LPAREN);
        expect(toks, tokind, TOKEN_TYPE_STRLITERAL);
        add_ast_node_token(node, current_token(toks, *tokind)); /* dir */
        expect(toks, tokind, TOKEN_TYPE_COMMA);
        expect(toks, tokind, TOKEN_TYPE_STRLITERAL);
        add_ast_node_token(node, current_token(toks, *tokind)); /* file ext */
        expect(toks, tokind, TOKEN_TYPE_COMMA);
        expect(toks, tokind, TOKEN_TYPE_STRLITERAL);
        add_ast_node_token(node, current_token(toks, *tokind)); /* command */
        expect(toks, tokind, TOKEN_TYPE_RPAREN);
}

static void parse_block(struct ast_node *parent,
                        const struct array_list *toks, int *tokind)
{
        struct ast_node *node = add_ast_node_child(parent,
                                                   AST_NODE_TYPE_BLOCK);
        while (next_token(toks, tokind)->type != TOKEN_TYPE_RBRACE) {
                switch (current_token(toks, *tokind)->type) {
                case TOKEN_TYPE_LBRACE:
                        parse_block(node, toks, tokind);
                        break;
                case TOKEN_TYPE_PERCENTAGE:
                        parse_allcmd(node, toks, tokind);
                        break;
                case TOKEN_TYPE_DOLLAR:
                        parse_cmd(node, toks, tokind);
                        break;
                case TOKEN_TYPE_RANGLE:
                        parse_call(node, toks, tokind);
                        break;
                case TOKEN_TYPE_EXCLAMATION:
                        parse_log(node, toks, tokind);
                        break;
                default:
                        parser_error(current_token(toks, *tokind),
                                     ERROR_UNHANDLED_TOKEN);
                        break;
                }
                expect(toks, tokind, TOKEN_TYPE_SEMICOLON);
        }
}

static void parse_proc(struct ast_node *parent, const struct array_list *toks,
                       int *tokind)
{
        struct ast_node *node = add_ast_node_child(parent,
                                                   AST_NODE_TYPE_PROC);
        expect(toks, tokind, TOKEN_TYPE_IDENTIFIER);
        add_ast_node_token(node, current_token(toks, *tokind));
        expect(toks, tokind, TOKEN_TYPE_LBRACE);
        parse_block(node, toks, tokind);
}

/* the ast will be written onto the root ast node */
void parse(struct ast_node *root, const struct array_list *toks)
{
        int tokind = 0;
        while (next_token(toks, &tokind)->type != TOKEN_TYPE_EOF) {
                switch (current_token(toks, tokind)->type) {
                case TOKEN_TYPE_AT:
                        parse_proc(root, toks, &tokind);
                        break;
                default:
                        parser_error(current_token(toks, tokind),
                                     ERROR_UNHANDLED_TOKEN);
                        break;
                }
        }
}

static const char *ast_node_type_to_str(enum ast_node_type type)
{
        switch (type) {
        case AST_NODE_TYPE_ROOT: return "root";
        case AST_NODE_TYPE_PROC: return "procedure";
        case AST_NODE_TYPE_BLOCK: return "block";
        case AST_NODE_TYPE_ALLCMD: return "allcommand";
        case AST_NODE_TYPE_CMD: return "command";
        case AST_NODE_TYPE_CALL: return "call";
        case AST_NODE_TYPE_LOG: return "log";
        default: return "unknown";
        }
}

static void print_ast_node(const struct ast_node *node, int depth)
{
        for (int i = 0; i < depth; ++i)
                printf("\t");
        printf("| %s [", ast_node_type_to_str(node->type));
        for (int i = 0; i < node->toks.size; ++i) {
                const struct token *tok = ast_node_token(node, i);
                printf("%s, ", tok->value);
        }
        printf("]\n");
        for (int i = 0; i < node->children.size; ++i) {
                const struct ast_node *child = ast_node_child(node, i);
                print_ast_node(child, depth + 1);
        }
}

void print_ast(const struct ast_node *root)
{
        print_ast_node(root, 0);
}