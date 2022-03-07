#ifndef ARRAYLISTH_1657652
#define ARRAYLISTH_1657652

struct arraylist {
        void *data;
        int size, stride;
};

void inital(struct arraylist *al, int stride);

void cleanal(struct arraylist *al);

const void *getalelem(const struct arraylist *al, int ind);

void addalelem(struct arraylist *al, const void *elem);

#endif