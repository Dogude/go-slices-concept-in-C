# Visualizing Go Slices Mechanics (C Implementation)
An educational project that implements **Go-style Slices** in **C** to demonstrate exactly how Go manages dynamic arrays, length, capacity, and underlying memory.
This repository implements the same slice behaviors in both C and Go.

# What is a Slice ?
A slice is not an array. It is a **header** that points to an underlying array. It consists of three components (exactly 24 bytes on a 64-bit system):
1. **Data Pointer**: A pointer to the first element of the slice.
2. **Length (`len`)**: The number of elements currently in the slice(also as a write index into underlying array).
3. **Capacity (`cap`)**: The maximum number of elements the slice can hold.
* Reallocation only happens when len(s) == cap(s)
* in that case whether subslice or root slice
* data pointer will point different location on memory and old existing items will be copied to new location 

# Memory Layout Simulation
The underlying structure in both languages looks like this(for ` int[] `  ):
| Struct Field     | C Representation | Go Representation |
| :---             | :---         | :---               |
| **Data Pointer** | `int *data`  | `data uintptr` |
| **Length**       | `size_t len` | `len int` |
| **Capacity**     | `size_t cap` | `cap int` |

# Memory Management
  * In this example, linked list with header metadata used to chain all slice memory allocations and free them all once at the end of main.
  * This implementation is for educational purposes; in a real Go code, legacy memory should be freed,
  * sub-slices can also point to the same memory. In Go, this problem is solved with automatic garbage collection. 
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
     s2 = slice(s, 2, 5); // s2.data = s.data + low; this gives a address of shifted data pointer of root slice 
  ```
  ```c
  // C Equivalent:
  int main() {
    Slice s = make(0, 5);
    s = append(s, 12);
    s = append(s, 13);
    s = append(s, 139);
    s = append(s, 140);
    s = append(s, 141); // Triggers reallocation (len(s) == cap(s)) old items will copied to new memory location, after that line s is no longer points to old address
    s = append(s, 142); 

    PrintSlice(s);                         // Output: [12 13 139 140 141 142]

    Slice s2 = make(4, 10); // len == 4 , cap 10 , creates zeroed-out 10xint, append write index is len (4) 
    s2 = slice(s2, 2, 5);   // creating a subslice, len will be 5 - 2 , cap will be 10 - 2

    printf("%zu %zu\n", len(s2), cap(s2)); // Output: 3 8
    
    
    release(); // Custom cleanup
}
```

```go
    // Go Equivalent:
    package main
    
    import "fmt"
    
    func main() {
    	s := make([]int, 0, 5)
    	s = append(s, 12)
    	s = append(s, 13)
    	s = append(s, 139)
    	s = append(s, 140)
    	s = append(s, 141) // Triggers reallocation (len(s) == cap(s))
    	s = append(s, 142)
    
    	s2 := make([]int, 4, 10)
    	s2 = s2[2:5]
    
    	fmt.Printf("%d %d\n", len(s2), cap(s2)) // Output: 3 8
    	fmt.Println(s)   // Output: [12 13 139 140 141 142]

    }
```

