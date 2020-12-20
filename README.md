# Hashy

A hash map will need to be able to store any type in conjunction with the function pointers provided during construction. 

This hash map is thread safe and performant, which ensure that the hash map can support concurrent insertions and removals. Retrieving and placing an element should be constant time on average O(1)​ operation.

In this instance, it is advised you construct a hash map where each entry mapped by a hash is its own list (separate chaining). If more than one distinct key results in the same hash, the list should contain these keys at the same location.

## Usage

### Run the Program

``` shell
make all
```

### Clean the Program

``` shell
make clean
```

### Sample Output

``` shell
example1 begin
example2 begin
entry 0 ------- NONE -------
entry 1 ------- Phoebe, 328-------Lesi, 083-------Tyson, 242-------
entry 2 ------- NONE -------
entry 3 ------- John, 202-------
entry 4 ------- James, 245-------
entry 5 ------- Edward, 192-------Erik, 24-------Cheng, 190-------
This hashmap should include Cheng
entry 0 ------- NONE -------
entry 1 ------- Phoebe, 328-------
entry 2 ------- NONE -------
entry 3 ------- NONE -------
entry 4 ------- James, 111-------
entry 5 ------- Cheng, 190-------
example1 end
This hashmap should not include Change
entry 0 ------- NONE -------
entry 1 ------- Phoebe, 328-------
entry 2 ------- NONE -------
entry 3 ------- NONE -------
entry 4 ------- James, 111-------
entry 5 ------- NONE -------
example2 end
```

## Functionality

### Creation

To ensure we have a hashmap which can be used with any kind of hashing function and that we have a comparison function and destruction function for complex objects. The hash function will not involve the compression component, you will need to compress the hash to fit your implementation. If the hash, cmp or destruction function pointers are `NULL`, this function should return `NULL`.

```
struct hashmap* hash_map_new(size_t (*hash)(void*), int (*cmp)(void*,void*),
    void (*key_destruct)(void*), void (*value_destruct)(void*));
```

### Put

The put functions will insert a key and value into the hashmap, in the event the key exists within the hashmap, the value will be replaced, since the data structure owns the data, it will need to free the data. If the key does not exists, a new entry should be created and the key and value assumed to be owned by the hash map.

```
void hash_map_put_entry_move(struct hash_map* map, void* k, void* v);
```

### Remove

If the key exists with the hashmap, it will be removed as well as a the value, if the key does not exist, nothing will be removed. 

```
void hash_map_remove_entry(struct hash_map* map, void* k);
```

### Get

Your hashmap should retrieve the value given the key. In the event the key does not exist, your function should return `NULL`.

```
void* hash_map_get_value_ref(struct hash_map* map, void* k);
```

### Destroy

This will destroy the current entries and the map itself. The destruct function will be called on each object to ensure that it is freed. Destroys all entries within the hash map, using the destructor function. It will also free the map after all entries have been removed. If the map is `NULL`, nothing will occur.
```
void hash_map_destroy(struct hash_map* map);
```
