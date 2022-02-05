#ifndef ARRAYLIST_H_INCLUDED
#define ARRAYLIST_H_INCLUDED

#include "util.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct utl_array_list {
        void *data;
        uint32_t size, stride;
};

void utl_init_array_list(struct utl_array_list *al, uint32_t stride);

void utl_clean_array_list(struct utl_array_list *al);

void *utl_get_array_list_elem(struct utl_array_list *al, uint32_t ind);

void utl_add_array_list_elem(struct utl_array_list *al, const void *elem);

#endif