#ifndef ARRAYLIST_H_ge94d127w5p20hatwjicwj3ydks0rse7
#define ARRAYLIST_H_ge94d127w5p20hatwjicwj3ydks0rse7

struct array_list {
        void *data;
        int size, stride;
};

void init_array_list(struct array_list *al, int stride);

void clean_array_list(struct array_list *al);

const void *array_list_elem(const struct array_list *al, int ind);

void add_array_list_elem(struct array_list *al, const void *elem);

#endif