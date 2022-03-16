#include "lang/exec.h"

#include "util/debug.h"
#include <stdlib.h>
#include "util/arraylist.h"
#include <dirent.h>
#include "util/fmt.h"
#include "util/files.h"
#include <string.h>

#define ERROR_NO_PROC "calling nonexistant procedure"
#define ERROR_NO_DIR "opening nonexistant directory"
#define FMT_BUFFER_SIZE 1024

struct proc {
        const char *name;
        const struct ast_node *node;
};

/* all procedures are expected to be children of the root node */
static void get_procs(struct array_list *procs, const struct ast_node *root)
{
        for (int i = 0; i < root->children.size; ++i) {
                const struct ast_node *node = ast_node_child(root, i);
                const struct token *name = ast_node_token(node, 0);
                if (node->type != AST_NODE_TYPE_PROC)
                        continue;
                struct proc proc = {
                        .name = name->value,
                        .node = node,
                };
                add_array_list_elem(procs, &proc);
        }
}

static const struct proc *find_proc(const struct array_list *procs,
                                    const char *name)
{
        for (int i = 0; i < procs->size; ++i) {
                const struct proc *proc = array_list_elem(procs, i);
                if (strcmp(proc->name, name) == 0)
                        return proc;
        }
        return NULL;
}

static void exec_block(const struct ast_node *node,
                       const struct array_list *procs);

static void call_proc(const struct array_list *procs, const char *name)
{
        const struct proc *proc = find_proc(procs, name);
        if (proc == NULL)
                error(ERROR_NO_PROC);
        const struct ast_node *block = ast_node_child(proc->node, 0);
        exec_block(block, procs);
}

static inline void exec_log(const struct ast_node *node)
{
        for (int i = 0; i < node->toks.size; ++i) {
                const struct token *msg = ast_node_token(node, i);
                log_info(msg->value);
        }
}

static inline void exec_call(const struct ast_node *node,
                             const struct array_list *procs)
{
        for (int i = 0; i < node->toks.size; ++i) {
                const struct token *name = ast_node_token(node, i);
                call_proc(procs, name->value);
        }
}

static inline void exec_cmd(const struct ast_node *node)
{
        for (int i = 0; i < node->toks.size; ++i) {
                const struct token *cmd = ast_node_token(node, i);
                system(cmd->value);
        }
}

static void exec_allcmd(const struct ast_node *node)
{
        const struct token *dir = ast_node_token(node, 0);
        const struct token *ext = ast_node_token(node, 1);
        const struct token *cmd = ast_node_token(node, 2);

        DIR *d = opendir(dir->value);
        if (d == NULL)
                error(ERROR_NO_DIR);

        const struct dirent *de;
        for (de = readdir(d); de != NULL; de = readdir(d)) {
                if (de->d_type != DT_REG)
                        continue;
                if (strcmp(ext->value, file_ext(de->d_name)) != 0)
                        continue;
                char fmtbuf[FMT_BUFFER_SIZE] = { 0 };
                fmt_replace(fmtbuf, cmd->value, de->d_name);
                system(fmtbuf);
        }
}

static void exec_block(const struct ast_node *node,
                       const struct array_list *procs)
{
        for (int i = 0; i < node->children.size; ++i) {
                const struct ast_node *child = ast_node_child(node, i);
                switch (child->type) {
                case AST_NODE_TYPE_BLOCK:
                        exec_block(child, procs);
                        break;
                case AST_NODE_TYPE_ALLCMD:
                        exec_allcmd(child);
                        break;
                case AST_NODE_TYPE_CMD:
                        exec_cmd(child);
                        break;
                case AST_NODE_TYPE_CALL:
                        exec_call(child, procs);
                        break;
                case AST_NODE_TYPE_LOG:
                        exec_log(child);
                        break;
                }
        }
}

void exec(const struct ast_node *root)
{
        struct array_list procs;
        init_array_list(&procs, sizeof(struct proc));
        get_procs(&procs, root);
        call_proc(&procs, "main");
        clean_array_list(&procs);
}