#ifndef FMT_H_petzagiz0ttnfpr88r6v3cg31sllx1u2
#define FMT_H_petzagiz0ttnfpr88r6v3cg31sllx1u2

/* replaces every "%" in the src with the replace string
 * assumes dst is big enough to fit the output
 */
void fmt_replace(char *dst, const char *src, const char *replace);

#endif