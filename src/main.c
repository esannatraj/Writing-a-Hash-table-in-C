#include <stdio.h>
#include "hash_table.h"

int main() {
    ht_hash_table* ht = ht_new(); // Create a new hash table.

    // Insert key-value pairs.
    ht_insert(ht, "key1", "value1");
    ht_insert(ht, "key2", "value2");

    // Search for keys and print their values.
    printf("Search key1: %s\n", ht_search(ht, "key1")); // Expected: "value1"
    printf("Search key2: %s\n", ht_search(ht, "key2")); // Expected: "value2"

    // Delete a key and verify it no longer exists.
    ht_delete(ht, "key1");
    printf("Search key1 after deletion: %s\n", ht_search(ht, "key1")); // Expected: "(null)"

    // Clean up.
    ht_del_hash_table(ht);
    return 0;
}
