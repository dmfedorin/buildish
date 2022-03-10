#include "lang/exec.h"

#include "util/debug.h"
#include <stdlib.h>
#include "util/arraylist.h"
#include <dirent.h>
#include "util/fmt.h"
#include "util/files.h"
#include <string.h>

#define ERRNOPROC  "calling nonexistant procedure"
#define ERRNODIR   "opening nonexistant directory"
#define FMTBUFSIZE 1024

struct proc {
        const char *name;
        const struct astnode *node;
};

// all procedures are expected to be children of the root node
static void getprocs(struct arraylist *procs, const struct astnode *root)
{
        for (int i = 0; i < root->children.size; i++) {
                const struct astnode *node = getchild(root, i);
                const struct tok *name = gettok(node, 0);

                if (node->type != ANT_PROC)
                        continue;

                struct proc proc = {
                        .name = name->value, .node = node,
                };

                addalelem(procs, &proc);
        }
}

static const struct proc *getproc(const struct arraylist *procs,
                                  const char *name)
{
        for (int i = 0; i < procs->size; i++) {
                const struct proc *proc = getalelem(procs, i);

                if (strcmp(proc->name, name) == 0)
                        return proc;
        }

        return NULL;
}

static void execblock(const struct astnode *node,
                      const struct arraylist *procs);

static void callproc(const struct arraylist *procs, const char *name)
{
        const struct proc *proc = getproc(procs, name);

        if (proc == NULL)
                error(ERRNOPROC);

        const struct astnode *block = getchild(proc->node, 0);
        execblock(block, procs);
}

static inline void execlog(const struct astnode *node)
{
        for (int i = 0; i < node->toks.size; i++) {
                const struct tok *msg = gettok(node, i);
                loginfo(msg->value);
        }
}

static inline void execcall(const struct astnode *node,
                     const struct arraylist *procs)
{
        for (int i = 0; i < node->toks.size; i++) {
                const struct tok *name = gettok(node, i);
                callproc(procs, name->value);
        }
}

static inline void execcmd(const struct astnode *node)
{
        for (int i = 0; i < node->toks.size; i++) {
                const struct tok *cmd = gettok(node, i);
                system(cmd->value);
        }
}

static void execallcmd(const struct astnode *node)
{
        const struct tok *dir = gettok(node, 0);
        const struct tok *ext = gettok(node, 1);
        const struct tok *cmd = gettok(node, 2);

        DIR *d = opendir(dir->value);

        if (d == NULL)
                error(ERRNODIR);

        const struct dirent *de;

        while ((de = readdir(d)) != NULL) {
                if (de->d_type != DT_REG)
                        continue;

                if (strcmp(ext->value, fileext(de->d_name)) != 0)
                        continue;

                char fmtbuf[FMTBUFSIZE] = { 0 };
                fmtreplace(fmtbuf, cmd->value, de->d_name);

                system(fmtbuf);
        }
}

static void execblock(const struct astnode *node,
                      const struct arraylist *procs)
{
        for (int i = 0; i < node->children.size; i++) {
                const struct astnode *child = getchild(node, i);

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

void exec(const struct astnode *root)
{
        struct arraylist procs;
        inital(&procs, sizeof(struct proc));

        getprocs(&procs, root);

        callproc(&procs, "main");

        cleanal(&procs);
}