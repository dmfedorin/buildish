#ifndef LEX_H_165833
#define LEX_H_165833

#include "util/arraylist.h"

enum token_type {
        TOKEN_TYPE_NULL,
        TOKEN_TYPE_SOF,
        TOKEN_TYPE_EOF,
        
        TOKEN_TYPE_TILDE,       // ~
        TOKEN_TYPE_EXCLAMATION, // !
        TOKEN_TYPE_AT,          // @
        TOKEN_TYPE_HASH,        // #
        TOKEN_TYPE_DOLLAR,      // $
        TOKEN_TYPE_PERCENTAGE,  // %
        TOKEN_TYPE_CARET,       // ^
        TOKEN_TYPE_AMPERSAND,   // &
        TOKEN_TYPE_ASTERISK,    // *
        TOKEN_TYPE_HYPHEN,      // -
        TOKEN_TYPE_EQUALS,      // =
        TOKEN_TYPE_PLUS,        // +
        TOKEN_TYPE_SEMICOLON,   // ;
        TOKEN_TYPE_COLON,       // :
        TOKEN_TYPE_COMMA,       // ,
        TOKEN_TYPE_PERIOD,      // .
        TOKEN_TYPE_PIPE,        // |
        TOKEN_TYPE_SLASH,       // /
        TOKEN_TYPE_QUESTION,    // ?

        TOKEN_TYPE_LPAREN,   // (
        TOKEN_TYPE_RPAREN,   // )
        TOKEN_TYPE_LBRACKET, // [
        TOKEN_TYPE_RBRACKET, // ]
        TOKEN_TYPE_LBRACE,   // {
        TOKEN_TYPE_RBRACE,   // }
        TOKEN_TYPE_LANGLE,   // <
        TOKEN_TYPE_RANGLE,   // >

        TOKEN_TYPE_NUMLITERAL,
        TOKEN_TYPE_STRLITERAL,
        TOKEN_TYPE_IDENTIFIER,
};

struct token {
        enum token_type type;
        char *value;
        int line;
};

void inittoks(struct arraylist *toks);

void cleantoks(struct arraylist *toks);

// src needs to be null terminated
void lex(struct arraylist *toks, const char *src);

void printtoks(const struct arraylist *toks);

#endif