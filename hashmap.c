#include "hashmap.h"

void destruct(struct hash_map* map, struct entry* e) {
	map->key_destruct(e->key);
	map->value_destruct(e->value); 
} 

void assign(struct entry* e, void* k, void* v) {
	e->key = k;
	e->value = v;
}

void insert_entry(struct hash_map* map, struct entry* e, void* k, void* v) {
	assign(e, k, v);
	e->next = NULL;  
	map->size++;
}

void replace_entry(struct hash_map* map, struct entry* e, void* k, void* v) {
	destruct(map, e); 
	assign(e, k, v); 
}

void destroy_entry(struct hash_map* map, struct entry* e) {
	destruct(map, e); 
	free(e);
	e = NULL; 
}

void remove_entry(struct hash_map* map, struct entry* e, int i) {
	struct entry* temp = e; 
	map->entries[i] = temp->next;
	destroy_entry(map, temp); 
	map->size--;
}

struct hash_map* hash_map_new(size_t (*hash)(void*), int (*cmp)(void*,void*), void (*key_destruct)(void*), void (*value_destruct)(void*)) {
	struct hash_map* map; 
	if (hash != NULL && cmp != NULL && key_destruct != NULL && value_destruct != NULL) { 
		map = malloc(sizeof(struct hash_map)); 
		map->entries = malloc(sizeof(struct entry*) * HASHMAP_INIT_CAPACITY);  
		map->size = 0;
		map->capacity = HASHMAP_INIT_CAPACITY; 
		map->hash = hash;
		map->cmp = cmp;
		map->key_destruct = key_destruct;
		map->value_destruct = value_destruct;
		pthread_mutex_init(&map->lock, NULL); 

		for (int i = 0; i < HASHMAP_INIT_CAPACITY; i++) {
			map->entries[i] = NULL; 
		}
	} else {
		map = NULL;  
	}
	return map; 
}

void hash_map_put_entry_move(struct hash_map* map, void* k, void* v) { 
	if (k != NULL && v != NULL && map != NULL) { 
		pthread_mutex_lock(&map->lock);

		int i = map->hash(k) % map->capacity; 
		if (map->entries[i] != NULL) { // put into linked list 

			struct entry* cursor = map->entries[i];
			while (cursor->next != NULL) { 
				if (map->cmp(cursor->key, k) == true) {
					replace_entry(map, cursor, k, v);
					pthread_mutex_unlock(&map->lock); 
					return; 
				}
				cursor = cursor->next;
			}

			// loop to last node 
			if (map->cmp(cursor->key, k) == true) {
				replace_entry(map, cursor, k, v);
			} else {
				cursor->next = malloc(sizeof(struct entry));
				insert_entry(map, cursor->next, k, v);
			}

		} else { // put into empty entries 

			map->entries[i] = malloc(sizeof(struct entry));
			insert_entry(map, map->entries[i], k, v);

		}

		pthread_mutex_unlock(&map->lock); 
	}
}

void hash_map_remove_entry(struct hash_map* map, void* k) {
	if (k != NULL && map != NULL) { 
		pthread_mutex_lock(&map->lock);
		int i = map->hash(k) % map->capacity; 
		struct entry* cursor = map->entries[i];
		if (cursor == NULL) { 
			pthread_mutex_unlock(&map->lock); 
			return; 
		}

		while (cursor->next != NULL) {
			if (map->cmp(cursor->key, k) == true) {
				remove_entry(map, cursor, i); 
			}
			cursor = cursor->next;
		}
		if (map->cmp(cursor->key, k) == true) {
			remove_entry(map, cursor, i); 
		}
		pthread_mutex_unlock(&map->lock);
	}
}

void* hash_map_get_value_ref(struct hash_map* map, void* k) {
	if (k != NULL && map != NULL) { 
		pthread_mutex_lock(&map->lock);
		int i = map->hash(k) % map->capacity;
		struct entry* cursor = map->entries[i];
		if (cursor == NULL) { 
			pthread_mutex_unlock(&map->lock); 
			return NULL; 
		}

		while (cursor->next != NULL) {
			if (map->cmp(cursor->key, k) == true) {
				pthread_mutex_unlock(&map->lock); 
				return cursor->value;
			}
			cursor = cursor->next;
		}
		if (map->cmp(cursor->key, k) == true) {
			pthread_mutex_unlock(&map->lock); 
			return cursor->value;
		} 
		pthread_mutex_unlock(&map->lock);
		return NULL;
	} else {
		return NULL; 
	}
}

void hash_map_destroy(struct hash_map* map) {
	if (map == NULL) { return; }

	for (int i = 0; i < map->capacity; i++) {
		struct entry* cursor = map->entries[i];
		while (cursor != NULL) {
			struct entry* next = cursor->next;
			destroy_entry(map, cursor); 
			cursor = next;
		}
	}
	pthread_mutex_destroy(&map->lock);
	free(map->entries);
	free(map); 
}