# Linked Lists in KLang - Data Structures & Algorithms

## Table of Contents
1. [Introduction](#introduction)
2. [What is a Linked List?](#what-is-a-linked-list)
3. [Types of Linked Lists](#types-of-linked-lists)
4. [Node Structure](#node-structure)
5. [Linked List Operations](#linked-list-operations)
6. [Common Algorithms](#common-algorithms)
7. [Time and Space Complexity](#time-and-space-complexity)
8. [Comparison with Arrays](#comparison-with-arrays)
9. [Best Practices](#best-practices)
10. [Interview Problems](#interview-problems)
11. [Examples](#examples)

---

## Introduction

A **linked list** is a linear data structure where elements are stored in nodes, and each node points to the next node in the sequence. Unlike arrays, linked lists don't require contiguous memory allocation, making them more flexible for dynamic data management.

### Why Use Linked Lists?

- **Dynamic Size**: Grows and shrinks at runtime without reallocation
- **Efficient Insertions/Deletions**: O(1) when position is known
- **No Memory Waste**: Only allocates memory as needed
- **Flexible Memory**: Can use scattered memory locations

### Advantages over Arrays

| Feature | Linked List | Array |
|---------|------------|-------|
| Size | Dynamic | Fixed (or requires reallocation) |
| Insertion at beginning | O(1) | O(n) |
| Deletion at beginning | O(1) | O(n) |
| Memory allocation | As needed | Contiguous block |
| Random access | O(n) | O(1) |

---

## What is a Linked List?

A **linked list** consists of nodes where each node contains:
1. **Data**: The value stored in the node
2. **Next**: Reference (pointer) to the next node

### Visual Representation

```
Singly Linked List:
Head -> [10|•] -> [20|•] -> [30|•] -> [40|null]

Doubly Linked List:
Head -> [•|10|•] <-> [•|20|•] <-> [•|30|•] <-> [•|40|null]
         null

Circular Linked List:
Head -> [10|•] -> [20|•] -> [30|•] -> [40|•]
         ↑__________________________________|
```

### Characteristics

- **Non-contiguous**: Nodes can be anywhere in memory
- **Sequential Access**: Must traverse from head to reach a node
- **Dynamic**: Size changes easily
- **Pointer-based**: Nodes connected via references

---

## Types of Linked Lists

### 1. Singly Linked List

Each node has a reference to the **next** node only.

```klang
class Node {
    let data = null
    let next = null
    
    fn init(value) {
        this.data = value
        this.next = null
    }
}
```

**Use Cases**:
- Simple forward traversal
- Stack implementation
- Hash table collision handling

### 2. Doubly Linked List

Each node has references to both **next** and **previous** nodes.

```klang
class DoublyNode {
    let data = null
    let next = null
    let prev = null
    
    fn init(value) {
        this.data = value
        this.next = null
        this.prev = null
    }
}
```

**Use Cases**:
- Bidirectional traversal
- Browser history (forward/back)
- Deque implementation
- LRU cache

### 3. Circular Linked List

The last node points back to the first node, forming a circle.

```klang
class CircularNode {
    let data = null
    let next = null
    
    fn init(value) {
        this.data = value
        this.next = null
    }
}
```

**Use Cases**:
- Round-robin scheduling
- Circular buffers
- Multiplayer game turns

---

## Node Structure

### Basic Node Implementation

```klang
class Node {
    let data = null
    let next = null
    
    fn init(value) {
        this.data = value
        this.next = null
    }
    
    fn toString() -> string {
        return str(this.data)
    }
}
```

### Creating Nodes

```klang
# Create individual nodes
let node1 = new Node(10)
let node2 = new Node(20)
let node3 = new Node(30)

# Link nodes
node1.next = node2
node2.next = node3

# Result: 10 -> 20 -> 30 -> null
```

---

## Linked List Operations

### Basic Singly Linked List Class

```klang
class LinkedList {
    let head = null
    let size = 0
    
    fn init() {
        this.head = null
        this.size = 0
    }
    
    fn isEmpty() -> bool {
        return this.head == null
    }
    
    fn getSize() -> int {
        return this.size
    }
}
```

### 1. Insert at Beginning

```klang
fn insertAtBeginning(value) {
    let newNode = new Node(value)
    newNode.next = this.head
    this.head = newNode
    this.size = this.size + 1
}

# Usage
let list = new LinkedList()
list.insertAtBeginning(30)
list.insertAtBeginning(20)
list.insertAtBeginning(10)
# List: 10 -> 20 -> 30 -> null
```

**Time Complexity**: O(1)  
**Space Complexity**: O(1)

### 2. Insert at End

```klang
fn insertAtEnd(value) {
    let newNode = new Node(value)
    
    if (this.head == null) {
        this.head = newNode
        this.size = this.size + 1
        return
    }
    
    let current = this.head
    while (current.next != null) {
        current = current.next
    }
    
    current.next = newNode
    this.size = this.size + 1
}

# Usage
let list = new LinkedList()
list.insertAtEnd(10)
list.insertAtEnd(20)
list.insertAtEnd(30)
# List: 10 -> 20 -> 30 -> null
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1)

### 3. Insert at Position

```klang
fn insertAtPosition(value, position: int) -> bool {
    if (position < 0 || position > this.size) {
        return false
    }
    
    if (position == 0) {
        this.insertAtBeginning(value)
        return true
    }
    
    let newNode = new Node(value)
    let current = this.head
    let i = 0
    
    while (i < position - 1) {
        current = current.next
        i = i + 1
    }
    
    newNode.next = current.next
    current.next = newNode
    this.size = this.size + 1
    return true
}

# Usage
let list = new LinkedList()
list.insertAtEnd(10)
list.insertAtEnd(30)
list.insertAtPosition(20, 1)
# List: 10 -> 20 -> 30 -> null
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1)

### 4. Delete from Beginning

```klang
fn deleteFromBeginning() -> any {
    if (this.head == null) {
        return null
    }
    
    let data = this.head.data
    this.head = this.head.next
    this.size = this.size - 1
    return data
}

# Usage
let list = new LinkedList()
list.insertAtEnd(10)
list.insertAtEnd(20)
list.insertAtEnd(30)
let deleted = list.deleteFromBeginning()  # Returns 10
# List: 20 -> 30 -> null
```

**Time Complexity**: O(1)  
**Space Complexity**: O(1)

### 5. Delete from End

```klang
fn deleteFromEnd() -> any {
    if (this.head == null) {
        return null
    }
    
    if (this.head.next == null) {
        let data = this.head.data
        this.head = null
        this.size = this.size - 1
        return data
    }
    
    let current = this.head
    while (current.next.next != null) {
        current = current.next
    }
    
    let data = current.next.data
    current.next = null
    this.size = this.size - 1
    return data
}
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1)

### 6. Delete by Value

```klang
fn deleteByValue(value) -> bool {
    if (this.head == null) {
        return false
    }
    
    # If head node contains the value
    if (this.head.data == value) {
        this.head = this.head.next
        this.size = this.size - 1
        return true
    }
    
    let current = this.head
    while (current.next != null) {
        if (current.next.data == value) {
            current.next = current.next.next
            this.size = this.size - 1
            return true
        }
        current = current.next
    }
    
    return false  # Value not found
}

# Usage
let list = new LinkedList()
list.insertAtEnd(10)
list.insertAtEnd(20)
list.insertAtEnd(30)
list.deleteByValue(20)
# List: 10 -> 30 -> null
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1)

### 7. Search

```klang
fn search(value) -> bool {
    let current = this.head
    
    while (current != null) {
        if (current.data == value) {
            return true
        }
        current = current.next
    }
    
    return false
}

# Get node at position
fn getAt(position: int) -> any {
    if (position < 0 || position >= this.size) {
        return null
    }
    
    let current = this.head
    let i = 0
    
    while (i < position) {
        current = current.next
        i = i + 1
    }
    
    return current.data
}
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1)

### 8. Traversal

```klang
fn traverse() {
    let current = this.head
    
    while (current != null) {
        print(str(current.data) + " -> ")
        current = current.next
    }
    
    println("null")
}

# Convert to array
fn toArray() -> array {
    let result = []
    let current = this.head
    
    while (current != null) {
        result.push(current.data)
        current = current.next
    }
    
    return result
}
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1) for traverse, O(n) for toArray

---

## Common Algorithms

### 1. Reverse Linked List

Reverse the entire linked list in-place.

```klang
fn reverse() {
    let prev = null
    let current = this.head
    let next = null
    
    while (current != null) {
        next = current.next
        current.next = prev
        prev = current
        current = next
    }
    
    this.head = prev
}

# Usage
let list = new LinkedList()
list.insertAtEnd(1)
list.insertAtEnd(2)
list.insertAtEnd(3)
list.insertAtEnd(4)
list.reverse()
# List: 4 -> 3 -> 2 -> 1 -> null
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1)

### 2. Detect Cycle (Floyd's Algorithm)

Use two pointers (slow and fast) to detect if list has a cycle.

```klang
fn hasCycle() -> bool {
    if (this.head == null || this.head.next == null) {
        return false
    }
    
    let slow = this.head
    let fast = this.head
    
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

**Time Complexity**: O(n)  
**Space Complexity**: O(1)

### 3. Find Middle Element

Use slow and fast pointer technique.

```klang
fn findMiddle() -> any {
    if (this.head == null) {
        return null
    }
    
    let slow = this.head
    let fast = this.head
    
    while (fast != null && fast.next != null) {
        slow = slow.next
        fast = fast.next.next
    }
    
    return slow.data
}

# Usage
let list = new LinkedList()
list.insertAtEnd(1)
list.insertAtEnd(2)
list.insertAtEnd(3)
list.insertAtEnd(4)
list.insertAtEnd(5)
let middle = list.findMiddle()  # Returns 3
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1)

### 4. Merge Two Sorted Lists

Merge two sorted linked lists into one sorted list.

```klang
fn mergeSortedLists(list1: LinkedList, list2: LinkedList) -> LinkedList {
    let merged = new LinkedList()
    let dummy = new Node(0)
    let current = dummy
    
    let ptr1 = list1.head
    let ptr2 = list2.head
    
    while (ptr1 != null && ptr2 != null) {
        if (ptr1.data <= ptr2.data) {
            current.next = ptr1
            ptr1 = ptr1.next
        } else {
            current.next = ptr2
            ptr2 = ptr2.next
        }
        current = current.next
    }
    
    # Attach remaining nodes
    if (ptr1 != null) {
        current.next = ptr1
    }
    if (ptr2 != null) {
        current.next = ptr2
    }
    
    merged.head = dummy.next
    return merged
}

# Usage
let list1 = new LinkedList()
list1.insertAtEnd(1)
list1.insertAtEnd(3)
list1.insertAtEnd(5)

let list2 = new LinkedList()
list2.insertAtEnd(2)
list2.insertAtEnd(4)
list2.insertAtEnd(6)

let merged = mergeSortedLists(list1, list2)
# Result: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> null
```

**Time Complexity**: O(n + m)  
**Space Complexity**: O(1)

### 5. Remove Nth Node from End

```klang
fn removeNthFromEnd(n: int) -> bool {
    if (this.head == null || n <= 0) {
        return false
    }
    
    let dummy = new Node(0)
    dummy.next = this.head
    let fast = dummy
    let slow = dummy
    
    # Move fast pointer n steps ahead
    let i = 0
    while (i <= n && fast != null) {
        fast = fast.next
        i = i + 1
    }
    
    if (fast == null) {
        return false  # n is larger than list size
    }
    
    # Move both pointers until fast reaches end
    while (fast != null) {
        slow = slow.next
        fast = fast.next
    }
    
    # Remove the node
    slow.next = slow.next.next
    this.head = dummy.next
    this.size = this.size - 1
    return true
}
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1)

### 6. Check if Palindrome

```klang
fn isPalindrome() -> bool {
    if (this.head == null || this.head.next == null) {
        return true
    }
    
    # Find middle
    let slow = this.head
    let fast = this.head
    
    while (fast != null && fast.next != null) {
        slow = slow.next
        fast = fast.next.next
    }
    
    # Reverse second half
    let prev = null
    let current = slow
    let next = null
    
    while (current != null) {
        next = current.next
        current.next = prev
        prev = current
        current = next
    }
    
    # Compare first and second half
    let left = this.head
    let right = prev
    
    while (right != null) {
        if (left.data != right.data) {
            return false
        }
        left = left.next
        right = right.next
    }
    
    return true
}
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1)

---

## Time and Space Complexity

### Operation Complexities

| Operation | Singly | Doubly | Array |
|-----------|--------|--------|-------|
| Access by index | O(n) | O(n) | O(1) |
| Search | O(n) | O(n) | O(n) |
| Insert at beginning | O(1) | O(1) | O(n) |
| Insert at end | O(n) | O(1)* | O(1)** |
| Insert at position | O(n) | O(n) | O(n) |
| Delete from beginning | O(1) | O(1) | O(n) |
| Delete from end | O(n) | O(1)* | O(1) |
| Delete from position | O(n) | O(n) | O(n) |

\* With tail pointer  
\** Amortized

### Space Complexity

- **Per Node**: O(1) extra space for pointer(s)
- **Total**: O(n) for n nodes
- **Doubly Linked**: 2x pointer space vs singly

---

## Comparison with Arrays

### When to Use Linked Lists

✅ **Use Linked Lists when:**
- Frequent insertions/deletions at beginning
- Size is unknown and changes frequently
- Don't need random access
- Memory fragmentation is acceptable
- Need O(1) insertions at known positions

### When to Use Arrays

✅ **Use Arrays when:**
- Need fast random access (O(1))
- Size is known or changes rarely
- Memory should be contiguous
- Cache performance is critical
- Less overhead per element

### Real-World Examples

**Linked Lists:**
- Music playlist (next/previous song)
- Browser history
- Undo/redo functionality
- Memory management (free lists)

**Arrays:**
- High-frequency trading (fast access)
- Image/video processing
- Mathematical computations
- Gaming (particle systems)

---

## Best Practices

### 1. Always Check for Null

```klang
# Good: Check before dereferencing
fn safeAccess(node) {
    if (node != null && node.next != null) {
        return node.next.data
    }
    return null
}

# Bad: May cause null pointer error
fn unsafeAccess(node) {
    return node.next.data
}
```

### 2. Use Dummy Nodes for Simplicity

```klang
# Simplifies edge cases
fn insertInSortedOrder(value) {
    let dummy = new Node(0)
    dummy.next = this.head
    let current = dummy
    
    while (current.next != null && current.next.data < value) {
        current = current.next
    }
    
    let newNode = new Node(value)
    newNode.next = current.next
    current.next = newNode
    
    this.head = dummy.next
}
```

### 3. Maintain Size Counter

```klang
# Good: O(1) size check
class LinkedList {
    let head = null
    let size = 0
    
    fn getSize() -> int {
        return this.size  # O(1)
    }
}

# Bad: O(n) size check
fn getSizeSlow() -> int {
    let count = 0
    let current = this.head
    while (current != null) {
        count = count + 1
        current = current.next
    }
    return count
}
```

### 4. Use Two Pointer Technique

```klang
# Efficient for many problems
fn findNthFromEnd(n: int) -> any {
    let fast = this.head
    let slow = this.head
    
    # Move fast n steps ahead
    let i = 0
    while (i < n && fast != null) {
        fast = fast.next
        i = i + 1
    }
    
    # Move both until fast reaches end
    while (fast != null) {
        slow = slow.next
        fast = fast.next
    }
    
    return slow.data
}
```

### 5. Prefer Iterative over Recursive

```klang
# Good: Iterative (no stack overflow risk)
fn reverseIterative() {
    let prev = null
    let current = this.head
    
    while (current != null) {
        let next = current.next
        current.next = prev
        prev = current
        current = next
    }
    
    this.head = prev
}

# Recursive (may cause stack overflow for large lists)
fn reverseRecursive(node) {
    if (node == null || node.next == null) {
        return node
    }
    
    let rest = reverseRecursive(node.next)
    node.next.next = node
    node.next = null
    return rest
}
```

---

## Interview Problems

### Problem 1: Reverse Linked List

**Problem**: Reverse a singly linked list.

```klang
fn reverseList(head) -> Node {
    let prev = null
    let current = head
    
    while (current != null) {
        let nextTemp = current.next
        current.next = prev
        prev = current
        current = nextTemp
    }
    
    return prev
}

# Usage
let list = new LinkedList()
list.insertAtEnd(1)
list.insertAtEnd(2)
list.insertAtEnd(3)
list.insertAtEnd(4)
list.head = reverseList(list.head)
# Result: 4 -> 3 -> 2 -> 1 -> null
```

**Time**: O(n), **Space**: O(1)

### Problem 2: Linked List Cycle Detection

**Problem**: Detect if a linked list has a cycle.

```klang
fn detectCycle(head) -> bool {
    if (head == null) {
        return false
    }
    
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

**Time**: O(n), **Space**: O(1)

### Problem 3: Merge Two Sorted Lists

**Problem**: Merge two sorted linked lists into one sorted list.

```klang
fn mergeTwoLists(l1, l2) -> Node {
    let dummy = new Node(0)
    let current = dummy
    
    while (l1 != null && l2 != null) {
        if (l1.data <= l2.data) {
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

**Time**: O(n + m), **Space**: O(1)

### Problem 4: Remove Duplicates from Sorted List

**Problem**: Remove duplicate values from a sorted linked list.

```klang
fn removeDuplicates(head) -> Node {
    if (head == null) {
        return null
    }
    
    let current = head
    
    while (current != null && current.next != null) {
        if (current.data == current.next.data) {
            current.next = current.next.next
        } else {
            current = current.next
        }
    }
    
    return head
}

# Usage
# Input: 1 -> 1 -> 2 -> 3 -> 3
# Output: 1 -> 2 -> 3
```

**Time**: O(n), **Space**: O(1)

### Problem 5: Intersection of Two Linked Lists

**Problem**: Find the node where two linked lists intersect.

```klang
fn getIntersectionNode(headA, headB) -> Node {
    if (headA == null || headB == null) {
        return null
    }
    
    let ptrA = headA
    let ptrB = headB
    
    # Traverse both lists
    while (ptrA != ptrB) {
        # If end of list A, switch to list B
        if (ptrA == null) {
            ptrA = headB
        } else {
            ptrA = ptrA.next
        }
        
        # If end of list B, switch to list A
        if (ptrB == null) {
            ptrB = headA
        } else {
            ptrB = ptrB.next
        }
    }
    
    return ptrA  # Either intersection node or null
}
```

**Time**: O(n + m), **Space**: O(1)

### Problem 6: Add Two Numbers

**Problem**: Add two numbers represented by linked lists (digits in reverse order).

```klang
fn addTwoNumbers(l1, l2) -> Node {
    let dummy = new Node(0)
    let current = dummy
    let carry = 0
    
    while (l1 != null || l2 != null || carry > 0) {
        let sum = carry
        
        if (l1 != null) {
            sum = sum + l1.data
            l1 = l1.next
        }
        
        if (l2 != null) {
            sum = sum + l2.data
            l2 = l2.next
        }
        
        carry = sum / 10
        current.next = new Node(sum % 10)
        current = current.next
    }
    
    return dummy.next
}

# Example:
# Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
# Output: 7 -> 0 -> 8  (342 + 465 = 807)
```

**Time**: O(max(n, m)), **Space**: O(max(n, m))

---

## Examples

### Example 1: Complete Singly Linked List Implementation

```klang
class Node {
    let data = null
    let next = null
    
    fn init(value) {
        this.data = value
        this.next = null
    }
}

class LinkedList {
    let head = null
    let size = 0
    
    fn init() {
        this.head = null
        this.size = 0
    }
    
    fn insertAtBeginning(value) {
        let newNode = new Node(value)
        newNode.next = this.head
        this.head = newNode
        this.size = this.size + 1
    }
    
    fn insertAtEnd(value) {
        let newNode = new Node(value)
        
        if (this.head == null) {
            this.head = newNode
        } else {
            let current = this.head
            while (current.next != null) {
                current = current.next
            }
            current.next = newNode
        }
        
        this.size = this.size + 1
    }
    
    fn deleteByValue(value) -> bool {
        if (this.head == null) {
            return false
        }
        
        if (this.head.data == value) {
            this.head = this.head.next
            this.size = this.size - 1
            return true
        }
        
        let current = this.head
        while (current.next != null) {
            if (current.next.data == value) {
                current.next = current.next.next
                this.size = this.size - 1
                return true
            }
            current = current.next
        }
        
        return false
    }
    
    fn search(value) -> bool {
        let current = this.head
        
        while (current != null) {
            if (current.data == value) {
                return true
            }
            current = current.next
        }
        
        return false
    }
    
    fn reverse() {
        let prev = null
        let current = this.head
        let next = null
        
        while (current != null) {
            next = current.next
            current.next = prev
            prev = current
            current = next
        }
        
        this.head = prev
    }
    
    fn traverse() {
        let current = this.head
        let result = ""
        
        while (current != null) {
            result = result + str(current.data)
            if (current.next != null) {
                result = result + " -> "
            }
            current = current.next
        }
        
        result = result + " -> null"
        println(result)
    }
    
    fn getSize() -> int {
        return this.size
    }
}

# Usage
let list = new LinkedList()
list.insertAtEnd(10)
list.insertAtEnd(20)
list.insertAtEnd(30)
list.insertAtBeginning(5)
list.traverse()  # Output: 5 -> 10 -> 20 -> 30 -> null

println("Size: " + str(list.getSize()))  # Size: 4
println("Contains 20? " + str(list.search(20)))  # true

list.deleteByValue(20)
list.traverse()  # Output: 5 -> 10 -> 30 -> null

list.reverse()
list.traverse()  # Output: 30 -> 10 -> 5 -> null
```

### Example 2: Doubly Linked List Implementation

```klang
class DoublyNode {
    let data = null
    let next = null
    let prev = null
    
    fn init(value) {
        this.data = value
        this.next = null
        this.prev = null
    }
}

class DoublyLinkedList {
    let head = null
    let tail = null
    let size = 0
    
    fn init() {
        this.head = null
        this.tail = null
        this.size = 0
    }
    
    fn insertAtEnd(value) {
        let newNode = new DoublyNode(value)
        
        if (this.head == null) {
            this.head = newNode
            this.tail = newNode
        } else {
            this.tail.next = newNode
            newNode.prev = this.tail
            this.tail = newNode
        }
        
        this.size = this.size + 1
    }
    
    fn insertAtBeginning(value) {
        let newNode = new DoublyNode(value)
        
        if (this.head == null) {
            this.head = newNode
            this.tail = newNode
        } else {
            newNode.next = this.head
            this.head.prev = newNode
            this.head = newNode
        }
        
        this.size = this.size + 1
    }
    
    fn traverseForward() {
        let current = this.head
        let result = "null <- "
        
        while (current != null) {
            result = result + str(current.data)
            if (current.next != null) {
                result = result + " <-> "
            }
            current = current.next
        }
        
        result = result + " -> null"
        println(result)
    }
    
    fn traverseBackward() {
        let current = this.tail
        let result = "null <- "
        
        while (current != null) {
            result = result + str(current.data)
            if (current.prev != null) {
                result = result + " <-> "
            }
            current = current.prev
        }
        
        result = result + " -> null"
        println(result)
    }
}

# Usage
let dlist = new DoublyLinkedList()
dlist.insertAtEnd(10)
dlist.insertAtEnd(20)
dlist.insertAtEnd(30)
dlist.insertAtBeginning(5)

println("Forward:")
dlist.traverseForward()
# Output: null <- 5 <-> 10 <-> 20 <-> 30 -> null

println("Backward:")
dlist.traverseBackward()
# Output: null <- 30 <-> 20 <-> 10 <-> 5 -> null
```

### Example 3: LRU Cache using Doubly Linked List

```klang
class LRUCache {
    let capacity = 0
    let cache = {}
    let list = null
    
    fn init(cap: int) {
        this.capacity = cap
        this.cache = {}
        this.list = new DoublyLinkedList()
    }
    
    fn get(key) -> any {
        if (key in this.cache) {
            # Move to front (most recently used)
            let value = this.cache[key]
            this.list.moveToFront(key)
            return value
        }
        return null
    }
    
    fn put(key, value) {
        if (key in this.cache) {
            # Update value and move to front
            this.cache[key] = value
            this.list.moveToFront(key)
        } else {
            if (this.list.getSize() >= this.capacity) {
                # Remove least recently used (tail)
                let removed = this.list.removeTail()
                delete this.cache[removed]
            }
            
            this.cache[key] = value
            this.list.insertAtBeginning(key)
        }
    }
}
```

---

## Summary

- **Linked lists** are dynamic data structures with nodes containing data and pointers
- **Three types**: Singly, Doubly, and Circular linked lists
- **Insertion/Deletion** at beginning is O(1), very efficient
- **Random access** is O(n), slower than arrays
- **Use cases**: Dynamic data, frequent insertions/deletions, unknown size
- **Common patterns**: Two-pointer technique, dummy nodes, Floyd's cycle detection
- **Best for**: Undo/redo, music playlists, memory management

---

## Next Steps

Continue to:
- [Stacks](04_stacks.md)
- [Queues](05_queues.md)
- [Trees](06_trees.md)
- [Sorting Algorithms](../algorithms/01_sorting.md)

Happy coding! 🚀
