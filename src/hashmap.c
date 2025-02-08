// Implementation hashmap
#include "../libs/hashmap/hashmap.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void init_hashmap(hashmap *map) {
    if (map == NULL) {
        return;
    }

    map->capacity = MAP_CAPACITY;
    map->size = 0;
    memset(map->arr, 0, sizeof(node*) * MAP_CAPACITY);
}

int hash_function(char *key) {
    unsigned long hash = 5381;
    int c;

    // DJB2 algorithm
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % MAP_CAPACITY;
}

void mp_insert(hashmap *map, char *key, char *value) {
    int bucket_index = hash_function(key);

    node *new_node = (node*)malloc(sizeof(node));
    strcpy(new_node->key, key);
    strcpy(new_node->value, value);
    new_node->next = NULL;

    if (map->arr[bucket_index] != NULL) {
        node *tmp = map->arr[bucket_index];
        while (tmp->next != NULL && strcmp(tmp->key, key) != 0) {
            tmp = tmp->next;
        }
        if (strcmp(tmp->key, key) == 0) {
            strcpy(tmp->value, value);
        } else {
            tmp->next = new_node;
        }
    } else {
        map->arr[bucket_index] = new_node;
    }
    map->size += 1;
}

void mp_remove(hashmap *map, char *key) {
    if (map->size == 0) {
        return;
    }

    int bucket_index = hash_function(key);

    node *tmp = map->arr[bucket_index], *prev = NULL;
    while (tmp != NULL && strcmp(tmp->key, key) != 0) {
        printf("%s %s\n", tmp->key, key);
        prev = tmp;
        tmp = tmp->next;
    }

    if (tmp == NULL) {
        return;
    } else if (prev == NULL) {
        map->arr[bucket_index] = tmp->next;
    } else {
        prev->next = tmp->next;
    }
    
    free(tmp);
    map->size -= 1;
}

char *mp_search(hashmap* map, char *key) {
    int bucket_index = hash_function(key);

    node *tmp = map->arr[bucket_index];
    while (tmp != NULL && strcmp(tmp->key, key) != 0) {
        tmp = tmp->next;
    }

    if (tmp == NULL) {
        return NULL;
    }

    return tmp->value;
}

void mp_clear(hashmap *map) {
    for (int i=0; i<MAP_CAPACITY; i++) {
        if (map->arr[i] != NULL) {
            node *tmp = map->arr[i], *gb;
            while (tmp != NULL) {
                gb = tmp;
                tmp = tmp->next;
                free(gb);
            }
        }
    }
    map->size = 0;
}