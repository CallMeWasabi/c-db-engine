// Implementation hashmap
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../libs/hashmap/hashmap.h"

void init_hashmap(hashmap *map) {
    if (map == NULL) {
        return;
    }

    map->capacity = MAP_CAPACITY;
    map->size = 0;
    memset(map->arr, 0, sizeof(hash_node*) * MAP_CAPACITY);
}

void init_nhashmap(hashmap *map, int num_pair, hash_pair *pairs) {
    if (map == NULL) {
        return;
    }

    init_hashmap(map);

    for (int i=0; i<num_pair; i++) {
        mp_insert(map, pairs[i].first, pairs[i].second);
    }
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

    hash_node *new_node = (hash_node*)malloc(sizeof(hash_node));
    new_node->key = malloc(strlen(key) + 1);
    new_node->value = malloc(strlen(value) + 1);

    if (new_node->key == NULL || new_node->value == NULL) {
        fprintf(stderr, "Memory allocation failed for node->key and node->value\n");
        free(new_node->key);
        free(new_node->value);
        free(new_node);
        return;
    }

    strcpy(new_node->key, key);
    strcpy(new_node->value, value);
    new_node->next = NULL;

    if (map->arr[bucket_index] != NULL) {
        hash_node *tmp = map->arr[bucket_index];
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

    hash_node *tmp = map->arr[bucket_index], *prev = NULL;
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

    hash_node *tmp = map->arr[bucket_index];
    while (tmp != NULL && strcmp(tmp->key, key) != 0) {
        tmp = tmp->next;
    }

    if (tmp == NULL) {
        return NULL;
    }

    return tmp->value;
}

void mp_free(hashmap *map) {
    for (int i=0; i<MAP_CAPACITY; i++) {
        if (map->arr[i] != NULL) {
            hash_node *tmp = map->arr[i], *gb;
            while (tmp != NULL) {
                gb = tmp;
                tmp = tmp->next;
                free(gb->key);
                free(gb->value);
                free(gb);
            }
        }
    }
    map->size = 0;
    free(map);
}