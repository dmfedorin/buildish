#include "arraylist.h"

void init_array_list(struct array_list *al, uint32_t stride)
{
        al->size = 0;
        al->stride = stride;
        al->data = malloc(1);
}

void clean_array_list(struct array_list *al)
{
        free(al->data);
}

const void *get_array_list_elem(const struct array_list *al, uint32_t ind)
{
        return al->data + al->stride * ind;
}

static inline __attribute__ ((always_inline))
void *get_last_array_list_elem(struct array_list *al)
{
        return al->data + al->stride * max(0, al->size - 1);
}

void add_array_list_elem(struct array_list *al, const void *elem)
{
        al->size++;
        al->data = realloc(al->data, al->stride * al->size);

        memcpy(get_last_array_list_elem(al), elem, al->stride);
}