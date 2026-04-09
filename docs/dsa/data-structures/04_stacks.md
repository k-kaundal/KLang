# Stacks in KLang - Data Structures & Algorithms

## Table of Contents
1. [Introduction](#introduction)
2. [What is a Stack?](#what-is-a-stack)
3. [Stack Operations](#stack-operations)
4. [Implementation Methods](#implementation-methods)
5. [Time and Space Complexity](#time-and-space-complexity)
6. [Stack Algorithms](#stack-algorithms)
7. [Applications](#applications)
8. [Interview Problems](#interview-problems)
9. [Best Practices](#best-practices)
10. [Complete Examples](#complete-examples)

---

## Introduction

A **stack** is a linear data structure that follows the **Last In First Out (LIFO)** principle. The last element added to the stack is the first one to be removed, similar to a stack of plates where you add and remove from the top.

### Why Use Stacks?

- **LIFO Order**: Perfect for scenarios requiring reverse order processing
- **Efficient**: O(1) push and pop operations
- **Memory Management**: Used in function call management
- **Natural Fit**: Many problems naturally follow LIFO pattern
- **Backtracking**: Essential for undo/redo and navigation

---

## What is a Stack?

A **stack** is a collection of elements with two principal operations:
- **Push**: Add an element to the top
- **Pop**: Remove the element from the top

### LIFO Principle

**Last In, First Out (LIFO)** means the most recently added element is removed first.

### Visual Representation

```
Push operations:           Pop operations:
    
    |   |                     |   |
    | 3 |  <- top              |   |
    | 2 |                      | 2 |  <- top
    | 1 |                      | 1 |
    +---+                      +---+
    
After push(3)              After pop() -> returns 3
```

### Key Terminology

- **Top**: The end where elements are added/removed
- **Bottom**: The first element added (opposite end)
- **Underflow**: Attempting to pop from empty stack
- **Overflow**: Stack exceeds capacity (in fixed-size implementations)

---

## Stack Operations

### Core Operations

#### 1. Push - Add Element

Add an element to the top of the stack.

```klang
fn push(stack, element) {
    stack.push(element)
}
```

**Time Complexity**: O(1)  
**Space Complexity**: O(1)

#### 2. Pop - Remove Element

Remove and return the top element.

```klang
fn pop(stack) {
    if (stack.length() == 0) {
        print("Stack underflow!")
        return null
    }
    return stack.pop()
}
```

**Time Complexity**: O(1)  
**Space Complexity**: O(1)

#### 3. Peek/Top - View Top Element

Return the top element without removing it.

```klang
fn peek(stack) {
    if (stack.length() == 0) {
        return null
    }
    return stack[stack.length() - 1]
}
```

**Time Complexity**: O(1)  
**Space Complexity**: O(1)

#### 4. IsEmpty - Check if Empty

Check whether the stack is empty.

```klang
fn isEmpty(stack) {
    return stack.length() == 0
}
```

**Time Complexity**: O(1)  
**Space Complexity**: O(1)

#### 5. Size - Get Stack Size

Return the number of elements in the stack.

```klang
fn size(stack) {
    return stack.length()
}
```

**Time Complexity**: O(1)  
**Space Complexity**: O(1)

#### 6. Search - Find Element

Search for an element in the stack.

```klang
fn search(stack, target) {
    let i = stack.length() - 1
    while (i >= 0) {
        if (stack[i] == target) {
            return stack.length() - i  # Distance from top
        }
        i = i - 1
    }
    return -1  # Not found
}
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1)

---

## Implementation Methods

### 1. Array-Based Stack

Uses a dynamic array to store elements.

```klang
# Stack using array
class ArrayStack {
    constructor() {
        this.items = []
    }
    
    fn push(element) {
        this.items.push(element)
    }
    
    fn pop() {
        if (this.isEmpty()) {
            print("Stack underflow!")
            return null
        }
        return this.items.pop()
    }
    
    fn peek() {
        if (this.isEmpty()) {
            return null
        }
        return this.items[this.items.length() - 1]
    }
    
    fn isEmpty() {
        return this.items.length() == 0
    }
    
    fn size() {
        return this.items.length()
    }
    
    fn display() {
        if (this.isEmpty()) {
            print("Stack is empty")
            return
        }
        print("Stack (top to bottom):")
        let i = this.items.length() - 1
        while (i >= 0) {
            print("  " + this.items[i])
            i = i - 1
        }
    }
}

# Usage
let stack = ArrayStack()
stack.push(10)
stack.push(20)
stack.push(30)
stack.display()
print("Popped: " + stack.pop())
print("Top: " + stack.peek())
```

**Advantages**:
- Simple implementation
- O(1) access to any element by index
- Cache-friendly (contiguous memory)

**Disadvantages**:
- May require resizing (array growth)
- Wasted space if stack size varies

### 2. Linked List-Based Stack

Uses a linked list to store elements.

```klang
# Node class
class Node {
    constructor(data) {
        this.data = data
        this.next = null
    }
}

# Stack using linked list
class LinkedStack {
    constructor() {
        this.top = null
        this.count = 0
    }
    
    fn push(element) {
        let newNode = Node(element)
        newNode.next = this.top
        this.top = newNode
        this.count = this.count + 1
    }
    
    fn pop() {
        if (this.isEmpty()) {
            print("Stack underflow!")
            return null
        }
        let data = this.top.data
        this.top = this.top.next
        this.count = this.count - 1
        return data
    }
    
    fn peek() {
        if (this.isEmpty()) {
            return null
        }
        return this.top.data
    }
    
    fn isEmpty() {
        return this.top == null
    }
    
    fn size() {
        return this.count
    }
    
    fn display() {
        if (this.isEmpty()) {
            print("Stack is empty")
            return
        }
        print("Stack (top to bottom):")
        let current = this.top
        while (current != null) {
            print("  " + current.data)
            current = current.next
        }
    }
}

# Usage
let stack = LinkedStack()
stack.push(10)
stack.push(20)
stack.push(30)
stack.display()
```

**Advantages**:
- No resizing needed
- Memory allocated as needed
- No wasted space

**Disadvantages**:
- Extra memory for pointers
- No random access
- Less cache-friendly

---

## Time and Space Complexity

### Operation Complexities

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Push      | O(1)           | O(1)             |
| Pop       | O(1)           | O(1)             |
| Peek      | O(1)           | O(1)             |
| IsEmpty   | O(1)           | O(1)             |
| Size      | O(1)           | O(1)             |
| Search    | O(n)           | O(1)             |

### Space Complexity

- **Array-based**: O(n) where n is the number of elements
- **Linked list-based**: O(n) where n is the number of elements plus pointer overhead

---

## Stack Algorithms

### 1. Balanced Parentheses

Check if parentheses are balanced in an expression.

```klang
fn isBalanced(expression) {
    let stack = []
    let i = 0
    
    while (i < expression.length()) {
        let char = expression[i]
        
        # Push opening brackets
        if (char == '(' || char == '[' || char == '{') {
            stack.push(char)
        }
        # Check closing brackets
        else if (char == ')' || char == ']' || char == '}') {
            if (stack.length() == 0) {
                return false
            }
            
            let top = stack.pop()
            
            if (char == ')' && top != '(') {
                return false
            }
            if (char == ']' && top != '[') {
                return false
            }
            if (char == '}' && top != '{') {
                return false
            }
        }
        
        i = i + 1
    }
    
    return stack.length() == 0
}

# Test
print(isBalanced("({[]})"))      # true
print(isBalanced("({[})"))       # false
print(isBalanced("((()))"))      # true
print(isBalanced("(()"))         # false
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

### 2. Infix to Postfix Conversion

Convert infix expression to postfix (Reverse Polish Notation).

```klang
fn getPrecedence(op) {
    if (op == '+' || op == '-') {
        return 1
    }
    if (op == '*' || op == '/') {
        return 2
    }
    if (op == '^') {
        return 3
    }
    return 0
}

fn isOperator(char) {
    return char == '+' || char == '-' || char == '*' || char == '/' || char == '^'
}

fn infixToPostfix(infix) {
    let stack = []
    let result = ""
    let i = 0
    
    while (i < infix.length()) {
        let char = infix[i]
        
        # If operand, add to result
        if ((char >= 'a' && char <= 'z') || (char >= 'A' && char <= 'Z') || 
            (char >= '0' && char <= '9')) {
            result = result + char
        }
        # If '(', push to stack
        else if (char == '(') {
            stack.push(char)
        }
        # If ')', pop until '('
        else if (char == ')') {
            while (stack.length() > 0 && stack[stack.length() - 1] != '(') {
                result = result + stack.pop()
            }
            if (stack.length() > 0) {
                stack.pop()  # Remove '('
            }
        }
        # If operator
        else if (isOperator(char)) {
            while (stack.length() > 0 && 
                   getPrecedence(stack[stack.length() - 1]) >= getPrecedence(char)) {
                result = result + stack.pop()
            }
            stack.push(char)
        }
        
        i = i + 1
    }
    
    # Pop remaining operators
    while (stack.length() > 0) {
        result = result + stack.pop()
    }
    
    return result
}

# Test
print(infixToPostfix("a+b*c"))       # abc*+
print(infixToPostfix("(a+b)*c"))     # ab+c*
print(infixToPostfix("a+b*c-d"))     # abc*+d-
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

### 3. Evaluate Postfix Expression

Evaluate a postfix (RPN) expression.

```klang
fn evaluatePostfix(expression) {
    let stack = []
    let i = 0
    
    while (i < expression.length()) {
        let char = expression[i]
        
        # If digit, push to stack
        if (char >= '0' && char <= '9') {
            stack.push(char - '0')
        }
        # If operator, pop two operands and evaluate
        else if (char == '+' || char == '-' || char == '*' || char == '/') {
            if (stack.length() < 2) {
                print("Invalid expression")
                return null
            }
            
            let operand2 = stack.pop()
            let operand1 = stack.pop()
            let result = 0
            
            if (char == '+') {
                result = operand1 + operand2
            } else if (char == '-') {
                result = operand1 - operand2
            } else if (char == '*') {
                result = operand1 * operand2
            } else if (char == '/') {
                result = operand1 / operand2
            }
            
            stack.push(result)
        }
        
        i = i + 1
    }
    
    if (stack.length() == 1) {
        return stack.pop()
    }
    
    print("Invalid expression")
    return null
}

# Test
print(evaluatePostfix("23*5+"))    # 11 ((2*3)+5)
print(evaluatePostfix("53+2*"))    # 16 ((5+3)*2)
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

### 4. Next Greater Element

Find the next greater element for each element in an array.

```klang
fn nextGreaterElement(arr) {
    let result = []
    let stack = []
    let n = arr.length()
    
    # Initialize result with -1
    let i = 0
    while (i < n) {
        result.push(-1)
        i = i + 1
    }
    
    # Traverse array from right to left
    i = n - 1
    while (i >= 0) {
        # Pop elements smaller than current
        while (stack.length() > 0 && stack[stack.length() - 1] <= arr[i]) {
            stack.pop()
        }
        
        # If stack not empty, top is next greater
        if (stack.length() > 0) {
            result[i] = stack[stack.length() - 1]
        }
        
        # Push current element
        stack.push(arr[i])
        i = i - 1
    }
    
    return result
}

# Test
let arr = [4, 5, 2, 10, 8]
let result = nextGreaterElement(arr)
print("Array: " + arr)
print("Next Greater: " + result)  # [5, 10, 10, -1, -1]
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

### 5. Stock Span Problem

Calculate span of stock prices (days until previous higher price).

```klang
fn calculateSpan(prices) {
    let span = []
    let stack = []  # Store indices
    let n = prices.length()
    
    let i = 0
    while (i < n) {
        # Pop elements with price <= current price
        while (stack.length() > 0 && prices[stack[stack.length() - 1]] <= prices[i]) {
            stack.pop()
        }
        
        # Calculate span
        if (stack.length() == 0) {
            span.push(i + 1)  # All previous days
        } else {
            span.push(i - stack[stack.length() - 1])
        }
        
        # Push current index
        stack.push(i)
        i = i + 1
    }
    
    return span
}

# Test
let prices = [100, 80, 60, 70, 60, 75, 85]
let spans = calculateSpan(prices)
print("Prices: " + prices)
print("Spans: " + spans)  # [1, 1, 1, 2, 1, 4, 6]
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

---

## Applications

### 1. Function Call Stack

The runtime uses a stack to manage function calls.

```klang
fn factorial(n) {
    if (n <= 1) {
        return 1
    }
    return n * factorial(n - 1)
}

# Call stack visualization:
# factorial(3)
#   -> factorial(2)
#      -> factorial(1)
#         <- returns 1
#      <- returns 2
#   <- returns 6
```

### 2. Undo/Redo Functionality

```klang
class TextEditor {
    constructor() {
        this.text = ""
        this.undoStack = []
        this.redoStack = []
    }
    
    fn write(text) {
        this.undoStack.push(this.text)
        this.text = this.text + text
        this.redoStack = []  # Clear redo on new action
    }
    
    fn undo() {
        if (this.undoStack.length() > 0) {
            this.redoStack.push(this.text)
            this.text = this.undoStack.pop()
        }
    }
    
    fn redo() {
        if (this.redoStack.length() > 0) {
            this.undoStack.push(this.text)
            this.text = this.redoStack.pop()
        }
    }
    
    fn display() {
        print("Text: " + this.text)
    }
}

# Usage
let editor = TextEditor()
editor.write("Hello")
editor.write(" World")
editor.display()  # Text: Hello World
editor.undo()
editor.display()  # Text: Hello
editor.redo()
editor.display()  # Text: Hello World
```

### 3. Browser History

```klang
class BrowserHistory {
    constructor(homepage) {
        this.currentPage = homepage
        this.backStack = []
        this.forwardStack = []
    }
    
    fn visit(url) {
        this.backStack.push(this.currentPage)
        this.currentPage = url
        this.forwardStack = []  # Clear forward history
    }
    
    fn back() {
        if (this.backStack.length() > 0) {
            this.forwardStack.push(this.currentPage)
            this.currentPage = this.backStack.pop()
        }
        return this.currentPage
    }
    
    fn forward() {
        if (this.forwardStack.length() > 0) {
            this.backStack.push(this.currentPage)
            this.currentPage = this.forwardStack.pop()
        }
        return this.currentPage
    }
}

# Usage
let browser = BrowserHistory("google.com")
browser.visit("youtube.com")
browser.visit("github.com")
print(browser.back())      # youtube.com
print(browser.back())      # google.com
print(browser.forward())   # youtube.com
```

### 4. Expression Parsing

```klang
# Evaluate simple calculator expression
fn calculate(s) {
    let stack = []
    let num = 0
    let sign = '+'
    let i = 0
    
    while (i < s.length()) {
        let char = s[i]
        
        if (char >= '0' && char <= '9') {
            num = num * 10 + (char - '0')
        }
        
        if (char == '+' || char == '-' || char == '*' || char == '/' || i == s.length() - 1) {
            if (sign == '+') {
                stack.push(num)
            } else if (sign == '-') {
                stack.push(-num)
            } else if (sign == '*') {
                stack.push(stack.pop() * num)
            } else if (sign == '/') {
                stack.push(stack.pop() / num)
            }
            
            sign = char
            num = 0
        }
        
        i = i + 1
    }
    
    let result = 0
    while (stack.length() > 0) {
        result = result + stack.pop()
    }
    
    return result
}
```

---

## Interview Problems

### 1. Valid Parentheses (LeetCode #20)

**Problem**: Given a string containing `()[]{}`, determine if valid.

```klang
fn isValid(s) {
    let stack = []
    let map = {
        ')': '(',
        ']': '[',
        '}': '{'
    }
    
    let i = 0
    while (i < s.length()) {
        let char = s[i]
        
        if (char == '(' || char == '[' || char == '{') {
            stack.push(char)
        } else {
            if (stack.length() == 0) {
                return false
            }
            if (stack.pop() != map[char]) {
                return false
            }
        }
        
        i = i + 1
    }
    
    return stack.length() == 0
}

# Test cases
print(isValid("()"))        # true
print(isValid("()[]{}"))    # true
print(isValid("(]"))        # false
print(isValid("([)]"))      # false
print(isValid("{[]}"))      # true
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

### 2. Min Stack (LeetCode #155)

**Problem**: Design a stack with min() operation in O(1).

```klang
class MinStack {
    constructor() {
        this.stack = []
        this.minStack = []
    }
    
    fn push(val) {
        this.stack.push(val)
        
        if (this.minStack.length() == 0 || 
            val <= this.minStack[this.minStack.length() - 1]) {
            this.minStack.push(val)
        }
    }
    
    fn pop() {
        if (this.stack.length() == 0) {
            return null
        }
        
        let val = this.stack.pop()
        if (val == this.minStack[this.minStack.length() - 1]) {
            this.minStack.pop()
        }
        return val
    }
    
    fn top() {
        if (this.stack.length() == 0) {
            return null
        }
        return this.stack[this.stack.length() - 1]
    }
    
    fn getMin() {
        if (this.minStack.length() == 0) {
            return null
        }
        return this.minStack[this.minStack.length() - 1]
    }
}

# Test
let minStack = MinStack()
minStack.push(-2)
minStack.push(0)
minStack.push(-3)
print(minStack.getMin())   # -3
minStack.pop()
print(minStack.top())      # 0
print(minStack.getMin())   # -2
```

**Time Complexity**: O(1) for all operations  
**Space Complexity**: O(n)

### 3. Evaluate Reverse Polish Notation (LeetCode #150)

**Problem**: Evaluate arithmetic expression in RPN.

```klang
fn evalRPN(tokens) {
    let stack = []
    let i = 0
    
    while (i < tokens.length()) {
        let token = tokens[i]
        
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            let b = stack.pop()
            let a = stack.pop()
            
            if (token == "+") {
                stack.push(a + b)
            } else if (token == "-") {
                stack.push(a - b)
            } else if (token == "*") {
                stack.push(a * b)
            } else {
                stack.push(a / b)
            }
        } else {
            stack.push(parseInt(token))
        }
        
        i = i + 1
    }
    
    return stack.pop()
}

# Test
print(evalRPN(["2", "1", "+", "3", "*"]))  # 9
print(evalRPN(["4", "13", "5", "/", "+"]))  # 6
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

### 4. Daily Temperatures (LeetCode #739)

**Problem**: Find how many days until warmer temperature.

```klang
fn dailyTemperatures(temperatures) {
    let n = temperatures.length()
    let result = []
    let stack = []  # Store indices
    
    # Initialize result with zeros
    let i = 0
    while (i < n) {
        result.push(0)
        i = i + 1
    }
    
    i = 0
    while (i < n) {
        # While current temp is warmer than stack top
        while (stack.length() > 0 && 
               temperatures[i] > temperatures[stack[stack.length() - 1]]) {
            let idx = stack.pop()
            result[idx] = i - idx
        }
        
        stack.push(i)
        i = i + 1
    }
    
    return result
}

# Test
let temps = [73, 74, 75, 71, 69, 72, 76, 73]
print(dailyTemperatures(temps))  # [1, 1, 4, 2, 1, 1, 0, 0]
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

---

## Best Practices

### 1. Check for Empty Stack

Always check if stack is empty before pop/peek operations:

```klang
fn safePop(stack) {
    if (stack.length() == 0) {
        print("Error: Stack is empty")
        return null
    }
    return stack.pop()
}
```

### 2. Use Stack for LIFO Problems

If problem requires reverse order processing, consider using a stack:

- Balanced parentheses
- Backtracking
- Undo operations
- DFS traversal

### 3. Consider Auxiliary Stack

For problems like min/max stack, use auxiliary stack to track extremes:

```klang
# Track both value and minimum
class MinStack {
    constructor() {
        this.stack = []  # Store [value, min_at_this_point]
    }
    
    fn push(val) {
        if (this.stack.length() == 0) {
            this.stack.push([val, val])
        } else {
            let currentMin = this.stack[this.stack.length() - 1][1]
            this.stack.push([val, min(val, currentMin)])
        }
    }
}
```

### 4. Optimize Space with Single Stack

Sometimes you can use a single stack cleverly instead of multiple stacks:

```klang
# Store differences instead of actual values
class SpaceOptimizedMinStack {
    constructor() {
        this.stack = []
        this.min = null
    }
    
    fn push(val) {
        if (this.stack.length() == 0) {
            this.stack.push(0)
            this.min = val
        } else {
            this.stack.push(val - this.min)
            if (val < this.min) {
                this.min = val
            }
        }
    }
}
```

### 5. Use Stack for Monotonic Problems

For next greater/smaller element problems, use monotonic stack:

```klang
# Monotonic decreasing stack for next greater element
fn nextGreater(arr) {
    let stack = []  # Keep elements in decreasing order
    let result = []
    
    let i = arr.length() - 1
    while (i >= 0) {
        while (stack.length() > 0 && stack[stack.length() - 1] <= arr[i]) {
            stack.pop()
        }
        result[i] = stack.length() == 0 ? -1 : stack[stack.length() - 1]
        stack.push(arr[i])
        i = i - 1
    }
    
    return result
}
```

### 6. Clear Stack Properly

When reusing a stack, ensure it's properly cleared:

```klang
fn clearStack(stack) {
    while (stack.length() > 0) {
        stack.pop()
    }
    # Or: stack = []
}
```

### 7. Use Meaningful Variable Names

```klang
# Bad
let s = []

# Good
let operatorStack = []
let parenthesisStack = []
let historyStack = []
```

---

## Complete Examples

### Example 1: Complete Calculator

```klang
class Calculator {
    constructor() {
        this.history = []
    }
    
    fn evaluate(expression) {
        let numStack = []
        let opStack = []
        let i = 0
        let num = 0
        let hasNum = false
        
        while (i < expression.length()) {
            let char = expression[i]
            
            if (char >= '0' && char <= '9') {
                num = num * 10 + (char - '0')
                hasNum = true
            } else if (char == ' ') {
                # Skip spaces
            } else {
                if (hasNum) {
                    numStack.push(num)
                    num = 0
                    hasNum = false
                }
                
                if (char == '(') {
                    opStack.push(char)
                } else if (char == ')') {
                    while (opStack.length() > 0 && 
                           opStack[opStack.length() - 1] != '(') {
                        this.processOperator(numStack, opStack)
                    }
                    opStack.pop()  # Remove '('
                } else if (this.isOperator(char)) {
                    while (opStack.length() > 0 && 
                           this.precedence(opStack[opStack.length() - 1]) >= 
                           this.precedence(char)) {
                        this.processOperator(numStack, opStack)
                    }
                    opStack.push(char)
                }
            }
            
            i = i + 1
        }
        
        if (hasNum) {
            numStack.push(num)
        }
        
        while (opStack.length() > 0) {
            this.processOperator(numStack, opStack)
        }
        
        let result = numStack.pop()
        this.history.push(expression + " = " + result)
        return result
    }
    
    fn isOperator(char) {
        return char == '+' || char == '-' || char == '*' || char == '/'
    }
    
    fn precedence(op) {
        if (op == '+' || op == '-') return 1
        if (op == '*' || op == '/') return 2
        return 0
    }
    
    fn processOperator(numStack, opStack) {
        let op = opStack.pop()
        let b = numStack.pop()
        let a = numStack.pop()
        
        if (op == '+') numStack.push(a + b)
        else if (op == '-') numStack.push(a - b)
        else if (op == '*') numStack.push(a * b)
        else if (op == '/') numStack.push(a / b)
    }
    
    fn showHistory() {
        print("Calculation History:")
        let i = 0
        while (i < this.history.length()) {
            print("  " + this.history[i])
            i = i + 1
        }
    }
}

# Usage
let calc = Calculator()
print(calc.evaluate("3 + 5 * 2"))      # 13
print(calc.evaluate("(3 + 5) * 2"))    # 16
print(calc.evaluate("10 - 2 * 3"))     # 4
calc.showHistory()
```

### Example 2: Complete Bracket Matcher

```klang
class BracketMatcher {
    constructor() {
        this.pairs = {
            ')': '(',
            ']': '[',
            '}': '{',
            '>': '<'
        }
    }
    
    fn isValid(s) {
        let stack = []
        let i = 0
        
        while (i < s.length()) {
            let char = s[i]
            
            if (this.isOpening(char)) {
                stack.push(char)
            } else if (this.isClosing(char)) {
                if (stack.length() == 0) {
                    return {
                        valid: false,
                        error: "Unexpected closing bracket at position " + i,
                        position: i
                    }
                }
                
                let top = stack.pop()
                if (top != this.pairs[char]) {
                    return {
                        valid: false,
                        error: "Mismatched brackets at position " + i,
                        position: i
                    }
                }
            }
            
            i = i + 1
        }
        
        if (stack.length() > 0) {
            return {
                valid: false,
                error: "Unclosed brackets remaining",
                position: -1
            }
        }
        
        return { valid: true, error: null, position: -1 }
    }
    
    fn isOpening(char) {
        return char == '(' || char == '[' || char == '{' || char == '<'
    }
    
    fn isClosing(char) {
        return char == ')' || char == ']' || char == '}' || char == '>'
    }
    
    fn findErrors(s) {
        let errors = []
        let i = 0
        
        while (i < s.length()) {
            let result = this.isValid(s.substring(0, i + 1))
            if (!result.valid && result.position == i) {
                errors.push(result)
            }
            i = i + 1
        }
        
        return errors
    }
}

# Usage
let matcher = BracketMatcher()
let result = matcher.isValid("({[]})")
print("Valid: " + result.valid)

result = matcher.isValid("({[})")
print("Valid: " + result.valid)
print("Error: " + result.error)
```

### Example 3: Stack-based Maze Solver (DFS)

```klang
class MazeSolver {
    constructor(maze) {
        this.maze = maze
        this.rows = maze.length()
        this.cols = maze[0].length()
    }
    
    fn solve(startRow, startCol, endRow, endCol) {
        let stack = []
        let visited = []
        let parent = []
        
        # Initialize visited
        let i = 0
        while (i < this.rows) {
            visited.push([])
            parent.push([])
            let j = 0
            while (j < this.cols) {
                visited[i].push(false)
                parent[i].push(null)
                j = j + 1
            }
            i = i + 1
        }
        
        # Start DFS
        stack.push([startRow, startCol])
        visited[startRow][startCol] = true
        
        while (stack.length() > 0) {
            let current = stack.pop()
            let row = current[0]
            let col = current[1]
            
            # Check if reached end
            if (row == endRow && col == endCol) {
                return this.reconstructPath(parent, startRow, startCol, endRow, endCol)
            }
            
            # Explore neighbors (up, right, down, left)
            let directions = [[-1, 0], [0, 1], [1, 0], [0, -1]]
            let d = 0
            while (d < directions.length()) {
                let newRow = row + directions[d][0]
                let newCol = col + directions[d][1]
                
                if (this.isValid(newRow, newCol) && !visited[newRow][newCol] && 
                    this.maze[newRow][newCol] != 1) {
                    stack.push([newRow, newCol])
                    visited[newRow][newCol] = true
                    parent[newRow][newCol] = [row, col]
                }
                
                d = d + 1
            }
        }
        
        return []  # No path found
    }
    
    fn isValid(row, col) {
        return row >= 0 && row < this.rows && col >= 0 && col < this.cols
    }
    
    fn reconstructPath(parent, startRow, startCol, endRow, endCol) {
        let path = []
        let current = [endRow, endCol]
        
        while (current != null) {
            path.push(current)
            if (current[0] == startRow && current[1] == startCol) {
                break
            }
            current = parent[current[0]][current[1]]
        }
        
        # Reverse path
        let reversed = []
        let i = path.length() - 1
        while (i >= 0) {
            reversed.push(path[i])
            i = i - 1
        }
        
        return reversed
    }
}

# Usage
let maze = [
    [0, 1, 0, 0, 0],
    [0, 1, 0, 1, 0],
    [0, 0, 0, 1, 0],
    [1, 1, 0, 0, 0],
    [0, 0, 0, 1, 0]
]

let solver = MazeSolver(maze)
let path = solver.solve(0, 0, 4, 4)
print("Path found:")
let i = 0
while (i < path.length()) {
    print("  (" + path[i][0] + ", " + path[i][1] + ")")
    i = i + 1
}
```

---

## Summary

Stacks are fundamental LIFO data structures perfect for:
- Reversing operations (undo/redo)
- Backtracking (DFS, maze solving)
- Expression evaluation (calculators)
- Balanced parentheses checking
- Function call management

**Key Takeaways**:
- O(1) push/pop operations
- LIFO principle (Last In, First Out)
- Essential for many algorithms
- Can be implemented with arrays or linked lists
- Used extensively in compilers and runtimes

**When to Use Stacks**:
- Need LIFO ordering
- Backtracking required
- Matching/balancing operations
- Expression parsing
- Depth-first traversal

---

## Further Reading

- [Stack Applications in Compiler Design](../algorithms/compiler-design.md)
- [DFS with Stacks](../algorithms/graph-traversal.md)
- [Expression Evaluation](../algorithms/expression-evaluation.md)
- [Backtracking Algorithms](../algorithms/backtracking.md)

---

*Part of the KLang DSA Documentation Series*
