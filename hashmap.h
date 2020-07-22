#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define HASHMAP_INIT_CAPACITY (6)
#define HASHMAP_GROWTH_RT (2)
#define LOAD_FACTOR (0.75)

struct entry {
    void* value;
    void* key; 
    struct entry* next; 
}; 

struct hash_map {
    struct entry** entries; 
    int size; 
    int capacity;
    pthread_mutex_t lock;
    size_t (*hash)(void*);
    int (*cmp)(void*, void*);
    void (*key_destruct)(void*);
    void (*value_destruct)(void*);
}; 

struct hash_map* hash_map_new(size_t (*hash)(void*), int (*cmp)(void*,void*), void (*key_destruct)(void*), void (*value_destruct)(void*));

void hash_map_put_entry_move(struct hash_map* map, void* k, void* v);

void hash_map_remove_entry(struct hash_map* map, void* k);

void* hash_map_get_value_ref(struct hash_map* map, void* k);

void hash_map_destroy(struct hash_map* map);

#endif
