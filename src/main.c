#include "lang/lex.h"

int main(void)
{
        struct utl_array_list toks;
        lang_init_toks(&toks);

        const char *src = "123.23 342.5 3.141592653589 \"and stuff like this\"";

        lang_lex(&toks, src);
        lang_print_toks(&toks);

        lang_clean_toks(&toks);

        return 0;
}