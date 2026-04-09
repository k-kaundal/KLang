# Graphs in KLang - Data Structures & Algorithms

## Table of Contents
1. [Introduction](#introduction)
2. [What is a Graph?](#what-is-a-graph)
3. [Graph Terminology](#graph-terminology)
4. [Types of Graphs](#types-of-graphs)
5. [Graph Representations](#graph-representations)
6. [Graph Traversals](#graph-traversals)
7. [Graph Operations](#graph-operations)
8. [Graph Algorithms](#graph-algorithms)
9. [Time and Space Complexity](#time-and-space-complexity)
10. [Best Practices](#best-practices)
11. [Interview Problems](#interview-problems)
12. [Applications](#applications)
13. [Examples](#examples)

---

## Introduction

A **graph** is a non-linear data structure consisting of vertices (nodes) connected by edges (links). Graphs are fundamental in computer science for modeling relationships and connections between entities.

### Why Use Graphs?

- **Model Relationships**: Natural representation of connections and networks
- **Versatile Structure**: Can represent various real-world scenarios
- **Powerful Algorithms**: Shortest path, connectivity, flow problems
- **Network Analysis**: Social networks, transportation, communication
- **Dependency Management**: Task scheduling, compilation order

### Graphs vs Other Data Structures

| Feature | Graph | Tree | Array/List |
|---------|-------|------|------------|
| Structure | Network | Hierarchical | Linear |
| Cycles | Can have cycles | No cycles | N/A |
| Connections | Many-to-many | One-to-many | One-to-one |
| Root | No root | Single root | No root |
| Relationships | Arbitrary | Parent-child | Sequential |

---

## What is a Graph?

A **graph** G = (V, E) consists of:
1. **V**: A set of vertices (nodes)
2. **E**: A set of edges connecting pairs of vertices

### Visual Representation

#### Undirected Graph
```
    A --- B
    |     |
    |     |
    C --- D

Edges: {A-B, A-C, B-D, C-D}
```

#### Directed Graph (Digraph)
```
    A --> B
    ↑     ↓
    |     |
    C <-- D

Edges: {A→B, B→D, D→C, C→A}
```

#### Weighted Graph
```
      5
    A --- B
  2 |     | 3
    |     |
    C --- D
      4

Edges: {A-B:5, A-C:2, B-D:3, C-D:4}
```

### Characteristics

- **Vertices**: Entities or nodes in the graph
- **Edges**: Connections between vertices
- **Flexible**: Can represent symmetric or asymmetric relationships
- **Complex**: Can model intricate network structures

---

## Graph Terminology

### Essential Terms

1. **Vertex (Node)**: A fundamental unit of a graph
2. **Edge (Link/Arc)**: A connection between two vertices
3. **Adjacent Vertices**: Vertices connected by an edge
4. **Neighbors**: All vertices adjacent to a given vertex
5. **Degree**: Number of edges connected to a vertex
   - **In-degree**: Number of incoming edges (directed graphs)
   - **Out-degree**: Number of outgoing edges (directed graphs)
6. **Path**: Sequence of vertices connected by edges
7. **Cycle**: Path that starts and ends at the same vertex
8. **Connected Graph**: Path exists between any two vertices
9. **Disconnected Graph**: Some vertices are unreachable
10. **Component**: Maximal connected subgraph

### Advanced Terms

1. **Simple Graph**: No self-loops or multiple edges between same vertices
2. **Multigraph**: Multiple edges allowed between same vertices
3. **Complete Graph**: Every vertex connected to every other vertex
4. **Bipartite Graph**: Vertices can be divided into two disjoint sets
5. **Sparse Graph**: Relatively few edges (E << V²)
6. **Dense Graph**: Many edges (E ≈ V²)
7. **Subgraph**: Subset of vertices and edges from a graph
8. **Spanning Tree**: Acyclic subgraph connecting all vertices

### Example with Terms

```
Graph G:
    1 --- 2
    |     | \
    |     |  5
    3 --- 4 /

Vertices: {1, 2, 3, 4, 5}
Edges: {1-2, 1-3, 2-4, 2-5, 3-4, 4-5}

Degree(1) = 2  (connected to 2, 3)
Degree(2) = 3  (connected to 1, 4, 5)
Degree(4) = 3  (connected to 2, 3, 5)

Path from 1 to 5: 1 → 2 → 5
Cycle: 1 → 2 → 4 → 3 → 1
```

---

## Types of Graphs

### 1. Directed vs Undirected

#### Undirected Graph
- Edges have no direction
- If A connects to B, then B connects to A
- Represents symmetric relationships

```
A --- B    (A connected to B, B connected to A)
```

#### Directed Graph (Digraph)
- Edges have direction
- A → B doesn't imply B → A
- Represents asymmetric relationships

```
A --> B    (A points to B, but B doesn't point to A)
```

### 2. Weighted vs Unweighted

#### Unweighted Graph
- Edges have no associated values
- All connections are equal

```
A --- B --- C
```

#### Weighted Graph
- Edges have associated weights/costs
- Represents distance, cost, capacity, etc.

```
  10     20
A --- B --- C
```

### 3. Cyclic vs Acyclic

#### Cyclic Graph
- Contains at least one cycle
- Path exists from a vertex back to itself

```
A --> B
^     |
|     v
D <-- C    (Cycle: A → B → C → D → A)
```

#### Acyclic Graph
- No cycles exist
- **DAG**: Directed Acyclic Graph (special case)

```
A --> B --> D
|           ^
v           |
C ----------+    (No cycle exists)
```

### 4. Connected vs Disconnected

#### Connected Graph
- Path exists between any two vertices

```
A --- B --- C
|           |
D ----------+
```

#### Disconnected Graph
- Some vertices are unreachable from others

```
A --- B     D --- E

(A-B is one component, D-E is another)
```

### 5. Special Graph Types

#### Complete Graph (K_n)
- Every vertex connected to every other vertex
- n vertices have n(n-1)/2 edges

```
    A
   /|\
  / | \
 B--+--C
  \ | /
   \|/
    D

Complete graph K_4 (4 vertices, 6 edges)
```

#### Bipartite Graph
- Vertices can be divided into two disjoint sets
- Edges only between sets, not within

```
Set 1:  A   B        Set 2:  X   Y   Z
        |\ /|                |\  /|
        | X |                | \/ |
        |/ \|                | /\ |
        C   D                |/  \|
```

---

## Graph Representations

### 1. Adjacency Matrix

A 2D array where matrix[i][j] indicates edge from vertex i to vertex j.

#### Characteristics
- **Space**: O(V²)
- **Check edge exists**: O(1)
- **Add/remove edge**: O(1)
- **Find neighbors**: O(V)
- **Best for**: Dense graphs

#### Example

```
Graph:          Adjacency Matrix:
  0 --- 1           0  1  2  3
  |     |        0 [0, 1, 1, 0]
  2 --- 3        1 [1, 0, 0, 1]
                 2 [1, 0, 0, 1]
                 3 [0, 1, 1, 0]

matrix[0][1] = 1 means edge from 0 to 1
matrix[0][3] = 0 means no edge from 0 to 3
```

#### Implementation

```klang
class GraphMatrix {
    var vertices
    var matrix
    
    fn init(numVertices) {
        this.vertices = numVertices
        this.matrix = []
        
        // Initialize matrix with zeros
        for (let i = 0; i < numVertices; i = i + 1) {
            let row = []
            for (let j = 0; j < numVertices; j = j + 1) {
                row.push(0)
            }
            this.matrix.push(row)
        }
    }
    
    // Add edge (undirected)
    fn addEdge(src, dest) {
        if (src >= 0 && src < this.vertices && dest >= 0 && dest < this.vertices) {
            this.matrix[src][dest] = 1
            this.matrix[dest][src] = 1  // For undirected
        }
    }
    
    // Add directed edge
    fn addDirectedEdge(src, dest) {
        if (src >= 0 && src < this.vertices && dest >= 0 && dest < this.vertices) {
            this.matrix[src][dest] = 1
        }
    }
    
    // Add weighted edge
    fn addWeightedEdge(src, dest, weight) {
        if (src >= 0 && src < this.vertices && dest >= 0 && dest < this.vertices) {
            this.matrix[src][dest] = weight
            this.matrix[dest][src] = weight  // For undirected
        }
    }
    
    // Remove edge
    fn removeEdge(src, dest) {
        if (src >= 0 && src < this.vertices && dest >= 0 && dest < this.vertices) {
            this.matrix[src][dest] = 0
            this.matrix[dest][src] = 0  // For undirected
        }
    }
    
    // Check if edge exists
    fn hasEdge(src, dest) {
        if (src >= 0 && src < this.vertices && dest >= 0 && dest < this.vertices) {
            return this.matrix[src][dest] != 0
        }
        return false
    }
    
    // Get neighbors of a vertex
    fn getNeighbors(vertex) {
        let neighbors = []
        if (vertex >= 0 && vertex < this.vertices) {
            for (let i = 0; i < this.vertices; i = i + 1) {
                if (this.matrix[vertex][i] != 0) {
                    neighbors.push(i)
                }
            }
        }
        return neighbors
    }
    
    // Print graph
    fn print() {
        print("Adjacency Matrix:")
        for (let i = 0; i < this.vertices; i = i + 1) {
            let row = ""
            for (let j = 0; j < this.vertices; j = j + 1) {
                row = row + this.matrix[i][j] + " "
            }
            print(row)
        }
    }
}
```

### 2. Adjacency List

An array of lists where each index represents a vertex and contains a list of adjacent vertices.

#### Characteristics
- **Space**: O(V + E)
- **Check edge exists**: O(degree(V))
- **Add edge**: O(1)
- **Remove edge**: O(degree(V))
- **Find neighbors**: O(degree(V))
- **Best for**: Sparse graphs

#### Example

```
Graph:          Adjacency List:
  0 --- 1       0: [1, 2]
  |     |       1: [0, 3]
  2 --- 3       2: [0, 3]
                3: [1, 2]
```

#### Implementation

```klang
class GraphNode {
    var vertex
    var weight  // For weighted graphs
    
    fn init(v, w) {
        this.vertex = v
        this.weight = w
    }
}

class GraphList {
    var vertices
    var adjList
    
    fn init(numVertices) {
        this.vertices = numVertices
        this.adjList = []
        
        // Initialize adjacency list
        for (let i = 0; i < numVertices; i = i + 1) {
            this.adjList.push([])
        }
    }
    
    // Add edge (undirected)
    fn addEdge(src, dest) {
        if (src >= 0 && src < this.vertices && dest >= 0 && dest < this.vertices) {
            this.adjList[src].push(dest)
            this.adjList[dest].push(src)  // For undirected
        }
    }
    
    // Add directed edge
    fn addDirectedEdge(src, dest) {
        if (src >= 0 && src < this.vertices && dest >= 0 && dest < this.vertices) {
            this.adjList[src].push(dest)
        }
    }
    
    // Add weighted edge
    fn addWeightedEdge(src, dest, weight) {
        if (src >= 0 && src < this.vertices && dest >= 0 && dest < this.vertices) {
            let node1 = GraphNode(dest, weight)
            let node2 = GraphNode(src, weight)
            this.adjList[src].push(node1)
            this.adjList[dest].push(node2)  // For undirected
        }
    }
    
    // Remove edge
    fn removeEdge(src, dest) {
        if (src >= 0 && src < this.vertices && dest >= 0 && dest < this.vertices) {
            // Remove dest from src's list
            let newList1 = []
            for (let i = 0; i < this.adjList[src].length; i = i + 1) {
                if (this.adjList[src][i] != dest) {
                    newList1.push(this.adjList[src][i])
                }
            }
            this.adjList[src] = newList1
            
            // Remove src from dest's list (for undirected)
            let newList2 = []
            for (let i = 0; i < this.adjList[dest].length; i = i + 1) {
                if (this.adjList[dest][i] != src) {
                    newList2.push(this.adjList[dest][i])
                }
            }
            this.adjList[dest] = newList2
        }
    }
    
    // Check if edge exists
    fn hasEdge(src, dest) {
        if (src >= 0 && src < this.vertices && dest >= 0 && dest < this.vertices) {
            for (let i = 0; i < this.adjList[src].length; i = i + 1) {
                if (this.adjList[src][i] == dest) {
                    return true
                }
            }
        }
        return false
    }
    
    // Get neighbors of a vertex
    fn getNeighbors(vertex) {
        if (vertex >= 0 && vertex < this.vertices) {
            return this.adjList[vertex]
        }
        return []
    }
    
    // Get degree of a vertex
    fn getDegree(vertex) {
        if (vertex >= 0 && vertex < this.vertices) {
            return this.adjList[vertex].length
        }
        return 0
    }
    
    // Print graph
    fn print() {
        print("Adjacency List:")
        for (let i = 0; i < this.vertices; i = i + 1) {
            let list = i + ": "
            for (let j = 0; j < this.adjList[i].length; j = j + 1) {
                list = list + this.adjList[i][j] + " "
            }
            print(list)
        }
    }
}
```

### 3. Edge List

A list of all edges in the graph.

#### Characteristics
- **Space**: O(E)
- **Check edge exists**: O(E)
- **Add edge**: O(1)
- **Best for**: Algorithms that process edges sequentially

#### Example

```
Graph:          Edge List:
  0 --- 1       [(0, 1), (0, 2), (1, 3), (2, 3)]
  |     |       
  2 --- 3       
```

#### Implementation

```klang
class Edge {
    var src
    var dest
    var weight
    
    fn init(s, d, w) {
        this.src = s
        this.dest = d
        this.weight = w
    }
}

class GraphEdgeList {
    var vertices
    var edges
    
    fn init(numVertices) {
        this.vertices = numVertices
        this.edges = []
    }
    
    // Add edge
    fn addEdge(src, dest, weight) {
        let edge = Edge(src, dest, weight)
        this.edges.push(edge)
    }
    
    // Get all edges
    fn getEdges() {
        return this.edges
    }
    
    // Print graph
    fn print() {
        print("Edge List:")
        for (let i = 0; i < this.edges.length; i = i + 1) {
            let e = this.edges[i]
            print(e.src + " -> " + e.dest + " (weight: " + e.weight + ")")
        }
    }
}
```

### Comparison of Representations

| Operation | Adjacency Matrix | Adjacency List | Edge List |
|-----------|------------------|----------------|-----------|
| Space | O(V²) | O(V + E) | O(E) |
| Add vertex | O(V²) | O(1) | O(1) |
| Add edge | O(1) | O(1) | O(1) |
| Remove edge | O(1) | O(V) | O(E) |
| Check edge | O(1) | O(V) | O(E) |
| Find neighbors | O(V) | O(1) | O(E) |
| Space for sparse | Poor | Good | Best |
| Space for dense | Good | Poor | Poor |

**Best Choice:**
- **Dense graphs**: Adjacency Matrix
- **Sparse graphs**: Adjacency List
- **Edge-centric algorithms**: Edge List

---

## Graph Traversals

Graph traversals visit all vertices in a systematic order. Two main approaches:

1. **Breadth-First Search (BFS)**: Visit all neighbors before going deeper
2. **Depth-First Search (DFS)**: Visit as deep as possible before backtracking

### 1. Breadth-First Search (BFS)

BFS explores the graph level by level, visiting all vertices at distance k before visiting vertices at distance k+1.

#### Algorithm
1. Start at source vertex
2. Visit all neighbors (distance 1)
3. Visit all neighbors' neighbors (distance 2)
4. Continue until all reachable vertices visited

#### Visualization

```
Graph:              BFS from 0:
    0               Level 0: [0]
   / \              Level 1: [1, 2]
  1   2             Level 2: [3, 4]
 / \
3   4

Visit order: 0 → 1 → 2 → 3 → 4
```

#### Properties
- **Data Structure**: Queue (FIFO)
- **Time Complexity**: O(V + E)
- **Space Complexity**: O(V)
- **Finds**: Shortest path in unweighted graph
- **Order**: Level-order traversal

#### Implementation

```klang
class Queue {
    var items
    
    fn init() {
        this.items = []
    }
    
    fn enqueue(item) {
        this.items.push(item)
    }
    
    fn dequeue() {
        if (this.isEmpty()) {
            return null
        }
        let first = this.items[0]
        let newItems = []
        for (let i = 1; i < this.items.length; i = i + 1) {
            newItems.push(this.items[i])
        }
        this.items = newItems
        return first
    }
    
    fn isEmpty() {
        return this.items.length == 0
    }
}

class BFSGraph {
    var vertices
    var adjList
    
    fn init(numVertices) {
        this.vertices = numVertices
        this.adjList = []
        for (let i = 0; i < numVertices; i = i + 1) {
            this.adjList.push([])
        }
    }
    
    fn addEdge(src, dest) {
        this.adjList[src].push(dest)
        this.adjList[dest].push(src)  // For undirected
    }
    
    // BFS traversal from a given source
    fn bfs(start) {
        // Track visited vertices
        let visited = []
        for (let i = 0; i < this.vertices; i = i + 1) {
            visited.push(false)
        }
        
        // Result array
        let result = []
        
        // Create queue and enqueue start vertex
        let queue = Queue()
        visited[start] = true
        queue.enqueue(start)
        
        while (!queue.isEmpty()) {
            // Dequeue a vertex
            let vertex = queue.dequeue()
            result.push(vertex)
            
            // Get all adjacent vertices
            let neighbors = this.adjList[vertex]
            for (let i = 0; i < neighbors.length; i = i + 1) {
                let neighbor = neighbors[i]
                if (!visited[neighbor]) {
                    visited[neighbor] = true
                    queue.enqueue(neighbor)
                }
            }
        }
        
        return result
    }
    
    // BFS to find shortest path
    fn shortestPath(start, end) {
        let visited = []
        let parent = []
        for (let i = 0; i < this.vertices; i = i + 1) {
            visited.push(false)
            parent.push(-1)
        }
        
        let queue = Queue()
        visited[start] = true
        queue.enqueue(start)
        
        while (!queue.isEmpty()) {
            let vertex = queue.dequeue()
            
            if (vertex == end) {
                // Reconstruct path
                let path = []
                let current = end
                while (current != -1) {
                    path.push(current)
                    current = parent[current]
                }
                // Reverse path
                let reversedPath = []
                for (let i = path.length - 1; i >= 0; i = i - 1) {
                    reversedPath.push(path[i])
                }
                return reversedPath
            }
            
            let neighbors = this.adjList[vertex]
            for (let i = 0; i < neighbors.length; i = i + 1) {
                let neighbor = neighbors[i]
                if (!visited[neighbor]) {
                    visited[neighbor] = true
                    parent[neighbor] = vertex
                    queue.enqueue(neighbor)
                }
            }
        }
        
        return []  // No path found
    }
    
    // BFS to find distance
    fn findDistance(start, end) {
        let visited = []
        let distance = []
        for (let i = 0; i < this.vertices; i = i + 1) {
            visited.push(false)
            distance.push(0)
        }
        
        let queue = Queue()
        visited[start] = true
        distance[start] = 0
        queue.enqueue(start)
        
        while (!queue.isEmpty()) {
            let vertex = queue.dequeue()
            
            if (vertex == end) {
                return distance[end]
            }
            
            let neighbors = this.adjList[vertex]
            for (let i = 0; i < neighbors.length; i = i + 1) {
                let neighbor = neighbors[i]
                if (!visited[neighbor]) {
                    visited[neighbor] = true
                    distance[neighbor] = distance[vertex] + 1
                    queue.enqueue(neighbor)
                }
            }
        }
        
        return -1  // Not reachable
    }
}
```

### 2. Depth-First Search (DFS)

DFS explores the graph by going as deep as possible along each branch before backtracking.

#### Algorithm
1. Start at source vertex
2. Visit an unvisited neighbor
3. Recursively visit that neighbor's unvisited neighbors
4. Backtrack when no unvisited neighbors remain

#### Visualization

```
Graph:              DFS from 0:
    0               Visit 0
   / \              → Visit 1 (go deep)
  1   2             → Visit 3 (go deeper)
 / \                → Backtrack to 1
3   4               → Visit 4
                    → Backtrack to 0
                    → Visit 2

Visit order: 0 → 1 → 3 → 4 → 2
```

#### Properties
- **Data Structure**: Stack (LIFO) or Recursion
- **Time Complexity**: O(V + E)
- **Space Complexity**: O(V)
- **Finds**: Path existence, cycles, components
- **Order**: Depth-first traversal

#### Recursive Implementation

```klang
class DFSGraph {
    var vertices
    var adjList
    
    fn init(numVertices) {
        this.vertices = numVertices
        this.adjList = []
        for (let i = 0; i < numVertices; i = i + 1) {
            this.adjList.push([])
        }
    }
    
    fn addEdge(src, dest) {
        this.adjList[src].push(dest)
        this.adjList[dest].push(src)  // For undirected
    }
    
    // DFS helper function
    fn dfsUtil(vertex, visited, result) {
        // Mark current vertex as visited
        visited[vertex] = true
        result.push(vertex)
        
        // Recurse for all adjacent vertices
        let neighbors = this.adjList[vertex]
        for (let i = 0; i < neighbors.length; i = i + 1) {
            let neighbor = neighbors[i]
            if (!visited[neighbor]) {
                this.dfsUtil(neighbor, visited, result)
            }
        }
    }
    
    // DFS traversal from a given source
    fn dfs(start) {
        let visited = []
        for (let i = 0; i < this.vertices; i = i + 1) {
            visited.push(false)
        }
        
        let result = []
        this.dfsUtil(start, visited, result)
        return result
    }
    
    // DFS traversal for disconnected graph
    fn dfsAll() {
        let visited = []
        for (let i = 0; i < this.vertices; i = i + 1) {
            visited.push(false)
        }
        
        let result = []
        for (let i = 0; i < this.vertices; i = i + 1) {
            if (!visited[i]) {
                this.dfsUtil(i, visited, result)
            }
        }
        return result
    }
}
```

#### Iterative Implementation (Using Stack)

```klang
class Stack {
    var items
    
    fn init() {
        this.items = []
    }
    
    fn push(item) {
        this.items.push(item)
    }
    
    fn pop() {
        if (this.isEmpty()) {
            return null
        }
        let last = this.items[this.items.length - 1]
        let newItems = []
        for (let i = 0; i < this.items.length - 1; i = i + 1) {
            newItems.push(this.items[i])
        }
        this.items = newItems
        return last
    }
    
    fn isEmpty() {
        return this.items.length == 0
    }
}

fn dfsIterative(graph, start) {
    let visited = []
    for (let i = 0; i < graph.vertices; i = i + 1) {
        visited.push(false)
    }
    
    let result = []
    let stack = Stack()
    
    stack.push(start)
    
    while (!stack.isEmpty()) {
        let vertex = stack.pop()
        
        if (!visited[vertex]) {
            visited[vertex] = true
            result.push(vertex)
            
            // Push all neighbors to stack
            let neighbors = graph.adjList[vertex]
            // Push in reverse order to match recursive DFS order
            for (let i = neighbors.length - 1; i >= 0; i = i - 1) {
                if (!visited[neighbors[i]]) {
                    stack.push(neighbors[i])
                }
            }
        }
    }
    
    return result
}
```

### BFS vs DFS Comparison

| Feature | BFS | DFS |
|---------|-----|-----|
| Data Structure | Queue | Stack/Recursion |
| Order | Level-order | Depth-first |
| Shortest Path | ✓ (unweighted) | ✗ |
| Space | O(V) | O(V) |
| Time | O(V + E) | O(V + E) |
| Use Case | Shortest path, level-order | Topological sort, cycle detection |
| Memory Usage | Higher (stores all at level) | Lower (stores path) |

---

## Graph Operations

### Basic Operations

#### 1. Add Vertex

```klang
fn addVertex(graph) {
    graph.vertices = graph.vertices + 1
    graph.adjList.push([])
    return graph.vertices - 1
}
```

#### 2. Add Edge

```klang
// Undirected
fn addEdge(graph, src, dest) {
    if (src >= 0 && src < graph.vertices && dest >= 0 && dest < graph.vertices) {
        graph.adjList[src].push(dest)
        graph.adjList[dest].push(src)
    }
}

// Directed
fn addDirectedEdge(graph, src, dest) {
    if (src >= 0 && src < graph.vertices && dest >= 0 && dest < graph.vertices) {
        graph.adjList[src].push(dest)
    }
}
```

#### 3. Remove Edge

```klang
fn removeEdge(graph, src, dest) {
    // Remove dest from src's adjacency list
    let newList1 = []
    for (let i = 0; i < graph.adjList[src].length; i = i + 1) {
        if (graph.adjList[src][i] != dest) {
            newList1.push(graph.adjList[src][i])
        }
    }
    graph.adjList[src] = newList1
    
    // Remove src from dest's adjacency list (for undirected)
    let newList2 = []
    for (let i = 0; i < graph.adjList[dest].length; i = i + 1) {
        if (graph.adjList[dest][i] != src) {
            newList2.push(graph.adjList[dest][i])
        }
    }
    graph.adjList[dest] = newList2
}
```

#### 4. Remove Vertex

```klang
fn removeVertex(graph, vertex) {
    if (vertex < 0 || vertex >= graph.vertices) {
        return
    }
    
    // Remove all edges to this vertex
    for (let i = 0; i < graph.vertices; i = i + 1) {
        removeEdge(graph, i, vertex)
    }
    
    // Clear the vertex's adjacency list
    graph.adjList[vertex] = []
}
```

#### 5. Check if Edge Exists

```klang
fn hasEdge(graph, src, dest) {
    if (src < 0 || src >= graph.vertices) {
        return false
    }
    
    let neighbors = graph.adjList[src]
    for (let i = 0; i < neighbors.length; i = i + 1) {
        if (neighbors[i] == dest) {
            return true
        }
    }
    return false
}
```

#### 6. Get Neighbors

```klang
fn getNeighbors(graph, vertex) {
    if (vertex >= 0 && vertex < graph.vertices) {
        return graph.adjList[vertex]
    }
    return []
}
```

#### 7. Get In-Degree (Directed Graph)

```klang
fn getInDegree(graph, vertex) {
    let inDegree = 0
    for (let i = 0; i < graph.vertices; i = i + 1) {
        if (hasEdge(graph, i, vertex)) {
            inDegree = inDegree + 1
        }
    }
    return inDegree
}
```

#### 8. Get Out-Degree (Directed Graph)

```klang
fn getOutDegree(graph, vertex) {
    if (vertex >= 0 && vertex < graph.vertices) {
        return graph.adjList[vertex].length
    }
    return 0
}
```

---

## Graph Algorithms

### 1. Check if Path Exists

```klang
fn hasPath(graph, start, end) {
    if (start == end) {
        return true
    }
    
    let visited = []
    for (let i = 0; i < graph.vertices; i = i + 1) {
        visited.push(false)
    }
    
    let queue = Queue()
    visited[start] = true
    queue.enqueue(start)
    
    while (!queue.isEmpty()) {
        let vertex = queue.dequeue()
        
        let neighbors = graph.adjList[vertex]
        for (let i = 0; i < neighbors.length; i = i + 1) {
            let neighbor = neighbors[i]
            
            if (neighbor == end) {
                return true
            }
            
            if (!visited[neighbor]) {
                visited[neighbor] = true
                queue.enqueue(neighbor)
            }
        }
    }
    
    return false
}
```

### 2. Detect Cycle in Undirected Graph

```klang
fn hasCycleUtil(graph, vertex, visited, parent) {
    visited[vertex] = true
    
    let neighbors = graph.adjList[vertex]
    for (let i = 0; i < neighbors.length; i = i + 1) {
        let neighbor = neighbors[i]
        
        if (!visited[neighbor]) {
            if (hasCycleUtil(graph, neighbor, visited, vertex)) {
                return true
            }
        } else if (neighbor != parent) {
            // Visited neighbor that's not parent means cycle
            return true
        }
    }
    
    return false
}

fn hasCycle(graph) {
    let visited = []
    for (let i = 0; i < graph.vertices; i = i + 1) {
        visited.push(false)
    }
    
    // Check all components
    for (let i = 0; i < graph.vertices; i = i + 1) {
        if (!visited[i]) {
            if (hasCycleUtil(graph, i, visited, -1)) {
                return true
            }
        }
    }
    
    return false
}
```

### 3. Detect Cycle in Directed Graph

```klang
fn hasCycleDirectedUtil(graph, vertex, visited, recStack) {
    visited[vertex] = true
    recStack[vertex] = true
    
    let neighbors = graph.adjList[vertex]
    for (let i = 0; i < neighbors.length; i = i + 1) {
        let neighbor = neighbors[i]
        
        if (!visited[neighbor]) {
            if (hasCycleDirectedUtil(graph, neighbor, visited, recStack)) {
                return true
            }
        } else if (recStack[neighbor]) {
            // Back edge to a vertex in recursion stack
            return true
        }
    }
    
    recStack[vertex] = false
    return false
}

fn hasCycleDirected(graph) {
    let visited = []
    let recStack = []
    for (let i = 0; i < graph.vertices; i = i + 1) {
        visited.push(false)
        recStack.push(false)
    }
    
    for (let i = 0; i < graph.vertices; i = i + 1) {
        if (!visited[i]) {
            if (hasCycleDirectedUtil(graph, i, visited, recStack)) {
                return true
            }
        }
    }
    
    return false
}
```

### 4. Find Connected Components

```klang
fn findConnectedComponents(graph) {
    let visited = []
    for (let i = 0; i < graph.vertices; i = i + 1) {
        visited.push(false)
    }
    
    let components = []
    
    for (let i = 0; i < graph.vertices; i = i + 1) {
        if (!visited[i]) {
            let component = []
            dfsComponent(graph, i, visited, component)
            components.push(component)
        }
    }
    
    return components
}

fn dfsComponent(graph, vertex, visited, component) {
    visited[vertex] = true
    component.push(vertex)
    
    let neighbors = graph.adjList[vertex]
    for (let i = 0; i < neighbors.length; i = i + 1) {
        let neighbor = neighbors[i]
        if (!visited[neighbor]) {
            dfsComponent(graph, neighbor, visited, component)
        }
    }
}
```

### 5. Topological Sort (DAG)

```klang
fn topologicalSortUtil(graph, vertex, visited, stack) {
    visited[vertex] = true
    
    let neighbors = graph.adjList[vertex]
    for (let i = 0; i < neighbors.length; i = i + 1) {
        let neighbor = neighbors[i]
        if (!visited[neighbor]) {
            topologicalSortUtil(graph, neighbor, visited, stack)
        }
    }
    
    stack.push(vertex)
}

fn topologicalSort(graph) {
    let visited = []
    for (let i = 0; i < graph.vertices; i = i + 1) {
        visited.push(false)
    }
    
    let stack = Stack()
    
    for (let i = 0; i < graph.vertices; i = i + 1) {
        if (!visited[i]) {
            topologicalSortUtil(graph, i, visited, stack)
        }
    }
    
    // Pop all elements from stack
    let result = []
    while (!stack.isEmpty()) {
        result.push(stack.pop())
    }
    
    return result
}
```

### 6. Check if Graph is Bipartite

```klang
fn isBipartite(graph) {
    let colors = []
    for (let i = 0; i < graph.vertices; i = i + 1) {
        colors.push(-1)  // -1 means uncolored
    }
    
    // Check all components
    for (let i = 0; i < graph.vertices; i = i + 1) {
        if (colors[i] == -1) {
            if (!isBipartiteUtil(graph, i, colors)) {
                return false
            }
        }
    }
    
    return true
}

fn isBipartiteUtil(graph, start, colors) {
    let queue = Queue()
    colors[start] = 0
    queue.enqueue(start)
    
    while (!queue.isEmpty()) {
        let vertex = queue.dequeue()
        
        let neighbors = graph.adjList[vertex]
        for (let i = 0; i < neighbors.length; i = i + 1) {
            let neighbor = neighbors[i]
            
            if (colors[neighbor] == -1) {
                // Color with opposite color
                colors[neighbor] = 1 - colors[vertex]
                queue.enqueue(neighbor)
            } else if (colors[neighbor] == colors[vertex]) {
                // Same color as parent - not bipartite
                return false
            }
        }
    }
    
    return true
}
```

---

## Time and Space Complexity

### Graph Representations

| Operation | Adjacency Matrix | Adjacency List |
|-----------|------------------|----------------|
| Space | O(V²) | O(V + E) |
| Add vertex | O(V²) | O(1) |
| Add edge | O(1) | O(1) |
| Remove vertex | O(V²) | O(V + E) |
| Remove edge | O(1) | O(V) |
| Check edge | O(1) | O(V) |
| Find neighbors | O(V) | O(1) |

### Graph Traversals

| Algorithm | Time | Space | Notes |
|-----------|------|-------|-------|
| BFS | O(V + E) | O(V) | Queue + visited array |
| DFS (recursive) | O(V + E) | O(V) | Recursion stack |
| DFS (iterative) | O(V + E) | O(V) | Explicit stack |

### Graph Algorithms

| Algorithm | Time | Space | Notes |
|-----------|------|-------|-------|
| Path exists | O(V + E) | O(V) | BFS or DFS |
| Shortest path (unweighted) | O(V + E) | O(V) | BFS |
| Cycle detection | O(V + E) | O(V) | DFS |
| Connected components | O(V + E) | O(V) | DFS |
| Topological sort | O(V + E) | O(V) | DFS + stack |
| Bipartite check | O(V + E) | O(V) | BFS coloring |

---

## Best Practices

### Choosing Graph Representation

1. **Use Adjacency List when:**
   - Graph is sparse (E << V²)
   - Need to iterate over neighbors frequently
   - Memory is a concern
   - Most practical applications

2. **Use Adjacency Matrix when:**
   - Graph is dense (E ≈ V²)
   - Need O(1) edge lookups
   - Simple implementation preferred
   - Graph size is small

3. **Use Edge List when:**
   - Processing edges sequentially
   - Kruskal's algorithm (MST)
   - Bellman-Ford algorithm
   - Simple edge operations

### Traversal Selection

1. **Use BFS when:**
   - Finding shortest path (unweighted)
   - Level-order traversal needed
   - Closest nodes matter
   - Web crawling

2. **Use DFS when:**
   - Topological sorting
   - Cycle detection
   - Path finding (any path)
   - Maze solving

### Implementation Tips

1. **Always track visited vertices** to avoid infinite loops
2. **Check for disconnected components** when needed
3. **Use appropriate data structures** (Queue for BFS, Stack for DFS)
4. **Consider directed vs undirected** when adding edges
5. **Initialize data structures properly** (visited arrays, etc.)
6. **Handle edge cases**: empty graph, single vertex, disconnected

### Performance Optimization

1. **Choose right representation** based on graph density
2. **Use visited set/array** to avoid redundant work
3. **Early termination** when target found
4. **Bidirectional search** for shortest path
5. **Adjacency list for sparse graphs** saves space

---

## Interview Problems

### 1. Number of Islands (Medium)

**Problem**: Count the number of islands in a 2D grid where '1' is land and '0' is water.

```klang
fn numIslands(grid) {
    if (grid.length == 0) {
        return 0
    }
    
    let rows = grid.length
    let cols = grid[0].length
    let count = 0
    
    for (let i = 0; i < rows; i = i + 1) {
        for (let j = 0; j < cols; j = j + 1) {
            if (grid[i][j] == "1") {
                count = count + 1
                dfsIsland(grid, i, j)
            }
        }
    }
    
    return count
}

fn dfsIsland(grid, row, col) {
    let rows = grid.length
    let cols = grid[0].length
    
    // Boundary check and water check
    if (row < 0 || row >= rows || col < 0 || col >= cols || grid[row][col] == "0") {
        return
    }
    
    // Mark as visited
    grid[row][col] = "0"
    
    // Visit all 4 directions
    dfsIsland(grid, row + 1, col)  // Down
    dfsIsland(grid, row - 1, col)  // Up
    dfsIsland(grid, row, col + 1)  // Right
    dfsIsland(grid, row, col - 1)  // Left
}
```

### 2. Clone Graph (Medium)

**Problem**: Create a deep copy of an undirected graph.

```klang
class GraphNode {
    var val
    var neighbors
    
    fn init(v) {
        this.val = v
        this.neighbors = []
    }
}

fn cloneGraph(node) {
    if (node == null) {
        return null
    }
    
    let visited = {}  // Map from original to clone
    return cloneHelper(node, visited)
}

fn cloneHelper(node, visited) {
    if (node == null) {
        return null
    }
    
    // Check if already cloned
    if (visited[node.val] != null) {
        return visited[node.val]
    }
    
    // Create clone
    let clone = GraphNode(node.val)
    visited[node.val] = clone
    
    // Clone neighbors
    for (let i = 0; i < node.neighbors.length; i = i + 1) {
        let neighbor = node.neighbors[i]
        clone.neighbors.push(cloneHelper(neighbor, visited))
    }
    
    return clone
}
```

### 3. Course Schedule (Medium)

**Problem**: Determine if you can finish all courses given prerequisites (cycle detection).

```klang
fn canFinish(numCourses, prerequisites) {
    // Build adjacency list
    let graph = []
    for (let i = 0; i < numCourses; i = i + 1) {
        graph.push([])
    }
    
    for (let i = 0; i < prerequisites.length; i = i + 1) {
        let course = prerequisites[i][0]
        let prereq = prerequisites[i][1]
        graph[prereq].push(course)
    }
    
    // Check for cycle
    let visited = []
    let recStack = []
    for (let i = 0; i < numCourses; i = i + 1) {
        visited.push(false)
        recStack.push(false)
    }
    
    for (let i = 0; i < numCourses; i = i + 1) {
        if (!visited[i]) {
            if (hasCycleCourse(graph, i, visited, recStack)) {
                return false  // Cycle found
            }
        }
    }
    
    return true  // No cycle, can finish
}

fn hasCycleCourse(graph, course, visited, recStack) {
    visited[course] = true
    recStack[course] = true
    
    let neighbors = graph[course]
    for (let i = 0; i < neighbors.length; i = i + 1) {
        let neighbor = neighbors[i]
        
        if (!visited[neighbor]) {
            if (hasCycleCourse(graph, neighbor, visited, recStack)) {
                return true
            }
        } else if (recStack[neighbor]) {
            return true
        }
    }
    
    recStack[course] = false
    return false
}
```

### 4. Pacific Atlantic Water Flow (Medium)

**Problem**: Find cells where water can flow to both Pacific and Atlantic oceans.

```klang
fn pacificAtlantic(heights) {
    if (heights.length == 0) {
        return []
    }
    
    let rows = heights.length
    let cols = heights[0].length
    
    let pacific = []
    let atlantic = []
    for (let i = 0; i < rows; i = i + 1) {
        let pacRow = []
        let atlRow = []
        for (let j = 0; j < cols; j = j + 1) {
            pacRow.push(false)
            atlRow.push(false)
        }
        pacific.push(pacRow)
        atlantic.push(atlRow)
    }
    
    // DFS from Pacific edges (top and left)
    for (let i = 0; i < rows; i = i + 1) {
        dfsWater(heights, pacific, i, 0)  // Left edge
        dfsWater(heights, atlantic, i, cols - 1)  // Right edge
    }
    for (let j = 0; j < cols; j = j + 1) {
        dfsWater(heights, pacific, 0, j)  // Top edge
        dfsWater(heights, atlantic, rows - 1, j)  // Bottom edge
    }
    
    // Find cells reachable by both
    let result = []
    for (let i = 0; i < rows; i = i + 1) {
        for (let j = 0; j < cols; j = j + 1) {
            if (pacific[i][j] && atlantic[i][j]) {
                result.push([i, j])
            }
        }
    }
    
    return result
}

fn dfsWater(heights, visited, row, col) {
    let rows = heights.length
    let cols = heights[0].length
    
    visited[row][col] = true
    
    let directions = [[0, 1], [1, 0], [0, -1], [-1, 0]]
    for (let i = 0; i < 4; i = i + 1) {
        let newRow = row + directions[i][0]
        let newCol = col + directions[i][1]
        
        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols &&
            !visited[newRow][newCol] && heights[newRow][newCol] >= heights[row][col]) {
            dfsWater(heights, visited, newRow, newCol)
        }
    }
}
```

### 5. Graph Valid Tree (Medium)

**Problem**: Check if graph forms a valid tree (connected and acyclic).

```klang
fn validTree(n, edges) {
    // Tree must have exactly n-1 edges
    if (edges.length != n - 1) {
        return false
    }
    
    // Build adjacency list
    let graph = []
    for (let i = 0; i < n; i = i + 1) {
        graph.push([])
    }
    
    for (let i = 0; i < edges.length; i = i + 1) {
        let u = edges[i][0]
        let v = edges[i][1]
        graph[u].push(v)
        graph[v].push(u)
    }
    
    // Check if connected using BFS
    let visited = []
    for (let i = 0; i < n; i = i + 1) {
        visited.push(false)
    }
    
    let queue = Queue()
    visited[0] = true
    queue.enqueue(0)
    let count = 1
    
    while (!queue.isEmpty()) {
        let node = queue.dequeue()
        
        let neighbors = graph[node]
        for (let i = 0; i < neighbors.length; i = i + 1) {
            let neighbor = neighbors[i]
            if (!visited[neighbor]) {
                visited[neighbor] = true
                queue.enqueue(neighbor)
                count = count + 1
            }
        }
    }
    
    return count == n  // All nodes visited means connected
}
```

### 6. Word Ladder (Hard)

**Problem**: Find shortest transformation sequence from beginWord to endWord.

```klang
fn ladderLength(beginWord, endWord, wordList) {
    // Create set for O(1) lookup
    let wordSet = {}
    let found = false
    for (let i = 0; i < wordList.length; i = i + 1) {
        wordSet[wordList[i]] = true
        if (wordList[i] == endWord) {
            found = true
        }
    }
    
    if (!found) {
        return 0
    }
    
    let queue = Queue()
    queue.enqueue([beginWord, 1])
    
    while (!queue.isEmpty()) {
        let current = queue.dequeue()
        let word = current[0]
        let level = current[1]
        
        if (word == endWord) {
            return level
        }
        
        // Try all possible one-letter changes
        for (let i = 0; i < word.length; i = i + 1) {
            let chars = word.split("")
            
            for (let c = 97; c <= 122; c = c + 1) {  // 'a' to 'z'
                chars[i] = String.fromCharCode(c)
                let newWord = chars.join("")
                
                if (wordSet[newWord]) {
                    queue.enqueue([newWord, level + 1])
                    wordSet[newWord] = false  // Mark as visited
                }
            }
        }
    }
    
    return 0
}
```

### 7. Surrounded Regions (Medium)

**Problem**: Capture all regions surrounded by 'X'.

```klang
fn solve(board) {
    if (board.length == 0) {
        return
    }
    
    let rows = board.length
    let cols = board[0].length
    
    // Mark all 'O's connected to border
    for (let i = 0; i < rows; i = i + 1) {
        if (board[i][0] == "O") {
            markBorder(board, i, 0)
        }
        if (board[i][cols - 1] == "O") {
            markBorder(board, i, cols - 1)
        }
    }
    for (let j = 0; j < cols; j = j + 1) {
        if (board[0][j] == "O") {
            markBorder(board, 0, j)
        }
        if (board[rows - 1][j] == "O") {
            markBorder(board, rows - 1, j)
        }
    }
    
    // Flip remaining 'O' to 'X' and marked to 'O'
    for (let i = 0; i < rows; i = i + 1) {
        for (let j = 0; j < cols; j = j + 1) {
            if (board[i][j] == "O") {
                board[i][j] = "X"
            } else if (board[i][j] == "M") {
                board[i][j] = "O"
            }
        }
    }
}

fn markBorder(board, row, col) {
    let rows = board.length
    let cols = board[0].length
    
    if (row < 0 || row >= rows || col < 0 || col >= cols || board[row][col] != "O") {
        return
    }
    
    board[row][col] = "M"  // Mark
    
    markBorder(board, row + 1, col)
    markBorder(board, row - 1, col)
    markBorder(board, row, col + 1)
    markBorder(board, row, col - 1)
}
```

### 8. Network Delay Time (Medium)

**Problem**: Find time for all nodes to receive signal (Dijkstra's algorithm preview).

```klang
fn networkDelayTime(times, n, k) {
    // Build adjacency list with weights
    let graph = []
    for (let i = 0; i <= n; i = i + 1) {
        graph.push([])
    }
    
    for (let i = 0; i < times.length; i = i + 1) {
        let u = times[i][0]
        let v = times[i][1]
        let w = times[i][2]
        graph[u].push([v, w])
    }
    
    // Distance array
    let dist = []
    for (let i = 0; i <= n; i = i + 1) {
        dist.push(999999)  // Infinity
    }
    dist[k] = 0
    
    // BFS with priority (simplified - use priority queue for optimal)
    let visited = []
    for (let i = 0; i <= n; i = i + 1) {
        visited.push(false)
    }
    
    for (let i = 0; i < n; i = i + 1) {
        // Find min unvisited
        let minDist = 999999
        let u = -1
        for (let j = 1; j <= n; j = j + 1) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j]
                u = j
            }
        }
        
        if (u == -1) {
            break
        }
        
        visited[u] = true
        
        // Update distances
        let neighbors = graph[u]
        for (let j = 0; j < neighbors.length; j = j + 1) {
            let v = neighbors[j][0]
            let w = neighbors[j][1]
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w
            }
        }
    }
    
    // Find max distance
    let maxDist = 0
    for (let i = 1; i <= n; i = i + 1) {
        if (dist[i] == 999999) {
            return -1  // Some node unreachable
        }
        if (dist[i] > maxDist) {
            maxDist = dist[i]
        }
    }
    
    return maxDist
}
```

### 9. Minimum Height Trees (Medium)

**Problem**: Find roots of minimum height trees.

```klang
fn findMinHeightTrees(n, edges) {
    if (n == 1) {
        return [0]
    }
    
    // Build adjacency list
    let graph = []
    let degree = []
    for (let i = 0; i < n; i = i + 1) {
        graph.push([])
        degree.push(0)
    }
    
    for (let i = 0; i < edges.length; i = i + 1) {
        let u = edges[i][0]
        let v = edges[i][1]
        graph[u].push(v)
        graph[v].push(u)
        degree[u] = degree[u] + 1
        degree[v] = degree[v] + 1
    }
    
    // Find all leaves (degree 1)
    let leaves = []
    for (let i = 0; i < n; i = i + 1) {
        if (degree[i] == 1) {
            leaves.push(i)
        }
    }
    
    // Remove leaves layer by layer
    let remaining = n
    while (remaining > 2) {
        remaining = remaining - leaves.length
        let newLeaves = []
        
        for (let i = 0; i < leaves.length; i = i + 1) {
            let leaf = leaves[i]
            let neighbors = graph[leaf]
            
            for (let j = 0; j < neighbors.length; j = j + 1) {
                let neighbor = neighbors[j]
                degree[neighbor] = degree[neighbor] - 1
                if (degree[neighbor] == 1) {
                    newLeaves.push(neighbor)
                }
            }
        }
        
        leaves = newLeaves
    }
    
    return leaves
}
```

### 10. Evaluate Division (Medium)

**Problem**: Evaluate division equations (graph with weighted edges).

```klang
fn calcEquation(equations, values, queries) {
    // Build graph with division values
    let graph = {}
    
    for (let i = 0; i < equations.length; i = i + 1) {
        let a = equations[i][0]
        let b = equations[i][1]
        let value = values[i]
        
        if (graph[a] == null) {
            graph[a] = {}
        }
        if (graph[b] == null) {
            graph[b] = {}
        }
        
        graph[a][b] = value
        graph[b][a] = 1.0 / value
    }
    
    // Process each query
    let results = []
    for (let i = 0; i < queries.length; i = i + 1) {
        let start = queries[i][0]
        let end = queries[i][1]
        
        if (graph[start] == null || graph[end] == null) {
            results.push(-1.0)
        } else if (start == end) {
            results.push(1.0)
        } else {
            let visited = {}
            let result = dfsEvaluate(graph, start, end, 1.0, visited)
            results.push(result)
        }
    }
    
    return results
}

fn dfsEvaluate(graph, current, target, product, visited) {
    if (current == target) {
        return product
    }
    
    visited[current] = true
    
    let neighbors = graph[current]
    for (let neighbor in neighbors) {
        if (!visited[neighbor]) {
            let result = dfsEvaluate(graph, neighbor, target, 
                                     product * neighbors[neighbor], visited)
            if (result != -1.0) {
                return result
            }
        }
    }
    
    return -1.0
}
```

### 11. All Paths From Source to Target (Medium)

**Problem**: Find all paths from node 0 to node n-1 in a DAG.

```klang
fn allPathsSourceTarget(graph) {
    let paths = []
    let path = [0]
    let target = graph.length - 1
    
    dfsAllPaths(graph, 0, target, path, paths)
    return paths
}

fn dfsAllPaths(graph, node, target, path, paths) {
    if (node == target) {
        // Found a path - add copy to results
        let pathCopy = []
        for (let i = 0; i < path.length; i = i + 1) {
            pathCopy.push(path[i])
        }
        paths.push(pathCopy)
        return
    }
    
    let neighbors = graph[node]
    for (let i = 0; i < neighbors.length; i = i + 1) {
        let neighbor = neighbors[i]
        path.push(neighbor)
        dfsAllPaths(graph, neighbor, target, path, paths)
        path.pop()  // Backtrack
    }
}
```

### 12. Redundant Connection (Medium)

**Problem**: Find edge that creates a cycle (Union-Find).

```klang
fn findRedundantConnection(edges) {
    let n = edges.length
    let parent = []
    
    // Initialize parent
    for (let i = 0; i <= n; i = i + 1) {
        parent.push(i)
    }
    
    for (let i = 0; i < edges.length; i = i + 1) {
        let u = edges[i][0]
        let v = edges[i][1]
        
        let parentU = find(parent, u)
        let parentV = find(parent, v)
        
        if (parentU == parentV) {
            // Same parent - this edge creates cycle
            return edges[i]
        }
        
        // Union
        parent[parentV] = parentU
    }
    
    return []
}

fn find(parent, x) {
    if (parent[x] != x) {
        parent[x] = find(parent, parent[x])  // Path compression
    }
    return parent[x]
}
```

---

## Applications

### 1. Social Networks
- **Use Case**: Friend connections, followers, recommendations
- **Graph Type**: Undirected (Facebook) or Directed (Twitter)
- **Operations**: Find friends, mutual friends, shortest path

```klang
// Social network example
let socialNetwork = GraphList(5)
socialNetwork.addEdge(0, 1)  // Alice - Bob
socialNetwork.addEdge(0, 2)  // Alice - Carol
socialNetwork.addEdge(1, 3)  // Bob - David
socialNetwork.addEdge(2, 3)  // Carol - David
socialNetwork.addEdge(3, 4)  // David - Eve

// Find friends of Alice (vertex 0)
let aliceFriends = socialNetwork.getNeighbors(0)
print("Alice's friends:", aliceFriends)  // [1, 2] = Bob, Carol

// Find shortest connection from Alice to Eve
let path = socialNetwork.shortestPath(0, 4)
print("Connection path:", path)  // [0, 1, 3, 4]
```

### 2. Maps and Navigation
- **Use Case**: Road networks, GPS navigation, route planning
- **Graph Type**: Weighted, directed/undirected
- **Operations**: Shortest path, route optimization

```klang
// Road network example
let cityMap = GraphList(4)
cityMap.addWeightedEdge(0, 1, 10)  // City A to B: 10 km
cityMap.addWeightedEdge(0, 2, 5)   // City A to C: 5 km
cityMap.addWeightedEdge(1, 3, 8)   // City B to D: 8 km
cityMap.addWeightedEdge(2, 3, 15)  // City C to D: 15 km
```

### 3. Web Crawling and Page Rank
- **Use Case**: Web pages and hyperlinks
- **Graph Type**: Directed, large-scale
- **Operations**: BFS crawling, PageRank algorithm

```klang
// Web page graph example
let webGraph = GraphList(5)
webGraph.addDirectedEdge(0, 1)  // Page A links to B
webGraph.addDirectedEdge(0, 2)  // Page A links to C
webGraph.addDirectedEdge(1, 2)  // Page B links to C
webGraph.addDirectedEdge(2, 3)  // Page C links to D
webGraph.addDirectedEdge(3, 0)  // Page D links to A

// Crawl starting from page 0
let crawlOrder = webGraph.bfs(0)
print("Crawl order:", crawlOrder)
```

### 4. Course Prerequisites
- **Use Case**: Course scheduling, dependency resolution
- **Graph Type**: Directed acyclic graph (DAG)
- **Operations**: Topological sort, cycle detection

```klang
// Course prerequisite example
let courses = GraphList(6)
courses.addDirectedEdge(5, 2)  // Course 5 → Course 2
courses.addDirectedEdge(5, 0)  // Course 5 → Course 0
courses.addDirectedEdge(4, 0)  // Course 4 → Course 0
courses.addDirectedEdge(4, 1)  // Course 4 → Course 1
courses.addDirectedEdge(2, 3)  // Course 2 → Course 3
courses.addDirectedEdge(3, 1)  // Course 3 → Course 1

// Find course order
let courseOrder = topologicalSort(courses)
print("Take courses in order:", courseOrder)
```

### 5. Compiler and Dependency Resolution
- **Use Case**: Module dependencies, build order
- **Graph Type**: Directed acyclic graph (DAG)
- **Operations**: Topological sort

### 6. Network Flow
- **Use Case**: Internet routing, traffic flow
- **Graph Type**: Directed, weighted
- **Operations**: Max flow, min cut

### 7. Recommendation Systems
- **Use Case**: Product recommendations, content suggestions
- **Graph Type**: Bipartite (users-items)
- **Operations**: Graph matching, collaborative filtering

---

## Examples

### Complete Example 1: Social Network Analysis

```klang
// Build a social network
fn buildSocialNetwork() {
    let network = GraphList(6)
    
    // Add friendships
    network.addEdge(0, 1)  // Alice - Bob
    network.addEdge(0, 2)  // Alice - Carol
    network.addEdge(1, 2)  // Bob - Carol
    network.addEdge(1, 3)  // Bob - David
    network.addEdge(2, 4)  // Carol - Eve
    network.addEdge(3, 4)  // David - Eve
    network.addEdge(3, 5)  // David - Frank
    
    return network
}

// Find mutual friends
fn findMutualFriends(network, person1, person2) {
    let friends1 = network.getNeighbors(person1)
    let friends2 = network.getNeighbors(person2)
    
    let mutual = []
    for (let i = 0; i < friends1.length; i = i + 1) {
        for (let j = 0; j < friends2.length; j = j + 1) {
            if (friends1[i] == friends2[j]) {
                mutual.push(friends1[i])
            }
        }
    }
    
    return mutual
}

// Find friend suggestions (friends of friends)
fn suggestFriends(network, person) {
    let friends = network.getNeighbors(person)
    let suggestions = {}
    
    // For each friend
    for (let i = 0; i < friends.length; i = i + 1) {
        let friend = friends[i]
        let friendsOfFriend = network.getNeighbors(friend)
        
        // Add their friends as suggestions
        for (let j = 0; j < friendsOfFriend.length; j = j + 1) {
            let candidate = friendsOfFriend[j]
            
            // Skip if it's the person or already a friend
            if (candidate != person && !isFriend(friends, candidate)) {
                suggestions[candidate] = true
            }
        }
    }
    
    return Object.keys(suggestions)
}

fn isFriend(friends, person) {
    for (let i = 0; i < friends.length; i = i + 1) {
        if (friends[i] == person) {
            return true
        }
    }
    return false
}

// Usage
let network = buildSocialNetwork()
let mutualFriends = findMutualFriends(network, 0, 1)  // Alice and Bob
print("Mutual friends:", mutualFriends)  // [2] = Carol

let suggestions = suggestFriends(network, 0)  // For Alice
print("Friend suggestions for Alice:", suggestions)
```

### Complete Example 2: Maze Solver

```klang
// Solve a maze using BFS
fn solveMaze(maze, start, end) {
    let rows = maze.length
    let cols = maze[0].length
    
    let queue = Queue()
    let visited = []
    let parent = {}
    
    // Initialize visited
    for (let i = 0; i < rows; i = i + 1) {
        let row = []
        for (let j = 0; j < cols; j = j + 1) {
            row.push(false)
        }
        visited.push(row)
    }
    
    // Start BFS
    queue.enqueue(start)
    visited[start[0]][start[1]] = true
    parent[start[0] + "," + start[1]] = null
    
    let directions = [[0, 1], [1, 0], [0, -1], [-1, 0]]
    
    while (!queue.isEmpty()) {
        let current = queue.dequeue()
        let row = current[0]
        let col = current[1]
        
        // Found end
        if (row == end[0] && col == end[1]) {
            return reconstructPath(parent, start, end)
        }
        
        // Explore neighbors
        for (let i = 0; i < 4; i = i + 1) {
            let newRow = row + directions[i][0]
            let newCol = col + directions[i][1]
            
            // Check bounds and walls
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols &&
                maze[newRow][newCol] == 0 && !visited[newRow][newCol]) {
                
                visited[newRow][newCol] = true
                parent[newRow + "," + newCol] = [row, col]
                queue.enqueue([newRow, newCol])
            }
        }
    }
    
    return []  // No path found
}

fn reconstructPath(parent, start, end) {
    let path = []
    let current = end
    
    while (current != null) {
        path.push(current)
        let key = current[0] + "," + current[1]
        current = parent[key]
    }
    
    // Reverse path
    let reversedPath = []
    for (let i = path.length - 1; i >= 0; i = i - 1) {
        reversedPath.push(path[i])
    }
    
    return reversedPath
}

// Example maze (0 = path, 1 = wall)
let maze = [
    [0, 1, 0, 0, 0],
    [0, 1, 0, 1, 0],
    [0, 0, 0, 1, 0],
    [1, 1, 0, 0, 0],
    [0, 0, 0, 1, 0]
]

let start = [0, 0]
let end = [4, 4]
let path = solveMaze(maze, start, end)
print("Path from start to end:", path)
```

### Complete Example 3: Dependency Resolver

```klang
// Build dependency graph
fn buildDependencyGraph(dependencies) {
    let packages = {}
    
    // Create graph
    for (let i = 0; i < dependencies.length; i = i + 1) {
        let pkg = dependencies[i][0]
        let deps = dependencies[i][1]
        
        if (packages[pkg] == null) {
            packages[pkg] = []
        }
        
        for (let j = 0; j < deps.length; j = j + 1) {
            packages[pkg].push(deps[j])
            
            // Ensure dependency exists in graph
            if (packages[deps[j]] == null) {
                packages[deps[j]] = []
            }
        }
    }
    
    return packages
}

// Resolve installation order
fn resolveInstallOrder(packages) {
    let visited = {}
    let stack = []
    let visiting = {}
    
    for (let pkg in packages) {
        if (!visited[pkg]) {
            if (!resolveDFS(packages, pkg, visited, visiting, stack)) {
                return null  // Circular dependency
            }
        }
    }
    
    return stack
}

fn resolveDFS(packages, pkg, visited, visiting, stack) {
    visiting[pkg] = true
    
    let deps = packages[pkg]
    for (let i = 0; i < deps.length; i = i + 1) {
        let dep = deps[i]
        
        if (visiting[dep]) {
            return false  // Circular dependency
        }
        
        if (!visited[dep]) {
            if (!resolveDFS(packages, dep, visited, visiting, stack)) {
                return false
            }
        }
    }
    
    visiting[pkg] = false
    visited[pkg] = true
    stack.push(pkg)
    
    return true
}

// Example usage
let dependencies = [
    ["app", ["logger", "database"]],
    ["logger", ["filesystem"]],
    ["database", ["filesystem", "network"]],
    ["filesystem", []],
    ["network", []]
]

let graph = buildDependencyGraph(dependencies)
let installOrder = resolveInstallOrder(graph)

if (installOrder != null) {
    print("Install packages in order:", installOrder)
} else {
    print("Error: Circular dependency detected!")
}
```

---

## Summary

Graphs are versatile data structures that model relationships and connections:

### Key Takeaways

1. **Graphs consist of vertices and edges** representing entities and their connections
2. **Multiple representations** available: adjacency matrix, adjacency list, edge list
3. **Two main traversal methods**: BFS (level-order) and DFS (depth-first)
4. **Powerful algorithms** for paths, cycles, components, and more
5. **Wide applications** from social networks to navigation systems

### When to Use Graphs

- Modeling networks and relationships
- Finding paths and connections
- Analyzing dependencies
- Network flow problems
- Recommendation systems

### Best Practices Recap

- Choose representation based on graph density
- Track visited vertices to avoid infinite loops
- Use BFS for shortest paths in unweighted graphs
- Use DFS for cycle detection and topological sorting
- Consider directed vs undirected based on relationships

Graphs are fundamental to solving many real-world computational problems!
