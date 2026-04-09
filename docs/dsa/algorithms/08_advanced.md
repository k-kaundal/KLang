# Advanced Algorithms

## Introduction

Advanced algorithms represent the pinnacle of algorithmic problem-solving, combining multiple techniques and sophisticated data structures to solve complex computational challenges. These algorithms are essential for competitive programming, technical interviews at top tech companies, and real-world applications in fields like computer graphics, network optimization, signal processing, and distributed systems.

This comprehensive guide covers advanced algorithmic topics including:
- **All-Pairs Shortest Paths** with Floyd-Warshall
- **Advanced Tree Structures** (Segment Trees, Fenwick Trees)
- **Computational Geometry** fundamentals
- **Network Flow** algorithms
- **Fast Fourier Transform** basics
- **Randomized Algorithms**
- **Bit Manipulation** techniques
- **Complex Interview Problems** with complete solutions

Each topic includes detailed explanations, complete KLang implementations, complexity analysis, and real-world applications.

---

## All Pairs Shortest Path

### Floyd-Warshall Algorithm

The Floyd-Warshall algorithm finds shortest paths between all pairs of vertices in a weighted graph. Unlike Dijkstra's or Bellman-Ford which find paths from a single source, Floyd-Warshall computes the complete distance matrix.

**Time Complexity**: O(V³)  
**Space Complexity**: O(V²)  
**Supports**: Negative edge weights (but not negative cycles)

#### Complete Implementation

```klang
class FloydWarshall {
    var INF: int = 999999999;
    
    // Find all-pairs shortest paths
    func findAllPairsShortestPaths(graph: List<List<int>>): List<List<int>> {
        var n = graph.size();
        var dist = List<List<int>>();
        
        // Initialize distance matrix
        for (var i = 0; i < n; i++) {
            dist.add(List<int>());
            for (var j = 0; j < n; j++) {
                dist[i].add(graph[i][j]);
            }
        }
        
        // Floyd-Warshall: try each vertex as intermediate
        for (var k = 0; k < n; k++) {
            for (var i = 0; i < n; i++) {
                for (var j = 0; j < n; j++) {
                    if (dist[i][k] != this.INF && dist[k][j] != this.INF) {
                        if (dist[i][k] + dist[k][j] < dist[i][j]) {
                            dist[i][j] = dist[i][k] + dist[k][j];
                        }
                    }
                }
            }
        }
        
        return dist;
    }
    
    // With path reconstruction
    func findAllPathsWithReconstruction(graph: List<List<int>>): Pair<List<List<int>>, List<List<int>>> {
        var n = graph.size();
        var dist = List<List<int>>();
        var next = List<List<int>>();
        
        // Initialize
        for (var i = 0; i < n; i++) {
            dist.add(List<int>());
            next.add(List<int>());
            for (var j = 0; j < n; j++) {
                dist[i].add(graph[i][j]);
                if (graph[i][j] != this.INF && i != j) {
                    next[i].add(j);
                } else {
                    next[i].add(-1);
                }
            }
        }
        
        // Floyd-Warshall with path tracking
        for (var k = 0; k < n; k++) {
            for (var i = 0; i < n; i++) {
                for (var j = 0; j < n; j++) {
                    if (dist[i][k] != this.INF && dist[k][j] != this.INF) {
                        if (dist[i][k] + dist[k][j] < dist[i][j]) {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            next[i][j] = next[i][k];
                        }
                    }
                }
            }
        }
        
        return Pair<List<List<int>>, List<List<int>>>(dist, next);
    }
    
    // Reconstruct path between two vertices
    func reconstructPath(next: List<List<int>>, u: int, v: int): List<int> {
        if (next[u][v] == -1) {
            return List<int>(); // No path
        }
        
        var path = List<int>();
        path.add(u);
        
        while (u != v) {
            u = next[u][v];
            path.add(u);
        }
        
        return path;
    }
    
    // Detect negative cycles
    func hasNegativeCycle(graph: List<List<int>>): bool {
        var dist = this.findAllPairsShortestPaths(graph);
        var n = dist.size();
        
        // If any diagonal element is negative, there's a negative cycle
        for (var i = 0; i < n; i++) {
            if (dist[i][i] < 0) {
                return true;
            }
        }
        
        return false;
    }
    
    // Find transitive closure (reachability)
    func findTransitiveClosure(graph: List<List<bool>>): List<List<bool>> {
        var n = graph.size();
        var reach = List<List<bool>>();
        
        // Initialize reachability matrix
        for (var i = 0; i < n; i++) {
            reach.add(List<bool>());
            for (var j = 0; j < n; j++) {
                reach[i].add(graph[i][j] || i == j);
            }
        }
        
        // Warshall's algorithm (boolean version of Floyd-Warshall)
        for (var k = 0; k < n; k++) {
            for (var i = 0; i < n; i++) {
                for (var j = 0; j < n; j++) {
                    reach[i][j] = reach[i][j] || (reach[i][k] && reach[k][j]);
                }
            }
        }
        
        return reach;
    }
}
```

#### Example Usage

```klang
func exampleFloydWarshall() {
    var fw = FloydWarshall();
    
    // Create weighted graph (adjacency matrix)
    var INF = 999999999;
    var graph = List<List<int>>();
    graph.add([0, 3, INF, 7]);
    graph.add([8, 0, 2, INF]);
    graph.add([5, INF, 0, 1]);
    graph.add([2, INF, INF, 0]);
    
    // Find all shortest paths
    var dist = fw.findAllPairsShortestPaths(graph);
    
    println("All-pairs shortest distances:");
    for (var i = 0; i < dist.size(); i++) {
        for (var j = 0; j < dist[i].size(); j++) {
            if (dist[i][j] == INF) {
                print("INF ");
            } else {
                print(dist[i][j] + " ");
            }
        }
        println("");
    }
    
    // With path reconstruction
    var result = fw.findAllPathsWithReconstruction(graph);
    var distances = result.first;
    var next = result.second;
    
    println("\nPath from 0 to 3:");
    var path = fw.reconstructPath(next, 0, 3);
    for (var node in path) {
        print(node + " -> ");
    }
    println("END");
    
    // Check for negative cycles
    if (fw.hasNegativeCycle(graph)) {
        println("Graph contains negative cycle!");
    }
}
```

---

## Advanced Tree Structures

### Segment Tree

Segment Trees are powerful data structures for range queries and updates on arrays. They support operations like range sum, range minimum/maximum, and range updates in O(log n) time.

**Time Complexity**:
- Build: O(n)
- Query: O(log n)
- Update: O(log n)

**Space Complexity**: O(4n) ≈ O(n)

#### Complete Segment Tree Implementation

```klang
class SegmentTree {
    var tree: List<int>;
    var n: int;
    
    func init(arr: List<int>) {
        this.n = arr.size();
        this.tree = List<int>();
        
        // Segment tree needs 4*n space
        for (var i = 0; i < 4 * this.n; i++) {
            this.tree.add(0);
        }
        
        if (this.n > 0) {
            this.build(arr, 0, 0, this.n - 1);
        }
    }
    
    // Build segment tree
    func build(arr: List<int>, node: int, start: int, end: int) {
        if (start == end) {
            this.tree[node] = arr[start];
            return;
        }
        
        var mid = start + (end - start) / 2;
        var leftChild = 2 * node + 1;
        var rightChild = 2 * node + 2;
        
        this.build(arr, leftChild, start, mid);
        this.build(arr, rightChild, mid + 1, end);
        
        // Combine children (sum in this case)
        this.tree[node] = this.tree[leftChild] + this.tree[rightChild];
    }
    
    // Range query [l, r]
    func query(l: int, r: int): int {
        return this.queryHelper(0, 0, this.n - 1, l, r);
    }
    
    func queryHelper(node: int, start: int, end: int, l: int, r: int): int {
        // No overlap
        if (r < start || end < l) {
            return 0;
        }
        
        // Complete overlap
        if (l <= start && end <= r) {
            return this.tree[node];
        }
        
        // Partial overlap
        var mid = start + (end - start) / 2;
        var leftChild = 2 * node + 1;
        var rightChild = 2 * node + 2;
        
        var leftSum = this.queryHelper(leftChild, start, mid, l, r);
        var rightSum = this.queryHelper(rightChild, mid + 1, end, l, r);
        
        return leftSum + rightSum;
    }
    
    // Point update
    func update(index: int, value: int) {
        this.updateHelper(0, 0, this.n - 1, index, value);
    }
    
    func updateHelper(node: int, start: int, end: int, index: int, value: int) {
        if (start == end) {
            this.tree[node] = value;
            return;
        }
        
        var mid = start + (end - start) / 2;
        var leftChild = 2 * node + 1;
        var rightChild = 2 * node + 2;
        
        if (index <= mid) {
            this.updateHelper(leftChild, start, mid, index, value);
        } else {
            this.updateHelper(rightChild, mid + 1, end, index, value);
        }
        
        this.tree[node] = this.tree[leftChild] + this.tree[rightChild];
    }
    
    // Range update (lazy propagation)
    func rangeUpdate(l: int, r: int, value: int) {
        this.rangeUpdateHelper(0, 0, this.n - 1, l, r, value);
    }
    
    func rangeUpdateHelper(node: int, start: int, end: int, l: int, r: int, value: int) {
        if (r < start || end < l) {
            return;
        }
        
        if (start == end) {
            this.tree[node] += value;
            return;
        }
        
        var mid = start + (end - start) / 2;
        var leftChild = 2 * node + 1;
        var rightChild = 2 * node + 2;
        
        this.rangeUpdateHelper(leftChild, start, mid, l, r, value);
        this.rangeUpdateHelper(rightChild, mid + 1, end, l, r, value);
        
        this.tree[node] = this.tree[leftChild] + this.tree[rightChild];
    }
}

// Segment Tree for Range Minimum Query
class SegmentTreeRMQ {
    var tree: List<int>;
    var n: int;
    var INF: int = 999999999;
    
    func init(arr: List<int>) {
        this.n = arr.size();
        this.tree = List<int>();
        
        for (var i = 0; i < 4 * this.n; i++) {
            this.tree.add(this.INF);
        }
        
        if (this.n > 0) {
            this.build(arr, 0, 0, this.n - 1);
        }
    }
    
    func build(arr: List<int>, node: int, start: int, end: int) {
        if (start == end) {
            this.tree[node] = arr[start];
            return;
        }
        
        var mid = start + (end - start) / 2;
        var leftChild = 2 * node + 1;
        var rightChild = 2 * node + 2;
        
        this.build(arr, leftChild, start, mid);
        this.build(arr, rightChild, mid + 1, end);
        
        this.tree[node] = min(this.tree[leftChild], this.tree[rightChild]);
    }
    
    func query(l: int, r: int): int {
        return this.queryHelper(0, 0, this.n - 1, l, r);
    }
    
    func queryHelper(node: int, start: int, end: int, l: int, r: int): int {
        if (r < start || end < l) {
            return this.INF;
        }
        
        if (l <= start && end <= r) {
            return this.tree[node];
        }
        
        var mid = start + (end - start) / 2;
        var leftChild = 2 * node + 1;
        var rightChild = 2 * node + 2;
        
        var leftMin = this.queryHelper(leftChild, start, mid, l, r);
        var rightMin = this.queryHelper(rightChild, mid + 1, end, l, r);
        
        return min(leftMin, rightMin);
    }
    
    func update(index: int, value: int) {
        this.updateHelper(0, 0, this.n - 1, index, value);
    }
    
    func updateHelper(node: int, start: int, end: int, index: int, value: int) {
        if (start == end) {
            this.tree[node] = value;
            return;
        }
        
        var mid = start + (end - start) / 2;
        var leftChild = 2 * node + 1;
        var rightChild = 2 * node + 2;
        
        if (index <= mid) {
            this.updateHelper(leftChild, start, mid, index, value);
        } else {
            this.updateHelper(rightChild, mid + 1, end, index, value);
        }
        
        this.tree[node] = min(this.tree[leftChild], this.tree[rightChild]);
    }
}
```

### Fenwick Tree (Binary Indexed Tree)

Fenwick Trees provide an efficient way to calculate prefix sums and update elements. They're simpler than segment trees but support fewer operations.

**Time Complexity**:
- Build: O(n log n)
- Query: O(log n)
- Update: O(log n)

**Space Complexity**: O(n)

#### Complete Fenwick Tree Implementation

```klang
class FenwickTree {
    var tree: List<int>;
    var n: int;
    
    func init(size: int) {
        this.n = size;
        this.tree = List<int>();
        for (var i = 0; i <= size; i++) {
            this.tree.add(0);
        }
    }
    
    func initFromArray(arr: List<int>) {
        this.n = arr.size();
        this.tree = List<int>();
        for (var i = 0; i <= this.n; i++) {
            this.tree.add(0);
        }
        
        // Build tree
        for (var i = 0; i < arr.size(); i++) {
            this.update(i + 1, arr[i]);
        }
    }
    
    // Update value at index (1-indexed)
    func update(index: int, delta: int) {
        while (index <= this.n) {
            this.tree[index] += delta;
            index += index & (-index); // Add last set bit
        }
    }
    
    // Get prefix sum [1, index]
    func query(index: int): int {
        var sum = 0;
        while (index > 0) {
            sum += this.tree[index];
            index -= index & (-index); // Remove last set bit
        }
        return sum;
    }
    
    // Get range sum [left, right] (1-indexed)
    func rangeQuery(left: int, right: int): int {
        if (left > 1) {
            return this.query(right) - this.query(left - 1);
        }
        return this.query(right);
    }
    
    // Find index with given cumulative sum (binary search on BIT)
    func findIndex(sum: int): int {
        var pos = 0;
        var bitMask = 1;
        
        // Find highest power of 2 <= n
        while (bitMask <= this.n) {
            bitMask <<= 1;
        }
        bitMask >>= 1;
        
        while (bitMask > 0) {
            if (pos + bitMask <= this.n && this.tree[pos + bitMask] < sum) {
                sum -= this.tree[pos + bitMask];
                pos += bitMask;
            }
            bitMask >>= 1;
        }
        
        return pos + 1;
    }
}

// 2D Fenwick Tree for 2D range queries
class FenwickTree2D {
    var tree: List<List<int>>;
    var rows: int;
    var cols: int;
    
    func init(r: int, c: int) {
        this.rows = r;
        this.cols = c;
        this.tree = List<List<int>>();
        
        for (var i = 0; i <= r; i++) {
            this.tree.add(List<int>());
            for (var j = 0; j <= c; j++) {
                this.tree[i].add(0);
            }
        }
    }
    
    // Update cell (row, col) by delta
    func update(row: int, col: int, delta: int) {
        var i = row;
        while (i <= this.rows) {
            var j = col;
            while (j <= this.cols) {
                this.tree[i][j] += delta;
                j += j & (-j);
            }
            i += i & (-i);
        }
    }
    
    // Query sum from (1,1) to (row, col)
    func query(row: int, col: int): int {
        var sum = 0;
        var i = row;
        while (i > 0) {
            var j = col;
            while (j > 0) {
                sum += this.tree[i][j];
                j -= j & (-j);
            }
            i -= i & (-i);
        }
        return sum;
    }
    
    // Range query from (r1, c1) to (r2, c2)
    func rangeQuery(r1: int, c1: int, r2: int, c2: int): int {
        var sum = this.query(r2, c2);
        
        if (r1 > 1) {
            sum -= this.query(r1 - 1, c2);
        }
        if (c1 > 1) {
            sum -= this.query(r2, c1 - 1);
        }
        if (r1 > 1 && c1 > 1) {
            sum += this.query(r1 - 1, c1 - 1);
        }
        
        return sum;
    }
}
```

---

## Computational Geometry

### Point and Line Representation

```klang
class Point {
    var x: float;
    var y: float;
    
    func init(x: float, y: float) {
        this.x = x;
        this.y = y;
    }
    
    func distance(other: Point): float {
        var dx = this.x - other.x;
        var dy = this.y - other.y;
        return sqrt(dx * dx + dy * dy);
    }
    
    func distanceSquared(other: Point): float {
        var dx = this.x - other.x;
        var dy = this.y - other.y;
        return dx * dx + dy * dy;
    }
}

class Line {
    var p1: Point;
    var p2: Point;
    
    func init(p1: Point, p2: Point) {
        this.p1 = p1;
        this.p2 = p2;
    }
}
```

### Line Intersection

```klang
class GeometryAlgorithms {
    // Find orientation of ordered triplet (p, q, r)
    // Returns: 0 -> Collinear, 1 -> Clockwise, 2 -> Counterclockwise
    func orientation(p: Point, q: Point, r: Point): int {
        var val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
        
        if (abs(val) < 0.0001) {
            return 0; // Collinear
        }
        
        return val > 0 ? 1 : 2; // Clockwise or Counterclockwise
    }
    
    // Check if point q lies on segment pr
    func onSegment(p: Point, q: Point, r: Point): bool {
        return q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
               q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y);
    }
    
    // Check if two line segments intersect
    func doSegmentsIntersect(l1: Line, l2: Line): bool {
        var p1 = l1.p1;
        var q1 = l1.p2;
        var p2 = l2.p1;
        var q2 = l2.p2;
        
        var o1 = this.orientation(p1, q1, p2);
        var o2 = this.orientation(p1, q1, q2);
        var o3 = this.orientation(p2, q2, p1);
        var o4 = this.orientation(p2, q2, q1);
        
        // General case
        if (o1 != o2 && o3 != o4) {
            return true;
        }
        
        // Special cases (collinear points)
        if (o1 == 0 && this.onSegment(p1, p2, q1)) return true;
        if (o2 == 0 && this.onSegment(p1, q2, q1)) return true;
        if (o3 == 0 && this.onSegment(p2, p1, q2)) return true;
        if (o4 == 0 && this.onSegment(p2, q1, q2)) return true;
        
        return false;
    }
    
    // Cross product of vectors
    func crossProduct(o: Point, a: Point, b: Point): float {
        return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
    }
}
```

### Convex Hull (Graham's Scan)

```klang
class ConvexHull {
    // Find convex hull using Graham's scan
    func grahamScan(points: List<Point>): List<Point> {
        if (points.size() < 3) {
            return points;
        }
        
        // Find bottom-most point (or leftmost in case of tie)
        var minIdx = 0;
        for (var i = 1; i < points.size(); i++) {
            if (points[i].y < points[minIdx].y ||
                (points[i].y == points[minIdx].y && points[i].x < points[minIdx].x)) {
                minIdx = i;
            }
        }
        
        // Swap with first position
        var temp = points[0];
        points[0] = points[minIdx];
        points[minIdx] = temp;
        
        var p0 = points[0];
        
        // Sort points by polar angle with p0
        this.sortByPolarAngle(points, p0);
        
        // Create hull
        var hull = List<Point>();
        hull.add(points[0]);
        hull.add(points[1]);
        hull.add(points[2]);
        
        for (var i = 3; i < points.size(); i++) {
            // Remove points that make clockwise turn
            while (hull.size() > 1 && 
                   this.crossProduct(hull[hull.size() - 2], 
                                   hull[hull.size() - 1], 
                                   points[i]) <= 0) {
                hull.removeLast();
            }
            hull.add(points[i]);
        }
        
        return hull;
    }
    
    func crossProduct(o: Point, a: Point, b: Point): float {
        return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
    }
    
    func sortByPolarAngle(points: List<Point>, p0: Point) {
        // Simple bubble sort for demonstration
        // In practice, use library sort with custom comparator
        for (var i = 1; i < points.size() - 1; i++) {
            for (var j = i + 1; j < points.size(); j++) {
                var cross = this.crossProduct(p0, points[i], points[j]);
                if (cross < 0 || (cross == 0 && 
                    p0.distanceSquared(points[j]) < p0.distanceSquared(points[i]))) {
                    var temp = points[i];
                    points[i] = points[j];
                    points[j] = temp;
                }
            }
        }
    }
}
```

### Closest Pair of Points

```klang
class ClosestPair {
    // Find closest pair of points using divide and conquer
    func findClosestPair(points: List<Point>): float {
        if (points.size() < 2) {
            return 999999999.0;
        }
        
        // Sort by x-coordinate
        this.sortByX(points);
        
        return this.closestUtil(points, 0, points.size() - 1);
    }
    
    func closestUtil(points: List<Point>, left: int, right: int): float {
        // Base case: use brute force for small arrays
        if (right - left <= 3) {
            return this.bruteForce(points, left, right);
        }
        
        // Divide
        var mid = left + (right - left) / 2;
        var midPoint = points[mid];
        
        var dl = this.closestUtil(points, left, mid);
        var dr = this.closestUtil(points, mid + 1, right);
        
        var d = min(dl, dr);
        
        // Build strip of points closer than d to dividing line
        var strip = List<Point>();
        for (var i = left; i <= right; i++) {
            if (abs(points[i].x - midPoint.x) < d) {
                strip.add(points[i]);
            }
        }
        
        // Find closest in strip
        return min(d, this.stripClosest(strip, d));
    }
    
    func bruteForce(points: List<Point>, left: int, right: int): float {
        var minDist = 999999999.0;
        for (var i = left; i <= right; i++) {
            for (var j = i + 1; j <= right; j++) {
                var dist = points[i].distance(points[j]);
                if (dist < minDist) {
                    minDist = dist;
                }
            }
        }
        return minDist;
    }
    
    func stripClosest(strip: List<Point>, d: float): float {
        var minDist = d;
        
        // Sort strip by y-coordinate
        this.sortByY(strip);
        
        // Check only next 7 points
        for (var i = 0; i < strip.size(); i++) {
            for (var j = i + 1; j < strip.size() && 
                 (strip[j].y - strip[i].y) < minDist; j++) {
                var dist = strip[i].distance(strip[j]);
                if (dist < minDist) {
                    minDist = dist;
                }
            }
        }
        
        return minDist;
    }
    
    func sortByX(points: List<Point>) {
        // Bubble sort by x-coordinate
        for (var i = 0; i < points.size() - 1; i++) {
            for (var j = 0; j < points.size() - i - 1; j++) {
                if (points[j].x > points[j + 1].x) {
                    var temp = points[j];
                    points[j] = points[j + 1];
                    points[j + 1] = temp;
                }
            }
        }
    }
    
    func sortByY(points: List<Point>) {
        // Bubble sort by y-coordinate
        for (var i = 0; i < points.size() - 1; i++) {
            for (var j = 0; j < points.size() - i - 1; j++) {
                if (points[j].y > points[j + 1].y) {
                    var temp = points[j];
                    points[j] = points[j + 1];
                    points[j + 1] = temp;
                }
            }
        }
    }
}
```

---

## Network Flow

### Maximum Flow (Ford-Fulkerson with BFS)

The Ford-Fulkerson method computes maximum flow in a flow network. Using BFS (Edmonds-Karp algorithm) guarantees O(VE²) complexity.

```klang
class MaxFlow {
    var graph: List<List<int>>;
    var vertices: int;
    
    func init(v: int) {
        this.vertices = v;
        this.graph = List<List<int>>();
        for (var i = 0; i < v; i++) {
            this.graph.add(List<int>());
            for (var j = 0; j < v; j++) {
                this.graph[i].add(0);
            }
        }
    }
    
    func addEdge(u: int, v: int, capacity: int) {
        this.graph[u][v] = capacity;
    }
    
    // BFS to find augmenting path
    func bfs(source: int, sink: int, parent: List<int>): bool {
        var visited = List<bool>();
        for (var i = 0; i < this.vertices; i++) {
            visited.add(false);
            parent[i] = -1;
        }
        
        var queue = Queue<int>();
        queue.enqueue(source);
        visited[source] = true;
        
        while (!queue.isEmpty()) {
            var u = queue.dequeue();
            
            for (var v = 0; v < this.vertices; v++) {
                if (!visited[v] && this.graph[u][v] > 0) {
                    queue.enqueue(v);
                    visited[v] = true;
                    parent[v] = u;
                    
                    if (v == sink) {
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
    
    // Ford-Fulkerson algorithm using BFS (Edmonds-Karp)
    func fordFulkerson(source: int, sink: int): int {
        // Create residual graph (copy of original)
        var residualGraph = List<List<int>>();
        for (var i = 0; i < this.vertices; i++) {
            residualGraph.add(List<int>());
            for (var j = 0; j < this.vertices; j++) {
                residualGraph[i].add(this.graph[i][j]);
            }
        }
        
        // Store residual graph in object temporarily
        var originalGraph = this.graph;
        this.graph = residualGraph;
        
        var parent = List<int>();
        for (var i = 0; i < this.vertices; i++) {
            parent.add(-1);
        }
        
        var maxFlow = 0;
        
        // While there exists augmenting path
        while (this.bfs(source, sink, parent)) {
            // Find minimum capacity along the path
            var pathFlow = 999999999;
            var s = sink;
            
            while (s != source) {
                var p = parent[s];
                pathFlow = min(pathFlow, this.graph[p][s]);
                s = parent[s];
            }
            
            // Update residual capacities
            var v = sink;
            while (v != source) {
                var u = parent[v];
                this.graph[u][v] -= pathFlow;
                this.graph[v][u] += pathFlow;
                v = parent[v];
            }
            
            maxFlow += pathFlow;
        }
        
        // Restore original graph
        var finalResidualGraph = this.graph;
        this.graph = originalGraph;
        
        return maxFlow;
    }
    
    // Find minimum cut
    func findMinCut(source: int, sink: int): List<Pair<int, int>> {
        var maxFlowValue = this.fordFulkerson(source, sink);
        
        // Run BFS from source in residual graph
        var visited = List<bool>();
        for (var i = 0; i < this.vertices; i++) {
            visited.add(false);
        }
        
        var parent = List<int>();
        for (var i = 0; i < this.vertices; i++) {
            parent.add(-1);
        }
        
        this.bfs(source, sink, parent);
        
        // Mark all reachable vertices
        var queue = Queue<int>();
        queue.enqueue(source);
        visited[source] = true;
        
        while (!queue.isEmpty()) {
            var u = queue.dequeue();
            for (var v = 0; v < this.vertices; v++) {
                if (!visited[v] && this.graph[u][v] > 0) {
                    visited[v] = true;
                    queue.enqueue(v);
                }
            }
        }
        
        // Find edges in min cut
        var minCut = List<Pair<int, int>>();
        for (var i = 0; i < this.vertices; i++) {
            for (var j = 0; j < this.vertices; j++) {
                if (visited[i] && !visited[j] && this.graph[i][j] > 0) {
                    minCut.add(Pair<int, int>(i, j));
                }
            }
        }
        
        return minCut;
    }
}

// Bipartite Matching using Max Flow
class BipartiteMatching {
    func maxMatching(graph: List<List<int>>, m: int, n: int): int {
        // Create flow network
        // Source = 0, Left vertices = 1..m, Right vertices = m+1..m+n, Sink = m+n+1
        var vertices = m + n + 2;
        var flowGraph = MaxFlow(vertices);
        
        var source = 0;
        var sink = m + n + 1;
        
        // Add edges from source to left vertices
        for (var i = 1; i <= m; i++) {
            flowGraph.addEdge(source, i, 1);
        }
        
        // Add edges from left to right based on bipartite graph
        for (var i = 0; i < m; i++) {
            for (var j = 0; j < n; j++) {
                if (graph[i][j] == 1) {
                    flowGraph.addEdge(i + 1, m + 1 + j, 1);
                }
            }
        }
        
        // Add edges from right vertices to sink
        for (var i = 1; i <= n; i++) {
            flowGraph.addEdge(m + i, sink, 1);
        }
        
        return flowGraph.fordFulkerson(source, sink);
    }
}
```

---

## Fast Fourier Transform (FFT)

FFT is used for efficient polynomial multiplication and signal processing. It reduces complexity from O(n²) to O(n log n).

```klang
class Complex {
    var real: float;
    var imag: float;
    
    func init(r: float, i: float) {
        this.real = r;
        this.imag = i;
    }
    
    func add(other: Complex): Complex {
        return Complex(this.real + other.real, this.imag + other.imag);
    }
    
    func subtract(other: Complex): Complex {
        return Complex(this.real - other.real, this.imag - other.imag);
    }
    
    func multiply(other: Complex): Complex {
        var r = this.real * other.real - this.imag * other.imag;
        var i = this.real * other.imag + this.imag * other.real;
        return Complex(r, i);
    }
    
    func magnitude(): float {
        return sqrt(this.real * this.real + this.imag * this.imag);
    }
}

class FFT {
    var PI: float = 3.14159265358979323846;
    
    // Cooley-Tukey FFT algorithm
    func fft(x: List<Complex>, inverse: bool): List<Complex> {
        var n = x.size();
        
        if (n <= 1) {
            return x;
        }
        
        // Divide
        var even = List<Complex>();
        var odd = List<Complex>();
        
        for (var i = 0; i < n; i += 2) {
            even.add(x[i]);
            if (i + 1 < n) {
                odd.add(x[i + 1]);
            }
        }
        
        // Conquer
        even = this.fft(even, inverse);
        odd = this.fft(odd, inverse);
        
        // Combine
        var result = List<Complex>();
        for (var i = 0; i < n; i++) {
            result.add(Complex(0.0, 0.0));
        }
        
        for (var k = 0; k < n / 2; k++) {
            var angle = 2.0 * this.PI * k / n;
            if (inverse) {
                angle = -angle;
            }
            
            var w = Complex(cos(angle), sin(angle));
            var t = w.multiply(odd[k]);
            
            result[k] = even[k].add(t);
            result[k + n / 2] = even[k].subtract(t);
        }
        
        if (inverse) {
            for (var i = 0; i < n; i++) {
                result[i].real /= 2.0;
                result[i].imag /= 2.0;
            }
        }
        
        return result;
    }
    
    // Multiply two polynomials using FFT
    func multiplyPolynomials(a: List<float>, b: List<float>): List<float> {
        // Find next power of 2
        var n = 1;
        while (n < a.size() + b.size()) {
            n *= 2;
        }
        
        // Convert to complex and pad with zeros
        var ca = List<Complex>();
        var cb = List<Complex>();
        
        for (var i = 0; i < n; i++) {
            if (i < a.size()) {
                ca.add(Complex(a[i], 0.0));
            } else {
                ca.add(Complex(0.0, 0.0));
            }
            
            if (i < b.size()) {
                cb.add(Complex(b[i], 0.0));
            } else {
                cb.add(Complex(0.0, 0.0));
            }
        }
        
        // Apply FFT
        ca = this.fft(ca, false);
        cb = this.fft(cb, false);
        
        // Point-wise multiplication
        var cc = List<Complex>();
        for (var i = 0; i < n; i++) {
            cc.add(ca[i].multiply(cb[i]));
        }
        
        // Inverse FFT
        cc = this.fft(cc, true);
        
        // Extract real parts
        var result = List<float>();
        for (var i = 0; i < a.size() + b.size() - 1; i++) {
            result.add(cc[i].real);
        }
        
        return result;
    }
}
```

---

## Randomized Algorithms

### Quick Select (Kth Smallest Element)

```klang
class QuickSelect {
    // Find kth smallest element (0-indexed)
    func findKthSmallest(arr: List<int>, k: int): int {
        return this.quickSelect(arr, 0, arr.size() - 1, k);
    }
    
    func quickSelect(arr: List<int>, left: int, right: int, k: int): int {
        if (left == right) {
            return arr[left];
        }
        
        // Random pivot for average O(n)
        var pivotIndex = left + random() % (right - left + 1);
        pivotIndex = this.partition(arr, left, right, pivotIndex);
        
        if (k == pivotIndex) {
            return arr[k];
        } else if (k < pivotIndex) {
            return this.quickSelect(arr, left, pivotIndex - 1, k);
        } else {
            return this.quickSelect(arr, pivotIndex + 1, right, k);
        }
    }
    
    func partition(arr: List<int>, left: int, right: int, pivotIndex: int): int {
        var pivotValue = arr[pivotIndex];
        
        // Move pivot to end
        this.swap(arr, pivotIndex, right);
        
        var storeIndex = left;
        for (var i = left; i < right; i++) {
            if (arr[i] < pivotValue) {
                this.swap(arr, i, storeIndex);
                storeIndex++;
            }
        }
        
        // Move pivot to final position
        this.swap(arr, storeIndex, right);
        return storeIndex;
    }
    
    func swap(arr: List<int>, i: int, j: int) {
        var temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}
```

### Reservoir Sampling

```klang
class ReservoirSampling {
    // Select k random elements from stream
    func sample(stream: List<int>, k: int): List<int> {
        var reservoir = List<int>();
        
        // Fill reservoir with first k elements
        for (var i = 0; i < k && i < stream.size(); i++) {
            reservoir.add(stream[i]);
        }
        
        // Process remaining elements
        for (var i = k; i < stream.size(); i++) {
            // Random index from 0 to i
            var j = random() % (i + 1);
            
            // Replace element with decreasing probability
            if (j < k) {
                reservoir[j] = stream[i];
            }
        }
        
        return reservoir;
    }
    
    // Weighted reservoir sampling
    func weightedSample(items: List<int>, weights: List<float>, k: int): List<int> {
        var reservoir = List<Pair<int, float>>();
        
        for (var i = 0; i < items.size(); i++) {
            var key = pow(random() / 999999999.0, 1.0 / weights[i]);
            
            if (reservoir.size() < k) {
                reservoir.add(Pair<int, float>(items[i], key));
            } else {
                // Find minimum key in reservoir
                var minIdx = 0;
                for (var j = 1; j < reservoir.size(); j++) {
                    if (reservoir[j].second < reservoir[minIdx].second) {
                        minIdx = j;
                    }
                }
                
                if (key > reservoir[minIdx].second) {
                    reservoir[minIdx] = Pair<int, float>(items[i], key);
                }
            }
        }
        
        var result = List<int>();
        for (var i = 0; i < reservoir.size(); i++) {
            result.add(reservoir[i].first);
        }
        return result;
    }
}
```

### Monte Carlo Methods

```klang
class MonteCarlo {
    // Estimate PI using Monte Carlo
    func estimatePi(samples: int): float {
        var insideCircle = 0;
        
        for (var i = 0; i < samples; i++) {
            var x = random() / 999999999.0; // Random between 0 and 1
            var y = random() / 999999999.0;
            
            if (x * x + y * y <= 1.0) {
                insideCircle++;
            }
        }
        
        return 4.0 * insideCircle / samples;
    }
    
    // Estimate integral using Monte Carlo
    func estimateIntegral(f: func(float): float, a: float, b: float, samples: int): float {
        var sum = 0.0;
        
        for (var i = 0; i < samples; i++) {
            var x = a + (b - a) * random() / 999999999.0;
            sum += f(x);
        }
        
        return (b - a) * sum / samples;
    }
}
```

---

## Bit Manipulation Algorithms

### Advanced Bit Operations

```klang
class BitManipulation {
    // Count set bits (Brian Kernighan's algorithm)
    func countSetBits(n: int): int {
        var count = 0;
        while (n > 0) {
            n &= (n - 1); // Clear least significant set bit
            count++;
        }
        return count;
    }
    
    // Check if power of two
    func isPowerOfTwo(n: int): bool {
        return n > 0 && (n & (n - 1)) == 0;
    }
    
    // Find next power of two
    func nextPowerOfTwo(n: int): int {
        n--;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        return n + 1;
    }
    
    // Find position of rightmost set bit
    func positionOfRightmostSetBit(n: int): int {
        if (n == 0) return -1;
        return this.countSetBits((n & -n) - 1);
    }
    
    // XOR of all elements except self
    func singleNumber(nums: List<int>): int {
        var result = 0;
        for (var num in nums) {
            result ^= num;
        }
        return result;
    }
    
    // Find two unique numbers in array where all others appear twice
    func findTwoUniqueNumbers(nums: List<int>): Pair<int, int> {
        var xorAll = 0;
        for (var num in nums) {
            xorAll ^= num;
        }
        
        // Find rightmost set bit
        var rightmostBit = xorAll & -xorAll;
        
        var num1 = 0;
        var num2 = 0;
        
        for (var num in nums) {
            if ((num & rightmostBit) != 0) {
                num1 ^= num;
            } else {
                num2 ^= num;
            }
        }
        
        return Pair<int, int>(num1, num2);
    }
    
    // Generate all subsets using bits
    func generateSubsets(nums: List<int>): List<List<int>> {
        var result = List<List<int>>();
        var n = nums.size();
        var totalSubsets = 1 << n; // 2^n
        
        for (var i = 0; i < totalSubsets; i++) {
            var subset = List<int>();
            for (var j = 0; j < n; j++) {
                if ((i & (1 << j)) != 0) {
                    subset.add(nums[j]);
                }
            }
            result.add(subset);
        }
        
        return result;
    }
    
    // Reverse bits
    func reverseBits(n: int): int {
        var result = 0;
        for (var i = 0; i < 32; i++) {
            result <<= 1;
            result |= (n & 1);
            n >>= 1;
        }
        return result;
    }
    
    // Count bits required to convert A to B
    func bitSwapRequired(a: int, b: int): int {
        return this.countSetBits(a ^ b);
    }
}
```

---

## Interview Problems

### 1. Shortest Path in Binary Matrix

```klang
class ShortestPathBinaryMatrix {
    // Find shortest path from top-left to bottom-right
    // Can move in 8 directions
    func shortestPath(grid: List<List<int>>): int {
        if (grid.size() == 0 || grid[0][0] == 1) {
            return -1;
        }
        
        var n = grid.size();
        if (grid[n-1][n-1] == 1) {
            return -1;
        }
        
        var directions = [[−1,−1], [−1,0], [−1,1], [0,−1], [0,1], [1,−1], [1,0], [1,1]];
        var queue = Queue<Tuple<int, int, int>>(); // row, col, distance
        queue.enqueue(Tuple<int, int, int>(0, 0, 1));
        grid[0][0] = 1; // Mark visited
        
        while (!queue.isEmpty()) {
            var current = queue.dequeue();
            var row = current.first;
            var col = current.second;
            var dist = current.third;
            
            if (row == n - 1 && col == n - 1) {
                return dist;
            }
            
            for (var dir in directions) {
                var newRow = row + dir[0];
                var newCol = col + dir[1];
                
                if (newRow >= 0 && newRow < n && newCol >= 0 && newCol < n && 
                    grid[newRow][newCol] == 0) {
                    grid[newRow][newCol] = 1;
                    queue.enqueue(Tuple<int, int, int>(newRow, newCol, dist + 1));
                }
            }
        }
        
        return -1;
    }
}
```

### 2. Sliding Window Maximum

```klang
class SlidingWindowMaximum {
    // Find maximum in each sliding window of size k
    func maxSlidingWindow(nums: List<int>, k: int): List<int> {
        if (nums.size() == 0 || k == 0) {
            return List<int>();
        }
        
        var result = List<int>();
        var deque = Deque<int>(); // Store indices
        
        for (var i = 0; i < nums.size(); i++) {
            // Remove elements outside window
            while (!deque.isEmpty() && deque.front() < i - k + 1) {
                deque.removeFront();
            }
            
            // Remove smaller elements (they'll never be maximum)
            while (!deque.isEmpty() && nums[deque.back()] < nums[i]) {
                deque.removeBack();
            }
            
            deque.addBack(i);
            
            // Add to result after first window is complete
            if (i >= k - 1) {
                result.add(nums[deque.front()]);
            }
        }
        
        return result;
    }
}
```

### 3. Trapping Rain Water

```klang
class TrappingRainWater {
    // Calculate trapped rain water
    func trap(height: List<int>): int {
        if (height.size() == 0) {
            return 0;
        }
        
        var left = 0;
        var right = height.size() - 1;
        var leftMax = 0;
        var rightMax = 0;
        var water = 0;
        
        while (left < right) {
            if (height[left] < height[right]) {
                if (height[left] >= leftMax) {
                    leftMax = height[left];
                } else {
                    water += leftMax - height[left];
                }
                left++;
            } else {
                if (height[right] >= rightMax) {
                    rightMax = height[right];
                } else {
                    water += rightMax - height[right];
                }
                right--;
            }
        }
        
        return water;
    }
    
    // Using stacks
    func trapUsingStack(height: List<int>): int {
        var stack = Stack<int>();
        var water = 0;
        
        for (var i = 0; i < height.size(); i++) {
            while (!stack.isEmpty() && height[i] > height[stack.peek()]) {
                var top = stack.pop();
                
                if (stack.isEmpty()) {
                    break;
                }
                
                var distance = i - stack.peek() - 1;
                var boundedHeight = min(height[i], height[stack.peek()]) - height[top];
                water += distance * boundedHeight;
            }
            
            stack.push(i);
        }
        
        return water;
    }
}
```

### 4. The Skyline Problem

```klang
class SkylineProblem {
    // Find skyline from building coordinates
    func getSkyline(buildings: List<List<int>>): List<Pair<int, int>> {
        var events = List<Pair<int, int>>(); // (x-coordinate, height)
        
        // Create events for building start and end
        for (var building in buildings) {
            var left = building[0];
            var right = building[1];
            var height = building[2];
            
            events.add(Pair<int, int>(left, -height)); // Negative for start
            events.add(Pair<int, int>(right, height));  // Positive for end
        }
        
        // Sort events
        this.sortEvents(events);
        
        var result = List<Pair<int, int>>();
        var heights = List<int>(); // Max heap simulation
        heights.add(0);
        var prevMax = 0;
        
        for (var event in events) {
            var x = event.first;
            var h = event.second;
            
            if (h < 0) {
                heights.add(-h); // Building start
            } else {
                heights.remove(h); // Building end
            }
            
            var currentMax = this.findMax(heights);
            
            if (currentMax != prevMax) {
                result.add(Pair<int, int>(x, currentMax));
                prevMax = currentMax;
            }
        }
        
        return result;
    }
    
    func sortEvents(events: List<Pair<int, int>>) {
        // Bubble sort for demonstration
        for (var i = 0; i < events.size() - 1; i++) {
            for (var j = 0; j < events.size() - i - 1; j++) {
                if (events[j].first > events[j + 1].first ||
                    (events[j].first == events[j + 1].first && 
                     events[j].second > events[j + 1].second)) {
                    var temp = events[j];
                    events[j] = events[j + 1];
                    events[j + 1] = temp;
                }
            }
        }
    }
    
    func findMax(heights: List<int>): int {
        var max = 0;
        for (var h in heights) {
            if (h > max) {
                max = h;
            }
        }
        return max;
    }
}
```

### 5. Number of Islands II (Dynamic)

```klang
class NumberOfIslandsII {
    // Count islands as land is added dynamically
    func numIslands2(m: int, n: int, positions: List<Pair<int, int>>): List<int> {
        var result = List<int>();
        var uf = UnionFind(m * n);
        var grid = List<List<bool>>();
        
        for (var i = 0; i < m; i++) {
            grid.add(List<bool>());
            for (var j = 0; j < n; j++) {
                grid[i].add(false);
            }
        }
        
        var directions = [[0, 1], [1, 0], [0, -1], [-1, 0]];
        var count = 0;
        
        for (var pos in positions) {
            var r = pos.first;
            var c = pos.second;
            
            if (grid[r][c]) {
                result.add(count);
                continue;
            }
            
            grid[r][c] = true;
            count++;
            
            var id = r * n + c;
            
            // Check 4 neighbors
            for (var dir in directions) {
                var nr = r + dir[0];
                var nc = c + dir[1];
                
                if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc]) {
                    var nid = nr * n + nc;
                    if (uf.union(id, nid)) {
                        count--;
                    }
                }
            }
            
            result.add(count);
        }
        
        return result;
    }
}

class UnionFind {
    var parent: List<int>;
    var rank: List<int>;
    
    func init(size: int) {
        this.parent = List<int>();
        this.rank = List<int>();
        for (var i = 0; i < size; i++) {
            this.parent.add(i);
            this.rank.add(0);
        }
    }
    
    func find(x: int): int {
        if (this.parent[x] != x) {
            this.parent[x] = this.find(this.parent[x]);
        }
        return this.parent[x];
    }
    
    func union(x: int, y: int): bool {
        var rootX = this.find(x);
        var rootY = this.find(y);
        
        if (rootX == rootY) {
            return false;
        }
        
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
```

### 6. Maximum Rectangle in Histogram

```klang
class MaxRectangleHistogram {
    // Find largest rectangle in histogram
    func largestRectangleArea(heights: List<int>): int {
        var stack = Stack<int>();
        var maxArea = 0;
        var i = 0;
        
        while (i < heights.size()) {
            if (stack.isEmpty() || heights[i] >= heights[stack.peek()]) {
                stack.push(i);
                i++;
            } else {
                var top = stack.pop();
                var width = stack.isEmpty() ? i : i - stack.peek() - 1;
                var area = heights[top] * width;
                maxArea = max(maxArea, area);
            }
        }
        
        while (!stack.isEmpty()) {
            var top = stack.pop();
            var width = stack.isEmpty() ? i : i - stack.peek() - 1;
            var area = heights[top] * width;
            maxArea = max(maxArea, area);
        }
        
        return maxArea;
    }
}
```

### 7. Maximal Rectangle in Binary Matrix

```klang
class MaximalRectangle {
    // Find largest rectangle in binary matrix
    func maximalRectangle(matrix: List<List<char>>): int {
        if (matrix.size() == 0) {
            return 0;
        }
        
        var m = matrix.size();
        var n = matrix[0].size();
        var heights = List<int>();
        
        for (var i = 0; i < n; i++) {
            heights.add(0);
        }
        
        var maxArea = 0;
        var histSolver = MaxRectangleHistogram();
        
        for (var i = 0; i < m; i++) {
            for (var j = 0; j < n; j++) {
                if (matrix[i][j] == '1') {
                    heights[j]++;
                } else {
                    heights[j] = 0;
                }
            }
            
            var area = histSolver.largestRectangleArea(heights);
            maxArea = max(maxArea, area);
        }
        
        return maxArea;
    }
}
```

### 8. Alien Dictionary

```klang
class AlienDictionary {
    // Find order of characters in alien language
    func alienOrder(words: List<String>): String {
        var graph = Map<char, Set<char>>();
        var inDegree = Map<char, int>();
        
        // Initialize graph
        for (var word in words) {
            for (var c in word) {
                if (!graph.containsKey(c)) {
                    graph.set(c, Set<char>());
                    inDegree.set(c, 0);
                }
            }
        }
        
        // Build graph
        for (var i = 0; i < words.size() - 1; i++) {
            var word1 = words[i];
            var word2 = words[i + 1];
            var minLen = min(word1.length(), word2.length());
            
            // Invalid case: prefix comes after
            if (word1.length() > word2.length() && 
                word1.substring(0, minLen) == word2.substring(0, minLen)) {
                return "";
            }
            
            for (var j = 0; j < minLen; j++) {
                if (word1[j] != word2[j]) {
                    if (!graph.get(word1[j]).contains(word2[j])) {
                        graph.get(word1[j]).add(word2[j]);
                        inDegree.set(word2[j], inDegree.get(word2[j]) + 1);
                    }
                    break;
                }
            }
        }
        
        // Topological sort
        var queue = Queue<char>();
        for (var entry in inDegree) {
            if (entry.value == 0) {
                queue.enqueue(entry.key);
            }
        }
        
        var result = "";
        while (!queue.isEmpty()) {
            var c = queue.dequeue();
            result += c;
            
            for (var neighbor in graph.get(c)) {
                inDegree.set(neighbor, inDegree.get(neighbor) - 1);
                if (inDegree.get(neighbor) == 0) {
                    queue.enqueue(neighbor);
                }
            }
        }
        
        return result.length() == inDegree.size() ? result : "";
    }
}
```

### 9. Reconstruct Itinerary

```klang
class ReconstructItinerary {
    // Find itinerary from tickets using Eulerian path
    func findItinerary(tickets: List<Pair<String, String>>): List<String> {
        var graph = Map<String, List<String>>();
        
        // Build graph
        for (var ticket in tickets) {
            if (!graph.containsKey(ticket.first)) {
                graph.set(ticket.first, List<String>());
            }
            graph.get(ticket.first).add(ticket.second);
        }
        
        // Sort destinations
        for (var entry in graph) {
            this.sortList(entry.value);
        }
        
        var result = List<String>();
        this.dfs("JFK", graph, result);
        
        result.reverse();
        return result;
    }
    
    func dfs(airport: String, graph: Map<String, List<String>>, result: List<String>) {
        if (graph.containsKey(airport)) {
            var destinations = graph.get(airport);
            while (destinations.size() > 0) {
                var next = destinations[0];
                destinations.removeAt(0);
                this.dfs(next, graph, result);
            }
        }
        result.add(airport);
    }
    
    func sortList(list: List<String>) {
        for (var i = 0; i < list.size() - 1; i++) {
            for (var j = 0; j < list.size() - i - 1; j++) {
                if (list[j] > list[j + 1]) {
                    var temp = list[j];
                    list[j] = list[j + 1];
                    list[j + 1] = temp;
                }
            }
        }
    }
}
```

### 10. Cherry Pickup

```klang
class CherryPickup {
    // Maximum cherries collected going down and back up
    func cherryPickup(grid: List<List<int>>): int {
        var n = grid.size();
        var dp = List<List<List<int>>>();
        
        // Initialize 3D DP array
        for (var i = 0; i < n; i++) {
            dp.add(List<List<int>>());
            for (var j = 0; j < n; j++) {
                dp[i].add(List<int>());
                for (var k = 0; k < n; k++) {
                    dp[i][j].add(-1);
                }
            }
        }
        
        var result = this.solve(grid, 0, 0, 0, dp);
        return max(0, result);
    }
    
    func solve(grid: List<List<int>>, r1: int, c1: int, c2: int, 
               dp: List<List<List<int>>>): int {
        var r2 = r1 + c1 - c2;
        var n = grid.size();
        
        // Out of bounds or thorn
        if (r1 >= n || r2 >= n || c1 >= n || c2 >= n || 
            grid[r1][c1] == -1 || grid[r2][c2] == -1) {
            return -999999999;
        }
        
        // Reached destination
        if (r1 == n - 1 && c1 == n - 1) {
            return grid[r1][c1];
        }
        
        // Memoized
        if (dp[r1][c1][c2] != -1) {
            return dp[r1][c1][c2];
        }
        
        var cherries = grid[r1][c1];
        if (c1 != c2) {
            cherries += grid[r2][c2];
        }
        
        // Try all 4 combinations of moves
        var best = max(
            max(this.solve(grid, r1 + 1, c1, c2, dp),
                this.solve(grid, r1, c1 + 1, c2, dp)),
            max(this.solve(grid, r1 + 1, c1, c2 + 1, dp),
                this.solve(grid, r1, c1 + 1, c2 + 1, dp))
        );
        
        cherries += best;
        dp[r1][c1][c2] = cherries;
        
        return cherries;
    }
}
```

### 11. Count of Range Sum

```klang
class CountRangeSum {
    // Count range sums in [lower, upper]
    func countRangeSum(nums: List<int>, lower: int, upper: int): int {
        var n = nums.size();
        var prefixSum = List<long>();
        prefixSum.add(0);
        
        for (var i = 0; i < n; i++) {
            prefixSum.add(prefixSum[i] + nums[i]);
        }
        
        return this.countWhileMergeSort(prefixSum, 0, n, lower, upper);
    }
    
    func countWhileMergeSort(sums: List<long>, start: int, end: int, 
                             lower: int, upper: int): int {
        if (end - start <= 1) {
            return 0;
        }
        
        var mid = start + (end - start) / 2;
        var count = this.countWhileMergeSort(sums, start, mid, lower, upper) + 
                    this.countWhileMergeSort(sums, mid, end, lower, upper);
        
        var j = mid;
        var k = mid;
        var t = mid;
        var cache = List<long>();
        
        for (var i = start; i < mid; i++) {
            while (k < end && sums[k] - sums[i] < lower) k++;
            while (j < end && sums[j] - sums[i] <= upper) j++;
            count += j - k;
        }
        
        // Merge
        var i = start;
        j = mid;
        while (i < mid && j < end) {
            if (sums[i] < sums[j]) {
                cache.add(sums[i]);
                i++;
            } else {
                cache.add(sums[j]);
                j++;
            }
        }
        
        while (i < mid) {
            cache.add(sums[i]);
            i++;
        }
        while (j < end) {
            cache.add(sums[j]);
            j++;
        }
        
        for (i = 0; i < cache.size(); i++) {
            sums[start + i] = cache[i];
        }
        
        return count;
    }
}
```

### 12. Reverse Pairs

```klang
class ReversePairs {
    // Count pairs where i < j and nums[i] > 2 * nums[j]
    func reversePairs(nums: List<int>): int {
        return this.mergeSort(nums, 0, nums.size() - 1);
    }
    
    func mergeSort(nums: List<int>, left: int, right: int): int {
        if (left >= right) {
            return 0;
        }
        
        var mid = left + (right - left) / 2;
        var count = this.mergeSort(nums, left, mid) + 
                    this.mergeSort(nums, mid + 1, right);
        
        // Count reverse pairs
        var j = mid + 1;
        for (var i = left; i <= mid; i++) {
            while (j <= right && nums[i] > 2 * nums[j]) {
                j++;
            }
            count += j - (mid + 1);
        }
        
        // Merge
        var temp = List<int>();
        var i = left;
        j = mid + 1;
        
        while (i <= mid && j <= right) {
            if (nums[i] <= nums[j]) {
                temp.add(nums[i]);
                i++;
            } else {
                temp.add(nums[j]);
                j++;
            }
        }
        
        while (i <= mid) {
            temp.add(nums[i]);
            i++;
        }
        while (j <= right) {
            temp.add(nums[j]);
            j++;
        }
        
        for (i = 0; i < temp.size(); i++) {
            nums[left + i] = temp[i];
        }
        
        return count;
    }
}
```

---

## Comparison Tables

### When to Use Each Advanced Data Structure

| Data Structure | Best For | Time Complexity | Space | Strengths |
|---------------|----------|-----------------|-------|-----------|
| **Segment Tree** | Range queries/updates | O(log n) query/update | O(n) | Flexible, supports many operations |
| **Fenwick Tree** | Prefix sums, cumulative frequency | O(log n) | O(n) | Simpler, less space than SegTree |
| **Trie** | String prefix searches | O(m) m=length | O(ALPHABET * n * m) | Fast prefix operations |
| **Suffix Array** | String pattern matching | O(n log n) build | O(n) | Space-efficient string search |
| **Disjoint Set** | Connected components | O(α(n)) ≈ O(1) | O(n) | Union-find operations |

### Algorithm Selection Guide

| Problem Type | Algorithm | Complexity | When to Use |
|-------------|-----------|------------|-------------|
| **All-pairs shortest path** | Floyd-Warshall | O(V³) | Dense graphs, need all paths |
| **Single-source shortest path** | Dijkstra | O((V+E) log V) | Non-negative weights |
| **Network flow** | Ford-Fulkerson | O(E * max_flow) | Matching, min-cut problems |
| **Computational geometry** | Graham Scan | O(n log n) | Convex hull |
| **String matching** | KMP | O(n + m) | Pattern search |
| **Polynomial multiplication** | FFT | O(n log n) | Signal processing |

---

## Best Practices

### 1. Choosing Advanced Algorithms

```klang
// Decision tree for complex problems
class AlgorithmSelector {
    func selectAlgorithm(problemType: String): String {
        if (problemType == "graph_all_pairs") {
            return "Floyd-Warshall for dense graphs, Johnson's for sparse";
        }
        
        if (problemType == "range_query") {
            return "Segment Tree for updates, Fenwick for sums only";
        }
        
        if (problemType == "geometry") {
            return "Sweep line for intersections, Convex hull for boundaries";
        }
        
        if (problemType == "optimization") {
            return "DP for overlapping subproblems, Greedy if locally optimal";
        }
        
        return "Analyze constraints and choose appropriate technique";
    }
}
```

### 2. Optimization Techniques

- **Memoization**: Cache expensive computations
- **Lazy Propagation**: Defer updates in segment trees
- **Coordinate Compression**: Reduce space for sparse data
- **Bit Manipulation**: Fast operations for boolean logic
- **Two Pointers**: Linear scan instead of nested loops

### 3. Testing Advanced Algorithms

```klang
class AdvancedTesting {
    func testFloydWarshall() {
        var fw = FloydWarshall();
        
        // Test case 1: Simple graph
        var graph1 = [[0, 3, 999999999, 7],
                      [8, 0, 2, 999999999],
                      [5, 999999999, 0, 1],
                      [2, 999999999, 999999999, 0]];
        
        var dist1 = fw.findAllPairsShortestPaths(graph1);
        assert(dist1[0][1] == 3, "Test 1 failed");
        
        // Test case 2: Negative weights
        var graph2 = [[0, 1, 999999999],
                      [999999999, 0, -1],
                      [-1, 999999999, 0]];
        
        assert(!fw.hasNegativeCycle(graph2), "Test 2 failed");
        
        println("All Floyd-Warshall tests passed!");
    }
}
```

---

## When to Use Advanced Algorithms

### Recognition Patterns

1. **Floyd-Warshall**
   - Need all-pairs shortest paths
   - Graph is dense (V² edges)
   - Negative edges present (but no negative cycles)

2. **Segment Tree**
   - Frequent range queries and updates
   - Need to support multiple query types
   - Array size is reasonable (≤10⁶)

3. **Network Flow**
   - Matching problems (bipartite, maximum)
   - Min-cut/max-flow problems
   - Resource allocation with capacities

4. **Computational Geometry**
   - Points and lines in 2D/3D space
   - Convex hull, closest pairs
   - Intersection detection

5. **FFT**
   - Polynomial multiplication
   - Signal processing
   - Convolution operations

6. **Randomized Algorithms**
   - Need expected performance guarantees
   - Deterministic solution too complex
   - Streaming data (reservoir sampling)

---

## Common Pitfalls

### 1. Floyd-Warshall Mistakes

```klang
// ❌ Wrong: Incorrect loop order
for (var i = 0; i < n; i++) {
    for (var j = 0; j < n; j++) {
        for (var k = 0; k < n; k++) {
            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
        }
    }
}

// ✅ Correct: k must be outermost loop
for (var k = 0; k < n; k++) {
    for (var i = 0; i < n; i++) {
        for (var j = 0; j < n; j++) {
            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
        }
    }
}
```

### 2. Segment Tree Edge Cases

```klang
// ❌ Wrong: Not handling empty ranges
func query(l: int, r: int): int {
    return this.queryHelper(0, 0, this.n - 1, l, r);
}

// ✅ Correct: Validate input
func query(l: int, r: int): int {
    if (l < 0 || r >= this.n || l > r) {
        return 0; // or throw error
    }
    return this.queryHelper(0, 0, this.n - 1, l, r);
}
```

### 3. Bit Manipulation Overflow

```klang
// ❌ Wrong: May overflow
func generateSubsets(n: int) {
    var total = 1 << n; // Overflow if n > 30
}

// ✅ Correct: Check bounds
func generateSubsets(n: int) {
    if (n > 30) {
        throw Error("n too large, would overflow");
    }
    var total = 1 << n;
}
```

### 4. Geometry Precision Issues

```klang
// ❌ Wrong: Direct floating point comparison
if (distance == 0.0) {
    return true;
}

// ✅ Correct: Use epsilon for comparison
var EPSILON = 0.0001;
if (abs(distance) < EPSILON) {
    return true;
}
```

---

## Real-World Applications

### 1. Floyd-Warshall
- **Network Routing**: Finding optimal paths between all router pairs
- **Social Networks**: Computing degrees of separation
- **Game Theory**: Solving multi-player strategic games

### 2. Segment Trees
- **Database Systems**: Range aggregate queries
- **Graphics**: Ray tracing, collision detection
- **Financial Systems**: Time-series range analysis

### 3. Network Flow
- **Transportation**: Maximum flow in road networks
- **Job Assignment**: Bipartite matching problems
- **Image Segmentation**: Min-cut algorithms

### 4. Computational Geometry
- **Computer Graphics**: Rendering, clipping
- **Robotics**: Path planning, obstacle avoidance
- **GIS Systems**: Spatial queries, map overlay

### 5. FFT
- **Audio Processing**: Compression, filtering
- **Image Processing**: Convolution, filtering
- **Telecommunications**: Signal modulation

### 6. Randomized Algorithms
- **Data Streaming**: Sampling from large datasets
- **Load Balancing**: Random server selection
- **Monte Carlo Simulations**: Risk analysis, physics

---

## Summary

Advanced algorithms combine sophisticated techniques to solve complex computational problems:

1. **Floyd-Warshall** computes all-pairs shortest paths in O(V³)
2. **Segment Trees** enable efficient range queries and updates
3. **Fenwick Trees** provide simple prefix sum operations
4. **Computational Geometry** solves spatial problems
5. **Network Flow** optimizes resource allocation
6. **FFT** accelerates polynomial multiplication
7. **Randomized Algorithms** provide probabilistic solutions
8. **Bit Manipulation** optimizes boolean operations

**Key Takeaways**:
- Choose algorithms based on problem constraints
- Understand trade-offs between time and space
- Test edge cases thoroughly
- Consider numerical stability for geometry/FFT
- Use appropriate data structures for the task

These advanced techniques are essential for competitive programming, technical interviews, and building high-performance systems. Master them through practice and understanding their theoretical foundations.

---

**Next Steps**: Practice implementing these algorithms, solve contest problems, and explore advanced optimizations like lazy propagation, persistent data structures, and advanced graph algorithms (Hopcroft-Karp, Hungarian algorithm).
