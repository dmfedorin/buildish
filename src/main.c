#include "lang/lex.h"
#include "lang/parse.h"
#include <stdio.h>
#include "lang/exec.h"
#include "util/files.h"
#include <stdlib.h>
#include "term/opts.h"

#define FILEPATH "buildishprocs"
#define VERSION(major, minor, patch) #major "." #minor "." #patch

static char *src;
static struct array_list toks, opts;
static struct ast_node root;

static void clean(void)
{
        clean_ast(&root);
        clean_tokens(&toks);
        clean_opts(&opts);
        free(src);
}

int main(int argc, const char *argv[])
{
        atexit(clean);
        printf("buildish %s\n", VERSION(1, 3, 2));

        init_opts(&opts);
        get_opts(&opts, argc, argv);
        init_tokens(&toks);
        init_ast(&root);

        src = malloc(file_size(FILEPATH));
        read_file(src, FILEPATH);

        lex(&toks, src);
        parse(&root, &toks);

        if (opts_contain(&opts, "-dtoks"))
                print_tokens(&toks);
        if (opts_contain(&opts, "-dast"))
                print_ast(&root);

        exec(&root);
        return 0;
}