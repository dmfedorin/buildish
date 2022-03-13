#ifndef FILES_H_g3h600509f344bcdiiolvkocc9x9r8r7
#define FILES_H_g3h600509f344bcdiiolvkocc9x9r8r7

int file_size(const char *filepath);

void read_file(char *dst, const char *filepath);

const char *file_ext(const char *filename);

#endif