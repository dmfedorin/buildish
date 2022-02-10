#include "parse.h"

void lang_init_ast(struct lang_ast_node *root)
{
        root->type = LANG_AST_NODE_TYPE_ROOT;

        utl_init_array_list(&root->toks, sizeof(struct lang_tok));
        utl_init_array_list(&root->children, sizeof(struct lang_ast_node));
}

static void clean_node(struct lang_ast_node *node)
{
        for (uint32_t i = 0; i < node->children.size; i++) {
                struct lang_ast_node *children = node->children.data;

                clean_node(&children[i]);
        }

        utl_clean_array_list(&node->toks);
        utl_clean_array_list(&node->children);
}

void lang_clean_ast(struct lang_ast_node *root)
{
        clean_node(root);
}

static const struct lang_tok *prev_tok(const struct utl_array_list *toks,
                                       uint32_t *tok_ind)
{
        (*tok_ind)--;
        return utl_get_array_list_elem(toks, *tok_ind);
}

static const struct lang_tok *cur_tok(const struct utl_array_list *toks,
                                      uint32_t tok_ind)
{
        return utl_get_array_list_elem(toks, tok_ind);
}

static const struct lang_tok *next_tok(const struct utl_array_list *toks,
                                       uint32_t *tok_ind)
{
        (*tok_ind)++;
        return utl_get_array_list_elem(toks, *tok_ind);
}

static const struct lang_tok *peek_tok(const struct utl_array_list *toks,
                                       uint32_t tok_ind)
{
        return utl_get_array_list_elem(toks, tok_ind + 1);
}

static const void expect_tok(const struct utl_array_list *toks,
                             uint32_t *tok_ind, enum lang_tok_type type)
{
        if (next_tok(toks, tok_ind)->type != type)
                utl_error(LANG_PARSE_ERROR_UNEXPECTED);
}

static struct lang_ast_node *add_child(struct lang_ast_node *parent,
                                       enum lang_ast_node_type type)
{
        struct lang_ast_node child = {
                .type = type
        };

        utl_init_array_list(&child.toks, sizeof(struct lang_tok));
        utl_init_array_list(&child.children, sizeof(struct lang_ast_node));
        utl_add_array_list_elem(&parent->children, &child);

        struct lang_ast_node *children = parent->children.data;

        return &children[parent->children.size - 1];
}

static void parse_call(struct lang_ast_node *parent,
                       const struct utl_array_list *toks, uint32_t *tok_ind)
{
        struct lang_ast_node *node = add_child(parent,
                                               LANG_AST_NODE_TYPE_CALL);

        expect_tok(toks, tok_ind, LANG_TOK_TYPE_LPAREN);
        expect_tok(toks, tok_ind, LANG_TOK_TYPE_IDENTIFIER);
        utl_add_array_list_elem(&node->toks, cur_tok(toks, *tok_ind));
        expect_tok(toks, tok_ind, LANG_TOK_TYPE_RPAREN);
}

static void parse_command(struct lang_ast_node *parent,
                          const struct utl_array_list *toks,
                          uint32_t *tok_ind)
{
        struct lang_ast_node *node = add_child(parent,
                                               LANG_AST_NODE_TYPE_COMMAND);

        expect_tok(toks, tok_ind, LANG_TOK_TYPE_LPAREN);
        expect_tok(toks, tok_ind, LANG_TOK_TYPE_STRING_LITERAL);
        utl_add_array_list_elem(&node->toks, cur_tok(toks, *tok_ind));
        expect_tok(toks, tok_ind, LANG_TOK_TYPE_RPAREN);
}

static void parse_block(struct lang_ast_node *parent,
                        const struct utl_array_list *toks, uint32_t *tok_ind)
{
        struct lang_ast_node *node = add_child(parent,
                                               LANG_AST_NODE_TYPE_BLOCK);

        while (next_tok(toks, tok_ind)->type != LANG_TOK_TYPE_RBRACE) {
                switch (cur_tok(toks, *tok_ind)->type) {
                case LANG_TOK_TYPE_LBRACE:
                        parse_block(node, toks, tok_ind);
                        break;

                case LANG_TOK_TYPE_DOLLAR:
                        parse_command(node, toks, tok_ind);
                        break;

                case LANG_TOK_TYPE_RANGLE:
                        parse_call(node, toks, tok_ind);
                        break;

                default:
                        utl_error(LANG_PARSE_ERROR_UNHANDLED);
                }

                expect_tok(toks, tok_ind, LANG_TOK_TYPE_SEMICOLON);
        }
}

static void parse_procedure(struct lang_ast_node *parent,
                            const struct utl_array_list *toks,
                            uint32_t *tok_ind)
{
        struct lang_ast_node *node = add_child(parent,
                                               LANG_AST_NODE_TYPE_PROCEDURE);

        expect_tok(toks, tok_ind, LANG_TOK_TYPE_IDENTIFIER);
        utl_add_array_list_elem(&node->toks, cur_tok(toks, *tok_ind));
        expect_tok(toks, tok_ind, LANG_TOK_TYPE_LBRACE);
        parse_block(node, toks, tok_ind);
}

void lang_parse(struct lang_ast_node *root, const struct utl_array_list *toks)
{
        uint32_t tok_ind = 0;

        while (next_tok(toks, &tok_ind)->type != LANG_TOK_TYPE_EOF) {
                switch (cur_tok(toks, tok_ind)->type) {
                case LANG_TOK_TYPE_AT:
                        parse_procedure(root, toks, &tok_ind);
                        break;

                default:
                        utl_error(LANG_PARSE_ERROR_UNHANDLED);
                }
        }
}

static const char *to_string(enum lang_ast_node_type type)
{
        switch (type) {
        case LANG_AST_NODE_TYPE_ROOT:      return "root";
        case LANG_AST_NODE_TYPE_PROCEDURE: return "procedure";
        case LANG_AST_NODE_TYPE_BLOCK:     return "block";
        case LANG_AST_NODE_TYPE_COMMAND:   return "command";
        case LANG_AST_NODE_TYPE_CALL:      return "call";

        default: return "UNKNOWN";
        }
}

static void print_node(const struct lang_ast_node *node, uint32_t depth)
{
        for (uint32_t i = 0; i < depth; i++)
                printf("\t");

        printf("| %s [", to_string(node->type));

        for (uint32_t i = 0; i < node->toks.size; i++) {
                const struct lang_tok *tok
                        = utl_get_array_list_elem(&node->toks, i);

                printf("%s, ", tok->value);
        }

        printf("]\n");

        for (uint32_t i = 0; i < node->children.size; i++) {
                const struct lang_ast_node *child
                        = utl_get_array_list_elem(&node->children, i);

                print_node(child, depth + 1);
        }
}

void lang_print_ast(const struct lang_ast_node *root)
{
        print_node(root, 0);
}