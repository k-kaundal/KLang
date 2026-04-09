# Advanced Data Structures in KLang

## Introduction

Advanced data structures are specialized structures designed to solve complex problems efficiently. Unlike basic structures (arrays, linked lists, stacks, queues), these structures are optimized for specific operations like range queries, set operations, and probabilistic membership testing.

### Why Advanced Structures?

- **Performance**: Solve problems that basic structures can't handle efficiently
- **Specialized operations**: Range queries, union-find, prefix sums
- **Space-time tradeoffs**: Balance memory usage with query speed
- **Real-world applications**: Database indexing, network routing, text processing

### Key Characteristics

- **Logarithmic complexity**: Most operations run in O(log n)
- **Space efficient**: Often use less space than naive approaches
- **Problem-specific**: Each structure excels at particular problem types
- **Applications**: Competitive programming, system design, algorithm optimization

## Segment Trees

A **segment tree** is a tree data structure used for storing information about intervals or segments. It allows answering range queries efficiently while still allowing modifications.

### When to Use

- Range sum/min/max queries
- Range updates
- When you need both queries and updates
- Array elements can change (mutable)

### Structure

```klang
// Segment tree for range sum queries
//
// Array: [1, 3, 5, 7, 9, 11]
//
// Tree representation:
//                 36 [0-5]
//               /          \
//          9 [0-2]          27 [3-5]
//         /       \        /        \
//     4 [0-1]  5 [2]   16 [3-4]  11 [5]
//     /     \           /     \
//  1 [0]  3 [1]      7 [3]  9 [4]
```

### Properties

- **Height**: O(log n)
- **Build time**: O(n)
- **Query time**: O(log n)
- **Update time**: O(log n)
- **Space**: O(4n) = O(n)

### Complete Implementation

```klang
class SegmentTree {
    var tree: [int];
    var n: int;
    
    // Constructor: Build segment tree from array
    func new(arr: [int]) {
        this.n = arr.length;
        this.tree = [];
        
        // Allocate space for tree (4 * n is safe upper bound)
        let i = 0;
        while (i < 4 * this.n) {
            this.tree.push(0);
            i = i + 1;
        }
        
        if (this.n > 0) {
            this.build(arr, 0, 0, this.n - 1);
        }
    }
    
    // Build tree recursively
    func build(arr: [int], node: int, start: int, end: int) {
        if (start == end) {
            // Leaf node
            this.tree[node] = arr[start];
        } else {
            let mid = start + (end - start) / 2;
            let leftChild = 2 * node + 1;
            let rightChild = 2 * node + 2;
            
            // Build left and right subtrees
            this.build(arr, leftChild, start, mid);
            this.build(arr, rightChild, mid + 1, end);
            
            // Internal node stores sum of children
            this.tree[node] = this.tree[leftChild] + this.tree[rightChild];
        }
    }
    
    // Query sum in range [queryStart, queryEnd]
    func query(queryStart: int, queryEnd: int): int {
        return this.queryHelper(0, 0, this.n - 1, queryStart, queryEnd);
    }
    
    func queryHelper(node: int, start: int, end: int, 
                     queryStart: int, queryEnd: int): int {
        // No overlap
        if (queryStart > end || queryEnd < start) {
            return 0;
        }
        
        // Complete overlap
        if (queryStart <= start && queryEnd >= end) {
            return this.tree[node];
        }
        
        // Partial overlap
        let mid = start + (end - start) / 2;
        let leftChild = 2 * node + 1;
        let rightChild = 2 * node + 2;
        
        let leftSum = this.queryHelper(leftChild, start, mid, queryStart, queryEnd);
        let rightSum = this.queryHelper(rightChild, mid + 1, end, queryStart, queryEnd);
        
        return leftSum + rightSum;
    }
    
    // Update element at index to new value
    func update(index: int, value: int) {
        this.updateHelper(0, 0, this.n - 1, index, value);
    }
    
    func updateHelper(node: int, start: int, end: int, 
                      index: int, value: int) {
        if (start == end) {
            // Leaf node
            this.tree[node] = value;
        } else {
            let mid = start + (end - start) / 2;
            let leftChild = 2 * node + 1;
            let rightChild = 2 * node + 2;
            
            if (index <= mid) {
                // Update left subtree
                this.updateHelper(leftChild, start, mid, index, value);
            } else {
                // Update right subtree
                this.updateHelper(rightChild, mid + 1, end, index, value);
            }
            
            // Update current node
            this.tree[node] = this.tree[leftChild] + this.tree[rightChild];
        }
    }
    
    // Range update: add delta to all elements in range
    func rangeUpdate(left: int, right: int, delta: int) {
        this.rangeUpdateHelper(0, 0, this.n - 1, left, right, delta);
    }
    
    func rangeUpdateHelper(node: int, start: int, end: int,
                          left: int, right: int, delta: int) {
        // No overlap
        if (left > end || right < start) {
            return;
        }
        
        if (start == end) {
            // Leaf node
            this.tree[node] = this.tree[node] + delta;
            return;
        }
        
        let mid = start + (end - start) / 2;
        let leftChild = 2 * node + 1;
        let rightChild = 2 * node + 2;
        
        this.rangeUpdateHelper(leftChild, start, mid, left, right, delta);
        this.rangeUpdateHelper(rightChild, mid + 1, end, left, right, delta);
        
        this.tree[node] = this.tree[leftChild] + this.tree[rightChild];
    }
}

// Usage example
let arr = [1, 3, 5, 7, 9, 11];
let segTree = new SegmentTree(arr);

print(segTree.query(1, 4));  // Sum of arr[1..4] = 3+5+7+9 = 24

segTree.update(2, 10);       // arr[2] = 10
print(segTree.query(1, 4));  // Sum = 3+10+7+9 = 29
```

### Range Minimum Query

```klang
class SegmentTreeMin {
    var tree: [int];
    var n: int;
    
    func new(arr: [int]) {
        this.n = arr.length;
        this.tree = [];
        
        let i = 0;
        while (i < 4 * this.n) {
            this.tree.push(2147483647); // INT_MAX
            i = i + 1;
        }
        
        if (this.n > 0) {
            this.build(arr, 0, 0, this.n - 1);
        }
    }
    
    func build(arr: [int], node: int, start: int, end: int) {
        if (start == end) {
            this.tree[node] = arr[start];
        } else {
            let mid = start + (end - start) / 2;
            let leftChild = 2 * node + 1;
            let rightChild = 2 * node + 2;
            
            this.build(arr, leftChild, start, mid);
            this.build(arr, rightChild, mid + 1, end);
            
            // Store minimum of children
            if (this.tree[leftChild] < this.tree[rightChild]) {
                this.tree[node] = this.tree[leftChild];
            } else {
                this.tree[node] = this.tree[rightChild];
            }
        }
    }
    
    func query(queryStart: int, queryEnd: int): int {
        return this.queryHelper(0, 0, this.n - 1, queryStart, queryEnd);
    }
    
    func queryHelper(node: int, start: int, end: int,
                     queryStart: int, queryEnd: int): int {
        if (queryStart > end || queryEnd < start) {
            return 2147483647; // INT_MAX
        }
        
        if (queryStart <= start && queryEnd >= end) {
            return this.tree[node];
        }
        
        let mid = start + (end - start) / 2;
        let leftChild = 2 * node + 1;
        let rightChild = 2 * node + 2;
        
        let leftMin = this.queryHelper(leftChild, start, mid, queryStart, queryEnd);
        let rightMin = this.queryHelper(rightChild, mid + 1, end, queryStart, queryEnd);
        
        if (leftMin < rightMin) {
            return leftMin;
        } else {
            return rightMin;
        }
    }
}
```

## Fenwick Tree (Binary Indexed Tree)

A **Fenwick tree** or **Binary Indexed Tree (BIT)** is a data structure that can efficiently update elements and calculate prefix sums in logarithmic time.

### When to Use

- Prefix sum queries
- Cumulative frequency tables
- When updates are frequent
- Space-efficient alternative to segment trees

### Structure

```klang
// Array:        [3, 2, -1, 6, 5, 4, -3, 3]
// Index:         1  2   3  4  5  6   7  8  (1-indexed)
// 
// BIT structure (each node stores sum of range):
// tree[1] = arr[1]           = 3
// tree[2] = arr[1..2]        = 5
// tree[3] = arr[3]           = -1
// tree[4] = arr[1..4]        = 10
// tree[5] = arr[5]           = 5
// tree[6] = arr[5..6]        = 9
// tree[7] = arr[7]           = -3
// tree[8] = arr[1..8]        = 19
```

### Properties

- **Height**: O(log n)
- **Build time**: O(n log n) or O(n) with optimization
- **Query time**: O(log n)
- **Update time**: O(log n)
- **Space**: O(n)
- **Index**: 1-based indexing (easier implementation)

### Complete Implementation

```klang
class FenwickTree {
    var tree: [int];
    var n: int;
    
    // Constructor: Build from array
    func new(arr: [int]) {
        this.n = arr.length;
        this.tree = [0]; // Index 0 unused (1-based indexing)
        
        let i = 0;
        while (i < this.n) {
            this.tree.push(0);
            i = i + 1;
        }
        
        // Build tree
        i = 0;
        while (i < arr.length) {
            this.update(i + 1, arr[i]); // Convert to 1-based
            i = i + 1;
        }
    }
    
    // Get least significant bit
    func lsb(x: int): int {
        return x & (-x);
    }
    
    // Add delta to element at index (1-based)
    func update(index: int, delta: int) {
        while (index <= this.n) {
            this.tree[index] = this.tree[index] + delta;
            index = index + this.lsb(index);
        }
    }
    
    // Get prefix sum from 1 to index (1-based)
    func prefixSum(index: int): int {
        let sum = 0;
        while (index > 0) {
            sum = sum + this.tree[index];
            index = index - this.lsb(index);
        }
        return sum;
    }
    
    // Get sum of range [left, right] (1-based)
    func rangeSum(left: int, right: int): int {
        if (left > 1) {
            return this.prefixSum(right) - this.prefixSum(left - 1);
        } else {
            return this.prefixSum(right);
        }
    }
    
    // Set element at index to value (1-based)
    func set(index: int, value: int) {
        let currentValue = this.rangeSum(index, index);
        let delta = value - currentValue;
        this.update(index, delta);
    }
}

// Usage example
let arr = [3, 2, -1, 6, 5, 4, -3, 3];
let fenwick = new FenwickTree(arr);

// Query prefix sum
print(fenwick.prefixSum(4));      // Sum of first 4 elements = 10

// Query range sum
print(fenwick.rangeSum(2, 5));    // Sum of arr[2..5] = 2+(-1)+6+5 = 12

// Update element
fenwick.update(3, 5);              // Add 5 to arr[3]
print(fenwick.rangeSum(2, 5));    // Now = 17
```

### 2D Fenwick Tree

```klang
class FenwickTree2D {
    var tree: [[int]];
    var rows: int;
    var cols: int;
    
    func new(matrix: [[int]]) {
        this.rows = matrix.length;
        this.cols = 0;
        if (this.rows > 0) {
            this.cols = matrix[0].length;
        }
        
        // Initialize tree
        this.tree = [];
        let i = 0;
        while (i <= this.rows) {
            let row: [int] = [];
            let j = 0;
            while (j <= this.cols) {
                row.push(0);
                j = j + 1;
            }
            this.tree.push(row);
            i = i + 1;
        }
        
        // Build tree
        i = 0;
        while (i < this.rows) {
            let j = 0;
            while (j < this.cols) {
                this.update(i + 1, j + 1, matrix[i][j]);
                j = j + 1;
            }
            i = i + 1;
        }
    }
    
    func lsb(x: int): int {
        return x & (-x);
    }
    
    func update(row: int, col: int, delta: int) {
        let i = row;
        while (i <= this.rows) {
            let j = col;
            while (j <= this.cols) {
                this.tree[i][j] = this.tree[i][j] + delta;
                j = j + this.lsb(j);
            }
            i = i + this.lsb(i);
        }
    }
    
    func query(row: int, col: int): int {
        let sum = 0;
        let i = row;
        while (i > 0) {
            let j = col;
            while (j > 0) {
                sum = sum + this.tree[i][j];
                j = j - this.lsb(j);
            }
            i = i - this.lsb(i);
        }
        return sum;
    }
    
    func regionSum(row1: int, col1: int, row2: int, col2: int): int {
        // Sum of rectangle from (row1, col1) to (row2, col2)
        return this.query(row2, col2) 
             - this.query(row1 - 1, col2)
             - this.query(row2, col1 - 1)
             + this.query(row1 - 1, col1 - 1);
    }
}
```

## Union-Find (Disjoint Set Union)

**Union-Find** is a data structure that keeps track of elements partitioned into disjoint (non-overlapping) sets. It provides near-constant-time operations to merge sets and determine if elements are in the same set.

### When to Use

- Graph connectivity problems
- Detecting cycles in undirected graphs
- Kruskal's MST algorithm
- Network connectivity
- Group/cluster problems

### Structure

```klang
// Initial: Each element is its own set
// Sets: {0}, {1}, {2}, {3}, {4}, {5}
//
// After union(0, 1), union(2, 3), union(4, 5):
// Sets: {0, 1}, {2, 3}, {4, 5}
//
// After union(1, 3):
// Sets: {0, 1, 2, 3}, {4, 5}
```

### Properties

- **Find time**: O(α(n)) ≈ O(1) with path compression
- **Union time**: O(α(n)) ≈ O(1) with union by rank
- **Space**: O(n)
- **α(n)**: Inverse Ackermann function (grows extremely slowly)

### Complete Implementation

```klang
class UnionFind {
    var parent: [int];
    var rank: [int];
    var count: int; // Number of disjoint sets
    
    func new(n: int) {
        this.parent = [];
        this.rank = [];
        this.count = n;
        
        // Initialize each element as its own parent
        let i = 0;
        while (i < n) {
            this.parent.push(i);
            this.rank.push(0);
            i = i + 1;
        }
    }
    
    // Find with path compression
    func find(x: int): int {
        if (this.parent[x] != x) {
            // Path compression: make every node point to root
            this.parent[x] = this.find(this.parent[x]);
        }
        return this.parent[x];
    }
    
    // Union by rank
    func union(x: int, y: int): bool {
        let rootX = this.find(x);
        let rootY = this.find(y);
        
        // Already in same set
        if (rootX == rootY) {
            return false;
        }
        
        // Union by rank: attach smaller tree under larger tree
        if (this.rank[rootX] < this.rank[rootY]) {
            this.parent[rootX] = rootY;
        } else if (this.rank[rootX] > this.rank[rootY]) {
            this.parent[rootY] = rootX;
        } else {
            this.parent[rootY] = rootX;
            this.rank[rootX] = this.rank[rootX] + 1;
        }
        
        this.count = this.count - 1;
        return true;
    }
    
    // Check if x and y are in the same set
    func connected(x: int, y: int): bool {
        return this.find(x) == this.find(y);
    }
    
    // Get number of disjoint sets
    func getCount(): int {
        return this.count;
    }
    
    // Get size of set containing x
    func getSetSize(x: int): int {
        let root = this.find(x);
        let size = 0;
        let i = 0;
        while (i < this.parent.length) {
            if (this.find(i) == root) {
                size = size + 1;
            }
            i = i + 1;
        }
        return size;
    }
}

// Usage example
let uf = new UnionFind(6);

print(uf.getCount());           // 6 (initially 6 separate sets)

uf.union(0, 1);
uf.union(2, 3);
uf.union(4, 5);

print(uf.getCount());           // 3 sets: {0,1}, {2,3}, {4,5}
print(uf.connected(0, 1));      // true
print(uf.connected(0, 2));      // false

uf.union(1, 3);                 // Connect {0,1} and {2,3}
print(uf.connected(0, 3));      // true
print(uf.getCount());           // 2 sets: {0,1,2,3}, {4,5}
```

### Union-Find with Set Size Tracking

```klang
class UnionFindWithSize {
    var parent: [int];
    var size: [int]; // Size of each set
    var count: int;
    
    func new(n: int) {
        this.parent = [];
        this.size = [];
        this.count = n;
        
        let i = 0;
        while (i < n) {
            this.parent.push(i);
            this.size.push(1);
            i = i + 1;
        }
    }
    
    func find(x: int): int {
        if (this.parent[x] != x) {
            this.parent[x] = this.find(this.parent[x]);
        }
        return this.parent[x];
    }
    
    func union(x: int, y: int): bool {
        let rootX = this.find(x);
        let rootY = this.find(y);
        
        if (rootX == rootY) {
            return false;
        }
        
        // Union by size: attach smaller tree under larger tree
        if (this.size[rootX] < this.size[rootY]) {
            this.parent[rootX] = rootY;
            this.size[rootY] = this.size[rootY] + this.size[rootX];
        } else {
            this.parent[rootY] = rootX;
            this.size[rootX] = this.size[rootX] + this.size[rootY];
        }
        
        this.count = this.count - 1;
        return true;
    }
    
    func getSetSize(x: int): int {
        return this.size[this.find(x)];
    }
    
    func getLargestSetSize(): int {
        let maxSize = 0;
        let i = 0;
        while (i < this.parent.length) {
            if (this.parent[i] == i && this.size[i] > maxSize) {
                maxSize = this.size[i];
            }
            i = i + 1;
        }
        return maxSize;
    }
}
```

## Suffix Array

A **suffix array** is a sorted array of all suffixes of a string. It's a space-efficient alternative to suffix trees for string processing.

### When to Use

- Pattern matching in strings
- Finding repeated substrings
- Longest common prefix (LCP) problems
- String compression
- Text indexing

### Structure

```klang
// String: "banana"
// Suffixes:
// 0: "banana"
// 1: "anana"
// 2: "nana"
// 3: "ana"
// 4: "na"
// 5: "a"
//
// Sorted suffixes:
// 5: "a"      -> suffix array[0] = 5
// 3: "ana"    -> suffix array[1] = 3
// 1: "anana"  -> suffix array[2] = 1
// 0: "banana" -> suffix array[3] = 0
// 4: "na"     -> suffix array[4] = 4
// 2: "nana"   -> suffix array[5] = 2
//
// Suffix Array: [5, 3, 1, 0, 4, 2]
```

### Properties

- **Build time**: O(n log n) or O(n)
- **Search time**: O(m log n) for pattern of length m
- **Space**: O(n)

### Complete Implementation

```klang
class SuffixArray {
    var text: string;
    var suffixArr: [int];
    var lcp: [int]; // Longest Common Prefix array
    
    func new(text: string) {
        this.text = text;
        this.suffixArr = [];
        this.lcp = [];
        this.buildSuffixArray();
        this.buildLCPArray();
    }
    
    // Simple O(n^2 log n) construction
    func buildSuffixArray() {
        let n = this.text.length;
        
        // Create array of suffix indices
        let i = 0;
        while (i < n) {
            this.suffixArr.push(i);
            i = i + 1;
        }
        
        // Sort suffixes using bubble sort (simplified)
        // In practice, use more efficient sorting
        i = 0;
        while (i < n - 1) {
            let j = 0;
            while (j < n - i - 1) {
                if (this.compareSuffixes(this.suffixArr[j], 
                                        this.suffixArr[j + 1]) > 0) {
                    // Swap
                    let temp = this.suffixArr[j];
                    this.suffixArr[j] = this.suffixArr[j + 1];
                    this.suffixArr[j + 1] = temp;
                }
                j = j + 1;
            }
            i = i + 1;
        }
    }
    
    // Compare two suffixes
    func compareSuffixes(i: int, j: int): int {
        let len = this.text.length;
        while (i < len && j < len) {
            if (this.text[i] < this.text[j]) {
                return -1;
            } else if (this.text[i] > this.text[j]) {
                return 1;
            }
            i = i + 1;
            j = j + 1;
        }
        
        if (i < len) {
            return 1;
        } else if (j < len) {
            return -1;
        }
        return 0;
    }
    
    // Build LCP array: lcp[i] = longest common prefix of 
    // suffixes at suffixArr[i] and suffixArr[i-1]
    func buildLCPArray() {
        let n = this.text.length;
        this.lcp = [0]; // lcp[0] is always 0
        
        let i = 1;
        while (i < n) {
            let lcpLen = this.computeLCP(this.suffixArr[i - 1], 
                                         this.suffixArr[i]);
            this.lcp.push(lcpLen);
            i = i + 1;
        }
    }
    
    // Compute LCP length of two suffixes
    func computeLCP(i: int, j: int): int {
        let len = 0;
        let n = this.text.length;
        while (i < n && j < n && this.text[i] == this.text[j]) {
            len = len + 1;
            i = i + 1;
            j = j + 1;
        }
        return len;
    }
    
    // Search for pattern in text
    func search(pattern: string): [int] {
        let results: [int] = [];
        let n = this.suffixArr.length;
        
        // Binary search for pattern
        let left = 0;
        let right = n - 1;
        
        while (left <= right) {
            let mid = left + (right - left) / 2;
            let suffix = this.suffixArr[mid];
            let cmp = this.compareWithPattern(suffix, pattern);
            
            if (cmp == 0) {
                // Found match, collect all occurrences
                results.push(suffix);
                
                // Check left neighbors
                let i = mid - 1;
                while (i >= 0 && this.compareWithPattern(
                       this.suffixArr[i], pattern) == 0) {
                    results.push(this.suffixArr[i]);
                    i = i - 1;
                }
                
                // Check right neighbors
                i = mid + 1;
                while (i < n && this.compareWithPattern(
                       this.suffixArr[i], pattern) == 0) {
                    results.push(this.suffixArr[i]);
                    i = i + 1;
                }
                
                return results;
            } else if (cmp < 0) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return results;
    }
    
    // Compare suffix with pattern
    func compareWithPattern(suffixIdx: int, pattern: string): int {
        let i = 0;
        while (i < pattern.length && suffixIdx + i < this.text.length) {
            if (this.text[suffixIdx + i] < pattern[i]) {
                return -1;
            } else if (this.text[suffixIdx + i] > pattern[i]) {
                return 1;
            }
            i = i + 1;
        }
        
        if (i < pattern.length) {
            return -1; // Text ended before pattern
        }
        return 0; // Match
    }
    
    // Find longest repeated substring
    func longestRepeatedSubstring(): string {
        let maxLen = 0;
        let startIdx = 0;
        
        let i = 1;
        while (i < this.lcp.length) {
            if (this.lcp[i] > maxLen) {
                maxLen = this.lcp[i];
                startIdx = this.suffixArr[i];
            }
            i = i + 1;
        }
        
        if (maxLen == 0) {
            return "";
        }
        
        return this.text.substring(startIdx, startIdx + maxLen);
    }
}

// Usage example
let sa = new SuffixArray("banana");
print(sa.suffixArr);           // [5, 3, 1, 0, 4, 2]

let matches = sa.search("ana");
print(matches);                // [1, 3] (positions where "ana" occurs)

print(sa.longestRepeatedSubstring()); // "ana"
```

## Bloom Filter

A **Bloom filter** is a space-efficient probabilistic data structure used to test whether an element is a member of a set. It can have false positives but never false negatives.

### When to Use

- Space-constrained membership testing
- Cache filtering
- Spell checkers
- Database query optimization
- Network routers

### Properties

- **Space**: O(m) where m << n (very space efficient)
- **Insert time**: O(k) where k is number of hash functions
- **Query time**: O(k)
- **False positive rate**: Depends on m, n, and k
- **No false negatives**: If says "not present", definitely not present

### Complete Implementation

```klang
class BloomFilter {
    var bits: [bool];
    var size: int;
    var numHashes: int;
    
    // Constructor
    func new(size: int, numHashes: int) {
        this.size = size;
        this.numHashes = numHashes;
        this.bits = [];
        
        let i = 0;
        while (i < size) {
            this.bits.push(false);
            i = i + 1;
        }
    }
    
    // Simple hash function (in practice, use better hash functions)
    func hash(item: string, seed: int): int {
        let hash = seed;
        let i = 0;
        while (i < item.length) {
            // Simple polynomial rolling hash
            hash = (hash * 31 + item.charCodeAt(i)) % this.size;
            i = i + 1;
        }
        if (hash < 0) {
            hash = hash + this.size;
        }
        return hash;
    }
    
    // Add item to bloom filter
    func add(item: string) {
        let i = 0;
        while (i < this.numHashes) {
            let index = this.hash(item, i);
            this.bits[index] = true;
            i = i + 1;
        }
    }
    
    // Check if item might be in set
    func mightContain(item: string): bool {
        let i = 0;
        while (i < this.numHashes) {
            let index = this.hash(item, i);
            if (!this.bits[index]) {
                return false; // Definitely not in set
            }
            i = i + 1;
        }
        return true; // Might be in set (could be false positive)
    }
    
    // Clear all bits
    func clear() {
        let i = 0;
        while (i < this.size) {
            this.bits[i] = false;
            i = i + 1;
        }
    }
    
    // Estimate false positive probability
    func estimateFPP(numItems: int): float {
        // FPP ≈ (1 - e^(-k*n/m))^k
        // where k = numHashes, n = numItems, m = size
        let exponent = -1.0 * this.numHashes * numItems / this.size;
        let base = 1.0 - this.exp(exponent);
        return this.pow(base, this.numHashes);
    }
    
    // Helper: approximate e^x
    func exp(x: float): float {
        // Simple approximation using Taylor series
        let result = 1.0;
        let term = 1.0;
        let i = 1;
        while (i <= 20) {
            term = term * x / i;
            result = result + term;
            i = i + 1;
        }
        return result;
    }
    
    // Helper: approximate x^y
    func pow(x: float, y: int): float {
        let result = 1.0;
        let i = 0;
        while (i < y) {
            result = result * x;
            i = i + 1;
        }
        return result;
    }
}

// Usage example
let bloom = new BloomFilter(1000, 3); // 1000 bits, 3 hash functions

bloom.add("apple");
bloom.add("banana");
bloom.add("orange");

print(bloom.mightContain("apple"));   // true (definitely added)
print(bloom.mightContain("banana"));  // true (definitely added)
print(bloom.mightContain("grape"));   // false (definitely not added)
print(bloom.mightContain("mango"));   // false or true (might be false positive)
```

## Skip List

A **skip list** is a probabilistic data structure that allows fast search, insertion, and deletion operations. It's an alternative to balanced trees with simpler implementation.

### When to Use

- Simpler alternative to balanced trees
- Concurrent data structures (easier to lock)
- Sorted data with frequent insertions/deletions
- When you want O(log n) average case

### Structure

```klang
// Skip list with 3 levels
// Level 2: 1 -----------------> 9 ----------> null
// Level 1: 1 -------> 4 ------> 9 ----------> null
// Level 0: 1 -> 3 -> 4 -> 7 -> 9 -> 11 ----> null
```

### Properties

- **Search time**: O(log n) average, O(n) worst case
- **Insert time**: O(log n) average
- **Delete time**: O(log n) average
- **Space**: O(n log n) expected

### Complete Implementation

```klang
class SkipNode {
    var value: int;
    var forward: [SkipNode]; // Array of forward pointers
    
    func new(value: int, level: int) {
        this.value = value;
        this.forward = [];
        let i = 0;
        while (i <= level) {
            this.forward.push(null);
            i = i + 1;
        }
    }
}

class SkipList {
    var head: SkipNode;
    var maxLevel: int;
    var level: int;
    var probability: float;
    
    func new(maxLevel: int) {
        this.maxLevel = maxLevel;
        this.level = 0;
        this.probability = 0.5;
        this.head = new SkipNode(-2147483648, maxLevel); // INT_MIN
    }
    
    // Generate random level for new node
    func randomLevel(): int {
        let lvl = 0;
        while (this.random() < this.probability && lvl < this.maxLevel) {
            lvl = lvl + 1;
        }
        return lvl;
    }
    
    // Simple random number generator (0.0 to 1.0)
    func random(): float {
        // Simplified: in practice use better RNG
        return 0.5; // Placeholder
    }
    
    // Search for value
    func search(value: int): bool {
        let current = this.head;
        let i = this.level;
        
        // Start from highest level
        while (i >= 0) {
            while (current.forward[i] != null && 
                   current.forward[i].value < value) {
                current = current.forward[i];
            }
            i = i - 1;
        }
        
        current = current.forward[0];
        return current != null && current.value == value;
    }
    
    // Insert value
    func insert(value: int) {
        let update: [SkipNode] = [];
        let i = 0;
        while (i <= this.maxLevel) {
            update.push(null);
            i = i + 1;
        }
        
        let current = this.head;
        i = this.level;
        
        // Find position to insert
        while (i >= 0) {
            while (current.forward[i] != null && 
                   current.forward[i].value < value) {
                current = current.forward[i];
            }
            update[i] = current;
            i = i - 1;
        }
        
        current = current.forward[0];
        
        // Value already exists
        if (current != null && current.value == value) {
            return;
        }
        
        // Generate random level for new node
        let newLevel = this.randomLevel();
        
        // Update list level if necessary
        if (newLevel > this.level) {
            i = this.level + 1;
            while (i <= newLevel) {
                update[i] = this.head;
                i = i + 1;
            }
            this.level = newLevel;
        }
        
        // Create new node
        let newNode = new SkipNode(value, newLevel);
        
        // Update forward pointers
        i = 0;
        while (i <= newLevel) {
            newNode.forward[i] = update[i].forward[i];
            update[i].forward[i] = newNode;
            i = i + 1;
        }
    }
    
    // Delete value
    func delete(value: int): bool {
        let update: [SkipNode] = [];
        let i = 0;
        while (i <= this.maxLevel) {
            update.push(null);
            i = i + 1;
        }
        
        let current = this.head;
        i = this.level;
        
        // Find node to delete
        while (i >= 0) {
            while (current.forward[i] != null && 
                   current.forward[i].value < value) {
                current = current.forward[i];
            }
            update[i] = current;
            i = i - 1;
        }
        
        current = current.forward[0];
        
        // Value not found
        if (current == null || current.value != value) {
            return false;
        }
        
        // Update forward pointers
        i = 0;
        while (i <= this.level) {
            if (update[i].forward[i] != current) {
                break;
            }
            update[i].forward[i] = current.forward[i];
            i = i + 1;
        }
        
        // Update list level
        while (this.level > 0 && this.head.forward[this.level] == null) {
            this.level = this.level - 1;
        }
        
        return true;
    }
    
    // Display skip list
    func display() {
        print("Skip List:");
        let i = this.level;
        while (i >= 0) {
            print("Level " + i + ": ");
            let node = this.head.forward[i];
            while (node != null) {
                print(node.value + " ");
                node = node.forward[i];
            }
            print("");
            i = i - 1;
        }
    }
}

// Usage example
let skipList = new SkipList(4); // Max level 4

skipList.insert(3);
skipList.insert(6);
skipList.insert(7);
skipList.insert(9);
skipList.insert(12);
skipList.insert(19);

print(skipList.search(7));  // true
print(skipList.search(10)); // false

skipList.delete(7);
print(skipList.search(7));  // false
```

## Interview Problems

### Problem 1: Range Sum Query - Mutable (Segment Tree)

Given an array, support sum queries and updates.

```klang
class NumArray {
    var segTree: SegmentTree;
    
    func new(nums: [int]) {
        this.segTree = new SegmentTree(nums);
    }
    
    func update(index: int, val: int) {
        this.segTree.update(index, val);
    }
    
    func sumRange(left: int, right: int): int {
        return this.segTree.query(left, right);
    }
}

// Usage
let nums = [1, 3, 5, 7, 9, 11];
let numArray = new NumArray(nums);

print(numArray.sumRange(1, 3));  // 15 (3 + 5 + 7)
numArray.update(1, 10);
print(numArray.sumRange(1, 3));  // 22 (10 + 5 + 7)
```

### Problem 2: Range Sum Query 2D - Mutable (Fenwick Tree 2D)

Support 2D range sum queries and updates.

```klang
class NumMatrix {
    var fenwick: FenwickTree2D;
    var matrix: [[int]];
    
    func new(matrix: [[int]]) {
        this.matrix = matrix;
        this.fenwick = new FenwickTree2D(matrix);
    }
    
    func update(row: int, col: int, val: int) {
        let oldVal = this.matrix[row][col];
        let delta = val - oldVal;
        this.matrix[row][col] = val;
        this.fenwick.update(row + 1, col + 1, delta);
    }
    
    func sumRegion(row1: int, col1: int, row2: int, col2: int): int {
        return this.fenwick.regionSum(row1 + 1, col1 + 1, 
                                     row2 + 1, col2 + 1);
    }
}
```

### Problem 3: Number of Islands II (Union-Find)

Given an m x n grid initially filled with water, add lands one by one and return the number of islands after each operation.

```klang
func numIslands2(m: int, n: int, positions: [[int]]): [int] {
    let result: [int] = [];
    let uf = new UnionFind(m * n);
    let grid: [bool] = [];
    
    // Initialize grid
    let i = 0;
    while (i < m * n) {
        grid.push(false);
        i = i + 1;
    }
    
    let count = 0;
    let directions = [[0, 1], [1, 0], [0, -1], [-1, 0]];
    
    i = 0;
    while (i < positions.length) {
        let row = positions[i][0];
        let col = positions[i][1];
        let idx = row * n + col;
        
        if (grid[idx]) {
            result.push(count);
            i = i + 1;
            continue;
        }
        
        grid[idx] = true;
        count = count + 1;
        
        // Check 4 neighbors
        let j = 0;
        while (j < 4) {
            let newRow = row + directions[j][0];
            let newCol = col + directions[j][1];
            
            if (newRow >= 0 && newRow < m && newCol >= 0 && newCol < n) {
                let newIdx = newRow * n + newCol;
                if (grid[newIdx]) {
                    if (uf.union(idx, newIdx)) {
                        count = count - 1;
                    }
                }
            }
            j = j + 1;
        }
        
        result.push(count);
        i = i + 1;
    }
    
    return result;
}

// Usage
let positions = [[0,0], [0,1], [1,2], [2,1]];
print(numIslands2(3, 3, positions)); // [1, 1, 2, 3]
```

### Problem 4: Redundant Connection (Union-Find)

Find an edge that can be removed to make the graph a tree.

```klang
func findRedundantConnection(edges: [[int]]): [int] {
    let n = edges.length;
    let uf = new UnionFind(n + 1); // Nodes are 1-indexed
    
    let i = 0;
    while (i < edges.length) {
        let u = edges[i][0];
        let v = edges[i][1];
        
        if (!uf.union(u, v)) {
            // u and v are already connected, this edge creates a cycle
            return edges[i];
        }
        
        i = i + 1;
    }
    
    return [];
}

// Usage
let edges = [[1,2], [1,3], [2,3]];
print(findRedundantConnection(edges)); // [2, 3]
```

### Problem 5: Count of Smaller Numbers After Self (Fenwick Tree)

Count how many numbers to the right are smaller than current number.

```klang
func countSmaller(nums: [int]): [int] {
    let result: [int] = [];
    let n = nums.length;
    
    // Find range of numbers and normalize to 1-based indexing
    let minVal = 2147483647;
    let maxVal = -2147483648;
    let i = 0;
    while (i < n) {
        if (nums[i] < minVal) {
            minVal = nums[i];
        }
        if (nums[i] > maxVal) {
            maxVal = nums[i];
        }
        i = i + 1;
    }
    
    let offset = -minVal + 1;
    let range = maxVal - minVal + 1;
    let fenwick = new FenwickTree([]);
    
    // Initialize fenwick with zeros
    i = 0;
    while (i < range) {
        fenwick.tree.push(0);
        i = i + 1;
    }
    fenwick.n = range;
    
    // Process from right to left
    i = n - 1;
    while (i >= 0) {
        let normalized = nums[i] + offset;
        let count = fenwick.prefixSum(normalized - 1);
        result.push(count);
        fenwick.update(normalized, 1);
        i = i - 1;
    }
    
    // Reverse result
    let reversed: [int] = [];
    i = result.length - 1;
    while (i >= 0) {
        reversed.push(result[i]);
        i = i - 1;
    }
    
    return reversed;
}

// Usage
let nums = [5, 2, 6, 1];
print(countSmaller(nums)); // [2, 1, 1, 0]
```

### Problem 6: Longest Repeated Substring (Suffix Array)

Find the longest substring that appears at least twice.

```klang
func longestRepeatedSubstring(s: string): string {
    let sa = new SuffixArray(s);
    return sa.longestRepeatedSubstring();
}

// Usage
print(longestRepeatedSubstring("banana")); // "ana"
print(longestRepeatedSubstring("abcd"));   // "" (no repetition)
```

### Problem 7: Friend Circles (Union-Find)

Count the number of friend circles in a friendship matrix.

```klang
func findCircleNum(isConnected: [[int]]): int {
    let n = isConnected.length;
    let uf = new UnionFind(n);
    
    let i = 0;
    while (i < n) {
        let j = i + 1;
        while (j < n) {
            if (isConnected[i][j] == 1) {
                uf.union(i, j);
            }
            j = j + 1;
        }
        i = i + 1;
    }
    
    return uf.getCount();
}

// Usage
let isConnected = [[1,1,0], [1,1,0], [0,0,1]];
print(findCircleNum(isConnected)); // 2 friend circles
```

### Problem 8: Accounts Merge (Union-Find)

Merge accounts that share common emails.

```klang
func accountsMerge(accounts: [[string]]): [[string]] {
    let emailToId: map<string, int> = {};
    let emailToName: map<string, string> = {};
    let emailId = 0;
    
    // Map emails to IDs
    let i = 0;
    while (i < accounts.length) {
        let name = accounts[i][0];
        let j = 1;
        while (j < accounts[i].length) {
            let email = accounts[i][j];
            if (!emailToId.has(email)) {
                emailToId[email] = emailId;
                emailToName[email] = name;
                emailId = emailId + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    
    let uf = new UnionFind(emailId);
    
    // Union emails in same account
    i = 0;
    while (i < accounts.length) {
        let firstEmailId = emailToId[accounts[i][1]];
        let j = 2;
        while (j < accounts[i].length) {
            let emailId = emailToId[accounts[i][j]];
            uf.union(firstEmailId, emailId);
            j = j + 1;
        }
        i = i + 1;
    }
    
    // Group emails by root
    let groups: map<int, [string]> = {};
    for (let email in emailToId) {
        let id = emailToId[email];
        let root = uf.find(id);
        if (!groups.has(root)) {
            groups[root] = [];
        }
        groups[root].push(email);
    }
    
    // Build result
    let result: [[string]] = [];
    for (let root in groups) {
        let emails = groups[root];
        // Sort emails (simplified)
        let name = emailToName[emails[0]];
        let account: [string] = [name];
        account = account.concat(emails);
        result.push(account);
    }
    
    return result;
}
```

### Problem 9: Range Minimum Query (Segment Tree)

Support range minimum queries on an array.

```klang
func rangeMinQuery(nums: [int], queries: [[int]]): [int] {
    let segTree = new SegmentTreeMin(nums);
    let result: [int] = [];
    
    let i = 0;
    while (i < queries.length) {
        let left = queries[i][0];
        let right = queries[i][1];
        result.push(segTree.query(left, right));
        i = i + 1;
    }
    
    return result;
}

// Usage
let nums = [1, 3, 2, 7, 5, 4];
let queries = [[0, 2], [1, 4], [3, 5]];
print(rangeMinQuery(nums, queries)); // [1, 2, 4]
```

### Problem 10: Kth Largest Element in Stream (Fenwick Tree)

Maintain kth largest element as elements are added.

```klang
class KthLargest {
    var k: int;
    var fenwick: FenwickTree;
    var offset: int;
    var range: int;
    
    func new(k: int, nums: [int]) {
        this.k = k;
        this.offset = 10000; // Assuming values in range [-10000, 10000]
        this.range = 20001;
        
        // Initialize fenwick
        let zeros: [int] = [];
        let i = 0;
        while (i < this.range) {
            zeros.push(0);
            i = i + 1;
        }
        this.fenwick = new FenwickTree(zeros);
        
        // Add initial elements
        i = 0;
        while (i < nums.length) {
            this.add(nums[i]);
            i = i + 1;
        }
    }
    
    func add(val: int): int {
        let normalized = val + this.offset;
        this.fenwick.update(normalized, 1);
        
        // Binary search for kth largest
        let left = 1;
        let right = this.range;
        
        while (left < right) {
            let mid = left + (right - left) / 2;
            let count = this.fenwick.prefixSum(this.range) - 
                       this.fenwick.prefixSum(mid);
            
            if (count < this.k) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        return left - this.offset;
    }
}
```

### Problem 11: Pattern Matching (Suffix Array)

Find all occurrences of a pattern in text.

```klang
func findAllOccurrences(text: string, pattern: string): [int] {
    let sa = new SuffixArray(text);
    return sa.search(pattern);
}

// Usage
let text = "banana";
let pattern = "ana";
print(findAllOccurrences(text, pattern)); // [1, 3]
```

### Problem 12: Checking Membership (Bloom Filter)

Efficiently check if elements might be in a large set.

```klang
func checkUrlVisited(urls: [string], checks: [string]): [string] {
    let bloom = new BloomFilter(10000, 5);
    
    // Add all URLs to bloom filter
    let i = 0;
    while (i < urls.length) {
        bloom.add(urls[i]);
        i = i + 1;
    }
    
    // Check which URLs might have been visited
    let possiblyVisited: [string] = [];
    i = 0;
    while (i < checks.length) {
        if (bloom.mightContain(checks[i])) {
            possiblyVisited.push(checks[i]);
        }
        i = i + 1;
    }
    
    return possiblyVisited;
}
```

## When to Use Each Structure

### Segment Tree
**Use when:**
- Need range queries (sum, min, max, GCD)
- Array elements can be updated
- Both queries and updates are frequent
- Need O(log n) query and update

**Avoid when:**
- Only prefix queries needed (use Fenwick tree)
- No updates needed (use prefix sum array)
- Memory is very limited

### Fenwick Tree
**Use when:**
- Need prefix sum queries
- Updates are frequent
- Space efficiency is important
- Simpler than segment tree

**Avoid when:**
- Need general range queries (min, max, GCD)
- Query patterns are not prefix-based
- Need range updates with lazy propagation

### Union-Find
**Use when:**
- Graph connectivity problems
- Detecting cycles
- Dynamic connectivity
- Grouping/clustering problems

**Avoid when:**
- Need to iterate through components
- Need to remove edges
- Graph is very dense

### Suffix Array
**Use when:**
- String pattern matching
- Finding repeated substrings
- LCP computations
- Space-efficient string indexing

**Avoid when:**
- Only single pattern search (use KMP)
- Need to modify string dynamically
- Very short strings

### Bloom Filter
**Use when:**
- Memory is limited
- False positives are acceptable
- Need fast membership testing
- Set is very large

**Avoid when:**
- Cannot tolerate false positives
- Need to remove elements
- Need exact counts

### Skip List
**Use when:**
- Need sorted data structure
- Simpler than balanced trees
- Concurrent access patterns
- Good average-case performance acceptable

**Avoid when:**
- Need worst-case guarantees
- Space is very limited
- Deterministic behavior required

## Applications

### Database Systems
- **Segment Trees**: Range queries on indexed columns
- **Fenwick Trees**: Cumulative statistics
- **Bloom Filters**: Cache filtering, query optimization
- **Suffix Arrays**: Full-text search

### Network Systems
- **Union-Find**: Network connectivity, routing tables
- **Bloom Filters**: Packet filtering, caching
- **Skip Lists**: Distributed systems, concurrent data structures

### Text Processing
- **Suffix Arrays**: Search engines, pattern matching
- **Bloom Filters**: Spell checkers, duplicate detection

### Competitive Programming
- **Segment Trees**: Range query problems
- **Fenwick Trees**: Inversion counting, dynamic programming
- **Union-Find**: Graph problems, MST algorithms

### Real-time Systems
- **Fenwick Trees**: Real-time statistics
- **Skip Lists**: Concurrent data access
- **Bloom Filters**: Quick filtering

## Best Practices

### Segment Trees
```klang
// 1. Use 4*n space to be safe
let tree: [int] = new Array(4 * n);

// 2. Use 0-indexed arrays
func build(node: int, start: int, end: int) { }

// 3. Check for no overlap first in queries
if (queryStart > end || queryEnd < start) {
    return defaultValue;
}

// 4. Use lazy propagation for range updates
// Defer updates until necessary
```

### Fenwick Trees
```klang
// 1. Use 1-based indexing
let tree: [int] = new Array(n + 1);

// 2. Use bit manipulation for parent/child
func lsb(x: int): int {
    return x & (-x);
}

// 3. Remember to convert range queries
func rangeSum(left: int, right: int): int {
    return prefixSum(right) - prefixSum(left - 1);
}
```

### Union-Find
```klang
// 1. Always use path compression
func find(x: int): int {
    if (parent[x] != x) {
        parent[x] = find(parent[x]); // Path compression
    }
    return parent[x];
}

// 2. Use union by rank or size
if (rank[rootX] < rank[rootY]) {
    parent[rootX] = rootY;
} else {
    parent[rootY] = rootX;
    if (rank[rootX] == rank[rootY]) {
        rank[rootX] = rank[rootX] + 1;
    }
}

// 3. Track component count
count = count - 1 after successful union
```

### Suffix Arrays
```klang
// 1. Use efficient construction algorithms
// O(n log^2 n) is sufficient for most cases
// O(n log n) with radix sort
// O(n) with DC3 or SA-IS

// 2. Build LCP array for advanced queries
// Many string problems need LCP

// 3. Use binary search for pattern matching
// Takes advantage of sorted property
```

### Bloom Filters
```klang
// 1. Choose parameters carefully
// m = -n*ln(p) / (ln(2))^2  (number of bits)
// k = (m/n) * ln(2)          (number of hash functions)
// where p is desired false positive rate

// 2. Use good hash functions
// MurmurHash, CityHash, or FNV

// 3. Never remove elements
// Use counting bloom filter if removal needed

// 4. Monitor false positive rate
// Rebuild if FPR becomes too high
```

### Skip Lists
```klang
// 1. Use appropriate max level
// maxLevel = log2(n) is good default

// 2. Set probability carefully
// p = 0.5 balances speed and space
// p = 0.25 uses less space

// 3. Use dummy head node
// Simplifies edge cases

// 4. Consider lock-free implementation
// Good for concurrent access
```

## Complexity Summary

| Structure | Build | Query | Update | Space |
|-----------|-------|-------|--------|-------|
| Segment Tree | O(n) | O(log n) | O(log n) | O(n) |
| Fenwick Tree | O(n log n) | O(log n) | O(log n) | O(n) |
| Union-Find | O(n) | O(α(n)) | O(α(n)) | O(n) |
| Suffix Array | O(n log n) | O(m log n) | N/A | O(n) |
| Bloom Filter | O(nk) | O(k) | O(k) | O(m) |
| Skip List | O(n log n) | O(log n) | O(log n) | O(n log n) |

Where:
- n = number of elements
- m = pattern length (suffix array) or filter size (bloom filter)
- k = number of hash functions (bloom filter)
- α(n) = inverse Ackermann function (practically constant)

## Conclusion

Advanced data structures are powerful tools for solving complex problems efficiently. Each structure has its strengths:

- **Segment Trees** excel at range queries with updates
- **Fenwick Trees** provide space-efficient prefix operations
- **Union-Find** efficiently manages disjoint sets
- **Suffix Arrays** enable fast string operations
- **Bloom Filters** offer probabilistic membership testing
- **Skip Lists** provide simple probabilistic balancing

Choose the right structure based on:
1. **Problem requirements**: What operations are needed?
2. **Performance constraints**: Time and space limits
3. **Implementation complexity**: Simpler is often better
4. **Special properties**: Probabilistic vs deterministic

Master these structures to tackle advanced algorithmic challenges and system design problems effectively!
