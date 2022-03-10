#ifndef FILES_H_165548
#define FILES_H_165548

int file_size(const char *filepath);

void read_file(char *dst, const char *filepath);

const char *file_ext(const char *filename);

#endif