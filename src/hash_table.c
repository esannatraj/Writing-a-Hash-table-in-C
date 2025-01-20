#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>   


#include "hash_table.h"
#include "prime.h"

// Prime numbers for double hashing
static const int HT_PRIME_1 = 31;
static const int HT_PRIME_2 = 37;
static ht_item HT_DELETED_ITEM = {NULL, NULL};
static int HT_INITIAL_BASE_SIZE = 53;

static ht_item* ht_new_item(const char* k, const char* v);
static int ht_hash(const char* s, const int a, const int m);
static void ht_resize_up(ht_hash_table* ht);
static void ht_resize_down(ht_hash_table* ht);

// Creates a new hash table item (key-value pair).
static ht_item* ht_new_item(const char* k, const char* v) {
    ht_item* i = malloc(sizeof(ht_item)); // Allocates memory for the item.
    i->key = strdup(k);                  // Creates a copy of the key string.
    i->value = strdup(v);                // Creates a copy of the value string.

    return i; // Returns a pointer to the newly created item.
}

// Creates a new hash table with a specified base size.
static ht_hash_table* ht_new_sized(const int base_size) {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table)); // Allocate memory for the hash table structure.
    ht->base_size = base_size;                         // Store the base size for resizing logic.

    ht->size = next_prime(ht->base_size);              // Set the table size to the next prime >= base_size.
    ht->count = 0;                                     // Initialize the count of items to 0.

    // Allocate memory for the hash table buckets, initializing all to NULL.
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
    return ht;                                         // Return the initialized hash table.
}

// Creates a new hash table with the default initial size.
ht_hash_table* ht_new() {
    return ht_new_sized(HT_INITIAL_BASE_SIZE);         // Call the sizing function with the default base size.
}


// Frees memory associated with a hash table item.
static void ht_del_item(ht_item* i) {
    free(i->key);   // Frees the dynamically allocated memory for the key.
    free(i->value); // Frees the dynamically allocated memory for the value.
    free(i);        // Frees the memory for the item structure itself.
}

// Frees all memory associated with a hash table, including its items and buckets.
void ht_del_hash_table(ht_hash_table* ht) {
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i]; // Accesses the item at each bucket.
        if (item != NULL) {           // If the bucket is not empty, free the item.
            ht_del_item(item);
        }
    }
    free(ht->items); // Frees the memory for the array of buckets.
    free(ht);        // Frees the memory for the hash table structure itself.
}

// Hashes a string to a bucket index using a custom hash function.
static int ht_hash(const char* s, const int a, const int m) {
    long hash = 0;               // Initializes the hash value.
    const int len_s = strlen(s); // Gets the length of the string.

    // Processes each character in the string.
    for (int i = 0; i < len_s; i++) {
        // Computes the weighted ASCII value for the current character.
        hash += (long)pow(a, len_s - (i+1)) * s[i];
        // Applies the modulus operator to keep the hash value manageable.
        hash = hash % m; 
    }

    return (int)hash; // Returns the computed hash value as an integer.
}

// Computes a hash for the given string `s` using double hashing.
static int ht_get_hash(const char* s, const int num_buckets, const int attempts) {
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets); // Primary hash determines the initial bucket index.
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets); // Secondary hash determines the step size for probing.

    return (hash_a + (attempts * (hash_b + 1))) % num_buckets; // Combine hashes and calculate the final bucket index, wrapping with modulo.
}

// Inserts a new item into the hash table, replacing any existing item with the same key.
void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    const int load = ht->count * 100 / ht->size; // Avoids dealing with floating points
    if (load > 70) {
        ht_resize_up(ht);
    }
    ht_item* item = ht_new_item(key, value); // Create a new key-value item.
    int index = ht_get_hash(item->key, ht->size, 0); // Compute initial bucket index.
    ht_item* cur_item = ht->items[index]; // Get the current item at the bucket.
    int i = 1; // Initialize probe attempt counter.
    // Check if an item with the same key already exists.
    while (cur_item != NULL) {
        while (cur_item != &HT_DELETED_ITEM) { // Skip deleted buckets.
            if (strcmp(cur_item->key, key) == 0) { // Check if keys match.
                ht_del_item(cur_item); // Delete the existing item to avoid duplication.
                ht->items[index] = item; // Replace the item at the current bucket.
                return; // Exit after replacing the item.
            }
        }
    }
    // Continue probing until an empty or deleted bucket is found.
    while (cur_item != NULL && cur_item != &HT_DELETED_ITEM) {
        index = ht_get_hash(item->key, ht->size, i); // Compute the next index.
        cur_item = ht->items[index]; // Check the next bucket.
        i++; // Increment probe attempt counter.
    }

    ht->items[index] = item; // Place the item in the first empty or deleted bucket.
    ht->count++; // Increment the count of items in the hash table.
}


// Search for a value in the hash table by key.
char* ht_search(ht_hash_table* ht, const char* key) {
    int index = ht_get_hash(key, ht->size, 0); // Compute initial bucket index.
    ht_item* item = ht->items[index];         // Get the item at the initial index.
    int i = 1;                                // Initialize probe attempt counter.
    while (item != NULL) { // Continue probing if the bucket is not empty.
        if (item != &HT_DELETED_ITEM) { //If the bucket does not contain an item marked deleted
            if (strcmp(item->key, key) == 0) { // Check if the current item's key matches the search key.
            return item->value;               // Return the value if the key matches.
            }
        }        
        index = ht_get_hash(key, ht->size, i); // Compute the next index using double hashing.
        item = ht->items[index];              // Check the item at the new index.
        i++;                                  // Increment probe attempt counter.
    }
    return NULL;                              // Return NULL if the key is not found.
}

// Delete an item from the hash table by key.
void ht_delete(ht_hash_table* ht, const char* key) {
    const int load = ht->count * 100 / ht->size;
    if (load < 10) {
        ht_resize_down(ht);
    }
    int index = ht_get_hash(key, ht->size, 0); // Compute initial bucket index.
    ht_item* item = ht->items[index];         // Get the item at the initial index.
    int i = 1;                                // Initialize probe attempt counter.
    while (item != NULL) {                    // Continue probing until an empty bucket is found.
        if (item != &HT_DELETED_ITEM) {       // Skip already deleted buckets.
            if (strcmp(item->key, key) == 0) { // Check if the current item's key matches.
                ht_del_item(item);            // Free memory for the matched item.
                ht->items[index] = &HT_DELETED_ITEM; // Mark the bucket as deleted.
            }
        }
        index = ht_get_hash(key, ht->size, i); // Compute the next index using double hashing.
        item = ht->items[index];              // Get the item at the new index.
        i++;                                  // Increment probe attempt counter.
    }
    ht->count--;                              // Decrease the count of items in the hash table.
}

// Resizes the hash table to a new base size, reallocating and rehashing items.
static void ht_resize(ht_hash_table* ht, const int base_size) {
    // Ensure the base size doesn't drop below the initial minimum size.
    if (base_size < HT_INITIAL_BASE_SIZE) {
        return; // Skip resizing if the requested size is too small.
    }

    // Create a new hash table with the specified base size.
    ht_hash_table* new_ht = ht_new_sized(base_size);

    // Rehash all valid items from the old table into the new table.
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) { // Skip empty and deleted buckets.
            ht_insert(new_ht, item->key, item->value); // Reinsert item into the new hash table.
        }
    }

    // Update the old hash table's metadata to match the new table's base size and item count.
    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    // Swap the old hash table's size and items with the new hash table's.
    const int tmp_size = ht->size;         // Temporarily store the old size.
    ht->size = new_ht->size;               // Update size to the new table's size.
    new_ht->size = tmp_size;               // Assign old size to the temporary table.
    ht_item** tmp_items = ht->items;       // Temporarily store the old items array.
    ht->items = new_ht->items;             // Update items to point to the new table's array.
    new_ht->items = tmp_items;             // Assign old items array to the temporary table.

    // Delete the temporary new hash table, which now holds the old data.
    ht_del_hash_table(new_ht);
}

// Doubles the size of the hash table to reduce the load factor and collisions.
static void ht_resize_up(ht_hash_table* ht) {
    const int new_size = ht->base_size * 2; // Calculate the new size by doubling the base size.
    ht_resize(ht, new_size);               // Call the resizing function to expand the hash table.
}

// Halves the size of the hash table to save memory when underutilized.
static void ht_resize_down(ht_hash_table* ht) {
    const int new_size = ht->base_size / 2; // Calculate the new size by halving the base size.
    ht_resize(ht, new_size);               // Call the resizing function to shrink the hash table.
}
