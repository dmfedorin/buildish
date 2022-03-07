#ifndef FMTH_165529
#define FMTH_165529

// replaces every "%" in the src with the replace string
// assumes dst is big enough to fit the output
void fmtreplace(char *dst, const char *src, const char *replace);

#endif