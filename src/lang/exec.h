#ifndef EXEC_H_INCLUDED
#define EXEC_H_INCLUDED

#include "parse.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void execute(const struct ast_node *root);

#endif