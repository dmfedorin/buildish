#ifndef LEX_H_INCLUDED
#define LEX_H_INCLUDED

enum lang_tok_type {
        LANG_TOK_TYPE_NULL,
};

struct lang_tok {
        enum lang_tok_type type;
        char *value;
};

#endif