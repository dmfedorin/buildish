#ifndef ARRAYLIST_H_1657652
#define ARRAYLIST_H_1657652

struct array_list {
        void *data;
        int size, stride;
};

void init_array_list(struct array_list *al, int stride);

void clean_array_list(struct array_list *al);

const void *array_list_elem(const struct array_list *al, int ind);

void add_array_list_elem(struct array_list *al, const void *elem);

#endif