#ifndef HASHMAP_H
#define HASHMAP_H

#define MAP_CAPACITY 1024
#define LIMIT_KEY_LENGTH 100
#define LIMIT_VALUE_LENGTH 100

typedef struct _node {
    char key[LIMIT_KEY_LENGTH];
    char value[LIMIT_VALUE_LENGTH];
    struct _node *next;
} node;

typedef struct _hashmap {
    unsigned int size, capacity;
    struct _node *arr[MAP_CAPACITY];
} hashmap;

void init_hashmap(hashmap *map);
int hash_function(char *key);

void mp_insert(hashmap *map, char *key, char *value);
void mp_remove(hashmap *map, char *key);
char *mp_search(hashmap* map, char *key);
void mp_clear(hashmap *map);

#endif