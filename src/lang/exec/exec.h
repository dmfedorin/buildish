#ifndef EXEC_H_INCLUDED
#define EXEC_H_INCLUDED

#include "procs.h"

#include "../parse.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void execute(const struct astnode *root);

#endif