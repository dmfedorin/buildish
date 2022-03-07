#include "lang/lex.h"
#include "lang/parse.h"
#include <stdio.h>
#include "lang/exec.h"
#include "util/files.h"
#include <stdlib.h>
#include "term/opts.h"

#define FILEPATH               "buildishprocs"
#define VERSION(maj, min, pat) #maj "." #min "." #pat

static char *src;
static struct arraylist toks, opts;
static struct astnode root;

static void clean(void)
{
        cleanast(&root);
        cleantoks(&toks);
        cleanopts(&opts);
        free(src);
}

int main(int argc, const char *argv[])
{
        atexit(clean);
        
        printf("buildish %s\n", VERSION(1, 3, 2));

        initopts(&opts);
        getopts(&opts, argc, argv);

        inittoks(&toks);
        initast(&root);

        src = malloc(filesize(FILEPATH));
        rdfile(src, FILEPATH);

        lex(&toks, src);
        parse(&root, &toks);

        if (optscont(&opts, "-dtoks"))
                printtoks(&toks);
        
        if (optscont(&opts, "-dast"))
                printast(&root);

        exec(&root);

        return 0;
}