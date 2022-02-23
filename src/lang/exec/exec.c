#include "exec.h"

#define ERRNOMAIN     "no main procedure"
#define ERRINVALIDDIR "invalid directory"
#define FMTBUFSIZE    1024

static inline void execlog(const struct astnode *node)
{
        const struct tok *text = getalelem(&node->toks, 0);
        loginfo(text->value);
}

static inline void execproc(const struct astnode *node,
                            const struct arraylist *procs);

static inline void execcall(const struct astnode *node,
                            const struct arraylist *procs)
{
        const struct tok *callname = getalelem(&node->toks, 0);
        const struct astnode *proc = getprocnode(procs, callname->value);

        execproc(proc, procs);
}

static inline void execcmd(const struct astnode *node)
{
        const struct tok *cmd = getalelem(&node->toks, 0);
        system(cmd->value);
}

static void execallcmd(const struct astnode *node)
{
        const struct tok *dir = getalelem(&node->toks, 0);
        const struct tok *cmd = getalelem(&node->toks, 1);

        DIR *d = opendir(dir->value);

        if (d == NULL)
                error(ERRINVALIDDIR);

        const struct dirent *ent = readdir(d);

        while (ent != NULL) {
                if (ent->d_type != DT_REG) {
                        ent = readdir(d);
                        continue;
                }

                char fmtbuf[FMTBUFSIZE] = { 0 };
                fmtreplace(fmtbuf, cmd->value, ent->d_name);

                system(fmtbuf);

                ent = readdir(d);
        }

        closedir(d);
}

static void execblock(const struct astnode *node,
                      const struct arraylist *procs)
{
        for (int i = 0; i < node->children.size; i++) {
                const struct astnode *child = getalelem(&node->children, i);

                switch (child->type) {
                case ANT_BLOCK:
                        execblock(child, procs);
                        break;

                case ANT_ALLCMD:
                        execallcmd(child);
                        break;

                case ANT_CMD:
                        execcmd(child);
                        break;

                case ANT_CALL:
                        execcall(child, procs);
                        break;

                case ANT_LOG:
                        execlog(child);
                        break;
                }
        }
}

static inline void execproc(const struct astnode *node,
                            const struct arraylist *procs)
{
        const struct astnode *block = getalelem(&node->children, 0);
        execblock(block, procs);
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