#ifndef LEXH_165833
#define LEXH_165833

#include "util/arraylist.h"

enum toktype {
        TT_NULL,
        TT_SOF,
        TT_EOF,
        
        TT_TILDE,       // ~
        TT_EXCLAMATION, // !
        TT_AT,          // @
        TT_HASH,        // #
        TT_DOLLAR,      // $
        TT_PERCENTAGE,  // %
        TT_CARET,       // ^
        TT_AMPERSAND,   // &
        TT_ASTERISK,    // *
        TT_HYPHEN,      // -
        TT_EQUALS,      // =
        TT_PLUS,        // +
        TT_SEMICOLON,   // ;
        TT_COLON,       // :
        TT_COMMA,       // ,
        TT_PERIOD,      // .
        TT_PIPE,        // |
        TT_SLASH,       // /
        TT_QUESTION,    // ?

        TT_LPAREN,   // (
        TT_RPAREN,   // )
        TT_LBRACKET, // [
        TT_RBRACKET, // ]
        TT_LBRACE,   // {
        TT_RBRACE,   // }
        TT_LANGLE,   // <
        TT_RANGLE,   // >

        TT_NUMLITERAL,
        TT_STRLITERAL,
        TT_IDENTIFIER,
};

struct tok {
        enum toktype type;
        char *value;
        int line;
};

void inittoks(struct arraylist *toks);

void cleantoks(struct arraylist *toks);

// src needs to be null terminated
void lex(struct arraylist *toks, const char *src);

void printtoks(const struct arraylist *toks);

#endif