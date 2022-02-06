#include "lang/lex.h"

int main(void)
{
        struct utl_array_list toks;
        lang_init_toks(&toks);

        const char *src = "something_cool: (3.14159), \"string here\";";

        lang_lex(&toks, src);
        lang_print_toks(&toks);

        lang_clean_toks(&toks);

        return 0;
}