#include "lang/lex.h"
#include "lang/parse.h"
#include "util/files.h"

#define FILEPATH "buildishprocs"

int main(void)
{
        struct utl_array_list toks;
        lang_init_toks(&toks);

        char *src = malloc(utl_get_file_size(FILEPATH));
        utl_read_file(src, FILEPATH);

        lang_lex(&toks, src);
        lang_print_toks(&toks);

        struct lang_ast_node root;
        lang_init_ast(&root);

        lang_parse(&root, &toks);
        lang_print_ast(&root);

        lang_clean_ast(&root);
        lang_clean_toks(&toks);

        return 0;
}