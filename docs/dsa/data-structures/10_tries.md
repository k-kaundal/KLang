# Tries (Prefix Trees) in KLang - Data Structures & Algorithms

## Table of Contents
1. [Introduction](#introduction)
2. [What is a Trie?](#what-is-a-trie)
3. [Trie Node Structure](#trie-node-structure)
4. [Why Use Tries?](#why-use-tries)
5. [Trie Operations](#trie-operations)
6. [Time and Space Complexity](#time-and-space-complexity)
7. [Children Representation](#children-representation)
8. [Trie Algorithms](#trie-algorithms)
9. [Practical Examples](#practical-examples)
10. [Interview Problems](#interview-problems)
11. [Trie vs Hash Table](#trie-vs-hash-table)
12. [Space Optimization](#space-optimization)
13. [Best Practices](#best-practices)
14. [Complete Examples](#complete-examples)

---

## Introduction

A **Trie** (pronounced "try"), also known as a **prefix tree** or **digital tree**, is a tree-based data structure used for storing and retrieving strings. Each node represents a single character, and paths from root to nodes form words or prefixes.

### Why Use Tries?

- **Fast Prefix Operations**: O(m) time for prefix-based searches
- **Autocomplete**: Efficiently find all words with a given prefix
- **Space Efficient**: Shared prefixes reduce redundancy
- **Ordered Traversal**: Can iterate words in lexicographic order
- **Pattern Matching**: Ideal for dictionary operations and spell checking

### Real-World Use Cases

- **Autocomplete Systems**: Search suggestions in browsers and IDEs
- **Spell Checkers**: Dictionary lookup and correction suggestions
- **IP Routing**: Longest prefix matching in network routers
- **Text Editors**: Word prediction and completion
- **Contact Lists**: Fast prefix-based contact search
- **Search Engines**: Query suggestions and ranking

---

## What is a Trie?

A **Trie** is a tree where each node represents a character in a string. The path from root to any node represents a prefix of stored strings.

### Basic Structure

```
Trie storing: "cat", "car", "card", "dog", "door"

        (root)
        /    \
       c      d
       |      |
       a      o
      / \     |
     t   r    o
         |    |
         d    r
```

### Key Properties

1. **Root is Empty**: The root node doesn't contain a character
2. **Each Edge**: Represents a character
3. **Path from Root**: Forms a word or prefix
4. **End Marker**: Flag to indicate word completion
5. **Shared Prefixes**: Common prefixes share nodes

### Visual Representation with End Markers

```
Words: "app", "apple", "apply", "apt"

        (root)
          |
          a
          |
          p
         / \
        p*  t*
        |
        l
       / \
      e*  y*

* = isEndOfWord flag
```

---

## Trie Node Structure

Each trie node contains:
- **Children**: Array or map to child nodes
- **isEndOfWord**: Boolean flag marking word completion
- **Optional**: Count, value, or other metadata

### Basic Node Structure

```klang
class TrieNode {
    var children: Map<char, TrieNode>
    var isEndOfWord: bool
    
    func constructor() {
        this.children = {}
        this.isEndOfWord = false
    }
}
```

### Node with Count (for frequency tracking)

```klang
class TrieNodeWithCount {
    var children: Map<char, TrieNode>
    var isEndOfWord: bool
    var count: int  # How many times this word appears
    
    func constructor() {
        this.children = {}
        this.isEndOfWord = false
        this.count = 0
    }
}
```

### Node with Value (for key-value storage)

```klang
class TrieNodeWithValue<T> {
    var children: Map<char, TrieNode<T>>
    var isEndOfWord: bool
    var value: T?
    
    func constructor() {
        this.children = {}
        this.isEndOfWord = false
        this.value = null
    }
}
```

---

## Why Use Tries?

### Advantages over Other Data Structures

**1. Prefix Operations**
```klang
# Hash Table: Find all words with prefix "app"
# Must check every key: O(n * m) where n = keys, m = key length

# Trie: Traverse to "app" node, then get all descendants
# O(m + k) where m = prefix length, k = result count
```

**2. Memory Efficiency for Similar Strings**
```
Hash Table: "apple", "application", "apply"
- Stores: 18 characters total

Trie: Shares prefix "appl"
- Stores: 11 unique characters
```

**3. Lexicographic Ordering**
```klang
# Trie naturally maintains sorted order
# In-order traversal gives alphabetically sorted words
```

**4. Pattern Matching**
```klang
# Wildcards: "c?t" → "cat", "cot", "cut"
# Regular expressions: "ca*" → "ca", "cab", "cat", "card"
```

---

## Trie Operations

### 1. Insert Word - O(m)

Add a word to the trie, where m = word length.

```klang
func insert(word: string) {
    var current = this.root
    
    for i in range(word.length()) {
        let ch = word[i]
        
        if !current.children.contains(ch) {
            current.children[ch] = TrieNode()
        }
        
        current = current.children[ch]
    }
    
    current.isEndOfWord = true
}
```

**Visualization:**
```
Insert "cat" into empty trie:

Step 1: Insert 'c'     Step 2: Insert 'a'    Step 3: Insert 't'
  (root)                 (root)                 (root)
    |                      |                      |
    c                      c                      c
                           |                      |
                           a                      a
                                                  |
                                                  t*
```

### 2. Search Word - O(m)

Check if a complete word exists in the trie.

```klang
func search(word: string) -> bool {
    var current = this.root
    
    for i in range(word.length()) {
        let ch = word[i]
        
        if !current.children.contains(ch) {
            return false
        }
        
        current = current.children[ch]
    }
    
    return current.isEndOfWord
}
```

**Example:**
```klang
# Trie contains: "cat", "car"
trie.search("cat")   # → true (complete word)
trie.search("ca")    # → false (prefix only)
trie.search("card")  # → false (not in trie)
```

### 3. StartsWith (Prefix Search) - O(m)

Check if any word starts with the given prefix.

```klang
func startsWith(prefix: string) -> bool {
    var current = this.root
    
    for i in range(prefix.length()) {
        let ch = prefix[i]
        
        if !current.children.contains(ch) {
            return false
        }
        
        current = current.children[ch]
    }
    
    return true  # Prefix exists
}
```

**Example:**
```klang
# Trie contains: "apple", "application"
trie.startsWith("app")  # → true
trie.startsWith("appl") # → true
trie.startsWith("ban")  # → false
```

### 4. Delete Word - O(m)

Remove a word from the trie (complex operation).

```klang
func delete(word: string) -> bool {
    return this._deleteHelper(this.root, word, 0)
}

func _deleteHelper(node: TrieNode?, word: string, index: int) -> bool {
    if node == null {
        return false
    }
    
    # Base case: reached end of word
    if index == word.length() {
        if !node.isEndOfWord {
            return false  # Word doesn't exist
        }
        
        node.isEndOfWord = false
        
        # Delete node if it has no children
        return node.children.isEmpty()
    }
    
    let ch = word[index]
    let childNode = node.children.get(ch)
    
    let shouldDeleteChild = this._deleteHelper(childNode, word, index + 1)
    
    if shouldDeleteChild {
        node.children.remove(ch)
        
        # Delete current node if:
        # 1. Not end of another word
        # 2. Has no other children
        return !node.isEndOfWord && node.children.isEmpty()
    }
    
    return false
}
```

---

## Time and Space Complexity

### Time Complexity

| Operation | Average | Worst Case | Notes |
|-----------|---------|------------|-------|
| Insert | O(m) | O(m) | m = word length |
| Search | O(m) | O(m) | m = word length |
| StartsWith | O(m) | O(m) | m = prefix length |
| Delete | O(m) | O(m) | m = word length |
| Get All Words | O(n) | O(n) | n = total characters |

### Space Complexity

| Aspect | Space | Notes |
|--------|-------|-------|
| Node Storage | O(ALPHABET_SIZE * N * M) | N = words, M = avg length |
| Optimized | O(Total characters) | Shared prefixes |
| Best Case | O(k) | k = unique characters |
| Worst Case | O(n * m * 26) | No shared prefixes |

---

## Children Representation

### 1. Array Representation

**Fixed-size array for each node.**

```klang
class TrieNodeArray {
    var children: Array<TrieNode?>  # Size 26 for 'a'-'z'
    var isEndOfWord: bool
    
    func constructor() {
        this.children = Array(26, null)
        this.isEndOfWord = false
    }
    
    func getIndex(ch: char) -> int {
        return ch.code() - 'a'.code()
    }
    
    func insert(ch: char) {
        let index = this.getIndex(ch)
        if this.children[index] == null {
            this.children[index] = TrieNodeArray()
        }
    }
}
```

**Pros:**
- O(1) child access
- Simple implementation
- Fast operations

**Cons:**
- Wastes space (26 pointers per node)
- Not suitable for large alphabets (Unicode)
- Memory intensive

### 2. Hash Map Representation

**Dynamic map storing only existing children.**

```klang
class TrieNodeMap {
    var children: Map<char, TrieNode>
    var isEndOfWord: bool
    
    func constructor() {
        this.children = {}
        this.isEndOfWord = false
    }
    
    func insert(ch: char) {
        if !this.children.contains(ch) {
            this.children[ch] = TrieNodeMap()
        }
    }
}
```

**Pros:**
- Space efficient (only stores existing children)
- Works with any alphabet/Unicode
- Dynamic sizing

**Cons:**
- Slightly slower access O(1) average, O(n) worst
- Hash map overhead

### 3. Compressed Trie (Radix Tree)

**Nodes can contain multiple characters.**

```klang
class RadixNode {
    var children: Map<string, RadixNode>
    var isEndOfWord: bool
    var edge: string  # Edge label (may be multi-char)
    
    func constructor(edge: string = "") {
        this.children = {}
        this.isEndOfWord = false
        this.edge = edge
    }
}
```

---

## Trie Algorithms

### 1. Count Words with Prefix

Count how many words start with a given prefix.

```klang
func countWordsWithPrefix(prefix: string) -> int {
    var current = this.root
    
    # Navigate to prefix
    for i in range(prefix.length()) {
        let ch = prefix[i]
        if !current.children.contains(ch) {
            return 0
        }
        current = current.children[ch]
    }
    
    # Count words from this node
    return this._countWords(current)
}

func _countWords(node: TrieNode?) -> int {
    if node == null {
        return 0
    }
    
    var count = node.isEndOfWord ? 1 : 0
    
    for child in node.children.values() {
        count += this._countWords(child)
    }
    
    return count
}
```

### 2. Get All Words with Prefix

Retrieve all words starting with a prefix.

```klang
func getAllWordsWithPrefix(prefix: string) -> Array<string> {
    var result = []
    var current = this.root
    
    # Navigate to prefix
    for i in range(prefix.length()) {
        let ch = prefix[i]
        if !current.children.contains(ch) {
            return result
        }
        current = current.children[ch]
    }
    
    # Collect all words from this node
    this._collectWords(current, prefix, result)
    return result
}

func _collectWords(node: TrieNode?, prefix: string, result: Array<string>) {
    if node == null {
        return
    }
    
    if node.isEndOfWord {
        result.push(prefix)
    }
    
    for (ch, child) in node.children {
        this._collectWords(child, prefix + ch, result)
    }
}
```

### 3. Longest Common Prefix

Find the longest common prefix of all words.

```klang
func longestCommonPrefix() -> string {
    var prefix = ""
    var current = this.root
    
    while current.children.size() == 1 && !current.isEndOfWord {
        let (ch, child) = current.children.entries()[0]
        prefix += ch
        current = child
    }
    
    return prefix
}
```

### 4. Word Break Problem

Check if a string can be segmented into words from dictionary.

```klang
func wordBreak(s: string) -> bool {
    let n = s.length()
    var dp = Array(n + 1, false)
    dp[0] = true
    
    for i in range(1, n + 1) {
        for j in range(i) {
            if dp[j] && this.search(s.substring(j, i)) {
                dp[i] = true
                break
            }
        }
    }
    
    return dp[n]
}
```

---

## Practical Examples

### Example 1: Dictionary Implementation

```klang
class Dictionary {
    var root: TrieNode
    
    func constructor() {
        this.root = TrieNode()
    }
    
    func addWord(word: string) {
        var current = this.root
        
        for i in range(word.length()) {
            let ch = word[i].toLowerCase()
            
            if !current.children.contains(ch) {
                current.children[ch] = TrieNode()
            }
            
            current = current.children[ch]
        }
        
        current.isEndOfWord = true
    }
    
    func isValidWord(word: string) -> bool {
        var current = this.root
        
        for i in range(word.length()) {
            let ch = word[i].toLowerCase()
            
            if !current.children.contains(ch) {
                return false
            }
            
            current = current.children[ch]
        }
        
        return current.isEndOfWord
    }
    
    func getSuggestions(prefix: string) -> Array<string> {
        var suggestions = []
        var current = this.root
        
        # Navigate to prefix
        for i in range(prefix.length()) {
            let ch = prefix[i].toLowerCase()
            
            if !current.children.contains(ch) {
                return suggestions
            }
            
            current = current.children[ch]
        }
        
        # Collect all words from this point
        this._collectWords(current, prefix.toLowerCase(), suggestions)
        return suggestions
    }
    
    func _collectWords(node: TrieNode, prefix: string, result: Array<string>) {
        if node.isEndOfWord {
            result.push(prefix)
        }
        
        for (ch, child) in node.children {
            this._collectWords(child, prefix + ch, result)
        }
    }
}

# Usage
let dict = Dictionary()
dict.addWord("apple")
dict.addWord("application")
dict.addWord("apply")
dict.addWord("app")

print(dict.isValidWord("apple"))      # true
print(dict.isValidWord("appl"))       # false
print(dict.getSuggestions("app"))     # ["app", "apple", "application", "apply"]
```

### Example 2: Autocomplete System

```klang
class AutocompleteSystem {
    var root: TrieNode
    var maxSuggestions: int
    
    func constructor(maxSuggestions: int = 5) {
        this.root = TrieNode()
        this.maxSuggestions = maxSuggestions
    }
    
    func addQuery(query: string, frequency: int = 1) {
        var current = this.root
        
        for i in range(query.length()) {
            let ch = query[i]
            
            if !current.children.contains(ch) {
                current.children[ch] = TrieNodeWithCount()
            }
            
            current = current.children[ch]
        }
        
        current.isEndOfWord = true
        current.count += frequency
    }
    
    func autocomplete(prefix: string) -> Array<{query: string, count: int}> {
        var current = this.root
        
        # Navigate to prefix
        for i in range(prefix.length()) {
            let ch = prefix[i]
            
            if !current.children.contains(ch) {
                return []
            }
            
            current = current.children[ch]
        }
        
        # Collect suggestions with frequencies
        var suggestions = []
        this._collectSuggestions(current, prefix, suggestions)
        
        # Sort by frequency (descending) and return top results
        suggestions.sort((a, b) => b.count - a.count)
        return suggestions.slice(0, this.maxSuggestions)
    }
    
    func _collectSuggestions(
        node: TrieNode, 
        query: string, 
        result: Array<{query: string, count: int}>
    ) {
        if node.isEndOfWord {
            result.push({query: query, count: node.count})
        }
        
        for (ch, child) in node.children {
            this._collectSuggestions(child, query + ch, result)
        }
    }
}

# Usage
let autocomplete = AutocompleteSystem(3)

# Add search queries with frequencies
autocomplete.addQuery("python tutorial", 100)
autocomplete.addQuery("python basics", 80)
autocomplete.addQuery("python advanced", 60)
autocomplete.addQuery("java tutorial", 50)

let suggestions = autocomplete.autocomplete("python")
print(suggestions)
# Output: [
#   {query: "python tutorial", count: 100},
#   {query: "python basics", count: 80},
#   {query: "python advanced", count: 60}
# ]
```

### Example 3: Spell Checker

```klang
class SpellChecker {
    var dictionary: TrieNode
    
    func constructor() {
        this.dictionary = TrieNode()
    }
    
    func addWord(word: string) {
        var current = this.dictionary
        
        for i in range(word.length()) {
            let ch = word[i].toLowerCase()
            
            if !current.children.contains(ch) {
                current.children[ch] = TrieNode()
            }
            
            current = current.children[ch]
        }
        
        current.isEndOfWord = true
    }
    
    func isCorrect(word: string) -> bool {
        var current = this.dictionary
        
        for i in range(word.length()) {
            let ch = word[i].toLowerCase()
            
            if !current.children.contains(ch) {
                return false
            }
            
            current = current.children[ch]
        }
        
        return current.isEndOfWord
    }
    
    func getSuggestions(word: string) -> Array<string> {
        if this.isCorrect(word) {
            return [word]  # Word is correct
        }
        
        var suggestions = []
        
        # Try different edit operations
        this._getEditDistance1(word, suggestions)
        
        return suggestions.slice(0, 5)  # Top 5 suggestions
    }
    
    func _getEditDistance1(word: string, suggestions: Array<string>) {
        # Deletion: remove one character
        for i in range(word.length()) {
            let candidate = word.substring(0, i) + word.substring(i + 1)
            if this.isCorrect(candidate) && !suggestions.contains(candidate) {
                suggestions.push(candidate)
            }
        }
        
        # Insertion: add one character
        let alphabet = "abcdefghijklmnopqrstuvwxyz"
        for i in range(word.length() + 1) {
            for j in range(alphabet.length()) {
                let candidate = word.substring(0, i) + alphabet[j] + word.substring(i)
                if this.isCorrect(candidate) && !suggestions.contains(candidate) {
                    suggestions.push(candidate)
                }
            }
        }
        
        # Substitution: replace one character
        for i in range(word.length()) {
            for j in range(alphabet.length()) {
                let candidate = word.substring(0, i) + alphabet[j] + word.substring(i + 1)
                if this.isCorrect(candidate) && !suggestions.contains(candidate) {
                    suggestions.push(candidate)
                }
            }
        }
        
        # Transposition: swap adjacent characters
        for i in range(word.length() - 1) {
            let candidate = word.substring(0, i) + word[i + 1] + word[i] + word.substring(i + 2)
            if this.isCorrect(candidate) && !suggestions.contains(candidate) {
                suggestions.push(candidate)
            }
        }
    }
}

# Usage
let checker = SpellChecker()
checker.addWord("hello")
checker.addWord("world")
checker.addWord("help")

print(checker.isCorrect("hello"))     # true
print(checker.isCorrect("helo"))      # false
print(checker.getSuggestions("helo")) # ["hello", "help"]
```

### Example 4: Word Search Game

```klang
class WordSearchGame {
    var dictionary: TrieNode
    var board: Array<Array<char>>
    var rows: int
    var cols: int
    var foundWords: Set<string>
    
    func constructor(words: Array<string>) {
        this.dictionary = TrieNode()
        this.foundWords = Set()
        
        # Build dictionary trie
        for word in words {
            this.addWord(word)
        }
    }
    
    func addWord(word: string) {
        var current = this.dictionary
        
        for i in range(word.length()) {
            let ch = word[i]
            
            if !current.children.contains(ch) {
                current.children[ch] = TrieNode()
            }
            
            current = current.children[ch]
        }
        
        current.isEndOfWord = true
    }
    
    func findWords(board: Array<Array<char>>) -> Array<string> {
        this.board = board
        this.rows = board.length()
        this.cols = board[0].length()
        this.foundWords = Set()
        
        # Start search from each cell
        for i in range(this.rows) {
            for j in range(this.cols) {
                this._dfs(i, j, this.dictionary, "")
            }
        }
        
        return Array.from(this.foundWords)
    }
    
    func _dfs(row: int, col: int, node: TrieNode, word: string) {
        # Boundary check
        if row < 0 || row >= this.rows || col < 0 || col >= this.cols {
            return
        }
        
        let ch = this.board[row][col]
        
        # Already visited or not in trie
        if ch == '#' || !node.children.contains(ch) {
            return
        }
        
        let nextNode = node.children[ch]
        let nextWord = word + ch
        
        # Found a word
        if nextNode.isEndOfWord {
            this.foundWords.add(nextWord)
        }
        
        # Mark as visited
        this.board[row][col] = '#'
        
        # Explore all 4 directions
        this._dfs(row + 1, col, nextNode, nextWord)  # Down
        this._dfs(row - 1, col, nextNode, nextWord)  # Up
        this._dfs(row, col + 1, nextNode, nextWord)  # Right
        this._dfs(row, col - 1, nextNode, nextWord)  # Left
        
        # Restore cell
        this.board[row][col] = ch
    }
}

# Usage
let words = ["oath", "pea", "eat", "rain"]
let game = WordSearchGame(words)

let board = [
    ['o', 'a', 'a', 'n'],
    ['e', 't', 'a', 'e'],
    ['i', 'h', 'k', 'r'],
    ['i', 'f', 'l', 'v']
]

let found = game.findWords(board)
print(found)  # ["oath", "eat"]
```

---

## Interview Problems

### Problem 1: Implement Trie (Insert, Search, StartsWith)

**Problem**: Implement a trie with insert, search, and startsWith methods.

**LeetCode**: #208

```klang
class Trie {
    var root: TrieNode
    
    func constructor() {
        this.root = TrieNode()
    }
    
    func insert(word: string) {
        var current = this.root
        
        for i in range(word.length()) {
            let ch = word[i]
            
            if !current.children.contains(ch) {
                current.children[ch] = TrieNode()
            }
            
            current = current.children[ch]
        }
        
        current.isEndOfWord = true
    }
    
    func search(word: string) -> bool {
        var current = this.root
        
        for i in range(word.length()) {
            let ch = word[i]
            
            if !current.children.contains(ch) {
                return false
            }
            
            current = current.children[ch]
        }
        
        return current.isEndOfWord
    }
    
    func startsWith(prefix: string) -> bool {
        var current = this.root
        
        for i in range(prefix.length()) {
            let ch = prefix[i]
            
            if !current.children.contains(ch) {
                return false
            }
            
            current = current.children[ch]
        }
        
        return true
    }
}
```

### Problem 2: Add and Search Word (with wildcards)

**Problem**: Design a data structure that supports adding words and searching with '.' wildcard.

**LeetCode**: #211

```klang
class WordDictionary {
    var root: TrieNode
    
    func constructor() {
        this.root = TrieNode()
    }
    
    func addWord(word: string) {
        var current = this.root
        
        for i in range(word.length()) {
            let ch = word[i]
            
            if !current.children.contains(ch) {
                current.children[ch] = TrieNode()
            }
            
            current = current.children[ch]
        }
        
        current.isEndOfWord = true
    }
    
    func search(word: string) -> bool {
        return this._searchHelper(word, 0, this.root)
    }
    
    func _searchHelper(word: string, index: int, node: TrieNode?) -> bool {
        if node == null {
            return false
        }
        
        if index == word.length() {
            return node.isEndOfWord
        }
        
        let ch = word[index]
        
        if ch == '.' {
            # Wildcard: try all children
            for child in node.children.values() {
                if this._searchHelper(word, index + 1, child) {
                    return true
                }
            }
            return false
        } else {
            # Regular character
            if !node.children.contains(ch) {
                return false
            }
            return this._searchHelper(word, index + 1, node.children[ch])
        }
    }
}

# Usage
let dict = WordDictionary()
dict.addWord("bad")
dict.addWord("dad")
dict.addWord("mad")

print(dict.search("pad"))    # false
print(dict.search("bad"))    # true
print(dict.search(".ad"))    # true
print(dict.search("b.."))    # true
```

### Problem 3: Word Search II

**Problem**: Find all words from a dictionary that can be formed on a 2D board.

**LeetCode**: #212

```klang
func findWords(board: Array<Array<char>>, words: Array<string>) -> Array<string> {
    # Build trie from words
    let root = TrieNode()
    
    for word in words {
        var current = root
        for i in range(word.length()) {
            let ch = word[i]
            if !current.children.contains(ch) {
                current.children[ch] = TrieNode()
            }
            current = current.children[ch]
        }
        current.isEndOfWord = true
        current.word = word  # Store word at end node
    }
    
    let rows = board.length()
    let cols = board[0].length()
    var result = []
    
    # DFS from each cell
    for i in range(rows) {
        for j in range(cols) {
            _dfs(board, i, j, root, result)
        }
    }
    
    return result
}

func _dfs(board: Array<Array<char>>, row: int, col: int, node: TrieNode, result: Array<string>) {
    let rows = board.length()
    let cols = board[0].length()
    
    if row < 0 || row >= rows || col < 0 || col >= cols {
        return
    }
    
    let ch = board[row][col]
    
    if ch == '#' || !node.children.contains(ch) {
        return
    }
    
    let nextNode = node.children[ch]
    
    if nextNode.isEndOfWord {
        result.push(nextNode.word)
        nextNode.isEndOfWord = false  # Avoid duplicates
    }
    
    board[row][col] = '#'  # Mark visited
    
    _dfs(board, row + 1, col, nextNode, result)
    _dfs(board, row - 1, col, nextNode, result)
    _dfs(board, row, col + 1, nextNode, result)
    _dfs(board, row, col - 1, nextNode, result)
    
    board[row][col] = ch  # Restore
}
```

### Problem 4: Design Search Autocomplete System

**Problem**: Design a search autocomplete system that returns top 3 matching sentences.

**LeetCode**: #642

```klang
class AutocompleteSystem {
    var root: TrieNode
    var currentNode: TrieNode?
    var currentInput: string
    
    func constructor(sentences: Array<string>, times: Array<int>) {
        this.root = TrieNode()
        this.currentNode = this.root
        this.currentInput = ""
        
        # Build trie with frequencies
        for i in range(sentences.length()) {
            this.addSentence(sentences[i], times[i])
        }
    }
    
    func addSentence(sentence: string, frequency: int) {
        var current = this.root
        
        for i in range(sentence.length()) {
            let ch = sentence[i]
            
            if !current.children.contains(ch) {
                current.children[ch] = TrieNode()
            }
            
            current = current.children[ch]
        }
        
        current.isEndOfWord = true
        current.sentence = sentence
        current.frequency += frequency
    }
    
    func input(c: char) -> Array<string> {
        if c == '#' {
            # End of sentence - add to trie
            this.addSentence(this.currentInput, 1)
            this.currentInput = ""
            this.currentNode = this.root
            return []
        }
        
        this.currentInput += c
        
        if this.currentNode == null {
            return []
        }
        
        if !this.currentNode.children.contains(c) {
            this.currentNode = null
            return []
        }
        
        this.currentNode = this.currentNode.children[c]
        
        # Get all sentences from current node
        var candidates = []
        this._collectSentences(this.currentNode, candidates)
        
        # Sort by frequency (desc), then lexicographically
        candidates.sort((a, b) => {
            if a.frequency != b.frequency {
                return b.frequency - a.frequency
            }
            return a.sentence.compare(b.sentence)
        })
        
        # Return top 3
        return candidates.slice(0, 3).map(c => c.sentence)
    }
    
    func _collectSentences(node: TrieNode?, result: Array<{sentence: string, frequency: int}>) {
        if node == null {
            return
        }
        
        if node.isEndOfWord {
            result.push({sentence: node.sentence, frequency: node.frequency})
        }
        
        for child in node.children.values() {
            this._collectSentences(child, result)
        }
    }
}

# Usage
let sentences = ["i love you", "island", "iroman", "i love leetcode"]
let times = [5, 3, 2, 2]
let system = AutocompleteSystem(sentences, times)

print(system.input('i'))   # ["i love you", "island", "i love leetcode"]
print(system.input(' '))   # ["i love you", "i love leetcode"]
print(system.input('a'))   # []
print(system.input('#'))   # [] - adds "i a" to trie
```

### Problem 5: Replace Words

**Problem**: Replace words with their shortest root from dictionary.

**LeetCode**: #648

```klang
func replaceWords(dictionary: Array<string>, sentence: string) -> string {
    # Build trie from roots
    let root = TrieNode()
    
    for word in dictionary {
        var current = root
        for i in range(word.length()) {
            let ch = word[i]
            if !current.children.contains(ch) {
                current.children[ch] = TrieNode()
            }
            current = current.children[ch]
        }
        current.isEndOfWord = true
    }
    
    # Process each word in sentence
    let words = sentence.split(" ")
    
    for i in range(words.length()) {
        words[i] = _findRoot(root, words[i])
    }
    
    return words.join(" ")
}

func _findRoot(root: TrieNode, word: string) -> string {
    var current = root
    
    for i in range(word.length()) {
        let ch = word[i]
        
        if !current.children.contains(ch) {
            return word  # No root found
        }
        
        current = current.children[ch]
        
        if current.isEndOfWord {
            return word.substring(0, i + 1)  # Found root
        }
    }
    
    return word  # No root found
}

# Usage
let dictionary = ["cat", "bat", "rat"]
let sentence = "the cattle was rattled by the battery"
print(replaceWords(dictionary, sentence))
# Output: "the cat was rat by the bat"
```

### Problem 6: Longest Word in Dictionary

**Problem**: Find the longest word that can be built one character at a time.

**LeetCode**: #720

```klang
func longestWord(words: Array<string>) -> string {
    # Build trie
    let root = TrieNode()
    
    for word in words {
        var current = root
        for i in range(word.length()) {
            let ch = word[i]
            if !current.children.contains(ch) {
                current.children[ch] = TrieNode()
            }
            current = current.children[ch]
        }
        current.isEndOfWord = true
        current.word = word
    }
    
    var longest = ""
    var queue = [root]
    
    while !queue.isEmpty() {
        let node = queue.shift()
        
        if node.isEndOfWord || node == root {
            if node != root {
                if node.word.length() > longest.length() ||
                   (node.word.length() == longest.length() && node.word < longest) {
                    longest = node.word
                }
            }
            
            # Add children to queue (sorted for lexicographic order)
            let children = node.children.keys().sort()
            for ch in children {
                queue.push(node.children[ch])
            }
        }
    }
    
    return longest
}

# Usage
let words = ["w", "wo", "wor", "worl", "world"]
print(longestWord(words))  # "world"
```

### Problem 7: Maximum XOR of Two Numbers

**Problem**: Find maximum XOR of two numbers in an array using binary trie.

**LeetCode**: #421

```klang
func findMaximumXOR(nums: Array<int>) -> int {
    # Build binary trie (32 bits)
    let root = TrieNode()
    
    for num in nums {
        var current = root
        
        # Insert 32-bit binary representation
        for i in range(31, -1, -1) {
            let bit = (num >> i) & 1
            let ch = bit.toString()
            
            if !current.children.contains(ch) {
                current.children[ch] = TrieNode()
            }
            
            current = current.children[ch]
        }
    }
    
    var maxXOR = 0
    
    # For each number, find maximum XOR
    for num in nums {
        var current = root
        var currentXOR = 0
        
        for i in range(31, -1, -1) {
            let bit = (num >> i) & 1
            let toggledBit = 1 - bit
            let ch = toggledBit.toString()
            
            if current.children.contains(ch) {
                # Choose opposite bit for maximum XOR
                currentXOR |= (1 << i)
                current = current.children[ch]
            } else {
                # Must choose same bit
                current = current.children[bit.toString()]
            }
        }
        
        maxXOR = Math.max(maxXOR, currentXOR)
    }
    
    return maxXOR
}

# Usage
let nums = [3, 10, 5, 25, 2, 8]
print(findMaximumXOR(nums))  # 28 (25 XOR 5)
```

### Problem 8: Concatenated Words

**Problem**: Find all concatenated words in a dictionary.

**LeetCode**: #472

```klang
func findAllConcatenatedWords(words: Array<string>) -> Array<string> {
    # Build trie
    let root = TrieNode()
    
    for word in words {
        var current = root
        for i in range(word.length()) {
            let ch = word[i]
            if !current.children.contains(ch) {
                current.children[ch] = TrieNode()
            }
            current = current.children[ch]
        }
        current.isEndOfWord = true
    }
    
    var result = []
    
    # Check each word
    for word in words {
        if _isConcatenated(word, 0, root, root, 0) {
            result.push(word)
        }
    }
    
    return result
}

func _isConcatenated(
    word: string, 
    index: int, 
    root: TrieNode, 
    current: TrieNode,
    count: int
) -> bool {
    if index == word.length() {
        return count > 1 && current.isEndOfWord
    }
    
    let ch = word[index]
    
    if !current.children.contains(ch) {
        return false
    }
    
    current = current.children[ch]
    
    # Option 1: Continue current word
    var result = _isConcatenated(word, index + 1, root, current, count)
    
    # Option 2: End current word and start new one
    if current.isEndOfWord {
        result = result || _isConcatenated(word, index + 1, root, root, count + 1)
    }
    
    return result
}

# Usage
let words = ["cat", "cats", "catsdogcats", "dog", "dogcatsdog", "hippopotamuses", "rat", "ratcatdogcat"]
print(findAllConcatenatedWords(words))
# Output: ["catsdogcats", "dogcatsdog", "ratcatdogcat"]
```

### Problem 9: Implement Magic Dictionary

**Problem**: Build a dictionary that supports search with one character replaced.

**LeetCode**: #676

```klang
class MagicDictionary {
    var root: TrieNode
    
    func constructor() {
        this.root = TrieNode()
    }
    
    func buildDict(words: Array<string>) {
        for word in words {
            var current = this.root
            
            for i in range(word.length()) {
                let ch = word[i]
                
                if !current.children.contains(ch) {
                    current.children[ch] = TrieNode()
                }
                
                current = current.children[ch]
            }
            
            current.isEndOfWord = true
        }
    }
    
    func search(word: string) -> bool {
        return this._searchHelper(word, 0, this.root, false)
    }
    
    func _searchHelper(word: string, index: int, node: TrieNode, changed: bool) -> bool {
        if index == word.length() {
            return changed && node.isEndOfWord
        }
        
        let ch = word[index]
        
        # Try all children
        for (childChar, child) in node.children {
            if childChar == ch {
                # Same character - continue without change
                if this._searchHelper(word, index + 1, child, changed) {
                    return true
                }
            } else if !changed {
                # Different character - use the one change
                if this._searchHelper(word, index + 1, child, true) {
                    return true
                }
            }
        }
        
        return false
    }
}

# Usage
let dict = MagicDictionary()
dict.buildDict(["hello", "leetcode"])

print(dict.search("hello"))   # false (no change needed)
print(dict.search("hhllo"))   # true (change 'e' to 'h')
print(dict.search("hell"))    # false (can't match with one change)
print(dict.search("leetcoded"))  # false (too long)
```

### Problem 10: Stream of Characters

**Problem**: Check if any suffix of query matches a word from the wordlist.

**LeetCode**: #1032

```klang
class StreamChecker {
    var root: TrieNode
    var stream: string
    var maxLength: int
    
    func constructor(words: Array<string>) {
        this.root = TrieNode()
        this.stream = ""
        this.maxLength = 0
        
        # Build trie with reversed words
        for word in words {
            this.maxLength = Math.max(this.maxLength, word.length())
            
            var current = this.root
            
            # Insert word in reverse
            for i in range(word.length() - 1, -1, -1) {
                let ch = word[i]
                
                if !current.children.contains(ch) {
                    current.children[ch] = TrieNode()
                }
                
                current = current.children[ch]
            }
            
            current.isEndOfWord = true
        }
    }
    
    func query(letter: char) -> bool {
        this.stream += letter
        
        # Keep stream size manageable
        if this.stream.length() > this.maxLength {
            this.stream = this.stream.substring(this.stream.length() - this.maxLength)
        }
        
        # Check if any suffix matches
        var current = this.root
        
        for i in range(this.stream.length() - 1, -1, -1) {
            let ch = this.stream[i]
            
            if !current.children.contains(ch) {
                return false
            }
            
            current = current.children[ch]
            
            if current.isEndOfWord {
                return true
            }
        }
        
        return false
    }
}

# Usage
let checker = StreamChecker(["cd", "f", "kl"])
print(checker.query('a'))  # false
print(checker.query('b'))  # false
print(checker.query('c'))  # false
print(checker.query('d'))  # true (matches "cd")
print(checker.query('e'))  # false
print(checker.query('f'))  # true (matches "f")
```

---

## Trie vs Hash Table

### Comparison Table

| Aspect | Trie | Hash Table |
|--------|------|------------|
| **Lookup** | O(m) - m = key length | O(1) average |
| **Insert** | O(m) | O(1) average |
| **Delete** | O(m) | O(1) average |
| **Prefix Search** | O(m) - very efficient | O(n * m) - check all keys |
| **Space** | O(ALPHABET * N * M) | O(N * M) |
| **Ordered Traversal** | Yes - alphabetical | No ordering |
| **Autocomplete** | Excellent | Poor |
| **Memory** | Higher (pointers) | Lower |
| **Collision** | No collisions | Possible collisions |
| **Wildcard Search** | Efficient | Inefficient |

### When to Use Tries

**Use Tries When:**
- Need prefix-based operations (autocomplete, search suggestions)
- Implementing dictionary with pattern matching
- Building spell checkers or word games
- Need lexicographic ordering
- Working with shared prefixes
- IP routing (longest prefix match)

**Example:**
```klang
# Autocomplete - Trie is ideal
trie.getAllWordsWithPrefix("app")  # O(m + k)
# Returns: ["app", "apple", "application", "apply"]

# Hash Table - must check every key
# O(n * m) where n = total keys
```

### When to Use Hash Tables

**Use Hash Tables When:**
- Need O(1) exact key lookups
- No prefix operations required
- Space is limited
- Keys have no hierarchical structure
- Need key-value mapping

**Example:**
```klang
# Exact lookup - Hash Table is faster
hashTable.get("username")  # O(1)

# Trie - O(m) where m = key length
trie.search("username")    # O(8)
```

---

## Space Optimization

### 1. Compressed Trie (Radix Tree)

Merge chains of single-child nodes into single edges.

```
Regular Trie:         Compressed Trie:
    r                     r
    |                     |
    o                    omance
    |                     
    m                    
    |                    
    a                    
    |                    
    n                    
    |                    
    c                    
    |                    
    e                    
```

**Implementation:**

```klang
class RadixNode {
    var children: Map<string, RadixNode>
    var isEndOfWord: bool
    var edge: string
    
    func constructor(edge: string = "") {
        this.children = {}
        this.isEndOfWord = false
        this.edge = edge
    }
}

class RadixTree {
    var root: RadixNode
    
    func constructor() {
        this.root = RadixNode()
    }
    
    func insert(word: string) {
        this._insert(this.root, word, 0)
    }
    
    func _insert(node: RadixNode, word: string, index: int) {
        if index == word.length() {
            node.isEndOfWord = true
            return
        }
        
        let ch = word[index]
        
        # Find matching edge
        for (edgeLabel, child) in node.children {
            if edgeLabel[0] == ch {
                # Find common prefix
                let commonLen = this._commonPrefixLength(
                    edgeLabel, 
                    word.substring(index)
                )
                
                if commonLen == edgeLabel.length() {
                    # Full edge match - recurse
                    this._insert(child, word, index + commonLen)
                    return
                } else {
                    # Split edge
                    let newNode = RadixNode(edgeLabel.substring(0, commonLen))
                    let splitChild = RadixNode(edgeLabel.substring(commonLen))
                    
                    splitChild.children = child.children
                    splitChild.isEndOfWord = child.isEndOfWord
                    
                    newNode.children[edgeLabel.substring(commonLen)] = splitChild
                    node.children.remove(edgeLabel)
                    node.children[edgeLabel.substring(0, commonLen)] = newNode
                    
                    # Insert remaining part of word
                    if commonLen < word.length() - index {
                        this._insert(newNode, word, index + commonLen)
                    } else {
                        newNode.isEndOfWord = true
                    }
                    return
                }
            }
        }
        
        # No matching edge - create new
        node.children[word.substring(index)] = RadixNode(word.substring(index))
        node.children[word.substring(index)].isEndOfWord = true
    }
    
    func _commonPrefixLength(s1: string, s2: string) -> int {
        let minLen = Math.min(s1.length(), s2.length())
        
        for i in range(minLen) {
            if s1[i] != s2[i] {
                return i
            }
        }
        
        return minLen
    }
}
```

### 2. Ternary Search Tree

Each node has 3 children: less, equal, greater.

```klang
class TernaryNode {
    var char: char
    var left: TernaryNode?
    var mid: TernaryNode?
    var right: TernaryNode?
    var isEndOfWord: bool
    
    func constructor(char: char) {
        this.char = char
        this.left = null
        this.mid = null
        this.right = null
        this.isEndOfWord = false
    }
}

class TernarySearchTree {
    var root: TernaryNode?
    
    func constructor() {
        this.root = null
    }
    
    func insert(word: string) {
        this.root = this._insert(this.root, word, 0)
    }
    
    func _insert(node: TernaryNode?, word: string, index: int) -> TernaryNode {
        let ch = word[index]
        
        if node == null {
            node = TernaryNode(ch)
        }
        
        if ch < node.char {
            node.left = this._insert(node.left, word, index)
        } else if ch > node.char {
            node.right = this._insert(node.right, word, index)
        } else {
            if index < word.length() - 1 {
                node.mid = this._insert(node.mid, word, index + 1)
            } else {
                node.isEndOfWord = true
            }
        }
        
        return node
    }
    
    func search(word: string) -> bool {
        return this._search(this.root, word, 0)
    }
    
    func _search(node: TernaryNode?, word: string, index: int) -> bool {
        if node == null {
            return false
        }
        
        let ch = word[index]
        
        if ch < node.char {
            return this._search(node.left, word, index)
        } else if ch > node.char {
            return this._search(node.right, word, index)
        } else {
            if index == word.length() - 1 {
                return node.isEndOfWord
            }
            return this._search(node.mid, word, index + 1)
        }
    }
}
```

### 3. Array vs HashMap Trade-offs

**Small Alphabet (26 letters):**
```klang
# Use fixed array - faster access
var children = Array(26, null)  # 26 pointers per node
```

**Large Alphabet (Unicode):**
```klang
# Use hash map - space efficient
var children = {}  # Only stores existing children
```

**Hybrid Approach:**
```klang
class OptimizedTrieNode {
    var children: any  # Array or Map based on need
    var isEndOfWord: bool
    var childCount: int
    
    func constructor() {
        this.children = null  # Lazy initialization
        this.isEndOfWord = false
        this.childCount = 0
    }
    
    func addChild(ch: char) {
        if this.children == null {
            # First child - use array if small alphabet
            if ch >= 'a' && ch <= 'z' {
                this.children = Array(26, null)
            } else {
                this.children = {}
            }
        }
        
        this.childCount++
        # ... add logic
    }
}
```

---

## Applications

### 1. Autocomplete Systems

**Google Search, IDE code completion**

```klang
# User types "pro"
autocomplete.getSuggestions("pro")
# Returns: ["program", "programming", "project", "promise", "product"]
```

### 2. Spell Checking

**Word processors, browsers**

```klang
# Check if word exists
if !dictionary.search("teh") {
    # Suggest corrections
    suggestions = dictionary.findSimilar("teh")  # ["the", "tea", "ten"]
}
```

### 3. IP Routing (Longest Prefix Match)

**Network routers use tries for routing tables**

```klang
# Find longest matching prefix for IP address
route = routingTable.longestPrefixMatch("192.168.1.100")
```

### 4. T9 Predictive Text

**Old phone keyboards**

```klang
# 4663 → "good", "home", "gone", "hood"
words = t9Dictionary.getWords("4663")
```

### 5. Genome Sequence Analysis

**DNA pattern matching**

```klang
# Find all occurrences of sequence "ACGT"
genome.findSequence("ACGT")
```

### 6. URL Routing in Web Frameworks

**Match URL patterns to handlers**

```klang
router.addRoute("/api/users/:id")
router.addRoute("/api/products/:id")

handler = router.match("/api/users/123")
```

---

## Best Practices

### 1. Choose Right Representation

```klang
# Small alphabet (a-z): Use array
var children = Array(26, null)

# Large alphabet (Unicode): Use hash map
var children = {}

# Mixed: Use lazy initialization
```

### 2. Optimize Memory

```klang
# Use compressed trie for long words
# Use ternary search tree for memory efficiency
# Share common data structures
```

### 3. Handle Edge Cases

```klang
func insert(word: string) {
    if word == null || word.isEmpty() {
        return  # Handle empty strings
    }
    
    # Convert to lowercase for case-insensitive search
    word = word.toLowerCase()
    
    # ... rest of insertion
}
```

### 4. Implement Lazy Deletion

```klang
# Instead of physical deletion, mark as deleted
node.isEndOfWord = false
node.isDeleted = true

# Periodically clean up unused nodes
```

### 5. Cache Frequently Accessed Prefixes

```klang
class CachedTrie {
    var root: TrieNode
    var prefixCache: Map<string, Array<string>>
    
    func getWordsWithPrefix(prefix: string) -> Array<string> {
        if this.prefixCache.contains(prefix) {
            return this.prefixCache[prefix]
        }
        
        let words = this._collectWords(prefix)
        this.prefixCache[prefix] = words
        return words
    }
}
```

### 6. Use Iterative Approach for Deep Tries

```klang
# Avoid stack overflow with deep recursion
func insertIterative(word: string) {
    var current = this.root
    
    for ch in word {
        if !current.children.contains(ch) {
            current.children[ch] = TrieNode()
        }
        current = current.children[ch]
    }
    
    current.isEndOfWord = true
}
```

### 7. Implement Serialization

```klang
func serialize() -> string {
    var result = []
    this._serializeHelper(this.root, "", result)
    return result.join(",")
}

func _serializeHelper(node: TrieNode, word: string, result: Array<string>) {
    if node.isEndOfWord {
        result.push(word)
    }
    
    for (ch, child) in node.children {
        this._serializeHelper(child, word + ch, result)
    }
}

func deserialize(data: string) {
    let words = data.split(",")
    for word in words {
        this.insert(word)
    }
}
```

---

## Complete Examples

### Complete Trie Implementation

```klang
class TrieNode {
    var children: Map<char, TrieNode>
    var isEndOfWord: bool
    var word: string?
    var count: int
    
    func constructor() {
        this.children = {}
        this.isEndOfWord = false
        this.word = null
        this.count = 0
    }
}

class Trie {
    var root: TrieNode
    var wordCount: int
    
    func constructor() {
        this.root = TrieNode()
        this.wordCount = 0
    }
    
    # Insert word into trie
    func insert(word: string) {
        if word.isEmpty() {
            return
        }
        
        var current = this.root
        
        for i in range(word.length()) {
            let ch = word[i]
            
            if !current.children.contains(ch) {
                current.children[ch] = TrieNode()
            }
            
            current = current.children[ch]
        }
        
        if !current.isEndOfWord {
            this.wordCount++
        }
        
        current.isEndOfWord = true
        current.word = word
        current.count++
    }
    
    # Search for exact word
    func search(word: string) -> bool {
        let node = this._findNode(word)
        return node != null && node.isEndOfWord
    }
    
    # Check if any word starts with prefix
    func startsWith(prefix: string) -> bool {
        return this._findNode(prefix) != null
    }
    
    # Delete word from trie
    func delete(word: string) -> bool {
        return this._deleteHelper(this.root, word, 0)
    }
    
    func _deleteHelper(node: TrieNode?, word: string, index: int) -> bool {
        if node == null {
            return false
        }
        
        if index == word.length() {
            if !node.isEndOfWord {
                return false
            }
            
            node.isEndOfWord = false
            node.count = 0
            this.wordCount--
            
            return node.children.isEmpty()
        }
        
        let ch = word[index]
        let child = node.children.get(ch)
        
        let shouldDeleteChild = this._deleteHelper(child, word, index + 1)
        
        if shouldDeleteChild {
            node.children.remove(ch)
            return !node.isEndOfWord && node.children.isEmpty()
        }
        
        return false
    }
    
    # Get all words with given prefix
    func getAllWordsWithPrefix(prefix: string) -> Array<string> {
        var result = []
        let node = this._findNode(prefix)
        
        if node != null {
            this._collectWords(node, prefix, result)
        }
        
        return result
    }
    
    # Get count of words with prefix
    func countWordsWithPrefix(prefix: string) -> int {
        let node = this._findNode(prefix)
        
        if node == null {
            return 0
        }
        
        return this._countWords(node)
    }
    
    # Get longest common prefix of all words
    func longestCommonPrefix() -> string {
        var prefix = ""
        var current = this.root
        
        while current.children.size() == 1 && !current.isEndOfWord {
            let (ch, child) = current.children.entries()[0]
            prefix += ch
            current = child
        }
        
        return prefix
    }
    
    # Get all words in alphabetical order
    func getAllWords() -> Array<string> {
        var result = []
        this._collectWords(this.root, "", result)
        return result
    }
    
    # Check if word can be formed by concatenating other words
    func isComposed(word: string) -> bool {
        if word.isEmpty() {
            return false
        }
        
        let n = word.length()
        var dp = Array(n + 1, false)
        dp[0] = true
        
        for i in range(1, n + 1) {
            for j in range(i) {
                let substring = word.substring(j, i)
                if dp[j] && this.search(substring) && substring != word {
                    dp[i] = true
                    break
                }
            }
        }
        
        return dp[n]
    }
    
    # Helper: Find node at end of prefix
    func _findNode(prefix: string) -> TrieNode? {
        var current = this.root
        
        for i in range(prefix.length()) {
            let ch = prefix[i]
            
            if !current.children.contains(ch) {
                return null
            }
            
            current = current.children[ch]
        }
        
        return current
    }
    
    # Helper: Collect all words from node
    func _collectWords(node: TrieNode, prefix: string, result: Array<string>) {
        if node.isEndOfWord {
            result.push(prefix)
        }
        
        # Collect in alphabetical order
        let chars = node.children.keys().sort()
        
        for ch in chars {
            this._collectWords(node.children[ch], prefix + ch, result)
        }
    }
    
    # Helper: Count words from node
    func _countWords(node: TrieNode?) -> int {
        if node == null {
            return 0
        }
        
        var count = node.isEndOfWord ? 1 : 0
        
        for child in node.children.values() {
            count += this._countWords(child)
        }
        
        return count
    }
    
    # Get total number of words
    func size() -> int {
        return this.wordCount
    }
    
    # Check if trie is empty
    func isEmpty() -> bool {
        return this.wordCount == 0
    }
    
    # Clear all words
    func clear() {
        this.root = TrieNode()
        this.wordCount = 0
    }
}

# Usage Example
let trie = Trie()

# Insert words
trie.insert("apple")
trie.insert("application")
trie.insert("apply")
trie.insert("app")
trie.insert("banana")
trie.insert("band")

# Search
print(trie.search("apple"))          # true
print(trie.search("appl"))           # false
print(trie.startsWith("app"))        # true

# Get words with prefix
print(trie.getAllWordsWithPrefix("app"))
# Output: ["app", "apple", "application", "apply"]

print(trie.countWordsWithPrefix("ban"))  # 2

# Get all words
print(trie.getAllWords())
# Output: ["app", "apple", "application", "apply", "banana", "band"]

# Delete
trie.delete("apple")
print(trie.search("apple"))          # false
print(trie.search("application"))    # true

# Size
print(trie.size())                   # 5
```

---

## Summary

**Tries (Prefix Trees)** are specialized tree structures optimized for string operations:

### Key Takeaways

1. **Structure**: Tree where each node represents a character
2. **Operations**: Insert, Search, StartsWith all O(m) where m = word length
3. **Strengths**: Excellent for prefix-based operations and autocomplete
4. **Use Cases**: Dictionaries, autocomplete, spell checking, IP routing
5. **Trade-offs**: Higher memory usage but faster prefix operations vs hash tables
6. **Optimization**: Compressed tries, ternary search trees for space efficiency

### When to Use

- ✅ Prefix-based searches
- ✅ Autocomplete systems
- ✅ Dictionary implementations
- ✅ Pattern matching with wildcards
- ✅ Lexicographic ordering needed

### When Not to Use

- ❌ Simple key-value lookups (use hash table)
- ❌ Limited memory (high space overhead)
- ❌ No prefix operations needed
- ❌ Few shared prefixes

**Next Steps**: Practice implementing tries and solving the interview problems!
