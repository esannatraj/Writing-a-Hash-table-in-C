#include <stdio.h>
#include <string.h>

#include "hash_table.h"

// Prime numbers for double hashing
static const int HT_PRIME_1 = 31;
static const int HT_PRIME_2 = 37;
static ht_item HT_DELETED_ITEM = {NULL, NULL};

// Creates a new hash table item (key-value pair).
static ht_item* ht_new_item(char* k, char* v) {
    ht_item* i = malloc(sizeof(ht_item)); // Allocates memory for the item.
    i->key = strdup(k);                  // Creates a copy of the key string.
    i->value = strdup(v);                // Creates a copy of the value string.

    return i; // Returns a pointer to the newly created item.
}

// Creates and initializes a new hash table.
ht_hash_table* ht_new() {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table)); // Allocates memory for the hash table.
    ht->size = 53;                                    // Sets the initial size of the hash table (prime number).
    ht->count = 0;                                    // Initializes the item count to 0.
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*)); // Allocates and zeroes out memory for the array of pointers (buckets).

    return ht; // Returns a pointer to the newly created hash table.
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
static int ht_hash(char* s, const int a, const int m) {
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

// Inserts a new item into the hash table
void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    ht_item* item = ht_new_item(key, value); // Create a new key-value item.
    int index = ht_get_hash(item->key, ht->size, 0); // Compute initial bucket index.
    ht_item* cur_item = ht->items[index]; // Get the current item at the bucket.
    int i = 1; // Initialize probe attempt counter.
    while (cur_item != NULL && cur_item !=  &HT_DELETED_ITEM) { // Continue probing if the bucket is occupied.
        index = ht_get_hash(item->key, ht->size, i); // Compute the next index.
        cur_item = ht->items[index]; // Check the next bucket.
        i++; // Increment probe attempt counter.
    }
    ht->items[index] = item; // Place the item in the first empty bucket.
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
