#ifndef VECTOR_H
#include <stdio.h>
#define VECTOR_H
#define DEFAULT_VEC_CAPACITY 255

typedef struct _str_vector {
    size_t size;
    size_t capacity;
    char **arr;
} str_vector;

void init_str_vec(str_vector *vec);
void str_vec_push_back(str_vector *vec, const char *value);
void str_vec_pop_back(str_vector *vec);
void str_vec_remove(str_vector *vec, size_t index);
void str_vec_free(str_vector *vec);

#endif