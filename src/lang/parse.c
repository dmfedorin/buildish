#include "parse.h"

void init_ast(struct ast_node *root)
{
        root->type = AST_NODE_TYPE_ROOT;

        init_array_list(&root->toks, sizeof(struct tok));
        init_array_list(&root->children, sizeof(struct ast_node));
}

static void clean_node(struct ast_node *node)
{
        for (uint32_t i = 0; i < node->children.size; i++) {
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

static const struct tok *prev_tok(const struct array_list *toks,
                                  uint32_t *tok_ind)
{
        (*tok_ind)--;
        return get_array_list_elem(toks, *tok_ind);
}

static const struct tok *cur_tok(const struct array_list *toks,
                                 uint32_t tok_ind)
{
        return get_array_list_elem(toks, tok_ind);
}

static const struct tok *next_tok(const struct array_list *toks,
                                  uint32_t *tok_ind)
{
        (*tok_ind)++;
        return get_array_list_elem(toks, *tok_ind);
}

static const struct tok *peek_tok(const struct array_list *toks,
                                  uint32_t tok_ind)
{
        return get_array_list_elem(toks, tok_ind + 1);
}

static const void expect_tok(const struct array_list *toks,
                             uint32_t *tok_ind, enum tok_type type)
{
        if (next_tok(toks, tok_ind)->type != type)
                error(PARSE_ERROR_UNEXPECTED);
}

static struct ast_node *add_child(struct ast_node *parent,
                                  enum ast_node_type type)
{
        struct ast_node child = {
                .type = type
        };

        init_array_list(&child.toks, sizeof(struct tok));
        init_array_list(&child.children, sizeof(struct ast_node));
        add_array_list_elem(&parent->children, &child);

        struct ast_node *children = parent->children.data;

        return &children[parent->children.size - 1];
}

static void parse_call(struct ast_node *parent,
                       const struct array_list *toks, uint32_t *tok_ind)
{
        struct ast_node *node = add_child(parent, AST_NODE_TYPE_CALL);

        expect_tok(toks, tok_ind, TOK_TYPE_LPAREN);
        expect_tok(toks, tok_ind, TOK_TYPE_IDENTIFIER);
        add_array_list_elem(&node->toks, cur_tok(toks, *tok_ind));
        expect_tok(toks, tok_ind, TOK_TYPE_RPAREN);
}

static void parse_command(struct ast_node *parent,
                          const struct array_list *toks,
                          uint32_t *tok_ind)
{
        struct ast_node *node = add_child(parent, AST_NODE_TYPE_COMMAND);

        expect_tok(toks, tok_ind, TOK_TYPE_LPAREN);
        expect_tok(toks, tok_ind, TOK_TYPE_STRING_LITERAL);
        add_array_list_elem(&node->toks, cur_tok(toks, *tok_ind));
        expect_tok(toks, tok_ind, TOK_TYPE_RPAREN);
}

static void parse_block(struct ast_node *parent,
                        const struct array_list *toks, uint32_t *tok_ind)
{
        struct ast_node *node = add_child(parent, AST_NODE_TYPE_BLOCK);

        while (next_tok(toks, tok_ind)->type != TOK_TYPE_RBRACE) {
                switch (cur_tok(toks, *tok_ind)->type) {
                case TOK_TYPE_LBRACE:
                        parse_block(node, toks, tok_ind);
                        break;

                case TOK_TYPE_DOLLAR:
                        parse_command(node, toks, tok_ind);
                        break;

                case TOK_TYPE_RANGLE:
                        parse_call(node, toks, tok_ind);
                        break;

                default:
                        error(PARSE_ERROR_UNHANDLED);
                }

                expect_tok(toks, tok_ind, TOK_TYPE_SEMICOLON);
        }
}

static void parse_procedure(struct ast_node *parent,
                            const struct array_list *toks,
                            uint32_t *tok_ind)
{
        struct ast_node *node = add_child(parent, AST_NODE_TYPE_PROCEDURE);

        expect_tok(toks, tok_ind, TOK_TYPE_IDENTIFIER);
        add_array_list_elem(&node->toks, cur_tok(toks, *tok_ind));
        expect_tok(toks, tok_ind, TOK_TYPE_LBRACE);
        parse_block(node, toks, tok_ind);
}

void parse(struct ast_node *root, const struct array_list *toks)
{
        uint32_t tok_ind = 0;

        while (next_tok(toks, &tok_ind)->type != TOK_TYPE_EOF) {
                switch (cur_tok(toks, tok_ind)->type) {
                case TOK_TYPE_AT:
                        parse_procedure(root, toks, &tok_ind);
                        break;

                default:
                        error(PARSE_ERROR_UNHANDLED);
                }
        }
}

static const char *to_string(enum ast_node_type type)
{
        switch (type) {
        case AST_NODE_TYPE_ROOT:      return "root";
        case AST_NODE_TYPE_PROCEDURE: return "procedure";
        case AST_NODE_TYPE_BLOCK:     return "block";
        case AST_NODE_TYPE_COMMAND:   return "command";
        case AST_NODE_TYPE_CALL:      return "call";

        default: return "unknown";
        }
}

static void print_node(const struct ast_node *node, uint32_t depth)
{
        for (uint32_t i = 0; i < depth; i++)
                printf("\t");

        printf("| %s [", to_string(node->type));

        for (uint32_t i = 0; i < node->toks.size; i++) {
                const struct tok *tok = get_array_list_elem(&node->toks, i);

                printf("%s, ", tok->value);
        }

        printf("]\n");

        for (uint32_t i = 0; i < node->children.size; i++) {
                const struct ast_node *child
                        = get_array_list_elem(&node->children, i);

                print_node(child, depth + 1);
        }
}

void print_ast(const struct ast_node *root)
{
        print_node(root, 0);
}