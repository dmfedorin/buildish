#include "arraylist.h"

void utl_init_array_list(struct utl_array_list *al, uint32_t stride)
{
        al->size = 0;
        al->stride = stride;
        al->data = malloc(1);
}

void utl_clean_array_list(struct utl_array_list *al)
{
        free(al->data);
}

const void *utl_get_array_list_elem(const struct utl_array_list *al,
                                    uint32_t ind)
{
        return al->data + al->stride * ind;
}

static inline __attribute__ ((always_inline))
void *get_last_array_list_elem(struct utl_array_list *al)
{
        return al->data + al->stride * utl_max(0, al->size - 1);
}

void utl_add_array_list_elem(struct utl_array_list *al, const void *elem)
{
        al->size++;
        al->data = realloc(al->data, al->stride * al->size);

        memcpy(get_last_array_list_elem(al), elem, al->stride);
}