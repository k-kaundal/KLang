# Dynamic Programming in KLang - Data Structures & Algorithms

## Table of Contents
1. [Introduction](#introduction)
2. [DP Fundamentals](#dp-fundamentals)
3. [Simple DP Problems](#simple-dp-problems)
4. [Classic DP Problems](#classic-dp-problems)
5. [String DP](#string-dp)
6. [Grid DP](#grid-dp)
7. [DP Patterns](#dp-patterns)
8. [State Definition](#state-definition)
9. [Transition Formula](#transition-formula)
10. [Space Optimization](#space-optimization)
11. [Interview Problems](#interview-problems)
12. [Best Practices](#best-practices)
13. [Common Patterns](#common-patterns)

---

## Introduction

Dynamic Programming (DP) is an algorithmic paradigm that solves complex problems by breaking them down into simpler subproblems and storing their solutions to avoid redundant computation. It's one of the most powerful optimization techniques in computer science.

### What is Dynamic Programming?

**Dynamic Programming** is a method for solving problems by:
1. Breaking down the problem into overlapping subproblems
2. Solving each subproblem only once
3. Storing the solution for future use (memoization or tabulation)
4. Building the final solution from subproblem solutions

### When to Use Dynamic Programming

DP is applicable when a problem has these two key properties:

#### 1. Overlapping Subproblems

The problem can be broken down into subproblems that are reused multiple times.

```
Example: Fibonacci F(5)
       F(5)
      /    \
   F(4)    F(3)
   /  \    /  \
 F(3) F(2) F(2) F(1)
 /  \
F(2) F(1)

Notice: F(3) computed twice, F(2) computed three times!
```

#### 2. Optimal Substructure

The optimal solution to the problem can be constructed from optimal solutions of its subproblems.

```
Example: Shortest path from A to C through B
Optimal path A→C = Optimal path A→B + Optimal path B→C
```

### When to Use DP vs Other Approaches

- **Use DP**: Overlapping subproblems, optimal substructure, counting/optimization problems
- **Use Greedy**: Locally optimal choices lead to global optimum, no need to reconsider decisions
- **Use Divide & Conquer**: Independent subproblems (no overlap), like merge sort
- **Use Backtracking**: Finding all solutions, constraint satisfaction problems

### DP vs Recursion with Memoization

```klang
// Pure recursion - exponential time O(2^n)
fn fib_recursive(n: int) -> int {
    if n <= 1 { return n; }
    return fib_recursive(n - 1) + fib_recursive(n - 2);
}

// DP with memoization - linear time O(n)
fn fib_memo(n: int, memo: Map<int, int>) -> int {
    if n <= 1 { return n; }
    if memo.has(n) { return memo.get(n); }
    
    let result = fib_memo(n - 1, memo) + fib_memo(n - 2, memo);
    memo.set(n, result);
    return result;
}
```

---

## DP Fundamentals

There are two main approaches to implement Dynamic Programming:

### 1. Memoization (Top-Down Approach)

Start with the original problem and recursively break it down, storing solutions in a cache.

**Characteristics**:
- Recursive implementation
- Computes only necessary subproblems
- Uses recursion stack + memoization cache
- Natural and intuitive for many problems

```klang
// Memoization template
fn solve_memo(n: int, memo: Map<int, int>) -> int {
    // Base case
    if n is base case {
        return base_value;
    }
    
    // Check if already computed
    if memo.has(n) {
        return memo.get(n);
    }
    
    // Compute and store result
    let result = /* recursive computation */;
    memo.set(n, result);
    return result;
}
```

### 2. Tabulation (Bottom-Up Approach)

Start from the smallest subproblems and build up to the original problem iteratively.

**Characteristics**:
- Iterative implementation
- Computes all subproblems
- Uses a table (array/matrix)
- Often more space-efficient
- No recursion overhead

```klang
// Tabulation template
fn solve_tabulation(n: int) -> int {
    // Create DP table
    let dp = Array<int>(n + 1, 0);
    
    // Initialize base cases
    dp[0] = base_value_0;
    dp[1] = base_value_1;
    
    // Fill table bottom-up
    for i in 2..=n {
        dp[i] = /* compute from previous values */;
    }
    
    return dp[n];
}
```

### Comparison: Memoization vs Tabulation

```klang
// Problem: Calculate nth Fibonacci number

// 1. MEMOIZATION (Top-Down)
fn fib_memoization(n: int, memo: Map<int, int>) -> int {
    if n <= 1 { return n; }
    
    if memo.has(n) {
        return memo.get(n);
    }
    
    let result = fib_memoization(n - 1, memo) + fib_memoization(n - 2, memo);
    memo.set(n, result);
    return result;
}

fn fib_memo(n: int) -> int {
    let memo = Map<int, int>();
    return fib_memoization(n, memo);
}

// 2. TABULATION (Bottom-Up)
fn fib_tabulation(n: int) -> int {
    if n <= 1 { return n; }
    
    let dp = Array<int>(n + 1, 0);
    dp[0] = 0;
    dp[1] = 1;
    
    for i in 2..=n {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    
    return dp[n];
}

// Comparison:
// Memoization: O(n) time, O(n) space (stack + cache), computes only needed
// Tabulation: O(n) time, O(n) space (table), computes all subproblems
```

---

## Simple DP Problems

### 1. Fibonacci Numbers

Calculate the nth Fibonacci number where F(n) = F(n-1) + F(n-2).

#### Approach 1: Memoization

```klang
fn fibonacci_memo_helper(n: int, memo: Map<int, int>) -> int {
    // Base cases
    if n <= 1 {
        return n;
    }
    
    // Check cache
    if memo.has(n) {
        return memo.get(n);
    }
    
    // Compute and cache
    let result = fibonacci_memo_helper(n - 1, memo) + 
                 fibonacci_memo_helper(n - 2, memo);
    memo.set(n, result);
    return result;
}

fn fibonacci_memo(n: int) -> int {
    let memo = Map<int, int>();
    return fibonacci_memo_helper(n, memo);
}
```

#### Approach 2: Tabulation

```klang
fn fibonacci_tab(n: int) -> int {
    if n <= 1 { return n; }
    
    let dp = Array<int>(n + 1, 0);
    dp[0] = 0;
    dp[1] = 1;
    
    for i in 2..=n {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    
    return dp[n];
}
```

#### Approach 3: Space Optimized

```klang
fn fibonacci_optimized(n: int) -> int {
    if n <= 1 { return n; }
    
    let prev2 = 0;  // F(0)
    let prev1 = 1;  // F(1)
    
    for i in 2..=n {
        let curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n) for memoization/tabulation, O(1) for optimized

### 2. Climbing Stairs

You're climbing a staircase with n steps. You can climb 1 or 2 steps at a time. How many distinct ways can you climb to the top?

```klang
// This is actually Fibonacci in disguise!
// To reach step n, you can come from step n-1 (1 step) or n-2 (2 steps)
// ways(n) = ways(n-1) + ways(n-2)

fn climb_stairs_memo(n: int, memo: Map<int, int>) -> int {
    // Base cases
    if n <= 2 { return n; }
    
    if memo.has(n) {
        return memo.get(n);
    }
    
    let result = climb_stairs_memo(n - 1, memo) + climb_stairs_memo(n - 2, memo);
    memo.set(n, result);
    return result;
}

fn climb_stairs_tab(n: int) -> int {
    if n <= 2 { return n; }
    
    let dp = Array<int>(n + 1, 0);
    dp[1] = 1;
    dp[2] = 2;
    
    for i in 3..=n {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    
    return dp[n];
}

fn climb_stairs_optimized(n: int) -> int {
    if n <= 2 { return n; }
    
    let prev2 = 1;
    let prev1 = 2;
    
    for i in 3..=n {
        let curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}

// Example usage
fn test_climb_stairs() {
    let memo = Map<int, int>();
    println("Stairs 5 (memo): " + str(climb_stairs_memo(5, memo)));     // 8
    println("Stairs 5 (tab): " + str(climb_stairs_tab(5)));             // 8
    println("Stairs 5 (optimized): " + str(climb_stairs_optimized(5))); // 8
}
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n) for memoization/tabulation, O(1) for optimized

### 3. Min Cost Climbing Stairs

Given an array where cost[i] is the cost of step i, you can start from step 0 or 1 and climb 1 or 2 steps. Find minimum cost to reach the top (beyond last step).

```klang
fn min_cost_climbing_stairs(cost: Array<int>) -> int {
    let n = cost.len();
    if n == 0 { return 0; }
    if n == 1 { return cost[0]; }
    
    // dp[i] = minimum cost to reach step i
    let dp = Array<int>(n + 1, 0);
    
    // Base cases: can start from step 0 or 1 for free
    dp[0] = 0;
    dp[1] = 0;
    
    // Fill the DP table
    for i in 2..=n {
        // To reach step i, we can come from i-1 or i-2
        // Take minimum and add the cost
        let from_prev1 = dp[i - 1] + cost[i - 1];
        let from_prev2 = dp[i - 2] + cost[i - 2];
        dp[i] = min(from_prev1, from_prev2);
    }
    
    return dp[n];
}

// Space optimized version
fn min_cost_climbing_stairs_optimized(cost: Array<int>) -> int {
    let n = cost.len();
    if n == 0 { return 0; }
    if n == 1 { return cost[0]; }
    
    let prev2 = 0;  // dp[i-2]
    let prev1 = 0;  // dp[i-1]
    
    for i in 2..=n {
        let from_prev1 = prev1 + cost[i - 1];
        let from_prev2 = prev2 + cost[i - 2];
        let curr = min(from_prev1, from_prev2);
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}

// Example usage
fn test_min_cost_climbing() {
    let cost1 = [10, 15, 20];
    println("Min cost: " + str(min_cost_climbing_stairs(cost1))); // 15
    
    let cost2 = [1, 100, 1, 1, 1, 100, 1, 1, 100, 1];
    println("Min cost: " + str(min_cost_climbing_stairs(cost2))); // 6
}
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n) for standard, O(1) for optimized

---

## Classic DP Problems

### 1. 0/1 Knapsack Problem

Given weights and values of n items, and a knapsack capacity W, find the maximum value that can be obtained by selecting items (each item can be picked at most once).

```klang
// Memoization approach
fn knapsack_memo(weights: Array<int>, values: Array<int>, capacity: int, 
                 n: int, memo: Map<string, int>) -> int {
    // Base case: no items or no capacity
    if n == 0 || capacity == 0 {
        return 0;
    }
    
    // Create unique key for this state
    let key = str(n) + "," + str(capacity);
    if memo.has(key) {
        return memo.get(key);
    }
    
    // If current item's weight exceeds capacity, skip it
    if weights[n - 1] > capacity {
        let result = knapsack_memo(weights, values, capacity, n - 1, memo);
        memo.set(key, result);
        return result;
    }
    
    // Two choices: include or exclude current item
    let include = values[n - 1] + knapsack_memo(weights, values, 
                                                  capacity - weights[n - 1], n - 1, memo);
    let exclude = knapsack_memo(weights, values, capacity, n - 1, memo);
    
    let result = max(include, exclude);
    memo.set(key, result);
    return result;
}

// Tabulation approach
fn knapsack_tab(weights: Array<int>, values: Array<int>, capacity: int) -> int {
    let n = weights.len();
    
    // dp[i][w] = max value using first i items with capacity w
    let dp = Array<Array<int>>(n + 1);
    for i in 0..=n {
        dp[i] = Array<int>(capacity + 1, 0);
    }
    
    // Fill the DP table
    for i in 1..=n {
        for w in 0..=capacity {
            // Don't include current item
            dp[i][w] = dp[i - 1][w];
            
            // Include current item if it fits
            if weights[i - 1] <= w {
                let include_value = values[i - 1] + dp[i - 1][w - weights[i - 1]];
                dp[i][w] = max(dp[i][w], include_value);
            }
        }
    }
    
    return dp[n][capacity];
}

// Space optimized version (1D array)
fn knapsack_optimized(weights: Array<int>, values: Array<int>, capacity: int) -> int {
    let n = weights.len();
    let dp = Array<int>(capacity + 1, 0);
    
    for i in 0..<n {
        // Traverse backwards to avoid using updated values
        for w in capacity..=weights[i] by -1 {
            dp[w] = max(dp[w], values[i] + dp[w - weights[i]]);
        }
    }
    
    return dp[capacity];
}

// Example usage
fn test_knapsack() {
    let weights = [1, 3, 4, 5];
    let values = [1, 4, 5, 7];
    let capacity = 7;
    
    let memo = Map<string, int>();
    let result1 = knapsack_memo(weights, values, capacity, weights.len(), memo);
    let result2 = knapsack_tab(weights, values, capacity);
    let result3 = knapsack_optimized(weights, values, capacity);
    
    println("Max value (memo): " + str(result1));      // 9
    println("Max value (tab): " + str(result2));       // 9
    println("Max value (optimized): " + str(result3)); // 9
}
```

**Time Complexity**: O(n × W) where n is number of items, W is capacity  
**Space Complexity**: O(n × W) for tabulation, O(W) for optimized

### 2. Unbounded Knapsack

Similar to 0/1 knapsack, but items can be used unlimited times.

```klang
fn unbounded_knapsack(weights: Array<int>, values: Array<int>, capacity: int) -> int {
    let n = weights.len();
    let dp = Array<int>(capacity + 1, 0);
    
    // For each capacity
    for w in 1..=capacity {
        // Try each item
        for i in 0..<n {
            if weights[i] <= w {
                dp[w] = max(dp[w], values[i] + dp[w - weights[i]]);
            }
        }
    }
    
    return dp[capacity];
}

// Alternative: 2D approach
fn unbounded_knapsack_2d(weights: Array<int>, values: Array<int>, capacity: int) -> int {
    let n = weights.len();
    let dp = Array<Array<int>>(n + 1);
    
    for i in 0..=n {
        dp[i] = Array<int>(capacity + 1, 0);
    }
    
    for i in 1..=n {
        for w in 1..=capacity {
            // Don't take current item
            dp[i][w] = dp[i - 1][w];
            
            // Take current item (can take again, so use dp[i][...])
            if weights[i - 1] <= w {
                dp[i][w] = max(dp[i][w], values[i - 1] + dp[i][w - weights[i - 1]]);
            }
        }
    }
    
    return dp[n][capacity];
}

fn test_unbounded_knapsack() {
    let weights = [1, 3, 4, 5];
    let values = [10, 40, 50, 70];
    let capacity = 8;
    
    println("Max value: " + str(unbounded_knapsack(weights, values, capacity))); // 110
}
```

**Time Complexity**: O(n × W)  
**Space Complexity**: O(W)

### 3. Subset Sum Problem

Determine if there's a subset of given numbers that sums to a target value.

```klang
fn subset_sum(nums: Array<int>, target: int) -> bool {
    let n = nums.len();
    
    // dp[i][s] = can we make sum s using first i elements
    let dp = Array<Array<bool>>(n + 1);
    for i in 0..=n {
        dp[i] = Array<bool>(target + 1, false);
    }
    
    // Base case: sum 0 is always possible (empty subset)
    for i in 0..=n {
        dp[i][0] = true;
    }
    
    // Fill the table
    for i in 1..=n {
        for s in 1..=target {
            // Don't include current number
            dp[i][s] = dp[i - 1][s];
            
            // Include current number if it doesn't exceed sum
            if nums[i - 1] <= s {
                dp[i][s] = dp[i][s] || dp[i - 1][s - nums[i - 1]];
            }
        }
    }
    
    return dp[n][target];
}

// Space optimized version
fn subset_sum_optimized(nums: Array<int>, target: int) -> bool {
    let dp = Array<bool>(target + 1, false);
    dp[0] = true;  // Sum 0 is always possible
    
    for num in nums {
        // Traverse backwards to avoid using updated values
        for s in target..=num by -1 {
            if dp[s - num] {
                dp[s] = true;
            }
        }
    }
    
    return dp[target];
}

// Get the actual subset
fn subset_sum_with_path(nums: Array<int>, target: int) -> Array<int> {
    let n = nums.len();
    let dp = Array<Array<bool>>(n + 1);
    
    for i in 0..=n {
        dp[i] = Array<bool>(target + 1, false);
        dp[i][0] = true;
    }
    
    for i in 1..=n {
        for s in 1..=target {
            dp[i][s] = dp[i - 1][s];
            if nums[i - 1] <= s {
                dp[i][s] = dp[i][s] || dp[i - 1][s - nums[i - 1]];
            }
        }
    }
    
    // Backtrack to find the subset
    let result = Array<int>();
    if !dp[n][target] {
        return result;
    }
    
    let i = n;
    let s = target;
    while i > 0 && s > 0 {
        // If we didn't get this sum from previous row, we used nums[i-1]
        if !dp[i - 1][s] {
            result.push(nums[i - 1]);
            s -= nums[i - 1];
        }
        i -= 1;
    }
    
    return result;
}

fn test_subset_sum() {
    let nums = [3, 34, 4, 12, 5, 2];
    let target = 9;
    
    println("Can make " + str(target) + ": " + str(subset_sum(nums, target))); // true
    let subset = subset_sum_with_path(nums, target);
    println("Subset: " + str(subset)); // [4, 5] or [4, 3, 2] etc.
}
```

**Time Complexity**: O(n × target)  
**Space Complexity**: O(n × target) for standard, O(target) for optimized

### 4. Coin Change Problem

#### Problem 1: Minimum Coins

Find the minimum number of coins needed to make a given amount.

```klang
fn coin_change_min(coins: Array<int>, amount: int) -> int {
    // dp[i] = minimum coins needed to make amount i
    let dp = Array<int>(amount + 1, amount + 1); // Initialize with impossible value
    dp[0] = 0; // 0 coins for amount 0
    
    for i in 1..=amount {
        for coin in coins {
            if coin <= i {
                dp[i] = min(dp[i], 1 + dp[i - coin]);
            }
        }
    }
    
    return if dp[amount] > amount { -1 } else { dp[amount] };
}

// With coin tracking
fn coin_change_min_with_path(coins: Array<int>, amount: int) -> (int, Array<int>) {
    let dp = Array<int>(amount + 1, amount + 1);
    let used = Array<int>(amount + 1, -1); // Track which coin was used
    dp[0] = 0;
    
    for i in 1..=amount {
        for coin in coins {
            if coin <= i && dp[i - coin] + 1 < dp[i] {
                dp[i] = dp[i - coin] + 1;
                used[i] = coin;
            }
        }
    }
    
    if dp[amount] > amount {
        return (-1, Array<int>());
    }
    
    // Reconstruct the coins used
    let result = Array<int>();
    let curr = amount;
    while curr > 0 {
        let coin = used[curr];
        result.push(coin);
        curr -= coin;
    }
    
    return (dp[amount], result);
}
```

#### Problem 2: Number of Ways

Count the number of ways to make a given amount.

```klang
fn coin_change_ways(coins: Array<int>, amount: int) -> int {
    // dp[i] = number of ways to make amount i
    let dp = Array<int>(amount + 1, 0);
    dp[0] = 1; // One way to make 0: use no coins
    
    // For each coin
    for coin in coins {
        // Update all amounts that can use this coin
        for i in coin..=amount {
            dp[i] += dp[i - coin];
        }
    }
    
    return dp[amount];
}

fn test_coin_change() {
    let coins = [1, 2, 5];
    let amount = 11;
    
    println("Min coins for " + str(amount) + ": " + str(coin_change_min(coins, amount))); // 3
    
    let (min_count, used_coins) = coin_change_min_with_path(coins, amount);
    println("Coins used: " + str(used_coins)); // [5, 5, 1] or similar
    
    println("Ways to make " + str(amount) + ": " + str(coin_change_ways(coins, amount))); // 12
}
```

**Time Complexity**: O(amount × coins)  
**Space Complexity**: O(amount)

### 5. Longest Common Subsequence (LCS)

Find the length of the longest subsequence common to two strings.

```klang
fn lcs_length(s1: string, s2: string) -> int {
    let m = s1.len();
    let n = s2.len();
    
    // dp[i][j] = LCS length of s1[0..i-1] and s2[0..j-1]
    let dp = Array<Array<int>>(m + 1);
    for i in 0..=m {
        dp[i] = Array<int>(n + 1, 0);
    }
    
    for i in 1..=m {
        for j in 1..=n {
            if s1[i - 1] == s2[j - 1] {
                // Characters match: extend LCS
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                // Take maximum from either excluding s1[i-1] or s2[j-1]
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    return dp[m][n];
}

// Get the actual LCS string
fn lcs_string(s1: string, s2: string) -> string {
    let m = s1.len();
    let n = s2.len();
    
    let dp = Array<Array<int>>(m + 1);
    for i in 0..=m {
        dp[i] = Array<int>(n + 1, 0);
    }
    
    // Fill DP table
    for i in 1..=m {
        for j in 1..=n {
            if s1[i - 1] == s2[j - 1] {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    // Backtrack to construct LCS
    let result = "";
    let i = m;
    let j = n;
    
    while i > 0 && j > 0 {
        if s1[i - 1] == s2[j - 1] {
            result = s1[i - 1] + result;
            i -= 1;
            j -= 1;
        } else if dp[i - 1][j] > dp[i][j - 1] {
            i -= 1;
        } else {
            j -= 1;
        }
    }
    
    return result;
}

fn test_lcs() {
    let s1 = "ABCDGH";
    let s2 = "AEDFHR";
    
    println("LCS length: " + str(lcs_length(s1, s2)));     // 3
    println("LCS string: " + lcs_string(s1, s2));          // "ADH"
}
```

**Time Complexity**: O(m × n)  
**Space Complexity**: O(m × n)

### 6. Longest Increasing Subsequence (LIS)

Find the length of the longest strictly increasing subsequence in an array.

#### Approach 1: O(n²) Dynamic Programming

```klang
fn lis_dp(nums: Array<int>) -> int {
    let n = nums.len();
    if n == 0 { return 0; }
    
    // dp[i] = length of LIS ending at index i
    let dp = Array<int>(n, 1);
    let max_len = 1;
    
    for i in 1..<n {
        for j in 0..<i {
            if nums[j] < nums[i] {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
        max_len = max(max_len, dp[i]);
    }
    
    return max_len;
}

// Get the actual LIS
fn lis_with_sequence(nums: Array<int>) -> Array<int> {
    let n = nums.len();
    if n == 0 { return Array<int>(); }
    
    let dp = Array<int>(n, 1);
    let parent = Array<int>(n, -1);
    let max_len = 1;
    let max_idx = 0;
    
    for i in 1..<n {
        for j in 0..<i {
            if nums[j] < nums[i] && dp[j] + 1 > dp[i] {
                dp[i] = dp[j] + 1;
                parent[i] = j;
            }
        }
        if dp[i] > max_len {
            max_len = dp[i];
            max_idx = i;
        }
    }
    
    // Reconstruct LIS
    let result = Array<int>();
    let idx = max_idx;
    while idx != -1 {
        result.push(nums[idx]);
        idx = parent[idx];
    }
    result.reverse();
    
    return result;
}
```

#### Approach 2: O(n log n) Binary Search

```klang
fn lis_binary_search(nums: Array<int>) -> int {
    let n = nums.len();
    if n == 0 { return 0; }
    
    // tails[i] = smallest tail element for LIS of length i+1
    let tails = Array<int>();
    
    for num in nums {
        // Binary search for position to replace or append
        let left = 0;
        let right = tails.len();
        
        while left < right {
            let mid = left + (right - left) / 2;
            if tails[mid] < num {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        // Replace or append
        if left == tails.len() {
            tails.push(num);
        } else {
            tails[left] = num;
        }
    }
    
    return tails.len();
}

fn test_lis() {
    let nums = [10, 9, 2, 5, 3, 7, 101, 18];
    
    println("LIS length (DP): " + str(lis_dp(nums)));              // 4
    println("LIS length (Binary): " + str(lis_binary_search(nums))); // 4
    
    let sequence = lis_with_sequence(nums);
    println("LIS sequence: " + str(sequence));                     // [2, 3, 7, 18]
}
```

**Time Complexity**: O(n²) for DP, O(n log n) for binary search  
**Space Complexity**: O(n)

### 7. Edit Distance (Levenshtein Distance)

Find minimum number of operations (insert, delete, replace) to convert one string to another.

```klang
fn edit_distance(s1: string, s2: string) -> int {
    let m = s1.len();
    let n = s2.len();
    
    // dp[i][j] = edit distance between s1[0..i-1] and s2[0..j-1]
    let dp = Array<Array<int>>(m + 1);
    for i in 0..=m {
        dp[i] = Array<int>(n + 1, 0);
    }
    
    // Base cases
    for i in 0..=m {
        dp[i][0] = i; // Delete all characters from s1
    }
    for j in 0..=n {
        dp[0][j] = j; // Insert all characters of s2
    }
    
    // Fill the table
    for i in 1..=m {
        for j in 1..=n {
            if s1[i - 1] == s2[j - 1] {
                // Characters match, no operation needed
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                // Take minimum of three operations
                let replace = dp[i - 1][j - 1] + 1;
                let delete = dp[i - 1][j] + 1;
                let insert = dp[i][j - 1] + 1;
                dp[i][j] = min(replace, min(delete, insert));
            }
        }
    }
    
    return dp[m][n];
}

// With operation tracking
fn edit_distance_with_ops(s1: string, s2: string) -> (int, Array<string>) {
    let m = s1.len();
    let n = s2.len();
    
    let dp = Array<Array<int>>(m + 1);
    for i in 0..=m {
        dp[i] = Array<int>(n + 1, 0);
    }
    
    for i in 0..=m { dp[i][0] = i; }
    for j in 0..=n { dp[0][j] = j; }
    
    for i in 1..=m {
        for j in 1..=n {
            if s1[i - 1] == s2[j - 1] {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + min(dp[i - 1][j - 1], min(dp[i - 1][j], dp[i][j - 1]));
            }
        }
    }
    
    // Backtrack to find operations
    let operations = Array<string>();
    let i = m;
    let j = n;
    
    while i > 0 || j > 0 {
        if i == 0 {
            operations.push("Insert " + s2[j - 1]);
            j -= 1;
        } else if j == 0 {
            operations.push("Delete " + s1[i - 1]);
            i -= 1;
        } else if s1[i - 1] == s2[j - 1] {
            i -= 1;
            j -= 1;
        } else if dp[i][j] == dp[i - 1][j - 1] + 1 {
            operations.push("Replace " + s1[i - 1] + " with " + s2[j - 1]);
            i -= 1;
            j -= 1;
        } else if dp[i][j] == dp[i - 1][j] + 1 {
            operations.push("Delete " + s1[i - 1]);
            i -= 1;
        } else {
            operations.push("Insert " + s2[j - 1]);
            j -= 1;
        }
    }
    
    operations.reverse();
    return (dp[m][n], operations);
}

fn test_edit_distance() {
    let s1 = "horse";
    let s2 = "ros";
    
    println("Edit distance: " + str(edit_distance(s1, s2))); // 3
    
    let (dist, ops) = edit_distance_with_ops(s1, s2);
    println("Operations: " + str(ops));
    // ["Replace h with r", "Delete o", "Delete e"]
}
```

**Time Complexity**: O(m × n)  
**Space Complexity**: O(m × n)

### 8. Matrix Chain Multiplication

Find the minimum number of scalar multiplications needed to multiply a chain of matrices.

```klang
fn matrix_chain_order(dims: Array<int>) -> int {
    let n = dims.len() - 1; // Number of matrices
    
    // dp[i][j] = minimum multiplications for matrices i to j
    let dp = Array<Array<int>>(n);
    for i in 0..<n {
        dp[i] = Array<int>(n, 0);
    }
    
    // len is chain length
    for len in 2..=n {
        for i in 0..=(n - len) {
            let j = i + len - 1;
            dp[i][j] = int::MAX;
            
            // Try all possible split points
            for k in i..<j {
                // Cost of multiplying matrices i..k and k+1..j
                // Plus cost of multiplying the two results
                let cost = dp[i][k] + dp[k + 1][j] + 
                          dims[i] * dims[k + 1] * dims[j + 1];
                dp[i][j] = min(dp[i][j], cost);
            }
        }
    }
    
    return dp[0][n - 1];
}

// With split point tracking
fn matrix_chain_order_with_splits(dims: Array<int>) -> (int, Array<Array<int>>) {
    let n = dims.len() - 1;
    
    let dp = Array<Array<int>>(n);
    let split = Array<Array<int>>(n);
    
    for i in 0..<n {
        dp[i] = Array<int>(n, 0);
        split[i] = Array<int>(n, 0);
    }
    
    for len in 2..=n {
        for i in 0..=(n - len) {
            let j = i + len - 1;
            dp[i][j] = int::MAX;
            
            for k in i..<j {
                let cost = dp[i][k] + dp[k + 1][j] + dims[i] * dims[k + 1] * dims[j + 1];
                if cost < dp[i][j] {
                    dp[i][j] = cost;
                    split[i][j] = k;
                }
            }
        }
    }
    
    return (dp[0][n - 1], split);
}

fn print_optimal_parens(split: Array<Array<int>>, i: int, j: int) -> string {
    if i == j {
        return "A" + str(i);
    }
    
    let k = split[i][j];
    return "(" + print_optimal_parens(split, i, k) + " × " + 
           print_optimal_parens(split, k + 1, j) + ")";
}

fn test_matrix_chain() {
    // Matrix dimensions: A1(10×20), A2(20×30), A3(30×40), A4(40×30)
    let dims = [10, 20, 30, 40, 30];
    
    let min_cost = matrix_chain_order(dims);
    println("Minimum multiplications: " + str(min_cost)); // 30000
    
    let (cost, splits) = matrix_chain_order_with_splits(dims);
    let parens = print_optimal_parens(splits, 0, dims.len() - 2);
    println("Optimal order: " + parens); // ((A0 × A1) × (A2 × A3))
}
```

**Time Complexity**: O(n³)  
**Space Complexity**: O(n²)

---

## String DP

### 1. Longest Palindromic Substring

Find the longest palindromic substring in a given string.

```klang
fn longest_palindrome_substring(s: string) -> string {
    let n = s.len();
    if n == 0 { return ""; }
    
    // dp[i][j] = true if s[i..j] is palindrome
    let dp = Array<Array<bool>>(n);
    for i in 0..<n {
        dp[i] = Array<bool>(n, false);
    }
    
    let start = 0;
    let max_len = 1;
    
    // All single characters are palindromes
    for i in 0..<n {
        dp[i][i] = true;
    }
    
    // Check for length 2
    for i in 0..<(n - 1) {
        if s[i] == s[i + 1] {
            dp[i][i + 1] = true;
            start = i;
            max_len = 2;
        }
    }
    
    // Check for lengths 3 and above
    for len in 3..=n {
        for i in 0..=(n - len) {
            let j = i + len - 1;
            
            if s[i] == s[j] && dp[i + 1][j - 1] {
                dp[i][j] = true;
                start = i;
                max_len = len;
            }
        }
    }
    
    return s.substring(start, start + max_len);
}

// Expand around center approach - O(n²) time, O(1) space
fn longest_palindrome_expand(s: string) -> string {
    let n = s.len();
    if n == 0 { return ""; }
    
    let start = 0;
    let max_len = 0;
    
    fn expand(s: string, left: int, right: int) -> (int, int) {
        while left >= 0 && right < s.len() && s[left] == s[right] {
            left -= 1;
            right += 1;
        }
        return (left + 1, right - left - 1);
    }
    
    for i in 0..<n {
        // Odd length palindromes (center is a single character)
        let (start1, len1) = expand(s, i, i);
        
        // Even length palindromes (center is between two characters)
        let (start2, len2) = expand(s, i, i + 1);
        
        if len1 > max_len {
            start = start1;
            max_len = len1;
        }
        if len2 > max_len {
            start = start2;
            max_len = len2;
        }
    }
    
    return s.substring(start, start + max_len);
}
```

**Time Complexity**: O(n²)  
**Space Complexity**: O(n²) for DP, O(1) for expand approach

### 2. Longest Palindromic Subsequence

Find the length of the longest palindromic subsequence.

```klang
fn longest_palindrome_subsequence(s: string) -> int {
    let n = s.len();
    
    // dp[i][j] = length of LPS in s[i..j]
    let dp = Array<Array<int>>(n);
    for i in 0..<n {
        dp[i] = Array<int>(n, 0);
    }
    
    // Single characters are palindromes of length 1
    for i in 0..<n {
        dp[i][i] = 1;
    }
    
    // Build table for increasing lengths
    for len in 2..=n {
        for i in 0..=(n - len) {
            let j = i + len - 1;
            
            if s[i] == s[j] {
                dp[i][j] = 2 + (if len == 2 { 0 } else { dp[i + 1][j - 1] });
            } else {
                dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
            }
        }
    }
    
    return dp[0][n - 1];
}

// Alternative: LCS with reverse
fn longest_palindrome_subsequence_lcs(s: string) -> int {
    let reversed = s.reverse();
    return lcs_length(s, reversed);
}
```

**Time Complexity**: O(n²)  
**Space Complexity**: O(n²)

### 3. Word Break

Determine if a string can be segmented into words from a dictionary.

```klang
fn word_break(s: string, word_dict: Array<string>) -> bool {
    let n = s.len();
    
    // dp[i] = true if s[0..i-1] can be segmented
    let dp = Array<bool>(n + 1, false);
    dp[0] = true; // Empty string
    
    // Convert to set for O(1) lookup
    let dict_set = Set<string>();
    for word in word_dict {
        dict_set.add(word);
    }
    
    for i in 1..=n {
        for j in 0..<i {
            if dp[j] && dict_set.contains(s.substring(j, i)) {
                dp[i] = true;
                break;
            }
        }
    }
    
    return dp[n];
}

// Get all possible segmentations
fn word_break_all(s: string, word_dict: Array<string>) -> Array<string> {
    let dict_set = Set<string>();
    for word in word_dict {
        dict_set.add(word);
    }
    
    let memo = Map<int, Array<string>>();
    
    fn backtrack(start: int) -> Array<string> {
        if memo.has(start) {
            return memo.get(start);
        }
        
        let result = Array<string>();
        
        if start == s.len() {
            result.push("");
            return result;
        }
        
        for end in (start + 1)..=s.len() {
            let word = s.substring(start, end);
            if dict_set.contains(word) {
                let rest = backtrack(end);
                for sentence in rest {
                    result.push(word + (if sentence == "" { "" } else { " " + sentence }));
                }
            }
        }
        
        memo.set(start, result);
        return result;
    }
    
    return backtrack(0);
}

fn test_word_break() {
    let s = "leetcode";
    let dict = ["leet", "code"];
    
    println("Can break: " + str(word_break(s, dict))); // true
    
    let all = word_break_all(s, dict);
    println("All segmentations: " + str(all)); // ["leet code"]
}
```

**Time Complexity**: O(n² × k) where k is average word length  
**Space Complexity**: O(n)

---

## Grid DP

### 1. Unique Paths

Count unique paths from top-left to bottom-right in a grid (can only move right or down).

```klang
fn unique_paths(m: int, n: int) -> int {
    // dp[i][j] = number of paths to reach cell (i, j)
    let dp = Array<Array<int>>(m);
    for i in 0..<m {
        dp[i] = Array<int>(n, 0);
    }
    
    // Initialize first row and column (only one way to reach)
    for i in 0..<m {
        dp[i][0] = 1;
    }
    for j in 0..<n {
        dp[0][j] = 1;
    }
    
    // Fill the table
    for i in 1..<m {
        for j in 1..<n {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }
    
    return dp[m - 1][n - 1];
}

// Space optimized - O(n) space
fn unique_paths_optimized(m: int, n: int) -> int {
    let dp = Array<int>(n, 1);
    
    for i in 1..<m {
        for j in 1..<n {
            dp[j] += dp[j - 1];
        }
    }
    
    return dp[n - 1];
}

// With obstacles
fn unique_paths_with_obstacles(grid: Array<Array<int>>) -> int {
    let m = grid.len();
    let n = grid[0].len();
    
    if grid[0][0] == 1 || grid[m - 1][n - 1] == 1 {
        return 0;
    }
    
    let dp = Array<Array<int>>(m);
    for i in 0..<m {
        dp[i] = Array<int>(n, 0);
    }
    
    dp[0][0] = 1;
    
    // Fill first column
    for i in 1..<m {
        dp[i][0] = if grid[i][0] == 1 { 0 } else { dp[i - 1][0] };
    }
    
    // Fill first row
    for j in 1..<n {
        dp[0][j] = if grid[0][j] == 1 { 0 } else { dp[0][j - 1] };
    }
    
    // Fill rest of table
    for i in 1..<m {
        for j in 1..<n {
            if grid[i][j] == 1 {
                dp[i][j] = 0;
            } else {
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            }
        }
    }
    
    return dp[m - 1][n - 1];
}
```

**Time Complexity**: O(m × n)  
**Space Complexity**: O(m × n) for standard, O(n) for optimized

### 2. Minimum Path Sum

Find path with minimum sum from top-left to bottom-right.

```klang
fn min_path_sum(grid: Array<Array<int>>) -> int {
    let m = grid.len();
    let n = grid[0].len();
    
    let dp = Array<Array<int>>(m);
    for i in 0..<m {
        dp[i] = Array<int>(n, 0);
    }
    
    dp[0][0] = grid[0][0];
    
    // Fill first row
    for j in 1..<n {
        dp[0][j] = dp[0][j - 1] + grid[0][j];
    }
    
    // Fill first column
    for i in 1..<m {
        dp[i][0] = dp[i - 1][0] + grid[i][0];
    }
    
    // Fill rest
    for i in 1..<m {
        for j in 1..<n {
            dp[i][j] = grid[i][j] + min(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    
    return dp[m - 1][n - 1];
}

// Space optimized
fn min_path_sum_optimized(grid: Array<Array<int>>) -> int {
    let m = grid.len();
    let n = grid[0].len();
    
    let dp = Array<int>(n, 0);
    dp[0] = grid[0][0];
    
    for j in 1..<n {
        dp[j] = dp[j - 1] + grid[0][j];
    }
    
    for i in 1..<m {
        dp[0] += grid[i][0];
        for j in 1..<n {
            dp[j] = grid[i][j] + min(dp[j], dp[j - 1]);
        }
    }
    
    return dp[n - 1];
}
```

**Time Complexity**: O(m × n)  
**Space Complexity**: O(m × n) for standard, O(n) for optimized

### 3. Maximum Sum Path

Find path with maximum sum (can move right, down, or diagonally down-right).

```klang
fn max_path_sum(grid: Array<Array<int>>) -> int {
    let m = grid.len();
    let n = grid[0].len();
    
    let dp = Array<Array<int>>(m);
    for i in 0..<m {
        dp[i] = Array<int>(n, 0);
    }
    
    dp[0][0] = grid[0][0];
    
    // Fill first row
    for j in 1..<n {
        dp[0][j] = dp[0][j - 1] + grid[0][j];
    }
    
    // Fill first column
    for i in 1..<m {
        dp[i][0] = dp[i - 1][0] + grid[i][0];
    }
    
    // Fill rest (can come from top, left, or diagonal)
    for i in 1..<m {
        for j in 1..<n {
            dp[i][j] = grid[i][j] + max(dp[i - 1][j], max(dp[i][j - 1], dp[i - 1][j - 1]));
        }
    }
    
    return dp[m - 1][n - 1];
}
```

**Time Complexity**: O(m × n)  
**Space Complexity**: O(m × n)

---

## DP Patterns

Understanding common DP patterns helps recognize when and how to apply DP.

### 1. Linear DP (1D)

Problems where state depends on previous elements in a sequence.

**Pattern**: `dp[i]` depends on `dp[i-1]`, `dp[i-2]`, etc.

**Examples**:
- Fibonacci numbers
- Climbing stairs
- House robber
- Decode ways

```klang
// Template
fn linear_dp(arr: Array<int>) -> int {
    let n = arr.len();
    let dp = Array<int>(n, 0);
    
    // Base cases
    dp[0] = /* base value */;
    dp[1] = /* base value */;
    
    // Fill table
    for i in 2..<n {
        dp[i] = /* function of dp[i-1], dp[i-2], arr[i] */;
    }
    
    return dp[n - 1];
}
```

### 2. 2D DP (Grid/Matrix)

Problems involving two sequences or a 2D grid.

**Pattern**: `dp[i][j]` depends on `dp[i-1][j]`, `dp[i][j-1]`, `dp[i-1][j-1]`

**Examples**:
- Longest common subsequence
- Edit distance
- Unique paths
- Minimum path sum

```klang
// Template
fn grid_dp(grid: Array<Array<int>>) -> int {
    let m = grid.len();
    let n = grid[0].len();
    let dp = Array<Array<int>>(m);
    
    for i in 0..<m {
        dp[i] = Array<int>(n, 0);
    }
    
    // Base cases
    dp[0][0] = /* initial value */;
    
    // Fill table
    for i in 0..<m {
        for j in 0..<n {
            dp[i][j] = /* function of dp[i-1][j], dp[i][j-1], etc. */;
        }
    }
    
    return dp[m - 1][n - 1];
}
```

### 3. String DP

Problems involving string manipulation or subsequences.

**Pattern**: Often uses 2D DP with string indices

**Examples**:
- Longest palindromic substring
- Word break
- Regular expression matching

```klang
// Template
fn string_dp(s: string) -> int {
    let n = s.len();
    let dp = Array<Array<int>>(n);
    
    for i in 0..<n {
        dp[i] = Array<int>(n, 0);
    }
    
    // Process by length
    for len in 1..=n {
        for i in 0..=(n - len) {
            let j = i + len - 1;
            dp[i][j] = /* function of s[i], s[j], dp values */;
        }
    }
    
    return dp[0][n - 1];
}
```

### 4. State Machine DP

Problems with distinct states and transitions.

**Pattern**: `dp[i][state]` where state represents current condition

**Examples**:
- Best time to buy/sell stock (with cooldown/transactions)
- Paint house with color constraints

```klang
// Template
fn state_machine_dp(arr: Array<int>, num_states: int) -> int {
    let n = arr.len();
    let dp = Array<Array<int>>(n);
    
    for i in 0..<n {
        dp[i] = Array<int>(num_states, 0);
    }
    
    // Base cases
    for state in 0..<num_states {
        dp[0][state] = /* initial value */;
    }
    
    // Transitions
    for i in 1..<n {
        for state in 0..<num_states {
            dp[i][state] = /* max/min of transitions from previous states */;
        }
    }
    
    return max_of_all_states(dp[n - 1]);
}
```

### 5. Interval DP

Problems involving intervals or ranges.

**Pattern**: `dp[i][j]` represents solution for interval `[i, j]`

**Examples**:
- Matrix chain multiplication
- Burst balloons
- Palindrome partitioning

```klang
// Template
fn interval_dp(arr: Array<int>) -> int {
    let n = arr.len();
    let dp = Array<Array<int>>(n);
    
    for i in 0..<n {
        dp[i] = Array<int>(n, 0);
    }
    
    // Process by interval length
    for len in 1..=n {
        for i in 0..=(n - len) {
            let j = i + len - 1;
            
            // Try all split points
            for k in i..<j {
                dp[i][j] = /* optimal of dp[i][k] + dp[k+1][j] + cost */;
            }
        }
    }
    
    return dp[0][n - 1];
}
```

---

## State Definition

Defining the right DP state is crucial for solving problems.

### Steps to Define DP State

1. **Identify what you're optimizing**: Maximum, minimum, count, boolean
2. **Determine what information is needed**: Index, capacity, previous choice, etc.
3. **Define state variables**: `dp[i]`, `dp[i][j]`, `dp[i][j][k]`, etc.
4. **Write state meaning clearly**: "dp[i] = maximum value using first i elements"

### Common State Definitions

```klang
// 1D State Examples
dp[i] = /* result for position/index i */
dp[i] = /* result using first i elements */
dp[i] = /* result ending at position i */

// 2D State Examples
dp[i][j] = /* result for range [i, j] */
dp[i][j] = /* result using first i items with capacity j */
dp[i][state] = /* result at position i in given state */

// 3D State Examples (less common)
dp[i][j][k] = /* result at position i, with j transactions, in state k */
```

### Example: House Robber

```klang
// Problem: Rob houses, can't rob adjacent houses
// State definition: dp[i] = maximum money robbing houses 0..i

fn rob(houses: Array<int>) -> int {
    let n = houses.len();
    if n == 0 { return 0; }
    if n == 1 { return houses[0]; }
    
    let dp = Array<int>(n, 0);
    dp[0] = houses[0];
    dp[1] = max(houses[0], houses[1]);
    
    for i in 2..<n {
        // Either rob current house (+ dp[i-2]) or skip it (dp[i-1])
        dp[i] = max(houses[i] + dp[i - 2], dp[i - 1]);
    }
    
    return dp[n - 1];
}
```

---

## Transition Formula

The recurrence relation that defines how to compute current state from previous states.

### Steps to Derive Transition Formula

1. **Understand the problem constraints**
2. **List all choices/decisions at current state**
3. **Express current state in terms of previous states**
4. **Choose optimal (min/max) or sum all possibilities**

### Common Transition Patterns

```klang
// Choice between options
dp[i] = max(option1, option2, ...);
dp[i] = min(option1, option2, ...);

// Sum of ways
dp[i] = sum(dp[previous_states]);

// Include/exclude current element
dp[i] = max(include_current + dp[i-k], exclude_current);

// Multiple dependencies
dp[i][j] = f(dp[i-1][j], dp[i][j-1], dp[i-1][j-1], ...);
```

### Examples

```klang
// Fibonacci
dp[i] = dp[i-1] + dp[i-2]

// Coin change (min coins)
for coin in coins:
    dp[amount] = min(dp[amount], 1 + dp[amount - coin])

// Knapsack
dp[i][w] = max(
    dp[i-1][w],                      // exclude item
    value[i] + dp[i-1][w-weight[i]]  // include item
)

// LCS
if s1[i] == s2[j]:
    dp[i][j] = 1 + dp[i-1][j-1]
else:
    dp[i][j] = max(dp[i-1][j], dp[i][j-1])
```

---

## Space Optimization

Reducing space complexity from 2D to 1D or from O(n) to O(1).

### Technique 1: Rolling Array

When `dp[i]` only depends on `dp[i-1]` or `dp[i-2]`, use variables instead of array.

```klang
// Before: O(n) space
fn fib_space_n(n: int) -> int {
    let dp = Array<int>(n + 1, 0);
    dp[0] = 0;
    dp[1] = 1;
    for i in 2..=n {
        dp[i] = dp[i-1] + dp[i-2];
    }
    return dp[n];
}

// After: O(1) space
fn fib_space_1(n: int) -> int {
    if n <= 1 { return n; }
    let prev2 = 0;
    let prev1 = 1;
    for i in 2..=n {
        let curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
```

### Technique 2: 2D to 1D

When current row only depends on previous row.

```klang
// Before: O(m × n) space
fn unique_paths_2d(m: int, n: int) -> int {
    let dp = Array<Array<int>>(m);
    for i in 0..<m {
        dp[i] = Array<int>(n, 1);
    }
    
    for i in 1..<m {
        for j in 1..<n {
            dp[i][j] = dp[i-1][j] + dp[i][j-1];
        }
    }
    return dp[m-1][n-1];
}

// After: O(n) space
fn unique_paths_1d(m: int, n: int) -> int {
    let dp = Array<int>(n, 1);
    
    for i in 1..<m {
        for j in 1..<n {
            dp[j] += dp[j-1];  // dp[j] already has dp[i-1][j]
        }
    }
    return dp[n-1];
}
```

### Technique 3: In-Place Modification

When you can modify input array.

```klang
// Modify grid in-place for min path sum
fn min_path_sum_inplace(grid: Array<Array<int>>) -> int {
    let m = grid.len();
    let n = grid[0].len();
    
    for i in 1..<n {
        grid[0][i] += grid[0][i-1];
    }
    
    for i in 1..<m {
        grid[i][0] += grid[i-1][0];
    }
    
    for i in 1..<m {
        for j in 1..<n {
            grid[i][j] += min(grid[i-1][j], grid[i][j-1]);
        }
    }
    
    return grid[m-1][n-1];
}
```

---

## Interview Problems

### 1. House Robber

```klang
fn rob(nums: Array<int>) -> int {
    let n = nums.len();
    if n == 0 { return 0; }
    if n == 1 { return nums[0]; }
    
    let prev2 = 0;
    let prev1 = 0;
    
    for num in nums {
        let curr = max(prev1, prev2 + num);
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
```

### 2. House Robber II (Circular)

Houses are arranged in a circle - first and last are adjacent.

```klang
fn rob_circular(nums: Array<int>) -> int {
    let n = nums.len();
    if n == 0 { return 0; }
    if n == 1 { return nums[0]; }
    
    // Helper function to rob linear houses
    fn rob_range(nums: Array<int>, start: int, end: int) -> int {
        let prev2 = 0;
        let prev1 = 0;
        
        for i in start..=end {
            let curr = max(prev1, prev2 + nums[i]);
            prev2 = prev1;
            prev1 = curr;
        }
        
        return prev1;
    }
    
    // Case 1: Rob houses 0 to n-2 (exclude last)
    // Case 2: Rob houses 1 to n-1 (exclude first)
    return max(rob_range(nums, 0, n - 2), rob_range(nums, 1, n - 1));
}
```

### 3. Decode Ways

Count ways to decode a string where 'A'=1, 'B'=2, ..., 'Z'=26.

```klang
fn num_decodings(s: string) -> int {
    let n = s.len();
    if n == 0 || s[0] == '0' { return 0; }
    
    let dp = Array<int>(n + 1, 0);
    dp[0] = 1; // Empty string
    dp[1] = 1; // First character
    
    for i in 2..=n {
        // Single digit decode
        if s[i - 1] != '0' {
            dp[i] += dp[i - 1];
        }
        
        // Two digit decode
        let two_digit = (s[i - 2] - '0') * 10 + (s[i - 1] - '0');
        if two_digit >= 10 && two_digit <= 26 {
            dp[i] += dp[i - 2];
        }
    }
    
    return dp[n];
}
```

### 4. Partition Equal Subset Sum

Can array be partitioned into two subsets with equal sum?

```klang
fn can_partition(nums: Array<int>) -> bool {
    let total = 0;
    for num in nums {
        total += num;
    }
    
    // If total is odd, can't partition equally
    if total % 2 != 0 { return false; }
    
    let target = total / 2;
    
    // This is subset sum problem
    return subset_sum_optimized(nums, target);
}
```

### 5. Target Sum

Assign + or - to each number to reach target sum. Count ways.

```klang
fn find_target_sum_ways(nums: Array<int>, target: int) -> int {
    let total = 0;
    for num in nums {
        total += num;
    }
    
    // P - N = target, P + N = total
    // So P = (target + total) / 2
    if (target + total) % 2 != 0 || target > total || target < -total {
        return 0;
    }
    
    let sum = (target + total) / 2;
    
    // Count subsets with given sum
    let dp = Array<int>(sum + 1, 0);
    dp[0] = 1;
    
    for num in nums {
        for s in sum..=num by -1 {
            dp[s] += dp[s - num];
        }
    }
    
    return dp[sum];
}
```

### 6. Best Time to Buy and Sell Stock

#### One Transaction

```klang
fn max_profit_one(prices: Array<int>) -> int {
    let min_price = int::MAX;
    let max_profit = 0;
    
    for price in prices {
        min_price = min(min_price, price);
        max_profit = max(max_profit, price - min_price);
    }
    
    return max_profit;
}
```

#### Unlimited Transactions

```klang
fn max_profit_unlimited(prices: Array<int>) -> int {
    let profit = 0;
    
    for i in 1..<prices.len() {
        if prices[i] > prices[i - 1] {
            profit += prices[i] - prices[i - 1];
        }
    }
    
    return profit;
}
```

#### At Most K Transactions

```klang
fn max_profit_k(k: int, prices: Array<int>) -> int {
    let n = prices.len();
    if n == 0 { return 0; }
    
    // If k >= n/2, unlimited transactions
    if k >= n / 2 {
        return max_profit_unlimited(prices);
    }
    
    // dp[i][j] = max profit with at most i transactions up to day j
    let dp = Array<Array<int>>(k + 1);
    for i in 0..=k {
        dp[i] = Array<int>(n, 0);
    }
    
    for i in 1..=k {
        let max_diff = -prices[0];
        for j in 1..<n {
            dp[i][j] = max(dp[i][j - 1], prices[j] + max_diff);
            max_diff = max(max_diff, dp[i - 1][j] - prices[j]);
        }
    }
    
    return dp[k][n - 1];
}
```

#### With Cooldown

```klang
fn max_profit_cooldown(prices: Array<int>) -> int {
    let n = prices.len();
    if n == 0 { return 0; }
    
    // States: hold, sold, rest
    let hold = -prices[0];
    let sold = 0;
    let rest = 0;
    
    for i in 1..<n {
        let prev_hold = hold;
        let prev_sold = sold;
        let prev_rest = rest;
        
        hold = max(prev_hold, prev_rest - prices[i]);
        sold = prev_hold + prices[i];
        rest = max(prev_rest, prev_sold);
    }
    
    return max(sold, rest);
}
```

### 7. Longest Palindromic Substring (see String DP section)

### 8. Palindrome Partitioning

Minimum cuts needed to partition string into palindromes.

```klang
fn min_cut(s: string) -> int {
    let n = s.len();
    
    // is_pal[i][j] = true if s[i..j] is palindrome
    let is_pal = Array<Array<bool>>(n);
    for i in 0..<n {
        is_pal[i] = Array<bool>(n, false);
    }
    
    // Build palindrome table
    for len in 1..=n {
        for i in 0..=(n - len) {
            let j = i + len - 1;
            if len == 1 {
                is_pal[i][j] = true;
            } else if len == 2 {
                is_pal[i][j] = s[i] == s[j];
            } else {
                is_pal[i][j] = s[i] == s[j] && is_pal[i + 1][j - 1];
            }
        }
    }
    
    // dp[i] = min cuts for s[0..i]
    let dp = Array<int>(n, 0);
    
    for i in 0..<n {
        if is_pal[0][i] {
            dp[i] = 0;
        } else {
            dp[i] = i; // Maximum cuts
            for j in 0..i {
                if is_pal[j + 1][i] {
                    dp[i] = min(dp[i], dp[j] + 1);
                }
            }
        }
    }
    
    return dp[n - 1];
}
```

### 9. Interleaving String

Check if s3 is formed by interleaving s1 and s2.

```klang
fn is_interleave(s1: string, s2: string, s3: string) -> bool {
    let m = s1.len();
    let n = s2.len();
    
    if m + n != s3.len() { return false; }
    
    // dp[i][j] = can s3[0..i+j-1] be formed from s1[0..i-1] and s2[0..j-1]
    let dp = Array<Array<bool>>(m + 1);
    for i in 0..=m {
        dp[i] = Array<bool>(n + 1, false);
    }
    
    dp[0][0] = true;
    
    // First row
    for i in 1..=m {
        dp[i][0] = dp[i - 1][0] && s1[i - 1] == s3[i - 1];
    }
    
    // First column
    for j in 1..=n {
        dp[0][j] = dp[0][j - 1] && s2[j - 1] == s3[j - 1];
    }
    
    // Fill table
    for i in 1..=m {
        for j in 1..=n {
            let k = i + j - 1;
            dp[i][j] = (dp[i - 1][j] && s1[i - 1] == s3[k]) ||
                       (dp[i][j - 1] && s2[j - 1] == s3[k]);
        }
    }
    
    return dp[m][n];
}
```

### 10. Distinct Subsequences

Count distinct subsequences of s that equals t.

```klang
fn num_distinct(s: string, t: string) -> int {
    let m = s.len();
    let n = t.len();
    
    // dp[i][j] = number of distinct subsequences of s[0..i-1] that equal t[0..j-1]
    let dp = Array<Array<int>>(m + 1);
    for i in 0..=m {
        dp[i] = Array<int>(n + 1, 0);
    }
    
    // Empty string is subsequence of any string
    for i in 0..=m {
        dp[i][0] = 1;
    }
    
    for i in 1..=m {
        for j in 1..=n {
            dp[i][j] = dp[i - 1][j]; // Don't use s[i-1]
            
            if s[i - 1] == t[j - 1] {
                dp[i][j] += dp[i - 1][j - 1]; // Use s[i-1]
            }
        }
    }
    
    return dp[m][n];
}
```

### 11. Regular Expression Matching

```klang
fn is_match(s: string, p: string) -> bool {
    let m = s.len();
    let n = p.len();
    
    // dp[i][j] = does s[0..i-1] match p[0..j-1]
    let dp = Array<Array<bool>>(m + 1);
    for i in 0..=m {
        dp[i] = Array<bool>(n + 1, false);
    }
    
    dp[0][0] = true;
    
    // Handle patterns like a*, a*b*, etc.
    for j in 2..=n {
        if p[j - 1] == '*' {
            dp[0][j] = dp[0][j - 2];
        }
    }
    
    for i in 1..=m {
        for j in 1..=n {
            if p[j - 1] == '*' {
                // Zero occurrences of preceding element
                dp[i][j] = dp[i][j - 2];
                
                // One or more occurrences
                if p[j - 2] == s[i - 1] || p[j - 2] == '.' {
                    dp[i][j] = dp[i][j] || dp[i - 1][j];
                }
            } else if p[j - 1] == '.' || p[j - 1] == s[i - 1] {
                dp[i][j] = dp[i - 1][j - 1];
            }
        }
    }
    
    return dp[m][n];
}
```

### 12. Wildcard Matching

```klang
fn is_match_wildcard(s: string, p: string) -> bool {
    let m = s.len();
    let n = p.len();
    
    let dp = Array<Array<bool>>(m + 1);
    for i in 0..=m {
        dp[i] = Array<bool>(n + 1, false);
    }
    
    dp[0][0] = true;
    
    // Handle leading * in pattern
    for j in 1..=n {
        if p[j - 1] == '*' {
            dp[0][j] = dp[0][j - 1];
        }
    }
    
    for i in 1..=m {
        for j in 1..=n {
            if p[j - 1] == '*' {
                dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
            } else if p[j - 1] == '?' || p[j - 1] == s[i - 1] {
                dp[i][j] = dp[i - 1][j - 1];
            }
        }
    }
    
    return dp[m][n];
}
```

---

## Best Practices

### 1. Start with Recursion

Always start by writing a recursive solution, then optimize with memoization or tabulation.

```klang
// Step 1: Write recursive solution
fn solve_recursive(n: int) -> int {
    if base_case { return value; }
    return combine(solve_recursive(smaller_problem));
}

// Step 2: Add memoization
fn solve_memo(n: int, memo: Map) -> int {
    if base_case { return value; }
    if memo.has(n) { return memo.get(n); }
    
    let result = combine(solve_memo(smaller_problem, memo));
    memo.set(n, result);
    return result;
}

// Step 3: Convert to tabulation
fn solve_tab(n: int) -> int {
    let dp = Array(n);
    dp[base] = base_value;
    
    for i in range {
        dp[i] = combine(dp[previous]);
    }
    
    return dp[n];
}
```

### 2. Draw State Transition Diagram

Visualize how states relate to each other.

### 3. Write Clear State Definition

Always document what each DP state represents.

```klang
// dp[i] = maximum profit using first i houses
// dp[i][j] = LCS length of s1[0..i] and s2[0..j]
// dp[i][j][k] = result at position i with j transactions in state k
```

### 4. Initialize Base Cases Carefully

Wrong base cases lead to wrong answers.

```klang
// For sum/count problems
dp[0] = 1;  // One way to make sum 0

// For min problems
dp[0] = 0;
dp[i] = infinity; // Initialize to impossible value

// For max problems
dp[0] = 0;
dp[i] = -infinity;
```

### 5. Check Array Bounds

Avoid index out of bounds errors.

```klang
if i > 0 && j > 0 {
    dp[i][j] = dp[i-1][j-1];
}
```

### 6. Consider Space Optimization Last

First make it work, then optimize space.

### 7. Test with Small Examples

Verify with simple test cases before complex ones.

---

## Common Patterns

### Pattern Recognition Guide

| Problem Type | Key Indicator | Common Approach |
|-------------|---------------|-----------------|
| **Counting paths** | "How many ways to..." | Linear/Grid DP |
| **Optimization** | "Maximum/minimum value" | State optimization |
| **Subsequence** | "Longest/shortest subsequence" | 2D DP with two strings |
| **String matching** | "Match pattern/substring" | 2D DP with characters |
| **Knapsack variants** | "Select items with constraints" | 2D DP (items × capacity) |
| **Partition** | "Divide into groups" | Subset sum variant |
| **State transitions** | "Different states/conditions" | State machine DP |
| **Interval** | "Split into parts" | Interval DP |

### Quick Pattern Matching

```klang
// See these keywords? Think DP:
- "Maximum/Minimum"
- "How many ways"
- "Is it possible"
- "Longest/Shortest"
- "Optimal"

// See these structures? Likely DP:
- Grid/Matrix traversal
- String subsequences
- Choosing/not choosing items
- State transitions
- Interval splitting
```

### Common DP Mistakes

1. **Wrong base cases**: Always verify base case values
2. **Wrong iteration order**: Especially when space-optimizing
3. **Forgotten memoization check**: Always check cache first
4. **Integer overflow**: Use appropriate data types
5. **Not considering edge cases**: Empty arrays, single elements
6. **Inefficient space**: When 1D solution exists but using 2D

---

## Complexity Summary

| Problem | Time | Space |
|---------|------|-------|
| Fibonacci | O(n) | O(1) optimized |
| Climbing Stairs | O(n) | O(1) optimized |
| 0/1 Knapsack | O(n×W) | O(W) optimized |
| Coin Change | O(n×amount) | O(amount) |
| LCS | O(m×n) | O(m×n) |
| LIS (DP) | O(n²) | O(n) |
| LIS (Binary) | O(n log n) | O(n) |
| Edit Distance | O(m×n) | O(n) optimized |
| Matrix Chain | O(n³) | O(n²) |
| Unique Paths | O(m×n) | O(n) optimized |

---

This comprehensive guide covers the essential concepts, patterns, and problems in Dynamic Programming using KLang. Practice these patterns and problems to master DP!
