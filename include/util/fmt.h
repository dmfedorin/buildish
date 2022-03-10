#ifndef FMT_H_165529
#define FMT_H_165529

// replaces every "%" in the src with the replace string
// assumes dst is big enough to fit the output
void fmt_replace(char *dst, const char *src, const char *replace);

#endif