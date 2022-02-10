#include "lex.h"

#define LEX_BUFFER_SIZE 1024

void init_toks(struct array_list *toks)
{
        init_array_list(toks, sizeof(struct tok));
}

void clean_toks(struct array_list *toks)
{
        for (uint32_t i = 0; i < toks->size; i++) {
                struct tok *tok = (struct tok *)toks->data + i;
                free(tok->value);
        }

        clean_array_list(toks);
}

static enum tok_type to_tok_type(char c)
{
        switch (c) {
        case '~': return TOK_TYPE_TILDE;
        case '!': return TOK_TYPE_EXCLAMATION;
        case '@': return TOK_TYPE_AT;
        case '#': return TOK_TYPE_HASH;
        case '$': return TOK_TYPE_DOLLAR;
        case '%': return TOK_TYPE_PERCENTAGE;
        case '^': return TOK_TYPE_CARET;
        case '&': return TOK_TYPE_AMPERSAND;
        case '*': return TOK_TYPE_ASTERISK;
        case '-': return TOK_TYPE_HYPHEN;
        case '=': return TOK_TYPE_EQUALS;
        case '+': return TOK_TYPE_PLUS;
        case ';': return TOK_TYPE_SEMICOLON;
        case ':': return TOK_TYPE_COLON;
        case '|': return TOK_TYPE_PIPE;
        case '/': return TOK_TYPE_SLASH;
        case '?': return TOK_TYPE_QUESTION;

        case '(': return TOK_TYPE_LPAREN;
        case ')': return TOK_TYPE_RPAREN;
        case '[': return TOK_TYPE_LBRACKET;
        case ']': return TOK_TYPE_RBRACKET;
        case '{': return TOK_TYPE_LBRACE;
        case '}': return TOK_TYPE_RBRACE;
        case '<': return TOK_TYPE_LANGLE;
        case '>': return TOK_TYPE_RANGLE;

        default: return TOK_TYPE_NULL;
        }
}

static inline __attribute__ ((always_inline)) bool is_special(char c)
{
        return to_tok_type(c) != TOK_TYPE_NULL;
}

static void add_token(struct array_list *toks, enum tok_type type,
                      const char *value)
{
        struct tok tok = {
                .type = type,
                .value = malloc(strlen(value) + 1)
        };

        /*
        add a null character at the end of the tokens value as it wouldnt
        normally be added, and is required in null terminated strings
        */
        tok.value[strlen(value)] = '\0';

        memcpy(tok.value, value, strlen(value));
        add_array_list_elem(toks, &tok);
}

static inline void skip_comment(const char **cur_char)
{
        // the first backtick will end the comment if not skipped
        (*cur_char)++;

        while (**cur_char != '`')
                (*cur_char)++;
        
        /*
        skip one more character to stop another comment from being started
        during lexing
        */
        (*cur_char)++;
}

static inline void lex_special(struct array_list *toks,
                               const char **cur_char)
{
        char special_buf[2] = {
                **cur_char, '\0',
        };

        add_token(toks, to_tok_type(**cur_char), special_buf);

        (*cur_char)++;
}

static inline void lex_number_literal(struct array_list *toks,
                                      const char **cur_char)
{
        char buf[LEX_BUFFER_SIZE];
        memset(buf, '\0', LEX_BUFFER_SIZE);

        uint32_t buf_ind = 0;

        while (isdigit(**cur_char) || **cur_char == '.') {
                buf[buf_ind] = **cur_char;
                buf_ind++;
                (*cur_char)++;
        }

        add_token(toks, TOK_TYPE_NUMBER_LITERAL, buf);
}

static inline void lex_string_literal(struct array_list *toks,
                                      const char **cur_char)
{
        char buf[LEX_BUFFER_SIZE];
        memset(buf, '\0', LEX_BUFFER_SIZE);
        
        uint32_t buf_ind = 0;

        // dont lex the first quote of the string
        (*cur_char)++;

        while (**cur_char != '"') {
                buf[buf_ind] = **cur_char;
                buf_ind++;
                (*cur_char)++;
        }
        
        add_token(toks, TOK_TYPE_STRING_LITERAL, buf);

        /*
        skip past last quote or else lex will immediately try to create
        another string literal, causing a segfault
        */
        (*cur_char)++;
}

static inline void lex_identifier(struct array_list *toks,
                                  const char **cur_char)
{
        char buf[LEX_BUFFER_SIZE];
        memset(buf, '\0', LEX_BUFFER_SIZE);
        
        uint32_t buf_ind = 0;

        while (isalpha(**cur_char) || **cur_char == '_') {
                buf[buf_ind] = **cur_char;
                buf_ind++;
                (*cur_char)++;
        }

        add_token(toks, TOK_TYPE_IDENTIFIER, buf);
}

// src needs to be null terminated
void lex(struct array_list *toks, const char *src)
{
        add_token(toks, TOK_TYPE_SOF, "SOF");
        
        const char *cur_char = src;

        while (*cur_char != '\0') {
                if (*cur_char == '`')
                        skip_comment(&cur_char);
                else if (is_special(*cur_char))
                        lex_special(toks, &cur_char);
                else if (isdigit(*cur_char))
                        lex_number_literal(toks, &cur_char);
                else if (*cur_char == '"')
                        lex_string_literal(toks, &cur_char);
                else if (isalpha(*cur_char) || *cur_char == '_')
                        lex_identifier(toks, &cur_char);
                else
                        cur_char++;
        }

        add_token(toks, TOK_TYPE_EOF, "EOF");
}

static char to_char(enum tok_type type)
{
        switch (type) {
        case TOK_TYPE_NULL: return 'N';
        case TOK_TYPE_SOF:  return 'S';
        case TOK_TYPE_EOF:  return 'E';

        case TOK_TYPE_TILDE:       return '~';
        case TOK_TYPE_EXCLAMATION: return '!';
        case TOK_TYPE_AT:          return '@';
        case TOK_TYPE_HASH:        return '#';
        case TOK_TYPE_DOLLAR:      return '$';
        case TOK_TYPE_PERCENTAGE:  return '%';
        case TOK_TYPE_CARET:       return '^';
        case TOK_TYPE_AMPERSAND:   return '&';
        case TOK_TYPE_ASTERISK:    return '*';
        case TOK_TYPE_HYPHEN:      return '-';
        case TOK_TYPE_EQUALS:      return '=';
        case TOK_TYPE_PLUS:        return '+';
        case TOK_TYPE_SEMICOLON:   return ';';
        case TOK_TYPE_COLON:       return ':';
        case TOK_TYPE_PIPE:        return '|';
        case TOK_TYPE_SLASH:       return '/';
        case TOK_TYPE_QUESTION:    return '?';

        case TOK_TYPE_LPAREN:   return '(';
        case TOK_TYPE_RPAREN:   return ')';
        case TOK_TYPE_LBRACKET: return '[';
        case TOK_TYPE_RBRACKET: return ']';
        case TOK_TYPE_LBRACE:   return '{';
        case TOK_TYPE_RBRACE:   return '}';
        case TOK_TYPE_LANGLE:   return '<';
        case TOK_TYPE_RANGLE:   return '>';

        case TOK_TYPE_NUMBER_LITERAL: return 'n';
        case TOK_TYPE_STRING_LITERAL: return 's';
        case TOK_TYPE_IDENTIFIER:     return 'i';

        default: return 'U';
        }
}

void print_toks(const struct array_list *toks)
{
        for (uint32_t i = 0; i < toks->size; i++) {
                const struct tok *tok = get_array_list_elem(toks, i);

                printf("[%d] (%d, %c) - \"%s\"\n", i, tok->type,
                       to_char(tok->type), tok->value);
        }
}