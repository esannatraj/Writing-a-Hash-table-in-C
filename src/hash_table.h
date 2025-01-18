// Defines a single key-value pair (item) in the hash table.
typedef struct {
    char* key;   // Pointer to the key (a string).
    char* value; // Pointer to the associated value (a string).
} ht_item;

// Defines the hash table structure.
typedef struct {
    int size;      // Total number of buckets (slots) in the hash table.
    int count;     // Number of items currently stored in the hash table.
    ht_item** items; // Array of pointers to ht_item (buckets for key-value pairs).
} ht_hash_table;