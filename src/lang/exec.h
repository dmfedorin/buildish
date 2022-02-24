#ifndef EXEC_H_INCLUDED
#define EXEC_H_INCLUDED

#include "parse.h"

#include "../util/debug.h"
#include "../util/arraylist.h"
#include "../util/fmt.h"

#include <dirent.h>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void exec(const struct astnode *root);

#endif