# C/C++ Compatibility Features

## Overview

KLang provides optional C/C++ compatibility features that allow low-level programming when needed, while maintaining safety by default. These features must be explicitly enabled as they bypass some of KLang's safety guarantees.

## Enabling C/C++ Features

C/C++ features are disabled by default for safety. Enable them via environment variables:

```bash
# Enable all C/C++ features
export KLANG_ENABLE_POINTERS=1
export KLANG_ENABLE_MANUAL_MEMORY=1
export KLANG_ENABLE_INLINE_ASM=1
```

Or check the current status:

```bash
klang config
```

## Pointer Operations

### Basic Pointers

When `KLANG_ENABLE_POINTERS=1` is set:

```klang
# Declare a pointer
let ptr: *int = null

# Take address of variable
let x = 42
ptr = &x

# Dereference pointer
let y = *ptr  # y = 42

# Pointer arithmetic
ptr = ptr + 1
ptr++
ptr--
```

### Pointer Types

```klang
# Integer pointer
let int_ptr: *int

# Float pointer
let float_ptr: *float

# String pointer
let str_ptr: *string

# Void pointer (generic pointer)
let void_ptr: *void

# Pointer to pointer
let ptr_ptr: **int

# Array pointer
let arr_ptr: *[int]
```

### Null Pointers

```klang
# Check for null
let ptr: *int = null
if (ptr == null) {
    println("Pointer is null")
}

# Safe dereference
if (ptr != null) {
    let value = *ptr
}
```

## Manual Memory Management

### Allocation and Deallocation

When `KLANG_ENABLE_MANUAL_MEMORY=1` is set:

```klang
# Allocate memory
let ptr = malloc(sizeof(int))

# Use memory
*ptr = 42

# Free memory
free(ptr)
ptr = null  # Good practice
```

### Array Allocation

```klang
# Allocate array
let size = 100
let arr = malloc(size * sizeof(int))

# Use array
arr[0] = 10
arr[1] = 20

# Free array
free(arr)
```

### Struct Allocation

```klang
struct Point {
    x: int,
    y: int
}

# Allocate struct
let pt = malloc(sizeof(Point))

# Access fields
pt->x = 10
pt->y = 20

# Or use (*pt).x
(*pt).x = 10

# Free struct
free(pt)
```

### Memory Management Functions

```klang
# malloc - allocate memory
let ptr = malloc(1024)

# calloc - allocate and zero-initialize
let ptr = calloc(10, sizeof(int))

# realloc - resize allocation
ptr = realloc(ptr, 2048)

# free - deallocate memory
free(ptr)

# sizeof - get size of type
let size = sizeof(int)
let struct_size = sizeof(Point)
```

## Structs and Unions

### Struct Definition

```klang
struct Person {
    name: string,
    age: int,
    height: float
}

# Create struct
let person = Person {
    name: "Alice",
    age: 30,
    height: 1.65
}

# Access fields
println(person.name)
person.age = 31

# Pointer to struct
let ptr = &person
ptr->age = 32
```

### Nested Structs

```klang
struct Address {
    street: string,
    city: string,
    zip: int
}

struct Employee {
    name: string,
    address: Address,
    salary: float
}

let emp = Employee {
    name: "Bob",
    address: Address {
        street: "123 Main St",
        city: "Springfield",
        zip: 12345
    },
    salary: 75000.0
}

println(emp.address.city)
```

### Unions

```klang
union Data {
    i: int,
    f: float,
    s: string
}

let data = Data()
data.i = 42
println(data.i)

data.f = 3.14
println(data.f)  # Warning: overwrites integer value
```

## Type Definitions

### Typedef

```klang
# Basic typedef
typedef int32 = int
typedef uint = unsigned int

# Pointer typedef
typedef IntPtr = *int

# Function pointer typedef
typedef Callback = fn(int, int) -> int

# Struct typedef
typedef Point2D = struct {
    x: float,
    y: float
}
```

## Bitwise Operations

KLang supports all C/C++ bitwise operators:

```klang
let a = 0b1010  # Binary literal: 10
let b = 0b1100  # Binary literal: 12

# Bitwise AND
let c = a & b   # 0b1000 = 8

# Bitwise OR
let d = a | b   # 0b1110 = 14

# Bitwise XOR
let e = a ^ b   # 0b0110 = 6

# Bitwise NOT
let f = ~a      # 0b...0101 (complement)

# Left shift
let g = a << 2  # 0b101000 = 40

# Right shift
let h = a >> 1  # 0b0101 = 5

# Compound assignments
a &= b
a |= b
a ^= b
a <<= 2
a >>= 1
```

### Bit Manipulation

```klang
# Set bit
fn setBit(num: int, pos: int) -> int {
    return num | (1 << pos)
}

# Clear bit
fn clearBit(num: int, pos: int) -> int {
    return num & ~(1 << pos)
}

# Toggle bit
fn toggleBit(num: int, pos: int) -> int {
    return num ^ (1 << pos)
}

# Check bit
fn checkBit(num: int, pos: int) -> bool {
    return (num & (1 << pos)) != 0
}

let x = 0b1010
x = setBit(x, 0)     # 0b1011
x = clearBit(x, 3)   # 0b0011
x = toggleBit(x, 1)  # 0b0001
let bit = checkBit(x, 0)  # true
```

## Inline Assembly

When `KLANG_ENABLE_INLINE_ASM=1` is set:

```klang
# Basic inline assembly
asm {
    "mov eax, 1"
    "mov ebx, 2"
    "add eax, ebx"
}

# With inputs and outputs
fn add_asm(a: int, b: int) -> int {
    let result: int
    asm {
        "add %1, %2"
        "mov %0, %1"
        : "=r"(result)
        : "r"(a), "r"(b)
    }
    return result
}

# SIMD operations (x86-64)
fn simd_add(a: *float, b: *float, result: *float, count: int) {
    asm {
        "movups xmm0, [%0]"
        "movups xmm1, [%1]"
        "addps xmm0, xmm1"
        "movups [%2], xmm0"
        : 
        : "r"(a), "r"(b), "r"(result)
        : "xmm0", "xmm1"
    }
}
```

## Memory Alignment

Control memory alignment for performance:

```klang
# Aligned allocation
let ptr = aligned_alloc(16, 1024)  # 16-byte aligned

# Struct alignment
struct __attribute__((aligned(16))) AlignedData {
    x: float,
    y: float,
    z: float,
    w: float
}

# Check alignment
let alignment = alignof(AlignedData)  # Returns 16
```

## Volatile and Const

### Volatile Qualifier

For hardware registers and multi-threaded access:

```klang
# Volatile variable (always read from memory)
let volatile status: int = 0

# Volatile pointer
let volatile_ptr: volatile *int
```

### Const Qualifier

```klang
# Constant variable
const MAX_SIZE: int = 1024

# Const pointer
let const_ptr: const *int

# Pointer to const
let ptr_to_const: *const int
```

## Low-Level Examples

### Custom Memory Allocator

```klang
struct MemoryPool {
    buffer: *byte,
    size: int,
    used: int
}

fn createPool(size: int) -> *MemoryPool {
    let pool = malloc(sizeof(MemoryPool))
    pool->buffer = malloc(size)
    pool->size = size
    pool->used = 0
    return pool
}

fn allocFromPool(pool: *MemoryPool, size: int) -> *void {
    if (pool->used + size > pool->size) {
        return null
    }
    
    let ptr = pool->buffer + pool->used
    pool->used += size
    return ptr as *void
}

fn destroyPool(pool: *MemoryPool) {
    free(pool->buffer)
    free(pool)
}
```

### Ring Buffer

```klang
struct RingBuffer {
    buffer: *int,
    capacity: int,
    head: int,
    tail: int,
    count: int
}

fn createRingBuffer(capacity: int) -> *RingBuffer {
    let rb = malloc(sizeof(RingBuffer))
    rb->buffer = malloc(capacity * sizeof(int))
    rb->capacity = capacity
    rb->head = 0
    rb->tail = 0
    rb->count = 0
    return rb
}

fn push(rb: *RingBuffer, value: int) -> bool {
    if (rb->count >= rb->capacity) {
        return false
    }
    
    rb->buffer[rb->tail] = value
    rb->tail = (rb->tail + 1) % rb->capacity
    rb->count++
    return true
}

fn pop(rb: *RingBuffer) -> int? {
    if (rb->count == 0) {
        return null
    }
    
    let value = rb->buffer[rb->head]
    rb->head = (rb->head + 1) % rb->capacity
    rb->count--
    return value
}
```

### Bit Field Struct

```klang
struct Flags {
    enabled: 1 bit,
    visible: 1 bit,
    active: 1 bit,
    reserved: 5 bits
}

let flags = Flags()
flags.enabled = 1
flags.visible = 1
flags.active = 0
```

## Safety Considerations

### Memory Leaks

```klang
# BAD: Memory leak
fn createData() {
    let ptr = malloc(1024)
    # forgot to free!
}

# GOOD: Proper cleanup
fn createAndUseData() {
    let ptr = malloc(1024)
    # use ptr...
    free(ptr)
}

# BETTER: Use defer
fn smartAlloc() {
    let ptr = malloc(1024)
    defer free(ptr)
    
    # use ptr...
    # automatically freed when function returns
}
```

### Dangling Pointers

```klang
# BAD: Dangling pointer
let ptr: *int = null
{
    let x = 42
    ptr = &x
}  # x goes out of scope
# ptr is now dangling!

# GOOD: Keep data alive
let x = 42
let ptr = &x
# use ptr...
# both ptr and x in scope
```

### Buffer Overflows

```klang
# BAD: Buffer overflow
let arr = malloc(10 * sizeof(int))
arr[20] = 42  # Out of bounds!

# GOOD: Bounds checking
let arr = malloc(10 * sizeof(int))
let index = 20
if (index < 10) {
    arr[index] = 42
}

# BETTER: Use safe arrays
let arr = [int; 10]  # Fixed-size array with bounds checking
arr[20] = 42  # Runtime error - bounds check
```

## Best Practices

1. **Enable features only when needed** - Keep safety features on by default
2. **Use defer for cleanup** - Automatically free resources
3. **Check for null** - Always check pointers before dereferencing
4. **Bounds check arrays** - Prevent buffer overflows
5. **Initialize pointers** - Always initialize to null
6. **Free memory** - Always free what you allocate
7. **Use smart pointers when possible** - Let the language manage memory
8. **Document unsafe code** - Mark and document why manual memory management is needed

## Mixing Safe and Unsafe Code

```klang
# Safe KLang code (default)
fn safeFn() {
    let x = [1, 2, 3, 4, 5]  # Safe array with automatic memory management
    println(x[0])
}

# Unsafe C-style code
fn unsafeFn() {
    # Enable for this function only
    let ptr = malloc(5 * sizeof(int))
    defer free(ptr)
    
    ptr[0] = 1
    ptr[1] = 2
    # ...
}

# Call both
safeFn()    # Safe
unsafeFn()  # Unsafe but controlled
```

## Performance Considerations

C/C++ features provide maximum performance when needed:

- **Pointers**: Zero-cost abstraction for direct memory access
- **Manual memory**: Control over allocation/deallocation
- **Inline assembly**: Maximum control for critical paths
- **Bitwise operations**: Efficient bit manipulation

However, use them judiciously as they sacrifice safety for performance.

## See Also

- [Build Modes Guide](BUILD_MODES.md)
- [Performance Tuning](PERFORMANCE.md)
- [Memory Management](MEMORY.md)
- [API Reference](API_REFERENCE.md)
