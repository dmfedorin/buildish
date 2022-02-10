#ifndef LEX_H_INCLUDED
#define LEX_H_INCLUDED

#include "../util/arraylist.h"

#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

enum tok_type {
        TOK_TYPE_NULL,
        TOK_TYPE_SOF,
        TOK_TYPE_EOF,
        
        TOK_TYPE_TILDE,       // ~
        TOK_TYPE_EXCLAMATION, // !
        TOK_TYPE_AT,          // @
        TOK_TYPE_HASH,        // #
        TOK_TYPE_DOLLAR,      // $
        TOK_TYPE_PERCENTAGE,  // %
        TOK_TYPE_CARET,       // ^
        TOK_TYPE_AMPERSAND,   // &
        TOK_TYPE_ASTERISK,    // *
        TOK_TYPE_HYPHEN,      // -
        TOK_TYPE_EQUALS,      // =
        TOK_TYPE_PLUS,        // +
        TOK_TYPE_SEMICOLON,   // ;
        TOK_TYPE_COLON,       // :
        TOK_TYPE_PIPE,        // |
        TOK_TYPE_SLASH,       // /
        TOK_TYPE_QUESTION,    // ?

        TOK_TYPE_LPAREN,   // (
        TOK_TYPE_RPAREN,   // )
        TOK_TYPE_LBRACKET, // [
        TOK_TYPE_RBRACKET, // ]
        TOK_TYPE_LBRACE,   // {
        TOK_TYPE_RBRACE,   // }
        TOK_TYPE_LANGLE,   // <
        TOK_TYPE_RANGLE,   // >

        TOK_TYPE_NUMBER_LITERAL,
        TOK_TYPE_STRING_LITERAL,
        TOK_TYPE_IDENTIFIER,
};

struct tok {
        enum tok_type type;
        char *value;
};

void init_toks(struct array_list *toks);

void clean_toks(struct array_list *toks);

// src needs to be null terminated
void lex(struct array_list *toks, const char *src);

void print_toks(const struct array_list *toks);

#endif