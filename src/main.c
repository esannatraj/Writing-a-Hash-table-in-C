#include <stdio.h>
#include "hash_table.h"

int main() {
    ht_hash_table* ht = ht_new();
    ht_insert(ht, "key1", "value1");
    ht_insert(ht, "key2", "value2");

    printf("Search key1: %s\n", ht_search(ht, "key1"));
    printf("Search key2: %s\n", ht_search(ht, "key2"));

    ht_delete(ht, "key1");
    printf("Search key1 after deletion: %s\n", ht_search(ht, "key1"));

    ht_del_hash_table(ht);
    return 0;
}
