# Data Structures & Algorithms (DSA) - Complete Guide

## Overview

This directory contains comprehensive documentation for Data Structures and Algorithms in KLang, covering basic to advanced concepts with theory, implementation, complexity analysis, and real-world examples.

## Documentation Structure

### Data Structures

#### Phase 1: Basic Data Structures
- **01_arrays.md** ✅ - Arrays, operations, algorithms
- **02_strings.md** ✅ - String manipulation, patterns
- **03_linked_lists.md** ⏳ - Singly, doubly, circular lists
- **04_stacks.md** ⏳ - LIFO, operations, applications
- **05_queues.md** ⏳ - FIFO, circular, priority queues

#### Phase 2: Intermediate Data Structures
- **06_hash_tables.md** ⏳ - Hash maps, sets, collision handling
- **07_trees.md** ⏳ - Binary trees, BST, traversals
- **08_heaps.md** ⏳ - Min/max heaps, heap operations
- **09_graphs.md** ⏳ - Representations, adjacency list/matrix

#### Phase 3: Advanced Data Structures
- **10_tries.md** ⏳ - Prefix trees, autocomplete
- **11_advanced_structures.md** ✅ - Segment trees, Fenwick trees, union-find, suffix arrays, bloom filters, skip lists

### Algorithms

#### Phase 1: Basic Algorithms
- **01_sorting.md** ✅ - Bubble, selection, insertion, merge, quick, heap, counting, radix sort
- **02_searching.md** ✅ - Linear, binary, interpolation, jump, exponential, ternary search, rotated arrays
- **03_recursion.md** ⏳ - Basics, backtracking, memoization

#### Phase 2: Intermediate Algorithms
- **04_dynamic_programming.md** ⏳ - Fibonacci, knapsack, LCS, LIS
- **05_greedy.md** ⏳ - Activity selection, fractional knapsack
- **06_graph_algorithms.md** ⏳ - BFS, DFS, Dijkstra, Bellman-Ford
- **07_string_algorithms.md** ⏳ - KMP, Rabin-Karp

#### Phase 3: Advanced Algorithms
- **08_advanced_algorithms.md** ⏳ - Floyd-Warshall, MST, geometry

## Quick Reference

### Array Operations

| Operation | Time | Space | Code |
|-----------|------|-------|------|
| Access | O(1) | O(1) | `arr[i]` |
| Search | O(n) | O(1) | Linear search |
| Binary Search | O(log n) | O(1) | On sorted array |
| Insert at end | O(1) | O(1) | `arr.push(val)` |
| Insert at start | O(n) | O(1) | `arr.unshift(val)` |
| Delete from end | O(1) | O(1) | `arr.pop()` |

### String Operations

| Operation | Time | Space | Code |
|-----------|------|-------|------|
| Access char | O(1) | O(1) | `str[i]` |
| Length | O(1) | O(1) | `len(str)` |
| Concatenation | O(n+m) | O(n+m) | `str1 + str2` |
| Substring | O(k) | O(k) | `str.slice(i, j)` |
| Search | O(n*m) | O(1) | `str.indexOf(sub)` |
| Split | O(n) | O(n) | `str.split(delim)` |
| Join | O(n) | O(n) | `arr.join(sep)` |

### Common Algorithms

**Sorting:**
```klang
# Bubble Sort - O(n²)
fn bubbleSort(arr) {
    let n = len(arr)
    let i = 0
    while (i < n - 1) {
        let j = 0
        while (j < n - i - 1) {
            if (arr[j] > arr[j + 1]) {
                let temp = arr[j]
                arr[j] = arr[j + 1]
                arr[j + 1] = temp
            }
            j = j + 1
        }
        i = i + 1
    }
}
```

**Binary Search:**
```klang
# Binary Search - O(log n)
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
    return -1
}
```

## Complexity Cheat Sheet

### Time Complexities (from fastest to slowest)

- **O(1)** - Constant: Array access, hash table lookup
- **O(log n)** - Logarithmic: Binary search, balanced tree operations
- **O(n)** - Linear: Array traversal, linear search
- **O(n log n)** - Linearithmic: Merge sort, quick sort, heap sort
- **O(n²)** - Quadratic: Bubble sort, selection sort, insertion sort
- **O(n³)** - Cubic: Matrix multiplication (naive)
- **O(2ⁿ)** - Exponential: Recursive fibonacci, subset generation
- **O(n!)** - Factorial: Permutation generation, TSP brute force

### Space Complexities

- **O(1)** - Constant: In-place algorithms
- **O(n)** - Linear: Extra array for merge sort
- **O(log n)** - Logarithmic: Recursive call stack for binary search
- **O(n²)** - Quadratic: 2D arrays, dynamic programming tables

## Learning Path

### For Beginners (Start Here)

1. **Arrays** - Master basic operations
   - Array creation, access, modification
   - Linear search, finding min/max
   - Basic array algorithms

2. **Strings** - Text manipulation
   - String operations
   - Pattern matching
   - String algorithms

3. **Sorting** - Understand comparison-based sorting
   - Bubble sort (simple)
   - Insertion sort (for small arrays)
   - Merge sort (divide and conquer)

4. **Searching** - Find elements efficiently
   - Linear search
   - Binary search

### For Intermediate Learners

1. **Linked Lists** - Dynamic data structures
2. **Stacks and Queues** - LIFO and FIFO
3. **Trees** - Hierarchical data
4. **Hash Tables** - Fast lookups
5. **Dynamic Programming** - Optimization problems
6. **Graph Algorithms** - Network problems

### For Advanced Learners

1. **Advanced Trees** - AVL, Red-Black, B-trees
2. **Advanced Graphs** - MST, Shortest paths
3. **Tries** - Prefix trees
4. **Segment Trees** - Range queries
5. **Computational Geometry**
6. **NP-Complete Problems**

## Common Interview Problems

### Arrays
- Two Sum
- Maximum Subarray (Kadane's)
- Merge Sorted Arrays
- Rotate Array
- Best Time to Buy and Sell Stock

### Strings
- Valid Palindrome
- Reverse String
- First Unique Character
- Longest Substring Without Repeating Characters
- Valid Anagram
- Group Anagrams

### Linked Lists
- Reverse Linked List
- Detect Cycle
- Merge Two Sorted Lists
- Remove Nth Node From End

### Trees
- Binary Tree Traversal (Inorder, Preorder, Postorder)
- Maximum Depth
- Validate BST
- Lowest Common Ancestor

### Dynamic Programming
- Fibonacci
- Climbing Stairs
- Coin Change
- Longest Common Subsequence
- 0/1 Knapsack

### Graphs
- Number of Islands
- Clone Graph
- Course Schedule
- Shortest Path
- Minimum Spanning Tree

## Problem-Solving Patterns

### 1. Two Pointers
Used for: Arrays, strings, linked lists
```klang
fn twoPointerExample(arr) {
    let left = 0
    let right = len(arr) - 1
    while (left < right) {
        # Process elements
        left = left + 1
        right = right - 1
    }
}
```

### 2. Sliding Window
Used for: Subarrays, substrings
```klang
fn slidingWindow(arr, k: int) {
    let sum = 0
    # Initial window
    let i = 0
    while (i < k) {
        sum = sum + arr[i]
        i = i + 1
    }
    
    # Slide window
    let maxSum = sum
    i = k
    while (i < len(arr)) {
        sum = sum + arr[i] - arr[i - k]
        if (sum > maxSum) {
            maxSum = sum
        }
        i = i + 1
    }
    return maxSum
}
```

### 3. Fast and Slow Pointers
Used for: Cycle detection, middle element
```klang
fn hasCycle(head) {
    let slow = head
    let fast = head
    
    while (fast != null && fast.next != null) {
        slow = slow.next
        fast = fast.next.next
        if (slow == fast) {
            return true
        }
    }
    return false
}
```

### 4. Divide and Conquer
Used for: Sorting, searching, tree problems
```klang
fn mergeSort(arr) {
    if (len(arr) <= 1) {
        return arr
    }
    
    let mid = len(arr) / 2
    let left = mergeSort(arr.slice(0, mid))
    let right = mergeSort(arr.slice(mid))
    
    return merge(left, right)
}
```

### 5. Dynamic Programming
Used for: Optimization problems
```klang
fn fibonacci(n: int) -> int {
    let dp = [0, 1]
    let i = 2
    while (i <= n) {
        dp.push(dp[i-1] + dp[i-2])
        i = i + 1
    }
    return dp[n]
}
```

## Test Files

All DSA concepts include comprehensive test files:

```bash
# Run array tests
./klang run tests/dsa/data-structures/test_arrays.kl

# Run sorting tests
./klang run tests/dsa/algorithms/test_sorting.kl

# Run graph tests
./klang run tests/dsa/data-structures/test_graphs.kl
```

## Examples

Practical examples for each concept:

```bash
# Array examples
./klang run examples/dsa/data-structures/example_arrays.kl

# Sorting examples
./klang run examples/dsa/algorithms/example_sorting.kl

# Graph examples
./klang run examples/dsa/data-structures/example_graphs.kl
```

## Best Practices

### 1. Analyze Complexity First
Always consider time and space complexity before implementing.

### 2. Choose Right Data Structure
- **Fast access**: Arrays, hash tables
- **Fast insertion/deletion**: Linked lists
- **Ordered data**: Trees, heaps
- **Relationships**: Graphs

### 3. Optimize Iteratively
1. Start with brute force (correctness)
2. Identify bottlenecks
3. Optimize using better algorithm/structure
4. Test thoroughly

### 4. Handle Edge Cases
- Empty inputs
- Single element
- All same elements
- Maximum/minimum values
- Negative numbers

### 5. Write Clean Code
```klang
# Good: Clear variable names
fn findMaximum(numbers) {
    let maximum = numbers[0]
    let i = 1
    while (i < len(numbers)) {
        if (numbers[i] > maximum) {
            maximum = numbers[i]
        }
        i = i + 1
    }
    return maximum
}

# Bad: Unclear names
fn f(a) {
    let m = a[0]
    let i = 1
    while (i < len(a)) {
        if (a[i] > m) {
            m = a[i]
        }
        i = i + 1
    }
    return m
}
```

## Progress Tracking

See `DSA_PROGRESS.md` for detailed progress on all DSA concepts.

## Resources

### Internal
- `/docs/dsa/data-structures/` - Data structure guides
- `/docs/dsa/algorithms/` - Algorithm guides
- `/tests/dsa/` - Comprehensive tests
- `/examples/dsa/` - Practical examples

### Practice
- Solve problems from each category
- Implement algorithms from scratch
- Analyze complexity of your solutions
- Compare with optimal solutions

## Contributing

When adding new DSA documentation:
1. Include theory and explanation
2. Provide complexity analysis
3. Add multiple examples
4. Create comprehensive tests
5. Include real-world applications
6. Document edge cases

---

**Happy Learning!** 🚀

*Master data structures and algorithms to become a better programmer!*

*Last Updated: 2026-04-09*
*Version: 1.1*
