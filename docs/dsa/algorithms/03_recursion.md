# Recursion in KLang - Data Structures & Algorithms

## Table of Contents
1. [Introduction](#introduction)
2. [Recursion Fundamentals](#recursion-fundamentals)
3. [Simple Recursion Examples](#simple-recursion-examples)
4. [Recursion vs Iteration](#recursion-vs-iteration)
5. [Types of Recursion](#types-of-recursion)
6. [Backtracking](#backtracking)
7. [Memoization](#memoization)
8. [Tail Recursion](#tail-recursion)
9. [Recursion Tree](#recursion-tree)
10. [Interview Problems](#interview-problems)
11. [Best Practices](#best-practices)
12. [Common Mistakes](#common-mistakes)

---

## Introduction

Recursion is a powerful programming technique where a function calls itself to solve smaller instances of the same problem. It's a fundamental concept in computer science and forms the basis for many algorithms and data structures.

### What is Recursion?

**Recursion** is a method of solving problems where the solution depends on solutions to smaller instances of the same problem. A recursive function calls itself with modified arguments until it reaches a base case.

### When to Use Recursion

Recursion is particularly useful when:

1. **Natural Recursive Structure**: Problem can be broken into similar subproblems (tree traversal, factorial)
2. **Backtracking Problems**: Need to explore multiple possibilities (N-Queens, Sudoku)
3. **Divide and Conquer**: Problem splits into independent subproblems (merge sort, quick sort)
4. **Tree/Graph Problems**: Naturally recursive data structures
5. **Mathematical Problems**: Defined recursively (Fibonacci, GCD)

### Advantages

- **Elegant Code**: Often more readable and concise than iterative solutions
- **Natural Fit**: Perfect for problems with recursive structure
- **Easier Implementation**: Complex problems become simpler
- **Backtracking**: Essential for exploring solution spaces

### Disadvantages

- **Memory Overhead**: Each call uses stack space
- **Performance**: Function call overhead
- **Stack Overflow**: Risk with deep recursion
- **Debugging**: Can be harder to trace execution

---

## Recursion Fundamentals

Every recursive function has two essential components:

### 1. Base Case (Termination Condition)

The condition that stops the recursion. Without it, the function would call itself infinitely.

```klang
// Example: Base case for factorial
fn factorial(n: int) -> int {
    if n <= 1 {
        return 1;  // Base case
    }
    return n * factorial(n - 1);
}
```

### 2. Recursive Case (Self-Call)

The part where the function calls itself with modified arguments, making progress toward the base case.

```klang
// Recursive case breaks problem into smaller subproblem
return n * factorial(n - 1);  // Recursive case
```

### Call Stack Visualization

When a function calls itself, each call is added to the call stack:

```
factorial(4) call stack:

factorial(4)
    ↓ calls factorial(3)
    factorial(3)
        ↓ calls factorial(2)
        factorial(2)
            ↓ calls factorial(1)
            factorial(1) → returns 1 (base case)
        ← returns 2 * 1 = 2
    ← returns 3 * 2 = 6
← returns 4 * 6 = 24
```

### How Recursion Works

```klang
fn recursive_function(problem) {
    // Base case - smallest problem we can solve directly
    if problem is simple enough {
        return direct_solution;
    }
    
    // Recursive case - break into smaller problems
    smaller_problem = reduce(problem);
    smaller_solution = recursive_function(smaller_problem);
    
    // Combine solutions
    return construct_solution(smaller_solution);
}
```

---

## Simple Recursion Examples

### 1. Factorial

Calculate n! = n × (n-1) × (n-2) × ... × 1

```klang
// Factorial: n! = n * (n-1)!
fn factorial(n: int) -> int {
    // Base case
    if n <= 1 {
        return 1;
    }
    
    // Recursive case
    return n * factorial(n - 1);
}

// Example usage
fn test_factorial() {
    println("5! = " + str(factorial(5)));  // 120
    println("7! = " + str(factorial(7)));  // 5040
}
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n) - call stack depth

### 2. Fibonacci Sequence

Find the nth Fibonacci number: F(n) = F(n-1) + F(n-2)

```klang
// Fibonacci: 0, 1, 1, 2, 3, 5, 8, 13, 21...
fn fibonacci(n: int) -> int {
    // Base cases
    if n <= 1 {
        return n;
    }
    
    // Recursive case: F(n) = F(n-1) + F(n-2)
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Example usage
fn test_fibonacci() {
    for i in 0..10 {
        println("F(" + str(i) + ") = " + str(fibonacci(i)));
    }
}
```

**Time Complexity**: O(2^n) - exponential due to repeated calculations  
**Space Complexity**: O(n) - maximum call stack depth

### 3. Sum of Digits

Calculate the sum of all digits in a number.

```klang
// Sum of digits: 1234 → 1 + 2 + 3 + 4 = 10
fn sum_of_digits(n: int) -> int {
    // Base case: single digit
    if n < 10 {
        return n;
    }
    
    // Recursive case: last digit + sum of remaining
    return (n % 10) + sum_of_digits(n / 10);
}

// Example usage
fn test_sum_of_digits() {
    println("Sum of digits in 1234: " + str(sum_of_digits(1234)));  // 10
    println("Sum of digits in 9875: " + str(sum_of_digits(9875)));  // 29
}
```

**Time Complexity**: O(log n) - number of digits  
**Space Complexity**: O(log n) - call stack depth

### 4. Power Function

Calculate x raised to the power n: x^n

```klang
// Power: x^n = x * x^(n-1)
fn power(x: int, n: int) -> int {
    // Base case
    if n == 0 {
        return 1;
    }
    
    // Recursive case
    return x * power(x, n - 1);
}

// Optimized version using divide and conquer
fn power_optimized(x: int, n: int) -> int {
    // Base case
    if n == 0 {
        return 1;
    }
    
    // If n is negative
    if n < 0 {
        return 1 / power_optimized(x, -n);
    }
    
    // If n is even: x^n = (x^(n/2))^2
    if n % 2 == 0 {
        let half = power_optimized(x, n / 2);
        return half * half;
    }
    
    // If n is odd: x^n = x * x^(n-1)
    return x * power_optimized(x, n - 1);
}

// Example usage
fn test_power() {
    println("2^5 = " + str(power(2, 5)));           // 32
    println("3^4 = " + str(power_optimized(3, 4)));  // 81
    println("5^0 = " + str(power_optimized(5, 0)));  // 1
}
```

**Time Complexity**: O(n) for basic, O(log n) for optimized  
**Space Complexity**: O(n) for basic, O(log n) for optimized

### 5. Greatest Common Divisor (GCD)

Find GCD using Euclidean algorithm.

```klang
// GCD using Euclidean algorithm: gcd(a, b) = gcd(b, a % b)
fn gcd(a: int, b: int) -> int {
    // Base case
    if b == 0 {
        return a;
    }
    
    // Recursive case
    return gcd(b, a % b);
}

// Example usage
fn test_gcd() {
    println("GCD(48, 18) = " + str(gcd(48, 18)));  // 6
    println("GCD(100, 15) = " + str(gcd(100, 15)));  // 5
}
```

**Time Complexity**: O(log min(a, b))  
**Space Complexity**: O(log min(a, b))

### 6. Reverse a String

```klang
// Reverse string recursively
fn reverse_string(s: string) -> string {
    // Base case: empty or single character
    if s.length() <= 1 {
        return s;
    }
    
    // Recursive case: last char + reverse of rest
    return s[s.length() - 1] + reverse_string(s.substring(0, s.length() - 1));
}

// Example usage
fn test_reverse() {
    println(reverse_string("hello"));  // "olleh"
    println(reverse_string("recursion"));  // "noisrucer"
}
```

---

## Recursion vs Iteration

Both recursion and iteration can solve the same problems, but each has trade-offs.

### Comparison Table

| Aspect | Recursion | Iteration |
|--------|-----------|-----------|
| **Code Clarity** | Often more elegant and readable | Can be more verbose |
| **Memory Usage** | Uses call stack (higher overhead) | Uses loop variables (lower overhead) |
| **Performance** | Function call overhead | Generally faster |
| **Stack Overflow** | Risk with deep recursion | No risk |
| **Problem Fit** | Natural for tree/graph problems | Better for simple loops |
| **Debugging** | Can be harder to trace | Usually easier |

### Example: Factorial

**Recursive Approach:**
```klang
fn factorial_recursive(n: int) -> int {
    if n <= 1 {
        return 1;
    }
    return n * factorial_recursive(n - 1);
}
```

**Iterative Approach:**
```klang
fn factorial_iterative(n: int) -> int {
    let result = 1;
    for i in 2..=n {
        result *= i;
    }
    return result;
}
```

### Example: Fibonacci

**Recursive Approach (inefficient):**
```klang
fn fib_recursive(n: int) -> int {
    if n <= 1 {
        return n;
    }
    return fib_recursive(n - 1) + fib_recursive(n - 2);
}
// Time: O(2^n), Space: O(n)
```

**Iterative Approach:**
```klang
fn fib_iterative(n: int) -> int {
    if n <= 1 {
        return n;
    }
    
    let prev = 0;
    let curr = 1;
    
    for i in 2..=n {
        let next = prev + curr;
        prev = curr;
        curr = next;
    }
    
    return curr;
}
// Time: O(n), Space: O(1)
```

### When to Choose Recursion

1. **Tree/Graph Traversal**: Natural recursive structure
2. **Backtracking Problems**: Exploring solution spaces
3. **Divide and Conquer**: Merge sort, quick sort
4. **Code Simplicity**: When recursive solution is much cleaner

### When to Choose Iteration

1. **Performance Critical**: Avoid function call overhead
2. **Deep Recursion**: Risk of stack overflow
3. **Simple Loops**: Counting, accumulating
4. **Memory Constraints**: Limited stack space

---

## Types of Recursion

### 1. Direct Recursion

A function calls itself directly.

```klang
fn direct_recursion(n: int) -> int {
    if n <= 0 {
        return 0;
    }
    return n + direct_recursion(n - 1);  // Calls itself
}
```

### 2. Indirect Recursion

Functions call each other in a cycle: A → B → A

```klang
fn function_a(n: int) -> int {
    if n <= 0 {
        return 0;
    }
    return n + function_b(n - 1);  // Calls B
}

fn function_b(n: int) -> int {
    if n <= 0 {
        return 0;
    }
    return n + function_a(n - 1);  // Calls A
}
```

### 3. Tail Recursion

The recursive call is the last operation in the function. Can be optimized by compilers.

```klang
// Tail recursive factorial
fn factorial_tail(n: int, accumulator: int) -> int {
    if n <= 1 {
        return accumulator;
    }
    return factorial_tail(n - 1, n * accumulator);  // Last operation
}

fn factorial(n: int) -> int {
    return factorial_tail(n, 1);
}
```

**Benefits:**
- Can be optimized to use constant stack space
- Converted to iteration by compiler
- No stack overflow risk

### 4. Head Recursion

The recursive call is the first operation; processing happens after the recursive call.

```klang
// Head recursive factorial
fn factorial_head(n: int) -> int {
    if n <= 1 {
        return 1;
    }
    let result = factorial_head(n - 1);  // Recursive call first
    return n * result;  // Processing after
}
```

### 5. Tree Recursion

The function makes multiple recursive calls (branches like a tree).

```klang
// Tree recursion - Fibonacci
fn fibonacci(n: int) -> int {
    if n <= 1 {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);  // Two recursive calls
}
```

**Characteristics:**
- Multiple recursive calls per invocation
- Creates tree-like call structure
- Often exponential time complexity
- Common in backtracking and divide-and-conquer

---

## Backtracking

Backtracking is an algorithmic technique for solving problems recursively by trying to build a solution incrementally and abandoning solutions ("backtracking") as soon as it determines they cannot lead to a valid solution.

### What is Backtracking?

Backtracking is a refined brute force approach that:
1. Builds candidates for solutions incrementally
2. Abandons candidates ("backtracks") when they cannot lead to valid solutions
3. Explores all possibilities systematically

### Backtracking Template

```klang
fn backtrack(state, choices) {
    // Base case: found valid solution
    if is_solution(state) {
        add_solution(state);
        return;
    }
    
    // Try each choice
    for choice in choices {
        // Make choice
        if is_valid(choice, state) {
            make_choice(choice, state);
            
            // Recurse
            backtrack(state, next_choices);
            
            // Backtrack (undo choice)
            undo_choice(choice, state);
        }
    }
}
```

### General Pattern

1. **Choose**: Make a choice and add to partial solution
2. **Explore**: Recursively explore with this choice
3. **Unchoose**: Backtrack by removing the choice

---

### Problem 1: N-Queens Problem

Place N queens on an N×N chessboard so no two queens attack each other.

```klang
class NQueens {
    fn new() -> NQueens {
        return NQueens();
    }
    
    // Solve N-Queens problem
    fn solve_n_queens(n: int) -> [[string]] {
        let solutions: [[string]] = [];
        let board: [[bool]] = [];
        
        // Initialize board
        for i in 0..n {
            let row: [bool] = [];
            for j in 0..n {
                row.push(false);
            }
            board.push(row);
        }
        
        this.backtrack(board, 0, n, solutions);
        return solutions;
    }
    
    // Backtracking function
    fn backtrack(board: [[bool]], row: int, n: int, solutions: [[string]]) {
        // Base case: placed all queens
        if row == n {
            solutions.push(this.board_to_string(board, n));
            return;
        }
        
        // Try placing queen in each column of current row
        for col in 0..n {
            if this.is_safe(board, row, col, n) {
                // Make choice
                board[row][col] = true;
                
                // Recurse to next row
                this.backtrack(board, row + 1, n, solutions);
                
                // Backtrack
                board[row][col] = false;
            }
        }
    }
    
    // Check if placing queen at (row, col) is safe
    fn is_safe(board: [[bool]], row: int, col: int, n: int) -> bool {
        // Check column
        for i in 0..row {
            if board[i][col] {
                return false;
            }
        }
        
        // Check upper-left diagonal
        let i = row - 1;
        let j = col - 1;
        while i >= 0 && j >= 0 {
            if board[i][j] {
                return false;
            }
            i -= 1;
            j -= 1;
        }
        
        // Check upper-right diagonal
        i = row - 1;
        j = col + 1;
        while i >= 0 && j < n {
            if board[i][j] {
                return false;
            }
            i -= 1;
            j += 1;
        }
        
        return true;
    }
    
    // Convert board to string representation
    fn board_to_string(board: [[bool]], n: int) -> [string] {
        let result: [string] = [];
        for i in 0..n {
            let row = "";
            for j in 0..n {
                if board[i][j] {
                    row += "Q";
                } else {
                    row += ".";
                }
            }
            result.push(row);
        }
        return result;
    }
}

// Example usage
fn test_n_queens() {
    let solver = NQueens::new();
    let solutions = solver.solve_n_queens(4);
    
    println("N-Queens solutions for 4x4 board:");
    for solution in solutions {
        for row in solution {
            println(row);
        }
        println("---");
    }
}
```

**Output for 4-Queens:**
```
.Q..
...Q
Q...
..Q.
---
..Q.
Q...
...Q
.Q..
---
```

**Time Complexity**: O(N!)  
**Space Complexity**: O(N²)

---

### Problem 2: Sudoku Solver

Solve a 9×9 Sudoku puzzle.

```klang
class SudokuSolver {
    fn new() -> SudokuSolver {
        return SudokuSolver();
    }
    
    // Solve Sudoku puzzle
    fn solve_sudoku(board: [[int]]) -> bool {
        return this.backtrack(board);
    }
    
    // Backtracking function
    fn backtrack(board: [[int]]) -> bool {
        // Find next empty cell
        let empty = this.find_empty_cell(board);
        
        // Base case: no empty cells, puzzle solved
        if empty.is_none() {
            return true;
        }
        
        let row = empty.row;
        let col = empty.col;
        
        // Try digits 1-9
        for num in 1..=9 {
            if this.is_valid(board, row, col, num) {
                // Make choice
                board[row][col] = num;
                
                // Recurse
                if this.backtrack(board) {
                    return true;  // Solution found
                }
                
                // Backtrack
                board[row][col] = 0;
            }
        }
        
        return false;  // No solution
    }
    
    // Find next empty cell (with 0)
    fn find_empty_cell(board: [[int]]) -> Option<{row: int, col: int}> {
        for i in 0..9 {
            for j in 0..9 {
                if board[i][j] == 0 {
                    return Some({row: i, col: j});
                }
            }
        }
        return None;
    }
    
    // Check if placing num at (row, col) is valid
    fn is_valid(board: [[int]], row: int, col: int, num: int) -> bool {
        // Check row
        for j in 0..9 {
            if board[row][j] == num {
                return false;
            }
        }
        
        // Check column
        for i in 0..9 {
            if board[i][col] == num {
                return false;
            }
        }
        
        // Check 3x3 box
        let box_row = (row / 3) * 3;
        let box_col = (col / 3) * 3;
        
        for i in box_row..(box_row + 3) {
            for j in box_col..(box_col + 3) {
                if board[i][j] == num {
                    return false;
                }
            }
        }
        
        return true;
    }
    
    // Print board
    fn print_board(board: [[int]]) {
        for i in 0..9 {
            if i % 3 == 0 && i != 0 {
                println("- - - - - - - - - - - -");
            }
            
            let row = "";
            for j in 0..9 {
                if j % 3 == 0 && j != 0 {
                    row += " | ";
                }
                
                if board[i][j] == 0 {
                    row += ". ";
                } else {
                    row += str(board[i][j]) + " ";
                }
            }
            println(row);
        }
    }
}

// Example usage
fn test_sudoku() {
    let board = [
        [5, 3, 0, 0, 7, 0, 0, 0, 0],
        [6, 0, 0, 1, 9, 5, 0, 0, 0],
        [0, 9, 8, 0, 0, 0, 0, 6, 0],
        [8, 0, 0, 0, 6, 0, 0, 0, 3],
        [4, 0, 0, 8, 0, 3, 0, 0, 1],
        [7, 0, 0, 0, 2, 0, 0, 0, 6],
        [0, 6, 0, 0, 0, 0, 2, 8, 0],
        [0, 0, 0, 4, 1, 9, 0, 0, 5],
        [0, 0, 0, 0, 8, 0, 0, 7, 9]
    ];
    
    let solver = SudokuSolver::new();
    
    println("Original board:");
    solver.print_board(board);
    
    if solver.solve_sudoku(board) {
        println("\nSolved board:");
        solver.print_board(board);
    } else {
        println("\nNo solution exists");
    }
}
```

**Time Complexity**: O(9^(n²)) where n is number of empty cells  
**Space Complexity**: O(n²) for recursion stack

---

### Problem 3: Generate Parentheses

Generate all combinations of n pairs of valid parentheses.

```klang
class GenerateParentheses {
    fn new() -> GenerateParentheses {
        return GenerateParentheses();
    }
    
    // Generate all valid parentheses combinations
    fn generate_parenthesis(n: int) -> [string] {
        let result: [string] = [];
        this.backtrack("", 0, 0, n, result);
        return result;
    }
    
    // Backtracking function
    fn backtrack(current: string, open: int, close: int, max: int, result: [string]) {
        // Base case: reached maximum length
        if current.length() == max * 2 {
            result.push(current);
            return;
        }
        
        // Add opening parenthesis if we can
        if open < max {
            this.backtrack(current + "(", open + 1, close, max, result);
        }
        
        // Add closing parenthesis if valid
        if close < open {
            this.backtrack(current + ")", open, close + 1, max, result);
        }
    }
}

// Example usage
fn test_generate_parentheses() {
    let gen = GenerateParentheses::new();
    
    let result = gen.generate_parenthesis(3);
    println("Valid parentheses for n=3:");
    for combo in result {
        println(combo);
    }
    // Output: ((())), (()()), (())(), ()(()), ()()()
}
```

**Time Complexity**: O(4^n / √n) - Catalan number  
**Space Complexity**: O(n) - recursion depth

---

### Problem 4: Word Search

Find if a word exists in a 2D board where the word can be constructed from letters of adjacent cells.

```klang
class WordSearch {
    fn new() -> WordSearch {
        return WordSearch();
    }
    
    // Check if word exists in board
    fn exist(board: [[char]], word: string) -> bool {
        let rows = board.length();
        let cols = board[0].length();
        let visited: [[bool]] = [];
        
        // Initialize visited matrix
        for i in 0..rows {
            let row: [bool] = [];
            for j in 0..cols {
                row.push(false);
            }
            visited.push(row);
        }
        
        // Try starting from each cell
        for i in 0..rows {
            for j in 0..cols {
                if this.backtrack(board, word, 0, i, j, visited) {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    // Backtracking function
    fn backtrack(board: [[char]], word: string, index: int, 
                 row: int, col: int, visited: [[bool]]) -> bool {
        // Base case: found entire word
        if index == word.length() {
            return true;
        }
        
        let rows = board.length();
        let cols = board[0].length();
        
        // Check boundaries
        if row < 0 || row >= rows || col < 0 || col >= cols {
            return false;
        }
        
        // Check if already visited or character doesn't match
        if visited[row][col] || board[row][col] != word[index] {
            return false;
        }
        
        // Make choice
        visited[row][col] = true;
        
        // Explore all 4 directions
        let directions = [[0, 1], [1, 0], [0, -1], [-1, 0]];
        
        for dir in directions {
            let new_row = row + dir[0];
            let new_col = col + dir[1];
            
            if this.backtrack(board, word, index + 1, new_row, new_col, visited) {
                return true;
            }
        }
        
        // Backtrack
        visited[row][col] = false;
        
        return false;
    }
}

// Example usage
fn test_word_search() {
    let board = [
        ['A', 'B', 'C', 'E'],
        ['S', 'F', 'C', 'S'],
        ['A', 'D', 'E', 'E']
    ];
    
    let searcher = WordSearch::new();
    
    println("Word 'ABCCED': " + str(searcher.exist(board, "ABCCED")));  // true
    println("Word 'SEE': " + str(searcher.exist(board, "SEE")));        // true
    println("Word 'ABCB': " + str(searcher.exist(board, "ABCB")));      // false
}
```

**Time Complexity**: O(N × M × 4^L) where N×M is board size, L is word length  
**Space Complexity**: O(L) - recursion depth

---

## Memoization

Memoization is an optimization technique that stores the results of expensive function calls and returns the cached result when the same inputs occur again.

### Why Memoization?

Many recursive algorithms have **overlapping subproblems** - they compute the same values multiple times. Memoization eliminates redundant calculations.

### Fibonacci: Before and After

**Without Memoization (Inefficient):**
```klang
fn fib(n: int) -> int {
    if n <= 1 {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}
// Time: O(2^n) - exponential!
// fib(5) calls fib(3) twice, fib(2) three times, etc.
```

**Call tree for fib(5):**
```
                    fib(5)
                   /      \
              fib(4)        fib(3)
             /     \        /     \
        fib(3)   fib(2)  fib(2)  fib(1)
        /    \    /   \   /   \
    fib(2) fib(1) f(1) f(0) f(1) f(0)
    /   \
  f(1) f(0)
```

**With Memoization (Efficient):**
```klang
class FibMemo {
    let memo: Map<int, int>;
    
    fn new() -> FibMemo {
        return FibMemo { memo: Map::new() };
    }
    
    fn fib(n: int) -> int {
        // Check if already computed
        if this.memo.contains(n) {
            return this.memo.get(n);
        }
        
        // Base cases
        if n <= 1 {
            return n;
        }
        
        // Compute and store result
        let result = this.fib(n - 1) + this.fib(n - 2);
        this.memo.insert(n, result);
        
        return result;
    }
}

// Example usage
fn test_fib_memo() {
    let fib_solver = FibMemo::new();
    
    for i in 0..15 {
        println("F(" + str(i) + ") = " + str(fib_solver.fib(i)));
    }
}
// Time: O(n) - each value computed once
// Space: O(n) - memoization table
```

### Time Complexity Improvement

| Approach | Time Complexity | Space Complexity |
|----------|----------------|------------------|
| Naive Recursion | O(2^n) | O(n) |
| Memoization | O(n) | O(n) |
| Iteration | O(n) | O(1) |

### Generic Memoization Pattern

```klang
class Memoizer<T, R> {
    let cache: Map<T, R>;
    
    fn new() -> Memoizer<T, R> {
        return Memoizer { cache: Map::new() };
    }
    
    fn compute(key: T, compute_fn: fn(T) -> R) -> R {
        if this.cache.contains(key) {
            return this.cache.get(key);
        }
        
        let result = compute_fn(key);
        this.cache.insert(key, result);
        return result;
    }
}
```

### More Examples with Memoization

**Climbing Stairs:**
```klang
class ClimbStairs {
    let memo: Map<int, int>;
    
    fn new() -> ClimbStairs {
        return ClimbStairs { memo: Map::new() };
    }
    
    fn climb_stairs(n: int) -> int {
        if n <= 2 {
            return n;
        }
        
        if this.memo.contains(n) {
            return this.memo.get(n);
        }
        
        let result = this.climb_stairs(n - 1) + this.climb_stairs(n - 2);
        this.memo.insert(n, result);
        
        return result;
    }
}
```

**Longest Common Subsequence:**
```klang
class LCS {
    let memo: Map<string, int>;
    
    fn new() -> LCS {
        return LCS { memo: Map::new() };
    }
    
    fn lcs(s1: string, s2: string) -> int {
        let key = s1 + "," + s2;
        
        if this.memo.contains(key) {
            return this.memo.get(key);
        }
        
        if s1.length() == 0 || s2.length() == 0 {
            return 0;
        }
        
        let result: int;
        if s1[0] == s2[0] {
            result = 1 + this.lcs(s1.substring(1), s2.substring(1));
        } else {
            let skip1 = this.lcs(s1.substring(1), s2);
            let skip2 = this.lcs(s1, s2.substring(1));
            result = max(skip1, skip2);
        }
        
        this.memo.insert(key, result);
        return result;
    }
}
```

---

## Tail Recursion

### What is Tail Recursion?

A function is **tail recursive** if the recursive call is the **last operation** performed before returning. No operations are performed on the result of the recursive call.

### Non-Tail Recursive

```klang
fn factorial(n: int) -> int {
    if n <= 1 {
        return 1;
    }
    return n * factorial(n - 1);  // Multiplication after recursive call
}
```

The multiplication happens **after** the recursive call returns, so this is NOT tail recursive.

### Tail Recursive

```klang
fn factorial_tail(n: int, accumulator: int) -> int {
    if n <= 1 {
        return accumulator;
    }
    return factorial_tail(n - 1, n * accumulator);  // Last operation is recursive call
}

fn factorial(n: int) -> int {
    return factorial_tail(n, 1);
}
```

The recursive call is the **last operation**, so this is tail recursive.

### Benefits of Tail Recursion

1. **Tail Call Optimization (TCO)**: Compiler can reuse stack frame
2. **Constant Space**: O(1) space instead of O(n)
3. **No Stack Overflow**: Can handle very deep recursion
4. **Performance**: As fast as iteration

### How TCO Works

Without optimization:
```
factorial(5, 1)
  → factorial(4, 5)
    → factorial(3, 20)
      → factorial(2, 60)
        → factorial(1, 120)
          → return 120
```

With optimization (reuses stack frame):
```
factorial(5, 1)
factorial(4, 5)  // Same frame
factorial(3, 20) // Same frame
factorial(2, 60) // Same frame
factorial(1, 120) // Same frame
→ return 120
```

### More Tail Recursive Examples

**Sum of List:**
```klang
// Non-tail recursive
fn sum(arr: [int], index: int) -> int {
    if index >= arr.length() {
        return 0;
    }
    return arr[index] + sum(arr, index + 1);  // Addition after
}

// Tail recursive
fn sum_tail(arr: [int], index: int, accumulator: int) -> int {
    if index >= arr.length() {
        return accumulator;
    }
    return sum_tail(arr, index + 1, accumulator + arr[index]);  // Last operation
}

fn sum(arr: [int]) -> int {
    return sum_tail(arr, 0, 0);
}
```

**Reverse List:**
```klang
// Tail recursive reverse
fn reverse_tail(arr: [int], index: int, result: [int]) -> [int] {
    if index < 0 {
        return result;
    }
    result.push(arr[index]);
    return reverse_tail(arr, index - 1, result);
}

fn reverse(arr: [int]) -> [int] {
    return reverse_tail(arr, arr.length() - 1, []);
}
```

**GCD (Already tail recursive):**
```klang
fn gcd(a: int, b: int) -> int {
    if b == 0 {
        return a;
    }
    return gcd(b, a % b);  // Tail recursive
}
```

---

## Recursion Tree

A recursion tree helps visualize recursive calls and analyze time complexity.

### Fibonacci Recursion Tree

For `fib(5)`:
```
                        fib(5)                     Level 0: 1 call
                       /      \
                  fib(4)        fib(3)              Level 1: 2 calls
                 /     \        /     \
            fib(3)   fib(2)  fib(2)  fib(1)        Level 2: 4 calls
            /    \    /   \   /   \
        fib(2) fib(1) 1   0  1   0  1              Level 3: 8 calls
        /   \
       1     0                                      Level 4: 16 calls (partial)

Total calls ≈ 2^0 + 2^1 + 2^2 + ... + 2^n = O(2^n)
```

### Calculating Time Complexity from Tree

**Steps:**
1. Count operations at each level
2. Count number of levels (tree height)
3. Multiply or sum depending on problem

**Example: Merge Sort**
```
                    [8,3,1,7,0,10,2]               Level 0: n work
                   /               \
         [8,3,1,7]                 [0,10,2]         Level 1: n work total
         /       \                 /      \
    [8,3]       [1,7]          [0,10]    [2]        Level 2: n work total
    /  \        /  \           /  \
  [8]  [3]    [1]  [7]       [0]  [10]              Level 3: n work total

Height = log(n) levels
Work per level = n
Total = O(n log n)
```

### Recursion Tree for Power Function

**Naive: O(n)**
```
power(2, 8)
  → 2 * power(2, 7)
    → 2 * power(2, 6)
      → ... (8 calls)
```

**Optimized: O(log n)**
```
power(2, 8)
  → power(2, 4) * power(2, 4)
    → [power(2, 2) * power(2, 2)] * [...]
      → [[power(2, 1) * ...] * ...] * [...]

Height = log(8) = 3
Total calls = 3
```

---

## Interview Problems

### Problem 1: Subsets

Generate all subsets of a set.

```klang
class Subsets {
    fn new() -> Subsets {
        return Subsets();
    }
    
    fn subsets(nums: [int]) -> [[int]] {
        let result: [[int]] = [];
        let current: [int] = [];
        this.backtrack(nums, 0, current, result);
        return result;
    }
    
    fn backtrack(nums: [int], start: int, current: [int], result: [[int]]) {
        // Add current subset
        result.push(current.clone());
        
        // Try adding each remaining element
        for i in start..nums.length() {
            current.push(nums[i]);
            this.backtrack(nums, i + 1, current, result);
            current.pop();  // Backtrack
        }
    }
}

// Example: [1,2,3] → [[], [1], [2], [3], [1,2], [1,3], [2,3], [1,2,3]]
```

**Time Complexity**: O(2^n × n)  
**Space Complexity**: O(n)

---

### Problem 2: Permutations

Generate all permutations of an array.

```klang
class Permutations {
    fn new() -> Permutations {
        return Permutations();
    }
    
    fn permute(nums: [int]) -> [[int]] {
        let result: [[int]] = [];
        let current: [int] = [];
        let used: [bool] = [];
        
        for _ in nums {
            used.push(false);
        }
        
        this.backtrack(nums, current, used, result);
        return result;
    }
    
    fn backtrack(nums: [int], current: [int], used: [bool], result: [[int]]) {
        // Base case: permutation complete
        if current.length() == nums.length() {
            result.push(current.clone());
            return;
        }
        
        // Try each unused number
        for i in 0..nums.length() {
            if !used[i] {
                // Choose
                current.push(nums[i]);
                used[i] = true;
                
                // Explore
                this.backtrack(nums, current, used, result);
                
                // Unchoose
                current.pop();
                used[i] = false;
            }
        }
    }
}

// Example: [1,2,3] → [[1,2,3], [1,3,2], [2,1,3], [2,3,1], [3,1,2], [3,2,1]]
```

**Time Complexity**: O(n! × n)  
**Space Complexity**: O(n)

---

### Problem 3: Combinations

Generate all combinations of k numbers from 1 to n.

```klang
class Combinations {
    fn new() -> Combinations {
        return Combinations();
    }
    
    fn combine(n: int, k: int) -> [[int]] {
        let result: [[int]] = [];
        let current: [int] = [];
        this.backtrack(n, k, 1, current, result);
        return result;
    }
    
    fn backtrack(n: int, k: int, start: int, current: [int], result: [[int]]) {
        // Base case: combination complete
        if current.length() == k {
            result.push(current.clone());
            return;
        }
        
        // Try each number from start to n
        for i in start..=n {
            current.push(i);
            this.backtrack(n, k, i + 1, current, result);
            current.pop();  // Backtrack
        }
    }
}

// Example: combine(4, 2) → [[1,2], [1,3], [1,4], [2,3], [2,4], [3,4]]
```

**Time Complexity**: O(C(n,k) × k) = O(n! / ((n-k)! × k!))  
**Space Complexity**: O(k)

---

### Problem 4: Letter Combinations of Phone Number

Given a string containing digits 2-9, return all possible letter combinations.

```klang
class LetterCombinations {
    let digit_map: Map<char, string>;
    
    fn new() -> LetterCombinations {
        let map = Map::new();
        map.insert('2', "abc");
        map.insert('3', "def");
        map.insert('4', "ghi");
        map.insert('5', "jkl");
        map.insert('6', "mno");
        map.insert('7', "pqrs");
        map.insert('8', "tuv");
        map.insert('9', "wxyz");
        
        return LetterCombinations { digit_map: map };
    }
    
    fn letter_combinations(digits: string) -> [string] {
        if digits.length() == 0 {
            return [];
        }
        
        let result: [string] = [];
        this.backtrack(digits, 0, "", result);
        return result;
    }
    
    fn backtrack(digits: string, index: int, current: string, result: [string]) {
        // Base case: processed all digits
        if index == digits.length() {
            result.push(current);
            return;
        }
        
        // Get letters for current digit
        let digit = digits[index];
        let letters = this.digit_map.get(digit);
        
        // Try each letter
        for letter in letters {
            this.backtrack(digits, index + 1, current + letter, result);
        }
    }
}

// Example: "23" → ["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"]
```

**Time Complexity**: O(4^n × n) where n is length of digits  
**Space Complexity**: O(n)

---

### Problem 5: Palindrome Partitioning

Partition string into all possible palindrome substrings.

```klang
class PalindromePartition {
    fn new() -> PalindromePartition {
        return PalindromePartition();
    }
    
    fn partition(s: string) -> [[string]] {
        let result: [[string]] = [];
        let current: [string] = [];
        this.backtrack(s, 0, current, result);
        return result;
    }
    
    fn backtrack(s: string, start: int, current: [string], result: [[string]]) {
        // Base case: processed entire string
        if start == s.length() {
            result.push(current.clone());
            return;
        }
        
        // Try all possible substrings starting from start
        for end in start..s.length() {
            let substring = s.substring(start, end + 1);
            
            if this.is_palindrome(substring) {
                current.push(substring);
                this.backtrack(s, end + 1, current, result);
                current.pop();  // Backtrack
            }
        }
    }
    
    fn is_palindrome(s: string) -> bool {
        let left = 0;
        let right = s.length() - 1;
        
        while left < right {
            if s[left] != s[right] {
                return false;
            }
            left += 1;
            right -= 1;
        }
        
        return true;
    }
}

// Example: "aab" → [["a","a","b"], ["aa","b"]]
```

**Time Complexity**: O(n × 2^n)  
**Space Complexity**: O(n)

---

### Problem 6: Combination Sum

Find all combinations that sum to target (elements can be reused).

```klang
class CombinationSum {
    fn new() -> CombinationSum {
        return CombinationSum();
    }
    
    fn combination_sum(candidates: [int], target: int) -> [[int]] {
        let result: [[int]] = [];
        let current: [int] = [];
        this.backtrack(candidates, target, 0, 0, current, result);
        return result;
    }
    
    fn backtrack(candidates: [int], target: int, start: int, 
                 current_sum: int, current: [int], result: [[int]]) {
        // Base case: found target sum
        if current_sum == target {
            result.push(current.clone());
            return;
        }
        
        // Base case: exceeded target
        if current_sum > target {
            return;
        }
        
        // Try each candidate
        for i in start..candidates.length() {
            current.push(candidates[i]);
            // Can reuse same element, so pass i (not i+1)
            this.backtrack(candidates, target, i, 
                          current_sum + candidates[i], current, result);
            current.pop();  // Backtrack
        }
    }
}

// Example: candidates=[2,3,6,7], target=7 → [[2,2,3], [7]]
```

**Time Complexity**: O(n^(target/min))  
**Space Complexity**: O(target/min)

---

### Problem 7: Path Sum II

Find all root-to-leaf paths with given sum in binary tree.

```klang
class TreeNode {
    let val: int;
    let left: Option<TreeNode>;
    let right: Option<TreeNode>;
    
    fn new(val: int) -> TreeNode {
        return TreeNode {
            val: val,
            left: None,
            right: None
        };
    }
}

class PathSum {
    fn new() -> PathSum {
        return PathSum();
    }
    
    fn path_sum(root: Option<TreeNode>, target_sum: int) -> [[int]] {
        let result: [[int]] = [];
        let current: [int] = [];
        this.backtrack(root, target_sum, 0, current, result);
        return result;
    }
    
    fn backtrack(node: Option<TreeNode>, target: int, 
                 current_sum: int, current: [int], result: [[int]]) {
        if node.is_none() {
            return;
        }
        
        let node_val = node.unwrap().val;
        current.push(node_val);
        current_sum += node_val;
        
        // Check if leaf node with target sum
        if node.left.is_none() && node.right.is_none() {
            if current_sum == target {
                result.push(current.clone());
            }
        } else {
            // Explore left and right subtrees
            this.backtrack(node.left, target, current_sum, current, result);
            this.backtrack(node.right, target, current_sum, current, result);
        }
        
        current.pop();  // Backtrack
    }
}
```

**Time Complexity**: O(n²) - visit all nodes, copy paths  
**Space Complexity**: O(h) - tree height

---

### Problem 8: Binary Tree Paths

Return all root-to-leaf paths in binary tree.

```klang
class BinaryTreePaths {
    fn new() -> BinaryTreePaths {
        return BinaryTreePaths();
    }
    
    fn binary_tree_paths(root: Option<TreeNode>) -> [string] {
        let result: [string] = [];
        
        if root.is_none() {
            return result;
        }
        
        this.find_paths(root, "", result);
        return result;
    }
    
    fn find_paths(node: Option<TreeNode>, path: string, result: [string]) {
        if node.is_none() {
            return;
        }
        
        let current_path = path;
        if path.length() > 0 {
            current_path += "->";
        }
        current_path += str(node.unwrap().val);
        
        // Leaf node - add path
        if node.left.is_none() && node.right.is_none() {
            result.push(current_path);
            return;
        }
        
        // Recurse on children
        this.find_paths(node.left, current_path, result);
        this.find_paths(node.right, current_path, result);
    }
}
```

---

### Problem 9: Restore IP Addresses

Generate all valid IP addresses from a string of digits.

```klang
class RestoreIP {
    fn new() -> RestoreIP {
        return RestoreIP();
    }
    
    fn restore_ip_addresses(s: string) -> [string] {
        let result: [string] = [];
        let current: [string] = [];
        this.backtrack(s, 0, current, result);
        return result;
    }
    
    fn backtrack(s: string, start: int, current: [string], result: [string]) {
        // Base case: 4 parts and processed entire string
        if current.length() == 4 {
            if start == s.length() {
                result.push(current.join("."));
            }
            return;
        }
        
        // Try segments of length 1, 2, 3
        for len in 1..=3 {
            if start + len > s.length() {
                break;
            }
            
            let segment = s.substring(start, start + len);
            
            if this.is_valid_segment(segment) {
                current.push(segment);
                this.backtrack(s, start + len, current, result);
                current.pop();  // Backtrack
            }
        }
    }
    
    fn is_valid_segment(s: string) -> bool {
        // No leading zeros (except "0" itself)
        if s.length() > 1 && s[0] == '0' {
            return false;
        }
        
        let num = parse_int(s);
        return num >= 0 && num <= 255;
    }
}

// Example: "25525511135" → ["255.255.11.135", "255.255.111.35"]
```

---

### Problem 10: Tower of Hanoi

Classic recursive puzzle: Move n disks from source to destination using auxiliary peg.

```klang
class TowerOfHanoi {
    fn new() -> TowerOfHanoi {
        return TowerOfHanoi();
    }
    
    fn solve(n: int, source: string, destination: string, auxiliary: string) {
        if n == 1 {
            println("Move disk 1 from " + source + " to " + destination);
            return;
        }
        
        // Move n-1 disks from source to auxiliary
        this.solve(n - 1, source, auxiliary, destination);
        
        // Move largest disk from source to destination
        println("Move disk " + str(n) + " from " + source + " to " + destination);
        
        // Move n-1 disks from auxiliary to destination
        this.solve(n - 1, auxiliary, destination, source);
    }
}

// Example usage
fn test_tower_of_hanoi() {
    let tower = TowerOfHanoi::new();
    tower.solve(3, "A", "C", "B");
}
```

**Output:**
```
Move disk 1 from A to C
Move disk 2 from A to B
Move disk 1 from C to B
Move disk 3 from A to C
Move disk 1 from B to A
Move disk 2 from B to C
Move disk 1 from A to C
```

**Time Complexity**: O(2^n)  
**Space Complexity**: O(n)

---

### Problem 11: Gray Code

Generate n-bit Gray code sequence.

```klang
class GrayCode {
    fn new() -> GrayCode {
        return GrayCode();
    }
    
    fn gray_code(n: int) -> [int] {
        if n == 0 {
            return [0];
        }
        
        // Get (n-1)-bit Gray code
        let prev = this.gray_code(n - 1);
        let result: [int] = [];
        
        // First half: add 0 prefix (same as prev)
        for num in prev {
            result.push(num);
        }
        
        // Second half: add 1 prefix (reverse order)
        let prefix = 1 << (n - 1);
        for i in (0..prev.length()).rev() {
            result.push(prefix | prev[i]);
        }
        
        return result;
    }
}

// Example: n=2 → [0, 1, 3, 2] (binary: 00, 01, 11, 10)
```

---

### Problem 12: Decode Ways

Count ways to decode a string of digits (A=1, B=2, ..., Z=26).

```klang
class DecodeWays {
    let memo: Map<int, int>;
    
    fn new() -> DecodeWays {
        return DecodeWays { memo: Map::new() };
    }
    
    fn num_decodings(s: string) -> int {
        if s.length() == 0 || s[0] == '0' {
            return 0;
        }
        
        return this.decode_helper(s, 0);
    }
    
    fn decode_helper(s: string, index: int) -> int {
        // Base case: reached end
        if index == s.length() {
            return 1;
        }
        
        // Check memo
        if this.memo.contains(index) {
            return this.memo.get(index);
        }
        
        // Can't start with 0
        if s[index] == '0' {
            return 0;
        }
        
        // Decode single digit
        let ways = this.decode_helper(s, index + 1);
        
        // Decode two digits if valid
        if index + 1 < s.length() {
            let two_digit = parse_int(s.substring(index, index + 2));
            if two_digit >= 10 && two_digit <= 26 {
                ways += this.decode_helper(s, index + 2);
            }
        }
        
        this.memo.insert(index, ways);
        return ways;
    }
}

// Example: "226" → 3 (BZ, VF, BBF)
```

---

## Best Practices

### 1. Always Have a Base Case

**Bad:**
```klang
fn factorial(n: int) -> int {
    return n * factorial(n - 1);  // Infinite recursion!
}
```

**Good:**
```klang
fn factorial(n: int) -> int {
    if n <= 1 {
        return 1;  // Base case
    }
    return n * factorial(n - 1);
}
```

### 2. Make Progress Toward Base Case

Ensure each recursive call brings you closer to the base case.

**Bad:**
```klang
fn sum(n: int) -> int {
    if n == 0 {
        return 0;
    }
    return n + sum(n);  // No progress!
}
```

**Good:**
```klang
fn sum(n: int) -> int {
    if n == 0 {
        return 0;
    }
    return n + sum(n - 1);  // Progress: n decreases
}
```

### 3. Use Memoization for Overlapping Subproblems

If your recursion recalculates the same values, use memoization.

```klang
// Without memoization: O(2^n)
fn fib(n: int) -> int {
    if n <= 1 { return n; }
    return fib(n-1) + fib(n-2);
}

// With memoization: O(n)
class FibMemo {
    let memo: Map<int, int>;
    fn fib(n: int) -> int {
        if this.memo.contains(n) { return this.memo.get(n); }
        if n <= 1 { return n; }
        let result = this.fib(n-1) + this.fib(n-2);
        this.memo.insert(n, result);
        return result;
    }
}
```

### 4. Consider Iterative Alternative

For simple recursions, iteration may be better for performance and memory.

```klang
// Recursive: O(n) space
fn sum_recursive(n: int) -> int {
    if n == 0 { return 0; }
    return n + sum_recursive(n - 1);
}

// Iterative: O(1) space
fn sum_iterative(n: int) -> int {
    let total = 0;
    for i in 1..=n {
        total += i;
    }
    return total;
}
```

### 5. Use Helper Functions

Keep public API clean with a helper function for recursion.

```klang
// Public API
fn reverse_list(arr: [int]) -> [int] {
    return reverse_helper(arr, 0, arr.length() - 1);
}

// Private recursive helper
fn reverse_helper(arr: [int], left: int, right: int) -> [int] {
    if left >= right {
        return arr;
    }
    swap(arr, left, right);
    return reverse_helper(arr, left + 1, right - 1);
}
```

### 6. Document Recursive Functions

Clearly document base case, recursive case, and assumptions.

```klang
/**
 * Calculate factorial of n
 * 
 * Base case: n <= 1 returns 1
 * Recursive case: n * factorial(n-1)
 * 
 * @param n - non-negative integer
 * @return n!
 */
fn factorial(n: int) -> int {
    if n <= 1 {
        return 1;
    }
    return n * factorial(n - 1);
}
```

### 7. Validate Input

Check for invalid inputs before recursing.

```klang
fn factorial(n: int) -> int {
    if n < 0 {
        throw Error("Factorial undefined for negative numbers");
    }
    
    if n <= 1 {
        return 1;
    }
    
    return n * factorial(n - 1);
}
```

### 8. Consider Tail Recursion

Use tail recursion when possible for optimization.

```klang
// Not tail recursive
fn sum(n: int) -> int {
    if n == 0 { return 0; }
    return n + sum(n - 1);  // Addition after return
}

// Tail recursive
fn sum_tail(n: int, acc: int) -> int {
    if n == 0 { return acc; }
    return sum_tail(n - 1, acc + n);  // Last operation
}
```

---

## Common Mistakes

### 1. Missing Base Case

**Problem:**
```klang
fn countdown(n: int) {
    println(n);
    countdown(n - 1);  // No base case - infinite recursion!
}
```

**Solution:**
```klang
fn countdown(n: int) {
    if n <= 0 {
        return;  // Base case
    }
    println(n);
    countdown(n - 1);
}
```

### 2. Stack Overflow

**Problem:**
```klang
fn sum(n: int) -> int {
    if n == 0 { return 0; }
    return n + sum(n - 1);
}

sum(1000000);  // Stack overflow!
```

**Solution:**
```klang
// Use iteration for deep recursion
fn sum(n: int) -> int {
    let total = 0;
    for i in 1..=n {
        total += i;
    }
    return total;
}

// Or use tail recursion with TCO
fn sum_tail(n: int, acc: int) -> int {
    if n == 0 { return acc; }
    return sum_tail(n - 1, acc + n);
}
```

### 3. Not Making Progress

**Problem:**
```klang
fn bad_recursion(n: int) -> int {
    if n == 0 { return 0; }
    return bad_recursion(n);  // n doesn't change!
}
```

**Solution:**
```klang
fn good_recursion(n: int) -> int {
    if n == 0 { return 0; }
    return n + good_recursion(n - 1);  // n decreases
}
```

### 4. Inefficient Recursion Without Memoization

**Problem:**
```klang
// O(2^n) - very slow
fn fib(n: int) -> int {
    if n <= 1 { return n; }
    return fib(n - 1) + fib(n - 2);
}

fib(40);  // Takes forever!
```

**Solution:**
```klang
class FibMemo {
    let memo: Map<int, int>;
    
    fn fib(n: int) -> int {
        if this.memo.contains(n) {
            return this.memo.get(n);
        }
        
        if n <= 1 { return n; }
        
        let result = this.fib(n - 1) + this.fib(n - 2);
        this.memo.insert(n, result);
        return result;
    }
}
```

### 5. Modifying Input Incorrectly

**Problem:**
```klang
fn process_array(arr: [int]) -> [int] {
    if arr.length() == 0 {
        return arr;
    }
    
    arr.remove(0);  // Modifies original array!
    return process_array(arr);
}
```

**Solution:**
```klang
fn process_array(arr: [int], index: int) -> [int] {
    if index >= arr.length() {
        return [];
    }
    
    let result = process_array(arr, index + 1);
    result.insert(0, arr[index]);
    return result;
}
```

### 6. Wrong Base Case Condition

**Problem:**
```klang
fn factorial(n: int) -> int {
    if n == 1 {  // What about n == 0?
        return 1;
    }
    return n * factorial(n - 1);
}

factorial(0);  // Error or wrong result
```

**Solution:**
```klang
fn factorial(n: int) -> int {
    if n <= 1 {  // Handles 0 and 1
        return 1;
    }
    return n * factorial(n - 1);
}
```

### 7. Not Handling Edge Cases

**Problem:**
```klang
fn find_max(arr: [int], index: int) -> int {
    if index == arr.length() - 1 {
        return arr[index];
    }
    return max(arr[index], find_max(arr, index + 1));
}

find_max([], 0);  // Crashes!
```

**Solution:**
```klang
fn find_max(arr: [int], index: int) -> int {
    // Handle empty array
    if arr.length() == 0 {
        throw Error("Cannot find max of empty array");
    }
    
    if index == arr.length() - 1 {
        return arr[index];
    }
    
    return max(arr[index], find_max(arr, index + 1));
}
```

### 8. Forgetting to Return Result

**Problem:**
```klang
fn factorial(n: int) -> int {
    if n <= 1 {
        return 1;
    }
    n * factorial(n - 1);  // Missing return!
}
```

**Solution:**
```klang
fn factorial(n: int) -> int {
    if n <= 1 {
        return 1;
    }
    return n * factorial(n - 1);  // Return the result
}
```

---

## Summary

### Key Takeaways

1. **Recursion** = Function calling itself with simpler inputs
2. **Base case** is mandatory to prevent infinite recursion
3. **Recursive case** must make progress toward base case
4. **Use memoization** for overlapping subproblems
5. **Backtracking** explores solution spaces systematically
6. **Tail recursion** can be optimized by compiler
7. **Consider iteration** for simple cases or deep recursion

### When to Use Recursion

✅ Tree/graph traversal  
✅ Backtracking problems  
✅ Divide and conquer algorithms  
✅ Mathematical recurrence relations  
✅ Problems with recursive structure  

### When NOT to Use Recursion

❌ Simple iteration suffices  
❌ Very deep recursion (stack overflow risk)  
❌ Performance critical without tail call optimization  
❌ Memory constrained environments  

### Complexity Patterns

| Pattern | Time | Space | Example |
|---------|------|-------|---------|
| Linear | O(n) | O(n) | Factorial, Sum |
| Binary Tree | O(2^n) | O(n) | Fibonacci (naive) |
| Divide & Conquer | O(n log n) | O(log n) | Merge Sort |
| Memoized | O(n) | O(n) | Fibonacci (memo) |
| Backtracking | O(k^n) | O(n) | N-Queens, Subsets |

---

**Happy Recursing in KLang! 🔄**
