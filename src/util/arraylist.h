#ifndef ARRAYLIST_H_INCLUDED
#define ARRAYLIST_H_INCLUDED

#include "util.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct arraylist {
        void *data;
        uint32_t size, stride;
};

void inital(struct arraylist *al, uint32_t stride);

void cleanal(struct arraylist *al);

const void *getalelem(const struct arraylist *al, uint32_t ind);

void addalelem(struct arraylist *al, const void *elem);

#endif