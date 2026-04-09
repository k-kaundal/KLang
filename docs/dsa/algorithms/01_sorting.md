# Sorting Algorithms in KLang - Data Structures & Algorithms

## Table of Contents
1. [Introduction](#introduction)
2. [What is Sorting?](#what-is-sorting)
3. [Stability in Sorting](#stability-in-sorting)
4. [Bubble Sort](#bubble-sort)
5. [Selection Sort](#selection-sort)
6. [Insertion Sort](#insertion-sort)
7. [Merge Sort](#merge-sort)
8. [Quick Sort](#quick-sort)
9. [Heap Sort](#heap-sort)
10. [Counting Sort](#counting-sort)
11. [Radix Sort](#radix-sort)
12. [Comparison Table](#comparison-table)
13. [When to Use Which Sort](#when-to-use-which-sort)
14. [Interview Problems](#interview-problems)
15. [Best Practices](#best-practices)

---

## Introduction

Sorting is one of the most fundamental operations in computer science and forms the basis for many other algorithms. Understanding sorting algorithms is crucial for:

- **Efficient Data Processing**: Many algorithms require sorted data
- **Search Optimization**: Binary search requires sorted arrays
- **Database Operations**: Indexing and query optimization
- **Interview Preparation**: Common topic in technical interviews
- **Algorithm Design**: Understanding time-space trade-offs

This comprehensive guide covers all major sorting algorithms with complete implementations in KLang, complexity analysis, and practical applications.

---

## What is Sorting?

**Sorting** is the process of arranging elements in a specific order (ascending or descending) based on a comparison criterion.

### Why is Sorting Important?

1. **Faster Searching**: Binary search on sorted data is O(log n)
2. **Data Analysis**: Finding median, percentiles, outliers
3. **Algorithm Prerequisites**: Many algorithms need sorted input
4. **User Experience**: Displaying organized data
5. **Optimization**: Efficient duplicate detection, merging

### Key Concepts

- **Comparison-based Sorting**: Uses comparisons (>, <, ==) to order elements
- **Non-comparison Sorting**: Uses properties like digit values (Counting, Radix)
- **In-place Sorting**: Uses O(1) extra space
- **Stable Sorting**: Maintains relative order of equal elements
- **Adaptive Sorting**: Performs better on partially sorted data

---

## Stability in Sorting

**Stability** is a critical property of sorting algorithms that is often overlooked but can be crucial in many applications.

### What is Stability?

A sorting algorithm is **stable** if it preserves the relative order of elements with equal keys.

```
Before Sort: [(3, "A"), (1, "B"), (3, "C"), (2, "D")]
                        ↓
Stable Sort:   [(1, "B"), (2, "D"), (3, "A"), (3, "C")]
                                      ↑         ↑
                                  Order preserved!

Unstable Sort: [(1, "B"), (2, "D"), (3, "C"), (3, "A")]
                                      ↑         ↑
                                  Order changed!
```

### Why Does Stability Matter?

1. **Multi-level Sorting**: Sorting by secondary keys while preserving primary order
2. **Database Operations**: Maintaining insertion order for equal values
3. **UI/UX**: Preserving user's manual arrangements
4. **Merge Operations**: Combining sorted lists predictably

**Example Use Case**:
```
Students sorted by name: [Alice-85, Bob-90, Charlie-85]
Now sort by score while keeping name order for same scores:
Stable:   [Alice-85, Charlie-85, Bob-90]  ✓ (alphabetical preserved)
Unstable: [Charlie-85, Alice-85, Bob-90]  ✗ (alphabetical lost)
```

### Stable Algorithms
- Bubble Sort
- Insertion Sort
- Merge Sort
- Counting Sort
- Radix Sort

### Unstable Algorithms
- Selection Sort
- Quick Sort (standard implementation)
- Heap Sort

---

## Bubble Sort

### Overview

Bubble Sort is the simplest sorting algorithm that repeatedly steps through the list, compares adjacent elements, and swaps them if they're in the wrong order. The algorithm gets its name because smaller elements "bubble" to the top of the list.

### How It Works

1. Compare adjacent pairs of elements
2. Swap them if they're in wrong order
3. Repeat until no swaps are needed
4. After each pass, the largest element reaches its correct position

### Visual Representation

```
Pass 1: [5, 2, 8, 1, 9]
        [2, 5, 8, 1, 9]  (swap 5 and 2)
        [2, 5, 8, 1, 9]  (no swap)
        [2, 5, 1, 8, 9]  (swap 8 and 1)
        [2, 5, 1, 8, 9]  (no swap)
                    ↑ Largest in place

Pass 2: [2, 5, 1, 8, 9]
        [2, 5, 1, 8, 9]  (no swap)
        [2, 1, 5, 8, 9]  (swap 5 and 1)
        [2, 1, 5, 8, 9]  (no swap)
                 ↑ Second largest in place

Pass 3: [2, 1, 5, 8, 9]
        [1, 2, 5, 8, 9]  (swap 2 and 1)
        [1, 2, 5, 8, 9]  (no swap)
              ↑ Third largest in place

Sorted: [1, 2, 5, 8, 9]
```

### Complete Implementation

```klang
// Basic Bubble Sort
fn bubbleSort(arr) {
    let n = arr.length()
    
    for (let i = 0; i < n - 1; i = i + 1) {
        for (let j = 0; j < n - i - 1; j = j + 1) {
            if (arr[j] > arr[j + 1]) {
                // Swap elements
                let temp = arr[j]
                arr[j] = arr[j + 1]
                arr[j + 1] = temp
            }
        }
    }
    
    return arr
}

// Optimized Bubble Sort with early termination
fn bubbleSortOptimized(arr) {
    let n = arr.length()
    
    for (let i = 0; i < n - 1; i = i + 1) {
        let swapped = false
        
        for (let j = 0; j < n - i - 1; j = j + 1) {
            if (arr[j] > arr[j + 1]) {
                // Swap elements
                let temp = arr[j]
                arr[j] = arr[j + 1]
                arr[j + 1] = temp
                swapped = true
            }
        }
        
        // If no swaps occurred, array is sorted
        if (!swapped) {
            break
        }
    }
    
    return arr
}

// Example usage
let numbers = [64, 34, 25, 12, 22, 11, 90]
print("Original array: " + numbers)
bubbleSort(numbers)
print("Sorted array: " + numbers)
```

### Complexity Analysis

| Case | Time Complexity | Space Complexity |
|------|----------------|------------------|
| Best | O(n) | O(1) |
| Average | O(n²) | O(1) |
| Worst | O(n²) | O(1) |

- **Best Case**: Already sorted array (with optimization)
- **Worst Case**: Reverse sorted array
- **Space**: In-place algorithm, uses only O(1) extra space

### Characteristics

- **Stable**: ✓ Yes (equal elements maintain order)
- **In-place**: ✓ Yes
- **Adaptive**: ✓ Yes (with optimization flag)
- **Online**: ✗ No

### When to Use Bubble Sort

✅ **Use When**:
- Array is small (< 10 elements)
- Array is nearly sorted
- Simplicity is more important than efficiency
- Teaching sorting concepts
- Memory is extremely constrained

❌ **Avoid When**:
- Dealing with large datasets
- Performance is critical
- Better alternatives are available

---

## Selection Sort

### Overview

Selection Sort divides the array into sorted and unsorted regions. It repeatedly finds the minimum element from the unsorted region and moves it to the end of the sorted region.

### How It Works

1. Find the minimum element in unsorted region
2. Swap it with the first element of unsorted region
3. Move the boundary between sorted and unsorted
4. Repeat until entire array is sorted

### Visual Representation

```
Initial: [29, 10, 14, 37, 13]
          ↓ sorted | unsorted →

Step 1: Find min in [29, 10, 14, 37, 13] → 10
        [10, 29, 14, 37, 13]
         ↑ sorted | unsorted →

Step 2: Find min in [29, 14, 37, 13] → 13
        [10, 13, 14, 37, 29]
         ↑   ↑ sorted | unsorted →

Step 3: Find min in [14, 37, 29] → 14
        [10, 13, 14, 37, 29]
         ↑   ↑   ↑ sorted | unsorted →

Step 4: Find min in [37, 29] → 29
        [10, 13, 14, 29, 37]
         ↑   ↑   ↑   ↑ sorted | unsorted →

Step 5: Only one element left
        [10, 13, 14, 29, 37]
         ↑________________↑ All sorted
```

### Complete Implementation

```klang
// Selection Sort
fn selectionSort(arr) {
    let n = arr.length()
    
    for (let i = 0; i < n - 1; i = i + 1) {
        // Find minimum element in unsorted array
        let minIdx = i
        
        for (let j = i + 1; j < n; j = j + 1) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j
            }
        }
        
        // Swap minimum element with first element
        if (minIdx != i) {
            let temp = arr[i]
            arr[i] = arr[minIdx]
            arr[minIdx] = temp
        }
    }
    
    return arr
}

// Selection Sort with maximum element (descending order)
fn selectionSortDescending(arr) {
    let n = arr.length()
    
    for (let i = 0; i < n - 1; i = i + 1) {
        let maxIdx = i
        
        for (let j = i + 1; j < n; j = j + 1) {
            if (arr[j] > arr[maxIdx]) {
                maxIdx = j
            }
        }
        
        if (maxIdx != i) {
            let temp = arr[i]
            arr[i] = arr[maxIdx]
            arr[maxIdx] = temp
        }
    }
    
    return arr
}

// Example usage
let numbers = [64, 25, 12, 22, 11]
print("Original: " + numbers)
selectionSort(numbers)
print("Sorted: " + numbers)
```

### Complexity Analysis

| Case | Time Complexity | Space Complexity |
|------|----------------|------------------|
| Best | O(n²) | O(1) |
| Average | O(n²) | O(1) |
| Worst | O(n²) | O(1) |

- **All Cases**: Always performs n²/2 comparisons
- **Minimum Swaps**: At most n-1 swaps (better than bubble sort)
- **Space**: In-place algorithm, O(1) extra space

### Characteristics

- **Stable**: ✗ No (can swap non-adjacent equal elements)
- **In-place**: ✓ Yes
- **Adaptive**: ✗ No (always same number of comparisons)
- **Online**: ✗ No

### When to Use Selection Sort

✅ **Use When**:
- Minimizing number of swaps is important
- Memory writes are expensive (flash memory)
- Array is small
- Simplicity is valued

❌ **Avoid When**:
- Stability is required
- Large datasets
- Performance matters

---

## Insertion Sort

### Overview

Insertion Sort builds the final sorted array one element at a time. It's like sorting playing cards in your hands: you pick one card and insert it into its correct position among the already sorted cards.

### How It Works

1. Start with the second element (first is already "sorted")
2. Compare with elements in the sorted portion
3. Shift larger elements one position right
4. Insert the element in its correct position
5. Repeat for all elements

### Visual Representation

```
Initial: [12, 11, 13, 5, 6]
         [12] | 11, 13, 5, 6
         ↑ sorted | unsorted →

Step 1: Insert 11
        [11, 12] | 13, 5, 6
         ↑___↑ sorted

Step 2: Insert 13
        [11, 12, 13] | 5, 6
         ↑_______↑ sorted

Step 3: Insert 5
        [5, 11, 12, 13] | 6
        ↑___________↑ sorted

Step 4: Insert 6
        [5, 6, 11, 12, 13]
        ↑______________↑ All sorted
```

### Complete Implementation

```klang
// Insertion Sort
fn insertionSort(arr) {
    let n = arr.length()
    
    for (let i = 1; i < n; i = i + 1) {
        let key = arr[i]
        let j = i - 1
        
        // Move elements greater than key one position ahead
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j]
            j = j - 1
        }
        
        // Insert key at correct position
        arr[j + 1] = key
    }
    
    return arr
}

// Optimized with binary search for position finding
fn insertionSortBinarySearch(arr) {
    let n = arr.length()
    
    for (let i = 1; i < n; i = i + 1) {
        let key = arr[i]
        let pos = binarySearchPosition(arr, key, 0, i - 1)
        
        // Shift elements
        for (let j = i - 1; j >= pos; j = j - 1) {
            arr[j + 1] = arr[j]
        }
        
        arr[pos] = key
    }
    
    return arr
}

// Helper: Find insertion position using binary search
fn binarySearchPosition(arr, key, left, right) {
    while (left <= right) {
        let mid = left + (right - left) / 2
        
        if (arr[mid] > key) {
            right = mid - 1
        } else {
            left = mid + 1
        }
    }
    
    return left
}

// Recursive Insertion Sort
fn insertionSortRecursive(arr, n) {
    // Base case
    if (n <= 1) {
        return arr
    }
    
    // Sort first n-1 elements
    insertionSortRecursive(arr, n - 1)
    
    // Insert nth element at correct position
    let key = arr[n - 1]
    let j = n - 2
    
    while (j >= 0 && arr[j] > key) {
        arr[j + 1] = arr[j]
        j = j - 1
    }
    
    arr[j + 1] = key
    return arr
}

// Example usage
let numbers = [12, 11, 13, 5, 6]
print("Original: " + numbers)
insertionSort(numbers)
print("Sorted: " + numbers)
```

### Complexity Analysis

| Case | Time Complexity | Space Complexity |
|------|----------------|------------------|
| Best | O(n) | O(1) |
| Average | O(n²) | O(1) |
| Worst | O(n²) | O(1) |

- **Best Case**: Already sorted array - O(n)
- **Worst Case**: Reverse sorted array - O(n²)
- **Average**: About n²/4 comparisons and shifts
- **Space**: In-place, O(1) iterative, O(n) recursive stack

### Characteristics

- **Stable**: ✓ Yes
- **In-place**: ✓ Yes
- **Adaptive**: ✓ Yes (excellent for nearly sorted data)
- **Online**: ✓ Yes (can sort as data arrives)

### When to Use Insertion Sort

✅ **Use When**:
- Array is small (< 10-20 elements)
- Array is nearly sorted
- Sorting data as it arrives (online)
- Implementing hybrid sorting algorithms
- Stability is required

❌ **Avoid When**:
- Large datasets
- Data is random or reverse sorted

**Note**: Many optimized algorithms (like TimSort) use insertion sort for small subarrays!

---

## Merge Sort

### Overview

Merge Sort is a **divide-and-conquer** algorithm that divides the array into two halves, recursively sorts them, and then merges the sorted halves. It's one of the most efficient sorting algorithms with guaranteed O(n log n) performance.

### How It Works

1. **Divide**: Split array into two halves
2. **Conquer**: Recursively sort both halves
3. **Combine**: Merge the two sorted halves

### Visual Representation

```
                    [38, 27, 43, 3, 9, 82, 10]
                              |
                     -------- DIVIDE --------
                    /                        \
           [38, 27, 43, 3]              [9, 82, 10]
                |                            |
              DIVIDE                       DIVIDE
             /      \                      /     \
      [38, 27]      [43, 3]          [9, 82]    [10]
        |              |                |
      DIVIDE         DIVIDE           DIVIDE
      /    \         /    \           /    \
    [38]  [27]     [43]  [3]        [9]   [82]
      \    /         \    /           \    /
       MERGE          MERGE            MERGE
        |              |                |
    [27, 38]        [3, 43]          [9, 82]
        \              /                |
         ---- MERGE ---                 |
              |                         |
        [3, 27, 38, 43]            [9, 82, 10]
              \                         /
               -------- MERGE ---------
                        |
              [3, 9, 10, 27, 38, 43, 82]
```

### Recursion Tree

```
Level 0:                [8 elements]              - 1 merge of 8 elements
                       /            \
Level 1:        [4 elements]    [4 elements]      - 2 merges of 4 elements each
                /      \        /       \
Level 2:      [2]      [2]    [2]      [2]        - 4 merges of 2 elements each
             / \      / \     / \      / \
Level 3:   [1] [1]  [1] [1] [1] [1]  [1] [1]      - 8 merges of 1 element each

Height = log₂(n) = log₂(8) = 3 levels
Work per level = O(n)
Total work = O(n log n)
```

### Complete Implementation

```klang
// Merge Sort - Main function
fn mergeSort(arr, left, right) {
    if (left < right) {
        // Find middle point
        let mid = left + (right - left) / 2
        
        // Sort first and second halves
        mergeSort(arr, left, mid)
        mergeSort(arr, mid + 1, right)
        
        // Merge the sorted halves
        merge(arr, left, mid, right)
    }
    
    return arr
}

// Merge two sorted subarrays
fn merge(arr, left, mid, right) {
    // Sizes of two subarrays
    let n1 = mid - left + 1
    let n2 = right - mid
    
    // Create temporary arrays
    let leftArr = []
    let rightArr = []
    
    // Copy data to temporary arrays
    for (let i = 0; i < n1; i = i + 1) {
        leftArr.push(arr[left + i])
    }
    for (let j = 0; j < n2; j = j + 1) {
        rightArr.push(arr[mid + 1 + j])
    }
    
    // Merge temporary arrays back
    let i = 0      // Initial index of left subarray
    let j = 0      // Initial index of right subarray
    let k = left   // Initial index of merged array
    
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i]
            i = i + 1
        } else {
            arr[k] = rightArr[j]
            j = j + 1
        }
        k = k + 1
    }
    
    // Copy remaining elements of leftArr[]
    while (i < n1) {
        arr[k] = leftArr[i]
        i = i + 1
        k = k + 1
    }
    
    // Copy remaining elements of rightArr[]
    while (j < n2) {
        arr[k] = rightArr[j]
        j = j + 1
        k = k + 1
    }
}

// Helper function for easy use
fn mergeSortArray(arr) {
    if (arr.length() <= 1) {
        return arr
    }
    
    mergeSort(arr, 0, arr.length() - 1)
    return arr
}

// Example usage
let numbers = [38, 27, 43, 3, 9, 82, 10]
print("Original: " + numbers)
mergeSortArray(numbers)
print("Sorted: " + numbers)
```

### Complexity Analysis

| Case | Time Complexity | Space Complexity |
|------|----------------|------------------|
| Best | O(n log n) | O(n) |
| Average | O(n log n) | O(n) |
| Worst | O(n log n) | O(n) |

- **All Cases**: Consistently O(n log n)
- **Recurrence**: T(n) = 2T(n/2) + O(n)
- **Space**: O(n) for temporary arrays, O(log n) for recursion stack

### Characteristics

- **Stable**: ✓ Yes
- **In-place**: ✗ No (requires O(n) extra space)
- **Adaptive**: ✗ No
- **Online**: ✗ No
- **Parallel**: ✓ Yes (can parallelize recursive calls)

### When to Use Merge Sort

✅ **Use When**:
- Guaranteed O(n log n) performance needed
- Stability is required
- Sorting linked lists (O(1) space possible)
- External sorting (large files)
- Parallel processing available

❌ **Avoid When**:
- Memory is very limited
- In-place sorting is required
- Array is small

---

## Quick Sort

### Overview

Quick Sort is a highly efficient **divide-and-conquer** algorithm that picks a pivot element and partitions the array around it, placing smaller elements before it and larger elements after it.

### How It Works

1. **Choose Pivot**: Select an element as pivot
2. **Partition**: Rearrange array so elements < pivot are on left, > pivot on right
3. **Recursively Sort**: Apply same process to left and right subarrays

### Visual Representation

```
Initial: [10, 7, 8, 9, 1, 5]  (pivot = 5)
          
Partition:
[1, 5, 8, 9, 10, 7]
    ↑
  pivot at correct position
  
Left: [1] | Pivot: [5] | Right: [8, 9, 10, 7]
  ✓ sorted            (pivot = 7)

Partition Right:
[1] | [5] | [7, 9, 10, 8]
              ↑
         pivot at position
         
[1] | [5] | Left: [] | Pivot: [7] | Right: [9, 10, 8]
                       ✓                   (pivot = 8)

Continue...

Final: [1, 5, 7, 8, 9, 10]
```

### Partition Process

```
Array: [10, 80, 30, 90, 40, 50, 70]  Pivot = 70

i                                     j
↓                                     ↓
[10, 80, 30, 90, 40, 50, 70]
 ↓                           ↓
10 < 70: increment i         swap with 70

    i                             j
    ↓                             ↓
[10, 80, 30, 90, 40, 50, 70]
     ↓                       ↓
80 > 70: keep i              move j

    i                   j
    ↓                   ↓
[10, 80, 30, 90, 40, 50, 70]
     ↓               ↓
swap 80 and 50

    i                   j
    ↓                   ↓
[10, 50, 30, 90, 40, 80, 70]

Continue process...

Result: [10, 50, 30, 40, 70, 80, 90]
                      ↑
                  pivot position
```

### Complete Implementation

```klang
// Quick Sort - Main function
fn quickSort(arr, low, high) {
    if (low < high) {
        // Partition index
        let pi = partition(arr, low, high)
        
        // Recursively sort elements before and after partition
        quickSort(arr, low, pi - 1)
        quickSort(arr, pi + 1, high)
    }
    
    return arr
}

// Partition function (Lomuto partition scheme)
fn partition(arr, low, high) {
    // Choose rightmost element as pivot
    let pivot = arr[high]
    let i = low - 1  // Index of smaller element
    
    for (let j = low; j < high; j = j + 1) {
        // If current element is smaller than pivot
        if (arr[j] < pivot) {
            i = i + 1
            
            // Swap arr[i] and arr[j]
            let temp = arr[i]
            arr[i] = arr[j]
            arr[j] = temp
        }
    }
    
    // Swap arr[i+1] and arr[high] (pivot)
    let temp = arr[i + 1]
    arr[i + 1] = arr[high]
    arr[high] = temp
    
    return i + 1
}

// Hoare Partition Scheme (alternative)
fn partitionHoare(arr, low, high) {
    let pivot = arr[low]
    let i = low - 1
    let j = high + 1
    
    while (true) {
        // Find leftmost element >= pivot
        i = i + 1
        while (arr[i] < pivot) {
            i = i + 1
        }
        
        // Find rightmost element <= pivot
        j = j - 1
        while (arr[j] > pivot) {
            j = j - 1
        }
        
        // If pointers crossed, return partition point
        if (i >= j) {
            return j
        }
        
        // Swap arr[i] and arr[j]
        let temp = arr[i]
        arr[i] = arr[j]
        arr[j] = temp
    }
}

// Quick Sort with random pivot
fn quickSortRandomPivot(arr, low, high) {
    if (low < high) {
        // Random pivot
        let randomIdx = low + Math.floor(Math.random() * (high - low + 1))
        
        // Swap with last element
        let temp = arr[randomIdx]
        arr[randomIdx] = arr[high]
        arr[high] = temp
        
        let pi = partition(arr, low, high)
        quickSortRandomPivot(arr, low, pi - 1)
        quickSortRandomPivot(arr, pi + 1, high)
    }
    
    return arr
}

// Quick Sort with median-of-three pivot
fn medianOfThree(arr, low, high) {
    let mid = low + (high - low) / 2
    
    // Sort low, mid, high
    if (arr[low] > arr[mid]) {
        let temp = arr[low]
        arr[low] = arr[mid]
        arr[mid] = temp
    }
    
    if (arr[mid] > arr[high]) {
        let temp = arr[mid]
        arr[mid] = arr[high]
        arr[high] = temp
    }
    
    if (arr[low] > arr[mid]) {
        let temp = arr[low]
        arr[low] = arr[mid]
        arr[mid] = temp
    }
    
    // Place median at high position
    let temp = arr[mid]
    arr[mid] = arr[high]
    arr[high] = temp
    
    return arr[high]
}

// Helper function
fn quickSortArray(arr) {
    if (arr.length() <= 1) {
        return arr
    }
    
    quickSort(arr, 0, arr.length() - 1)
    return arr
}

// Example usage
let numbers = [10, 7, 8, 9, 1, 5]
print("Original: " + numbers)
quickSortArray(numbers)
print("Sorted: " + numbers)
```

### Pivot Selection Strategies

1. **First Element**: Simple but bad for sorted arrays
2. **Last Element**: Simple but bad for sorted arrays
3. **Random Element**: Good average performance
4. **Median-of-Three**: Choose median of first, middle, last
5. **Median-of-Medians**: Guarantees O(n log n) but slower in practice

### Complexity Analysis

| Case | Time Complexity | Space Complexity |
|------|----------------|------------------|
| Best | O(n log n) | O(log n) |
| Average | O(n log n) | O(log n) |
| Worst | O(n²) | O(n) |

- **Best Case**: Balanced partitions - O(n log n)
- **Worst Case**: Unbalanced partitions (sorted array with bad pivot) - O(n²)
- **Average**: O(n log n) with good pivot selection
- **Space**: O(log n) recursion stack in best case, O(n) in worst

### Characteristics

- **Stable**: ✗ No (standard implementation)
- **In-place**: ✓ Yes (excluding recursion stack)
- **Adaptive**: Partial (depends on pivot selection)
- **Cache-friendly**: ✓ Yes (good locality of reference)

### When to Use Quick Sort

✅ **Use When**:
- Average O(n log n) performance is acceptable
- In-place sorting is important
- Cache performance matters
- General-purpose sorting needed

❌ **Avoid When**:
- Guaranteed O(n log n) is required
- Stability is needed
- Stack space is limited

**Note**: Quick Sort is often the default choice for general-purpose sorting in many libraries!

---

## Heap Sort

### Overview

Heap Sort uses a binary heap data structure to sort elements. It first builds a max heap from the array, then repeatedly extracts the maximum element and rebuilds the heap.

### How It Works

1. **Build Max Heap**: Convert array into max heap
2. **Extract Max**: Swap root (max) with last element
3. **Heapify**: Restore heap property
4. **Repeat**: Until all elements are sorted

### Visual Representation

```
Array: [4, 10, 3, 5, 1]

Step 1: Build Max Heap
         10
        /  \
       5    3
      / \
     4   1
Heap: [10, 5, 3, 4, 1]

Step 2: Swap 10 with last element (1)
         1
        / \
       5   3
      /
     4
     
Heapify:  5
         / \
        4   3
       /
      1
Sorted: [10]

Step 3: Swap 5 with last (1)
         1
        / \
       4   3

Heapify:  4
         / \
        1   3
Sorted: [5, 10]

Continue until all sorted: [1, 3, 4, 5, 10]
```

### Complete Implementation

```klang
// Heap Sort - Main function
fn heapSort(arr) {
    let n = arr.length()
    
    // Build max heap
    for (let i = n / 2 - 1; i >= 0; i = i - 1) {
        heapify(arr, n, i)
    }
    
    // Extract elements from heap one by one
    for (let i = n - 1; i > 0; i = i - 1) {
        // Move current root to end
        let temp = arr[0]
        arr[0] = arr[i]
        arr[i] = temp
        
        // Heapify the reduced heap
        heapify(arr, i, 0)
    }
    
    return arr
}

// Heapify a subtree rooted at index i
fn heapify(arr, n, i) {
    let largest = i          // Initialize largest as root
    let left = 2 * i + 1     // Left child
    let right = 2 * i + 2    // Right child
    
    // If left child is larger than root
    if (left < n && arr[left] > arr[largest]) {
        largest = left
    }
    
    // If right child is larger than largest so far
    if (right < n && arr[right] > arr[largest]) {
        largest = right
    }
    
    // If largest is not root
    if (largest != i) {
        let swap = arr[i]
        arr[i] = arr[largest]
        arr[largest] = swap
        
        // Recursively heapify the affected subtree
        heapify(arr, n, largest)
    }
}

// Iterative heapify (to avoid recursion)
fn heapifyIterative(arr, n, i) {
    while (true) {
        let largest = i
        let left = 2 * i + 1
        let right = 2 * i + 2
        
        if (left < n && arr[left] > arr[largest]) {
            largest = left
        }
        
        if (right < n && arr[right] > arr[largest]) {
            largest = right
        }
        
        if (largest == i) {
            break
        }
        
        let temp = arr[i]
        arr[i] = arr[largest]
        arr[largest] = temp
        
        i = largest
    }
}

// Min Heap Sort (for descending order)
fn heapSortDescending(arr) {
    let n = arr.length()
    
    // Build min heap
    for (let i = n / 2 - 1; i >= 0; i = i - 1) {
        minHeapify(arr, n, i)
    }
    
    for (let i = n - 1; i > 0; i = i - 1) {
        let temp = arr[0]
        arr[0] = arr[i]
        arr[i] = temp
        
        minHeapify(arr, i, 0)
    }
    
    return arr
}

fn minHeapify(arr, n, i) {
    let smallest = i
    let left = 2 * i + 1
    let right = 2 * i + 2
    
    if (left < n && arr[left] < arr[smallest]) {
        smallest = left
    }
    
    if (right < n && arr[right] < arr[smallest]) {
        smallest = right
    }
    
    if (smallest != i) {
        let temp = arr[i]
        arr[i] = arr[smallest]
        arr[smallest] = temp
        
        minHeapify(arr, n, smallest)
    }
}

// Example usage
let numbers = [12, 11, 13, 5, 6, 7]
print("Original: " + numbers)
heapSort(numbers)
print("Sorted: " + numbers)
```

### Complexity Analysis

| Case | Time Complexity | Space Complexity |
|------|----------------|------------------|
| Best | O(n log n) | O(1) |
| Average | O(n log n) | O(1) |
| Worst | O(n log n) | O(1) |

- **Build Heap**: O(n)
- **Heapify**: O(log n)
- **Total**: O(n) + n × O(log n) = O(n log n)
- **Space**: O(1) iterative, O(log n) recursive stack

### Characteristics

- **Stable**: ✗ No
- **In-place**: ✓ Yes
- **Adaptive**: ✗ No
- **Cache-friendly**: ✗ No (poor locality)

### When to Use Heap Sort

✅ **Use When**:
- Guaranteed O(n log n) with O(1) space needed
- In-place sorting required
- Worst-case performance matters

❌ **Avoid When**:
- Stability is required
- Cache performance is critical
- Better alternatives available

---

## Counting Sort

### Overview

Counting Sort is a **non-comparison** based sorting algorithm that counts the occurrences of each distinct element and uses arithmetic to determine positions. It's extremely efficient when the range of input is small.

### How It Works

1. **Count**: Count frequency of each element
2. **Cumulative**: Convert counts to cumulative counts
3. **Place**: Place elements at correct positions
4. **Result**: Build sorted output array

### Visual Representation

```
Input: [1, 4, 1, 2, 7, 5, 2]  Range: 1 to 7

Step 1: Count frequencies
Index:  0  1  2  3  4  5  6  7
Count: [0, 2, 2, 0, 1, 1, 0, 1]
        ↑  ↑  ↑     ↑  ↑     ↑
        1  1  2     4  5     7
       appears appears...

Step 2: Cumulative count (positions)
Index:  0  1  2  3  4  5  6  7
Count: [0, 2, 4, 4, 5, 6, 6, 7]
           ↑  ↑     ↑  ↑     ↑
          1s 2s    4  5     7
         end end  ends ends ends
         at at    at  at    at
         2  4     5   6     7

Step 3: Place elements in output
Output: [1, 1, 2, 2, 4, 5, 7]
```

### Complete Implementation

```klang
// Counting Sort
fn countingSort(arr) {
    let n = arr.length()
    
    if (n == 0) {
        return arr
    }
    
    // Find range
    let max = arr[0]
    let min = arr[0]
    
    for (let i = 1; i < n; i = i + 1) {
        if (arr[i] > max) {
            max = arr[i]
        }
        if (arr[i] < min) {
            min = arr[i]
        }
    }
    
    let range = max - min + 1
    
    // Create count array
    let count = []
    for (let i = 0; i < range; i = i + 1) {
        count.push(0)
    }
    
    // Store count of each element
    for (let i = 0; i < n; i = i + 1) {
        count[arr[i] - min] = count[arr[i] - min] + 1
    }
    
    // Change count[i] to actual position
    for (let i = 1; i < range; i = i + 1) {
        count[i] = count[i] + count[i - 1]
    }
    
    // Build output array
    let output = []
    for (let i = 0; i < n; i = i + 1) {
        output.push(0)
    }
    
    // Place elements in sorted order (traverse backwards for stability)
    for (let i = n - 1; i >= 0; i = i - 1) {
        let idx = arr[i] - min
        output[count[idx] - 1] = arr[i]
        count[idx] = count[idx] - 1
    }
    
    // Copy output to original array
    for (let i = 0; i < n; i = i + 1) {
        arr[i] = output[i]
    }
    
    return arr
}

// Simple Counting Sort (for small positive integers)
fn countingSortSimple(arr, maxVal) {
    let count = []
    
    // Initialize count array
    for (let i = 0; i <= maxVal; i = i + 1) {
        count.push(0)
    }
    
    // Count occurrences
    for (let i = 0; i < arr.length(); i = i + 1) {
        count[arr[i]] = count[arr[i]] + 1
    }
    
    // Rebuild array
    let idx = 0
    for (let i = 0; i <= maxVal; i = i + 1) {
        while (count[i] > 0) {
            arr[idx] = i
            idx = idx + 1
            count[i] = count[i] - 1
        }
    }
    
    return arr
}

// Example usage
let numbers = [4, 2, 2, 8, 3, 3, 1]
print("Original: " + numbers)
countingSort(numbers)
print("Sorted: " + numbers)
```

### Complexity Analysis

| Case | Time Complexity | Space Complexity |
|------|----------------|------------------|
| Best | O(n + k) | O(k) |
| Average | O(n + k) | O(k) |
| Worst | O(n + k) | O(k) |

- **n**: Number of elements
- **k**: Range of input (max - min + 1)
- **Time**: O(n) for counting + O(k) for cumulative + O(n) for placing
- **Space**: O(k) for count array, O(n) for output array

### Characteristics

- **Stable**: ✓ Yes (with proper implementation)
- **In-place**: ✗ No
- **Adaptive**: ✗ No
- **Non-comparison**: ✓ Yes

### When to Use Counting Sort

✅ **Use When**:
- Range (k) is small compared to n
- Elements are integers or can be mapped to integers
- Stability is important
- Linear time is needed

❌ **Avoid When**:
- Range is very large (k >> n)
- Elements are floating-point
- Memory is limited

**Example**: Sorting ages (0-150), grades (0-100), small bounded integers

---

## Radix Sort

### Overview

Radix Sort is a **non-comparison** sorting algorithm that sorts integers by processing individual digits. It sorts numbers digit by digit starting from the least significant digit (LSD) or most significant digit (MSD).

### How It Works

1. **Process Each Digit**: Starting from least significant
2. **Use Stable Sort**: Sort by current digit (often Counting Sort)
3. **Repeat**: For all digits
4. **Result**: Fully sorted array

### Visual Representation

```
Input: [170, 45, 75, 90, 802, 24, 2, 66]

Pass 1: Sort by ones place (LSD)
170, 90, 802, 2, 24, 45, 75, 66
 ↑   ↑    ↑  ↑   ↑   ↑   ↑   ↑
 0   0    2  2   4   5   5   6

Pass 2: Sort by tens place
802, 2, 24, 45, 66, 170, 75, 90
  ↑  ↑   ↑   ↑   ↑    ↑   ↑   ↑
  0  0   2   4   6    7   7   9

Pass 3: Sort by hundreds place
2, 24, 45, 66, 75, 90, 170, 802
↑   ↑   ↑   ↑   ↑   ↑    ↑    ↑
0   0   0   0   0   0    1    8

Final: [2, 24, 45, 66, 75, 90, 170, 802]
```

### Complete Implementation

```klang
// Radix Sort (LSD - Least Significant Digit)
fn radixSort(arr) {
    let n = arr.length()
    
    if (n == 0) {
        return arr
    }
    
    // Find maximum number to know number of digits
    let max = arr[0]
    for (let i = 1; i < n; i = i + 1) {
        if (arr[i] > max) {
            max = arr[i]
        }
    }
    
    // Do counting sort for every digit
    // exp is 10^i where i is current digit number
    let exp = 1
    while (max / exp > 0) {
        countingSortByDigit(arr, exp)
        exp = exp * 10
    }
    
    return arr
}

// Counting sort based on digit at exp position
fn countingSortByDigit(arr, exp) {
    let n = arr.length()
    let output = []
    let count = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  // 0-9 digits
    
    // Initialize output
    for (let i = 0; i < n; i = i + 1) {
        output.push(0)
    }
    
    // Store count of occurrences
    for (let i = 0; i < n; i = i + 1) {
        let digit = (arr[i] / exp) % 10
        count[digit] = count[digit] + 1
    }
    
    // Change count[i] to actual position
    for (let i = 1; i < 10; i = i + 1) {
        count[i] = count[i] + count[i - 1]
    }
    
    // Build output array (backwards for stability)
    for (let i = n - 1; i >= 0; i = i - 1) {
        let digit = (arr[i] / exp) % 10
        output[count[digit] - 1] = arr[i]
        count[digit] = count[digit] - 1
    }
    
    // Copy output to arr
    for (let i = 0; i < n; i = i + 1) {
        arr[i] = output[i]
    }
}

// Radix Sort for strings (MSD - Most Significant Digit)
fn radixSortStrings(arr) {
    let n = arr.length()
    
    if (n == 0) {
        return arr
    }
    
    // Find maximum length
    let maxLen = 0
    for (let i = 0; i < n; i = i + 1) {
        if (arr[i].length() > maxLen) {
            maxLen = arr[i].length()
        }
    }
    
    // Pad strings to same length
    for (let i = 0; i < n; i = i + 1) {
        while (arr[i].length() < maxLen) {
            arr[i] = arr[i] + " "
        }
    }
    
    // Sort by each character position
    for (let pos = maxLen - 1; pos >= 0; pos = pos - 1) {
        countingSortByChar(arr, pos)
    }
    
    // Remove padding
    for (let i = 0; i < n; i = i + 1) {
        arr[i] = arr[i].trim()
    }
    
    return arr
}

fn countingSortByChar(arr, pos) {
    let n = arr.length()
    let output = []
    let count = []
    
    // Initialize arrays
    for (let i = 0; i < n; i = i + 1) {
        output.push("")
    }
    for (let i = 0; i < 256; i = i + 1) {  // ASCII range
        count.push(0)
    }
    
    // Count occurrences
    for (let i = 0; i < n; i = i + 1) {
        let charCode = arr[i].charCodeAt(pos)
        count[charCode] = count[charCode] + 1
    }
    
    // Cumulative count
    for (let i = 1; i < 256; i = i + 1) {
        count[i] = count[i] + count[i - 1]
    }
    
    // Build output
    for (let i = n - 1; i >= 0; i = i - 1) {
        let charCode = arr[i].charCodeAt(pos)
        output[count[charCode] - 1] = arr[i]
        count[charCode] = count[charCode] - 1
    }
    
    // Copy back
    for (let i = 0; i < n; i = i + 1) {
        arr[i] = output[i]
    }
}

// Example usage
let numbers = [170, 45, 75, 90, 802, 24, 2, 66]
print("Original: " + numbers)
radixSort(numbers)
print("Sorted: " + numbers)
```

### Complexity Analysis

| Case | Time Complexity | Space Complexity |
|------|----------------|------------------|
| Best | O(d × (n + k)) | O(n + k) |
| Average | O(d × (n + k)) | O(n + k) |
| Worst | O(d × (n + k)) | O(n + k) |

- **d**: Number of digits in maximum number
- **n**: Number of elements
- **k**: Range of digit values (10 for decimal)
- **Time**: d passes of counting sort
- **Space**: O(n + k) for counting sort

### Characteristics

- **Stable**: ✓ Yes
- **In-place**: ✗ No
- **Adaptive**: ✗ No
- **Non-comparison**: ✓ Yes

### When to Use Radix Sort

✅ **Use When**:
- Sorting integers with limited digits
- Sorting strings of similar length
- Linear time is needed
- Stability matters

❌ **Avoid When**:
- Numbers have many digits (d is large)
- Memory is very limited
- Floating-point numbers
- Variable-length data

**Use Cases**: Sorting phone numbers, zip codes, dates, fixed-length codes

---

## Comparison Table

### Performance Comparison

| Algorithm | Best | Average | Worst | Space | Stable | In-Place |
|-----------|------|---------|-------|-------|--------|----------|
| **Bubble Sort** | O(n) | O(n²) | O(n²) | O(1) | ✓ | ✓ |
| **Selection Sort** | O(n²) | O(n²) | O(n²) | O(1) | ✗ | ✓ |
| **Insertion Sort** | O(n) | O(n²) | O(n²) | O(1) | ✓ | ✓ |
| **Merge Sort** | O(n log n) | O(n log n) | O(n log n) | O(n) | ✓ | ✗ |
| **Quick Sort** | O(n log n) | O(n log n) | O(n²) | O(log n) | ✗ | ✓ |
| **Heap Sort** | O(n log n) | O(n log n) | O(n log n) | O(1) | ✗ | ✓ |
| **Counting Sort** | O(n + k) | O(n + k) | O(n + k) | O(k) | ✓ | ✗ |
| **Radix Sort** | O(d(n + k)) | O(d(n + k)) | O(d(n + k)) | O(n + k) | ✓ | ✗ |

### Properties Comparison

| Feature | Comparison-Based | Non-Comparison |
|---------|-----------------|----------------|
| **Lower Bound** | Ω(n log n) | Can be O(n) |
| **Examples** | Merge, Quick, Heap | Counting, Radix |
| **Works on** | Any comparable | Integers, specific types |
| **Flexibility** | High | Limited |

---

## When to Use Which Sort

### Decision Guide

```
┌─────────────────────────────────────────┐
│         Start: Need to sort?            │
└──────────────┬──────────────────────────┘
               │
               ▼
         ┌───────────┐
         │ Array     │
         │ size?     │
         └─────┬─────┘
         ______│______
        /             \
    Small            Large
    (< 50)           (> 50)
      │                │
      ▼                ▼
┌─────────────┐  ┌─────────────┐
│ Insertion   │  │ Need        │
│ Sort        │  │ guaranteed  │
│             │  │ O(n log n)? │
└─────────────┘  └──────┬──────┘
                   _____│_____
                  /           \
                Yes           No
                 │            │
                 ▼            ▼
           ┌──────────┐  ┌──────────┐
           │ Stability│  │ In-place │
           │ needed?  │  │ critical?│
           └────┬─────┘  └────┬─────┘
            ____│____     ____│____
           /         \   /         \
         Yes        No  Yes        No
          │          │   │          │
          ▼          ▼   ▼          ▼
    ┌─────────┐  ┌────┐ ┌────┐ ┌─────────┐
    │ Merge   │  │Heap│ │Quick│ │ Quick   │
    │ Sort    │  │Sort│ │Sort │ │ or Merge│
    └─────────┘  └────┘ └────┘ └─────────┘
```

### Specific Scenarios

#### 1. Small Arrays (n < 10-50)
**Use**: Insertion Sort
- Simple implementation
- Low overhead
- Good cache performance

#### 2. Nearly Sorted Data
**Use**: Insertion Sort or Bubble Sort (optimized)
- O(n) performance when mostly sorted
- Adaptive behavior

#### 3. Large Arrays, Guaranteed Performance
**Use**: Merge Sort or Heap Sort
- Guaranteed O(n log n)
- Heap Sort if space is limited

#### 4. Large Arrays, Average Case Performance
**Use**: Quick Sort
- Fast in practice
- Good cache locality
- Most widely used

#### 5. Stability Required
**Use**: Merge Sort or Insertion Sort (small arrays)
- Preserves relative order
- Essential for multi-key sorting

#### 6. Limited Memory
**Use**: Heap Sort or Quick Sort
- In-place algorithms
- O(1) or O(log n) extra space

#### 7. Integer Keys with Small Range
**Use**: Counting Sort
- O(n + k) time
- Extremely fast for bounded integers

#### 8. Multi-digit Integers or Fixed-length Strings
**Use**: Radix Sort
- O(d × (n + k)) time
- Linear for small d

#### 9. Linked Lists
**Use**: Merge Sort
- No random access needed
- Can be done in O(1) space for linked lists

#### 10. External Sorting (Large Files)
**Use**: External Merge Sort
- Minimal disk I/O
- Works with limited RAM

---

## Interview Problems

### Problem 1: Sort Colors (Dutch National Flag)

**Problem**: Sort an array with only 0s, 1s, and 2s in-place in one pass.

```klang
// LeetCode 75: Sort Colors
// Time: O(n), Space: O(1)
fn sortColors(nums) {
    let low = 0
    let mid = 0
    let high = nums.length() - 1
    
    while (mid <= high) {
        if (nums[mid] == 0) {
            // Swap with low
            let temp = nums[low]
            nums[low] = nums[mid]
            nums[mid] = temp
            low = low + 1
            mid = mid + 1
        } else if (nums[mid] == 1) {
            mid = mid + 1
        } else {  // nums[mid] == 2
            // Swap with high
            let temp = nums[mid]
            nums[mid] = nums[high]
            nums[high] = temp
            high = high - 1
        }
    }
    
    return nums
}

// Example
let colors = [2, 0, 2, 1, 1, 0]
sortColors(colors)
print(colors)  // [0, 0, 1, 1, 2, 2]
```

### Problem 2: Kth Largest Element

**Problem**: Find the kth largest element in an unsorted array.

```klang
// LeetCode 215: Kth Largest Element
// Time: O(n) average, O(n²) worst, Space: O(1)
fn findKthLargest(nums, k) {
    return quickSelect(nums, 0, nums.length() - 1, nums.length() - k)
}

fn quickSelect(nums, left, right, k) {
    if (left == right) {
        return nums[left]
    }
    
    let pivotIdx = partition(nums, left, right)
    
    if (k == pivotIdx) {
        return nums[k]
    } else if (k < pivotIdx) {
        return quickSelect(nums, left, pivotIdx - 1, k)
    } else {
        return quickSelect(nums, pivotIdx + 1, right, k)
    }
}

fn partition(nums, left, right) {
    let pivot = nums[right]
    let i = left
    
    for (let j = left; j < right; j = j + 1) {
        if (nums[j] < pivot) {
            let temp = nums[i]
            nums[i] = nums[j]
            nums[j] = temp
            i = i + 1
        }
    }
    
    let temp = nums[i]
    nums[i] = nums[right]
    nums[right] = temp
    
    return i
}

// Example
let nums = [3, 2, 1, 5, 6, 4]
print(findKthLargest(nums, 2))  // 5
```

### Problem 3: Merge K Sorted Lists

**Problem**: Merge k sorted linked lists into one sorted list.

```klang
// LeetCode 23: Merge K Sorted Lists
class ListNode {
    let val = null
    let next = null
    
    fn init(value) {
        this.val = value
        this.next = null
    }
}

fn mergeKLists(lists) {
    if (lists.length() == 0) {
        return null
    }
    
    return mergeKHelper(lists, 0, lists.length() - 1)
}

fn mergeKHelper(lists, left, right) {
    if (left == right) {
        return lists[left]
    }
    
    if (left + 1 == right) {
        return mergeTwoLists(lists[left], lists[right])
    }
    
    let mid = left + (right - left) / 2
    let leftList = mergeKHelper(lists, left, mid)
    let rightList = mergeKHelper(lists, mid + 1, right)
    
    return mergeTwoLists(leftList, rightList)
}

fn mergeTwoLists(l1, l2) {
    let dummy = ListNode(-1)
    let current = dummy
    
    while (l1 != null && l2 != null) {
        if (l1.val < l2.val) {
            current.next = l1
            l1 = l1.next
        } else {
            current.next = l2
            l2 = l2.next
        }
        current = current.next
    }
    
    if (l1 != null) {
        current.next = l1
    }
    if (l2 != null) {
        current.next = l2
    }
    
    return dummy.next
}
```

### Problem 4: Largest Number

**Problem**: Given a list of non-negative integers, arrange them to form the largest number.

```klang
// LeetCode 179: Largest Number
// Time: O(n log n), Space: O(n)
fn largestNumber(nums) {
    // Convert to strings
    let strs = []
    for (let i = 0; i < nums.length(); i = i + 1) {
        strs.push(nums[i].toString())
    }
    
    // Custom sort
    customSort(strs)
    
    // Handle edge case: all zeros
    if (strs[0] == "0") {
        return "0"
    }
    
    // Concatenate
    let result = ""
    for (let i = 0; i < strs.length(); i = i + 1) {
        result = result + strs[i]
    }
    
    return result
}

fn customSort(strs) {
    // Sort using custom comparator
    for (let i = 0; i < strs.length() - 1; i = i + 1) {
        for (let j = i + 1; j < strs.length(); j = j + 1) {
            let order1 = strs[i] + strs[j]
            let order2 = strs[j] + strs[i]
            
            if (order1 < order2) {
                let temp = strs[i]
                strs[i] = strs[j]
                strs[j] = temp
            }
        }
    }
}

// Example
let nums = [3, 30, 34, 5, 9]
print(largestNumber(nums))  // "9534330"
```

### Problem 5: Sort List (Linked List)

**Problem**: Sort a linked list in O(n log n) time and O(1) space.

```klang
// LeetCode 148: Sort List
// Time: O(n log n), Space: O(1)
fn sortList(head) {
    if (head == null || head.next == null) {
        return head
    }
    
    // Find middle using slow and fast pointers
    let slow = head
    let fast = head
    let prev = null
    
    while (fast != null && fast.next != null) {
        prev = slow
        slow = slow.next
        fast = fast.next.next
    }
    
    // Split list
    prev.next = null
    
    // Recursively sort both halves
    let left = sortList(head)
    let right = sortList(slow)
    
    // Merge sorted halves
    return mergeSortedLists(left, right)
}

fn mergeSortedLists(l1, l2) {
    let dummy = ListNode(0)
    let current = dummy
    
    while (l1 != null && l2 != null) {
        if (l1.val < l2.val) {
            current.next = l1
            l1 = l1.next
        } else {
            current.next = l2
            l2 = l2.next
        }
        current = current.next
    }
    
    current.next = l1 != null ? l1 : l2
    
    return dummy.next
}
```

### Problem 6: Meeting Rooms II

**Problem**: Given meeting time intervals, find minimum number of conference rooms required.

```klang
// LeetCode 253: Meeting Rooms II
// Time: O(n log n), Space: O(n)
class Interval {
    let start = 0
    let end = 0
    
    fn init(s, e) {
        this.start = s
        this.end = e
    }
}

fn minMeetingRooms(intervals) {
    if (intervals.length() == 0) {
        return 0
    }
    
    // Separate start and end times
    let starts = []
    let ends = []
    
    for (let i = 0; i < intervals.length(); i = i + 1) {
        starts.push(intervals[i].start)
        ends.push(intervals[i].end)
    }
    
    // Sort both arrays
    mergeSortArray(starts)
    mergeSortArray(ends)
    
    let rooms = 0
    let maxRooms = 0
    let startIdx = 0
    let endIdx = 0
    
    while (startIdx < starts.length()) {
        if (starts[startIdx] < ends[endIdx]) {
            rooms = rooms + 1
            maxRooms = Math.max(maxRooms, rooms)
            startIdx = startIdx + 1
        } else {
            rooms = rooms - 1
            endIdx = endIdx + 1
        }
    }
    
    return maxRooms
}

// Example
let meetings = [
    Interval(0, 30),
    Interval(5, 10),
    Interval(15, 20)
]
print(minMeetingRooms(meetings))  // 2
```

### Problem 7: Top K Frequent Elements

**Problem**: Find the k most frequent elements in an array.

```klang
// LeetCode 347: Top K Frequent Elements
// Time: O(n), Space: O(n)
fn topKFrequent(nums, k) {
    // Count frequencies
    let freqMap = {}
    for (let i = 0; i < nums.length(); i = i + 1) {
        let num = nums[i]
        if (freqMap.hasKey(num)) {
            freqMap[num] = freqMap[num] + 1
        } else {
            freqMap[num] = 1
        }
    }
    
    // Bucket sort by frequency
    let buckets = []
    for (let i = 0; i <= nums.length(); i = i + 1) {
        buckets.push([])
    }
    
    for (let key in freqMap) {
        let freq = freqMap[key]
        buckets[freq].push(key)
    }
    
    // Collect top k
    let result = []
    for (let i = buckets.length() - 1; i >= 0 && result.length() < k; i = i - 1) {
        for (let j = 0; j < buckets[i].length(); j = j + 1) {
            result.push(buckets[i][j])
            if (result.length() == k) {
                break
            }
        }
    }
    
    return result
}

// Example
let nums = [1, 1, 1, 2, 2, 3]
print(topKFrequent(nums, 2))  // [1, 2]
```

### Problem 8: Sort Characters By Frequency

**Problem**: Sort characters in a string by decreasing frequency.

```klang
// LeetCode 451: Sort Characters by Frequency
// Time: O(n), Space: O(n)
fn frequencySort(s) {
    // Count frequencies
    let freqMap = {}
    for (let i = 0; i < s.length(); i = i + 1) {
        let char = s[i]
        if (freqMap.hasKey(char)) {
            freqMap[char] = freqMap[char] + 1
        } else {
            freqMap[char] = 1
        }
    }
    
    // Bucket sort
    let buckets = []
    for (let i = 0; i <= s.length(); i = i + 1) {
        buckets.push([])
    }
    
    for (let char in freqMap) {
        let freq = freqMap[char]
        buckets[freq].push(char)
    }
    
    // Build result
    let result = ""
    for (let i = buckets.length() - 1; i >= 0; i = i - 1) {
        for (let j = 0; j < buckets[i].length(); j = j + 1) {
            let char = buckets[i][j]
            for (let count = 0; count < i; count = count + 1) {
                result = result + char
            }
        }
    }
    
    return result
}

// Example
print(frequencySort("tree"))  // "eert" or "eetr"
```

### Problem 9: Custom Sort String

**Problem**: Sort string based on custom order defined in another string.

```klang
// LeetCode 791: Custom Sort String
// Time: O(n), Space: O(1)
fn customSortString(order, s) {
    // Count characters in s
    let count = {}
    for (let i = 0; i < s.length(); i = i + 1) {
        let char = s[i]
        if (count.hasKey(char)) {
            count[char] = count[char] + 1
        } else {
            count[char] = 1
        }
    }
    
    let result = ""
    
    // Add characters in order
    for (let i = 0; i < order.length(); i = i + 1) {
        let char = order[i]
        if (count.hasKey(char)) {
            for (let j = 0; j < count[char]; j = j + 1) {
                result = result + char
            }
            count.delete(char)
        }
    }
    
    // Add remaining characters
    for (let char in count) {
        for (let j = 0; j < count[char]; j = j + 1) {
            result = result + char
        }
    }
    
    return result
}

// Example
print(customSortString("cba", "abcd"))  // "cbad"
```

### Problem 10: Pancake Sorting

**Problem**: Sort array using only pancake flips (reverse prefix).

```klang
// LeetCode 969: Pancake Sorting
// Time: O(n²), Space: O(1)
fn pancakeSort(arr) {
    let result = []
    let n = arr.length()
    
    for (let size = n; size > 1; size = size - 1) {
        // Find index of maximum in arr[0...size-1]
        let maxIdx = 0
        for (let i = 1; i < size; i = i + 1) {
            if (arr[i] > arr[maxIdx]) {
                maxIdx = i
            }
        }
        
        // Move max to beginning if not already there
        if (maxIdx != 0) {
            flip(arr, maxIdx)
            result.push(maxIdx + 1)
        }
        
        // Move max to its final position
        flip(arr, size - 1)
        result.push(size)
    }
    
    return result
}

fn flip(arr, k) {
    let left = 0
    let right = k
    
    while (left < right) {
        let temp = arr[left]
        arr[left] = arr[right]
        arr[right] = temp
        left = left + 1
        right = right - 1
    }
}

// Example
let arr = [3, 2, 4, 1]
print(pancakeSort(arr))  // Sequence of flips
```

### Problem 11: Wiggle Sort II

**Problem**: Reorder array so nums[0] < nums[1] > nums[2] < nums[3]...

```klang
// LeetCode 324: Wiggle Sort II
// Time: O(n log n), Space: O(n)
fn wiggleSort(nums) {
    let sorted = []
    for (let i = 0; i < nums.length(); i = i + 1) {
        sorted.push(nums[i])
    }
    
    mergeSortArray(sorted)
    
    let n = nums.length()
    let mid = (n + 1) / 2
    let left = mid - 1
    let right = n - 1
    
    for (let i = 0; i < n; i = i + 1) {
        if (i % 2 == 0) {
            nums[i] = sorted[left]
            left = left - 1
        } else {
            nums[i] = sorted[right]
            right = right - 1
        }
    }
    
    return nums
}

// Example
let nums = [1, 5, 1, 1, 6, 4]
wiggleSort(nums)
print(nums)  // [1, 6, 1, 5, 1, 4]
```

### Problem 12: Sort Array by Increasing Frequency

**Problem**: Sort by frequency (ascending), then by value (descending).

```klang
// LeetCode 1636: Sort Array by Increasing Frequency
fn frequencySort(nums) {
    // Count frequencies
    let freqMap = {}
    for (let i = 0; i < nums.length(); i = i + 1) {
        let num = nums[i]
        if (freqMap.hasKey(num)) {
            freqMap[num] = freqMap[num] + 1
        } else {
            freqMap[num] = 1
        }
    }
    
    // Custom sort
    for (let i = 0; i < nums.length() - 1; i = i + 1) {
        for (let j = i + 1; j < nums.length(); j = j + 1) {
            let freq1 = freqMap[nums[i]]
            let freq2 = freqMap[nums[j]]
            
            // Sort by frequency ascending, then value descending
            if (freq1 > freq2 || (freq1 == freq2 && nums[i] < nums[j])) {
                let temp = nums[i]
                nums[i] = nums[j]
                nums[j] = temp
            }
        }
    }
    
    return nums
}

// Example
let nums = [1, 1, 2, 2, 2, 3]
print(frequencySort(nums))  // [3, 1, 1, 2, 2, 2]
```

---

## Best Practices

### 1. Choose the Right Algorithm

- **Know the data**: Size, range, distribution
- **Know the constraints**: Time, space, stability
- **Profile first**: Don't optimize prematurely

### 2. Implement Carefully

```klang
// ✓ Good: Clear variable names
fn bubbleSort(arr) {
    let n = arr.length()
    for (let i = 0; i < n - 1; i = i + 1) {
        let swapped = false
        // ... clear logic
    }
}

// ✗ Bad: Unclear names
fn bs(a) {
    let n = a.length()
    for (let x = 0; x < n - 1; x = x + 1) {
        let f = false
        // ... confusing
    }
}
```

### 3. Handle Edge Cases

```klang
fn safeSort(arr) {
    // Check for null or empty
    if (arr == null || arr.length() == 0) {
        return arr
    }
    
    // Check for single element
    if (arr.length() == 1) {
        return arr
    }
    
    // Proceed with sorting
    return quickSortArray(arr)
}
```

### 4. Use Hybrid Approaches

```klang
// Combine algorithms for better performance
fn hybridSort(arr, left, right) {
    let size = right - left + 1
    
    // Use insertion sort for small subarrays
    if (size < 10) {
        return insertionSortRange(arr, left, right)
    }
    
    // Use quick sort for larger arrays
    return quickSort(arr, left, right)
}
```

### 5. Consider Stability When Needed

```klang
// For multi-key sorting, use stable sort
class Student {
    let name = ""
    let score = 0
}

// Sort by score (stable sort preserves name order)
let students = [
    Student("Alice", 85),
    Student("Bob", 90),
    Student("Charlie", 85)
]

// Stable sort maintains: Alice before Charlie (both 85)
mergeSort(students)  // ✓ Stable
quickSort(students)  // ✗ Might change order
```

### 6. Optimize for Common Cases

```klang
fn optimizedSort(arr) {
    // Check if already sorted
    if (isSorted(arr)) {
        return arr
    }
    
    // Check if reverse sorted
    if (isReverseSorted(arr)) {
        return reverse(arr)
    }
    
    // Otherwise, sort normally
    return quickSortArray(arr)
}

fn isSorted(arr) {
    for (let i = 0; i < arr.length() - 1; i = i + 1) {
        if (arr[i] > arr[i + 1]) {
            return false
        }
    }
    return true
}
```

### 7. Test Thoroughly

```klang
fn testSort(sortFunc) {
    // Test empty array
    assert(sortFunc([]) == [])
    
    // Test single element
    assert(sortFunc([5]) == [5])
    
    // Test already sorted
    assert(sortFunc([1, 2, 3]) == [1, 2, 3])
    
    // Test reverse sorted
    assert(sortFunc([3, 2, 1]) == [1, 2, 3])
    
    // Test duplicates
    assert(sortFunc([3, 1, 2, 1, 3]) == [1, 1, 2, 3, 3])
    
    // Test negative numbers
    assert(sortFunc([-1, -5, 0, 3]) == [-5, -1, 0, 3])
}
```

### 8. Document Complexity

```klang
/**
 * Sorts array using merge sort algorithm
 * 
 * Time Complexity: O(n log n) - all cases
 * Space Complexity: O(n) - temporary arrays
 * 
 * Stable: Yes
 * In-place: No
 * 
 * Best for: Large arrays, guaranteed performance, linked lists
 */
fn mergeSort(arr) {
    // implementation
}
```

### 9. Use Built-in Sorts When Available

```klang
// If KLang has built-in sort, use it for production
arr.sort()  // Usually optimized

// Implement custom only when:
// 1. Learning/practice
// 2. Specific requirements (stability, custom comparison)
// 3. Performance critical with known data patterns
```

### 10. Measure and Compare

```klang
fn benchmarkSort(sortFunc, arr) {
    let start = getTimestamp()
    sortFunc(arr)
    let end = getTimestamp()
    
    print("Time: " + (end - start) + "ms")
}

// Compare algorithms
let data = generateRandomArray(10000)
benchmarkSort(quickSort, data.copy())
benchmarkSort(mergeSort, data.copy())
benchmarkSort(heapSort, data.copy())
```

---

## Conclusion

Sorting algorithms are fundamental to computer science and essential for technical interviews. Understanding when and how to use each algorithm is crucial for writing efficient code.

### Key Takeaways

1. **Know the Trade-offs**: Time, space, stability
2. **Choose Wisely**: Match algorithm to data characteristics
3. **Hybrid is Best**: Combine algorithms for optimal performance
4. **Practice Implementation**: Build muscle memory
5. **Understand Applications**: Where each algorithm shines

### Further Study

- **TimSort**: Python's hybrid merge-insertion sort
- **IntroSort**: C++'s hybrid quick-heap-insertion sort
- **External Sorting**: For data larger than memory
- **Parallel Sorting**: Multi-threaded algorithms
- **Network Sorting**: Sorting networks and parallel comparisons

Keep practicing, and happy sorting! 🚀
