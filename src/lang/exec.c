#include "exec.h"

#define ERRNOPROC "calling nonexistant procedure"

struct proc {
        const char *name;
        const struct astnode *node;
};

// all procedures are expected to be children of the root node
static void getprocs(struct arraylist *procs, const struct astnode *root)
{
        for (int i = 0; i < root->children.size; i++) {
                const struct astnode *node = getalelem(&root->children, 0);
                const struct tok *name = getalelem(&node->toks, 0);

                if (node->type != ANT_PROC)
                        continue;

                struct proc proc = {
                        .name = name, .node = node,
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

static void callproc(const struct arraylist *procs, const char *name)
{
        const struct proc *proc = getproc(procs, name);

        if (proc == NULL)
                error(ERRNOPROC);
}

static void execlog(const struct astnode *node)
{
        for (int i = 0; i < node->toks.size; i++) {
                const struct tok *msg = getalelem(&node->toks, i);
                loginfo(msg->value);
        }
}

static void execcall(const struct astnode *node)
{
}

static void execcmd(const struct astnode *node)
{
}

static void execallcmd(const struct astnode *node)
{
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
                        break;

                case ANT_CMD:
                        break;

                case ANT_CALL:
                        break;

                case ANT_LOG:
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