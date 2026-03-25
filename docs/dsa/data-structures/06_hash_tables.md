# Hash Tables in KLang - Data Structures & Algorithms

## Table of Contents
1. [Introduction](#introduction)
2. [What is a Hash Table?](#what-is-a-hash-table)
3. [Hash Functions](#hash-functions)
4. [Collision Handling](#collision-handling)
5. [Hash Table Operations](#hash-table-operations)
6. [Load Factor and Resizing](#load-factor-and-resizing)
7. [Time and Space Complexity](#time-and-space-complexity)
8. [Hash Table Algorithms](#hash-table-algorithms)
9. [Practical Examples](#practical-examples)
10. [Interview Problems](#interview-problems)
11. [Best Practices](#best-practices)
12. [Complete Examples](#complete-examples)

---

## Introduction

A **hash table** (also known as hash map) is a data structure that implements an associative array abstract data type, mapping keys to values. It uses a **hash function** to compute an index into an array of buckets or slots, from which the desired value can be found.

### Why Use Hash Tables?

- **Fast Lookups**: Average O(1) time for search, insert, and delete
- **Key-Value Mapping**: Direct association between keys and values
- **Flexible Keys**: Can use strings, numbers, or custom objects as keys
- **Efficient Memory**: Only stores actual data, not entire key space
- **Real-World Applications**: Databases, caches, dictionaries, symbol tables

### Real-World Use Cases

- **Databases**: Index structures for fast record retrieval
- **Caching**: LRU cache, memoization
- **Compilers**: Symbol tables for variable/function names
- **Routing**: URL to handler mapping in web servers
- **Deduplication**: Finding duplicates in large datasets

---

## What is a Hash Table?

A **hash table** stores data as key-value pairs using a hash function to determine where to store each entry.

### Key-Value Pairs

Each entry in a hash table consists of:
- **Key**: Unique identifier used to access the value
- **Value**: The data associated with the key

```klang
# Example key-value pairs
hash_table["name"] = "Alice"      # Key: "name", Value: "Alice"
hash_table["age"] = 30            # Key: "age", Value: 30
hash_table["email"] = "alice@example.com"
```

### How Hash Tables Work

```
1. Hash Function: key → hash code
2. Compression: hash code → index (0 to table_size-1)
3. Storage: store value at computed index

Example:
Key "apple" → hash("apple") = 92834765
           → 92834765 % 10 = 5
           → Store at index 5
```

### Visual Representation

```
Hash Table with 10 slots:

Index    Key        Value       Hash
  0   → ["cat"]  → "meow"      h("cat") % 10 = 0
  1   → empty
  2   → ["dog"]  → "woof"      h("dog") % 10 = 2
  3   → empty
  4   → ["bird"] → "tweet"     h("bird") % 10 = 4
  5   → ["fish"] → "blub"      h("fish") % 10 = 5
  6   → empty
  7   → empty
  8   → empty
  9   → empty
```

### Key Terminology

- **Hash Function**: Converts keys to array indices
- **Bucket/Slot**: Storage location in the array
- **Collision**: When two keys hash to the same index
- **Load Factor**: Ratio of entries to table size
- **Rehashing**: Resizing and rebuilding the hash table

---

## Hash Functions

A **hash function** converts keys into array indices. A good hash function should:
- Be **deterministic**: same key always produces same hash
- Be **uniform**: distribute keys evenly across the table
- Be **fast**: O(1) time complexity
- Minimize **collisions**: different keys should produce different hashes

### Simple Hash Functions

#### 1. Division Method

```klang
fn hashDivision(key, tableSize) {
    # Convert key to integer if needed
    let code = 0
    let i = 0
    while (i < key.length()) {
        code = code + key.charCodeAt(i)
        i = i + 1
    }
    return code % tableSize
}

# Example
print(hashDivision("apple", 10))  # Returns index 0-9
```

**Pros**: Simple and fast  
**Cons**: Table size should be prime to reduce collisions

#### 2. Multiplication Method

```klang
fn hashMultiplication(key, tableSize) {
    let A = 0.6180339887  # Golden ratio constant
    let code = 0
    let i = 0
    
    while (i < key.length()) {
        code = code + key.charCodeAt(i)
        i = i + 1
    }
    
    let fractional = (code * A) - Math.floor(code * A)
    return Math.floor(tableSize * fractional)
}
```

**Pros**: Table size doesn't need to be prime  
**Cons**: Slightly more complex

#### 3. Polynomial Rolling Hash (Better for Strings)

```klang
fn hashPolynomial(key, tableSize) {
    let hash = 0
    let prime = 31  # Common choice: 31, 37, 41
    let i = 0
    
    while (i < key.length()) {
        hash = (hash * prime + key.charCodeAt(i)) % tableSize
        i = i + 1
    }
    
    return hash
}

# Example
print(hashPolynomial("hello", 100))
```

**Pros**: Better distribution, fewer collisions  
**Cons**: More computation

### Integer Hash Function

```klang
fn hashInteger(key, tableSize) {
    # For integer keys
    let hash = key
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b
    hash = (hash >> 16) ^ hash
    return hash % tableSize
}
```

---

## Collision Handling

When two keys hash to the same index, we have a **collision**. Several strategies exist to handle this.

### 1. Chaining (Separate Chaining)

Each bucket contains a linked list (or array) of all entries that hash to that index.

```klang
# Hash table with chaining
class Node {
    init(key, value) {
        this.key = key
        this.value = value
        this.next = null
    }
}

class HashTableChaining {
    init(size) {
        this.size = size
        this.table = []
        let i = 0
        while (i < size) {
            this.table.push(null)
            i = i + 1
        }
        this.count = 0
    }
    
    hash(key) {
        let hash = 0
        let i = 0
        while (i < key.length()) {
            hash = (hash * 31 + key.charCodeAt(i)) % this.size
            i = i + 1
        }
        return hash
    }
    
    insert(key, value) {
        let index = this.hash(key)
        let node = this.table[index]
        
        # Check if key already exists
        while (node != null) {
            if (node.key == key) {
                node.value = value  # Update existing
                return
            }
            node = node.next
        }
        
        # Insert new node at head
        let newNode = Node(key, value)
        newNode.next = this.table[index]
        this.table[index] = newNode
        this.count = this.count + 1
    }
    
    search(key) {
        let index = this.hash(key)
        let node = this.table[index]
        
        while (node != null) {
            if (node.key == key) {
                return node.value
            }
            node = node.next
        }
        return null
    }
    
    delete(key) {
        let index = this.hash(key)
        let node = this.table[index]
        let prev = null
        
        while (node != null) {
            if (node.key == key) {
                if (prev == null) {
                    this.table[index] = node.next
                } else {
                    prev.next = node.next
                }
                this.count = this.count - 1
                return true
            }
            prev = node
            node = node.next
        }
        return false
    }
}

# Usage
let ht = HashTableChaining(10)
ht.insert("apple", 100)
ht.insert("banana", 200)
print(ht.search("apple"))   # 100
ht.delete("apple")
print(ht.search("apple"))   # null
```

**Advantages**:
- Simple implementation
- Never "fills up"
- Good performance with good hash function

**Disadvantages**:
- Extra memory for pointers
- Cache performance (non-contiguous memory)
- Degrades to O(n) with many collisions

### 2. Open Addressing - Linear Probing

Store all entries in the array itself. When collision occurs, search for next empty slot.

```klang
class HashTableLinearProbing {
    init(size) {
        this.size = size
        this.keys = []
        this.values = []
        let i = 0
        while (i < size) {
            this.keys.push(null)
            this.values.push(null)
            i = i + 1
        }
        this.count = 0
    }
    
    hash(key) {
        let hash = 0
        let i = 0
        while (i < key.length()) {
            hash = (hash * 31 + key.charCodeAt(i)) % this.size
            i = i + 1
        }
        return hash
    }
    
    insert(key, value) {
        if (this.count >= this.size) {
            print("Hash table is full!")
            return false
        }
        
        let index = this.hash(key)
        let i = 0
        
        # Linear probing: check index, index+1, index+2, ...
        while (i < this.size) {
            let probeIndex = (index + i) % this.size
            
            # Empty slot or same key (update)
            if (this.keys[probeIndex] == null || this.keys[probeIndex] == key) {
                if (this.keys[probeIndex] == null) {
                    this.count = this.count + 1
                }
                this.keys[probeIndex] = key
                this.values[probeIndex] = value
                return true
            }
            
            i = i + 1
        }
        return false
    }
    
    search(key) {
        let index = this.hash(key)
        let i = 0
        
        while (i < this.size) {
            let probeIndex = (index + i) % this.size
            
            if (this.keys[probeIndex] == null) {
                return null  # Key not found
            }
            if (this.keys[probeIndex] == key) {
                return this.values[probeIndex]
            }
            
            i = i + 1
        }
        return null
    }
    
    delete(key) {
        let index = this.hash(key)
        let i = 0
        
        while (i < this.size) {
            let probeIndex = (index + i) % this.size
            
            if (this.keys[probeIndex] == null) {
                return false
            }
            if (this.keys[probeIndex] == key) {
                this.keys[probeIndex] = null
                this.values[probeIndex] = null
                this.count = this.count - 1
                
                # Rehash subsequent entries
                this.rehashCluster(probeIndex)
                return true
            }
            
            i = i + 1
        }
        return false
    }
    
    rehashCluster(startIndex) {
        let i = (startIndex + 1) % this.size
        
        while (this.keys[i] != null) {
            let key = this.keys[i]
            let value = this.values[i]
            
            this.keys[i] = null
            this.values[i] = null
            this.count = this.count - 1
            
            this.insert(key, value)
            i = (i + 1) % this.size
        }
    }
}

# Usage
let ht = HashTableLinearProbing(10)
ht.insert("apple", 100)
ht.insert("apricot", 150)  # May collide with apple
print(ht.search("apple"))   # 100
```

**Advantages**:
- No extra memory for pointers
- Better cache performance
- Simple implementation

**Disadvantages**:
- Clustering (groups of consecutive filled slots)
- Performance degrades as load factor increases
- Deletion is complex

### 3. Open Addressing - Quadratic Probing

Instead of checking consecutive slots, use quadratic function.

```klang
fn insertQuadratic(ht, key, value) {
    let index = hash(key, ht.size)
    let i = 0
    
    while (i < ht.size) {
        let probeIndex = (index + i * i) % ht.size  # i^2 probing
        
        if (ht.keys[probeIndex] == null) {
            ht.keys[probeIndex] = key
            ht.values[probeIndex] = value
            return true
        }
        
        i = i + 1
    }
    return false
}
```

**Formula**: `h(k, i) = (h(k) + i²) % table_size`

**Advantages**:
- Reduces primary clustering
- Better distribution than linear probing

**Disadvantages**:
- Secondary clustering can occur
- May not probe all slots

### 4. Double Hashing

Use a second hash function for probing interval.

```klang
fn hash1(key, size) {
    let hash = 0
    let i = 0
    while (i < key.length()) {
        hash = (hash * 31 + key.charCodeAt(i)) % size
        i = i + 1
    }
    return hash
}

fn hash2(key, size) {
    let hash = 0
    let i = 0
    while (i < key.length()) {
        hash = (hash * 37 + key.charCodeAt(i)) % (size - 1)
        i = i + 1
    }
    return hash + 1  # Ensure non-zero
}

fn insertDoubleHash(ht, key, value) {
    let index = hash1(key, ht.size)
    let step = hash2(key, ht.size)
    let i = 0
    
    while (i < ht.size) {
        let probeIndex = (index + i * step) % ht.size
        
        if (ht.keys[probeIndex] == null) {
            ht.keys[probeIndex] = key
            ht.values[probeIndex] = value
            return true
        }
        
        i = i + 1
    }
    return false
}
```

**Formula**: `h(k, i) = (h1(k) + i * h2(k)) % table_size`

**Advantages**:
- Best open addressing method
- Minimizes clustering
- Uniform probe sequence

**Disadvantages**:
- More complex
- Requires good secondary hash function

---

## Hash Table Operations

### Core Operations

#### 1. Insert/Put - Add Key-Value Pair

```klang
fn put(hashTable, key, value) {
    let index = hash(key, hashTable.size)
    hashTable.insert(index, key, value)
}
```

**Time Complexity**: O(1) average, O(n) worst case  
**Space Complexity**: O(1)

#### 2. Search/Get - Retrieve Value

```klang
fn get(hashTable, key) {
    let index = hash(key, hashTable.size)
    return hashTable.search(index, key)
}
```

**Time Complexity**: O(1) average, O(n) worst case  
**Space Complexity**: O(1)

#### 3. Delete/Remove - Remove Key-Value Pair

```klang
fn remove(hashTable, key) {
    let index = hash(key, hashTable.size)
    return hashTable.delete(index, key)
}
```

**Time Complexity**: O(1) average, O(n) worst case  
**Space Complexity**: O(1)

#### 4. Contains - Check Key Existence

```klang
fn contains(hashTable, key) {
    return get(hashTable, key) != null
}
```

**Time Complexity**: O(1) average, O(n) worst case  
**Space Complexity**: O(1)

#### 5. Size - Get Number of Entries

```klang
fn size(hashTable) {
    return hashTable.count
}
```

**Time Complexity**: O(1)  
**Space Complexity**: O(1)

#### 6. Keys - Get All Keys

```klang
fn keys(hashTable) {
    let keyList = []
    let i = 0
    
    while (i < hashTable.size) {
        let node = hashTable.table[i]
        while (node != null) {
            keyList.push(node.key)
            node = node.next
        }
        i = i + 1
    }
    
    return keyList
}
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

#### 7. Values - Get All Values

```klang
fn values(hashTable) {
    let valueList = []
    let i = 0
    
    while (i < hashTable.size) {
        let node = hashTable.table[i]
        while (node != null) {
            valueList.push(node.value)
            node = node.next
        }
        i = i + 1
    }
    
    return valueList
}
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

---

## Load Factor and Resizing

The **load factor** (α) measures how full the hash table is:

```
Load Factor (α) = Number of entries / Table size

Example: 7 entries in table of size 10
α = 7 / 10 = 0.7 (70% full)
```

### When to Resize

- **Chaining**: Resize when α > 0.75 (75% full)
- **Open Addressing**: Resize when α > 0.5 (50% full)

### Resizing Implementation

```klang
class DynamicHashTable {
    init() {
        this.size = 10
        this.count = 0
        this.table = []
        let i = 0
        while (i < this.size) {
            this.table.push(null)
            i = i + 1
        }
    }
    
    loadFactor() {
        return this.count / this.size
    }
    
    shouldResize() {
        return this.loadFactor() > 0.75
    }
    
    resize() {
        print("Resizing from " + this.size + " to " + (this.size * 2))
        
        let oldTable = this.table
        let oldSize = this.size
        
        # Double the size
        this.size = this.size * 2
        this.count = 0
        this.table = []
        
        let i = 0
        while (i < this.size) {
            this.table.push(null)
            i = i + 1
        }
        
        # Rehash all entries
        i = 0
        while (i < oldSize) {
            let node = oldTable[i]
            while (node != null) {
                this.insert(node.key, node.value)
                node = node.next
            }
            i = i + 1
        }
    }
    
    insert(key, value) {
        if (this.shouldResize()) {
            this.resize()
        }
        
        let index = this.hash(key)
        let node = this.table[index]
        
        # Check if key exists
        while (node != null) {
            if (node.key == key) {
                node.value = value
                return
            }
            node = node.next
        }
        
        # Insert new node
        let newNode = Node(key, value)
        newNode.next = this.table[index]
        this.table[index] = newNode
        this.count = this.count + 1
    }
    
    hash(key) {
        let hash = 0
        let i = 0
        while (i < key.length()) {
            hash = (hash * 31 + key.charCodeAt(i)) % this.size
            i = i + 1
        }
        return hash
    }
}

# Usage
let ht = DynamicHashTable()
let i = 0
while (i < 20) {
    ht.insert("key" + i, i * 10)
    print("Load factor: " + ht.loadFactor())
    i = i + 1
}
```

**Resizing Time Complexity**: O(n) - need to rehash all entries  
**Amortized Insert**: O(1) - resizing happens rarely

---

## Time and Space Complexity

### Operation Complexities

| Operation | Average Case | Worst Case | Space |
|-----------|-------------|------------|-------|
| Insert    | O(1)        | O(n)       | O(1)  |
| Search    | O(1)        | O(n)       | O(1)  |
| Delete    | O(1)        | O(n)       | O(1)  |
| Contains  | O(1)        | O(n)       | O(1)  |
| Keys      | O(n)        | O(n)       | O(n)  |
| Values    | O(n)        | O(n)       | O(n)  |

### Space Complexity

- **Chaining**: O(n + m) where n = entries, m = table size
- **Open Addressing**: O(m) where m = table size
- **Overall**: O(n) for n entries

### Performance Factors

1. **Hash Function Quality**: Good distribution → fewer collisions
2. **Load Factor**: Lower load → better performance
3. **Collision Resolution**: Chaining vs. open addressing trade-offs
4. **Table Size**: Prime numbers reduce collisions

---

## Hash Table Algorithms

### 1. Find Duplicates

Find all duplicate elements in an array.

```klang
fn findDuplicates(arr) {
    let seen = {}
    let duplicates = []
    let i = 0
    
    while (i < arr.length()) {
        if (seen[arr[i]]) {
            # Already seen, it's a duplicate
            if (!seen[arr[i] + "_dup"]) {
                duplicates.push(arr[i])
                seen[arr[i] + "_dup"] = true
            }
        } else {
            seen[arr[i]] = true
        }
        i = i + 1
    }
    
    return duplicates
}

# Test
let arr = [1, 2, 3, 2, 4, 5, 3, 6]
print(findDuplicates(arr))  # [2, 3]
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

### 2. Group Anagrams

Group strings that are anagrams of each other.

```klang
fn sortString(str) {
    # Simple bubble sort for string characters
    let chars = []
    let i = 0
    while (i < str.length()) {
        chars.push(str[i])
        i = i + 1
    }
    
    i = 0
    while (i < chars.length()) {
        let j = 0
        while (j < chars.length() - i - 1) {
            if (chars[j] > chars[j + 1]) {
                let temp = chars[j]
                chars[j] = chars[j + 1]
                chars[j + 1] = temp
            }
            j = j + 1
        }
        i = i + 1
    }
    
    let result = ""
    i = 0
    while (i < chars.length()) {
        result = result + chars[i]
        i = i + 1
    }
    return result
}

fn groupAnagrams(strs) {
    let groups = {}
    let i = 0
    
    while (i < strs.length()) {
        let sorted = sortString(strs[i])
        
        if (!groups[sorted]) {
            groups[sorted] = []
        }
        groups[sorted].push(strs[i])
        
        i = i + 1
    }
    
    # Convert to array of groups
    let result = []
    let keys = Object.keys(groups)
    i = 0
    while (i < keys.length()) {
        result.push(groups[keys[i]])
        i = i + 1
    }
    
    return result
}

# Test
let words = ["eat", "tea", "tan", "ate", "nat", "bat"]
print(groupAnagrams(words))
# [["eat", "tea", "ate"], ["tan", "nat"], ["bat"]]
```

**Time Complexity**: O(n * k log k) where n = number of strings, k = max string length  
**Space Complexity**: O(n * k)

### 3. First Non-Repeating Character

Find the first character that appears only once.

```klang
fn firstNonRepeating(str) {
    let freq = {}
    let i = 0
    
    # Count frequencies
    while (i < str.length()) {
        let char = str[i]
        if (freq[char]) {
            freq[char] = freq[char] + 1
        } else {
            freq[char] = 1
        }
        i = i + 1
    }
    
    # Find first with frequency 1
    i = 0
    while (i < str.length()) {
        if (freq[str[i]] == 1) {
            return i
        }
        i = i + 1
    }
    
    return -1
}

# Test
print(firstNonRepeating("leetcode"))     # 0 ('l')
print(firstNonRepeating("loveleetcode")) # 2 ('v')
print(firstNonRepeating("aabb"))         # -1
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1) - at most 26 lowercase letters

### 4. Subarray Sum Equals K

Count subarrays with sum equal to k.

```klang
fn subarraySum(nums, k) {
    let count = 0
    let sum = 0
    let sumFreq = {}
    sumFreq[0] = 1  # Base case: empty subarray
    
    let i = 0
    while (i < nums.length()) {
        sum = sum + nums[i]
        
        # Check if (sum - k) exists
        let complement = sum - k
        if (sumFreq[complement]) {
            count = count + sumFreq[complement]
        }
        
        # Add current sum to frequency map
        if (sumFreq[sum]) {
            sumFreq[sum] = sumFreq[sum] + 1
        } else {
            sumFreq[sum] = 1
        }
        
        i = i + 1
    }
    
    return count
}

# Test
print(subarraySum([1, 1, 1], 2))        # 2
print(subarraySum([1, 2, 3], 3))        # 2
print(subarraySum([1, -1, 0], 0))       # 3
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

### 5. Longest Consecutive Sequence

Find length of longest consecutive elements sequence.

```klang
fn longestConsecutive(nums) {
    if (nums.length() == 0) {
        return 0
    }
    
    let numSet = {}
    let i = 0
    
    # Add all numbers to hash set
    while (i < nums.length()) {
        numSet[nums[i]] = true
        i = i + 1
    }
    
    let longest = 0
    i = 0
    
    while (i < nums.length()) {
        let num = nums[i]
        
        # Only start counting if it's the beginning of a sequence
        if (!numSet[num - 1]) {
            let currentNum = num
            let currentLength = 1
            
            while (numSet[currentNum + 1]) {
                currentNum = currentNum + 1
                currentLength = currentLength + 1
            }
            
            if (currentLength > longest) {
                longest = currentLength
            }
        }
        
        i = i + 1
    }
    
    return longest
}

# Test
print(longestConsecutive([100, 4, 200, 1, 3, 2]))  # 4 ([1,2,3,4])
print(longestConsecutive([0, 3, 7, 2, 5, 8, 4, 6, 0, 1]))  # 9
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

---

## Practical Examples

### 1. Dictionary Implementation

Simple dictionary with string keys.

```klang
class Dictionary {
    init() {
        this.size = 50
        this.table = []
        let i = 0
        while (i < this.size) {
            this.table.push(null)
            i = i + 1
        }
    }
    
    hash(key) {
        let hash = 0
        let i = 0
        while (i < key.length()) {
            hash = (hash * 31 + key.charCodeAt(i)) % this.size
            i = i + 1
        }
        return hash
    }
    
    define(word, definition) {
        let index = this.hash(word)
        let entry = Node(word, definition)
        entry.next = this.table[index]
        this.table[index] = entry
    }
    
    lookup(word) {
        let index = this.hash(word)
        let node = this.table[index]
        
        while (node != null) {
            if (node.key == word) {
                return node.value
            }
            node = node.next
        }
        return "Word not found"
    }
}

# Usage
let dict = Dictionary()
dict.define("algorithm", "Step-by-step procedure for solving a problem")
dict.define("hash", "Convert data to fixed-size value")
dict.define("collision", "When two keys map to same index")

print(dict.lookup("algorithm"))
print(dict.lookup("hash"))
print(dict.lookup("unknown"))
```

### 2. LRU Cache with Hash Table

Least Recently Used cache using hash table + doubly linked list.

```klang
class DLLNode {
    init(key, value) {
        this.key = key
        this.value = value
        this.prev = null
        this.next = null
    }
}

class LRUCache {
    init(capacity) {
        this.capacity = capacity
        this.cache = {}  # Hash table: key -> node
        this.size = 0
        
        # Dummy head and tail
        this.head = DLLNode(0, 0)
        this.tail = DLLNode(0, 0)
        this.head.next = this.tail
        this.tail.prev = this.head
    }
    
    removeNode(node) {
        node.prev.next = node.next
        node.next.prev = node.prev
    }
    
    addToHead(node) {
        node.next = this.head.next
        node.prev = this.head
        this.head.next.prev = node
        this.head.next = node
    }
    
    moveToHead(node) {
        this.removeNode(node)
        this.addToHead(node)
    }
    
    removeTail() {
        let node = this.tail.prev
        this.removeNode(node)
        return node
    }
    
    get(key) {
        if (!this.cache[key]) {
            return -1
        }
        
        let node = this.cache[key]
        this.moveToHead(node)  # Mark as recently used
        return node.value
    }
    
    put(key, value) {
        if (this.cache[key]) {
            # Update existing
            let node = this.cache[key]
            node.value = value
            this.moveToHead(node)
        } else {
            # Add new
            let node = DLLNode(key, value)
            this.cache[key] = node
            this.addToHead(node)
            this.size = this.size + 1
            
            if (this.size > this.capacity) {
                # Remove least recently used
                let removed = this.removeTail()
                delete this.cache[removed.key]
                this.size = this.size - 1
            }
        }
    }
}

# Usage
let cache = LRUCache(2)
cache.put("a", 1)
cache.put("b", 2)
print(cache.get("a"))     # 1
cache.put("c", 3)         # Evicts key "b"
print(cache.get("b"))     # -1 (not found)
cache.put("d", 4)         # Evicts key "a"
print(cache.get("a"))     # -1 (not found)
print(cache.get("c"))     # 3
print(cache.get("d"))     # 4
```

**Time Complexity**: O(1) for get and put  
**Space Complexity**: O(capacity)

### 3. Frequency Counter

Count word frequencies in text.

```klang
fn wordFrequency(text) {
    let freq = {}
    let words = text.split(" ")
    let i = 0
    
    while (i < words.length()) {
        let word = words[i].toLowerCase()
        
        if (freq[word]) {
            freq[word] = freq[word] + 1
        } else {
            freq[word] = 1
        }
        
        i = i + 1
    }
    
    return freq
}

fn topKFrequent(freq, k) {
    # Convert to array of [word, count] pairs
    let items = []
    let keys = Object.keys(freq)
    let i = 0
    
    while (i < keys.length()) {
        items.push([keys[i], freq[keys[i]]])
        i = i + 1
    }
    
    # Sort by frequency (descending)
    i = 0
    while (i < items.length()) {
        let j = 0
        while (j < items.length() - i - 1) {
            if (items[j][1] < items[j + 1][1]) {
                let temp = items[j]
                items[j] = items[j + 1]
                items[j + 1] = temp
            }
            j = j + 1
        }
        i = i + 1
    }
    
    # Return top k
    let result = []
    i = 0
    while (i < k && i < items.length()) {
        result.push(items[i])
        i = i + 1
    }
    
    return result
}

# Usage
let text = "the quick brown fox jumps over the lazy dog the fox"
let freq = wordFrequency(text)
print("All frequencies:", freq)
print("Top 3:", topKFrequent(freq, 3))
```

---

## Interview Problems

### Problem 1: Two Sum

Find two numbers that add up to target.

```klang
fn twoSum(nums, target) {
    let seen = {}
    let i = 0
    
    while (i < nums.length()) {
        let complement = target - nums[i]
        
        if (seen[complement] != null) {
            return [seen[complement], i]
        }
        
        seen[nums[i]] = i
        i = i + 1
    }
    
    return []
}

# Test
print(twoSum([2, 7, 11, 15], 9))       # [0, 1]
print(twoSum([3, 2, 4], 6))            # [1, 2]
print(twoSum([3, 3], 6))               # [0, 1]
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

**Explanation**: Use hash table to store seen numbers. For each number, check if (target - number) exists in the table.

### Problem 2: Valid Anagram

Check if two strings are anagrams.

```klang
fn isAnagram(s, t) {
    if (s.length() != t.length()) {
        return false
    }
    
    let count = {}
    let i = 0
    
    # Count characters in s
    while (i < s.length()) {
        if (count[s[i]]) {
            count[s[i]] = count[s[i]] + 1
        } else {
            count[s[i]] = 1
        }
        i = i + 1
    }
    
    # Subtract characters in t
    i = 0
    while (i < t.length()) {
        if (!count[t[i]]) {
            return false
        }
        count[t[i]] = count[t[i]] - 1
        if (count[t[i]] < 0) {
            return false
        }
        i = i + 1
    }
    
    return true
}

# Test
print(isAnagram("anagram", "nagaram"))  # true
print(isAnagram("rat", "car"))          # false
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1) - at most 26 letters

### Problem 3: Group Anagrams

Group strings that are anagrams (shown in algorithms section).

### Problem 4: Longest Substring Without Repeating Characters

Find length of longest substring without repeating characters.

```klang
fn lengthOfLongestSubstring(s) {
    let seen = {}
    let maxLen = 0
    let start = 0
    let i = 0
    
    while (i < s.length()) {
        let char = s[i]
        
        # If character seen and within current window
        if (seen[char] != null && seen[char] >= start) {
            start = seen[char] + 1
        }
        
        seen[char] = i
        let currentLen = i - start + 1
        if (currentLen > maxLen) {
            maxLen = currentLen
        }
        
        i = i + 1
    }
    
    return maxLen
}

# Test
print(lengthOfLongestSubstring("abcabcbb"))   # 3 ("abc")
print(lengthOfLongestSubstring("bbbbb"))      # 1 ("b")
print(lengthOfLongestSubstring("pwwkew"))     # 3 ("wke")
```

**Time Complexity**: O(n)  
**Space Complexity**: O(min(n, m)) where m is charset size

**Explanation**: Use sliding window with hash table. Store last seen index of each character.

### Problem 5: Contains Duplicate

Check if array contains any duplicates.

```klang
fn containsDuplicate(nums) {
    let seen = {}
    let i = 0
    
    while (i < nums.length()) {
        if (seen[nums[i]]) {
            return true
        }
        seen[nums[i]] = true
        i = i + 1
    }
    
    return false
}

# Test
print(containsDuplicate([1, 2, 3, 1]))        # true
print(containsDuplicate([1, 2, 3, 4]))        # false
print(containsDuplicate([1, 1, 1, 3, 3, 4]))  # true
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

### Problem 6: First Unique Character

Find index of first non-repeating character.

```klang
fn firstUniqChar(s) {
    let freq = {}
    let i = 0
    
    # Count all characters
    while (i < s.length()) {
        if (freq[s[i]]) {
            freq[s[i]] = freq[s[i]] + 1
        } else {
            freq[s[i]] = 1
        }
        i = i + 1
    }
    
    # Find first with count 1
    i = 0
    while (i < s.length()) {
        if (freq[s[i]] == 1) {
            return i
        }
        i = i + 1
    }
    
    return -1
}

# Test
print(firstUniqChar("leetcode"))      # 0
print(firstUniqChar("loveleetcode"))  # 2
print(firstUniqChar("aabb"))          # -1
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1) - constant alphabet size

### Problem 7: Intersection of Two Arrays

Find intersection of two arrays.

```klang
fn intersection(nums1, nums2) {
    let set1 = {}
    let result = []
    let i = 0
    
    # Add all from nums1 to set
    while (i < nums1.length()) {
        set1[nums1[i]] = true
        i = i + 1
    }
    
    # Check nums2 against set
    let added = {}
    i = 0
    while (i < nums2.length()) {
        if (set1[nums2[i]] && !added[nums2[i]]) {
            result.push(nums2[i])
            added[nums2[i]] = true
        }
        i = i + 1
    }
    
    return result
}

# Test
print(intersection([1, 2, 2, 1], [2, 2]))        # [2]
print(intersection([4, 9, 5], [9, 4, 9, 8, 4]))  # [4, 9] or [9, 4]
```

**Time Complexity**: O(n + m)  
**Space Complexity**: O(n)

### Problem 8: Happy Number

Determine if a number is happy (sum of squares of digits eventually reaches 1).

```klang
fn getNext(n) {
    let sum = 0
    while (n > 0) {
        let digit = n % 10
        sum = sum + digit * digit
        n = Math.floor(n / 10)
    }
    return sum
}

fn isHappy(n) {
    let seen = {}
    
    while (n != 1 && !seen[n]) {
        seen[n] = true
        n = getNext(n)
    }
    
    return n == 1
}

# Test
print(isHappy(19))   # true (1^2 + 9^2 = 82, 8^2 + 2^2 = 68, ... → 1)
print(isHappy(2))    # false (forms a cycle)
```

**Time Complexity**: O(log n)  
**Space Complexity**: O(log n)

**Explanation**: Use hash set to detect cycles. If we see same number again, it's a cycle.

### Problem 9: Isomorphic Strings

Check if two strings are isomorphic (characters can be replaced to get one from other).

```klang
fn isIsomorphic(s, t) {
    if (s.length() != t.length()) {
        return false
    }
    
    let mapST = {}
    let mapTS = {}
    let i = 0
    
    while (i < s.length()) {
        let charS = s[i]
        let charT = t[i]
        
        # Check s -> t mapping
        if (mapST[charS]) {
            if (mapST[charS] != charT) {
                return false
            }
        } else {
            mapST[charS] = charT
        }
        
        # Check t -> s mapping
        if (mapTS[charT]) {
            if (mapTS[charT] != charS) {
                return false
            }
        } else {
            mapTS[charT] = charS
        }
        
        i = i + 1
    }
    
    return true
}

# Test
print(isIsomorphic("egg", "add"))      # true
print(isIsomorphic("foo", "bar"))      # false
print(isIsomorphic("paper", "title"))  # true
```

**Time Complexity**: O(n)  
**Space Complexity**: O(1) - constant alphabet size

### Problem 10: Top K Frequent Elements

Find k most frequent elements in array.

```klang
fn topKFrequentElements(nums, k) {
    # Count frequencies
    let freq = {}
    let i = 0
    
    while (i < nums.length()) {
        if (freq[nums[i]]) {
            freq[nums[i]] = freq[nums[i]] + 1
        } else {
            freq[nums[i]] = 1
        }
        i = i + 1
    }
    
    # Bucket sort: index = frequency
    let buckets = []
    i = 0
    while (i <= nums.length()) {
        buckets.push([])
        i = i + 1
    }
    
    let keys = Object.keys(freq)
    i = 0
    while (i < keys.length()) {
        let num = parseInt(keys[i])
        let count = freq[num]
        buckets[count].push(num)
        i = i + 1
    }
    
    # Collect top k from highest frequency
    let result = []
    i = buckets.length() - 1
    while (i >= 0 && result.length() < k) {
        let j = 0
        while (j < buckets[i].length() && result.length() < k) {
            result.push(buckets[i][j])
            j = j + 1
        }
        i = i - 1
    }
    
    return result
}

# Test
print(topKFrequentElements([1, 1, 1, 2, 2, 3], 2))  # [1, 2]
print(topKFrequentElements([1], 1))                  # [1]
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

**Explanation**: Use hash table for frequencies, then bucket sort for O(n) time.

---

## Best Practices

### 1. Choose Right Hash Function

```klang
# Good: Consider all characters
fn goodHash(key, size) {
    let hash = 0
    let i = 0
    while (i < key.length()) {
        hash = (hash * 31 + key.charCodeAt(i)) % size
        i = i + 1
    }
    return hash
}

# Bad: Only first character
fn badHash(key, size) {
    return key.charCodeAt(0) % size  # Many collisions!
}
```

### 2. Use Prime Table Sizes

```klang
# Good: Prime number size
let tableSizes = [11, 23, 47, 97, 197, 397, 797, 1597]

# Bad: Power of 2 (more collisions with poor hash functions)
let badSizes = [16, 32, 64, 128, 256]
```

### 3. Monitor Load Factor

```klang
fn maintainLoadFactor(ht) {
    if (ht.count / ht.size > 0.75) {
        resize(ht)
    }
}
```

### 4. Choose Appropriate Collision Method

```klang
# Chaining: Good for high load factors
# Use when: Memory for pointers is acceptable, high collision rate

# Open Addressing: Good for cache performance
# Use when: Low load factor maintained, no pointer overhead desired
```

### 5. Handle Edge Cases

```klang
fn robustGet(ht, key) {
    if (key == null || key == "") {
        return null
    }
    
    let value = ht.get(key)
    return value
}

fn robustPut(ht, key, value) {
    if (key == null || key == "") {
        print("Invalid key")
        return false
    }
    
    ht.put(key, value)
    return true
}
```

### 6. Use Appropriate Data Types

```klang
# For unique items: Hash Set
fn uniqueElements(arr) {
    let set = {}
    let i = 0
    while (i < arr.length()) {
        set[arr[i]] = true
        i = i + 1
    }
    return Object.keys(set)
}

# For counting: Hash Map
fn countElements(arr) {
    let count = {}
    let i = 0
    while (i < arr.length()) {
        if (count[arr[i]]) {
            count[arr[i]] = count[arr[i]] + 1
        } else {
            count[arr[i]] = 1
        }
        i = i + 1
    }
    return count
}
```

### 7. Immutable Keys

```klang
# Good: Use immutable types as keys
let ht = {}
ht["string_key"] = "value"
ht[42] = "number key"

# Bad: Mutable objects as keys (can change, breaking hash)
# let obj = {id: 1}
# ht[obj] = "value"  # If obj changes, hash changes!
```

### 8. Benchmark Hash Functions

```klang
fn benchmarkHashFunction(hashFn, keys, tableSize) {
    let collisions = 0
    let distribution = []
    let i = 0
    
    while (i < tableSize) {
        distribution.push(0)
        i = i + 1
    }
    
    i = 0
    while (i < keys.length()) {
        let index = hashFn(keys[i], tableSize)
        if (distribution[index] > 0) {
            collisions = collisions + 1
        }
        distribution[index] = distribution[index] + 1
        i = i + 1
    }
    
    print("Collisions: " + collisions)
    print("Distribution: " + distribution)
}
```

---

## Complete Examples

### Example 1: Complete Hash Table with Chaining

```klang
class Node {
    init(key, value) {
        this.key = key
        this.value = value
        this.next = null
    }
}

class CompleteHashTable {
    init(initialSize) {
        this.size = initialSize || 10
        this.count = 0
        this.table = []
        let i = 0
        while (i < this.size) {
            this.table.push(null)
            i = i + 1
        }
    }
    
    # Polynomial rolling hash
    hash(key) {
        let hash = 0
        let prime = 31
        let i = 0
        
        while (i < key.length()) {
            hash = (hash * prime + key.charCodeAt(i)) % this.size
            i = i + 1
        }
        
        return hash
    }
    
    # Insert or update
    put(key, value) {
        if (this.shouldResize()) {
            this.resize()
        }
        
        let index = this.hash(key)
        let node = this.table[index]
        
        # Update if exists
        while (node != null) {
            if (node.key == key) {
                node.value = value
                return
            }
            node = node.next
        }
        
        # Insert at head
        let newNode = Node(key, value)
        newNode.next = this.table[index]
        this.table[index] = newNode
        this.count = this.count + 1
    }
    
    # Get value by key
    get(key) {
        let index = this.hash(key)
        let node = this.table[index]
        
        while (node != null) {
            if (node.key == key) {
                return node.value
            }
            node = node.next
        }
        
        return null
    }
    
    # Remove key
    remove(key) {
        let index = this.hash(key)
        let node = this.table[index]
        let prev = null
        
        while (node != null) {
            if (node.key == key) {
                if (prev == null) {
                    this.table[index] = node.next
                } else {
                    prev.next = node.next
                }
                this.count = this.count - 1
                return true
            }
            prev = node
            node = node.next
        }
        
        return false
    }
    
    # Check if key exists
    contains(key) {
        return this.get(key) != null
    }
    
    # Get all keys
    keys() {
        let keys = []
        let i = 0
        
        while (i < this.size) {
            let node = this.table[i]
            while (node != null) {
                keys.push(node.key)
                node = node.next
            }
            i = i + 1
        }
        
        return keys
    }
    
    # Get all values
    values() {
        let values = []
        let i = 0
        
        while (i < this.size) {
            let node = this.table[i]
            while (node != null) {
                values.push(node.value)
                node = node.next
            }
            i = i + 1
        }
        
        return values
    }
    
    # Get load factor
    loadFactor() {
        return this.count / this.size
    }
    
    # Check if should resize
    shouldResize() {
        return this.loadFactor() > 0.75
    }
    
    # Resize and rehash
    resize() {
        let oldTable = this.table
        let oldSize = this.size
        
        this.size = this.size * 2
        this.count = 0
        this.table = []
        
        let i = 0
        while (i < this.size) {
            this.table.push(null)
            i = i + 1
        }
        
        # Rehash all entries
        i = 0
        while (i < oldSize) {
            let node = oldTable[i]
            while (node != null) {
                this.put(node.key, node.value)
                node = node.next
            }
            i = i + 1
        }
    }
    
    # Display contents
    display() {
        print("Hash Table (size=" + this.size + ", count=" + this.count + 
              ", load=" + this.loadFactor() + "):")
        
        let i = 0
        while (i < this.size) {
            let node = this.table[i]
            if (node != null) {
                let chain = "  [" + i + "] -> "
                while (node != null) {
                    chain = chain + "(" + node.key + ":" + node.value + ") -> "
                    node = node.next
                }
                chain = chain + "null"
                print(chain)
            }
            i = i + 1
        }
    }
    
    # Get statistics
    stats() {
        let maxChainLength = 0
        let emptyBuckets = 0
        let totalChainLength = 0
        let i = 0
        
        while (i < this.size) {
            let node = this.table[i]
            let chainLength = 0
            
            if (node == null) {
                emptyBuckets = emptyBuckets + 1
            } else {
                while (node != null) {
                    chainLength = chainLength + 1
                    node = node.next
                }
                totalChainLength = totalChainLength + chainLength
                if (chainLength > maxChainLength) {
                    maxChainLength = chainLength
                }
            }
            
            i = i + 1
        }
        
        let avgChainLength = 0
        if (this.size - emptyBuckets > 0) {
            avgChainLength = totalChainLength / (this.size - emptyBuckets)
        }
        
        print("Statistics:")
        print("  Total entries: " + this.count)
        print("  Table size: " + this.size)
        print("  Load factor: " + this.loadFactor())
        print("  Empty buckets: " + emptyBuckets)
        print("  Max chain length: " + maxChainLength)
        print("  Avg chain length: " + avgChainLength)
    }
}

# Usage Example
print("=== Complete Hash Table Demo ===\n")

let ht = CompleteHashTable(5)

print("Inserting elements...")
ht.put("apple", 100)
ht.put("banana", 200)
ht.put("cherry", 300)
ht.put("date", 400)
ht.put("elderberry", 500)

ht.display()

print("\nSearching:")
print("apple: " + ht.get("apple"))
print("banana: " + ht.get("banana"))
print("grape: " + ht.get("grape"))

print("\nAdding more elements (trigger resize):")
ht.put("fig", 600)
ht.put("grape", 700)
ht.put("honeydew", 800)

ht.display()

print("\nUpdating value:")
ht.put("apple", 999)
print("apple: " + ht.get("apple"))

print("\nRemoving elements:")
ht.remove("banana")
ht.remove("date")

ht.display()

print("\nAll keys: " + ht.keys())
print("All values: " + ht.values())

print("\n")
ht.stats()
```

### Example 2: Phone Book Application

```klang
class Contact {
    init(name, phone, email) {
        this.name = name
        this.phone = phone
        this.email = email || ""
    }
    
    toString() {
        return this.name + " - " + this.phone + 
               (this.email ? " (" + this.email + ")" : "")
    }
}

class PhoneBook {
    init() {
        this.contacts = CompleteHashTable(20)
    }
    
    addContact(name, phone, email) {
        let contact = Contact(name, phone, email)
        this.contacts.put(name, contact)
        print("Added: " + contact.toString())
    }
    
    findContact(name) {
        let contact = this.contacts.get(name)
        if (contact) {
            print("Found: " + contact.toString())
            return contact
        } else {
            print("Contact not found: " + name)
            return null
        }
    }
    
    updateContact(name, phone, email) {
        if (this.contacts.contains(name)) {
            let contact = Contact(name, phone, email)
            this.contacts.put(name, contact)
            print("Updated: " + contact.toString())
        } else {
            print("Contact not found: " + name)
        }
    }
    
    deleteContact(name) {
        if (this.contacts.remove(name)) {
            print("Deleted: " + name)
        } else {
            print("Contact not found: " + name)
        }
    }
    
    listAll() {
        print("\n=== All Contacts ===")
        let names = this.contacts.keys()
        let i = 0
        while (i < names.length()) {
            let contact = this.contacts.get(names[i])
            print("  " + contact.toString())
            i = i + 1
        }
    }
}

# Usage
print("=== Phone Book Application ===\n")

let phoneBook = PhoneBook()

phoneBook.addContact("Alice Smith", "555-1234", "alice@email.com")
phoneBook.addContact("Bob Jones", "555-5678", "bob@email.com")
phoneBook.addContact("Charlie Brown", "555-9012")

print("\n")
phoneBook.findContact("Alice Smith")
phoneBook.findContact("Dave Wilson")

print("\n")
phoneBook.updateContact("Bob Jones", "555-0000", "newbob@email.com")

phoneBook.listAll()

print("\n")
phoneBook.deleteContact("Charlie Brown")

phoneBook.listAll()
```

---

## Summary

Hash tables are one of the most important and versatile data structures:

### Key Takeaways

1. **Fast Operations**: O(1) average time for insert, search, delete
2. **Hash Function**: Critical for performance - should distribute uniformly
3. **Collision Handling**: Choose between chaining and open addressing
4. **Load Factor**: Monitor and resize to maintain performance
5. **Real-World Use**: Databases, caches, compilers, web servers

### When to Use Hash Tables

✅ **Use when:**
- Need fast lookups by key
- Order doesn't matter
- Keys are hashable and immutable
- Implementing sets or maps
- Counting frequencies
- Finding duplicates

❌ **Don't use when:**
- Need ordered data (use trees)
- Need range queries (use trees)
- Keys are not hashable
- Memory is extremely limited
- Need predictable worst-case performance

### Time Complexity Summary

| Operation | Average | Worst Case |
|-----------|---------|------------|
| Insert    | O(1)    | O(n)       |
| Search    | O(1)    | O(n)       |
| Delete    | O(1)    | O(n)       |
| Space     | O(n)    | O(n)       |

### Common Patterns

1. **Two Sum Pattern**: Use hash table to find complements
2. **Frequency Counting**: Count occurrences of elements
3. **Grouping**: Group anagrams, similar items
4. **Caching**: Store computed results (memoization)
5. **Deduplication**: Remove or find duplicates

Master hash tables to excel in coding interviews and build efficient applications!

---

**Next Topics**: 
- [Trees](./07_trees.md)
- [Heaps](./08_heaps.md)
- [Graphs](./09_graphs.md)
