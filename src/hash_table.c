#include <stdio.h>
#include <string.h>

#include "hash_table.h"

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
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*)); 
    // Allocates and zeroes out memory for the array of pointers (buckets).

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