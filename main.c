#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "hashmap.h"

#define NTHREAD (2)

size_t hash(void* string) {
    size_t value = 0;
    for (char* cursor = (char*)string; *cursor != 0; cursor++) {
        value = (value + *cursor) % 1000;
    }
    return value;
}

int cmp(void* a, void* b) {
    if(strcmp((char*)a, (char*)b) == 0) {
        return true;
    }
    return false;
}

void key_destruct(void* k) {
    k = NULL;
}

void value_destruct(void* v) {
    v = NULL;
}

void print_hash(struct hash_map* map) {
    for (int i = 0; i < map->capacity; i++) {
		if (!map->entries[i]) {
			printf("entry %d ------- NONE -------\n", i);
		} else {
            struct entry* cursor = map->entries[i];
            printf("entry %d ------- ", i);
            while (cursor->next) {
                printf("%s, %s-------", cursor->key, cursor->value);
                cursor = cursor->next;
            }
			printf("%s, %s-------\n", cursor->key, cursor->value);
		}
	}
}

void* example1(void* k) {
    k = NULL;
    printf("example1 begin\n");
    struct hash_map* map = hash_map_new(hash, cmp, key_destruct, value_destruct);
    hash_map_put_entry_move(map, "Phoebe", "328");
    hash_map_put_entry_move(map, "Lesi", "083");
    hash_map_put_entry_move(map, "Edward", "192");
    hash_map_put_entry_move(map, "Erik", "24");
    hash_map_put_entry_move(map, "John", "202");
    hash_map_put_entry_move(map, "Tyson", "242");
    hash_map_put_entry_move(map, "James", "245");
    hash_map_put_entry_move(map, "Cheng", "190");
    print_hash(map);
    hash_map_destroy(map);
    printf("example1 end\n");
    return k;
}

void* example2(void* k) {
    k = NULL;
    printf("example2 begin\n");
    struct hash_map* map = hash_map_new(hash, cmp, key_destruct, value_destruct);
    hash_map_put_entry_move(map, "Phoebe", "328");
    hash_map_put_entry_move(map, "James", "245");
    hash_map_put_entry_move(map, "James", "111");
    hash_map_put_entry_move(map, "Cheng", "190");
    puts("This hashmap should include Cheng");
    print_hash(map);
    hash_map_remove_entry(map, "Cheng");
    hash_map_remove_entry(map, "nonono");
    puts("This hashmap should not include Change");
    print_hash(map);
    hash_map_destroy(map);
    printf("example2 end\n");
    return k;
}

int main() {
    int i = 0;

    pthread_t* thread_group = malloc(sizeof(pthread_t) * NTHREAD);

    pthread_create(&thread_group[0], NULL, example1, NULL);
    pthread_create(&thread_group[1], NULL, example2, NULL);

    for (i = 0; i < NTHREAD; i++) {
        pthread_join(thread_group[i], NULL);
    }


    return 0;
}
