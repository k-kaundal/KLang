# Greedy Algorithms

## Table of Contents
- [Introduction](#introduction)
- [Greedy vs Dynamic Programming](#greedy-vs-dynamic-programming)
- [Greedy Fundamentals](#greedy-fundamentals)
- [Classic Greedy Problems](#classic-greedy-problems)
- [Greedy Patterns](#greedy-patterns)
- [Interval Problems](#interval-problems)
- [Greedy on Arrays](#greedy-on-arrays)
- [Greedy with Strings](#greedy-with-strings)
- [Interview Problems](#interview-problems)
- [When Greedy Works](#when-greedy-works)
- [When Greedy Fails](#when-greedy-fails)
- [Best Practices](#best-practices)
- [Proof Techniques](#proof-techniques)

---

## Introduction

**Greedy algorithms** make locally optimal choices at each step with the hope of finding a global optimum. Unlike dynamic programming, greedy algorithms don't reconsider previous choices—they commit to each decision immediately.

### Key Characteristics

1. **Greedy Choice Property**: A globally optimal solution can be arrived at by making locally optimal choices
2. **Optimal Substructure**: An optimal solution contains optimal solutions to subproblems
3. **No Backtracking**: Once a choice is made, it's never reconsidered
4. **Efficiency**: Usually faster than DP (O(n log n) vs O(n²) typically)

### When Greedy Works

Greedy algorithms work when:
- The problem exhibits the greedy choice property
- Local optimization leads to global optimization
- Making the best choice at each step doesn't prevent finding the overall best solution

```klang
// General Greedy Algorithm Template
function greedy_solution(input: [int]) -> int {
    // 1. Sort/Prepare data (if needed)
    sort(input);
    
    // 2. Initialize result
    let result: int = 0;
    
    // 3. Make greedy choices
    for item in input {
        if (is_valid_choice(item)) {
            result = result + item;
        }
    }
    
    return result;
}
```

---

## Greedy vs Dynamic Programming

| Aspect | Greedy | Dynamic Programming |
|--------|--------|---------------------|
| **Choice** | Makes one choice per step | Explores all choices |
| **Backtracking** | No | Yes (implicitly) |
| **Optimality** | Not always optimal | Always optimal (if done correctly) |
| **Time Complexity** | Usually O(n log n) | Usually O(n²) or higher |
| **Space Complexity** | O(1) or O(n) | O(n) or O(n²) |
| **Example** | Activity Selection | 0/1 Knapsack |

### Example: Coin Change

**Greedy Works** (US coins: 25, 10, 5, 1):
```klang
function coin_change_greedy(amount: int, coins: [int]) -> int {
    sort_descending(coins);
    let count: int = 0;
    
    for coin in coins {
        while (amount >= coin) {
            amount = amount - coin;
            count = count + 1;
        }
    }
    
    return count;
}

// For amount = 30 with [25, 10, 5, 1]: 25 + 5 = 2 coins ✓
```

**Greedy Fails** (arbitrary coins: 25, 10, 1):
```klang
// For amount = 30:
// Greedy: 25 + 1 + 1 + 1 + 1 + 1 = 6 coins ✗
// Optimal: 10 + 10 + 10 = 3 coins ✓
// Need DP for general case!
```

---

## Greedy Fundamentals

### Greedy Choice Property

A problem has the **greedy choice property** if making locally optimal choices leads to a globally optimal solution.

**Proof Strategy**:
1. Assume there's an optimal solution that differs from the greedy choice
2. Show that swapping to the greedy choice doesn't worsen the solution
3. Conclude the greedy solution is also optimal

### Optimal Substructure

After making a greedy choice, the remaining problem should be a smaller instance of the same problem.

**Example**: In Activity Selection, after choosing the earliest-ending activity, the remaining problem is to select from activities that don't overlap with it.

### Proof of Correctness

```klang
// Proving Activity Selection is Correct
/*
Theorem: Always choosing the activity that finishes earliest 
         produces an optimal solution.

Proof (Exchange Argument):
1. Let A = {a1, a2, ..., ak} be our greedy solution (sorted by finish time)
2. Let O = {o1, o2, ..., om} be any optimal solution
3. If A ≠ O, let ai be the first activity where they differ
4. Since ai finishes earliest, we can replace oi with ai in O
5. This doesn't conflict with later activities in O
6. Therefore, O' is also optimal, and agrees with A longer
7. By induction, A is optimal
*/
```

---

## Classic Greedy Problems

### Activity Selection

**Problem**: Given n activities with start and finish times, select the maximum number of non-overlapping activities.

```klang
struct Activity {
    start: int,
    finish: int,
    index: int
}

function activity_selection(activities: [Activity]) -> [int] {
    // Sort by finish time (greedy choice)
    sort_by_finish_time(activities);
    
    let selected: [int] = [];
    let last_finish: int = -1;
    
    for activity in activities {
        if (activity.start >= last_finish) {
            selected.push(activity.index);
            last_finish = activity.finish;
        }
    }
    
    return selected;
}

// Example
function test_activity_selection() {
    let activities: [Activity] = [
        Activity { start: 1, finish: 3, index: 0 },
        Activity { start: 2, finish: 4, index: 1 },
        Activity { start: 3, finish: 5, index: 2 },
        Activity { start: 0, finish: 6, index: 3 },
        Activity { start: 5, finish: 7, index: 4 },
        Activity { start: 8, finish: 9, index: 5 }
    ];
    
    let result: [int] = activity_selection(activities);
    // Output: [0, 2, 4, 5] - activities that don't overlap
    print("Selected activities: ");
    print(result);
}
```

**Time Complexity**: O(n log n) for sorting, O(n) for selection
**Space Complexity**: O(n) for result

### Fractional Knapsack

**Problem**: Given weights, values, and capacity, maximize value by taking fractions of items.

```klang
struct Item {
    weight: float,
    value: float,
    ratio: float  // value per unit weight
}

function fractional_knapsack(items: [Item], capacity: float) -> float {
    // Calculate value/weight ratio
    for item in items {
        item.ratio = item.value / item.weight;
    }
    
    // Sort by ratio (descending)
    sort_by_ratio_desc(items);
    
    let total_value: float = 0.0;
    let remaining: float = capacity;
    
    for item in items {
        if (remaining >= item.weight) {
            // Take whole item
            total_value = total_value + item.value;
            remaining = remaining - item.weight;
        } else {
            // Take fraction
            total_value = total_value + (item.ratio * remaining);
            break;
        }
    }
    
    return total_value;
}

// Example
function test_fractional_knapsack() {
    let items: [Item] = [
        Item { weight: 10.0, value: 60.0, ratio: 0.0 },  // 6.0/unit
        Item { weight: 20.0, value: 100.0, ratio: 0.0 }, // 5.0/unit
        Item { weight: 30.0, value: 120.0, ratio: 0.0 }  // 4.0/unit
    ];
    
    let capacity: float = 50.0;
    let max_value: float = fractional_knapsack(items, capacity);
    // Take all of item 0 (60), all of item 1 (100), 20/30 of item 2 (80)
    // Total: 240
    print("Maximum value: " + max_value);
}
```

**Note**: For 0/1 Knapsack (can't take fractions), greedy doesn't work—use DP instead!

### Huffman Coding

**Problem**: Build optimal prefix-free binary codes for data compression.

```klang
struct HuffmanNode {
    char: string,
    freq: int,
    left: HuffmanNode?,
    right: HuffmanNode?
}

class MinHeap {
    data: [HuffmanNode],
    size: int,
    
    function insert(node: HuffmanNode) {
        this.data.push(node);
        this.size = this.size + 1;
        this.heapify_up(this.size - 1);
    }
    
    function extract_min() -> HuffmanNode {
        let min: HuffmanNode = this.data[0];
        this.data[0] = this.data[this.size - 1];
        this.size = this.size - 1;
        this.heapify_down(0);
        return min;
    }
    
    function heapify_up(index: int) {
        while (index > 0) {
            let parent: int = (index - 1) / 2;
            if (this.data[index].freq < this.data[parent].freq) {
                swap(this.data, index, parent);
                index = parent;
            } else {
                break;
            }
        }
    }
    
    function heapify_down(index: int) {
        while (true) {
            let smallest: int = index;
            let left: int = 2 * index + 1;
            let right: int = 2 * index + 2;
            
            if (left < this.size && this.data[left].freq < this.data[smallest].freq) {
                smallest = left;
            }
            if (right < this.size && this.data[right].freq < this.data[smallest].freq) {
                smallest = right;
            }
            
            if (smallest != index) {
                swap(this.data, index, smallest);
                index = smallest;
            } else {
                break;
            }
        }
    }
}

function build_huffman_tree(chars: [string], freqs: [int]) -> HuffmanNode {
    let heap: MinHeap = MinHeap { data: [], size: 0 };
    
    // Create leaf nodes
    for i in 0..chars.length {
        let node: HuffmanNode = HuffmanNode {
            char: chars[i],
            freq: freqs[i],
            left: null,
            right: null
        };
        heap.insert(node);
    }
    
    // Build tree bottom-up
    while (heap.size > 1) {
        let left: HuffmanNode = heap.extract_min();
        let right: HuffmanNode = heap.extract_min();
        
        let merged: HuffmanNode = HuffmanNode {
            char: "",  // Internal node
            freq: left.freq + right.freq,
            left: left,
            right: right
        };
        
        heap.insert(merged);
    }
    
    return heap.extract_min();
}

function generate_codes(root: HuffmanNode, code: string, codes: Map<string, string>) {
    if (root == null) {
        return;
    }
    
    // Leaf node
    if (root.left == null && root.right == null) {
        codes.set(root.char, code);
        return;
    }
    
    generate_codes(root.left, code + "0", codes);
    generate_codes(root.right, code + "1", codes);
}

// Example
function test_huffman_coding() {
    let chars: [string] = ["a", "b", "c", "d", "e", "f"];
    let freqs: [int] = [5, 9, 12, 13, 16, 45];
    
    let root: HuffmanNode = build_huffman_tree(chars, freqs);
    let codes: Map<string, string> = Map();
    
    generate_codes(root, "", codes);
    
    print("Huffman Codes:");
    for char in chars {
        print(char + ": " + codes.get(char));
    }
    // f: 0, c: 100, d: 101, a: 1100, b: 1101, e: 111
}
```

**Time Complexity**: O(n log n) where n is number of unique characters
**Space Complexity**: O(n) for tree and heap

### Job Sequencing with Deadlines

**Problem**: Schedule jobs to maximize profit, each job has a deadline and profit.

```klang
struct Job {
    id: int,
    deadline: int,
    profit: int
}

function job_sequencing(jobs: [Job], max_deadline: int) -> [int] {
    // Sort by profit (descending)
    sort_by_profit_desc(jobs);
    
    let slots: [bool] = create_array(max_deadline + 1, false);
    let scheduled: [int] = [];
    let total_profit: int = 0;
    
    for job in jobs {
        // Find latest available slot before deadline
        for slot in job.deadline..0 {
            if (!slots[slot]) {
                slots[slot] = true;
                scheduled.push(job.id);
                total_profit = total_profit + job.profit;
                break;
            }
        }
    }
    
    return scheduled;
}

// Example
function test_job_sequencing() {
    let jobs: [Job] = [
        Job { id: 1, deadline: 2, profit: 100 },
        Job { id: 2, deadline: 1, profit: 19 },
        Job { id: 3, deadline: 2, profit: 27 },
        Job { id: 4, deadline: 1, profit: 25 },
        Job { id: 5, deadline: 3, profit: 15 }
    ];
    
    let result: [int] = job_sequencing(jobs, 3);
    // Schedule jobs 1, 3, 5 for max profit = 142
    print("Scheduled jobs: ");
    print(result);
}
```

### Minimum Spanning Tree (Kruskal's Algorithm)

```klang
struct Edge {
    src: int,
    dest: int,
    weight: int
}

class UnionFind {
    parent: [int],
    rank: [int],
    
    function new(n: int) -> UnionFind {
        let parent: [int] = [];
        let rank: [int] = [];
        
        for i in 0..n {
            parent.push(i);
            rank.push(0);
        }
        
        return UnionFind { parent: parent, rank: rank };
    }
    
    function find(x: int) -> int {
        if (this.parent[x] != x) {
            this.parent[x] = this.find(this.parent[x]);  // Path compression
        }
        return this.parent[x];
    }
    
    function union(x: int, y: int) -> bool {
        let px: int = this.find(x);
        let py: int = this.find(y);
        
        if (px == py) {
            return false;  // Already in same set
        }
        
        // Union by rank
        if (this.rank[px] < this.rank[py]) {
            this.parent[px] = py;
        } else if (this.rank[px] > this.rank[py]) {
            this.parent[py] = px;
        } else {
            this.parent[py] = px;
            this.rank[px] = this.rank[px] + 1;
        }
        
        return true;
    }
}

function kruskal_mst(n: int, edges: [Edge]) -> [Edge] {
    // Sort edges by weight
    sort_by_weight(edges);
    
    let uf: UnionFind = UnionFind.new(n);
    let mst: [Edge] = [];
    
    for edge in edges {
        if (uf.union(edge.src, edge.dest)) {
            mst.push(edge);
            if (mst.length == n - 1) {
                break;
            }
        }
    }
    
    return mst;
}
```

---

## Greedy Patterns

### Pattern 1: Sorting-Based Greedy

**Strategy**: Sort input by some criteria, then make greedy choices in order.

```klang
// Pattern: Sort then iterate
function sorting_greedy(items: [Item]) -> Result {
    sort(items, compare_function);
    
    let result: Result = initialize();
    
    for item in items {
        if (can_add(item, result)) {
            add_to_result(item, result);
        }
    }
    
    return result;
}
```

### Pattern 2: Priority Queue Greedy

**Strategy**: Use a heap to always access the best choice.

```klang
// Pattern: Min/Max heap for best choice
function priority_queue_greedy(items: [Item]) -> Result {
    let heap: MinHeap = MinHeap.new();
    
    for item in items {
        heap.insert(item);
    }
    
    let result: Result = initialize();
    
    while (!heap.is_empty()) {
        let best: Item = heap.extract_min();
        process(best, result);
    }
    
    return result;
}
```

### Pattern 3: Interval Scheduling

**Strategy**: Sort intervals by start or end time, greedily select non-overlapping ones.

```klang
// Pattern: Interval greedy
function interval_greedy(intervals: [Interval]) -> [Interval] {
    sort_by_end_time(intervals);
    
    let selected: [Interval] = [];
    let last_end: int = -infinity;
    
    for interval in intervals {
        if (interval.start >= last_end) {
            selected.push(interval);
            last_end = interval.end;
        }
    }
    
    return selected;
}
```

---

## Interval Problems

### Merge Intervals

```klang
struct Interval {
    start: int,
    end: int
}

function merge_intervals(intervals: [Interval]) -> [Interval] {
    if (intervals.length == 0) {
        return [];
    }
    
    sort_by_start(intervals);
    
    let merged: [Interval] = [intervals[0]];
    
    for i in 1..intervals.length {
        let current: Interval = intervals[i];
        let last: Interval = merged[merged.length - 1];
        
        if (current.start <= last.end) {
            // Overlapping - merge
            last.end = max(last.end, current.end);
        } else {
            // Non-overlapping - add new
            merged.push(current);
        }
    }
    
    return merged;
}
```

### Non-Overlapping Intervals

```klang
function erase_overlap_intervals(intervals: [Interval]) -> int {
    if (intervals.length == 0) {
        return 0;
    }
    
    sort_by_end(intervals);
    
    let count: int = 0;
    let prev_end: int = intervals[0].end;
    
    for i in 1..intervals.length {
        if (intervals[i].start < prev_end) {
            // Overlapping - remove this interval
            count = count + 1;
        } else {
            prev_end = intervals[i].end;
        }
    }
    
    return count;
}
```

### Meeting Rooms

```klang
function min_meeting_rooms(intervals: [Interval]) -> int {
    let starts: [int] = [];
    let ends: [int] = [];
    
    for interval in intervals {
        starts.push(interval.start);
        ends.push(interval.end);
    }
    
    sort(starts);
    sort(ends);
    
    let rooms: int = 0;
    let max_rooms: int = 0;
    let i: int = 0;
    let j: int = 0;
    
    while (i < starts.length) {
        if (starts[i] < ends[j]) {
            rooms = rooms + 1;
            max_rooms = max(max_rooms, rooms);
            i = i + 1;
        } else {
            rooms = rooms - 1;
            j = j + 1;
        }
    }
    
    return max_rooms;
}
```

---

## Greedy on Arrays

### Jump Game

```klang
function can_jump(nums: [int]) -> bool {
    let max_reach: int = 0;
    
    for i in 0..nums.length {
        if (i > max_reach) {
            return false;  // Can't reach this position
        }
        max_reach = max(max_reach, i + nums[i]);
        if (max_reach >= nums.length - 1) {
            return true;
        }
    }
    
    return max_reach >= nums.length - 1;
}
```

### Jump Game II

```klang
function min_jumps(nums: [int]) -> int {
    let jumps: int = 0;
    let current_end: int = 0;
    let farthest: int = 0;
    
    for i in 0..(nums.length - 1) {
        farthest = max(farthest, i + nums[i]);
        
        if (i == current_end) {
            jumps = jumps + 1;
            current_end = farthest;
            
            if (current_end >= nums.length - 1) {
                break;
            }
        }
    }
    
    return jumps;
}
```

### Gas Station

```klang
function can_complete_circuit(gas: [int], cost: [int]) -> int {
    let total_gas: int = 0;
    let total_cost: int = 0;
    let tank: int = 0;
    let start: int = 0;
    
    for i in 0..gas.length {
        total_gas = total_gas + gas[i];
        total_cost = total_cost + cost[i];
        tank = tank + gas[i] - cost[i];
        
        if (tank < 0) {
            start = i + 1;
            tank = 0;
        }
    }
    
    if (total_gas < total_cost) {
        return -1;
    }
    
    return start;
}
```

### Candy Distribution

```klang
function min_candy(ratings: [int]) -> int {
    let n: int = ratings.length;
    let candies: [int] = create_array(n, 1);
    
    // Left to right: ensure right neighbor with higher rating gets more
    for i in 1..n {
        if (ratings[i] > ratings[i - 1]) {
            candies[i] = candies[i - 1] + 1;
        }
    }
    
    // Right to left: ensure left neighbor with higher rating gets more
    for i in (n - 2)..(-1) {
        if (ratings[i] > ratings[i + 1]) {
            candies[i] = max(candies[i], candies[i + 1] + 1);
        }
    }
    
    let total: int = 0;
    for candy in candies {
        total = total + candy;
    }
    
    return total;
}
```

---

## Greedy with Strings

### Reorganize String

```klang
function reorganize_string(s: string) -> string {
    // Count frequencies
    let freq: Map<char, int> = Map();
    for c in s {
        freq.set(c, freq.get(c, 0) + 1);
    }
    
    // Max heap by frequency
    let heap: MaxHeap = MaxHeap.new();
    for (char, count) in freq {
        heap.insert((count, char));
    }
    
    let result: string = "";
    let prev: (int, char)? = null;
    
    while (!heap.is_empty() || prev != null) {
        if (prev != null && heap.is_empty()) {
            return "";  // Can't reorganize
        }
        
        let (count, char): (int, char) = heap.extract_max();
        result = result + char;
        
        if (prev != null) {
            heap.insert(prev);
        }
        
        count = count - 1;
        prev = if (count > 0) { (count, char) } else { null };
    }
    
    return result;
}
```

### Remove K Digits

```klang
function remove_k_digits(num: string, k: int) -> string {
    let stack: [char] = [];
    let removed: int = 0;
    
    for digit in num {
        while (removed < k && stack.length > 0 && stack[stack.length - 1] > digit) {
            stack.pop();
            removed = removed + 1;
        }
        stack.push(digit);
    }
    
    // Remove remaining digits from end
    while (removed < k) {
        stack.pop();
        removed = removed + 1;
    }
    
    // Build result, skip leading zeros
    let result: string = "";
    let leading: bool = true;
    
    for digit in stack {
        if (leading && digit == '0') {
            continue;
        }
        leading = false;
        result = result + digit;
    }
    
    return if (result == "") { "0" } else { result };
}
```

---

## Interview Problems

### Problem 1: Task Scheduler

**Problem**: Schedule tasks with cooldown period between same tasks.

```klang
function least_interval(tasks: [char], n: int) -> int {
    let freq: [int] = create_array(26, 0);
    
    for task in tasks {
        freq[task - 'A'] = freq[task - 'A'] + 1;
    }
    
    sort_desc(freq);
    
    let max_freq: int = freq[0];
    let idle_time: int = (max_freq - 1) * n;
    
    for i in 1..26 {
        if (freq[i] == 0) {
            break;
        }
        idle_time = idle_time - min(max_freq - 1, freq[i]);
    }
    
    idle_time = max(0, idle_time);
    
    return tasks.length + idle_time;
}
```

### Problem 2: Partition Labels

```klang
function partition_labels(s: string) -> [int] {
    // Find last occurrence of each character
    let last: [int] = create_array(26, 0);
    
    for i in 0..s.length {
        last[s[i] - 'a'] = i;
    }
    
    let result: [int] = [];
    let start: int = 0;
    let end: int = 0;
    
    for i in 0..s.length {
        end = max(end, last[s[i] - 'a']);
        
        if (i == end) {
            result.push(end - start + 1);
            start = i + 1;
        }
    }
    
    return result;
}
```

### Problem 3: Queue Reconstruction by Height

```klang
struct Person {
    height: int,
    k: int  // Number of people in front with height >= this person's height
}

function reconstruct_queue(people: [Person]) -> [Person] {
    // Sort by height descending, then by k ascending
    sort(people, function(a: Person, b: Person) -> int {
        if (a.height != b.height) {
            return b.height - a.height;
        }
        return a.k - b.k;
    });
    
    let result: [Person] = [];
    
    for person in people {
        result.insert(person.k, person);
    }
    
    return result;
}
```

### Problem 4: Minimum Number of Arrows

```klang
function find_min_arrows(points: [[int]]) -> int {
    if (points.length == 0) {
        return 0;
    }
    
    // Sort by end position
    sort(points, function(a: [int], b: [int]) -> int {
        return a[1] - b[1];
    });
    
    let arrows: int = 1;
    let arrow_pos: int = points[0][1];
    
    for i in 1..points.length {
        if (points[i][0] > arrow_pos) {
            // Need new arrow
            arrows = arrows + 1;
            arrow_pos = points[i][1];
        }
    }
    
    return arrows;
}
```

### Problem 5: Assign Cookies

```klang
function find_content_children(g: [int], s: [int]) -> int {
    sort(g);  // Children's greed
    sort(s);  // Cookie sizes
    
    let i: int = 0;
    let j: int = 0;
    let count: int = 0;
    
    while (i < g.length && j < s.length) {
        if (s[j] >= g[i]) {
            count = count + 1;
            i = i + 1;
        }
        j = j + 1;
    }
    
    return count;
}
```

### Problem 6: Maximum Subarray (Kadane's - Greedy Variant)

```klang
function max_subarray(nums: [int]) -> int {
    let max_sum: int = nums[0];
    let current_sum: int = nums[0];
    
    for i in 1..nums.length {
        current_sum = max(nums[i], current_sum + nums[i]);
        max_sum = max(max_sum, current_sum);
    }
    
    return max_sum;
}
```

### Problem 7: Best Time to Buy and Sell Stock II

```klang
function max_profit(prices: [int]) -> int {
    let profit: int = 0;
    
    for i in 1..prices.length {
        if (prices[i] > prices[i - 1]) {
            profit = profit + (prices[i] - prices[i - 1]);
        }
    }
    
    return profit;
}
```

### Problem 8: Minimum Platforms Required

```klang
function min_platforms(arrivals: [int], departures: [int]) -> int {
    sort(arrivals);
    sort(departures);
    
    let platforms: int = 0;
    let max_platforms: int = 0;
    let i: int = 0;
    let j: int = 0;
    
    while (i < arrivals.length) {
        if (arrivals[i] <= departures[j]) {
            platforms = platforms + 1;
            max_platforms = max(max_platforms, platforms);
            i = i + 1;
        } else {
            platforms = platforms - 1;
            j = j + 1;
        }
    }
    
    return max_platforms;
}
```

### Problem 9: Wiggle Subsequence

```klang
function wiggle_max_length(nums: [int]) -> int {
    if (nums.length < 2) {
        return nums.length;
    }
    
    let prev_diff: int = nums[1] - nums[0];
    let count: int = if (prev_diff != 0) { 2 } else { 1 };
    
    for i in 2..nums.length {
        let diff: int = nums[i] - nums[i - 1];
        
        if ((diff > 0 && prev_diff <= 0) || (diff < 0 && prev_diff >= 0)) {
            count = count + 1;
            prev_diff = diff;
        }
    }
    
    return count;
}
```

### Problem 10: Boats to Save People

```klang
function num_rescue_boats(people: [int], limit: int) -> int {
    sort(people);
    
    let left: int = 0;
    let right: int = people.length - 1;
    let boats: int = 0;
    
    while (left <= right) {
        if (people[left] + people[right] <= limit) {
            left = left + 1;
        }
        right = right - 1;
        boats = boats + 1;
    }
    
    return boats;
}
```

### Problem 11: Two City Scheduling

```klang
function two_city_scheduling(costs: [[int]]) -> int {
    // Sort by difference (cost_a - cost_b)
    sort(costs, function(a: [int], b: [int]) -> int {
        return (a[0] - a[1]) - (b[0] - b[1]);
    });
    
    let total: int = 0;
    let n: int = costs.length / 2;
    
    // Send first half to city A, second half to city B
    for i in 0..n {
        total = total + costs[i][0];
    }
    
    for i in n..(2 * n) {
        total = total + costs[i][1];
    }
    
    return total;
}
```

### Problem 12: Minimum Add to Make Parentheses Valid

```klang
function min_add_to_make_valid(s: string) -> int {
    let open: int = 0;
    let close: int = 0;
    
    for c in s {
        if (c == '(') {
            open = open + 1;
        } else {
            if (open > 0) {
                open = open - 1;
            } else {
                close = close + 1;
            }
        }
    }
    
    return open + close;
}
```

---

## When Greedy Works

### Recognition Patterns

1. **Problem asks for maximum/minimum**
2. **Making local optimal choice doesn't prevent making global optimal choice**
3. **Problem has optimal substructure**
4. **Can prove via exchange argument**

### Key Indicators

- "Maximum number of non-overlapping..."
- "Minimum cost/time to..."
- "Schedule tasks to maximize..."
- "Select items with constraints..."

### Verification Checklist

```klang
// Before using greedy, verify:
/*
1. Greedy Choice Property?
   - Does local optimum lead to global optimum?
   
2. Optimal Substructure?
   - After greedy choice, is remaining problem smaller instance?
   
3. No Dependency?
   - Does current choice affect future choices adversely?
   
4. Proof Available?
   - Can you prove correctness via exchange argument?
*/
```

---

## When Greedy Fails

### Classic Counterexamples

#### 1. Coin Change (Arbitrary Denominations)
```klang
// Greedy fails for coins = [25, 10, 1], amount = 30
// Greedy: 25 + 1 + 1 + 1 + 1 + 1 = 6 coins
// Optimal: 10 + 10 + 10 = 3 coins
```

#### 2. 0/1 Knapsack
```klang
// Greedy by value/weight fails
// Items: [(w=10, v=60), (w=20, v=100), (w=30, v=120)]
// Capacity: 50
// Greedy by ratio: Take (10,60) and (20,100) = 160
// Optimal: Take (20,100) and (30,120) = 220
```

#### 3. Longest Path in DAG
```klang
// Greedy (always pick highest weight edge) fails
// Need DP or DFS for longest path
```

#### 4. Minimum Coin Change (Some Denominations)
```klang
// For coins [1, 3, 4] and amount 6:
// Greedy: 4 + 1 + 1 = 3 coins
// Optimal: 3 + 3 = 2 coins
```

### Why Greedy Fails

1. **Future Constraints**: Current choice limits future options
2. **Global Dependencies**: Local optimum conflicts with global optimum
3. **No Exchange Property**: Can't swap greedy choice with optimal choice
4. **Overlapping Subproblems**: Same subproblem occurs multiple times (use DP)

---

## Best Practices

### 1. Always Verify Greedy Works
```klang
// Don't assume greedy works - prove it!
function solve_problem_greedy(input: [int]) -> int {
    // 1. State the greedy choice
    // 2. Prove optimal substructure
    // 3. Prove greedy choice property
    // 4. Implement
    
    sort(input);  // Often needed
    // ... greedy logic
}
```

### 2. Sort First (Usually)
```klang
// Most greedy algorithms need sorted input
function greedy_with_sorting(items: [Item]) -> Result {
    sort(items, custom_comparator);  // Critical step
    // ... then make greedy choices
}
```

### 3. Use Appropriate Data Structures
```klang
// Priority queue for "always pick best"
let heap: MinHeap = MinHeap.new();

// Stack for "remember previous"
let stack: [int] = [];

// Union-Find for "group/merge"
let uf: UnionFind = UnionFind.new(n);
```

### 4. Handle Edge Cases
```klang
function greedy_solution(arr: [int]) -> int {
    if (arr.length == 0) {
        return 0;  // Empty case
    }
    
    if (arr.length == 1) {
        return arr[0];  // Single element
    }
    
    // ... main logic
}
```

### 5. Be Careful with Ties
```klang
// When elements are equal, define tiebreaker
sort(items, function(a: Item, b: Item) -> int {
    if (a.value != b.value) {
        return b.value - a.value;
    }
    return a.weight - b.weight;  // Tiebreaker
});
```

---

## Proof Techniques

### Technique 1: Exchange Argument

**Steps**:
1. Assume optimal solution O differs from greedy solution G
2. Find first position where they differ
3. Show swapping O's choice with G's choice doesn't worsen solution
4. Repeat until O = G, proving G is optimal

**Example**: Activity Selection
```klang
/*
Proof that selecting earliest-finishing activity is optimal:

Let A = greedy solution (earliest finish times)
Let O = any optimal solution

If A ≠ O:
- Let a₁ be first activity in A
- Let o₁ be first activity in O
- Since a₁ finishes earliest, we can replace o₁ with a₁
- This doesn't conflict with later activities in O
- So O' with a₁ is also optimal
- By induction, A is optimal ∎
*/
```

### Technique 2: Greedy Stays Ahead

**Steps**:
1. Show greedy solution is "ahead" at each step
2. Being ahead means closer to or better than optimal
3. At the end, greedy must be at least as good as optimal

**Example**: Interval Scheduling
```klang
/*
Proof that greedy maximizes number of intervals:

Claim: After k steps, greedy finishes no later than optimal

Base: After 1 step, greedy picks earliest-finishing interval

Inductive: If greedy finishes at time t after k intervals,
           and optimal finishes at time t' after k intervals,
           then t ≤ t'
           
           Greedy picks earliest finish for (k+1)-th interval
           among those starting after t
           
           Optimal must pick from intervals starting after t'
           Since t ≤ t', greedy has more choices
           
           So greedy finishes no later for (k+1)-th interval
           
Therefore, greedy selects at least as many intervals ∎
*/
```

### Technique 3: Contradiction

**Steps**:
1. Assume greedy is not optimal
2. Show this leads to contradiction
3. Conclude greedy must be optimal

**Example**: Huffman Coding
```klang
/*
Proof that Huffman gives optimal prefix-free code:

Assume H (Huffman tree) is not optimal
Let O be an optimal tree with lower cost

In O, let x and y be two leaves at maximum depth
Let their parent be p

Cost(O) = Cost(O without x,y) + freq(x) + freq(y) + freq(p)

In H, Huffman merges two minimum frequency symbols
If these aren't x and y, swapping them gives lower cost
This contradicts O being optimal

Therefore H must be optimal ∎
*/
```

### Technique 4: Structural Induction

**Steps**:
1. Base case: Show greedy works for smallest instance
2. Inductive step: Show if greedy works for smaller problem, it works after one more greedy choice
3. Conclude greedy works for all instances

---

## Summary

### When to Use Greedy
- ✓ Problem has greedy choice property
- ✓ Optimal substructure exists
- ✓ Local optimum leads to global optimum
- ✓ Can prove correctness

### When to Avoid Greedy
- ✗ Future constraints affect current choices
- ✗ Overlapping subproblems (use DP)
- ✗ Need to explore all possibilities
- ✗ Can't prove greedy choice property

### Key Takeaways

1. **Always prove correctness** - Greedy is tempting but often wrong
2. **Sort first** - Most greedy algorithms need sorted input
3. **Use right data structure** - Heaps, stacks, union-find as needed
4. **Consider alternatives** - If greedy fails, try DP or backtracking
5. **Master proof techniques** - Exchange argument, greedy stays ahead

### Complexity Comparison

| Algorithm | Time | Space | Notes |
|-----------|------|-------|-------|
| Activity Selection | O(n log n) | O(1) | Sorting dominates |
| Huffman Coding | O(n log n) | O(n) | Heap operations |
| Kruskal's MST | O(E log E) | O(V) | Sort edges + Union-Find |
| Fractional Knapsack | O(n log n) | O(1) | Sort by ratio |
| Job Sequencing | O(n²) | O(n) | Can optimize to O(n log n) |

---

## Practice Problems

### Easy
1. Assign Cookies
2. Lemonade Change
3. Maximum Subarray (Kadane's)
4. Best Time to Buy and Sell Stock II

### Medium
5. Jump Game
6. Jump Game II
7. Gas Station
8. Candy
9. Task Scheduler
10. Partition Labels
11. Non-overlapping Intervals
12. Minimum Number of Arrows
13. Queue Reconstruction by Height
14. Reorganize String

### Hard
15. Remove K Digits
16. Create Maximum Number
17. Split Array into Consecutive Subsequences
18. Minimum Number of Taps to Open

---

**Next**: [Backtracking](06_backtracking.md) | **Previous**: [Dynamic Programming](04_dynamic_programming.md)
