#include "lang/lex.h"

#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LEXBUFSIZE 1024

void inittoks(struct arraylist *toks)
{
        inital(toks, sizeof(struct tok));
}

void cleantoks(struct arraylist *toks)
{
        for (int i = 0; i < toks->size; i++) {
                struct tok *tok = (struct tok *)toks->data + i;
                free(tok->value);
        }

        cleanal(toks);
}

static enum toktype chartott(char c)
{
        switch (c) {
        case '~': return TT_TILDE;
        case '!': return TT_EXCLAMATION;
        case '@': return TT_AT;
        case '#': return TT_HASH;
        case '$': return TT_DOLLAR;
        case '%': return TT_PERCENTAGE;
        case '^': return TT_CARET;
        case '&': return TT_AMPERSAND;
        case '*': return TT_ASTERISK;
        case '-': return TT_HYPHEN;
        case '=': return TT_EQUALS;
        case '+': return TT_PLUS;
        case ';': return TT_SEMICOLON;
        case ':': return TT_COLON;
        case ',': return TT_COMMA;
        case '.': return TT_PERIOD;
        case '|': return TT_PIPE;
        case '/': return TT_SLASH;
        case '?': return TT_QUESTION;

        case '(': return TT_LPAREN;
        case ')': return TT_RPAREN;
        case '[': return TT_LBRACKET;
        case ']': return TT_RBRACKET;
        case '{': return TT_LBRACE;
        case '}': return TT_RBRACE;
        case '<': return TT_LANGLE;
        case '>': return TT_RANGLE;

        default: return TT_NULL;
        }
}

static inline bool isspecial(char c)
{
        return chartott(c) != TT_NULL;
}

static void addtok(struct arraylist *toks, enum toktype type,
                   const char *value, int line)
{
        struct tok tok = {
                .type = type, .value = malloc(strlen(value) + 1),
                .line = line,
        };

        /*
        add a null character at the end of the tokens value as it wouldnt
        normally be added, and is required in null terminated strings
        */
        tok.value[strlen(value)] = '\0';

        memcpy(tok.value, value, strlen(value));
        addalelem(toks, &tok);
}

static void skipcomment(const char **curchar)
{
        // the first backtick will end the comment if not skipped
        ++*curchar;

        while (**curchar != '`')
                ++*curchar;
        
        /*
        skip one more character to stop another comment from being started
        during lexing
        */
        ++*curchar;
}

static void lexspecial(struct arraylist *toks, const char **curchar, int line)
{
        char buf[2] = {
                **curchar, '\0',
        };

        addtok(toks, chartott(**curchar), buf, line);

        ++*curchar;
}

static void lexnumliteral(struct arraylist *toks, const char **curchar,
                          int line)
{
        char buf[LEXBUFSIZE] = { 0 };
        int bufind = 0;

        while (isdigit(**curchar) || **curchar == '.') {
                strncat(buf, *curchar, 1);
                ++*curchar;
        }

        addtok(toks, TT_NUMLITERAL, buf, line);
}

static void lexstrliteral(struct arraylist *toks, const char **curchar,
                          int line)
{
        char buf[LEXBUFSIZE] = { 0 };
        int bufind = 0;

        // dont lex the first quote of the string
        ++*curchar;

        while (**curchar != '"') {
                strncat(buf, *curchar, 1);
                ++*curchar;
        }
        
        addtok(toks, TT_STRLITERAL, buf, line);

        /*
        skip past last quote or else lex will immediately try to create
        another string literal, causing a segfault
        */
        ++*curchar;
}

static void lexidentifier(struct arraylist *toks, const char **curchar,
                          int line)
{
        char buf[LEXBUFSIZE] = { 0 };
        int bufind = 0;

        while (isalpha(**curchar) || **curchar == '_' || isdigit(**curchar)) {
                strncat(buf, *curchar, 1);
                ++*curchar;
        }

        addtok(toks, TT_IDENTIFIER, buf, line);
}

// src needs to be null terminated
void lex(struct arraylist *toks, const char *src)
{
        addtok(toks, TT_SOF, "SOF", -1);
        
        const char *curchar = src;
        int line = 1;

        while (*curchar != '\0') {
                if (*curchar == '\n')
                        line++;
                        
                if (*curchar == '`')
                        skipcomment(&curchar);
                else if (isspecial(*curchar))
                        lexspecial(toks, &curchar, line);
                else if (isdigit(*curchar))
                        lexnumliteral(toks, &curchar, line);
                else if (*curchar == '"')
                        lexstrliteral(toks, &curchar, line);
                else if (isalpha(*curchar) || *curchar == '_')
                        lexidentifier(toks, &curchar, line);
                else
                        curchar++;
        }

        addtok(toks, TT_EOF, "EOF", -1);
}

static char tttochar(enum toktype type)
{
        switch (type) {
        case TT_NULL: return 'N';
        case TT_SOF:  return 'S';
        case TT_EOF:  return 'E';

        case TT_TILDE:       return '~';
        case TT_EXCLAMATION: return '!';
        case TT_AT:          return '@';
        case TT_HASH:        return '#';
        case TT_DOLLAR:      return '$';
        case TT_PERCENTAGE:  return '%';
        case TT_CARET:       return '^';
        case TT_AMPERSAND:   return '&';
        case TT_ASTERISK:    return '*';
        case TT_HYPHEN:      return '-';
        case TT_EQUALS:      return '=';
        case TT_PLUS:        return '+';
        case TT_SEMICOLON:   return ';';
        case TT_COLON:       return ':';
        case TT_COMMA:       return ',';
        case TT_PERIOD:      return '.';
        case TT_PIPE:        return '|';
        case TT_SLASH:       return '/';
        case TT_QUESTION:    return '?';

        case TT_LPAREN:   return '(';
        case TT_RPAREN:   return ')';
        case TT_LBRACKET: return '[';
        case TT_RBRACKET: return ']';
        case TT_LBRACE:   return '{';
        case TT_RBRACE:   return '}';
        case TT_LANGLE:   return '<';
        case TT_RANGLE:   return '>';

        case TT_NUMLITERAL: return 'n';
        case TT_STRLITERAL: return 's';
        case TT_IDENTIFIER: return 'i';

        default: return 'U';
        }
}

void printtoks(const struct arraylist *toks)
{
        for (int i = 0; i < toks->size; i++) {
                const struct tok *tok = getalelem(toks, i);

                printf("[%d] (%d, %c) - \"%s\"\n", i, tok->type,
                       tttochar(tok->type), tok->value);
        }
}