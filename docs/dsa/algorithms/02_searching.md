# Searching Algorithms in KLang - Data Structures & Algorithms

## Table of Contents
1. [Introduction](#introduction)
2. [What is Searching?](#what-is-searching)
3. [Linear Search](#linear-search)
4. [Binary Search](#binary-search)
5. [Binary Search Variants](#binary-search-variants)
6. [Interpolation Search](#interpolation-search)
7. [Exponential Search](#exponential-search)
8. [Jump Search](#jump-search)
9. [Ternary Search](#ternary-search)
10. [Search in Rotated Arrays](#search-in-rotated-arrays)
11. [Comparison Table](#comparison-table)
12. [Interview Problems](#interview-problems)
13. [Best Practices](#best-practices)
14. [Common Pitfalls](#common-pitfalls)

---

## Introduction

Searching is one of the most fundamental operations in computer science, essential for retrieving information from data structures. Understanding search algorithms is crucial for:

- **Data Retrieval**: Finding specific elements efficiently
- **Algorithm Optimization**: Many algorithms depend on efficient searching
- **Database Operations**: Query optimization and indexing
- **Interview Preparation**: Core topic in technical interviews
- **Problem Solving**: Foundation for divide-and-conquer approaches

This comprehensive guide covers all major searching algorithms with complete implementations in KLang, complexity analysis, and practical applications.

---

## What is Searching?

**Searching** is the algorithmic process of finding a particular element or determining its absence in a collection of data.

### Why is Searching Important?

1. **Information Retrieval**: Core operation in databases and file systems
2. **Algorithm Foundation**: Basis for many advanced algorithms
3. **Performance Critical**: Can be bottleneck in applications
4. **Real-world Applications**: From web search to AI
5. **Problem Solving**: Essential technique for competitive programming

### Types of Searching

#### 1. Sequential Searching
- Linear Search
- Sentinel Linear Search
- Works on unsorted data

#### 2. Interval Searching
- Binary Search
- Interpolation Search
- Jump Search
- Exponential Search
- Ternary Search
- Requires sorted data

#### 3. Specialized Searching
- Hashing
- Tree-based search (BST, AVL, Red-Black)
- Graph search (BFS, DFS)

### Key Concepts

- **Sequential Access**: Examining elements one by one
- **Random Access**: Direct access to any element (arrays)
- **Divide and Conquer**: Breaking problem into smaller subproblems
- **Search Space**: The range in which we search for the target
- **Bounds**: Lower and upper limits of search space

---

## Linear Search

### Overview

Linear Search (also called Sequential Search) is the simplest searching algorithm that checks every element in the collection sequentially until the target is found or the end is reached.

### How It Works

1. Start from the first element
2. Compare each element with the target
3. If match found, return the index
4. If end reached without finding, return -1

### Visual Representation

```
Target: 7
Array: [3, 9, 2, 7, 5, 1]
        ↓
Step 1: 3 ≠ 7, continue
           ↓
Step 2: 9 ≠ 7, continue
              ↓
Step 3: 2 ≠ 7, continue
                 ↓
Step 4: 7 = 7, FOUND! Return index 3
```

### Complete Implementation

```klang
// Basic Linear Search
fn linearSearch(arr, target) {
    let n = arr.length()
    
    for (let i = 0; i < n; i = i + 1) {
        if (arr[i] == target) {
            return i  // Target found at index i
        }
    }
    
    return -1  // Target not found
}

// Linear Search with multiple occurrences
fn linearSearchAll(arr, target) {
    let n = arr.length()
    let indices = []
    
    for (let i = 0; i < n; i = i + 1) {
        if (arr[i] == target) {
            indices.push(i)
        }
    }
    
    return indices
}

// Linear Search from the end
fn linearSearchReverse(arr, target) {
    let n = arr.length()
    
    for (let i = n - 1; i >= 0; i = i - 1) {
        if (arr[i] == target) {
            return i
        }
    }
    
    return -1
}

// Sentinel Linear Search (optimized)
fn sentinelLinearSearch(arr, target) {
    let n = arr.length()
    let last = arr[n - 1]
    
    // Place target at the end as sentinel
    arr[n - 1] = target
    let i = 0
    
    // No need to check bounds - sentinel guarantees we'll find it
    while (arr[i] != target) {
        i = i + 1
    }
    
    // Restore last element
    arr[n - 1] = last
    
    // Check if we found the actual target or the sentinel
    if (i < n - 1 || last == target) {
        return i
    }
    
    return -1
}

// Example usage
let numbers = [64, 34, 25, 12, 22, 11, 90]
let target = 12

let index = linearSearch(numbers, target)
if (index != -1) {
    print("Element found at index: " + index)
} else {
    print("Element not found")
}

// Find all occurrences
let duplicates = [5, 2, 8, 2, 9, 2, 3]
let allIndices = linearSearchAll(duplicates, 2)
print("Found at indices: " + allIndices)  // [1, 3, 5]
```

### Complexity Analysis

| Case | Time Complexity | Space Complexity |
|------|----------------|------------------|
| Best | O(1) | O(1) |
| Average | O(n) | O(1) |
| Worst | O(n) | O(1) |

- **Best Case**: Target is first element
- **Average Case**: Target is in the middle
- **Worst Case**: Target is last element or not present
- **Space**: Only uses constant extra space

### Characteristics

- **Works on**: Sorted and unsorted arrays
- **Data Structure**: Arrays, linked lists (any sequential structure)
- **Stability**: N/A (not a sorting operation)
- **In-place**: ✓ Yes

### When to Use Linear Search

✅ **Use When**:
- Array is small (< 100 elements)
- Array is unsorted and searching once
- Data structure doesn't support random access (linked lists)
- Simplicity is preferred

❌ **Avoid When**:
- Array is large and sorted (use Binary Search)
- Multiple searches on same dataset (pre-sort and use Binary Search)
- Performance is critical

---

## Binary Search

### Overview

Binary Search is an efficient algorithm for searching in a **sorted** array. It works by repeatedly dividing the search interval in half, eliminating half of the remaining elements in each step.

### How It Works

1. Compare target with middle element
2. If equal, return the index
3. If target is smaller, search left half
4. If target is larger, search right half
5. Repeat until found or interval is empty

### Visual Representation

```
Target: 23
Sorted Array: [2, 5, 8, 12, 16, 23, 38, 56, 72, 91]
               L                 M                 R

Step 1: mid = (0 + 9) / 2 = 4
        arr[4] = 16 < 23, search right half
               [2, 5, 8, 12, 16, 23, 38, 56, 72, 91]
                                  L   M           R

Step 2: mid = (5 + 9) / 2 = 7
        arr[7] = 56 > 23, search left half
               [2, 5, 8, 12, 16, 23, 38, 56, 72, 91]
                                  L M  R

Step 3: mid = (5 + 6) / 2 = 5
        arr[5] = 23 = 23, FOUND! Return index 5
```

### Complete Implementation - Iterative

```klang
// Iterative Binary Search
fn binarySearch(arr, target) {
    let left = 0
    let right = arr.length() - 1
    
    while (left <= right) {
        // Avoid overflow: mid = left + (right - left) / 2
        let mid = left + (right - left) / 2
        
        if (arr[mid] == target) {
            return mid  // Target found
        } else if (arr[mid] < target) {
            left = mid + 1  // Search right half
        } else {
            right = mid - 1  // Search left half
        }
    }
    
    return -1  // Target not found
}

// Example usage
let sortedNumbers = [2, 5, 8, 12, 16, 23, 38, 56, 72, 91]
let target = 23

let index = binarySearch(sortedNumbers, target)
if (index != -1) {
    print("Element found at index: " + index)
} else {
    print("Element not found")
}
```

### Complete Implementation - Recursive

```klang
// Recursive Binary Search
fn binarySearchRecursive(arr, target, left, right) {
    // Base case: element not found
    if (left > right) {
        return -1
    }
    
    let mid = left + (right - left) / 2
    
    if (arr[mid] == target) {
        return mid  // Target found
    } else if (arr[mid] < target) {
        // Search right half
        return binarySearchRecursive(arr, target, mid + 1, right)
    } else {
        // Search left half
        return binarySearchRecursive(arr, target, left, mid - 1)
    }
}

// Wrapper function
fn binarySearchRec(arr, target) {
    return binarySearchRecursive(arr, target, 0, arr.length() - 1)
}

// Example usage
let sortedNumbers = [2, 5, 8, 12, 16, 23, 38, 56, 72, 91]
let target = 56

let index = binarySearchRec(sortedNumbers, target)
print("Element found at index: " + index)  // 7
```

### Step-by-Step Example

```
Search for 16 in [2, 5, 8, 12, 16, 23, 38, 56, 72, 91]

Iteration 1:
  left=0, right=9, mid=4
  arr[4]=16 == 16 → FOUND!
  
Search for 72 in [2, 5, 8, 12, 16, 23, 38, 56, 72, 91]

Iteration 1:
  left=0, right=9, mid=4
  arr[4]=16 < 72 → search right
  
Iteration 2:
  left=5, right=9, mid=7
  arr[7]=56 < 72 → search right
  
Iteration 3:
  left=8, right=9, mid=8
  arr[8]=72 == 72 → FOUND!
```

### Complexity Analysis

| Case | Time Complexity | Space Complexity |
|------|----------------|------------------|
| Best | O(1) | O(1) iterative / O(log n) recursive |
| Average | O(log n) | O(1) iterative / O(log n) recursive |
| Worst | O(log n) | O(1) iterative / O(log n) recursive |

- **Best Case**: Target is middle element
- **Average/Worst Case**: O(log n) comparisons
- **Space**: 
  - Iterative: O(1) extra space
  - Recursive: O(log n) call stack space

### Characteristics

- **Requires**: Sorted array
- **Random Access**: Must support direct indexing
- **Efficiency**: Much faster than linear search for large arrays
- **Divide and Conquer**: Divides problem size by 2 each step

### When to Use Binary Search

✅ **Use When**:
- Array is sorted
- Large dataset (> 100 elements)
- Multiple searches on same dataset
- Random access is available

❌ **Avoid When**:
- Array is unsorted (unless sorting cost is justified)
- Data structure doesn't support random access
- Very small arrays (linear search may be faster)

---

## Binary Search Variants

### 1. Find First Occurrence

Find the leftmost position of target in array with duplicates.

```klang
// Find first occurrence of target
fn findFirst(arr, target) {
    let left = 0
    let right = arr.length() - 1
    let result = -1
    
    while (left <= right) {
        let mid = left + (right - left) / 2
        
        if (arr[mid] == target) {
            result = mid  // Store potential answer
            right = mid - 1  // Continue searching left
        } else if (arr[mid] < target) {
            left = mid + 1
        } else {
            right = mid - 1
        }
    }
    
    return result
}

// Example
let arr = [1, 2, 2, 2, 3, 4, 5]
print(findFirst(arr, 2))  // Output: 1
```

### 2. Find Last Occurrence

Find the rightmost position of target in array with duplicates.

```klang
// Find last occurrence of target
fn findLast(arr, target) {
    let left = 0
    let right = arr.length() - 1
    let result = -1
    
    while (left <= right) {
        let mid = left + (right - left) / 2
        
        if (arr[mid] == target) {
            result = mid  // Store potential answer
            left = mid + 1  // Continue searching right
        } else if (arr[mid] < target) {
            left = mid + 1
        } else {
            right = mid - 1
        }
    }
    
    return result
}

// Example
let arr = [1, 2, 2, 2, 3, 4, 5]
print(findLast(arr, 2))  // Output: 3
```

### 3. Count Occurrences

Count total occurrences of target using first and last positions.

```klang
// Count occurrences of target
fn countOccurrences(arr, target) {
    let first = findFirst(arr, target)
    
    if (first == -1) {
        return 0  // Element not found
    }
    
    let last = findLast(arr, target)
    return last - first + 1
}

// Example
let arr = [1, 2, 2, 2, 3, 4, 5]
print(countOccurrences(arr, 2))  // Output: 3
print(countOccurrences(arr, 6))  // Output: 0
```

### 4. Find Insertion Position

Find the index where target should be inserted to maintain sorted order.

```klang
// Find insertion position (lower bound)
fn searchInsert(arr, target) {
    let left = 0
    let right = arr.length() - 1
    let result = arr.length()  // Default: insert at end
    
    while (left <= right) {
        let mid = left + (right - left) / 2
        
        if (arr[mid] >= target) {
            result = mid  // Potential insertion point
            right = mid - 1  // Look for earlier position
        } else {
            left = mid + 1
        }
    }
    
    return result
}

// Example
let arr = [1, 3, 5, 6]
print(searchInsert(arr, 5))  // Output: 2
print(searchInsert(arr, 2))  // Output: 1
print(searchInsert(arr, 7))  // Output: 4
print(searchInsert(arr, 0))  // Output: 0
```

---

## Interpolation Search

### Overview

Interpolation Search is an improved variant of Binary Search for **uniformly distributed** sorted arrays. Instead of always checking the middle element, it estimates the position of the target based on its value.

### How It Works

1. Estimate position using interpolation formula
2. If target matches, return position
3. If target is smaller, search left part
4. If target is larger, search right part
5. Repeat until found or bounds cross

### Formula

```
pos = left + ((target - arr[left]) * (right - left)) / (arr[right] - arr[left])
```

### Visual Representation

```
Target: 82
Array: [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]

Binary Search would check: 50 (middle)
Interpolation estimates: pos ≈ 7 (based on value 82)
  → Checks 80, much closer!
```

### Complete Implementation

```klang
// Interpolation Search
fn interpolationSearch(arr, target) {
    let left = 0
    let right = arr.length() - 1
    
    while (left <= right && target >= arr[left] && target <= arr[right]) {
        // Handle uniform array
        if (left == right) {
            if (arr[left] == target) {
                return left
            }
            return -1
        }
        
        // Estimate position using interpolation formula
        let pos = left + ((target - arr[left]) * (right - left)) / 
                        (arr[right] - arr[left])
        
        if (arr[pos] == target) {
            return pos  // Target found
        } else if (arr[pos] < target) {
            left = pos + 1  // Search right
        } else {
            right = pos - 1  // Search left
        }
    }
    
    return -1  // Target not found
}

// Example usage
let uniformArray = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
let target = 70

let index = interpolationSearch(uniformArray, target)
print("Element found at index: " + index)  // 6
```

### Complexity Analysis

| Case | Time Complexity | Space Complexity |
|------|----------------|------------------|
| Best | O(1) | O(1) |
| Average | O(log log n) | O(1) |
| Worst | O(n) | O(1) |

- **Best Case**: Target found immediately
- **Average Case**: O(log log n) for uniformly distributed data
- **Worst Case**: O(n) for non-uniform distribution
- **Space**: O(1) extra space

### When to Use Interpolation Search

✅ **Use When**:
- Data is uniformly distributed
- Array is sorted
- Large dataset with numeric values

❌ **Avoid When**:
- Data has non-uniform distribution
- Small arrays (overhead not worth it)
- Non-numeric data

---

## Exponential Search

### Overview

Exponential Search (also called Doubling Search or Galloping Search) is particularly useful for **unbounded** or **infinite** arrays. It finds the range where the element might be present and then performs binary search.

### How It Works

1. Start with bound = 1
2. Keep doubling bound until arr[bound] >= target
3. Perform binary search between bound/2 and bound

### Visual Representation

```
Target: 42
Array: [2, 3, 5, 8, 11, 15, 20, 28, 35, 42, 50, 63, 78, 90, 105]
        ↓     ↓        ↓                 ↓
     bound=1  2        4                 8 (28 < 42)
                                              ↓
                                          bound=16 (50 >= 42)
                                          
Binary Search in range [8, 15]
```

### Complete Implementation

```klang
// Exponential Search
fn exponentialSearch(arr, target) {
    let n = arr.length()
    
    // If target is at first position
    if (arr[0] == target) {
        return 0
    }
    
    // Find range for binary search by doubling
    let bound = 1
    while (bound < n && arr[bound] < target) {
        bound = bound * 2
    }
    
    // Perform binary search in found range
    let left = bound / 2
    let right = min(bound, n - 1)
    
    return binarySearchRange(arr, target, left, right)
}

// Binary search in a specific range
fn binarySearchRange(arr, target, left, right) {
    while (left <= right) {
        let mid = left + (right - left) / 2
        
        if (arr[mid] == target) {
            return mid
        } else if (arr[mid] < target) {
            left = mid + 1
        } else {
            right = mid - 1
        }
    }
    
    return -1
}

// Helper function
fn min(a, b) {
    if (a < b) {
        return a
    }
    return b
}

// Example usage
let arr = [2, 3, 5, 8, 11, 15, 20, 28, 35, 42, 50, 63, 78, 90, 105]
let target = 42

let index = exponentialSearch(arr, target)
print("Element found at index: " + index)  // 9
```

### Complexity Analysis

| Case | Time Complexity | Space Complexity |
|------|----------------|------------------|
| Best | O(1) | O(1) |
| Average | O(log n) | O(1) |
| Worst | O(log n) | O(1) |

- **Time**: O(log i) where i is the position of target
- **Space**: O(1) extra space
- **Doubling Phase**: O(log i) 
- **Binary Search Phase**: O(log i)

### When to Use Exponential Search

✅ **Use When**:
- Searching in unbounded/infinite arrays
- Target is likely to be near the beginning
- Array size is unknown

❌ **Avoid When**:
- Array size is known and small
- Target likely near the end

---

## Jump Search

### Overview

Jump Search is a searching algorithm for **sorted arrays** that works by jumping ahead by fixed steps and then performing linear search in the identified block.

### How It Works

1. Calculate jump size = √n
2. Jump forward until arr[jump] >= target
3. Perform linear search in previous block
4. Return index if found

### Visual Representation

```
Target: 55
Array: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, ..., 98, 99] (100 elements)
Jump size = √100 = 10

Step 1: Check arr[0] = 0 < 55, jump to 10
Step 2: Check arr[10] = 10 < 55, jump to 20
Step 3: Check arr[20] = 20 < 55, jump to 30
Step 4: Check arr[30] = 30 < 55, jump to 40
Step 5: Check arr[40] = 40 < 55, jump to 50
Step 6: Check arr[50] = 50 < 55, jump to 60
Step 7: Check arr[60] = 60 > 55, linear search in [50, 60)
Step 8: Linear search finds 55 at index 55
```

### Complete Implementation

```klang
// Jump Search
fn jumpSearch(arr, target) {
    let n = arr.length()
    let jump = sqrt(n)  // Optimal jump size
    let prev = 0
    
    // Jump forward until we overshoot or reach end
    while (prev < n && arr[min(jump, n) - 1] < target) {
        prev = jump
        jump = jump + sqrt(n)
        
        // If we've gone past the array
        if (prev >= n) {
            return -1
        }
    }
    
    // Linear search in identified block
    while (prev < n && arr[prev] < target) {
        prev = prev + 1
        
        // If we reach next block or end
        if (prev == min(jump, n)) {
            return -1
        }
    }
    
    // Check if element found
    if (prev < n && arr[prev] == target) {
        return prev
    }
    
    return -1
}

// Square root function (integer approximation)
fn sqrt(n) {
    if (n < 2) {
        return n
    }
    
    let x = n
    let y = (x + 1) / 2
    
    while (y < x) {
        x = y
        y = (x + n / x) / 2
    }
    
    return x
}

// Helper function
fn min(a, b) {
    if (a < b) {
        return a
    }
    return b
}

// Example usage
let arr = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
let target = 11

let index = jumpSearch(arr, target)
print("Element found at index: " + index)  // 11
```

### Complexity Analysis

| Case | Time Complexity | Space Complexity |
|------|----------------|------------------|
| Best | O(1) | O(1) |
| Average | O(√n) | O(1) |
| Worst | O(√n) | O(1) |

- **Time**: O(√n) - optimal jump size
- **Space**: O(1) extra space
- **Jumps**: O(√n) jumps
- **Linear Search**: O(√n) comparisons

### When to Use Jump Search

✅ **Use When**:
- Array is sorted
- Binary search overhead is high
- Backward jumping is expensive (linked arrays)

❌ **Avoid When**:
- Random access is very fast (use Binary Search)
- Array is very small

---

## Ternary Search

### Overview

Ternary Search is a divide-and-conquer algorithm that divides the array into **three parts** instead of two. It's particularly useful for finding the maximum or minimum of a **unimodal function**.

### How It Works

1. Divide array into three parts using two midpoints
2. Compare target with both midpoints
3. Eliminate two-thirds or continue in one-third
4. Repeat until found or interval is empty

### Visual Representation

```
Target: 23
Array: [2, 5, 8, 12, 16, 23, 38, 56, 72, 91]
        L          M1      M2           R

Step 1: mid1 = L + (R-L)/3 = 3, mid2 = R - (R-L)/3 = 6
        arr[3]=12 < 23, arr[6]=38 > 23
        Search in middle third [4, 5]
```

### Complete Implementation

```klang
// Ternary Search (for sorted array)
fn ternarySearch(arr, target) {
    let left = 0
    let right = arr.length() - 1
    
    while (left <= right) {
        // Divide into three parts
        let mid1 = left + (right - left) / 3
        let mid2 = right - (right - left) / 3
        
        if (arr[mid1] == target) {
            return mid1
        }
        if (arr[mid2] == target) {
            return mid2
        }
        
        if (target < arr[mid1]) {
            // Search in first third
            right = mid1 - 1
        } else if (target > arr[mid2]) {
            // Search in last third
            left = mid2 + 1
        } else {
            // Search in middle third
            left = mid1 + 1
            right = mid2 - 1
        }
    }
    
    return -1
}

// Ternary Search for finding maximum in unimodal function
fn ternarySearchMax(arr) {
    let left = 0
    let right = arr.length() - 1
    
    while (right - left > 2) {
        let mid1 = left + (right - left) / 3
        let mid2 = right - (right - left) / 3
        
        if (arr[mid1] < arr[mid2]) {
            // Maximum is in right part
            left = mid1
        } else {
            // Maximum is in left part
            right = mid2
        }
    }
    
    // Check remaining elements
    let maxIdx = left
    for (let i = left + 1; i <= right; i = i + 1) {
        if (arr[i] > arr[maxIdx]) {
            maxIdx = i
        }
    }
    
    return maxIdx
}

// Example usage
let arr = [2, 5, 8, 12, 16, 23, 38, 56, 72, 91]
let target = 23

let index = ternarySearch(arr, target)
print("Element found at index: " + index)  // 5

// Find peak in unimodal array
let unimodal = [1, 3, 8, 12, 15, 18, 16, 10, 5, 2]
let peakIdx = ternarySearchMax(unimodal)
print("Peak element at index: " + peakIdx)  // 5 (value 18)
```

### Complexity Analysis

| Case | Time Complexity | Space Complexity |
|------|----------------|------------------|
| Best | O(1) | O(1) |
| Average | O(log₃ n) | O(1) |
| Worst | O(log₃ n) | O(1) |

- **Time**: O(log₃ n) ≈ O(log n)
- **Space**: O(1) extra space
- **Comparisons**: More comparisons than Binary Search
- **Note**: Despite O(log₃ n), Binary Search is usually faster due to fewer comparisons per iteration

### When to Use Ternary Search

✅ **Use When**:
- Finding maximum/minimum in unimodal function
- Array has special properties
- Theoretical interest

❌ **Avoid When**:
- Simple searching in sorted array (use Binary Search)
- Performance is critical (Binary Search is faster)

---

## Search in Rotated Arrays

### Overview

A rotated sorted array is a sorted array that has been rotated at some pivot. For example, `[4, 5, 6, 7, 0, 1, 2]` is a rotation of `[0, 1, 2, 4, 5, 6, 7]`.

### How It Works

1. Find which half is sorted using mid element
2. Check if target lies in sorted half
3. If yes, search in sorted half
4. Otherwise, search in unsorted half
5. Repeat until found

### Visual Representation

```
Original: [0, 1, 2, 4, 5, 6, 7]
Rotated:  [4, 5, 6, 7, 0, 1, 2]
                    ↑
                  pivot

The array is divided into two sorted subarrays:
Left part: [4, 5, 6, 7] (sorted)
Right part: [0, 1, 2] (sorted)
```

### Complete Implementation

```klang
// Search in Rotated Sorted Array
fn searchRotated(arr, target) {
    let left = 0
    let right = arr.length() - 1
    
    while (left <= right) {
        let mid = left + (right - left) / 2
        
        if (arr[mid] == target) {
            return mid
        }
        
        // Determine which half is sorted
        if (arr[left] <= arr[mid]) {
            // Left half is sorted
            if (target >= arr[left] && target < arr[mid]) {
                // Target is in sorted left half
                right = mid - 1
            } else {
                // Target is in unsorted right half
                left = mid + 1
            }
        } else {
            // Right half is sorted
            if (target > arr[mid] && target <= arr[right]) {
                // Target is in sorted right half
                left = mid + 1
            } else {
                // Target is in unsorted left half
                right = mid - 1
            }
        }
    }
    
    return -1
}

// Find pivot (minimum element) in rotated array
fn findPivot(arr) {
    let left = 0
    let right = arr.length() - 1
    
    while (left < right) {
        let mid = left + (right - left) / 2
        
        if (arr[mid] > arr[right]) {
            // Pivot is in right half
            left = mid + 1
        } else {
            // Pivot is in left half or at mid
            right = mid
        }
    }
    
    return left
}

// Search in Rotated Array (alternative approach)
fn searchRotatedAlternative(arr, target) {
    // First find the pivot
    let pivot = findPivot(arr)
    let n = arr.length()
    
    // If target is at pivot
    if (arr[pivot] == target) {
        return pivot
    }
    
    // Decide which half to search
    if (pivot == 0) {
        // Array is not rotated
        return binarySearchRange(arr, target, 0, n - 1)
    }
    
    if (target >= arr[0]) {
        // Search in left sorted part
        return binarySearchRange(arr, target, 0, pivot - 1)
    } else {
        // Search in right sorted part
        return binarySearchRange(arr, target, pivot, n - 1)
    }
}

// Example usage
let rotated = [4, 5, 6, 7, 0, 1, 2]
let target = 0

let index = searchRotated(rotated, target)
print("Element found at index: " + index)  // 4

let pivot = findPivot(rotated)
print("Pivot (minimum) at index: " + pivot)  // 4
```

### Step-by-Step Example

```
Search for 1 in [4, 5, 6, 7, 0, 1, 2]

Iteration 1:
  left=0, right=6, mid=3
  arr[3]=7, arr[0]=4, arr[6]=2
  Left half [4,5,6,7] is sorted
  1 not in [4,7], search right half
  
Iteration 2:
  left=4, right=6, mid=5
  arr[5]=1 == 1 → FOUND at index 5
```

### Complexity Analysis

| Case | Time Complexity | Space Complexity |
|------|----------------|------------------|
| Best | O(1) | O(1) |
| Average | O(log n) | O(1) |
| Worst | O(log n) | O(1) |

---

## Comparison Table

| Algorithm | Time (Best) | Time (Avg) | Time (Worst) | Space | Requires | Notes |
|-----------|-------------|------------|--------------|-------|----------|-------|
| **Linear Search** | O(1) | O(n) | O(n) | O(1) | None | Works on unsorted |
| **Binary Search** | O(1) | O(log n) | O(log n) | O(1) | Sorted array | Most efficient for sorted |
| **Interpolation** | O(1) | O(log log n) | O(n) | O(1) | Sorted, uniform | Best for uniform data |
| **Jump Search** | O(1) | O(√n) | O(√n) | O(1) | Sorted array | Good for large arrays |
| **Exponential** | O(1) | O(log n) | O(log n) | O(1) | Sorted array | Good for unbounded |
| **Ternary Search** | O(1) | O(log₃ n) | O(log₃ n) | O(1) | Sorted/Unimodal | For unimodal functions |
| **Rotated Binary** | O(1) | O(log n) | O(log n) | O(1) | Rotated sorted | Modified binary search |

### Key Takeaways

1. **Unsorted Data**: Only Linear Search works
2. **Sorted Data**: Binary Search is usually best
3. **Uniform Distribution**: Interpolation Search excels
4. **Unbounded Arrays**: Exponential Search is ideal
5. **Large Arrays**: Jump Search can be efficient
6. **Unimodal Functions**: Ternary Search finds extrema

---

## Interview Problems

### Problem 1: Binary Search (Basic)

**Problem**: Implement binary search to find target in sorted array.

```klang
fn search(nums, target) {
    let left = 0
    let right = nums.length() - 1
    
    while (left <= right) {
        let mid = left + (right - left) / 2
        
        if (nums[mid] == target) {
            return mid
        } else if (nums[mid] < target) {
            left = mid + 1
        } else {
            right = mid - 1
        }
    }
    
    return -1
}

// Test
print(search([1, 2, 3, 4, 5], 3))  // 2
print(search([1, 2, 3, 4, 5], 6))  // -1
```

**Time**: O(log n), **Space**: O(1)

---

### Problem 2: Search in Rotated Sorted Array

**Problem**: Search for a target value in a rotated sorted array.

```klang
fn searchRotated(nums, target) {
    let left = 0
    let right = nums.length() - 1
    
    while (left <= right) {
        let mid = left + (right - left) / 2
        
        if (nums[mid] == target) {
            return mid
        }
        
        // Check which half is sorted
        if (nums[left] <= nums[mid]) {
            // Left half is sorted
            if (target >= nums[left] && target < nums[mid]) {
                right = mid - 1
            } else {
                left = mid + 1
            }
        } else {
            // Right half is sorted
            if (target > nums[mid] && target <= nums[right]) {
                left = mid + 1
            } else {
                right = mid - 1
            }
        }
    }
    
    return -1
}

// Test
print(searchRotated([4, 5, 6, 7, 0, 1, 2], 0))  // 4
print(searchRotated([4, 5, 6, 7, 0, 1, 2], 3))  // -1
```

**Time**: O(log n), **Space**: O(1)

---

### Problem 3: Find First and Last Position

**Problem**: Find the starting and ending position of a target value.

```klang
fn searchRange(nums, target) {
    let result = [-1, -1]
    
    // Find first position
    result[0] = findFirst(nums, target)
    
    // If not found, return [-1, -1]
    if (result[0] == -1) {
        return result
    }
    
    // Find last position
    result[1] = findLast(nums, target)
    
    return result
}

fn findFirst(nums, target) {
    let left = 0
    let right = nums.length() - 1
    let result = -1
    
    while (left <= right) {
        let mid = left + (right - left) / 2
        
        if (nums[mid] == target) {
            result = mid
            right = mid - 1  // Continue searching left
        } else if (nums[mid] < target) {
            left = mid + 1
        } else {
            right = mid - 1
        }
    }
    
    return result
}

fn findLast(nums, target) {
    let left = 0
    let right = nums.length() - 1
    let result = -1
    
    while (left <= right) {
        let mid = left + (right - left) / 2
        
        if (nums[mid] == target) {
            result = mid
            left = mid + 1  // Continue searching right
        } else if (nums[mid] < target) {
            left = mid + 1
        } else {
            right = mid - 1
        }
    }
    
    return result
}

// Test
print(searchRange([5, 7, 7, 8, 8, 10], 8))  // [3, 4]
print(searchRange([5, 7, 7, 8, 8, 10], 6))  // [-1, -1]
```

**Time**: O(log n), **Space**: O(1)

---

### Problem 4: Search in 2D Matrix

**Problem**: Search for a value in an m x n matrix where each row is sorted.

```klang
fn searchMatrix(matrix, target) {
    if (matrix.length() == 0) {
        return false
    }
    
    let m = matrix.length()
    let n = matrix[0].length()
    
    // Treat as 1D sorted array
    let left = 0
    let right = m * n - 1
    
    while (left <= right) {
        let mid = left + (right - left) / 2
        
        // Convert 1D index to 2D coordinates
        let row = mid / n
        let col = mid % n
        let midValue = matrix[row][col]
        
        if (midValue == target) {
            return true
        } else if (midValue < target) {
            left = mid + 1
        } else {
            right = mid - 1
        }
    }
    
    return false
}

// Test
let matrix = [
    [1, 3, 5, 7],
    [10, 11, 16, 20],
    [23, 30, 34, 60]
]
print(searchMatrix(matrix, 3))   // true
print(searchMatrix(matrix, 13))  // false
```

**Time**: O(log(m × n)), **Space**: O(1)

---

### Problem 5: Find Peak Element

**Problem**: Find a peak element (element greater than its neighbors).

```klang
fn findPeakElement(nums) {
    let left = 0
    let right = nums.length() - 1
    
    while (left < right) {
        let mid = left + (right - left) / 2
        
        if (nums[mid] > nums[mid + 1]) {
            // Peak is in left half (including mid)
            right = mid
        } else {
            // Peak is in right half
            left = mid + 1
        }
    }
    
    return left
}

// Test
print(findPeakElement([1, 2, 3, 1]))       // 2
print(findPeakElement([1, 2, 1, 3, 5, 6, 4]))  // 5
```

**Time**: O(log n), **Space**: O(1)

---

### Problem 6: Search Insert Position

**Problem**: Find the index where target would be inserted in sorted array.

```klang
fn searchInsert(nums, target) {
    let left = 0
    let right = nums.length() - 1
    
    while (left <= right) {
        let mid = left + (right - left) / 2
        
        if (nums[mid] == target) {
            return mid
        } else if (nums[mid] < target) {
            left = mid + 1
        } else {
            right = mid - 1
        }
    }
    
    return left
}

// Test
print(searchInsert([1, 3, 5, 6], 5))  // 2
print(searchInsert([1, 3, 5, 6], 2))  // 1
print(searchInsert([1, 3, 5, 6], 7))  // 4
```

**Time**: O(log n), **Space**: O(1)

---

### Problem 7: Find Minimum in Rotated Sorted Array

**Problem**: Find the minimum element in a rotated sorted array.

```klang
fn findMin(nums) {
    let left = 0
    let right = nums.length() - 1
    
    while (left < right) {
        let mid = left + (right - left) / 2
        
        if (nums[mid] > nums[right]) {
            // Minimum is in right half
            left = mid + 1
        } else {
            // Minimum is in left half or at mid
            right = mid
        }
    }
    
    return nums[left]
}

// Test
print(findMin([3, 4, 5, 1, 2]))  // 1
print(findMin([4, 5, 6, 7, 0, 1, 2]))  // 0
```

**Time**: O(log n), **Space**: O(1)

---

### Problem 8: Sqrt(x) using Binary Search

**Problem**: Compute and return the square root of x (integer part).

```klang
fn mySqrt(x) {
    if (x < 2) {
        return x
    }
    
    let left = 1
    let right = x / 2
    let result = 0
    
    while (left <= right) {
        let mid = left + (right - left) / 2
        let square = mid * mid
        
        if (square == x) {
            return mid
        } else if (square < x) {
            result = mid  // Store potential answer
            left = mid + 1
        } else {
            right = mid - 1
        }
    }
    
    return result
}

// Test
print(mySqrt(4))   // 2
print(mySqrt(8))   // 2
print(mySqrt(16))  // 4
print(mySqrt(25))  // 5
```

**Time**: O(log n), **Space**: O(1)

---

### Problem 9: Median of Two Sorted Arrays

**Problem**: Find the median of two sorted arrays.

```klang
fn findMedianSortedArrays(nums1, nums2) {
    // Ensure nums1 is smaller
    if (nums1.length() > nums2.length()) {
        let temp = nums1
        nums1 = nums2
        nums2 = temp
    }
    
    let m = nums1.length()
    let n = nums2.length()
    let left = 0
    let right = m
    
    while (left <= right) {
        let partition1 = (left + right) / 2
        let partition2 = (m + n + 1) / 2 - partition1
        
        let maxLeft1 = -999999
        if (partition1 > 0) {
            maxLeft1 = nums1[partition1 - 1]
        }
        
        let minRight1 = 999999
        if (partition1 < m) {
            minRight1 = nums1[partition1]
        }
        
        let maxLeft2 = -999999
        if (partition2 > 0) {
            maxLeft2 = nums2[partition2 - 1]
        }
        
        let minRight2 = 999999
        if (partition2 < n) {
            minRight2 = nums2[partition2]
        }
        
        if (maxLeft1 <= minRight2 && maxLeft2 <= minRight1) {
            // Found correct partition
            if ((m + n) % 2 == 0) {
                return (max(maxLeft1, maxLeft2) + min(minRight1, minRight2)) / 2.0
            } else {
                return max(maxLeft1, maxLeft2)
            }
        } else if (maxLeft1 > minRight2) {
            right = partition1 - 1
        } else {
            left = partition1 + 1
        }
    }
    
    return 0.0
}

fn max(a, b) {
    if (a > b) {
        return a
    }
    return b
}

fn min(a, b) {
    if (a < b) {
        return a
    }
    return b
}

// Test
print(findMedianSortedArrays([1, 3], [2]))  // 2.0
print(findMedianSortedArrays([1, 2], [3, 4]))  // 2.5
```

**Time**: O(log(min(m, n))), **Space**: O(1)

---

### Problem 10: Find K Closest Elements

**Problem**: Find k closest elements to target in sorted array.

```klang
fn findClosestElements(arr, k, x) {
    let left = 0
    let right = arr.length() - k
    
    while (left < right) {
        let mid = left + (right - left) / 2
        
        // Compare distances from x
        if (x - arr[mid] > arr[mid + k] - x) {
            left = mid + 1
        } else {
            right = mid
        }
    }
    
    let result = []
    for (let i = left; i < left + k; i = i + 1) {
        result.push(arr[i])
    }
    
    return result
}

// Test
print(findClosestElements([1, 2, 3, 4, 5], 4, 3))  // [1, 2, 3, 4]
print(findClosestElements([1, 2, 3, 4, 5], 4, -1))  // [1, 2, 3, 4]
```

**Time**: O(log(n - k) + k), **Space**: O(k)

---

## Best Practices

### 1. Choose the Right Algorithm

```klang
fn chooseSearchAlgorithm(arr, target, isSorted) {
    if (!isSorted) {
        // Only option for unsorted data
        return linearSearch(arr, target)
    }
    
    let n = arr.length()
    
    if (n < 100) {
        // Small array - simplicity wins
        return linearSearch(arr, target)
    } else if (isUniformlyDistributed(arr)) {
        // Uniform data - interpolation is best
        return interpolationSearch(arr, target)
    } else {
        // Default for sorted data
        return binarySearch(arr, target)
    }
}
```

### 2. Avoid Integer Overflow

```klang
// ❌ Bad: Can overflow for large left + right
let mid = (left + right) / 2

// ✅ Good: Prevents overflow
let mid = left + (right - left) / 2
```

### 3. Handle Edge Cases

```klang
fn robustBinarySearch(arr, target) {
    // Handle empty array
    if (arr.length() == 0) {
        return -1
    }
    
    // Handle single element
    if (arr.length() == 1) {
        if (arr[0] == target) {
            return 0
        }
        return -1
    }
    
    // Standard binary search
    let left = 0
    let right = arr.length() - 1
    
    while (left <= right) {
        let mid = left + (right - left) / 2
        
        if (arr[mid] == target) {
            return mid
        } else if (arr[mid] < target) {
            left = mid + 1
        } else {
            right = mid - 1
        }
    }
    
    return -1
}
```

### 4. Use Appropriate Data Structures

```klang
// For frequent searches, consider preprocessing
fn preprocessForSearch(arr) {
    // Option 1: Sort the array
    let sorted = sort(arr)
    
    // Option 2: Build hash table for O(1) lookup
    let hashTable = buildHashTable(arr)
    
    // Option 3: Build BST for O(log n) operations
    let bst = buildBST(arr)
    
    return sorted  // or hashTable or bst
}
```

### 5. Document Preconditions

```klang
// Binary search REQUIRES sorted array
// @param arr: sorted array in ascending order
// @param target: element to search for
// @return: index of target or -1 if not found
fn binarySearch(arr, target) {
    // Implementation...
}
```

---

## Common Pitfalls

### 1. Off-by-One Errors

```klang
// ❌ Wrong: Infinite loop possible
while (left < right) {
    let mid = (left + right) / 2
    if (arr[mid] < target) {
        left = mid  // ← Bug! Should be mid + 1
    }
}

// ✅ Correct
while (left <= right) {
    let mid = left + (right - left) / 2
    if (arr[mid] < target) {
        left = mid + 1
    } else {
        right = mid - 1
    }
}
```

### 2. Wrong Loop Condition

```klang
// ❌ Wrong: Misses last element
while (left < right) {
    // ...
}

// ✅ Correct for finding exact match
while (left <= right) {
    // ...
}

// ✅ Correct for finding boundary
while (left < right) {
    // ... (different update logic)
}
```

### 3. Infinite Loops

```klang
// ❌ Wrong: Can create infinite loop
while (left < right) {
    let mid = (left + right) / 2
    if (condition) {
        left = mid  // ← Danger if mid == left
    }
}

// ✅ Correct: Always makes progress
while (left < right) {
    let mid = left + (right - left) / 2
    if (condition) {
        left = mid + 1  // Always advances
    } else {
        right = mid
    }
}
```

### 4. Forgetting Array is Unsorted

```klang
// ❌ Wrong: Binary search on unsorted array
let arr = [5, 2, 8, 1, 9]
let index = binarySearch(arr, 2)  // Incorrect result!

// ✅ Correct: Sort first or use linear search
let sortedArr = sort(arr)
let index = binarySearch(sortedArr, 2)

// OR
let index = linearSearch(arr, 2)
```

### 5. Not Handling Duplicates

```klang
// ❌ Wrong: Returns any occurrence
fn binarySearch(arr, target) {
    // Standard implementation
}

// ✅ Correct: Specify which occurrence
fn findFirstOccurrence(arr, target) {
    // Implementation that continues searching left
}
```

### 6. Integer Division Issues

```klang
// ❌ Wrong: Integer division may not find exact middle
let mid = (left + right) / 2  // In some languages

// ✅ Correct: Explicit integer division
let mid = left + (right - left) / 2
```

### 7. Boundary Conditions

```klang
// ❌ Wrong: Doesn't handle edge cases
fn searchRotated(arr, target) {
    // Assumes arr.length() > 0
    let left = 0
    let right = arr.length() - 1  // Crashes if empty!
}

// ✅ Correct: Handle edge cases
fn searchRotated(arr, target) {
    if (arr.length() == 0) {
        return -1
    }
    // Rest of implementation
}
```

### 8. Comparing Floating Point

```klang
// ❌ Wrong: Floating point comparison
if (arr[mid] == target) {  // May fail for floats
    return mid
}

// ✅ Correct: Use epsilon for floating point
let epsilon = 0.000001
if (abs(arr[mid] - target) < epsilon) {
    return mid
}
```

---

## Summary

### Key Points

1. **Linear Search**: O(n), works on any array, simple
2. **Binary Search**: O(log n), requires sorted array, most common
3. **Interpolation Search**: O(log log n) average, best for uniform data
4. **Jump Search**: O(√n), good for large sorted arrays
5. **Exponential Search**: O(log n), excellent for unbounded arrays
6. **Ternary Search**: O(log₃ n), useful for unimodal functions

### Decision Tree

```
Is array sorted?
├─ No → Linear Search
└─ Yes → What's the size?
    ├─ Small (< 100) → Linear or Binary Search
    └─ Large → What's the distribution?
        ├─ Uniform → Interpolation Search
        ├─ Unknown → Binary Search (safest)
        └─ Unbounded → Exponential Search
```

### Performance Summary

- **Best for sorted arrays**: Binary Search
- **Best for uniform distribution**: Interpolation Search
- **Best for unsorted**: Linear Search
- **Best for unbounded**: Exponential Search
- **Best for finding extrema**: Ternary Search

---

## Additional Resources

### Practice Problems
1. LeetCode Binary Search Problems
2. HackerRank Search Challenges
3. CodeForces Binary Search Contests

### Further Reading
- Introduction to Algorithms (CLRS)
- The Algorithm Design Manual (Skiena)
- Competitive Programmer's Handbook

### Related Topics
- Sorting Algorithms
- Divide and Conquer
- Tree Structures (BST, AVL)
- Hash Tables
- Graph Search (BFS, DFS)

---

**Next**: [Divide and Conquer Algorithms](03_divide_and_conquer.md)

**Previous**: [Sorting Algorithms](01_sorting.md)

---

*This documentation is part of the KLang DSA series. For more information, visit the [KLang Documentation](../../README.md).*
