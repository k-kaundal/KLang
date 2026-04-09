# Strings in KLang - Data Structures & Algorithms

## Table of Contents
1. [Introduction](#introduction)
2. [What is a String?](#what-is-a-string)
3. [String Basics](#string-basics)
4. [String Operations](#string-operations)
5. [String Algorithms](#string-algorithms)
6. [Time and Space Complexity](#time-and-space-complexity)
7. [Best Practices](#best-practices)
8. [Interview Problems](#interview-problems)
9. [Examples](#examples)

---

## Introduction

Strings are one of the most commonly used data structures in programming. They represent sequences of characters and are essential for text processing, data manipulation, and communication between systems.

### Why Use Strings?

- **Text Processing**: Handle user input, file content, and data parsing
- **Communication**: Format messages, build APIs, and construct queries
- **Pattern Matching**: Search, validate, and transform text data
- **Data Representation**: Serialize and deserialize complex data
- **Universal**: Every programming problem involves strings at some level

---

## What is a String?

A **string** is a sequence of characters stored as a collection. Strings can be thought of as arrays of characters, but with specialized operations optimized for text manipulation.

### Characteristics

- **Sequential**: Characters stored in order
- **Indexed**: Access characters by zero-based index
- **Immutable (Often)**: Many languages treat strings as immutable
- **Variable Length**: Can grow or shrink dynamically
- **UTF-8 Support**: Modern strings support Unicode characters

### Visual Representation

```
String: "Hello"
Index:   0 1 2 3 4
Chars:   H e l l o
```

---

## String Basics

### Declaration and Initialization

```klang
# Empty string
let str = ""

# String with initial value
let message = "Hello, World!"

# String with special characters
let multiline = "Line 1\nLine 2\nLine 3"

# String concatenation
let firstName = "John"
let lastName = "Doe"
let fullName = firstName + " " + lastName  # "John Doe"
```

### Accessing Characters

```klang
let str = "KLang"

# Access by index
let first = str[0]      # 'K'
let second = str[1]     # 'L'
let last = str[4]       # 'g'

# Get string length
let length = len(str)   # 5

# Time Complexity: O(1)
```

### String Immutability

In many languages, strings are immutable, meaning they cannot be changed after creation.

```klang
let str = "Hello"

# This creates a NEW string, doesn't modify the original
str = str + " World"

# Individual character modification may not be supported
# str[0] = 'h'  # May cause error in immutable strings

# Workaround: Convert to array, modify, then join
let chars = str.split("")
chars[0] = 'h'
str = chars.join("")
```

---

## String Operations

### Basic Operations

#### 1. Length

```klang
let str = "Programming"
let length = len(str)  # 11

# Time Complexity: O(1)
```

#### 2. Concatenation

```klang
let str1 = "Hello"
let str2 = "World"
let result = str1 + " " + str2  # "Hello World"

# Using concat method
let result2 = str1.concat(" ", str2)

# Time Complexity: O(n + m) where n, m are string lengths
```

#### 3. Substring/Slice

```klang
let str = "Hello, World!"

# Get substring from index 0 to 5 (exclusive)
let sub1 = str.slice(0, 5)      # "Hello"

# Get substring from index 7 to end
let sub2 = str.slice(7)         # "World!"

# Negative indices work from end
let sub3 = str.slice(-6, -1)    # "World"

# Time Complexity: O(k) where k is substring length
```

#### 4. Character at Index

```klang
let str = "KLang"

let char1 = str[0]           # 'K'
let char2 = str.charAt(2)    # 'a'

# Time Complexity: O(1)
```

#### 5. Search and Find

```klang
let str = "Hello, World!"

# Find index of substring
let index1 = str.indexOf("World")      # 7
let index2 = str.indexOf("xyz")        # -1 (not found)

# Find last occurrence
let index3 = str.lastIndexOf("o")      # 8

# Check if string contains substring
let contains = str.includes("World")   # true

# Time Complexity: O(n*m) worst case for indexOf
```

#### 6. Case Conversion

```klang
let str = "Hello World"

let upper = str.toUpperCase()    # "HELLO WORLD"
let lower = str.toLowerCase()    # "hello world"

# Time Complexity: O(n)
```

#### 7. Trim Whitespace

```klang
let str = "   Hello World   "

let trimmed = str.trim()           # "Hello World"
let trimStart = str.trimStart()    # "Hello World   "
let trimEnd = str.trimEnd()        # "   Hello World"

# Time Complexity: O(n)
```

### Advanced Operations

#### 1. Split

```klang
let str = "apple,banana,cherry"

# Split by delimiter
let fruits = str.split(",")
# fruits = ["apple", "banana", "cherry"]

# Split into characters
let chars = str.split("")
# chars = ['a', 'p', 'p', 'l', 'e', ...]

# Split with limit
let limited = str.split(",", 2)
# limited = ["apple", "banana"]

# Time Complexity: O(n)
```

#### 2. Join

```klang
let words = ["Hello", "World", "from", "KLang"]

# Join with separator
let sentence = words.join(" ")
# sentence = "Hello World from KLang"

# Join without separator
let combined = words.join("")
# combined = "HelloWorldfromKLang"

# Time Complexity: O(n) where n is total characters
```

#### 3. Replace

```klang
let str = "Hello World"

# Replace first occurrence
let replaced1 = str.replace("World", "KLang")
# replaced1 = "Hello KLang"

# Replace all occurrences
let str2 = "foo bar foo baz foo"
let replaced2 = str2.replaceAll("foo", "qux")
# replaced2 = "qux bar qux baz qux"

# Time Complexity: O(n)
```

#### 4. Reverse

```klang
fn reverseString(str: string) -> string {
    let chars = str.split("")
    let left = 0
    let right = len(chars) - 1
    
    while (left < right) {
        let temp = chars[left]
        chars[left] = chars[right]
        chars[right] = temp
        left = left + 1
        right = right - 1
    }
    
    return chars.join("")
}

let str = "Hello"
let reversed = reverseString(str)  # "olleH"

# Time Complexity: O(n)
# Space Complexity: O(n)
```

#### 5. Character Code Conversion

```klang
# Get character code
let char = 'A'
let code = char.charCodeAt(0)  # 65

# Convert code to character
let newChar = String.fromCharCode(65)  # 'A'

# Time Complexity: O(1)
```

---

## String Algorithms

### 1. Character Frequency Count

Count occurrences of each character in a string.

```klang
fn charFrequency(str: string) -> object {
    let freq = {}
    let i = 0
    
    while (i < len(str)) {
        let char = str[i]
        if (char in freq) {
            freq[char] = freq[char] + 1
        } else {
            freq[char] = 1
        }
        i = i + 1
    }
    
    return freq
}

let str = "programming"
let freq = charFrequency(str)
# freq = {'p': 1, 'r': 2, 'o': 1, 'g': 2, 'a': 1, 'm': 2, 'i': 1, 'n': 1}
```

**Time Complexity**: O(n)  
**Space Complexity**: O(k) where k is unique characters

### 2. Palindrome Check

Determine if a string reads the same forwards and backwards.

```klang
fn isPalindrome(str: string) -> bool {
    # Convert to lowercase and remove non-alphanumeric
    let cleaned = ""
    let i = 0
    while (i < len(str)) {
        let char = str[i].toLowerCase()
        if (char >= 'a' && char <= 'z' || char >= '0' && char <= '9') {
            cleaned = cleaned + char
        }
        i = i + 1
    }
    
    # Two pointer approach
    let left = 0
    let right = len(cleaned) - 1
    
    while (left < right) {
        if (cleaned[left] != cleaned[right]) {
            return false
        }
        left = left + 1
        right = right - 1
    }
    
    return true
}

let str1 = "A man, a plan, a canal: Panama"
let result1 = isPalindrome(str1)  # true

let str2 = "race a car"
let result2 = isPalindrome(str2)  # false
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n) for cleaned string

### 3. Anagram Detection

Check if two strings are anagrams (contain same characters with same frequencies).

```klang
fn isAnagram(str1: string, str2: string) -> bool {
    # Different lengths can't be anagrams
    if (len(str1) != len(str2)) {
        return false
    }
    
    # Count character frequencies
    let freq1 = {}
    let freq2 = {}
    
    let i = 0
    while (i < len(str1)) {
        let char1 = str1[i].toLowerCase()
        let char2 = str2[i].toLowerCase()
        
        freq1[char1] = (freq1[char1] || 0) + 1
        freq2[char2] = (freq2[char2] || 0) + 1
        
        i = i + 1
    }
    
    # Compare frequencies
    for (let char in freq1) {
        if (freq1[char] != freq2[char]) {
            return false
        }
    }
    
    return true
}

let result1 = isAnagram("listen", "silent")  # true
let result2 = isAnagram("hello", "world")    # false
```

**Time Complexity**: O(n)  
**Space Complexity**: O(k) where k is unique characters

### 4. String Compression

Compress a string using character counts.

```klang
fn compressString(str: string) -> string {
    if (len(str) == 0) {
        return ""
    }
    
    let compressed = ""
    let count = 1
    let i = 1
    
    while (i < len(str)) {
        if (str[i] == str[i - 1]) {
            count = count + 1
        } else {
            compressed = compressed + str[i - 1] + str(count)
            count = 1
        }
        i = i + 1
    }
    
    # Add last character
    compressed = compressed + str[len(str) - 1] + str(count)
    
    # Return original if compressed is not smaller
    if (len(compressed) >= len(str)) {
        return str
    }
    
    return compressed
}

let str1 = "aabcccccaaa"
let result1 = compressString(str1)  # "a2b1c5a3"

let str2 = "abc"
let result2 = compressString(str2)  # "abc" (not compressed)
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n) for result string

### 5. Longest Substring Without Repeating Characters

Find the length of the longest substring without repeating characters.

```klang
fn lengthOfLongestSubstring(str: string) -> int {
    let seen = {}
    let maxLength = 0
    let start = 0
    let i = 0
    
    while (i < len(str)) {
        let char = str[i]
        
        # If character seen and in current window
        if (char in seen && seen[char] >= start) {
            start = seen[char] + 1
        }
        
        seen[char] = i
        maxLength = max(maxLength, i - start + 1)
        i = i + 1
    }
    
    return maxLength
}

let str = "abcabcbb"
let result = lengthOfLongestSubstring(str)  # 3 ("abc")
```

**Time Complexity**: O(n)  
**Space Complexity**: O(min(n, k)) where k is character set size

### 6. Pattern Matching (Naive Approach)

Find all occurrences of a pattern in a text.

```klang
fn findPattern(text: string, pattern: string) -> array {
    let results = []
    let n = len(text)
    let m = len(pattern)
    
    let i = 0
    while (i <= n - m) {
        let j = 0
        let match = true
        
        while (j < m) {
            if (text[i + j] != pattern[j]) {
                match = false
                break
            }
            j = j + 1
        }
        
        if (match) {
            results.push(i)
        }
        
        i = i + 1
    }
    
    return results
}

let text = "AABAACAADAABAABA"
let pattern = "AABA"
let indices = findPattern(text, pattern)
# indices = [0, 9, 12]
```

**Time Complexity**: O(n*m) where n is text length, m is pattern length  
**Space Complexity**: O(k) where k is number of matches

### 7. String Reversal (Word by Word)

Reverse the order of words in a string.

```klang
fn reverseWords(str: string) -> string {
    # Split into words
    let words = str.trim().split(" ")
    
    # Reverse the array
    let left = 0
    let right = len(words) - 1
    
    while (left < right) {
        let temp = words[left]
        words[left] = words[right]
        words[right] = temp
        left = left + 1
        right = right - 1
    }
    
    # Join back
    return words.join(" ")
}

let str = "Hello World from KLang"
let reversed = reverseWords(str)
# reversed = "KLang from World Hello"
```

**Time Complexity**: O(n)  
**Space Complexity**: O(n)

---

## Time and Space Complexity

### Operation Complexities

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Access character | O(1) | O(1) |
| Length | O(1) | O(1) |
| Concatenation | O(n + m) | O(n + m) |
| Substring/Slice | O(k) | O(k) |
| Search/IndexOf | O(n*m) | O(1) |
| Split | O(n) | O(n) |
| Join | O(n) | O(n) |
| Replace | O(n) | O(n) |
| Reverse | O(n) | O(n) |
| Case conversion | O(n) | O(n) |

### Algorithm Complexities

| Algorithm | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Character frequency | O(n) | O(k) |
| Palindrome check | O(n) | O(1) or O(n) |
| Anagram detection | O(n) | O(k) |
| String compression | O(n) | O(n) |
| Pattern matching (naive) | O(n*m) | O(1) |
| Pattern matching (KMP) | O(n + m) | O(m) |
| Longest substring | O(n) | O(k) |

**Legend**: n = string length, m = pattern length, k = unique characters

---

## Best Practices

### 1. Use String Builder for Multiple Concatenations

```klang
# Bad: Creates many intermediate strings
let result = ""
let i = 0
while (i < 1000) {
    result = result + str(i)  # O(n²) total
    i = i + 1
}

# Good: Use array and join
let parts = []
let i = 0
while (i < 1000) {
    parts.push(str(i))
    i = i + 1
}
let result = parts.join("")  # O(n) total
```

### 2. Pre-process Strings When Possible

```klang
# If searching multiple times, convert to lowercase once
fn countOccurrences(text: string, word: string) -> int {
    let lowerText = text.toLowerCase()
    let lowerWord = word.toLowerCase()
    
    # Now search in lowercase versions
    # ... search logic ...
}
```

### 3. Use Appropriate Data Structures

```klang
# For character frequency: Use hash map
let freq = {}

# For fast lookup: Use set
let seen = new Set()

# For ordered iteration: Use array
let chars = []
```

### 4. Handle Edge Cases

```klang
fn processString(str: string) -> string {
    # Check null/undefined
    if (!str) {
        return ""
    }
    
    # Check empty string
    if (len(str) == 0) {
        return ""
    }
    
    # Handle single character
    if (len(str) == 1) {
        return str
    }
    
    # Main logic
    # ...
}
```

### 5. Consider Unicode and Special Characters

```klang
# Be aware of multi-byte characters
let emoji = "👋"
let length = len(emoji)  # May be > 1 depending on encoding

# Normalize for comparison
fn compareStrings(str1: string, str2: string) -> bool {
    return str1.toLowerCase().trim() == str2.toLowerCase().trim()
}
```

---

## Interview Problems

### Problem 1: Valid Palindrome

Check if a string is a palindrome, considering only alphanumeric characters.

```klang
fn isPalindromeValid(s: string) -> bool {
    let left = 0
    let right = len(s) - 1
    
    while (left < right) {
        # Skip non-alphanumeric from left
        while (left < right && !isAlphanumeric(s[left])) {
            left = left + 1
        }
        
        # Skip non-alphanumeric from right
        while (left < right && !isAlphanumeric(s[right])) {
            right = right - 1
        }
        
        # Compare characters
        if (s[left].toLowerCase() != s[right].toLowerCase()) {
            return false
        }
        
        left = left + 1
        right = right - 1
    }
    
    return true
}

fn isAlphanumeric(char: string) -> bool {
    return (char >= 'a' && char <= 'z') ||
           (char >= 'A' && char <= 'Z') ||
           (char >= '0' && char <= '9')
}

let test1 = isPalindromeValid("A man, a plan, a canal: Panama")  # true
let test2 = isPalindromeValid("race a car")  # false
```

**Time**: O(n), **Space**: O(1)

### Problem 2: Reverse String

Reverse a string in-place.

```klang
fn reverseStringInPlace(chars: array) {
    let left = 0
    let right = len(chars) - 1
    
    while (left < right) {
        # Swap characters
        let temp = chars[left]
        chars[left] = chars[right]
        chars[right] = temp
        
        left = left + 1
        right = right - 1
    }
}

let chars = ['h', 'e', 'l', 'l', 'o']
reverseStringInPlace(chars)
# chars = ['o', 'l', 'l', 'e', 'h']
```

**Time**: O(n), **Space**: O(1)

### Problem 3: First Unique Character

Find the first non-repeating character in a string.

```klang
fn firstUniqChar(s: string) -> int {
    # Count frequencies
    let freq = {}
    let i = 0
    
    while (i < len(s)) {
        let char = s[i]
        freq[char] = (freq[char] || 0) + 1
        i = i + 1
    }
    
    # Find first unique
    i = 0
    while (i < len(s)) {
        if (freq[s[i]] == 1) {
            return i
        }
        i = i + 1
    }
    
    return -1  # No unique character
}

let str = "leetcode"
let index = firstUniqChar(str)  # Returns 0 ('l')

let str2 = "loveleetcode"
let index2 = firstUniqChar(str2)  # Returns 2 ('v')
```

**Time**: O(n), **Space**: O(k) where k is unique characters

### Problem 4: Longest Substring Without Repeating Characters

Find the length of the longest substring without repeating characters.

```klang
fn lengthOfLongestSubstring(s: string) -> int {
    let charSet = new Set()
    let maxLen = 0
    let left = 0
    let right = 0
    
    while (right < len(s)) {
        # If character not in set, expand window
        if (!charSet.has(s[right])) {
            charSet.add(s[right])
            maxLen = max(maxLen, right - left + 1)
            right = right + 1
        } else {
            # Remove leftmost character and shrink window
            charSet.delete(s[left])
            left = left + 1
        }
    }
    
    return maxLen
}

let str1 = "abcabcbb"
let result1 = lengthOfLongestSubstring(str1)  # 3 ("abc")

let str2 = "bbbbb"
let result2 = lengthOfLongestSubstring(str2)  # 1 ("b")

let str3 = "pwwkew"
let result3 = lengthOfLongestSubstring(str3)  # 3 ("wke")
```

**Time**: O(n), **Space**: O(min(n, k)) where k is character set size

### Problem 5: Valid Anagram

Check if two strings are anagrams of each other.

```klang
fn isAnagram(s: string, t: string) -> bool {
    # Quick length check
    if (len(s) != len(t)) {
        return false
    }
    
    # Count character frequencies
    let counts = {}
    let i = 0
    
    # Increment for s, decrement for t
    while (i < len(s)) {
        counts[s[i]] = (counts[s[i]] || 0) + 1
        counts[t[i]] = (counts[t[i]] || 0) - 1
        i = i + 1
    }
    
    # All counts should be zero
    for (let char in counts) {
        if (counts[char] != 0) {
            return false
        }
    }
    
    return true
}

let result1 = isAnagram("anagram", "nagaram")  # true
let result2 = isAnagram("rat", "car")          # false
```

**Time**: O(n), **Space**: O(k) where k is unique characters

### Problem 6: Group Anagrams

Group strings that are anagrams of each other.

```klang
fn groupAnagrams(strs: array) -> array {
    let groups = {}
    
    let i = 0
    while (i < len(strs)) {
        # Create sorted key for anagram group
        let chars = strs[i].split("")
        chars.sort()
        let key = chars.join("")
        
        # Add to appropriate group
        if (!(key in groups)) {
            groups[key] = []
        }
        groups[key].push(strs[i])
        
        i = i + 1
    }
    
    # Convert to array of arrays
    let result = []
    for (let key in groups) {
        result.push(groups[key])
    }
    
    return result
}

let strs = ["eat", "tea", "tan", "ate", "nat", "bat"]
let grouped = groupAnagrams(strs)
# [["eat","tea","ate"], ["tan","nat"], ["bat"]]
```

**Time**: O(n * k log k) where n is number of strings, k is max string length  
**Space**: O(n * k)

---

## Examples

### Example 1: String Utilities Class

```klang
class StringUtils {
    fn reverse(str: string) -> string {
        let chars = str.split("")
        let left = 0
        let right = len(chars) - 1
        
        while (left < right) {
            let temp = chars[left]
            chars[left] = chars[right]
            chars[right] = temp
            left = left + 1
            right = right - 1
        }
        
        return chars.join("")
    }
    
    fn isPalindrome(str: string) -> bool {
        let cleaned = this.removeNonAlpha(str).toLowerCase()
        return cleaned == this.reverse(cleaned)
    }
    
    fn removeNonAlpha(str: string) -> string {
        let result = ""
        let i = 0
        
        while (i < len(str)) {
            let char = str[i]
            if ((char >= 'a' && char <= 'z') ||
                (char >= 'A' && char <= 'Z') ||
                (char >= '0' && char <= '9')) {
                result = result + char
            }
            i = i + 1
        }
        
        return result
    }
    
    fn countWords(str: string) -> int {
        let words = str.trim().split(" ")
        return len(words)
    }
    
    fn capitalizeWords(str: string) -> string {
        let words = str.split(" ")
        let result = []
        
        let i = 0
        while (i < len(words)) {
            if (len(words[i]) > 0) {
                let capitalized = words[i][0].toUpperCase() + 
                                 words[i].slice(1).toLowerCase()
                result.push(capitalized)
            }
            i = i + 1
        }
        
        return result.join(" ")
    }
}

# Usage
let utils = new StringUtils()
println(utils.reverse("hello"))              # "olleh"
println(utils.isPalindrome("racecar"))       # true
println(utils.countWords("Hello World"))     # 2
println(utils.capitalizeWords("hello world")) # "Hello World"
```

### Example 2: Text Analyzer

```klang
class TextAnalyzer {
    let text: string = ""
    
    fn init(text: string) {
        this.text = text
    }
    
    fn wordCount() -> int {
        if (len(this.text) == 0) {
            return 0
        }
        let words = this.text.trim().split(" ")
        return len(words)
    }
    
    fn charCount() -> int {
        return len(this.text)
    }
    
    fn mostCommonChar() -> string {
        let freq = {}
        let maxChar = ""
        let maxCount = 0
        
        let i = 0
        while (i < len(this.text)) {
            let char = this.text[i]
            if (char != ' ') {
                freq[char] = (freq[char] || 0) + 1
                if (freq[char] > maxCount) {
                    maxCount = freq[char]
                    maxChar = char
                }
            }
            i = i + 1
        }
        
        return maxChar
    }
    
    fn averageWordLength() -> float {
        let words = this.text.trim().split(" ")
        if (len(words) == 0) {
            return 0.0
        }
        
        let totalLen = 0
        let i = 0
        while (i < len(words)) {
            totalLen = totalLen + len(words[i])
            i = i + 1
        }
        
        return float(totalLen) / float(len(words))
    }
}

# Usage
let analyzer = new TextAnalyzer("The quick brown fox jumps over the lazy dog")
println("Words: " + str(analyzer.wordCount()))
println("Characters: " + str(analyzer.charCount()))
println("Most common: " + analyzer.mostCommonChar())
println("Avg word length: " + str(analyzer.averageWordLength()))
```

### Example 3: Pattern Matcher

```klang
class PatternMatcher {
    fn findAll(text: string, pattern: string) -> array {
        let matches = []
        let i = 0
        
        while (i <= len(text) - len(pattern)) {
            if (this.matchAt(text, pattern, i)) {
                matches.push(i)
            }
            i = i + 1
        }
        
        return matches
    }
    
    fn matchAt(text: string, pattern: string, start: int) -> bool {
        let j = 0
        while (j < len(pattern)) {
            if (text[start + j] != pattern[j]) {
                return false
            }
            j = j + 1
        }
        return true
    }
    
    fn replaceAll(text: string, pattern: string, replacement: string) -> string {
        let matches = this.findAll(text, pattern)
        
        if (len(matches) == 0) {
            return text
        }
        
        let result = ""
        let lastIndex = 0
        
        let i = 0
        while (i < len(matches)) {
            let matchIndex = matches[i]
            result = result + text.slice(lastIndex, matchIndex)
            result = result + replacement
            lastIndex = matchIndex + len(pattern)
            i = i + 1
        }
        
        result = result + text.slice(lastIndex)
        return result
    }
}

# Usage
let matcher = new PatternMatcher()
let text = "hello world, hello universe"
let indices = matcher.findAll(text, "hello")
println("Found at: " + str(indices))  # [0, 13]

let replaced = matcher.replaceAll(text, "hello", "hi")
println(replaced)  # "hi world, hi universe"
```

---

## Summary

- **Strings** are sequences of characters used for text processing
- **Access** by index is O(1), length is O(1)
- **Concatenation** is O(n+m), should use array+join for multiple concatenations
- **Search** operations are typically O(n*m), can be optimized with algorithms like KMP
- **Two-pointer** technique is common for string manipulation
- **Hash maps** are useful for character frequency problems
- **Sliding window** technique solves substring problems efficiently
- Consider **immutability** when designing string algorithms

---

## Next Steps

Continue to:
- [Linked Lists](03_linked_lists.md)
- [Stacks and Queues](04_stacks_queues.md)
- [Hash Tables](05_hash_tables.md)
- [Sorting Algorithms](../algorithms/01_sorting.md)

Happy coding! 🚀
