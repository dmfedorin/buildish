#include "exec.h"

#define ERRNOMAIN "no main procedure"

static void execproc(const struct astnode *node,
                     const struct arraylist *procs);

static inline __attribute__ ((always_inline))
const struct tok *callnodetext(const struct astnode *node)
{
        return getalelem(&node->toks, 0);
}

static void execcall(const struct astnode *node,
                     const struct arraylist *procs)
{
        const struct astnode *proc = getprocnode(procs,
                                                 callnodetext(node)->value);

        execproc(proc, procs);
}

static void execcmd(const struct astnode *node)
{
        const struct tok *cmdtok = getalelem(&node->toks, 0);
        system(cmdtok->value);
}

static void execblock(const struct astnode *node,
                      const struct arraylist *procs)
{
        for (uint32_t i = 0; i < node->children.size; i++) {
                const struct astnode *child = getalelem(&node->children, i);

                switch (child->type) {
                case ANT_BLOCK:
                        execblock(child, procs);
                        break;

                case ANT_COMMAND:
                        execcmd(child);
                        break;

                case ANT_CALL:
                        execcall(child, procs);
                        break;
                }
        }
}

static inline __attribute__ ((always_inline))
const struct astnode *procnodeblock(const struct astnode *node)
{
        return getalelem(&node->children, 0);
}

static void execproc(const struct astnode *node,
                     const struct arraylist *procs)
{
        execblock(procnodeblock(node), procs);
}

void exec(const struct astnode *root)
{
        struct arraylist procs;
        inital(&procs, sizeof(struct proc));

        findprocs(&procs, root);

        const struct astnode *mainproc = getprocnode(&procs, "main");

        if (mainproc == NULL)
                error(ERRNOMAIN);

        execproc(mainproc, &procs);
}