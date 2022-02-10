#ifndef ARRAYLIST_H_INCLUDED
#define ARRAYLIST_H_INCLUDED

#include "util.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct array_list {
        void *data;
        uint32_t size, stride;
};

void init_array_list(struct array_list *al, uint32_t stride);

void clean_array_list(struct array_list *al);

const void *get_array_list_elem(const struct array_list *al, uint32_t ind);

void add_array_list_elem(struct array_list *al, const void *elem);

#endif