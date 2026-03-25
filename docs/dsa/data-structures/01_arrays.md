# Arrays in KLang - Data Structures & Algorithms

## Table of Contents
1. [Introduction](#introduction)
2. [What is an Array?](#what-is-an-array)
3. [Array Declaration and Initialization](#array-declaration-and-initialization)
4. [Array Operations](#array-operations)
5. [Common Array Algorithms](#common-array-algorithms)
6. [Time and Space Complexity](#time-and-space-complexity)
7. [Best Practices](#best-practices)
8. [Interview Problems](#interview-problems)
9. [Examples](#examples)

---

## Introduction

Arrays are the most fundamental data structure in programming. They store elements of the same type in contiguous memory locations, allowing efficient access by index.

### Why Use Arrays?

- **Fast Access**: O(1) time to access any element by index
- **Memory Efficient**: Elements stored contiguously
- **Simple**: Easy to understand and use
- **Foundation**: Basis for many other data structures

---

## What is an Array?

An **array** is a collection of elements stored in contiguous memory locations. Each element can be accessed using an index.

### Characteristics

- **Fixed or Dynamic Size**: Size can be fixed or grow dynamically
- **Indexed**: Elements accessed via zero-based index
- **Homogeneous**: All elements same type (in typed languages)
- **Contiguous**: Elements stored sequentially in memory

### Visual Representation

```
Array: [10, 20, 30, 40, 50]
Index:  0   1   2   3   4
```

---

## Array Declaration and Initialization

### Basic Declaration

```klang
# Empty array
let arr = []

# Array with initial values
let numbers = [1, 2, 3, 4, 5]

# Array of strings
let names = ["Alice", "Bob", "Charlie"]

# Array with type annotation
let scores: array = [95, 87, 92, 88]
```

### Creating Arrays with Specific Size

```klang
# Create array and populate
let arr = []
let i = 0
while (i < 10) {
    arr.push(i)
    i = i + 1
}
# arr = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```

### Multi-Dimensional Arrays

```klang
# 2D array (matrix)
let matrix = [
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
]

# Access elements
let element = matrix[1][2]  # Value: 6
```

---

## Array Operations

### Basic Operations

#### 1. Access Element

```klang
let arr = [10, 20, 30, 40, 50]
let first = arr[0]      # 10
let third = arr[2]      # 30
let last = arr[4]       # 50

# Time Complexity: O(1)
```

#### 2. Modify Element

```klang
let arr = [10, 20, 30]
arr[1] = 99
# arr = [10, 99, 30]

# Time Complexity: O(1)
```

#### 3. Insert at End (Push)

```klang
let arr = [1, 2, 3]
arr.push(4)
# arr = [1, 2, 3, 4]

# Time Complexity: O(1) amortized
```

#### 4. Remove from End (Pop)

```klang
let arr = [1, 2, 3, 4]
let last = arr.pop()
# arr = [1, 2, 3], last = 4

# Time Complexity: O(1)
```

#### 5. Insert at Beginning (Unshift)

```klang
let arr = [2, 3, 4]
arr.unshift(1)
# arr = [1, 2, 3, 4]

# Time Complexity: O(n) - must shift all elements
```

#### 6. Remove from Beginning (Shift)

```klang
let arr = [1, 2, 3, 4]
let first = arr.shift()
# arr = [2, 3, 4], first = 1

# Time Complexity: O(n) - must shift all elements
```

#### 7. Get Length

```klang
let arr = [10, 20, 30]
let size = len(arr)  # 3

# Time Complexity: O(1)
```

### Advanced Operations

#### 1. Slicing

```klang
let arr = [1, 2, 3, 4, 5]
# Get subarray from index 1 to 3 (exclusive)
let sub = arr.slice(1, 3)
# sub = [2, 3]
```

#### 2. Concatenation

```klang
let arr1 = [1, 2, 3]
let arr2 = [4, 5, 6]
let combined = arr1.concat(arr2)
# combined = [1, 2, 3, 4, 5, 6]
```

#### 3. Reversing

```klang
let arr = [1, 2, 3, 4, 5]
arr.reverse()
# arr = [5, 4, 3, 2, 1]
```

#### 4. Finding Elements

```klang
let arr = [10, 20, 30, 40]

# Find index of element
let index = arr.indexOf(30)  # 2

# Check if element exists
let exists = arr.includes(25)  # false
```

---

## Common Array Algorithms

### 1. Linear Search

Find an element in an unsorted array.

```klang
fn linearSearch(arr, target: int) -> int {
    let i = 0
    while (i < len(arr)) {
        if (arr[i] == target) {
            return i
        }
        i = i + 1
    }
    return -1  # Not found
}

let arr = [64, 34, 25, 12, 22, 11, 90]
let index = linearSearch(arr, 22)  # Returns 4
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1)

### 2. Binary Search

Find an element in a **sorted** array.

```klang
fn binarySearch(arr, target: int) -> int {
    let left = 0
    let right = len(arr) - 1
    
    while (left <= right) {
        let mid = (left + right) / 2
        
        if (arr[mid] == target) {
            return mid
        } else if (arr[mid] < target) {
            left = mid + 1
        } else {
            right = mid - 1
        }
    }
    
    return -1  # Not found
}

let arr = [1, 3, 5, 7, 9, 11, 13, 15]
let index = binarySearch(arr, 7)  # Returns 3
```

**Time Complexity**: O(log n)  
**Space Complexity**: O(1)

### 3. Finding Maximum Element

```klang
fn findMax(arr) -> int {
    if (len(arr) == 0) {
        return 0  # Or handle error
    }
    
    let max = arr[0]
    let i = 1
    while (i < len(arr)) {
        if (arr[i] > max) {
            max = arr[i]
        }
        i = i + 1
    }
    return max
}

let arr = [64, 34, 25, 12, 22, 90, 11]
let maximum = findMax(arr)  # Returns 90
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1)

### 4. Reversing an Array

```klang
fn reverseArray(arr) {
    let left = 0
    let right = len(arr) - 1
    
    while (left < right) {
        # Swap elements
        let temp = arr[left]
        arr[left] = arr[right]
        arr[right] = temp
        
        left = left + 1
        right = right - 1
    }
}

let arr = [1, 2, 3, 4, 5]
reverseArray(arr)
# arr = [5, 4, 3, 2, 1]
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1)

### 5. Rotating an Array

```klang
fn rotateRight(arr, k: int) {
    let n = len(arr)
    k = k % n  # Handle k > n
    
    # Reverse entire array
    reverse(arr, 0, n - 1)
    # Reverse first k elements
    reverse(arr, 0, k - 1)
    # Reverse remaining elements
    reverse(arr, k, n - 1)
}

fn reverse(arr, start: int, end: int) {
    while (start < end) {
        let temp = arr[start]
        arr[start] = arr[end]
        arr[end] = temp
        start = start + 1
        end = end - 1
    }
}

let arr = [1, 2, 3, 4, 5]
rotateRight(arr, 2)
# arr = [4, 5, 1, 2, 3]
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1)

---

## Time and Space Complexity

### Operation Complexities

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Access by index | O(1) | O(1) |
| Search (unsorted) | O(n) | O(1) |
| Search (sorted) | O(log n) | O(1) |
| Insert at end | O(1) amortized | O(1) |
| Insert at beginning | O(n) | O(1) |
| Delete from end | O(1) | O(1) |
| Delete from beginning | O(n) | O(1) |
| Delete from middle | O(n) | O(1) |

### Why Different Complexities?

- **O(1)**: Direct index access, no iteration needed
- **O(n)**: Must iterate through elements
- **O(log n)**: Binary search divides problem in half each time

---

## Best Practices

### 1. Prefer Built-in Methods

```klang
# Good: Use built-in method
let max = Math.max(...arr)

# Less efficient: Manual loop
let max = arr[0]
for (let i = 1; i < len(arr); i++) {
    if (arr[i] > max) {
        max = arr[i]
    }
}
```

### 2. Check Bounds

```klang
# Good: Check before access
if (index >= 0 && index < len(arr)) {
    let value = arr[index]
}

# Bad: May cause error
let value = arr[index]
```

### 3. Use Appropriate Algorithm

```klang
# For sorted array: Use binary search
if (isSorted(arr)) {
    index = binarySearch(arr, target)  # O(log n)
} else {
    index = linearSearch(arr, target)   # O(n)
}
```

### 4. Consider Space-Time Tradeoffs

```klang
# Time-efficient (with extra space)
let frequency = {}
for (let num in arr) {
    frequency[num] = (frequency[num] || 0) + 1
}

# Space-efficient (slower)
fn countOccurrences(arr, target) {
    let count = 0
    for (let num in arr) {
        if (num == target) count++
    }
    return count
}
```

---

## Interview Problems

### Problem 1: Two Sum

Find two numbers that add up to a target.

```klang
fn twoSum(arr, target: int) -> array {
    let seen = {}
    
    let i = 0
    while (i < len(arr)) {
        let complement = target - arr[i]
        
        if (complement in seen) {
            return [seen[complement], i]
        }
        
        seen[arr[i]] = i
        i = i + 1
    }
    
    return []  # Not found
}

let arr = [2, 7, 11, 15]
let result = twoSum(arr, 9)  # Returns [0, 1]
```

**Time**: O(n), **Space**: O(n)

### Problem 2: Maximum Subarray Sum (Kadane's Algorithm)

```klang
fn maxSubarraySum(arr) -> int {
    let maxSum = arr[0]
    let currentSum = arr[0]
    
    let i = 1
    while (i < len(arr)) {
        currentSum = max(arr[i], currentSum + arr[i])
        maxSum = max(maxSum, currentSum)
        i = i + 1
    }
    
    return maxSum
}

let arr = [-2, 1, -3, 4, -1, 2, 1, -5, 4]
let result = maxSubarraySum(arr)  # Returns 6 (subarray [4,-1,2,1])
```

**Time**: O(n), **Space**: O(1)

### Problem 3: Remove Duplicates from Sorted Array

```klang
fn removeDuplicates(arr) -> int {
    if (len(arr) == 0) {
        return 0
    }
    
    let uniqueIndex = 0
    let i = 1
    
    while (i < len(arr)) {
        if (arr[i] != arr[uniqueIndex]) {
            uniqueIndex = uniqueIndex + 1
            arr[uniqueIndex] = arr[i]
        }
        i = i + 1
    }
    
    return uniqueIndex + 1  # New length
}

let arr = [1, 1, 2, 2, 3, 4, 4]
let newLength = removeDuplicates(arr)
# arr = [1, 2, 3, 4, ...], newLength = 4
```

**Time**: O(n), **Space**: O(1)

---

## Examples

### Example 1: Array Statistics

```klang
class ArrayStats {
    let data: array = []
    
    fn init(arr) {
        this.data = arr
    }
    
    fn mean() -> float {
        if (len(this.data) == 0) {
            return 0.0
        }
        
        let sum = 0.0
        let i = 0
        while (i < len(this.data)) {
            sum = sum + float(this.data[i])
            i = i + 1
        }
        
        return sum / float(len(this.data))
    }
    
    fn max() -> int {
        if (len(this.data) == 0) {
            return 0
        }
        
        let maximum = this.data[0]
        let i = 1
        while (i < len(this.data)) {
            if (this.data[i] > maximum) {
                maximum = this.data[i]
            }
            i = i + 1
        }
        return maximum
    }
    
    fn min() -> int {
        if (len(this.data) == 0) {
            return 0
        }
        
        let minimum = this.data[0]
        let i = 1
        while (i < len(this.data)) {
            if (this.data[i] < minimum) {
                minimum = this.data[i]
            }
            i = i + 1
        }
        return minimum
    }
}

# Usage
let stats = new ArrayStats([64, 34, 25, 12, 22, 11, 90])
println("Mean: " + str(stats.mean()))
println("Max: " + str(stats.max()))
println("Min: " + str(stats.min()))
```

---

## Summary

- **Arrays** store elements in contiguous memory
- **Access** by index is O(1)
- **Search** is O(n) unsorted, O(log n) sorted
- **Insert/Delete** at end is O(1), at beginning is O(n)
- Use appropriate algorithms for better performance
- Consider time-space tradeoffs

---

## Next Steps

Continue to:
- [Strings](02_strings.md)
- [Linked Lists](03_linked_lists.md)
- [Sorting Algorithms](../algorithms/01_sorting.md)

Happy coding! 🚀
