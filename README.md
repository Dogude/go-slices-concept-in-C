# Visualizing Go Slices Mechanics (C Implementation)
An educational project that implements **Go-style Slices** in **C** to demonstrate exactly how Go manages dynamic arrays, length, capacity, and underlying memory.
This repository implements the same slice behaviors in both C and Go.

# What is a Slice ?
A slice is not an array. It is a **header** that points to an underlying array. It consists of three components (exactly 24 bytes on a 64-bit system):
1. **Data Pointer**: A pointer to the first element of the slice.
2. **Length (`len`)**: The number of elements currently in the slice(also as a write index into underlying array).
3. **Capacity (`cap`)**: The maximum number of elements the slice can hold.

# Memory Layout Simulation
The underlying structure in both languages looks like this(for ` int[] `  ):
| Struct Field     | C Representation | Go Representation |
| :---             | :---         | :---               |
| **Data Pointer** | `int *data`  | `array UNSAFE.Pointer` |
| **Length**       | `size_t len` | `int` |
| **Capacity**     | `size_t cap` | `int` |

# Memory Management
  * In this example, linked list with header metadata used to chain all slice memory allocations and free them all once at the end of main. 
# Code Comparison
  ```go
      Go:
      s := make([]int, 0, 5) // Automatically zeroes out memory
  ``` 
  ```c
      C:
      Slice s = make(0, 5); // Automatically zeroes out memory using memset
  ```
  ```go
      Go:
      s = append(s, 12) // If len(s) < cap(s), no allocation will happen, existing memory will be used
  ```

  ```c
      C:
      s = append(s, 12); // If len(s) < cap(s), no allocation will happen, existing memory will be used
  ```
  * Creating a subslice will not allocate a memory, it just pointing to same main slice`s memory
  ```go
     Go:
     s2 := s[2:5]
  ```
  ```c
     C:
     s2 = slice(s, 2, 5); // s2.data = s.data + low; 
  ```
  ```c
  int main() {
    Slice s = make(0, 5);
    s = append(s, 12);
    s = append(s, 13);
    s = append(s, 139);
    s = append(s, 140);
    s = append(s, 141); // Triggers reallocation (len(s) == cap(s))
    s = append(s, 142);

    Slice s2 = make(4, 10);
    s2 = slice(s2, 2, 5);

    printf("%zu %zu\n", len(s2), cap(s2)); // Output: 3 8
    PrintSlice(s);                         // Output: [ 12 13 139 140 141 142 ]
    
    release(); // Custom cleanup
}
```

