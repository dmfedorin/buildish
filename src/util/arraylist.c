#include "util/arraylist.h"

#include <stdlib.h>
#include <string.h>
#include "util/util.h"
#include <stdint.h>

void init_array_list(struct array_list *al, int stride)
{
        *al = (struct array_list){
                .size = 0,
                .stride = stride,
                .data = malloc(1),
        };
}

void clean_array_list(struct array_list *al)
{
        free(al->data);
}

const void *array_list_elem(const struct array_list *al, int ind)
{
        return (const void *)((uintptr_t)al->data + al->stride * ind);
}

static inline void *last_array_list_elem(struct array_list *al)
{
        int ind = max(0, al->size - 1);
        return (void *)((uintptr_t)al->data + al->stride * ind);
}

void add_array_list_elem(struct array_list *al, const void *elem)
{
        ++al->size;
        al->data = realloc(al->data, al->stride * al->size);
        memcpy(last_array_list_elem(al), elem, al->stride);
}