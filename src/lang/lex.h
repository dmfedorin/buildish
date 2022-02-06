#ifndef LEX_H_INCLUDED
#define LEX_H_INCLUDED

#include "../util/arraylist.h"

#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define LANG_LEX_BUFFER_SIZE 1024

enum lang_tok_type {
        LANG_TOK_TYPE_NULL,

        LANG_TOK_TYPE_BACKTICK,    // `
        LANG_TOK_TYPE_TILDE,       // ~
        LANG_TOK_TYPE_EXCLAMATION, // !
        LANG_TOK_TYPE_AT,          // @
        LANG_TOK_TYPE_HASH,        // #
        LANG_TOK_TYPE_DOLLAR,      // $
        LANG_TOK_TYPE_PERCENTAGE,  // %
        LANG_TOK_TYPE_CARET,       // ^
        LANG_TOK_TYPE_AMPERSAND,   // &
        LANG_TOK_TYPE_ASTERISK,    // *
        LANG_TOK_TYPE_HYPHEN,      // -
        LANG_TOK_TYPE_EQUALS,      // =
        LANG_TOK_TYPE_PLUS,        // +
        LANG_TOK_TYPE_SEMICOLON,   // ;
        LANG_TOK_TYPE_COLON,       // :
        LANG_TOK_TYPE_PIPE,        // |
        LANG_TOK_TYPE_SLASH,       // /
        LANG_TOK_TYPE_QUESTION,    // ?

        LANG_TOK_TYPE_LPAREN,   // (
        LANG_TOK_TYPE_RPAREN,   // )
        LANG_TOK_TYPE_LBRACKET, // [
        LANG_TOK_TYPE_RBRACKET, // ]
        LANG_TOK_TYPE_LBRACE,   // {
        LANG_TOK_TYPE_RBRACE,   // }
        LANG_TOK_TYPE_LANGLE,   // <
        LANG_TOK_TYPE_RANGLE,   // >

        LANG_TOK_TYPE_NUMBER_LITERAL,
        LANG_TOK_TYPE_STRING_LITERAL,
        LANG_TOK_TYPE_IDENTIFIER,
};

struct lang_tok {
        enum lang_tok_type type;
        char *value;
};

void lang_init_toks(struct utl_array_list *toks);

void lang_clean_toks(struct utl_array_list *toks);

// src needs to be null terminated
void lang_lex(struct utl_array_list *toks, const char *src);

void lang_print_toks(struct utl_array_list *toks);

#endif