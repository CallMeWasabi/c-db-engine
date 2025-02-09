#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libs/vector/vector.h"

void init_str_vec(str_vector *vec) {
    if (vec == NULL) {
        fprintf(stderr, "Invalid input to init_str_vector()\n");
        return;
    }

    vec->arr = (char**)malloc(sizeof(char*) * DEFAULT_VEC_CAPACITY);
    vec->size = 0;
    vec->capacity = DEFAULT_VEC_CAPACITY;

    memset(vec->arr, 0, sizeof(char*) * DEFAULT_VEC_CAPACITY);
}

void str_vec_push_back(str_vector *vec, const char *value) {
    if (vec == NULL || value == NULL) {
        fprintf(stderr, "Invalid input to str_vec_push_back()\n");
        return;
    } else if (vec->size + 1 >= vec->capacity) {
        vec->capacity *= 2;
        vec->arr = realloc(vec->arr, vec->capacity);

        if (vec->arr == NULL) {
            fprintf(stderr, "Memory reallocation failed for vector arr\n");
            str_vec_free(vec);
            return;
        }
    }
    vec->arr[vec->size] = strdup(value);

    if (vec->arr[vec->size] == NULL) {
        fprintf(stderr, "Memory allocation failed for strdup\n");
        free(vec->arr[vec->size]);
        return;
    }

    vec->size += 1;
}

void str_vec_pop_back(str_vector *vec) {
    if (vec == NULL || vec->size == 0) {
        fprintf(stderr, "Invalid input for str_vec_pop_back()\n");
        return;
    }

    free(vec->arr[vec->size]);
    vec->size -= 1;
}

void str_vec_free(str_vector *vec) {
    if (vec == NULL) {
        fprintf(stderr, "Invalid input for str_vec_free()\n");
        return;
    }

    for (int i=0; i<vec->size; i++) {
        free(vec->arr[i]);
    }
    free(vec->arr);
    free(vec);
}