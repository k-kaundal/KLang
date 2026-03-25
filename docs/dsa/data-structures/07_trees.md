# Trees in KLang - Data Structures & Algorithms

## Table of Contents
1. [Introduction](#introduction)
2. [What is a Tree?](#what-is-a-tree)
3. [Tree Terminology](#tree-terminology)
4. [Binary Trees](#binary-trees)
5. [Binary Search Tree (BST)](#binary-search-tree-bst)
6. [Tree Traversals](#tree-traversals)
7. [Balanced Trees](#balanced-trees)
8. [Tree Algorithms](#tree-algorithms)
9. [Time and Space Complexity](#time-and-space-complexity)
10. [Best Practices](#best-practices)
11. [Interview Problems](#interview-problems)
12. [Applications](#applications)
13. [Examples](#examples)

---

## Introduction

A **tree** is a hierarchical, non-linear data structure consisting of nodes connected by edges. Trees are one of the most fundamental data structures in computer science, representing hierarchical relationships between data.

### Why Use Trees?

- **Hierarchical Structure**: Natural representation of hierarchical data
- **Efficient Search**: O(log n) search in balanced trees
- **Efficient Insertion/Deletion**: O(log n) in balanced trees
- **Sorted Data**: BSTs keep data in sorted order
- **Natural Recursion**: Tree operations are naturally recursive

### Trees vs Linear Data Structures

| Feature | Tree | Array/List |
|---------|------|------------|
| Structure | Hierarchical | Linear |
| Access | O(log n) in BST | O(1) for arrays, O(n) for lists |
| Search | O(log n) in balanced BST | O(n) unsorted, O(log n) sorted array |
| Insertion | O(log n) in balanced BST | O(1) at end, O(n) elsewhere |
| Memory | Overhead for pointers | Contiguous or linked |

---

## What is a Tree?

A **tree** is a collection of nodes where:
1. One node is designated as the **root**
2. Every node (except root) has exactly one parent
3. Nodes can have zero or more children
4. There are no cycles (acyclic)

### Visual Representation

```
         1          <- Root
       /   \
      2     3       <- Internal nodes
     / \     \
    4   5     6     <- Leaf nodes
```

### Characteristics

- **Hierarchical**: Parent-child relationships
- **Acyclic**: No cycles or loops
- **Connected**: Path exists between any two nodes
- **Recursive**: Each subtree is also a tree

---

## Tree Terminology

### Essential Terms

1. **Root**: The topmost node with no parent
2. **Parent**: A node that has children
3. **Child**: A node descended from another node
4. **Leaf (External Node)**: A node with no children
5. **Internal Node**: A node with at least one child
6. **Sibling**: Nodes with the same parent
7. **Ancestor**: Any node on the path from root to a node
8. **Descendant**: Any node reachable by going down from a node

### Measurements

1. **Depth of a Node**: Number of edges from root to the node
2. **Height of a Node**: Number of edges on longest path from node to a leaf
3. **Height of Tree**: Height of the root node
4. **Level**: Depth + 1 (root is at level 1 or 0 depending on convention)
5. **Degree**: Number of children a node has
6. **Edge**: Connection between two nodes

### Example with Measurements

```
Level 0:       1          (depth=0, height=2)
             /   \
Level 1:    2     3       (depth=1, height=1)
           / \     \
Level 2:  4   5     6     (depth=2, height=0, leaves)

Tree Height: 2
Root: 1
Leaves: 4, 5, 6
Internal nodes: 1, 2, 3
```

---

## Binary Trees

A **binary tree** is a tree where each node has **at most two children**, typically referred to as the **left child** and **right child**.

### Node Structure

```klang
class TreeNode {
    let data = null
    let left = null
    let right = null
    
    fn init(value) {
        this.data = value
        this.left = null
        this.right = null
    }
}
```

### Types of Binary Trees

#### 1. Full Binary Tree
Every node has 0 or 2 children (no node has only 1 child).

```
       1
     /   \
    2     3
   / \
  4   5
```

#### 2. Complete Binary Tree
All levels are fully filled except possibly the last, which is filled from left to right.

```
       1
     /   \
    2     3
   / \   /
  4   5 6
```

#### 3. Perfect Binary Tree
All internal nodes have 2 children and all leaves are at the same level.

```
       1
     /   \
    2     3
   / \   / \
  4   5 6   7
```

#### 4. Balanced Binary Tree
Height of left and right subtrees differ by at most 1 for all nodes.

```
       1
     /   \
    2     3
   / \
  4   5
```

#### 5. Degenerate (Pathological) Tree
Each parent has only one child (essentially a linked list).

```
1
 \
  2
   \
    3
     \
      4
```

### Binary Tree Implementation

```klang
class BinaryTree {
    let root = null
    
    fn init() {
        this.root = null
    }
    
    fn isEmpty() {
        return this.root == null
    }
    
    fn getRoot() {
        return this.root
    }
}
```

---

## Binary Search Tree (BST)

A **Binary Search Tree** is a binary tree with the ordering property:
- **Left subtree** contains only nodes with values **less than** the parent node
- **Right subtree** contains only nodes with values **greater than** the parent node
- Both left and right subtrees are also BSTs

### BST Property

```
       8
     /   \
    3     10
   / \      \
  1   6      14
     / \    /
    4   7  13

For node 8: All left values (1,3,4,6,7) < 8 < All right values (10,13,14)
```

### BST Implementation

```klang
class BST {
    let root = null
    
    fn init() {
        this.root = null
    }
    
    // Insert a value into the BST
    fn insert(value) {
        this.root = this.insertRec(this.root, value)
    }
    
    fn insertRec(node, value) {
        // Base case: empty position found
        if (node == null) {
            return TreeNode(value)
        }
        
        // Recursive case: traverse to find position
        if (value < node.data) {
            node.left = this.insertRec(node.left, value)
        } else if (value > node.data) {
            node.right = this.insertRec(node.right, value)
        }
        // If value equals node.data, don't insert duplicates
        
        return node
    }
    
    // Search for a value in the BST
    fn search(value) {
        return this.searchRec(this.root, value)
    }
    
    fn searchRec(node, value) {
        // Base cases: not found or found
        if (node == null) {
            return false
        }
        if (node.data == value) {
            return true
        }
        
        // Recursive search in appropriate subtree
        if (value < node.data) {
            return this.searchRec(node.left, value)
        } else {
            return this.searchRec(node.right, value)
        }
    }
    
    // Delete a value from the BST
    fn delete(value) {
        this.root = this.deleteRec(this.root, value)
    }
    
    fn deleteRec(node, value) {
        if (node == null) {
            return null
        }
        
        // Find the node to delete
        if (value < node.data) {
            node.left = this.deleteRec(node.left, value)
        } else if (value > node.data) {
            node.right = this.deleteRec(node.right, value)
        } else {
            // Node found - handle three cases
            
            // Case 1: Node with no children (leaf)
            if (node.left == null && node.right == null) {
                return null
            }
            
            // Case 2: Node with one child
            if (node.left == null) {
                return node.right
            }
            if (node.right == null) {
                return node.left
            }
            
            // Case 3: Node with two children
            // Find inorder successor (minimum in right subtree)
            let successor = this.findMin(node.right)
            node.data = successor.data
            node.right = this.deleteRec(node.right, successor.data)
        }
        
        return node
    }
    
    fn findMin(node) {
        while (node.left != null) {
            node = node.left
        }
        return node
    }
    
    fn findMax(node) {
        while (node.right != null) {
            node = node.right
        }
        return node
    }
}
```

---

## Tree Traversals

Tree traversal is the process of visiting each node in the tree exactly once in a specific order.

### 1. Inorder Traversal (Left, Root, Right)

Visits nodes in **ascending order** for BST.

```klang
fn inorder(node) {
    if (node != null) {
        this.inorder(node.left)      // Visit left subtree
        print(node.data)              // Visit root
        this.inorder(node.right)      // Visit right subtree
    }
}
```

**Example**:
```
Tree:      4
         /   \
        2     6
       / \   / \
      1   3 5   7

Inorder: 1, 2, 3, 4, 5, 6, 7
```

**Use Cases**: Get sorted order from BST, expression evaluation

### 2. Preorder Traversal (Root, Left, Right)

Visits root before children.

```klang
fn preorder(node) {
    if (node != null) {
        print(node.data)              // Visit root
        this.preorder(node.left)      // Visit left subtree
        this.preorder(node.right)     // Visit right subtree
    }
}
```

**Example**:
```
Tree:      4
         /   \
        2     6
       / \   / \
      1   3 5   7

Preorder: 4, 2, 1, 3, 6, 5, 7
```

**Use Cases**: Copy tree, prefix expression, tree serialization

### 3. Postorder Traversal (Left, Right, Root)

Visits root after children.

```klang
fn postorder(node) {
    if (node != null) {
        this.postorder(node.left)     // Visit left subtree
        this.postorder(node.right)    // Visit right subtree
        print(node.data)              // Visit root
    }
}
```

**Example**:
```
Tree:      4
         /   \
        2     6
       / \   / \
      1   3 5   7

Postorder: 1, 3, 2, 5, 7, 6, 4
```

**Use Cases**: Delete tree, postfix expression, calculate directory size

### 4. Level-Order Traversal (Breadth-First Search)

Visits nodes level by level from left to right.

```klang
fn levelOrder(root) {
    if (root == null) {
        return
    }
    
    let queue = Queue()
    queue.enqueue(root)
    
    while (!queue.isEmpty()) {
        let node = queue.dequeue()
        print(node.data)
        
        if (node.left != null) {
            queue.enqueue(node.left)
        }
        if (node.right != null) {
            queue.enqueue(node.right)
        }
    }
}
```

**Example**:
```
Tree:      4
         /   \
        2     6
       / \   / \
      1   3 5   7

Level-order: 4, 2, 6, 1, 3, 5, 7
```

**Use Cases**: Find shortest path, level-wise operations, tree width

### Traversal Comparison

```klang
// Example tree:
//       1
//      / \
//     2   3
//    / \
//   4   5

// Inorder:    4, 2, 5, 1, 3
// Preorder:   1, 2, 4, 5, 3
// Postorder:  4, 5, 2, 3, 1
// Level-order: 1, 2, 3, 4, 5
```

---

## Balanced Trees

A **balanced tree** maintains a small height relative to the number of nodes, ensuring O(log n) operations.

### AVL Tree Concept

An **AVL tree** (Adelson-Velsky and Landis) is a self-balancing BST where:
- Heights of left and right subtrees differ by at most 1
- Balance is maintained through rotations after insertions/deletions

### Balance Factor

```
Balance Factor = Height(Left Subtree) - Height(Right Subtree)
```

For AVL tree: Balance Factor ∈ {-1, 0, 1}

### AVL Rotations

#### 1. Left Rotation (LL)
```
    x                y
     \              / \
      y     →      x   z
       \
        z
```

#### 2. Right Rotation (RR)
```
      z            y
     /            / \
    y      →     x   z
   /
  x
```

#### 3. Left-Right Rotation (LR)
```
    z            z           y
   /            /           / \
  x      →     y     →     x   z
   \          /
    y        x
```

#### 4. Right-Left Rotation (RL)
```
  x          x             y
   \          \           / \
    z    →     y    →    x   z
   /            \
  y              z
```

### Simple AVL Node

```klang
class AVLNode {
    let data = null
    let left = null
    let right = null
    let height = 1
    
    fn init(value) {
        this.data = value
        this.left = null
        this.right = null
        this.height = 1
    }
}
```

---

## Tree Algorithms

### 1. Calculate Height

```klang
fn height(node) {
    if (node == null) {
        return -1  // or 0, depending on definition
    }
    
    let leftHeight = this.height(node.left)
    let rightHeight = this.height(node.right)
    
    return 1 + max(leftHeight, rightHeight)
}
```

### 2. Count Nodes

```klang
fn countNodes(node) {
    if (node == null) {
        return 0
    }
    
    return 1 + this.countNodes(node.left) + this.countNodes(node.right)
}
```

### 3. Count Leaf Nodes

```klang
fn countLeaves(node) {
    if (node == null) {
        return 0
    }
    
    if (node.left == null && node.right == null) {
        return 1  // This is a leaf
    }
    
    return this.countLeaves(node.left) + this.countLeaves(node.right)
}
```

### 4. Find Minimum Value

```klang
fn findMinValue(node) {
    if (node == null) {
        return null
    }
    
    // In BST, minimum is leftmost node
    while (node.left != null) {
        node = node.left
    }
    
    return node.data
}
```

### 5. Find Maximum Value

```klang
fn findMaxValue(node) {
    if (node == null) {
        return null
    }
    
    // In BST, maximum is rightmost node
    while (node.right != null) {
        node = node.right
    }
    
    return node.data
}
```

### 6. Validate BST

```klang
fn isValidBST(node, min, max) {
    if (node == null) {
        return true
    }
    
    // Check if current node violates BST property
    if (node.data <= min || node.data >= max) {
        return false
    }
    
    // Recursively check left and right subtrees
    return this.isValidBST(node.left, min, node.data) &&
           this.isValidBST(node.right, node.data, max)
}

// Usage: isValidBST(root, -Infinity, Infinity)
```

### 7. Lowest Common Ancestor (BST)

```klang
fn lowestCommonAncestorBST(node, p, q) {
    if (node == null) {
        return null
    }
    
    // If both p and q are smaller, LCA is in left subtree
    if (p < node.data && q < node.data) {
        return this.lowestCommonAncestorBST(node.left, p, q)
    }
    
    // If both p and q are greater, LCA is in right subtree
    if (p > node.data && q > node.data) {
        return this.lowestCommonAncestorBST(node.right, p, q)
    }
    
    // Otherwise, current node is LCA
    return node
}
```

### 8. Lowest Common Ancestor (Binary Tree)

```klang
fn lowestCommonAncestor(node, p, q) {
    if (node == null || node == p || node == q) {
        return node
    }
    
    let left = this.lowestCommonAncestor(node.left, p, q)
    let right = this.lowestCommonAncestor(node.right, p, q)
    
    // If both left and right are non-null, current node is LCA
    if (left != null && right != null) {
        return node
    }
    
    // Otherwise return non-null child
    return left != null ? left : right
}
```

### 9. Check if Trees are Identical

```klang
fn areIdentical(node1, node2) {
    // Both empty
    if (node1 == null && node2 == null) {
        return true
    }
    
    // One empty, one not
    if (node1 == null || node2 == null) {
        return false
    }
    
    // Check current nodes and recurse
    return node1.data == node2.data &&
           this.areIdentical(node1.left, node2.left) &&
           this.areIdentical(node1.right, node2.right)
}
```

### 10. Mirror/Invert Tree

```klang
fn invertTree(node) {
    if (node == null) {
        return null
    }
    
    // Swap left and right children
    let temp = node.left
    node.left = node.right
    node.right = temp
    
    // Recursively invert subtrees
    this.invertTree(node.left)
    this.invertTree(node.right)
    
    return node
}
```

---

## Time and Space Complexity

### Binary Search Tree Operations

| Operation | Average Case | Worst Case (Unbalanced) | Balanced Tree |
|-----------|--------------|-------------------------|---------------|
| Search | O(log n) | O(n) | O(log n) |
| Insert | O(log n) | O(n) | O(log n) |
| Delete | O(log n) | O(n) | O(log n) |
| Find Min/Max | O(log n) | O(n) | O(log n) |
| Traversal | O(n) | O(n) | O(n) |

### Traversal Complexities

| Traversal | Time Complexity | Space Complexity |
|-----------|-----------------|------------------|
| Inorder | O(n) | O(h) recursion stack |
| Preorder | O(n) | O(h) recursion stack |
| Postorder | O(n) | O(h) recursion stack |
| Level-order | O(n) | O(w) queue, w = max width |

Where:
- n = number of nodes
- h = height of tree
- w = maximum width of tree

### Space Complexity Factors

```klang
// Recursion uses call stack
fn recursive(node) {
    if (node == null) return
    recursive(node.left)   // Stack depth = height
    recursive(node.right)
}

// Iteration uses explicit queue/stack
fn iterative(root) {
    let queue = Queue()
    queue.enqueue(root)    // Space = max nodes at a level
    // ...
}
```

---

## Best Practices

### 1. Always Check for Null

```klang
fn safeOperation(node) {
    if (node == null) {
        return  // or appropriate default value
    }
    // Proceed with operation
}
```

### 2. Use Recursion for Tree Operations

```klang
// Recursion is natural for tree structure
fn processTree(node) {
    if (node == null) return
    
    processTree(node.left)   // Left subtree
    // Process current node
    processTree(node.right)  // Right subtree
}
```

### 3. Maintain BST Property

```klang
fn insert(node, value) {
    if (value < node.data) {
        // Insert in left subtree (smaller values)
        node.left = this.insert(node.left, value)
    } else if (value > node.data) {
        // Insert in right subtree (larger values)
        node.right = this.insert(node.right, value)
    }
    return node
}
```

### 4. Consider Balanced Trees for Production

```klang
// For guaranteed O(log n) operations, use:
// - AVL Tree
// - Red-Black Tree
// - B-Tree (for databases)
```

### 5. Choose Appropriate Traversal

```klang
// Inorder: For sorted output from BST
// Preorder: For copying/serializing tree
// Postorder: For deleting tree, calculating size
// Level-order: For shortest path, level operations
```

### 6. Handle Edge Cases

```klang
fn robustOperation(root) {
    // Empty tree
    if (root == null) {
        return null
    }
    
    // Single node
    if (root.left == null && root.right == null) {
        return root.data
    }
    
    // Normal case
    // ...
}
```

### 7. Use Helper Functions

```klang
class BST {
    fn search(value) {
        return this.searchHelper(this.root, value)
    }
    
    fn searchHelper(node, value) {
        // Actual implementation
    }
}
```

### 8. Validate Input

```klang
fn insert(value) {
    if (value == null) {
        throw "Cannot insert null value"
    }
    this.root = this.insertRec(this.root, value)
}
```

---

## Interview Problems

### Problem 1: Maximum Depth of Binary Tree

**Description**: Find the maximum depth (height) of a binary tree.

**Solution**:
```klang
fn maxDepth(root) {
    if (root == null) {
        return 0
    }
    
    let leftDepth = this.maxDepth(root.left)
    let rightDepth = this.maxDepth(root.right)
    
    return 1 + max(leftDepth, rightDepth)
}

// Time: O(n), Space: O(h)
```

### Problem 2: Validate Binary Search Tree

**Description**: Determine if a binary tree is a valid BST.

**Solution**:
```klang
fn isValidBST(root) {
    return this.validate(root, null, null)
}

fn validate(node, min, max) {
    if (node == null) {
        return true
    }
    
    // Check current node value bounds
    if ((min != null && node.data <= min) || 
        (max != null && node.data >= max)) {
        return false
    }
    
    // Validate subtrees with updated bounds
    return this.validate(node.left, min, node.data) &&
           this.validate(node.right, node.data, max)
}

// Time: O(n), Space: O(h)
```

### Problem 3: Invert Binary Tree

**Description**: Invert a binary tree (mirror it).

**Solution**:
```klang
fn invertTree(root) {
    if (root == null) {
        return null
    }
    
    // Swap children
    let temp = root.left
    root.left = root.right
    root.right = temp
    
    // Recursively invert subtrees
    this.invertTree(root.left)
    this.invertTree(root.right)
    
    return root
}

// Time: O(n), Space: O(h)
```

### Problem 4: Symmetric Tree

**Description**: Check if a binary tree is symmetric around its center.

**Solution**:
```klang
fn isSymmetric(root) {
    if (root == null) {
        return true
    }
    return this.isMirror(root.left, root.right)
}

fn isMirror(left, right) {
    // Both null - symmetric
    if (left == null && right == null) {
        return true
    }
    
    // One null - not symmetric
    if (left == null || right == null) {
        return false
    }
    
    // Check values and recurse
    return left.data == right.data &&
           this.isMirror(left.left, right.right) &&
           this.isMirror(left.right, right.left)
}

// Time: O(n), Space: O(h)
```

### Problem 5: Binary Tree Level Order Traversal

**Description**: Return level-order traversal as array of arrays (one per level).

**Solution**:
```klang
fn levelOrder(root) {
    let result = []
    if (root == null) {
        return result
    }
    
    let queue = Queue()
    queue.enqueue(root)
    
    while (!queue.isEmpty()) {
        let levelSize = queue.size()
        let currentLevel = []
        
        for (let i = 0; i < levelSize; i = i + 1) {
            let node = queue.dequeue()
            currentLevel.push(node.data)
            
            if (node.left != null) {
                queue.enqueue(node.left)
            }
            if (node.right != null) {
                queue.enqueue(node.right)
            }
        }
        
        result.push(currentLevel)
    }
    
    return result
}

// Time: O(n), Space: O(w) where w is max width
```

### Problem 6: Lowest Common Ancestor

**Description**: Find the lowest common ancestor of two nodes in a BST.

**Solution**:
```klang
fn lowestCommonAncestor(root, p, q) {
    if (root == null) {
        return null
    }
    
    // Both in left subtree
    if (p.data < root.data && q.data < root.data) {
        return this.lowestCommonAncestor(root.left, p, q)
    }
    
    // Both in right subtree
    if (p.data > root.data && q.data > root.data) {
        return this.lowestCommonAncestor(root.right, p, q)
    }
    
    // Split point - current node is LCA
    return root
}

// Time: O(h), Space: O(h)
```

### Problem 7: Binary Tree Paths

**Description**: Return all root-to-leaf paths in a binary tree.

**Solution**:
```klang
fn binaryTreePaths(root) {
    let result = []
    if (root == null) {
        return result
    }
    
    this.findPaths(root, "", result)
    return result
}

fn findPaths(node, path, result) {
    if (node == null) {
        return
    }
    
    // Add current node to path
    path = path + toString(node.data)
    
    // Leaf node - add path to result
    if (node.left == null && node.right == null) {
        result.push(path)
        return
    }
    
    // Continue building path
    path = path + "->"
    this.findPaths(node.left, path, result)
    this.findPaths(node.right, path, result)
}

// Time: O(n), Space: O(n)
```

### Problem 8: Same Tree

**Description**: Check if two binary trees are identical.

**Solution**:
```klang
fn isSameTree(p, q) {
    // Both null
    if (p == null && q == null) {
        return true
    }
    
    // One null, one not
    if (p == null || q == null) {
        return false
    }
    
    // Compare values and recurse
    return p.data == q.data &&
           this.isSameTree(p.left, q.left) &&
           this.isSameTree(p.right, q.right)
}

// Time: O(min(n, m)), Space: O(min(h1, h2))
```

### Problem 9: Balanced Binary Tree

**Description**: Check if a binary tree is height-balanced (height difference ≤ 1).

**Solution**:
```klang
fn isBalanced(root) {
    return this.checkBalance(root) != -1
}

fn checkBalance(node) {
    if (node == null) {
        return 0
    }
    
    let leftHeight = this.checkBalance(node.left)
    if (leftHeight == -1) {
        return -1  // Left subtree unbalanced
    }
    
    let rightHeight = this.checkBalance(node.right)
    if (rightHeight == -1) {
        return -1  // Right subtree unbalanced
    }
    
    // Check current node balance
    if (abs(leftHeight - rightHeight) > 1) {
        return -1  // Unbalanced
    }
    
    return 1 + max(leftHeight, rightHeight)
}

// Time: O(n), Space: O(h)
```

### Problem 10: Minimum Depth

**Description**: Find the minimum depth from root to nearest leaf.

**Solution**:
```klang
fn minDepth(root) {
    if (root == null) {
        return 0
    }
    
    // Leaf node
    if (root.left == null && root.right == null) {
        return 1
    }
    
    // Only left subtree exists
    if (root.left != null && root.right == null) {
        return 1 + this.minDepth(root.left)
    }
    
    // Only right subtree exists
    if (root.left == null && root.right != null) {
        return 1 + this.minDepth(root.right)
    }
    
    // Both subtrees exist
    return 1 + min(this.minDepth(root.left), this.minDepth(root.right))
}

// Time: O(n), Space: O(h)
```

### Problem 11: Path Sum

**Description**: Check if there exists a root-to-leaf path with a given sum.

**Solution**:
```klang
fn hasPathSum(root, targetSum) {
    if (root == null) {
        return false
    }
    
    // Leaf node - check if sum matches
    if (root.left == null && root.right == null) {
        return root.data == targetSum
    }
    
    // Recurse with reduced target
    let remainingSum = targetSum - root.data
    return this.hasPathSum(root.left, remainingSum) ||
           this.hasPathSum(root.right, remainingSum)
}

// Time: O(n), Space: O(h)
```

### Problem 12: Count Complete Tree Nodes

**Description**: Count nodes in a complete binary tree efficiently.

**Solution**:
```klang
fn countNodes(root) {
    if (root == null) {
        return 0
    }
    
    let leftHeight = this.getHeight(root.left)
    let rightHeight = this.getHeight(root.right)
    
    if (leftHeight == rightHeight) {
        // Left subtree is perfect, count and check right
        return (1 << leftHeight) + this.countNodes(root.right)
    } else {
        // Right subtree is perfect, count and check left
        return (1 << rightHeight) + this.countNodes(root.left)
    }
}

fn getHeight(node) {
    let height = 0
    while (node != null) {
        height = height + 1
        node = node.left
    }
    return height
}

// Time: O(log²n), Space: O(log n)
```

---

## Applications

### 1. File Systems

Directory structures are represented as trees.

```klang
class FileNode {
    let name = ""
    let isDirectory = false
    let children = []  // Subdirectories/files
    
    fn init(name, isDir) {
        this.name = name
        this.isDirectory = isDir
        this.children = []
    }
}

// Example:
// root/
//   ├── home/
//   │   ├── user/
//   │   └── docs/
//   └── var/
```

### 2. Document Object Model (DOM)

HTML documents are tree structures.

```klang
// HTML:
// <html>
//   <body>
//     <div>
//       <p>Text</p>
//     </div>
//   </body>
// </html>

// Represented as tree with html as root
```

### 3. Database Indexing

B-trees and B+ trees for efficient database queries.

```klang
// B-Tree properties:
// - Multiple keys per node
// - Keeps data sorted
// - O(log n) search, insert, delete
// - Used in: MySQL, PostgreSQL, SQLite
```

### 4. Expression Parsing

Mathematical expressions as binary trees.

```klang
// Expression: (3 + 5) * (8 - 2)
//       *
//      / \
//     +   -
//    / \ / \
//   3  5 8  2
```

### 5. Autocomplete and Spell Check

Trie (prefix tree) for efficient string operations.

```klang
// Dictionary tree for fast prefix matching
// Used in: Search engines, IDEs, spell checkers
```

### 6. Routing Tables

IP routing uses tree structures.

```klang
// Network routing decisions
// Hierarchical IP address matching
```

### 7. Game Trees

Decision trees in game AI (chess, tic-tac-toe).

```klang
// Each node represents a game state
// Children are possible moves
// Used in: Minimax algorithm
```

### 8. Syntax Trees

Compiler abstract syntax trees (AST).

```klang
// Source code parsed into tree structure
// Used for: Compilation, code analysis
```

### 9. Organization Charts

Corporate hierarchies.

```klang
// CEO at root
// Managers as internal nodes
// Employees as leaves
```

### 10. XML/JSON Parsing

Structured data representation.

```klang
// Nested data structures
// Parent-child relationships
```

---

## Examples

### Example 1: Build and Use BST

```klang
class TreeNode {
    let data = null
    let left = null
    let right = null
    
    fn init(value) {
        this.data = value
        this.left = null
        this.right = null
    }
}

class BST {
    let root = null
    
    fn init() {
        this.root = null
    }
    
    fn insert(value) {
        this.root = this.insertRec(this.root, value)
    }
    
    fn insertRec(node, value) {
        if (node == null) {
            return TreeNode(value)
        }
        
        if (value < node.data) {
            node.left = this.insertRec(node.left, value)
        } else if (value > node.data) {
            node.right = this.insertRec(node.right, value)
        }
        
        return node
    }
    
    fn inorder(node) {
        if (node != null) {
            this.inorder(node.left)
            print(node.data)
            this.inorder(node.right)
        }
    }
}

// Usage
let bst = BST()
bst.insert(50)
bst.insert(30)
bst.insert(70)
bst.insert(20)
bst.insert(40)
bst.insert(60)
bst.insert(80)

print("Inorder traversal:")
bst.inorder(bst.root)
// Output: 20, 30, 40, 50, 60, 70, 80
```

### Example 2: Level Order Traversal

```klang
fn levelOrderTraversal(root) {
    if (root == null) {
        return
    }
    
    let queue = []
    queue.push(root)
    
    while (queue.length > 0) {
        let node = queue.shift()  // Dequeue
        print(node.data)
        
        if (node.left != null) {
            queue.push(node.left)
        }
        if (node.right != null) {
            queue.push(node.right)
        }
    }
}

// Usage
levelOrderTraversal(bst.root)
// Output: 50, 30, 70, 20, 40, 60, 80
```

### Example 3: Find Path to Node

```klang
fn findPath(root, target, path) {
    if (root == null) {
        return false
    }
    
    // Add current node to path
    path.push(root.data)
    
    // Target found
    if (root.data == target) {
        return true
    }
    
    // Search in left and right subtrees
    if (this.findPath(root.left, target, path) ||
        this.findPath(root.right, target, path)) {
        return true
    }
    
    // Not found in this path - backtrack
    path.pop()
    return false
}

// Usage
let path = []
if (findPath(bst.root, 40, path)) {
    print("Path to 40:", path)
    // Output: [50, 30, 40]
}
```

### Example 4: Diameter of Tree

```klang
class Solution {
    let diameter = 0
    
    fn diameterOfBinaryTree(root) {
        this.diameter = 0
        this.height(root)
        return this.diameter
    }
    
    fn height(node) {
        if (node == null) {
            return 0
        }
        
        let left = this.height(node.left)
        let right = this.height(node.right)
        
        // Update diameter if path through this node is longer
        this.diameter = max(this.diameter, left + right)
        
        return 1 + max(left, right)
    }
}

// Usage
let sol = Solution()
let diameter = sol.diameterOfBinaryTree(bst.root)
print("Diameter:", diameter)
```

### Example 5: Serialize and Deserialize Tree

```klang
fn serialize(root) {
    if (root == null) {
        return "null,"
    }
    
    return toString(root.data) + "," +
           this.serialize(root.left) +
           this.serialize(root.right)
}

fn deserialize(data) {
    let values = data.split(",")
    let index = 0
    return this.deserializeHelper(values, index)
}

fn deserializeHelper(values, index) {
    if (values[index] == "null") {
        index = index + 1
        return null
    }
    
    let node = TreeNode(parseInt(values[index]))
    index = index + 1
    
    node.left = this.deserializeHelper(values, index)
    node.right = this.deserializeHelper(values, index)
    
    return node
}

// Usage
let serialized = serialize(bst.root)
print("Serialized:", serialized)

let newTree = deserialize(serialized)
print("Deserialized and inorder:")
bst.inorder(newTree)
```

---

## Summary

Trees are fundamental hierarchical data structures with numerous applications:

- **Binary Search Trees** provide O(log n) operations when balanced
- **Tree Traversals** offer different ways to visit nodes
- **Balanced Trees** (AVL, Red-Black) guarantee optimal performance
- **Common Algorithms** include height, validation, LCA, and more
- **Applications** span file systems, databases, compilers, and beyond

**Key Takeaways**:
1. Trees naturally represent hierarchical relationships
2. BST property enables efficient search
3. Balance is crucial for optimal performance
4. Recursion simplifies tree operations
5. Different traversals serve different purposes
6. Understanding trees is essential for interviews and real-world systems

**Next Steps**:
- Practice implementing tree operations
- Solve interview problems regularly
- Study advanced trees (AVL, Red-Black, B-Trees)
- Explore tree applications in real systems
- Learn graph algorithms (trees are special graphs)

---

*For more DSA documentation, see [KLang DSA Documentation](../README.md)*
