# Graph Algorithms

## Introduction

Graph algorithms are fundamental techniques for solving problems involving relationships between entities. Graphs consist of vertices (nodes) and edges (connections), and can be directed or undirected, weighted or unweighted. This guide covers essential graph algorithms with complete KLang implementations.

### Graph Representations

```klang
// Adjacency List (most common)
class Graph {
    var adjList: Map<int, List<int>>;
    var vertices: int;
    var isDirected: bool;

    func init(v: int, directed: bool) {
        this.vertices = v;
        this.isDirected = directed;
        this.adjList = Map<int, List<int>>();
        for (var i = 0; i < v; i++) {
            this.adjList.set(i, List<int>());
        }
    }

    func addEdge(u: int, v: int) {
        this.adjList.get(u).add(v);
        if (!this.isDirected) {
            this.adjList.get(v).add(u);
        }
    }
}

// Weighted Graph
class WeightedGraph {
    var adjList: Map<int, List<Pair<int, int>>>;
    var vertices: int;

    func init(v: int) {
        this.vertices = v;
        this.adjList = Map<int, List<Pair<int, int>>>();
        for (var i = 0; i < v; i++) {
            this.adjList.set(i, List<Pair<int, int>>());
        }
    }

    func addEdge(u: int, v: int, weight: int) {
        this.adjList.get(u).add(Pair<int, int>(v, weight));
    }
}
```

---

## Graph Traversals

### BFS Applications

#### Shortest Path in Unweighted Graph

```klang
class BFSShortestPath {
    func findShortestPath(graph: Graph, start: int, end: int): List<int> {
        var queue = Queue<int>();
        var visited = Set<int>();
        var parent = Map<int, int>();
        
        queue.enqueue(start);
        visited.add(start);
        parent.set(start, -1);
        
        while (!queue.isEmpty()) {
            var node = queue.dequeue();
            
            if (node == end) {
                return this.reconstructPath(parent, start, end);
            }
            
            for (var neighbor in graph.adjList.get(node)) {
                if (!visited.contains(neighbor)) {
                    visited.add(neighbor);
                    parent.set(neighbor, node);
                    queue.enqueue(neighbor);
                }
            }
        }
        
        return List<int>(); // No path found
    }
    
    func reconstructPath(parent: Map<int, int>, start: int, end: int): List<int> {
        var path = List<int>();
        var current = end;
        
        while (current != -1) {
            path.add(current);
            current = parent.get(current);
        }
        
        path.reverse();
        return path;
    }
}
```

#### Level Order Traversal

```klang
class LevelOrder {
    func levelOrderTraversal(graph: Graph, start: int): List<List<int>> {
        var result = List<List<int>>();
        var queue = Queue<int>();
        var visited = Set<int>();
        
        queue.enqueue(start);
        visited.add(start);
        
        while (!queue.isEmpty()) {
            var levelSize = queue.size();
            var level = List<int>();
            
            for (var i = 0; i < levelSize; i++) {
                var node = queue.dequeue();
                level.add(node);
                
                for (var neighbor in graph.adjList.get(node)) {
                    if (!visited.contains(neighbor)) {
                        visited.add(neighbor);
                        queue.enqueue(neighbor);
                    }
                }
            }
            
            result.add(level);
        }
        
        return result;
    }
}
```

### DFS Applications

#### Cycle Detection in Directed Graph

```klang
class CycleDetectionDirected {
    var visited: Set<int>;
    var recStack: Set<int>;
    
    func hasCycle(graph: Graph): bool {
        this.visited = Set<int>();
        this.recStack = Set<int>();
        
        for (var i = 0; i < graph.vertices; i++) {
            if (!this.visited.contains(i)) {
                if (this.dfs(graph, i)) {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    func dfs(graph: Graph, node: int): bool {
        this.visited.add(node);
        this.recStack.add(node);
        
        for (var neighbor in graph.adjList.get(node)) {
            if (!this.visited.contains(neighbor)) {
                if (this.dfs(graph, neighbor)) {
                    return true;
                }
            } else if (this.recStack.contains(neighbor)) {
                return true; // Cycle detected
            }
        }
        
        this.recStack.remove(node);
        return false;
    }
}
```

#### Cycle Detection in Undirected Graph

```klang
class CycleDetectionUndirected {
    var visited: Set<int>;
    
    func hasCycle(graph: Graph): bool {
        this.visited = Set<int>();
        
        for (var i = 0; i < graph.vertices; i++) {
            if (!this.visited.contains(i)) {
                if (this.dfs(graph, i, -1)) {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    func dfs(graph: Graph, node: int, parent: int): bool {
        this.visited.add(node);
        
        for (var neighbor in graph.adjList.get(node)) {
            if (!this.visited.contains(neighbor)) {
                if (this.dfs(graph, neighbor, node)) {
                    return true;
                }
            } else if (neighbor != parent) {
                return true; // Cycle detected
            }
        }
        
        return false;
    }
}
```

---

## Shortest Path Algorithms

### Dijkstra's Algorithm

**Time Complexity**: O((V + E) log V) with min-heap  
**Space Complexity**: O(V)  
**Use Case**: Single-source shortest path with non-negative weights

```klang
class Dijkstra {
    func findShortestPath(graph: WeightedGraph, start: int): Map<int, int> {
        var dist = Map<int, int>();
        var pq = PriorityQueue<Pair<int, int>>(); // (distance, node)
        
        // Initialize distances
        for (var i = 0; i < graph.vertices; i++) {
            dist.set(i, 2147483647); // INT_MAX
        }
        dist.set(start, 0);
        
        pq.enqueue(Pair<int, int>(0, start));
        
        while (!pq.isEmpty()) {
            var current = pq.dequeue();
            var currentDist = current.first;
            var node = current.second;
            
            if (currentDist > dist.get(node)) {
                continue;
            }
            
            for (var edge in graph.adjList.get(node)) {
                var neighbor = edge.first;
                var weight = edge.second;
                var newDist = dist.get(node) + weight;
                
                if (newDist < dist.get(neighbor)) {
                    dist.set(neighbor, newDist);
                    pq.enqueue(Pair<int, int>(newDist, neighbor));
                }
            }
        }
        
        return dist;
    }
    
    func findShortestPathWithRoute(graph: WeightedGraph, start: int, end: int): Pair<int, List<int>> {
        var dist = Map<int, int>();
        var parent = Map<int, int>();
        var pq = PriorityQueue<Pair<int, int>>();
        
        for (var i = 0; i < graph.vertices; i++) {
            dist.set(i, 2147483647);
            parent.set(i, -1);
        }
        dist.set(start, 0);
        
        pq.enqueue(Pair<int, int>(0, start));
        
        while (!pq.isEmpty()) {
            var current = pq.dequeue();
            var node = current.second;
            
            if (node == end) {
                break;
            }
            
            for (var edge in graph.adjList.get(node)) {
                var neighbor = edge.first;
                var weight = edge.second;
                var newDist = dist.get(node) + weight;
                
                if (newDist < dist.get(neighbor)) {
                    dist.set(neighbor, newDist);
                    parent.set(neighbor, node);
                    pq.enqueue(Pair<int, int>(newDist, neighbor));
                }
            }
        }
        
        var path = this.reconstructPath(parent, start, end);
        return Pair<int, List<int>>(dist.get(end), path);
    }
    
    func reconstructPath(parent: Map<int, int>, start: int, end: int): List<int> {
        var path = List<int>();
        var current = end;
        
        while (current != -1) {
            path.add(current);
            current = parent.get(current);
        }
        
        path.reverse();
        return path;
    }
}
```

### Bellman-Ford Algorithm

**Time Complexity**: O(V × E)  
**Space Complexity**: O(V)  
**Use Case**: Single-source shortest path, handles negative weights, detects negative cycles

```klang
class BellmanFord {
    func findShortestPath(edges: List<Edge>, vertices: int, start: int): Map<int, int> {
        var dist = Map<int, int>();
        
        // Initialize distances
        for (var i = 0; i < vertices; i++) {
            dist.set(i, 2147483647);
        }
        dist.set(start, 0);
        
        // Relax edges V-1 times
        for (var i = 0; i < vertices - 1; i++) {
            for (var edge in edges) {
                if (dist.get(edge.from) != 2147483647) {
                    var newDist = dist.get(edge.from) + edge.weight;
                    if (newDist < dist.get(edge.to)) {
                        dist.set(edge.to, newDist);
                    }
                }
            }
        }
        
        // Check for negative cycles
        for (var edge in edges) {
            if (dist.get(edge.from) != 2147483647) {
                var newDist = dist.get(edge.from) + edge.weight;
                if (newDist < dist.get(edge.to)) {
                    throw Exception("Graph contains negative cycle");
                }
            }
        }
        
        return dist;
    }
}

class Edge {
    var from: int;
    var to: int;
    var weight: int;
    
    func init(f: int, t: int, w: int) {
        this.from = f;
        this.to = t;
        this.weight = w;
    }
}
```

### Floyd-Warshall Algorithm

**Time Complexity**: O(V³)  
**Space Complexity**: O(V²)  
**Use Case**: All-pairs shortest path

```klang
class FloydWarshall {
    func allPairsShortestPath(adjMatrix: List<List<int>>): List<List<int>> {
        var n = adjMatrix.size();
        var dist = List<List<int>>();
        
        // Initialize distance matrix
        for (var i = 0; i < n; i++) {
            var row = List<int>();
            for (var j = 0; j < n; j++) {
                row.add(adjMatrix[i][j]);
            }
            dist.add(row);
        }
        
        // Floyd-Warshall
        for (var k = 0; k < n; k++) {
            for (var i = 0; i < n; i++) {
                for (var j = 0; j < n; j++) {
                    if (dist[i][k] != 2147483647 && dist[k][j] != 2147483647) {
                        var newDist = dist[i][k] + dist[k][j];
                        if (newDist < dist[i][j]) {
                            dist[i][j] = newDist;
                        }
                    }
                }
            }
        }
        
        return dist;
    }
    
    func hasNegativeCycle(dist: List<List<int>>): bool {
        var n = dist.size();
        for (var i = 0; i < n; i++) {
            if (dist[i][i] < 0) {
                return true;
            }
        }
        return false;
    }
}
```

---

## Minimum Spanning Tree

### Kruskal's Algorithm

**Time Complexity**: O(E log E)  
**Space Complexity**: O(V)  
**Use Case**: Find MST by sorting edges

```klang
class UnionFind {
    var parent: List<int>;
    var rank: List<int>;
    
    func init(n: int) {
        this.parent = List<int>();
        this.rank = List<int>();
        
        for (var i = 0; i < n; i++) {
            this.parent.add(i);
            this.rank.add(0);
        }
    }
    
    func find(x: int): int {
        if (this.parent[x] != x) {
            this.parent[x] = this.find(this.parent[x]); // Path compression
        }
        return this.parent[x];
    }
    
    func union(x: int, y: int): bool {
        var rootX = this.find(x);
        var rootY = this.find(y);
        
        if (rootX == rootY) {
            return false; // Already in same set
        }
        
        // Union by rank
        if (this.rank[rootX] < this.rank[rootY]) {
            this.parent[rootX] = rootY;
        } else if (this.rank[rootX] > this.rank[rootY]) {
            this.parent[rootY] = rootX;
        } else {
            this.parent[rootY] = rootX;
            this.rank[rootX]++;
        }
        
        return true;
    }
}

class KruskalMST {
    func findMST(vertices: int, edges: List<Edge>): Pair<int, List<Edge>> {
        // Sort edges by weight
        edges.sort((a, b) => a.weight - b.weight);
        
        var uf = UnionFind(vertices);
        var mstEdges = List<Edge>();
        var totalWeight = 0;
        
        for (var edge in edges) {
            if (uf.union(edge.from, edge.to)) {
                mstEdges.add(edge);
                totalWeight += edge.weight;
                
                if (mstEdges.size() == vertices - 1) {
                    break;
                }
            }
        }
        
        return Pair<int, List<Edge>>(totalWeight, mstEdges);
    }
}
```

### Prim's Algorithm

**Time Complexity**: O(E log V)  
**Space Complexity**: O(V)  
**Use Case**: Find MST by growing tree from start node

```klang
class PrimMST {
    func findMST(graph: WeightedGraph, start: int): Pair<int, List<Edge>> {
        var visited = Set<int>();
        var pq = PriorityQueue<Triple<int, int, int>>(); // (weight, from, to)
        var mstEdges = List<Edge>();
        var totalWeight = 0;
        
        visited.add(start);
        
        // Add all edges from start node
        for (var edge in graph.adjList.get(start)) {
            pq.enqueue(Triple<int, int, int>(edge.second, start, edge.first));
        }
        
        while (!pq.isEmpty() && visited.size() < graph.vertices) {
            var current = pq.dequeue();
            var weight = current.first;
            var from = current.second;
            var to = current.third;
            
            if (visited.contains(to)) {
                continue;
            }
            
            visited.add(to);
            mstEdges.add(Edge(from, to, weight));
            totalWeight += weight;
            
            // Add edges from newly visited node
            for (var edge in graph.adjList.get(to)) {
                if (!visited.contains(edge.first)) {
                    pq.enqueue(Triple<int, int, int>(edge.second, to, edge.first));
                }
            }
        }
        
        return Pair<int, List<Edge>>(totalWeight, mstEdges);
    }
}
```

### MST Algorithm Comparison

| Algorithm | Time Complexity | Space | Best For |
|-----------|----------------|-------|----------|
| Kruskal   | O(E log E)     | O(V)  | Sparse graphs, sorted edges |
| Prim      | O(E log V)     | O(V)  | Dense graphs, priority queue |

---

## Topological Sort

### DFS-Based Approach

**Time Complexity**: O(V + E)  
**Space Complexity**: O(V)

```klang
class TopologicalSortDFS {
    var visited: Set<int>;
    var stack: Stack<int>;
    
    func topologicalSort(graph: Graph): List<int> {
        this.visited = Set<int>();
        this.stack = Stack<int>();
        
        for (var i = 0; i < graph.vertices; i++) {
            if (!this.visited.contains(i)) {
                this.dfs(graph, i);
            }
        }
        
        var result = List<int>();
        while (!this.stack.isEmpty()) {
            result.add(this.stack.pop());
        }
        
        return result;
    }
    
    func dfs(graph: Graph, node: int) {
        this.visited.add(node);
        
        for (var neighbor in graph.adjList.get(node)) {
            if (!this.visited.contains(neighbor)) {
                this.dfs(graph, neighbor);
            }
        }
        
        this.stack.push(node);
    }
}
```

### Kahn's Algorithm (BFS-Based)

**Time Complexity**: O(V + E)  
**Space Complexity**: O(V)

```klang
class KahnTopologicalSort {
    func topologicalSort(graph: Graph): List<int> {
        var inDegree = Map<int, int>();
        var queue = Queue<int>();
        var result = List<int>();
        
        // Calculate in-degree for all vertices
        for (var i = 0; i < graph.vertices; i++) {
            inDegree.set(i, 0);
        }
        
        for (var i = 0; i < graph.vertices; i++) {
            for (var neighbor in graph.adjList.get(i)) {
                inDegree.set(neighbor, inDegree.get(neighbor) + 1);
            }
        }
        
        // Add vertices with in-degree 0 to queue
        for (var i = 0; i < graph.vertices; i++) {
            if (inDegree.get(i) == 0) {
                queue.enqueue(i);
            }
        }
        
        // Process vertices
        while (!queue.isEmpty()) {
            var node = queue.dequeue();
            result.add(node);
            
            for (var neighbor in graph.adjList.get(node)) {
                inDegree.set(neighbor, inDegree.get(neighbor) - 1);
                if (inDegree.get(neighbor) == 0) {
                    queue.enqueue(neighbor);
                }
            }
        }
        
        // Check for cycle
        if (result.size() != graph.vertices) {
            throw Exception("Graph has a cycle");
        }
        
        return result;
    }
}
```

---

## Strongly Connected Components

### Kosaraju's Algorithm

**Time Complexity**: O(V + E)  
**Space Complexity**: O(V)

```klang
class KosarajuSCC {
    var visited: Set<int>;
    var stack: Stack<int>;
    
    func findSCCs(graph: Graph): List<List<int>> {
        this.visited = Set<int>();
        this.stack = Stack<int>();
        
        // First DFS to fill stack
        for (var i = 0; i < graph.vertices; i++) {
            if (!this.visited.contains(i)) {
                this.dfs1(graph, i);
            }
        }
        
        // Create transpose graph
        var transpose = this.getTranspose(graph);
        
        // Second DFS on transpose
        this.visited = Set<int>();
        var sccs = List<List<int>>();
        
        while (!this.stack.isEmpty()) {
            var node = this.stack.pop();
            if (!this.visited.contains(node)) {
                var scc = List<int>();
                this.dfs2(transpose, node, scc);
                sccs.add(scc);
            }
        }
        
        return sccs;
    }
    
    func dfs1(graph: Graph, node: int) {
        this.visited.add(node);
        
        for (var neighbor in graph.adjList.get(node)) {
            if (!this.visited.contains(neighbor)) {
                this.dfs1(graph, neighbor);
            }
        }
        
        this.stack.push(node);
    }
    
    func dfs2(graph: Graph, node: int, scc: List<int>) {
        this.visited.add(node);
        scc.add(node);
        
        for (var neighbor in graph.adjList.get(node)) {
            if (!this.visited.contains(neighbor)) {
                this.dfs2(graph, neighbor, scc);
            }
        }
    }
    
    func getTranspose(graph: Graph): Graph {
        var transpose = Graph(graph.vertices, true);
        
        for (var i = 0; i < graph.vertices; i++) {
            for (var neighbor in graph.adjList.get(i)) {
                transpose.addEdge(neighbor, i);
            }
        }
        
        return transpose;
    }
}
```

### Tarjan's Algorithm (Brief)

**Time Complexity**: O(V + E)  
**Space Complexity**: O(V)

```klang
class TarjanSCC {
    var index: int;
    var stack: Stack<int>;
    var onStack: Set<int>;
    var indices: Map<int, int>;
    var lowLink: Map<int, int>;
    var sccs: List<List<int>>;
    
    func findSCCs(graph: Graph): List<List<int>> {
        this.index = 0;
        this.stack = Stack<int>();
        this.onStack = Set<int>();
        this.indices = Map<int, int>();
        this.lowLink = Map<int, int>();
        this.sccs = List<List<int>>();
        
        for (var i = 0; i < graph.vertices; i++) {
            if (!this.indices.containsKey(i)) {
                this.strongConnect(graph, i);
            }
        }
        
        return this.sccs;
    }
    
    func strongConnect(graph: Graph, v: int) {
        this.indices.set(v, this.index);
        this.lowLink.set(v, this.index);
        this.index++;
        this.stack.push(v);
        this.onStack.add(v);
        
        for (var w in graph.adjList.get(v)) {
            if (!this.indices.containsKey(w)) {
                this.strongConnect(graph, w);
                this.lowLink.set(v, min(this.lowLink.get(v), this.lowLink.get(w)));
            } else if (this.onStack.contains(w)) {
                this.lowLink.set(v, min(this.lowLink.get(v), this.indices.get(w)));
            }
        }
        
        if (this.lowLink.get(v) == this.indices.get(v)) {
            var scc = List<int>();
            var w: int;
            do {
                w = this.stack.pop();
                this.onStack.remove(w);
                scc.add(w);
            } while (w != v);
            this.sccs.add(scc);
        }
    }
}
```

---

## Bipartite Graph

### BFS Coloring

**Time Complexity**: O(V + E)  
**Space Complexity**: O(V)

```klang
class BipartiteBFS {
    func isBipartite(graph: Graph): bool {
        var color = Map<int, int>();
        
        for (var i = 0; i < graph.vertices; i++) {
            if (!color.containsKey(i)) {
                if (!this.bfsCheck(graph, i, color)) {
                    return false;
                }
            }
        }
        
        return true;
    }
    
    func bfsCheck(graph: Graph, start: int, color: Map<int, int>): bool {
        var queue = Queue<int>();
        queue.enqueue(start);
        color.set(start, 0);
        
        while (!queue.isEmpty()) {
            var node = queue.dequeue();
            var currentColor = color.get(node);
            
            for (var neighbor in graph.adjList.get(node)) {
                if (!color.containsKey(neighbor)) {
                    color.set(neighbor, 1 - currentColor);
                    queue.enqueue(neighbor);
                } else if (color.get(neighbor) == currentColor) {
                    return false; // Not bipartite
                }
            }
        }
        
        return true;
    }
}
```

### DFS Coloring

```klang
class BipartiteDFS {
    var color: Map<int, int>;
    
    func isBipartite(graph: Graph): bool {
        this.color = Map<int, int>();
        
        for (var i = 0; i < graph.vertices; i++) {
            if (!this.color.containsKey(i)) {
                if (!this.dfsCheck(graph, i, 0)) {
                    return false;
                }
            }
        }
        
        return true;
    }
    
    func dfsCheck(graph: Graph, node: int, c: int): bool {
        this.color.set(node, c);
        
        for (var neighbor in graph.adjList.get(node)) {
            if (!this.color.containsKey(neighbor)) {
                if (!this.dfsCheck(graph, neighbor, 1 - c)) {
                    return false;
                }
            } else if (this.color.get(neighbor) == c) {
                return false;
            }
        }
        
        return true;
    }
}
```

---

## Graph Coloring

### Greedy Coloring

**Time Complexity**: O(V + E)  
**Space Complexity**: O(V)

```klang
class GraphColoring {
    func greedyColoring(graph: Graph): Map<int, int> {
        var result = Map<int, int>();
        var available = List<bool>();
        
        // Initialize all colors as available
        for (var i = 0; i < graph.vertices; i++) {
            available.add(true);
        }
        
        // Assign first color to first vertex
        result.set(0, 0);
        
        // Assign colors to remaining vertices
        for (var u = 1; u < graph.vertices; u++) {
            // Mark colors of adjacent vertices as unavailable
            for (var neighbor in graph.adjList.get(u)) {
                if (result.containsKey(neighbor)) {
                    available[result.get(neighbor)] = false;
                }
            }
            
            // Find first available color
            var color = 0;
            for (var i = 0; i < graph.vertices; i++) {
                if (available[i]) {
                    color = i;
                    break;
                }
            }
            
            result.set(u, color);
            
            // Reset available colors
            for (var i = 0; i < graph.vertices; i++) {
                available[i] = true;
            }
        }
        
        return result;
    }
    
    func chromaticNumber(graph: Graph): int {
        var coloring = this.greedyColoring(graph);
        var maxColor = 0;
        
        for (var vertex = 0; vertex < graph.vertices; vertex++) {
            maxColor = max(maxColor, coloring.get(vertex));
        }
        
        return maxColor + 1;
    }
}
```

---

## Network Flow

### Max Flow Problem (Ford-Fulkerson - Intro)

**Time Complexity**: O(E × max_flow)  
**Space Complexity**: O(V²)

```klang
class MaxFlow {
    func fordFulkerson(capacity: List<List<int>>, source: int, sink: int): int {
        var n = capacity.size();
        var residualGraph = List<List<int>>();
        
        // Create residual graph
        for (var i = 0; i < n; i++) {
            var row = List<int>();
            for (var j = 0; j < n; j++) {
                row.add(capacity[i][j]);
            }
            residualGraph.add(row);
        }
        
        var maxFlow = 0;
        var parent = List<int>();
        
        // While there exists augmenting path
        while (this.bfs(residualGraph, source, sink, parent)) {
            var pathFlow = 2147483647;
            
            // Find minimum capacity in path
            var s = sink;
            while (s != source) {
                pathFlow = min(pathFlow, residualGraph[parent[s]][s]);
                s = parent[s];
            }
            
            // Update residual capacities
            s = sink;
            while (s != source) {
                var u = parent[s];
                residualGraph[u][s] -= pathFlow;
                residualGraph[s][u] += pathFlow;
                s = parent[s];
            }
            
            maxFlow += pathFlow;
        }
        
        return maxFlow;
    }
    
    func bfs(graph: List<List<int>>, source: int, sink: int, parent: List<int>): bool {
        var n = graph.size();
        var visited = List<bool>();
        
        for (var i = 0; i < n; i++) {
            visited.add(false);
            parent.add(-1);
        }
        
        var queue = Queue<int>();
        queue.enqueue(source);
        visited[source] = true;
        
        while (!queue.isEmpty()) {
            var u = queue.dequeue();
            
            for (var v = 0; v < n; v++) {
                if (!visited[v] && graph[u][v] > 0) {
                    queue.enqueue(v);
                    parent[v] = u;
                    visited[v] = true;
                    
                    if (v == sink) {
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
}
```

---

## Interview Problems

### 1. Clone Graph

**Problem**: Clone a graph with nodes containing values and neighbors.

```klang
class Node {
    var val: int;
    var neighbors: List<Node>;
    
    func init(v: int) {
        this.val = v;
        this.neighbors = List<Node>();
    }
}

class CloneGraph {
    func cloneGraph(node: Node): Node {
        if (node == null) {
            return null;
        }
        
        var visited = Map<Node, Node>();
        return this.dfs(node, visited);
    }
    
    func dfs(node: Node, visited: Map<Node, Node>): Node {
        if (visited.containsKey(node)) {
            return visited.get(node);
        }
        
        var clone = Node(node.val);
        visited.set(node, clone);
        
        for (var neighbor in node.neighbors) {
            clone.neighbors.add(this.dfs(neighbor, visited));
        }
        
        return clone;
    }
}
```

### 2. Course Schedule I

**Problem**: Determine if all courses can be completed given prerequisites.

```klang
class CourseSchedule {
    func canFinish(numCourses: int, prerequisites: List<Pair<int, int>>): bool {
        var graph = Graph(numCourses, true);
        
        for (var prereq in prerequisites) {
            graph.addEdge(prereq.second, prereq.first);
        }
        
        var detector = CycleDetectionDirected();
        return !detector.hasCycle(graph);
    }
}
```

### 3. Course Schedule II

**Problem**: Return the order of courses to complete all courses.

```klang
class CourseScheduleII {
    func findOrder(numCourses: int, prerequisites: List<Pair<int, int>>): List<int> {
        var graph = Graph(numCourses, true);
        
        for (var prereq in prerequisites) {
            graph.addEdge(prereq.second, prereq.first);
        }
        
        try {
            var sorter = KahnTopologicalSort();
            return sorter.topologicalSort(graph);
        } catch (Exception e) {
            return List<int>(); // Cycle detected, impossible
        }
    }
}
```

### 4. Network Delay Time

**Problem**: Find minimum time for signal to reach all nodes.

```klang
class NetworkDelayTime {
    func networkDelayTime(times: List<Triple<int, int, int>>, n: int, k: int): int {
        var graph = WeightedGraph(n + 1);
        
        for (var time in times) {
            graph.addEdge(time.first, time.second, time.third);
        }
        
        var dijkstra = Dijkstra();
        var dist = dijkstra.findShortestPath(graph, k);
        
        var maxTime = 0;
        for (var i = 1; i <= n; i++) {
            if (i == k) continue;
            
            if (dist.get(i) == 2147483647) {
                return -1; // Some node unreachable
            }
            maxTime = max(maxTime, dist.get(i));
        }
        
        return maxTime;
    }
}
```

### 5. Cheapest Flights Within K Stops

**Problem**: Find cheapest flight with at most K stops.

```klang
class CheapestFlights {
    func findCheapestPrice(n: int, flights: List<Triple<int, int, int>>, 
                          src: int, dst: int, k: int): int {
        var prices = List<int>();
        for (var i = 0; i < n; i++) {
            prices.add(2147483647);
        }
        prices[src] = 0;
        
        // Relax edges k+1 times
        for (var i = 0; i <= k; i++) {
            var temp = prices.clone();
            
            for (var flight in flights) {
                var from = flight.first;
                var to = flight.second;
                var price = flight.third;
                
                if (prices[from] != 2147483647) {
                    temp[to] = min(temp[to], prices[from] + price);
                }
            }
            
            prices = temp;
        }
        
        return prices[dst] == 2147483647 ? -1 : prices[dst];
    }
}
```

### 6. Minimum Height Trees

**Problem**: Find root nodes that minimize tree height.

```klang
class MinimumHeightTrees {
    func findMinHeightTrees(n: int, edges: List<Pair<int, int>>): List<int> {
        if (n == 1) {
            return List<int>([0]);
        }
        
        var graph = Graph(n, false);
        var degree = List<int>();
        
        for (var i = 0; i < n; i++) {
            degree.add(0);
        }
        
        for (var edge in edges) {
            graph.addEdge(edge.first, edge.second);
            degree[edge.first]++;
            degree[edge.second]++;
        }
        
        var leaves = Queue<int>();
        for (var i = 0; i < n; i++) {
            if (degree[i] == 1) {
                leaves.enqueue(i);
            }
        }
        
        var remaining = n;
        while (remaining > 2) {
            var leavesCount = leaves.size();
            remaining -= leavesCount;
            
            for (var i = 0; i < leavesCount; i++) {
                var leaf = leaves.dequeue();
                
                for (var neighbor in graph.adjList.get(leaf)) {
                    degree[neighbor]--;
                    if (degree[neighbor] == 1) {
                        leaves.enqueue(neighbor);
                    }
                }
            }
        }
        
        var result = List<int>();
        while (!leaves.isEmpty()) {
            result.add(leaves.dequeue());
        }
        return result;
    }
}
```

### 7. Redundant Connection

**Problem**: Find edge that creates cycle in undirected graph.

```klang
class RedundantConnection {
    func findRedundantConnection(edges: List<Pair<int, int>>): Pair<int, int> {
        var n = edges.size();
        var uf = UnionFind(n + 1);
        
        for (var edge in edges) {
            if (!uf.union(edge.first, edge.second)) {
                return edge; // This edge creates a cycle
            }
        }
        
        return Pair<int, int>(-1, -1);
    }
}
```

### 8. Critical Connections

**Problem**: Find bridges in a network (Tarjan's bridge algorithm).

```klang
class CriticalConnections {
    var time: int;
    var disc: List<int>;
    var low: List<int>;
    var bridges: List<Pair<int, int>>;
    
    func criticalConnections(n: int, connections: List<Pair<int, int>>): List<Pair<int, int>> {
        var graph = Graph(n, false);
        for (var conn in connections) {
            graph.addEdge(conn.first, conn.second);
        }
        
        this.time = 0;
        this.disc = List<int>();
        this.low = List<int>();
        this.bridges = List<Pair<int, int>>();
        
        for (var i = 0; i < n; i++) {
            this.disc.add(-1);
            this.low.add(-1);
        }
        
        for (var i = 0; i < n; i++) {
            if (this.disc[i] == -1) {
                this.dfs(graph, i, -1);
            }
        }
        
        return this.bridges;
    }
    
    func dfs(graph: Graph, u: int, parent: int) {
        this.disc[u] = this.time;
        this.low[u] = this.time;
        this.time++;
        
        for (var v in graph.adjList.get(u)) {
            if (v == parent) continue;
            
            if (this.disc[v] == -1) {
                this.dfs(graph, v, u);
                this.low[u] = min(this.low[u], this.low[v]);
                
                if (this.low[v] > this.disc[u]) {
                    this.bridges.add(Pair<int, int>(u, v));
                }
            } else {
                this.low[u] = min(this.low[u], this.disc[v]);
            }
        }
    }
}
```

### 9. Word Ladder

**Problem**: Find shortest transformation sequence from beginWord to endWord.

```klang
class WordLadder {
    func ladderLength(beginWord: string, endWord: string, wordList: List<string>): int {
        var wordSet = Set<string>();
        for (var word in wordList) {
            wordSet.add(word);
        }
        
        if (!wordSet.contains(endWord)) {
            return 0;
        }
        
        var queue = Queue<Pair<string, int>>();
        queue.enqueue(Pair<string, int>(beginWord, 1));
        
        while (!queue.isEmpty()) {
            var current = queue.dequeue();
            var word = current.first;
            var level = current.second;
            
            if (word == endWord) {
                return level;
            }
            
            // Try all possible one-letter transformations
            for (var i = 0; i < word.length(); i++) {
                var chars = word.toCharArray();
                for (var c = 'a'; c <= 'z'; c++) {
                    chars[i] = c;
                    var newWord = String.fromCharArray(chars);
                    
                    if (wordSet.contains(newWord)) {
                        queue.enqueue(Pair<string, int>(newWord, level + 1));
                        wordSet.remove(newWord);
                    }
                }
            }
        }
        
        return 0;
    }
}
```

### 10. All Paths From Source to Target

**Problem**: Find all paths from node 0 to node n-1.

```klang
class AllPaths {
    func allPathsSourceTarget(graph: List<List<int>>): List<List<int>> {
        var result = List<List<int>>();
        var path = List<int>();
        path.add(0);
        
        this.dfs(graph, 0, graph.size() - 1, path, result);
        return result;
    }
    
    func dfs(graph: List<List<int>>, node: int, target: int, 
             path: List<int>, result: List<List<int>>) {
        if (node == target) {
            result.add(path.clone());
            return;
        }
        
        for (var neighbor in graph[node]) {
            path.add(neighbor);
            this.dfs(graph, neighbor, target, path, result);
            path.removeLast();
        }
    }
}
```

### 11. Find the Town Judge

**Problem**: Find person trusted by all but trusts nobody.

```klang
class TownJudge {
    func findJudge(n: int, trust: List<Pair<int, int>>): int {
        var trustCount = List<int>();
        
        for (var i = 0; i <= n; i++) {
            trustCount.add(0);
        }
        
        for (var t in trust) {
            trustCount[t.first]--; // This person trusts someone
            trustCount[t.second]++; // This person is trusted
        }
        
        for (var i = 1; i <= n; i++) {
            if (trustCount[i] == n - 1) {
                return i;
            }
        }
        
        return -1;
    }
}
```

### 12. Keys and Rooms

**Problem**: Can visit all rooms starting from room 0?

```klang
class KeysAndRooms {
    func canVisitAllRooms(rooms: List<List<int>>): bool {
        var visited = Set<int>();
        var queue = Queue<int>();
        
        queue.enqueue(0);
        visited.add(0);
        
        while (!queue.isEmpty()) {
            var room = queue.dequeue();
            
            for (var key in rooms[room]) {
                if (!visited.contains(key)) {
                    visited.add(key);
                    queue.enqueue(key);
                }
            }
        }
        
        return visited.size() == rooms.size();
    }
}
```

### 13. Evaluate Division

**Problem**: Evaluate division equations (a/b = k).

```klang
class EvaluateDivision {
    func calcEquation(equations: List<Pair<string, string>>, 
                      values: List<double>,
                      queries: List<Pair<string, string>>): List<double> {
        var graph = Map<string, List<Pair<string, double>>>();
        
        // Build graph
        for (var i = 0; i < equations.size(); i++) {
            var a = equations[i].first;
            var b = equations[i].second;
            var value = values[i];
            
            if (!graph.containsKey(a)) {
                graph.set(a, List<Pair<string, double>>());
            }
            if (!graph.containsKey(b)) {
                graph.set(b, List<Pair<string, double>>());
            }
            
            graph.get(a).add(Pair<string, double>(b, value));
            graph.get(b).add(Pair<string, double>(a, 1.0 / value));
        }
        
        var result = List<double>();
        
        for (var query in queries) {
            var start = query.first;
            var end = query.second;
            
            if (!graph.containsKey(start) || !graph.containsKey(end)) {
                result.add(-1.0);
            } else if (start == end) {
                result.add(1.0);
            } else {
                var visited = Set<string>();
                result.add(this.dfs(graph, start, end, 1.0, visited));
            }
        }
        
        return result;
    }
    
    func dfs(graph: Map<string, List<Pair<string, double>>>, 
             current: string, target: string, product: double, 
             visited: Set<string>): double {
        visited.add(current);
        
        if (current == target) {
            return product;
        }
        
        for (var neighbor in graph.get(current)) {
            var next = neighbor.first;
            var value = neighbor.second;
            
            if (!visited.contains(next)) {
                var result = this.dfs(graph, next, target, product * value, visited);
                if (result != -1.0) {
                    return result;
                }
            }
        }
        
        return -1.0;
    }
}
```

---

## Complexity Analysis

### Algorithm Comparison Table

| Algorithm | Time Complexity | Space | Use Case |
|-----------|----------------|-------|----------|
| BFS | O(V + E) | O(V) | Shortest path (unweighted), level order |
| DFS | O(V + E) | O(V) | Cycle detection, topological sort, SCC |
| Dijkstra | O((V+E) log V) | O(V) | Shortest path (non-negative weights) |
| Bellman-Ford | O(V × E) | O(V) | Shortest path (negative weights) |
| Floyd-Warshall | O(V³) | O(V²) | All-pairs shortest path |
| Kruskal's MST | O(E log E) | O(V) | MST (sparse graphs) |
| Prim's MST | O(E log V) | O(V) | MST (dense graphs) |
| Topological Sort | O(V + E) | O(V) | Ordering with dependencies |
| Kosaraju's SCC | O(V + E) | O(V) | Find strongly connected components |
| Tarjan's SCC | O(V + E) | O(V) | Find SCCs (single pass) |
| Union-Find | O(α(n)) | O(n) | Cycle detection, connected components |
| Max Flow (FF) | O(E × f) | O(V²) | Network flow (f = max flow) |

---

## Best Practices

### Algorithm Selection Guide

1. **Shortest Path**:
   - Unweighted graph → BFS
   - Non-negative weights → Dijkstra
   - Negative weights → Bellman-Ford
   - All pairs → Floyd-Warshall

2. **Minimum Spanning Tree**:
   - Sparse graph → Kruskal's
   - Dense graph → Prim's

3. **Cycle Detection**:
   - Directed graph → DFS with recursion stack
   - Undirected graph → DFS with parent or Union-Find

4. **Topological Sort**:
   - DFS-based for recursive approach
   - Kahn's algorithm for cycle detection

5. **Connected Components**:
   - Undirected → DFS/BFS or Union-Find
   - Strongly connected (directed) → Kosaraju's or Tarjan's

### Implementation Tips

1. **Choose Right Representation**:
   - Adjacency list for sparse graphs
   - Adjacency matrix for dense graphs

2. **Handle Edge Cases**:
   - Empty graphs
   - Single vertex
   - Disconnected components
   - Self-loops

3. **Optimize for Problem**:
   - Use appropriate data structures (priority queues, union-find)
   - Consider space-time tradeoffs
   - Preprocess when beneficial

4. **Test Thoroughly**:
   - Test with cycles
   - Test with disconnected graphs
   - Test with negative weights (if applicable)
   - Test edge cases

### Common Pitfalls

1. **Not handling disconnected graphs**
2. **Forgetting to mark visited nodes**
3. **Using wrong algorithm for weighted graphs**
4. **Not detecting negative cycles in Bellman-Ford**
5. **Incorrect parent tracking in Union-Find**
6. **Stack overflow with DFS on large graphs**

---

## Summary

Graph algorithms are essential for solving complex relationship and network problems. Key takeaways:

- **BFS** for shortest paths in unweighted graphs and level-order traversal
- **DFS** for cycle detection, topological sorting, and exploring all paths
- **Dijkstra** for single-source shortest path with non-negative weights
- **Bellman-Ford** when negative weights are present
- **Floyd-Warshall** for all-pairs shortest paths
- **Kruskal's and Prim's** for minimum spanning trees
- **Topological Sort** for ordering tasks with dependencies
- **Union-Find** for efficient cycle detection and connected components
- **Kosaraju's and Tarjan's** for strongly connected components

Choose the right algorithm based on:
- Graph properties (weighted, directed, dense vs sparse)
- Problem requirements (single-source vs all-pairs)
- Time and space constraints

Master these algorithms to solve a wide range of graph problems efficiently!

---

*Next: [Advanced Algorithms](07_advanced_algorithms.md)*
