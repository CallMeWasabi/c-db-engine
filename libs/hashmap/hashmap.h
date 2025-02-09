#ifndef HASHMAP_H
#include <stdio.h>
#define HASHMAP_H
#define MAP_CAPACITY 1023
#define LIMIT_KEY_LENGTH 255
#define LIMIT_VALUE_LENGTH 255

typedef struct _hash_pair {
    char *first;
    char *second;
} hash_pair;

typedef struct _hash_node {
    char *key;
    char *value;
    struct _hash_node *next;
} hash_node;

typedef struct _hashmap {
    size_t size;
    size_t capacity;
    struct _hash_node *arr[MAP_CAPACITY];
} hashmap;

void init_hashmap(hashmap *map);
void init_nhashmap(hashmap *map, int num_pair, hash_pair *pairs);
int hash_function(char *key);

void mp_insert(hashmap *map, char *key, char *value);
void mp_remove(hashmap *map, char *key);
char *mp_search(hashmap* map, char *key);
void mp_free(hashmap *map);

#endif