#include "arraylist.h"

void inital(struct arraylist *al, int stride)
{
        al->size = 0;
        al->stride = stride;
        al->data = malloc(1);
}

void cleanal(struct arraylist *al)
{
        free(al->data);
}

const void *getalelem(const struct arraylist *al, int ind)
{
        return al->data + al->stride * ind;
}

static inline __attribute__ ((always_inline))
void *lastalelem(struct arraylist *al)
{
        return al->data + al->stride * max(0, al->size - 1);
}

void addalelem(struct arraylist *al, const void *elem)
{
        al->size++;
        al->data = realloc(al->data, al->stride * al->size);

        memcpy(lastalelem(al), elem, al->stride);
}