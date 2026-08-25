# Visualizing Go Slices Mechanics (C Implementation)
An educational project that implements **Go-style Slices** in **C** to demonstrate exactly how Go manages dynamic arrays, length, capacity, and underlying memory.
This repository implements the same slice behaviors in both C and Go.

# What is a Slice ?
A slice is not an array. It is a **header** that points to an underlying array. It consists of three components (exactly 24 bytes on a 64-bit system):
1. **Data Pointer**: A pointer to the first element of the slice.
2. **Length (`len`)**: The number of elements currently in the slice(also as a write index into underlying array).
3. **Capacity (`cap`)**: The maximum number of elements the slice can hold.

# Memory Layout Simulation
The underlying structure in both languages looks like this(for ``` int[] ``` ):
| Struct Field     | C Representation | Go Representation |
| :---             | :---         | :---               |
| **Data Pointer** | `int *data`  | `array UNSAFE.Pointer` |
| **Length**       | `size_t len` | `int` |
| **Capacity**     | `size_t cap` | `int` |

# Memory Management
  * In this example,  
# Code Comparison
  ```go
      s := make([]int, 0, 5)
  ``` 
  ```c
      Slice s = make(0, 5); // Automatically zeroes out memory using memset
  ```
  ```go
      s = append(s, 12)
  ```

  ```c
      s = append(s, 12); // If len < cap, no allocation will happen
  ```
