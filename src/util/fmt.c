#include "fmt.h"

// replaces every "%" in the src with the replace string
// assumes dst is big enough to fit the output
void fmtreplace(char *dst, const char *src, const char *replace)
{
        const char *curchar = src;

        while (*curchar != '\0') {
                if (*curchar == '%')
                        strcat(dst, replace);
                else
                        strncat(dst, curchar, 1);

                curchar++;
        }
}