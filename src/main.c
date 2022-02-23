#include "lang/lex.h"
#include "lang/parse.h"
#include "lang/exec/exec.h"
#include "util/files.h"
#include "term/opts.h"

#define FILEPATH               "buildishprocs"
#define VERSION(maj, min, pat) #maj "." #min "." #pat

static struct arraylist toks, opts;
static struct astnode root;

static inline void clean(void)
{
        cleanast(&root);
        cleantoks(&toks);
        cleanopts(&opts);
}

int main(int argc, const char **argv)
{
        atexit(clean);
        
        printf("buildish %s\n", VERSION(1, 3, 0));

        initopts(&opts);
        getopts(&opts, argc, argv);

        inittoks(&toks);
        initast(&root);

        char *src = malloc(filesize(FILEPATH));
        rdfile(src, FILEPATH);

        lex(&toks, src);
        parse(&root, &toks);

        if (optscont(&opts, "-debug")) {
                printtoks(&toks);
                printast(&root);
        }

        exec(&root);

        return 0;
}