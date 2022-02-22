#ifndef FMT_H_INCLUDED
#define FMT_H_INCLUDED

#include <string.h>

// replaces every "%" in the src with the replace string
// assumes dst is big enough to fit the output
void fmtreplace(char *dst, const char *src, const char *replace);

#endif