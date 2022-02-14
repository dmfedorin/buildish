#ifndef ARRAYLIST_H_INCLUDED
#define ARRAYLIST_H_INCLUDED

#include "util.h"

#include <stdlib.h>
#include <string.h>

struct arraylist {
        void *data;
        int size, stride;
};

void inital(struct arraylist *al, int stride);

void cleanal(struct arraylist *al);

const void *getalelem(const struct arraylist *al, int ind);

void addalelem(struct arraylist *al, const void *elem);

#endif