# Heaps in KLang

## Introduction

A **heap** is a specialized tree-based data structure that satisfies the heap property. Heaps are complete binary trees where every parent node satisfies a specific ordering relationship with its children. They are fundamental for implementing priority queues and efficient sorting algorithms.

### Complete Binary Tree

A complete binary tree is a binary tree where:
- All levels are completely filled except possibly the last level
- The last level is filled from left to right
- Height is always O(log n) for n nodes

### Key Characteristics

- **Efficient operations**: Insert and delete in O(log n) time
- **Fast access**: Get min/max element in O(1) time
- **Space efficient**: Array-based implementation with no extra pointers
- **Applications**: Priority queues, heap sort, graph algorithms

## Types of Heaps

### Min Heap

In a **min heap**, the parent node is always smaller than or equal to its children. The smallest element is at the root.

**Property**: For any node i, `parent[i] ≤ children[i]`

```klang
// Min Heap visualization
//       1
//      / \
//     3   5
//    / \ / \
//   7  8 9  10
//
// Array: [1, 3, 5, 7, 8, 9, 10]
```

### Max Heap

In a **max heap**, the parent node is always greater than or equal to its children. The largest element is at the root.

**Property**: For any node i, `parent[i] ≥ children[i]`

```klang
// Max Heap visualization
//       10
//      /  \
//     9    8
//    / \  / \
//   5  7 3   1
//
// Array: [10, 9, 8, 5, 7, 3, 1]
```

## Heap Properties

### 1. Structural Property
- Must be a complete binary tree
- All levels filled except possibly the last
- Last level filled from left to right

### 2. Heap Order Property
- **Min Heap**: Parent ≤ Children
- **Max Heap**: Parent ≥ Children
- No ordering between siblings

### 3. Height Property
- Height = O(log n)
- Path from root to any leaf ≤ ⌊log₂ n⌋

## Array Representation

Heaps are efficiently represented using arrays without storing explicit parent-child pointers.

### Index Relationships

For a node at index `i`:
- **Parent**: `(i - 1) / 2`
- **Left Child**: `2 * i + 1`
- **Right Child**: `2 * i + 2`

### Example

```klang
// Array: [1, 3, 5, 7, 8, 9, 10]
// Index:  0  1  2  3  4  5   6
//
// Node at index 1 (value 3):
//   - Parent: (1-1)/2 = 0 (value 1)
//   - Left child: 2*1+1 = 3 (value 7)
//   - Right child: 2*1+2 = 4 (value 8)
```

## Operations

### Insert (Bubble Up) - O(log n)

Add element at the end and bubble up to restore heap property.

```klang
class MinHeap {
    var heap: [int];
    var size: int;
    
    func new() {
        this.heap = [];
        this.size = 0;
    }
    
    func insert(value: int) {
        // Add to end
        this.heap.push(value);
        this.size = this.size + 1;
        
        // Bubble up
        this.bubbleUp(this.size - 1);
    }
    
    func bubbleUp(index: int) {
        while (index > 0) {
            let parentIndex = (index - 1) / 2;
            
            if (this.heap[index] >= this.heap[parentIndex]) {
                break;
            }
            
            // Swap with parent
            this.swap(index, parentIndex);
            index = parentIndex;
        }
    }
    
    func swap(i: int, j: int) {
        let temp = this.heap[i];
        this.heap[i] = this.heap[j];
        this.heap[j] = temp;
    }
}
```

### Extract Min/Max (Bubble Down) - O(log n)

Remove root element and bubble down last element to restore heap property.

```klang
func extractMin(): int {
    if (this.size == 0) {
        return -1; // or throw error
    }
    
    let min = this.heap[0];
    
    // Move last element to root
    this.heap[0] = this.heap[this.size - 1];
    this.heap.pop();
    this.size = this.size - 1;
    
    // Bubble down
    if (this.size > 0) {
        this.bubbleDown(0);
    }
    
    return min;
}

func bubbleDown(index: int) {
    while (true) {
        let smallest = index;
        let leftChild = 2 * index + 1;
        let rightChild = 2 * index + 2;
        
        // Compare with left child
        if (leftChild < this.size && 
            this.heap[leftChild] < this.heap[smallest]) {
            smallest = leftChild;
        }
        
        // Compare with right child
        if (rightChild < this.size && 
            this.heap[rightChild] < this.heap[smallest]) {
            smallest = rightChild;
        }
        
        // If smallest is still index, we're done
        if (smallest == index) {
            break;
        }
        
        // Swap and continue
        this.swap(index, smallest);
        index = smallest;
    }
}
```

### Peek - O(1)

Get the minimum (or maximum) element without removing it.

```klang
func peek(): int {
    if (this.size == 0) {
        return -1; // or throw error
    }
    return this.heap[0];
}

func isEmpty(): bool {
    return this.size == 0;
}

func getSize(): int {
    return this.size;
}
```

### Heapify - O(n)

Convert an arbitrary array into a heap.

```klang
func heapify(arr: [int]): MinHeap {
    let heap = new MinHeap();
    heap.heap = arr;
    heap.size = arr.length;
    
    // Start from last non-leaf node and bubble down
    let startIdx = (heap.size / 2) - 1;
    let i = startIdx;
    
    while (i >= 0) {
        heap.bubbleDown(i);
        i = i - 1;
    }
    
    return heap;
}
```

## Max Heap Implementation

```klang
class MaxHeap {
    var heap: [int];
    var size: int;
    
    func new() {
        this.heap = [];
        this.size = 0;
    }
    
    func insert(value: int) {
        this.heap.push(value);
        this.size = this.size + 1;
        this.bubbleUp(this.size - 1);
    }
    
    func bubbleUp(index: int) {
        while (index > 0) {
            let parentIndex = (index - 1) / 2;
            
            if (this.heap[index] <= this.heap[parentIndex]) {
                break;
            }
            
            this.swap(index, parentIndex);
            index = parentIndex;
        }
    }
    
    func extractMax(): int {
        if (this.size == 0) {
            return -1;
        }
        
        let max = this.heap[0];
        this.heap[0] = this.heap[this.size - 1];
        this.heap.pop();
        this.size = this.size - 1;
        
        if (this.size > 0) {
            this.bubbleDown(0);
        }
        
        return max;
    }
    
    func bubbleDown(index: int) {
        while (true) {
            let largest = index;
            let leftChild = 2 * index + 1;
            let rightChild = 2 * index + 2;
            
            if (leftChild < this.size && 
                this.heap[leftChild] > this.heap[largest]) {
                largest = leftChild;
            }
            
            if (rightChild < this.size && 
                this.heap[rightChild] > this.heap[largest]) {
                largest = rightChild;
            }
            
            if (largest == index) {
                break;
            }
            
            this.swap(index, largest);
            index = largest;
        }
    }
    
    func peek(): int {
        if (this.size == 0) {
            return -1;
        }
        return this.heap[0];
    }
    
    func swap(i: int, j: int) {
        let temp = this.heap[i];
        this.heap[i] = this.heap[j];
        this.heap[j] = temp;
    }
}
```

## Heap Sort - O(n log n)

Heap sort uses a heap to sort an array in O(n log n) time.

```klang
func heapSort(arr: [int]): [int] {
    let n = arr.length;
    
    // Build max heap
    let i = (n / 2) - 1;
    while (i >= 0) {
        heapifyDown(arr, n, i);
        i = i - 1;
    }
    
    // Extract elements one by one
    i = n - 1;
    while (i > 0) {
        // Move current root to end
        let temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        
        // Heapify reduced heap
        heapifyDown(arr, i, 0);
        i = i - 1;
    }
    
    return arr;
}

func heapifyDown(arr: [int], n: int, index: int) {
    let largest = index;
    let left = 2 * index + 1;
    let right = 2 * index + 2;
    
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }
    
    if (largest != index) {
        let temp = arr[index];
        arr[index] = arr[largest];
        arr[largest] = temp;
        
        heapifyDown(arr, n, largest);
    }
}

// Usage
let arr = [12, 11, 13, 5, 6, 7];
let sorted = heapSort(arr);
// sorted = [5, 6, 7, 11, 12, 13]
```

## Priority Queue Implementation

A priority queue is an abstract data type where each element has a priority. Elements with higher priority are dequeued first.

```klang
class PriorityQueue {
    var minHeap: MinHeap;
    
    func new() {
        this.minHeap = new MinHeap();
    }
    
    func enqueue(value: int) {
        this.minHeap.insert(value);
    }
    
    func dequeue(): int {
        return this.minHeap.extractMin();
    }
    
    func peek(): int {
        return this.minHeap.peek();
    }
    
    func isEmpty(): bool {
        return this.minHeap.isEmpty();
    }
    
    func size(): int {
        return this.minHeap.getSize();
    }
}

// Usage
let pq = new PriorityQueue();
pq.enqueue(5);
pq.enqueue(2);
pq.enqueue(8);
pq.enqueue(1);

print(pq.dequeue()); // 1
print(pq.dequeue()); // 2
print(pq.peek());    // 5
```

## Heap Algorithms

### 1. Kth Largest Element

Find the kth largest element in an array using a min heap.

```klang
func findKthLargest(nums: [int], k: int): int {
    let minHeap = new MinHeap();
    
    // Keep only k largest elements in heap
    let i = 0;
    while (i < nums.length) {
        minHeap.insert(nums[i]);
        
        if (minHeap.getSize() > k) {
            minHeap.extractMin();
        }
        
        i = i + 1;
    }
    
    return minHeap.peek();
}

// Usage
let nums = [3, 2, 1, 5, 6, 4];
let k = 2;
print(findKthLargest(nums, k)); // 5 (2nd largest)
```

### 2. Merge K Sorted Arrays

Merge k sorted arrays efficiently using a min heap.

```klang
class HeapNode {
    var value: int;
    var arrayIndex: int;
    var elementIndex: int;
    
    func new(val: int, arrIdx: int, elemIdx: int) {
        this.value = val;
        this.arrayIndex = arrIdx;
        this.elementIndex = elemIdx;
    }
}

func mergeKSortedArrays(arrays: [[int]]): [int] {
    let result: [int] = [];
    let minHeap = new MinHeap(); // Assume modified for HeapNode
    
    // Insert first element from each array
    let i = 0;
    while (i < arrays.length) {
        if (arrays[i].length > 0) {
            let node = new HeapNode(arrays[i][0], i, 0);
            minHeap.insert(node);
        }
        i = i + 1;
    }
    
    // Extract min and add next element from same array
    while (!minHeap.isEmpty()) {
        let node = minHeap.extractMin();
        result.push(node.value);
        
        let nextIndex = node.elementIndex + 1;
        if (nextIndex < arrays[node.arrayIndex].length) {
            let nextNode = new HeapNode(
                arrays[node.arrayIndex][nextIndex],
                node.arrayIndex,
                nextIndex
            );
            minHeap.insert(nextNode);
        }
    }
    
    return result;
}

// Usage
let arrays = [[1, 4, 7], [2, 5, 8], [3, 6, 9]];
let merged = mergeKSortedArrays(arrays);
// merged = [1, 2, 3, 4, 5, 6, 7, 8, 9]
```

### 3. Running Median

Find the median of a stream of numbers efficiently.

```klang
class MedianFinder {
    var maxHeap: MaxHeap; // Lower half
    var minHeap: MinHeap; // Upper half
    
    func new() {
        this.maxHeap = new MaxHeap();
        this.minHeap = new MinHeap();
    }
    
    func addNum(num: int) {
        // Add to max heap first
        if (this.maxHeap.isEmpty() || num <= this.maxHeap.peek()) {
            this.maxHeap.insert(num);
        } else {
            this.minHeap.insert(num);
        }
        
        // Balance heaps
        if (this.maxHeap.getSize() > this.minHeap.getSize() + 1) {
            this.minHeap.insert(this.maxHeap.extractMax());
        } else if (this.minHeap.getSize() > this.maxHeap.getSize()) {
            this.maxHeap.insert(this.minHeap.extractMin());
        }
    }
    
    func findMedian(): float {
        if (this.maxHeap.getSize() == this.minHeap.getSize()) {
            return (this.maxHeap.peek() + this.minHeap.peek()) / 2.0;
        } else {
            return this.maxHeap.peek();
        }
    }
}

// Usage
let mf = new MedianFinder();
mf.addNum(1);
mf.addNum(2);
print(mf.findMedian()); // 1.5
mf.addNum(3);
print(mf.findMedian()); // 2.0
```

## Interview Problems

### Problem 1: Kth Largest Element in Array

**Description**: Find the kth largest element in an unsorted array.

**Solution**:
```klang
func findKthLargest(nums: [int], k: int): int {
    let minHeap = new MinHeap();
    
    let i = 0;
    while (i < nums.length) {
        if (minHeap.getSize() < k) {
            minHeap.insert(nums[i]);
        } else if (nums[i] > minHeap.peek()) {
            minHeap.extractMin();
            minHeap.insert(nums[i]);
        }
        i = i + 1;
    }
    
    return minHeap.peek();
}

// Time: O(n log k), Space: O(k)
```

### Problem 2: Top K Frequent Elements

**Description**: Given an array, return the k most frequent elements.

**Solution**:
```klang
class FreqNode {
    var value: int;
    var freq: int;
    
    func new(val: int, f: int) {
        this.value = val;
        this.freq = f;
    }
}

func topKFrequent(nums: [int], k: int): [int] {
    // Count frequencies
    let freqMap: map<int, int> = {};
    let i = 0;
    while (i < nums.length) {
        if (freqMap.has(nums[i])) {
            freqMap[nums[i]] = freqMap[nums[i]] + 1;
        } else {
            freqMap[nums[i]] = 1;
        }
        i = i + 1;
    }
    
    // Use min heap to keep k most frequent
    let minHeap = new MinHeap(); // Modified for FreqNode
    
    for (let key in freqMap) {
        let node = new FreqNode(key, freqMap[key]);
        minHeap.insert(node);
        
        if (minHeap.getSize() > k) {
            minHeap.extractMin();
        }
    }
    
    // Extract results
    let result: [int] = [];
    while (!minHeap.isEmpty()) {
        let node = minHeap.extractMin();
        result.push(node.value);
    }
    
    return result;
}

// Time: O(n log k), Space: O(n)
```

### Problem 3: Merge K Sorted Lists

**Description**: Merge k sorted linked lists into one sorted list.

**Solution**:
```klang
class ListNode {
    var val: int;
    var next: ListNode;
    
    func new(value: int) {
        this.val = value;
        this.next = null;
    }
}

func mergeKLists(lists: [ListNode]): ListNode {
    let minHeap = new MinHeap(); // Modified for ListNode
    
    // Add first node from each list
    let i = 0;
    while (i < lists.length) {
        if (lists[i] != null) {
            minHeap.insert(lists[i]);
        }
        i = i + 1;
    }
    
    let dummy = new ListNode(0);
    let current = dummy;
    
    while (!minHeap.isEmpty()) {
        let node = minHeap.extractMin();
        current.next = node;
        current = current.next;
        
        if (node.next != null) {
            minHeap.insert(node.next);
        }
    }
    
    return dummy.next;
}

// Time: O(n log k), Space: O(k)
// where n is total nodes, k is number of lists
```

### Problem 4: Find Median from Data Stream

**Description**: Design a data structure that supports adding numbers and finding the median.

**Solution**: (See Running Median implementation above)

```klang
// Time: addNum O(log n), findMedian O(1)
// Space: O(n)
```

### Problem 5: Kth Smallest Element in Sorted Matrix

**Description**: Find kth smallest element in a row and column sorted matrix.

**Solution**:
```klang
func kthSmallest(matrix: [[int]], k: int): int {
    let n = matrix.length;
    let minHeap = new MinHeap(); // Modified for matrix nodes
    
    // Add first element from each row
    let i = 0;
    while (i < n && i < k) {
        let node = new HeapNode(matrix[i][0], i, 0);
        minHeap.insert(node);
        i = i + 1;
    }
    
    let count = 0;
    let result = 0;
    
    while (count < k) {
        let node = minHeap.extractMin();
        result = node.value;
        count = count + 1;
        
        if (node.elementIndex + 1 < n) {
            let nextNode = new HeapNode(
                matrix[node.arrayIndex][node.elementIndex + 1],
                node.arrayIndex,
                node.elementIndex + 1
            );
            minHeap.insert(nextNode);
        }
    }
    
    return result;
}

// Time: O(k log n), Space: O(n)
```

### Problem 6: Last Stone Weight

**Description**: Smash two heaviest stones together. If weights are x and y (x ≤ y), result is y-x. Return weight of last stone.

**Solution**:
```klang
func lastStoneWeight(stones: [int]): int {
    let maxHeap = new MaxHeap();
    
    // Add all stones to heap
    let i = 0;
    while (i < stones.length) {
        maxHeap.insert(stones[i]);
        i = i + 1;
    }
    
    // Smash stones
    while (maxHeap.getSize() > 1) {
        let stone1 = maxHeap.extractMax();
        let stone2 = maxHeap.extractMax();
        
        if (stone1 != stone2) {
            maxHeap.insert(stone1 - stone2);
        }
    }
    
    if (maxHeap.isEmpty()) {
        return 0;
    }
    return maxHeap.peek();
}

// Time: O(n log n), Space: O(n)
```

### Problem 7: K Closest Points to Origin

**Description**: Find k closest points to origin (0, 0).

**Solution**:
```klang
class Point {
    var x: int;
    var y: int;
    var distance: int;
    
    func new(px: int, py: int) {
        this.x = px;
        this.y = py;
        this.distance = px * px + py * py;
    }
}

func kClosest(points: [[int]], k: int): [[int]] {
    let maxHeap = new MaxHeap(); // Modified for Point
    
    let i = 0;
    while (i < points.length) {
        let point = new Point(points[i][0], points[i][1]);
        
        if (maxHeap.getSize() < k) {
            maxHeap.insert(point);
        } else if (point.distance < maxHeap.peek().distance) {
            maxHeap.extractMax();
            maxHeap.insert(point);
        }
        
        i = i + 1;
    }
    
    let result: [[int]] = [];
    while (!maxHeap.isEmpty()) {
        let point = maxHeap.extractMax();
        result.push([point.x, point.y]);
    }
    
    return result;
}

// Time: O(n log k), Space: O(k)
```

### Problem 8: Reorganize String

**Description**: Reorganize string so no two adjacent characters are the same.

**Solution**:
```klang
func reorganizeString(s: string): string {
    // Count character frequencies
    let freqMap: map<char, int> = {};
    let i = 0;
    while (i < s.length) {
        let ch = s[i];
        if (freqMap.has(ch)) {
            freqMap[ch] = freqMap[ch] + 1;
        } else {
            freqMap[ch] = 1;
        }
        i = i + 1;
    }
    
    // Build max heap based on frequency
    let maxHeap = new MaxHeap(); // Modified for char/freq pairs
    for (let ch in freqMap) {
        maxHeap.insert({char: ch, freq: freqMap[ch]});
    }
    
    let result = "";
    let prev = null;
    
    while (!maxHeap.isEmpty()) {
        let current = maxHeap.extractMax();
        result = result + current.char;
        current.freq = current.freq - 1;
        
        // Add previous back to heap
        if (prev != null && prev.freq > 0) {
            maxHeap.insert(prev);
        }
        
        prev = current;
    }
    
    // Check if reorganization was successful
    if (result.length != s.length) {
        return "";
    }
    
    return result;
}

// Time: O(n log k), Space: O(k)
// where k is number of unique characters
```

### Problem 9: Task Scheduler

**Description**: Schedule tasks with cooling period. Same task must wait n intervals.

**Solution**:
```klang
func leastInterval(tasks: [char], n: int): int {
    // Count task frequencies
    let freqMap: map<char, int> = {};
    let i = 0;
    while (i < tasks.length) {
        if (freqMap.has(tasks[i])) {
            freqMap[tasks[i]] = freqMap[tasks[i]] + 1;
        } else {
            freqMap[tasks[i]] = 1;
        }
        i = i + 1;
    }
    
    // Build max heap
    let maxHeap = new MaxHeap();
    for (let task in freqMap) {
        maxHeap.insert(freqMap[task]);
    }
    
    let time = 0;
    
    while (!maxHeap.isEmpty()) {
        let temp: [int] = [];
        let k = 0;
        
        // Execute tasks in cooling cycle
        while (k <= n) {
            if (!maxHeap.isEmpty()) {
                let freq = maxHeap.extractMax();
                if (freq > 1) {
                    temp.push(freq - 1);
                }
            }
            
            time = time + 1;
            
            if (maxHeap.isEmpty() && temp.length == 0) {
                break;
            }
            
            k = k + 1;
        }
        
        // Add back to heap
        i = 0;
        while (i < temp.length) {
            maxHeap.insert(temp[i]);
            i = i + 1;
        }
    }
    
    return time;
}

// Time: O(n), Space: O(1) - at most 26 unique tasks
```

### Problem 10: Ugly Number II

**Description**: Find the nth ugly number (positive numbers whose prime factors are only 2, 3, or 5).

**Solution**:
```klang
func nthUglyNumber(n: int): int {
    let minHeap = new MinHeap();
    let seen: map<int, bool> = {};
    
    minHeap.insert(1);
    seen[1] = true;
    
    let primes = [2, 3, 5];
    let ugly = 1;
    
    let i = 0;
    while (i < n) {
        ugly = minHeap.extractMin();
        
        // Generate new ugly numbers
        let j = 0;
        while (j < 3) {
            let newUgly = ugly * primes[j];
            if (!seen.has(newUgly)) {
                minHeap.insert(newUgly);
                seen[newUgly] = true;
            }
            j = j + 1;
        }
        
        i = i + 1;
    }
    
    return ugly;
}

// Time: O(n log n), Space: O(n)
```

## Applications

### 1. Task Scheduling

Heaps are ideal for scheduling tasks by priority or deadline.

```klang
class Task {
    var id: int;
    var priority: int;
    var description: string;
    
    func new(taskId: int, prio: int, desc: string) {
        this.id = taskId;
        this.priority = prio;
        this.description = desc;
    }
}

class TaskScheduler {
    var taskQueue: MaxHeap; // Higher priority first
    
    func new() {
        this.taskQueue = new MaxHeap();
    }
    
    func addTask(task: Task) {
        this.taskQueue.insert(task);
    }
    
    func getNextTask(): Task {
        return this.taskQueue.extractMax();
    }
}
```

### 2. Dijkstra's Shortest Path

Priority queue (min heap) for efficient shortest path finding.

```klang
func dijkstra(graph: [[int]], start: int): [int] {
    let n = graph.length;
    let distances: [int] = [];
    let visited: [bool] = [];
    
    // Initialize
    let i = 0;
    while (i < n) {
        distances.push(999999); // infinity
        visited.push(false);
        i = i + 1;
    }
    distances[start] = 0;
    
    // Min heap: (distance, node)
    let pq = new MinHeap();
    pq.insert({dist: 0, node: start});
    
    while (!pq.isEmpty()) {
        let current = pq.extractMin();
        let u = current.node;
        
        if (visited[u]) {
            continue;
        }
        visited[u] = true;
        
        // Update neighbors
        i = 0;
        while (i < n) {
            if (graph[u][i] > 0 && !visited[i]) {
                let newDist = distances[u] + graph[u][i];
                if (newDist < distances[i]) {
                    distances[i] = newDist;
                    pq.insert({dist: newDist, node: i});
                }
            }
            i = i + 1;
        }
    }
    
    return distances;
}
```

### 3. Huffman Coding

Build optimal prefix codes for data compression.

```klang
class HuffmanNode {
    var freq: int;
    var char: char;
    var left: HuffmanNode;
    var right: HuffmanNode;
    
    func new(f: int, c: char) {
        this.freq = f;
        this.char = c;
        this.left = null;
        this.right = null;
    }
}

func buildHuffmanTree(freqMap: map<char, int>): HuffmanNode {
    let minHeap = new MinHeap(); // Compare by frequency
    
    // Create leaf nodes
    for (let ch in freqMap) {
        let node = new HuffmanNode(freqMap[ch], ch);
        minHeap.insert(node);
    }
    
    // Build tree
    while (minHeap.getSize() > 1) {
        let left = minHeap.extractMin();
        let right = minHeap.extractMin();
        
        let parent = new HuffmanNode(
            left.freq + right.freq,
            '\0'
        );
        parent.left = left;
        parent.right = right;
        
        minHeap.insert(parent);
    }
    
    return minHeap.extractMin();
}
```

### 4. Event-Driven Simulation

Process events in chronological order.

```klang
class Event {
    var time: int;
    var type: string;
    var data: any;
    
    func new(t: int, tp: string, d: any) {
        this.time = t;
        this.type = tp;
        this.data = d;
    }
}

class EventSimulator {
    var eventQueue: MinHeap; // Order by time
    var currentTime: int;
    
    func new() {
        this.eventQueue = new MinHeap();
        this.currentTime = 0;
    }
    
    func scheduleEvent(event: Event) {
        this.eventQueue.insert(event);
    }
    
    func run() {
        while (!this.eventQueue.isEmpty()) {
            let event = this.eventQueue.extractMin();
            this.currentTime = event.time;
            this.processEvent(event);
        }
    }
    
    func processEvent(event: Event) {
        print("Processing " + event.type + " at time " + event.time);
        // Handle event logic
    }
}
```

## Best Practices

### 1. Choose the Right Heap Type

```klang
// Use min heap for: smallest element, shortest path, earliest time
let minHeap = new MinHeap();

// Use max heap for: largest element, highest priority
let maxHeap = new MaxHeap();
```

### 2. Consider Space-Time Tradeoffs

```klang
// For top K elements: Use heap of size K
// Time: O(n log k), Space: O(k)
func topK(nums: [int], k: int): [int] {
    let heap = new MinHeap();
    // Keep only k elements in heap
}

// For heap sort: Use in-place heapify
// Time: O(n log n), Space: O(1)
func heapSort(arr: [int]): [int] {
    // Heapify in-place
}
```

### 3. Handle Edge Cases

```klang
func safeExtractMin(heap: MinHeap): int {
    if (heap.isEmpty()) {
        return -1; // Or throw error
    }
    return heap.extractMin();
}

func safePeek(heap: MinHeap): int {
    if (heap.isEmpty()) {
        return -1; // Or throw error
    }
    return heap.peek();
}
```

### 4. Use Heapify for Bulk Operations

```klang
// Building heap from array
// O(n) heapify is faster than n insertions O(n log n)
let arr = [5, 3, 7, 1, 9, 2];
let heap = heapify(arr); // O(n)

// Instead of:
let heap2 = new MinHeap();
let i = 0;
while (i < arr.length) {
    heap2.insert(arr[i]); // O(n log n) total
    i = i + 1;
}
```

### 5. Consider Alternative Structures

```klang
// For fixed small K: Use sorted array
// Better cache locality, simpler code
func topKSmall(nums: [int], k: int): [int] {
    if (k < 10) {
        // Use sorted array
    } else {
        // Use heap
    }
}

// For mostly sorted data: Consider other structures
// Heap doesn't maintain sorted order beyond root
```

### 6. Optimize Comparisons

```klang
// Use custom comparison for complex objects
class Task {
    var priority: int;
    var timestamp: int;
    
    func compare(other: Task): int {
        if (this.priority != other.priority) {
            return this.priority - other.priority;
        }
        return this.timestamp - other.timestamp;
    }
}
```

### 7. Lazy Deletion

```klang
// Mark as deleted instead of removing
class LazyHeap {
    var heap: MinHeap;
    var deleted: map<int, bool>;
    
    func extractMin(): int {
        while (!this.heap.isEmpty()) {
            let val = this.heap.extractMin();
            if (!this.deleted.has(val)) {
                return val;
            }
        }
        return -1;
    }
    
    func delete(val: int) {
        this.deleted[val] = true;
    }
}
```

### 8. Memory Management

```klang
// Preallocate for known size
class PreallocatedHeap {
    var heap: [int];
    var capacity: int;
    var size: int;
    
    func new(cap: int) {
        this.capacity = cap;
        this.size = 0;
        this.heap = [];
        
        // Preallocate
        let i = 0;
        while (i < cap) {
            this.heap.push(0);
            i = i + 1;
        }
    }
}
```

## Complexity Summary

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Insert | O(log n) | O(1) |
| Extract Min/Max | O(log n) | O(1) |
| Peek | O(1) | O(1) |
| Heapify | O(n) | O(1) |
| Delete | O(log n) | O(1) |
| Search | O(n) | O(1) |
| Heap Sort | O(n log n) | O(1) |

## Key Takeaways

1. **Efficient Priority Access**: O(1) access to min/max element
2. **Balanced Operations**: O(log n) insert and delete operations
3. **Space Efficient**: Array-based with no pointer overhead
4. **Versatile**: Priority queues, sorting, scheduling
5. **Heapify**: O(n) construction from array
6. **Not Sorted**: Only root is guaranteed min/max
7. **Cache Friendly**: Array representation improves locality
8. **Applications**: Dijkstra, Huffman coding, task scheduling

---

**Next Steps**: Practice implementing heaps and solving problems on LeetCode. Experiment with different heap variants (d-ary heaps, Fibonacci heaps) for specialized use cases.
