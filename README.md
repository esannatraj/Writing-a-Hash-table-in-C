## Writing a Hash Table in C

### **Project Overview**
This project implements a **hash table** in the C programming language. A hash table is a data structure that provides fast insertion, deletion, and lookup operations. The project includes features like:
- **Dynamic resizing** to maintain optimal performance.
- **Double hashing** for collision resolution.
- Support for inserting, searching, and deleting key-value pairs.

The primary purpose of this project is to demonstrate the implementation of a hash table from scratch while adhering to efficient and modular coding practices.

---

### **File Structure**
```
.
├── build/                   # Directory for compiled output
├── src/                     # Source code directory
│   ├── hash_table.c         # Implementation of the hash table
│   ├── hash_table.h         # Header file for the hash table
│   ├── main.c               # Entry point and usage examples
│   ├── prime.c              # Helper functions for prime number calculations
│   └── prime.h              # Header file for prime number helper functions
├── Makefile                 # Build configuration file
```

---

### **How It Works**
#### **Key Components**
1. **Hash Table Implementation (`hash_table.c`)**:
   - Uses **double hashing** to handle collisions efficiently.
   - Dynamically resizes when the load factor exceeds thresholds.

2. **Prime Utilities (`prime.c`)**:
   - Provides helper functions to find the next prime number for hash table resizing.

3. **Main Program (`main.c`)**:
   - Demonstrates basic usage of the hash table, including insertion, search, and deletion.

---

### **Setup Instructions**
#### **Requirements**
- A C compiler like `gcc`.
- A build system like `make`.

#### **Build Instructions**
1. Clone or download this repository to your system.
2. Navigate to the project directory:
   ```bash
   cd Writing-a-Hash-table-in-C
   ```
3. Build the project using `make`:
   ```bash
   make
   ```
4. The compiled executable will be located in the `build/` directory as `hash_table_program`.

#### **Run the Program**
Execute the program to see a demonstration of the hash table:
```bash
./build/hash_table_program
```

---

### **Key Features**
1. **Dynamic Resizing**:
   - The hash table doubles its size when the load factor exceeds 70%.
   - It halves its size when the load factor drops below 10%.

2. **Collision Handling**:
   - Implements **double hashing** to resolve key collisions effectively.

3. **Efficiency**:
   - Uses prime numbers for table sizes to minimize collisions.

4. **Memory Management**:
   - Ensures proper allocation and deallocation of memory to prevent leaks.

---

### **Sample Output**
Here’s an example of the program’s output when executed:
```
Search key1: value1
Search key2: value2
Search key1 after deletion: (null)
```

This demonstrates inserting, searching, and deleting key-value pairs in the hash table.

---

### **Code Highlights**
#### **Hashing Function**
The hash function combines a primary and secondary hash to compute the bucket index:
```c
static int ht_get_hash(const char* s, const int num_buckets, const int attempts) {
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempts * (hash_b + 1))) % num_buckets;
}
```

#### **Dynamic Resizing**
The hash table resizes dynamically to maintain efficiency:
```c
static void ht_resize_up(ht_hash_table* ht) {
    const int new_size = ht->base_size * 2;
    ht_resize(ht, new_size);
}
```

---

### **Future Enhancements**
- Add support for more complex data types as values.
- Implement serialization for saving and loading the hash table to/from disk.
- Introduce multithreading support for concurrent operations.

---

### **Credits**
This project was developed as part of a learning exercise to implement core data structures in C. 