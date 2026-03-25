# Queues in KLang - Data Structures & Algorithms

## Table of Contents
1. [Introduction](#introduction)
2. [What is a Queue?](#what-is-a-queue)
3. [Queue Basics](#queue-basics)
4. [Types of Queues](#types-of-queues)
5. [Implementation Methods](#implementation-methods)
6. [Queue Operations](#queue-operations)
7. [Time and Space Complexity](#time-and-space-complexity)
8. [Queue Algorithms](#queue-algorithms)
9. [Applications](#applications)
10. [Interview Problems](#interview-problems)
11. [Best Practices](#best-practices)
12. [Complete Implementations](#complete-implementations)

---

## Introduction

A **Queue** is a linear data structure that follows the First-In-First-Out (FIFO) principle. Elements are added at the rear (enqueue) and removed from the front (dequeue), just like a real-world queue or line.

### Why Use Queues?

- **Order Processing**: Maintains order of elements
- **Task Scheduling**: CPU scheduling, print spooling
- **BFS Traversal**: Level-order tree/graph traversal
- **Resource Sharing**: Printer queues, IO buffers
- **Message Queues**: Asynchronous communication

---

## What is a Queue?

A **queue** is a collection where elements are added at one end (rear) and removed from the other end (front), maintaining FIFO order.

### Characteristics

- **FIFO Order**: First In, First Out
- **Two Ends**: Front (removal) and Rear (insertion)
- **Sequential**: Elements processed in order
- **Bounded/Unbounded**: Fixed or dynamic size

### Visual Representation

```
Queue Operations:
    Enqueue (rear)                    Dequeue (front)
         ↓                                   ↓
    [5] [4] [3] [2] [1] →
    rear              front

After Enqueue(6):  [6] [5] [4] [3] [2] [1]
After Dequeue():   [6] [5] [4] [3] [2]
```

---

## Queue Basics

### Core Operations

1. **Enqueue**: Add element to rear
2. **Dequeue**: Remove element from front
3. **Front/Peek**: View front element without removal
4. **Rear**: View rear element
5. **isEmpty**: Check if queue is empty
6. **Size**: Get number of elements

### Basic Queue Interface

```klang
class Queue {
    enqueue(value)  # Add to rear
    dequeue()       # Remove from front
    front()         # Get front element
    rear()          # Get rear element
    isEmpty()       # Check if empty
    size()          # Get size
}
```

---

## Types of Queues

### 1. Simple Queue (Linear Queue)

Basic FIFO queue with front and rear pointers.

```klang
# Simple Queue Example
class SimpleQueue {
    func init() {
        this.items = []
    }
    
    func enqueue(value) {
        this.items.push(value)
    }
    
    func dequeue() {
        if (this.isEmpty()) {
            return null
        }
        return this.items.shift()
    }
    
    func isEmpty() {
        return this.items.length == 0
    }
}

# Usage
let queue = SimpleQueue()
queue.enqueue(1)
queue.enqueue(2)
queue.enqueue(3)
print(queue.dequeue())  # Output: 1
```

### 2. Circular Queue

Queue with fixed size where rear wraps around to the beginning.

```klang
class CircularQueue {
    func init(capacity) {
        this.items = []
        this.capacity = capacity
        this.front = 0
        this.rear = -1
        this.count = 0
        
        let i = 0
        while (i < capacity) {
            this.items.push(null)
            i = i + 1
        }
    }
    
    func enqueue(value) {
        if (this.isFull()) {
            print("Queue is full")
            return false
        }
        
        this.rear = (this.rear + 1) % this.capacity
        this.items[this.rear] = value
        this.count = this.count + 1
        return true
    }
    
    func dequeue() {
        if (this.isEmpty()) {
            return null
        }
        
        let value = this.items[this.front]
        this.items[this.front] = null
        this.front = (this.front + 1) % this.capacity
        this.count = this.count - 1
        return value
    }
    
    func isFull() {
        return this.count == this.capacity
    }
    
    func isEmpty() {
        return this.count == 0
    }
    
    func size() {
        return this.count
    }
}

# Usage
let cq = CircularQueue(3)
cq.enqueue(1)
cq.enqueue(2)
cq.enqueue(3)
print(cq.dequeue())  # 1
cq.enqueue(4)        # Wraps around
```

### 3. Priority Queue

Elements dequeued based on priority, not insertion order.

```klang
class PriorityQueue {
    func init() {
        this.items = []
    }
    
    func enqueue(value, priority) {
        let item = {value: value, priority: priority}
        
        if (this.isEmpty()) {
            this.items.push(item)
            return
        }
        
        # Insert at correct position based on priority
        let i = 0
        let added = false
        while (i < this.items.length) {
            if (item.priority < this.items[i].priority) {
                this.items.splice(i, 0, item)
                added = true
                break
            }
            i = i + 1
        }
        
        if (!added) {
            this.items.push(item)
        }
    }
    
    func dequeue() {
        if (this.isEmpty()) {
            return null
        }
        return this.items.shift().value
    }
    
    func isEmpty() {
        return this.items.length == 0
    }
}

# Usage
let pq = PriorityQueue()
pq.enqueue("Task 1", 3)
pq.enqueue("Task 2", 1)  # Higher priority (lower number)
pq.enqueue("Task 3", 2)
print(pq.dequeue())  # Output: "Task 2" (priority 1)
```

### 4. Deque (Double-Ended Queue)

Queue allowing insertion and deletion at both ends.

```klang
class Deque {
    func init() {
        this.items = []
    }
    
    func enqueueFront(value) {
        this.items.unshift(value)
    }
    
    func enqueueRear(value) {
        this.items.push(value)
    }
    
    func dequeueFront() {
        if (this.isEmpty()) {
            return null
        }
        return this.items.shift()
    }
    
    func dequeueRear() {
        if (this.isEmpty()) {
            return null
        }
        return this.items.pop()
    }
    
    func peekFront() {
        if (this.isEmpty()) {
            return null
        }
        return this.items[0]
    }
    
    func peekRear() {
        if (this.isEmpty()) {
            return null
        }
        return this.items[this.items.length - 1]
    }
    
    func isEmpty() {
        return this.items.length == 0
    }
    
    func size() {
        return this.items.length
    }
}

# Usage
let deque = Deque()
deque.enqueueRear(1)
deque.enqueueFront(2)
deque.enqueueRear(3)
# Deque: [2, 1, 3]
print(deque.dequeueFront())  # 2
print(deque.dequeueRear())   # 3
```

---

## Implementation Methods

### 1. Array-Based Queue

Simple implementation using dynamic array with shift operation.

```klang
class ArrayQueue {
    func init() {
        this.items = []
    }
    
    func enqueue(value) {
        this.items.push(value)
    }
    
    func dequeue() {
        if (this.isEmpty()) {
            return null
        }
        return this.items.shift()  # O(n) operation
    }
    
    func front() {
        if (this.isEmpty()) {
            return null
        }
        return this.items[0]
    }
    
    func rear() {
        if (this.isEmpty()) {
            return null
        }
        return this.items[this.items.length - 1]
    }
    
    func isEmpty() {
        return this.items.length == 0
    }
    
    func size() {
        return this.items.length
    }
}
```

### 2. Linked List-Based Queue

Efficient O(1) enqueue and dequeue operations.

```klang
class Node {
    func init(value) {
        this.value = value
        this.next = null
    }
}

class LinkedListQueue {
    func init() {
        this.frontNode = null
        this.rearNode = null
        this.count = 0
    }
    
    func enqueue(value) {
        let newNode = Node(value)
        
        if (this.isEmpty()) {
            this.frontNode = newNode
            this.rearNode = newNode
        } else {
            this.rearNode.next = newNode
            this.rearNode = newNode
        }
        this.count = this.count + 1
    }
    
    func dequeue() {
        if (this.isEmpty()) {
            return null
        }
        
        let value = this.frontNode.value
        this.frontNode = this.frontNode.next
        
        if (this.frontNode == null) {
            this.rearNode = null
        }
        
        this.count = this.count - 1
        return value
    }
    
    func front() {
        if (this.isEmpty()) {
            return null
        }
        return this.frontNode.value
    }
    
    func rear() {
        if (this.isEmpty()) {
            return null
        }
        return this.rearNode.value
    }
    
    func isEmpty() {
        return this.frontNode == null
    }
    
    func size() {
        return this.count
    }
}
```

### 3. Circular Array Implementation

Optimized array-based queue avoiding shift operation.

```klang
class CircularArrayQueue {
    func init(capacity) {
        this.items = []
        this.capacity = capacity
        this.front = 0
        this.rear = 0
        this.count = 0
        
        # Initialize array
        let i = 0
        while (i < capacity) {
            this.items.push(null)
            i = i + 1
        }
    }
    
    func enqueue(value) {
        if (this.isFull()) {
            this.resize()
        }
        
        this.items[this.rear] = value
        this.rear = (this.rear + 1) % this.capacity
        this.count = this.count + 1
    }
    
    func dequeue() {
        if (this.isEmpty()) {
            return null
        }
        
        let value = this.items[this.front]
        this.items[this.front] = null
        this.front = (this.front + 1) % this.capacity
        this.count = this.count - 1
        return value
    }
    
    func resize() {
        let newCapacity = this.capacity * 2
        let newItems = []
        
        let i = 0
        while (i < newCapacity) {
            newItems.push(null)
            i = i + 1
        }
        
        # Copy elements to new array
        let j = 0
        let idx = this.front
        while (j < this.count) {
            newItems[j] = this.items[idx]
            idx = (idx + 1) % this.capacity
            j = j + 1
        }
        
        this.items = newItems
        this.front = 0
        this.rear = this.count
        this.capacity = newCapacity
    }
    
    func isFull() {
        return this.count == this.capacity
    }
    
    func isEmpty() {
        return this.count == 0
    }
    
    func size() {
        return this.count
    }
}
```

---

## Queue Operations

### Core Operations with Implementation

```klang
class Queue {
    func init() {
        this.items = []
    }
    
    # Enqueue - Add element to rear - O(1)
    func enqueue(value) {
        this.items.push(value)
        return true
    }
    
    # Dequeue - Remove element from front - O(1) amortized
    func dequeue() {
        if (this.isEmpty()) {
            print("Queue underflow")
            return null
        }
        return this.items.shift()
    }
    
    # Front/Peek - View front element - O(1)
    func front() {
        if (this.isEmpty()) {
            return null
        }
        return this.items[0]
    }
    
    # Rear - View rear element - O(1)
    func rear() {
        if (this.isEmpty()) {
            return null
        }
        return this.items[this.items.length - 1]
    }
    
    # IsEmpty - Check if empty - O(1)
    func isEmpty() {
        return this.items.length == 0
    }
    
    # Size - Get number of elements - O(1)
    func size() {
        return this.items.length
    }
    
    # Clear - Remove all elements - O(1)
    func clear() {
        this.items = []
    }
    
    # Display - Print queue - O(n)
    func display() {
        if (this.isEmpty()) {
            print("Queue is empty")
            return
        }
        
        print("Front -> ", false)
        let i = 0
        while (i < this.items.length) {
            print(this.items[i], false)
            if (i < this.items.length - 1) {
                print(" <- ", false)
            }
            i = i + 1
        }
        print(" <- Rear")
    }
}
```

---

## Time and Space Complexity

### Operation Complexities

| Operation | Array-Based | Linked List | Circular Array |
|-----------|-------------|-------------|----------------|
| Enqueue   | O(1)        | O(1)        | O(1) amortized |
| Dequeue   | O(n)*       | O(1)        | O(1)           |
| Front     | O(1)        | O(1)        | O(1)           |
| Rear      | O(1)        | O(1)        | O(1)           |
| IsEmpty   | O(1)        | O(1)        | O(1)           |
| Size      | O(1)        | O(1)        | O(1)           |

*Array shift operation is O(n)

### Space Complexity

- **Array-Based**: O(n) where n is number of elements
- **Linked List**: O(n) + extra space for node pointers
- **Circular Array**: O(capacity)

---

## Queue Algorithms

### 1. Level-Order Traversal (BFS)

```klang
class TreeNode {
    func init(value) {
        this.value = value
        this.left = null
        this.right = null
    }
}

func levelOrderTraversal(root) {
    if (root == null) {
        return []
    }
    
    let result = []
    let queue = Queue()
    queue.enqueue(root)
    
    while (!queue.isEmpty()) {
        let levelSize = queue.size()
        let level = []
        
        let i = 0
        while (i < levelSize) {
            let node = queue.dequeue()
            level.push(node.value)
            
            if (node.left != null) {
                queue.enqueue(node.left)
            }
            if (node.right != null) {
                queue.enqueue(node.right)
            }
            i = i + 1
        }
        result.push(level)
    }
    
    return result
}

# Usage
#       1
#      / \
#     2   3
#    / \
#   4   5
let root = TreeNode(1)
root.left = TreeNode(2)
root.right = TreeNode(3)
root.left.left = TreeNode(4)
root.left.right = TreeNode(5)

print(levelOrderTraversal(root))
# Output: [[1], [2, 3], [4, 5]]
```

### 2. Task Scheduling (Round Robin)

```klang
class Task {
    func init(id, burstTime) {
        this.id = id
        this.burstTime = burstTime
        this.remainingTime = burstTime
    }
}

func roundRobinScheduling(tasks, quantum) {
    let queue = Queue()
    let currentTime = 0
    let completionTimes = []
    
    # Add all tasks to queue
    let i = 0
    while (i < tasks.length) {
        queue.enqueue(tasks[i])
        i = i + 1
    }
    
    while (!queue.isEmpty()) {
        let task = queue.dequeue()
        
        if (task.remainingTime > quantum) {
            # Execute for quantum time
            currentTime = currentTime + quantum
            task.remainingTime = task.remainingTime - quantum
            print("Task " + task.id + " executed for " + quantum + " units")
            
            # Re-enqueue task
            queue.enqueue(task)
        } else {
            # Task completes
            currentTime = currentTime + task.remainingTime
            print("Task " + task.id + " completed at time " + currentTime)
            completionTimes.push({id: task.id, time: currentTime})
            task.remainingTime = 0
        }
    }
    
    return completionTimes
}

# Usage
let tasks = [
    Task(1, 10),
    Task(2, 5),
    Task(3, 8)
]
roundRobinScheduling(tasks, 3)
```

### 3. Sliding Window Maximum

```klang
func slidingWindowMaximum(arr, k) {
    if (arr.length == 0 || k == 0) {
        return []
    }
    
    let result = []
    let deque = Deque()  # Stores indices
    
    let i = 0
    while (i < arr.length) {
        # Remove elements outside window
        while (!deque.isEmpty() && deque.peekFront() < i - k + 1) {
            deque.dequeueFront()
        }
        
        # Remove smaller elements (not useful)
        while (!deque.isEmpty() && arr[deque.peekRear()] < arr[i]) {
            deque.dequeueRear()
        }
        
        deque.enqueueRear(i)
        
        # Add to result if window is complete
        if (i >= k - 1) {
            result.push(arr[deque.peekFront()])
        }
        
        i = i + 1
    }
    
    return result
}

# Usage
let arr = [1, 3, -1, -3, 5, 3, 6, 7]
let k = 3
print(slidingWindowMaximum(arr, k))
# Output: [3, 3, 5, 5, 6, 7]
```

### 4. Generate Binary Numbers

```klang
func generateBinaryNumbers(n) {
    let result = []
    let queue = Queue()
    
    queue.enqueue("1")
    
    let i = 0
    while (i < n) {
        let binary = queue.dequeue()
        result.push(binary)
        
        queue.enqueue(binary + "0")
        queue.enqueue(binary + "1")
        
        i = i + 1
    }
    
    return result
}

# Usage
print(generateBinaryNumbers(10))
# Output: ["1", "10", "11", "100", "101", "110", "111", "1000", "1001", "1010"]
```

---

## Applications

### 1. Breadth-First Search (BFS) in Graphs

```klang
class Graph {
    func init(vertices) {
        this.vertices = vertices
        this.adjList = {}
        
        let i = 0
        while (i < vertices) {
            this.adjList[i] = []
            i = i + 1
        }
    }
    
    func addEdge(u, v) {
        this.adjList[u].push(v)
    }
    
    func bfs(start) {
        let visited = {}
        let queue = Queue()
        
        visited[start] = true
        queue.enqueue(start)
        
        let result = []
        
        while (!queue.isEmpty()) {
            let vertex = queue.dequeue()
            result.push(vertex)
            
            let neighbors = this.adjList[vertex]
            let i = 0
            while (i < neighbors.length) {
                let neighbor = neighbors[i]
                if (!visited[neighbor]) {
                    visited[neighbor] = true
                    queue.enqueue(neighbor)
                }
                i = i + 1
            }
        }
        
        return result
    }
}

# Usage
let graph = Graph(6)
graph.addEdge(0, 1)
graph.addEdge(0, 2)
graph.addEdge(1, 3)
graph.addEdge(1, 4)
graph.addEdge(2, 5)

print(graph.bfs(0))  # Output: [0, 1, 2, 3, 4, 5]
```

### 2. CPU Scheduling

```klang
class Process {
    func init(id, arrivalTime, burstTime) {
        this.id = id
        this.arrivalTime = arrivalTime
        this.burstTime = burstTime
        this.remainingTime = burstTime
        this.completionTime = 0
        this.waitingTime = 0
        this.turnaroundTime = 0
    }
}

func fcfsScheduling(processes) {
    let queue = Queue()
    let currentTime = 0
    
    # Sort by arrival time
    processes.sort(func(a, b) {
        return a.arrivalTime - b.arrivalTime
    })
    
    let i = 0
    while (i < processes.length) {
        let process = processes[i]
        
        if (currentTime < process.arrivalTime) {
            currentTime = process.arrivalTime
        }
        
        # Execute process
        currentTime = currentTime + process.burstTime
        process.completionTime = currentTime
        process.turnaroundTime = process.completionTime - process.arrivalTime
        process.waitingTime = process.turnaroundTime - process.burstTime
        
        print("Process " + process.id + " completed at " + currentTime)
        
        i = i + 1
    }
    
    # Calculate average waiting time
    let totalWait = 0
    i = 0
    while (i < processes.length) {
        totalWait = totalWait + processes[i].waitingTime
        i = i + 1
    }
    
    print("Average waiting time: " + (totalWait / processes.length))
}
```

### 3. Print Queue

```klang
class PrintJob {
    func init(id, pages, priority) {
        this.id = id
        this.pages = pages
        this.priority = priority
    }
}

class PrintQueue {
    func init() {
        this.queue = PriorityQueue()
    }
    
    func addJob(job) {
        this.queue.enqueue(job, job.priority)
        print("Added print job " + job.id + " (" + job.pages + " pages)")
    }
    
    func processNextJob() {
        if (this.queue.isEmpty()) {
            print("No jobs in queue")
            return null
        }
        
        let job = this.queue.dequeue()
        print("Printing job " + job.id + " (" + job.pages + " pages)")
        return job
    }
    
    func isEmpty() {
        return this.queue.isEmpty()
    }
}

# Usage
let printer = PrintQueue()
printer.addJob(PrintJob(1, 10, 3))
printer.addJob(PrintJob(2, 5, 1))   # Higher priority
printer.addJob(PrintJob(3, 8, 2))

while (!printer.isEmpty()) {
    printer.processNextJob()
}
```

### 4. Message Queue System

```klang
class Message {
    func init(id, sender, receiver, content) {
        this.id = id
        this.sender = sender
        this.receiver = receiver
        this.content = content
        this.timestamp = Date.now()
    }
}

class MessageQueue {
    func init() {
        this.queue = Queue()
        this.processed = []
    }
    
    func sendMessage(message) {
        this.queue.enqueue(message)
        print("Message " + message.id + " queued from " + message.sender)
    }
    
    func processMessage() {
        if (this.queue.isEmpty()) {
            return null
        }
        
        let message = this.queue.dequeue()
        print("Processing message " + message.id + ": " + message.content)
        this.processed.push(message)
        return message
    }
    
    func processAll() {
        while (!this.queue.isEmpty()) {
            this.processMessage()
        }
    }
    
    func getPendingCount() {
        return this.queue.size()
    }
}

# Usage
let msgQueue = MessageQueue()
msgQueue.sendMessage(Message(1, "Alice", "Bob", "Hello"))
msgQueue.sendMessage(Message(2, "Bob", "Alice", "Hi there"))
msgQueue.processAll()
```

---

## Interview Problems

### 1. Implement Queue using Stacks

```klang
class QueueUsingStacks {
    func init() {
        this.stack1 = []  # For enqueue
        this.stack2 = []  # For dequeue
    }
    
    func enqueue(value) {
        this.stack1.push(value)
    }
    
    func dequeue() {
        if (this.isEmpty()) {
            return null
        }
        
        # Move elements from stack1 to stack2 if needed
        if (this.stack2.length == 0) {
            while (this.stack1.length > 0) {
                this.stack2.push(this.stack1.pop())
            }
        }
        
        return this.stack2.pop()
    }
    
    func front() {
        if (this.isEmpty()) {
            return null
        }
        
        if (this.stack2.length == 0) {
            while (this.stack1.length > 0) {
                this.stack2.push(this.stack1.pop())
            }
        }
        
        return this.stack2[this.stack2.length - 1]
    }
    
    func isEmpty() {
        return this.stack1.length == 0 && this.stack2.length == 0
    }
}

# Usage
let queue = QueueUsingStacks()
queue.enqueue(1)
queue.enqueue(2)
print(queue.dequeue())  # 1
queue.enqueue(3)
print(queue.dequeue())  # 2
print(queue.front())    # 3
```

### 2. Implement Stack using Queues

```klang
class StackUsingQueues {
    func init() {
        this.queue1 = Queue()
        this.queue2 = Queue()
    }
    
    func push(value) {
        # Add to queue2
        this.queue2.enqueue(value)
        
        # Move all from queue1 to queue2
        while (!this.queue1.isEmpty()) {
            this.queue2.enqueue(this.queue1.dequeue())
        }
        
        # Swap queues
        let temp = this.queue1
        this.queue1 = this.queue2
        this.queue2 = temp
    }
    
    func pop() {
        if (this.isEmpty()) {
            return null
        }
        return this.queue1.dequeue()
    }
    
    func top() {
        if (this.isEmpty()) {
            return null
        }
        return this.queue1.front()
    }
    
    func isEmpty() {
        return this.queue1.isEmpty()
    }
}

# Usage
let stack = StackUsingQueues()
stack.push(1)
stack.push(2)
stack.push(3)
print(stack.pop())  # 3
print(stack.top())  # 2
```

### 3. Design Circular Queue

```klang
class MyCircularQueue {
    func init(k) {
        this.capacity = k
        this.items = []
        this.front = 0
        this.rear = -1
        this.count = 0
        
        let i = 0
        while (i < k) {
            this.items.push(null)
            i = i + 1
        }
    }
    
    func enQueue(value) {
        if (this.isFull()) {
            return false
        }
        
        this.rear = (this.rear + 1) % this.capacity
        this.items[this.rear] = value
        this.count = this.count + 1
        return true
    }
    
    func deQueue() {
        if (this.isEmpty()) {
            return false
        }
        
        this.items[this.front] = null
        this.front = (this.front + 1) % this.capacity
        this.count = this.count - 1
        return true
    }
    
    func Front() {
        if (this.isEmpty()) {
            return -1
        }
        return this.items[this.front]
    }
    
    func Rear() {
        if (this.isEmpty()) {
            return -1
        }
        return this.items[this.rear]
    }
    
    func isEmpty() {
        return this.count == 0
    }
    
    func isFull() {
        return this.count == this.capacity
    }
}

# Usage
let cq = MyCircularQueue(3)
print(cq.enQueue(1))  # true
print(cq.enQueue(2))  # true
print(cq.enQueue(3))  # true
print(cq.enQueue(4))  # false (full)
print(cq.Rear())      # 3
print(cq.isFull())    # true
print(cq.deQueue())   # true
print(cq.enQueue(4))  # true
print(cq.Rear())      # 4
```

### 4. First Non-Repeating Character in Stream

```klang
func firstNonRepeatingCharacter(stream) {
    let queue = Queue()
    let charCount = {}
    let result = []
    
    let i = 0
    while (i < stream.length) {
        let char = stream[i]
        
        # Update count
        if (charCount[char] == null) {
            charCount[char] = 0
        }
        charCount[char] = charCount[char] + 1
        
        # Add to queue if first occurrence
        if (charCount[char] == 1) {
            queue.enqueue(char)
        }
        
        # Remove repeating characters from front
        while (!queue.isEmpty() && charCount[queue.front()] > 1) {
            queue.dequeue()
        }
        
        # Add current result
        if (queue.isEmpty()) {
            result.push("#")
        } else {
            result.push(queue.front())
        }
        
        i = i + 1
    }
    
    return result
}

# Usage
print(firstNonRepeatingCharacter("aabccxb"))
# Output: ["a", "#", "b", "b", "b", "b", "x"]
```

### 5. Rotting Oranges (Multi-source BFS)

```klang
func rottingOranges(grid) {
    let rows = grid.length
    if (rows == 0) {
        return 0
    }
    let cols = grid[0].length
    
    let queue = Queue()
    let fresh = 0
    
    # Find all rotten oranges and count fresh ones
    let i = 0
    while (i < rows) {
        let j = 0
        while (j < cols) {
            if (grid[i][j] == 2) {
                queue.enqueue({row: i, col: j})
            } else if (grid[i][j] == 1) {
                fresh = fresh + 1
            }
            j = j + 1
        }
        i = i + 1
    }
    
    if (fresh == 0) {
        return 0
    }
    
    let minutes = 0
    let directions = [[0, 1], [1, 0], [0, -1], [-1, 0]]
    
    while (!queue.isEmpty() && fresh > 0) {
        let size = queue.size()
        
        let k = 0
        while (k < size) {
            let cell = queue.dequeue()
            
            # Check all 4 directions
            let d = 0
            while (d < 4) {
                let newRow = cell.row + directions[d][0]
                let newCol = cell.col + directions[d][1]
                
                if (newRow >= 0 && newRow < rows && 
                    newCol >= 0 && newCol < cols &&
                    grid[newRow][newCol] == 1) {
                    grid[newRow][newCol] = 2
                    fresh = fresh - 1
                    queue.enqueue({row: newRow, col: newCol})
                }
                d = d + 1
            }
            k = k + 1
        }
        minutes = minutes + 1
    }
    
    return fresh == 0 ? minutes : -1
}

# Usage
let grid = [
    [2, 1, 1],
    [1, 1, 0],
    [0, 1, 1]
]
print(rottingOranges(grid))  # Output: 4
```

---

## Best Practices

### 1. Choose Right Implementation

```klang
# Use linked list for frequent dequeue operations
class OptimalQueue {
    func init() {
        this.front = null
        this.rear = null
        this.size = 0
    }
    # Linked list implementation...
}

# Use circular array for bounded size
class BoundedQueue {
    func init(capacity) {
        # Circular array implementation
    }
}
```

### 2. Handle Edge Cases

```klang
func safeDequeue(queue) {
    if (queue.isEmpty()) {
        print("Warning: Queue is empty")
        return null
    }
    return queue.dequeue()
}

func safeEnqueue(queue, value, maxSize) {
    if (queue.size() >= maxSize) {
        print("Warning: Queue is full")
        return false
    }
    queue.enqueue(value)
    return true
}
```

### 3. Use Appropriate Queue Type

```klang
# Use priority queue for task scheduling
func scheduleTask(task, priority) {
    let taskQueue = PriorityQueue()
    taskQueue.enqueue(task, priority)
}

# Use deque for sliding window problems
func slidingWindow(arr, k) {
    let deque = Deque()
    # Implementation...
}

# Use simple queue for BFS
func bfs(graph, start) {
    let queue = Queue()
    queue.enqueue(start)
    # Implementation...
}
```

### 4. Memory Management

```klang
# Clear queue when done
func processAndClear(queue) {
    while (!queue.isEmpty()) {
        let item = queue.dequeue()
        # Process item
    }
    # Queue is now empty and ready for reuse
}

# Limit queue size for bounded scenarios
class BoundedMessageQueue {
    func init(maxSize) {
        this.queue = Queue()
        this.maxSize = maxSize
    }
    
    func enqueue(message) {
        if (this.queue.size() >= this.maxSize) {
            # Remove oldest message
            this.queue.dequeue()
        }
        this.queue.enqueue(message)
    }
}
```

### 5. Performance Considerations

```klang
# Avoid array shift for better performance
# BAD: O(n) dequeue
class SlowQueue {
    func dequeue() {
        return this.items.shift()  # O(n)
    }
}

# GOOD: O(1) dequeue with circular array
class FastQueue {
    func dequeue() {
        let value = this.items[this.front]
        this.front = (this.front + 1) % this.capacity  # O(1)
        return value
    }
}
```

---

## Complete Implementations

### Production-Ready Queue Class

```klang
class ProductionQueue {
    func init(capacity) {
        this.items = []
        this.capacity = capacity || -1  # -1 for unlimited
        this.front = 0
        this.rear = 0
        this.count = 0
        
        if (this.capacity > 0) {
            let i = 0
            while (i < this.capacity) {
                this.items.push(null)
                i = i + 1
            }
        }
    }
    
    func enqueue(value) {
        # Check if bounded and full
        if (this.capacity > 0 && this.count >= this.capacity) {
            throw "Queue overflow: maximum capacity reached"
        }
        
        if (this.capacity < 0) {
            # Unbounded queue
            this.items.push(value)
            this.count = this.count + 1
        } else {
            # Bounded circular queue
            this.items[this.rear] = value
            this.rear = (this.rear + 1) % this.capacity
            this.count = this.count + 1
        }
        
        return true
    }
    
    func dequeue() {
        if (this.isEmpty()) {
            throw "Queue underflow: queue is empty"
        }
        
        let value
        if (this.capacity < 0) {
            value = this.items.shift()
        } else {
            value = this.items[this.front]
            this.items[this.front] = null
            this.front = (this.front + 1) % this.capacity
        }
        
        this.count = this.count - 1
        return value
    }
    
    func peek() {
        if (this.isEmpty()) {
            return null
        }
        
        if (this.capacity < 0) {
            return this.items[0]
        } else {
            return this.items[this.front]
        }
    }
    
    func isEmpty() {
        return this.count == 0
    }
    
    func isFull() {
        if (this.capacity < 0) {
            return false
        }
        return this.count >= this.capacity
    }
    
    func size() {
        return this.count
    }
    
    func clear() {
        if (this.capacity < 0) {
            this.items = []
        } else {
            let i = 0
            while (i < this.capacity) {
                this.items[i] = null
                i = i + 1
            }
        }
        this.front = 0
        this.rear = 0
        this.count = 0
    }
    
    func toArray() {
        let result = []
        
        if (this.capacity < 0) {
            return this.items.slice()
        } else {
            let i = 0
            let idx = this.front
            while (i < this.count) {
                result.push(this.items[idx])
                idx = (idx + 1) % this.capacity
                i = i + 1
            }
        }
        
        return result
    }
    
    func display() {
        if (this.isEmpty()) {
            print("Queue: []")
            return
        }
        
        let arr = this.toArray()
        print("Queue (front to rear): [" + arr.join(", ") + "]")
        print("Size: " + this.count)
        if (this.capacity > 0) {
            print("Capacity: " + this.capacity)
        }
    }
}

# Usage
let queue = ProductionQueue(5)
queue.enqueue(10)
queue.enqueue(20)
queue.enqueue(30)
queue.display()
print("Dequeued: " + queue.dequeue())
queue.display()
```

---

## Summary

Queues are essential data structures that follow FIFO principle:

- **Simple Queue**: Basic FIFO operations
- **Circular Queue**: Efficient space utilization
- **Priority Queue**: Order by priority
- **Deque**: Operations at both ends

**Key Operations**: All O(1) with proper implementation

**Common Uses**:
- BFS traversal
- Task scheduling
- Message queues
- Resource management

**Interview Tips**:
- Know different implementations
- Understand when to use each type
- Practice queue-based algorithms
- Handle edge cases properly

Master queues for efficient data processing in ordered scenarios!
