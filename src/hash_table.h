#ifndef HASH_TABLE_H
#define HASH_TABLE_H

// Defines a single key-value pair (item) in the hash table.
typedef struct {
    char* key;   // Pointer to the key (a string).
    char* value; // Pointer to the associated value (a string).
} ht_item;

// Defines the hash table structure.
typedef struct {
    int size;      // Total number of buckets (slots) in the hash table.
    int count;     // Number of items currently stored in the hash table.
    int base_size; // Starting number of buckets in the hash table.
    ht_item** items; // Array of pointers to ht_item (buckets for key-value pairs).
} ht_hash_table;

// Function declarations
ht_hash_table* ht_new(); // Declare ht_new
void ht_del_hash_table(ht_hash_table* ht); // Declare ht_del_hash_table
void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* ht, const char* key);

#endif // HASH_TABLE_H