#include "lex.h"

void lang_init_toks(struct utl_array_list *toks)
{
        utl_init_array_list(toks, sizeof(struct lang_tok));
}

void lang_clean_toks(struct utl_array_list *toks)
{
        for (uint32_t i = 0; i < toks->size; i++) {
                struct lang_tok *tok = utl_get_array_list_elem(toks, i);
                free(tok->value);
        }

        utl_clean_array_list(toks);
}

static enum lang_tok_type to_tok_type(char c)
{
        switch (c) {
        case '`': return LANG_TOK_TYPE_BACKTICK;
        case '~': return LANG_TOK_TYPE_TILDE;
        case '!': return LANG_TOK_TYPE_EXCLAMATION;
        case '@': return LANG_TOK_TYPE_AT;
        case '#': return LANG_TOK_TYPE_HASH;
        case '$': return LANG_TOK_TYPE_DOLLAR;
        case '%': return LANG_TOK_TYPE_PERCENTAGE;
        case '^': return LANG_TOK_TYPE_CARET;
        case '&': return LANG_TOK_TYPE_AMPERSAND;
        case '*': return LANG_TOK_TYPE_ASTERISK;
        case '-': return LANG_TOK_TYPE_HYPHEN;
        case '=': return LANG_TOK_TYPE_EQUALS;
        case '+': return LANG_TOK_TYPE_PLUS;
        case ';': return LANG_TOK_TYPE_SEMICOLON;
        case ':': return LANG_TOK_TYPE_COLON;
        case '|': return LANG_TOK_TYPE_PIPE;
        case '/': return LANG_TOK_TYPE_SLASH;
        case '?': return LANG_TOK_TYPE_QUESTION;

        case '(': return LANG_TOK_TYPE_LPAREN;
        case ')': return LANG_TOK_TYPE_RPAREN;
        case '[': return LANG_TOK_TYPE_LBRACKET;
        case ']': return LANG_TOK_TYPE_RBRACKET;
        case '{': return LANG_TOK_TYPE_LBRACE;
        case '}': return LANG_TOK_TYPE_RBRACE;
        case '<': return LANG_TOK_TYPE_LANGLE;
        case '>': return LANG_TOK_TYPE_RANGLE;

        default: return LANG_TOK_TYPE_NULL;
        }
}

static inline __attribute__ ((always_inline)) bool is_special(char c)
{
        return to_tok_type(c) != LANG_TOK_TYPE_NULL;
}

static void add_token(struct utl_array_list *toks, enum lang_tok_type type,
                      const char *value)
{
        struct lang_tok tok = {
                .type = type,
                .value = malloc(strlen(value))
        };

        memcpy(tok.value, value, strlen(value));
        utl_add_array_list_elem(toks, &tok);
}

static inline void lex_number_literal(struct utl_array_list *toks,
                                      const char **c)
{
        char buf[LANG_LEX_BUFFER_SIZE];
        memset(buf, '\0', LANG_LEX_BUFFER_SIZE);

        uint32_t buf_ind = 0;

        while (isdigit(**c) || **c == '.') {
                buf[buf_ind] = **c;
                buf_ind++;
                (*c)++;
        }

        add_token(toks, LANG_TOK_TYPE_NUMBER_LITERAL, buf);
}

static inline void lex_string_literal(struct utl_array_list *toks,
                                      const char **c)
{
        char buf[LANG_LEX_BUFFER_SIZE];
        memset(buf, '\0', LANG_LEX_BUFFER_SIZE);
        
        uint32_t buf_ind = 0;

        // dont lex the first quote of the string
        (*c)++;

        while (**c != '"') {
                buf[buf_ind] = **c;
                buf_ind++;
                (*c)++;
        }

        /*
        skip past last quote or else lex will immediately try to create
        another string literal, causing a segfault
        */
        (*c)++;

        add_token(toks, LANG_TOK_TYPE_STRING_LITERAL, buf);
}

// src needs to be null terminated
void lang_lex(struct utl_array_list *toks, const char *src)
{
        const char *c = src;

        while (*c != '\0') {
                if (isdigit(*c))
                        lex_number_literal(toks, &c);
                else if (*c == '"')
                        lex_string_literal(toks, &c);
                else
                        c++;
        }
}

static char to_char(enum lang_tok_type type)
{
        switch (type) {
        case LANG_TOK_TYPE_NULL: return 'N';

        case LANG_TOK_TYPE_BACKTICK:    return '`';
        case LANG_TOK_TYPE_TILDE:       return '~';
        case LANG_TOK_TYPE_EXCLAMATION: return '!';
        case LANG_TOK_TYPE_AT:          return '@';
        case LANG_TOK_TYPE_HASH:        return '#';
        case LANG_TOK_TYPE_DOLLAR:      return '$';
        case LANG_TOK_TYPE_PERCENTAGE:  return '%';
        case LANG_TOK_TYPE_CARET:       return '^';
        case LANG_TOK_TYPE_AMPERSAND:   return '&';
        case LANG_TOK_TYPE_ASTERISK:    return '*';
        case LANG_TOK_TYPE_HYPHEN:      return '-';
        case LANG_TOK_TYPE_EQUALS:      return '=';
        case LANG_TOK_TYPE_PLUS:        return '+';
        case LANG_TOK_TYPE_SEMICOLON:   return ';';
        case LANG_TOK_TYPE_COLON:       return ':';
        case LANG_TOK_TYPE_PIPE:        return '|';
        case LANG_TOK_TYPE_SLASH:       return '/';
        case LANG_TOK_TYPE_QUESTION:    return '?';

        case LANG_TOK_TYPE_LPAREN:   return '(';
        case LANG_TOK_TYPE_RPAREN:   return ')';
        case LANG_TOK_TYPE_LBRACKET: return '[';
        case LANG_TOK_TYPE_RBRACKET: return ']';
        case LANG_TOK_TYPE_LBRACE:   return '{';
        case LANG_TOK_TYPE_RBRACE:   return '}';
        case LANG_TOK_TYPE_LANGLE:   return '<';
        case LANG_TOK_TYPE_RANGLE:   return '>';

        case LANG_TOK_TYPE_NUMBER_LITERAL: return 'n';
        case LANG_TOK_TYPE_STRING_LITERAL: return 's';

        default: return 'U';
        }
}

void lang_print_toks(struct utl_array_list *toks)
{
        for (uint32_t i = 0; i < toks->size; i++) {
                struct lang_tok *tok = utl_get_array_list_elem(toks, i);

                printf("[%d] \"%s\" (%d, %c)\n", i, tok->value,
                       tok->type, to_char(tok->type));
        }
}