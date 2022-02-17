#include "lang/lex.h"
#include "lang/parse.h"
#include "lang/exec/exec.h"
#include "util/files.h"

#define FILEPATH                     "buildishprocs"
#define VERSION(major, minor, patch) #major "." #minor "." #patch

static struct arraylist toks;
static struct astnode root;

static inline void clean(void)
{
        cleanast(&root);
        cleantoks(&toks);
}

int main(int argc, const char **argv)
{
        atexit(clean);
        
        printf("buildish %s\n", VERSION(1, 1, 1));

        inittoks(&toks);
        initast(&root);

        char *src = malloc(filesize(FILEPATH));
        rdfile(src, FILEPATH);

        lex(&toks, src);
        parse(&root, &toks);
        exec(&root);

        return 0;
}