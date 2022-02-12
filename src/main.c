#include "lang/lex.h"
#include "lang/parse.h"
#include "util/files.h"

#define FILEPATH "buildishprocs"

int main(void)
{
        struct arraylist toks;
        inittoks(&toks);

        char *src = malloc(filesize(FILEPATH));
        rdfile(src, FILEPATH);

        lex(&toks, src);
        printtoks(&toks);

        struct astnode root;
        initast(&root);

        parse(&root, &toks);
        printast(&root);

        cleanast(&root);
        cleantoks(&toks);

        return 0;
}