#include "lang/lex.h"

#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LEX_BUFFER_SIZE 1024

void init_tokens(struct array_list *toks)
{
        init_array_list(toks, sizeof(struct token));
}

void clean_tokens(struct array_list *toks)
{
        for (int i = 0; i < toks->size; ++i) {
                struct token *tok = (struct token *)toks->data + i;
                free(tok->value);
        }
        clean_array_list(toks);
}

static enum token_type char_to_token_type(char c)
{
        switch (c) {
        case '~': return TOKEN_TYPE_TILDE;
        case '!': return TOKEN_TYPE_EXCLAMATION;
        case '@': return TOKEN_TYPE_AT;
        case '#': return TOKEN_TYPE_HASH;
        case '$': return TOKEN_TYPE_DOLLAR;
        case '%': return TOKEN_TYPE_PERCENTAGE;
        case '^': return TOKEN_TYPE_CARET;
        case '&': return TOKEN_TYPE_AMPERSAND;
        case '*': return TOKEN_TYPE_ASTERISK;
        case '-': return TOKEN_TYPE_HYPHEN;
        case '=': return TOKEN_TYPE_EQUALS;
        case '+': return TOKEN_TYPE_PLUS;
        case ';': return TOKEN_TYPE_SEMICOLON;
        case ':': return TOKEN_TYPE_COLON;
        case ',': return TOKEN_TYPE_COMMA;
        case '.': return TOKEN_TYPE_PERIOD;
        case '|': return TOKEN_TYPE_PIPE;
        case '/': return TOKEN_TYPE_SLASH;
        case '?': return TOKEN_TYPE_QUESTION;

        case '(': return TOKEN_TYPE_LPAREN;
        case ')': return TOKEN_TYPE_RPAREN;
        case '[': return TOKEN_TYPE_LBRACKET;
        case ']': return TOKEN_TYPE_RBRACKET;
        case '{': return TOKEN_TYPE_LBRACE;
        case '}': return TOKEN_TYPE_RBRACE;
        case '<': return TOKEN_TYPE_LANGLE;
        case '>': return TOKEN_TYPE_RANGLE;

        default: return TOKEN_TYPE_NULL;
        }
}

static inline bool is_special(char c)
{
        return char_to_token_type(c) != TOKEN_TYPE_NULL;
}

static void add_token(struct array_list *toks, enum token_type type,
                      const char *value, int line)
{
        struct token tok = {
                .line = line,
                .type = type,
                .value = malloc(strlen(value) + 1),
        };

        /* add a null character at the end of the tokens value as it wouldnt
         * normally be added, and is required in null terminated strings
         */
        tok.value[strlen(value)] = '\0';

        memcpy(tok.value, value, strlen(value));
        add_array_list_elem(toks, &tok);
}

static void skip_comment(const char **curchar)
{
        /* the first backtick will end the comment if not skipped */
        ++*curchar;

        while (**curchar != '`')
                ++*curchar;
        
        /* skip one more character to stop another comment from being started
         * during lexing
         */
        ++*curchar;
}

static void lex_special(struct array_list *toks, const char **curchar,
                        int line)
{
        char buf[2] = {
                **curchar, '\0',
        };
        add_token(toks, char_to_token_type(**curchar), buf, line);
        ++*curchar;
}

static void lex_num_literal(struct array_list *toks, const char **curchar,
                            int line)
{
        char buf[LEX_BUFFER_SIZE] = { 0 };
        while (isdigit(**curchar) || **curchar == '.') {
                strncat(buf, *curchar, 1);
                ++*curchar;
        }
        add_token(toks, TOKEN_TYPE_NUMLITERAL, buf, line);
}

static void lex_str_literal(struct array_list *toks, const char **curchar,
                            int line)
{
        char buf[LEX_BUFFER_SIZE] = { 0 };

        /* dont lex the first quote of the string */
        ++*curchar;

        while (**curchar != '"') {
                strncat(buf, *curchar, 1);
                ++*curchar;
        }
        add_token(toks, TOKEN_TYPE_STRLITERAL, buf, line);

        /* skip past last quote or else lex will immediately try to create
         * another string literal, causing a segfault
         */
        ++*curchar;
}

static void lex_identifier(struct array_list *toks, const char **curchar,
                           int line)
{
        char buf[LEX_BUFFER_SIZE] = { 0 };
        while (isalpha(**curchar) || **curchar == '_' || isdigit(**curchar)) {
                strncat(buf, *curchar, 1);
                ++*curchar;
        }
        add_token(toks, TOKEN_TYPE_IDENTIFIER, buf, line);
}

/* src needs to be null terminated
 * will write the lexed tokens into the toks array list
 */
void lex(struct array_list *toks, const char *src)
{
        add_token(toks, TOKEN_TYPE_SOF, "SOF", -1);
        const char *curchar = src;
        int line = 1;
        while (*curchar != '\0') {
                if (*curchar == '\n')
                        ++line;

                if (*curchar == '`')
                        skip_comment(&curchar);
                else if (is_special(*curchar))
                        lex_special(toks, &curchar, line);
                else if (isdigit(*curchar))
                        lex_num_literal(toks, &curchar, line);
                else if (*curchar == '"')
                        lex_str_literal(toks, &curchar, line);
                else if (isalpha(*curchar) || *curchar == '_')
                        lex_identifier(toks, &curchar, line);
                else
                        ++curchar;
        }
        add_token(toks, TOKEN_TYPE_EOF, "EOF", -1);
}

static char token_type_to_char(enum token_type type)
{
        switch (type) {
        case TOKEN_TYPE_NULL: return 'N';
        case TOKEN_TYPE_SOF:  return 'S';
        case TOKEN_TYPE_EOF:  return 'E';

        case TOKEN_TYPE_TILDE:       return '~';
        case TOKEN_TYPE_EXCLAMATION: return '!';
        case TOKEN_TYPE_AT:          return '@';
        case TOKEN_TYPE_HASH:        return '#';
        case TOKEN_TYPE_DOLLAR:      return '$';
        case TOKEN_TYPE_PERCENTAGE:  return '%';
        case TOKEN_TYPE_CARET:       return '^';
        case TOKEN_TYPE_AMPERSAND:   return '&';
        case TOKEN_TYPE_ASTERISK:    return '*';
        case TOKEN_TYPE_HYPHEN:      return '-';
        case TOKEN_TYPE_EQUALS:      return '=';
        case TOKEN_TYPE_PLUS:        return '+';
        case TOKEN_TYPE_SEMICOLON:   return ';';
        case TOKEN_TYPE_COLON:       return ':';
        case TOKEN_TYPE_COMMA:       return ',';
        case TOKEN_TYPE_PERIOD:      return '.';
        case TOKEN_TYPE_PIPE:        return '|';
        case TOKEN_TYPE_SLASH:       return '/';
        case TOKEN_TYPE_QUESTION:    return '?';

        case TOKEN_TYPE_LPAREN:   return '(';
        case TOKEN_TYPE_RPAREN:   return ')';
        case TOKEN_TYPE_LBRACKET: return '[';
        case TOKEN_TYPE_RBRACKET: return ']';
        case TOKEN_TYPE_LBRACE:   return '{';
        case TOKEN_TYPE_RBRACE:   return '}';
        case TOKEN_TYPE_LANGLE:   return '<';
        case TOKEN_TYPE_RANGLE:   return '>';

        case TOKEN_TYPE_NUMLITERAL: return 'n';
        case TOKEN_TYPE_STRLITERAL: return 's';
        case TOKEN_TYPE_IDENTIFIER: return 'i';

        default: return 'U';
        }
}

void print_tokens(const struct array_list *toks)
{
        for (int i = 0; i < toks->size; ++i) {
                const struct token *tok = array_list_elem(toks, i);
                printf("[%d] (%d, %c) - \"%s\"\n", i, tok->type,
                       token_type_to_char(tok->type), tok->value);
        }
}