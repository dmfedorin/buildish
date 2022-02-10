#include "lang/lex.h"
#include "lang/parse.h"
#include "util/files.h"

#define FILEPATH "buildishprocs"

int main(void)
{
        struct array_list toks;
        init_toks(&toks);

        char *src = malloc(get_file_size(FILEPATH));
        read_file(src, FILEPATH);

        lex(&toks, src);
        print_toks(&toks);

        struct ast_node root;
        init_ast(&root);

        parse(&root, &toks);
        print_ast(&root);

        clean_ast(&root);
        clean_toks(&toks);

        return 0;
}