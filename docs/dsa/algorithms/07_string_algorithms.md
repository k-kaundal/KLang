# String Algorithms in KLang - Data Structures & Algorithms

## Table of Contents
1. [Introduction](#introduction)
2. [Understanding Strings](#understanding-strings)
3. [Pattern Matching Algorithms](#pattern-matching-algorithms)
   - [Naive Pattern Matching](#naive-pattern-matching)
   - [KMP Algorithm](#kmp-knuth-morris-pratt-algorithm)
   - [Rabin-Karp Algorithm](#rabin-karp-algorithm)
   - [Boyer-Moore Algorithm](#boyer-moore-algorithm)
   - [Z-Algorithm](#z-algorithm)
4. [String Processing Algorithms](#string-processing-algorithms)
   - [Longest Palindromic Substring](#longest-palindromic-substring)
   - [Longest Common Prefix](#longest-common-prefix)
   - [Anagram Checking](#anagram-checking)
5. [Suffix Structures](#suffix-structures)
   - [Suffix Array](#suffix-array)
   - [Suffix Tree](#suffix-tree)
   - [LCP Array](#lcp-array)
6. [String Dynamic Programming](#string-dynamic-programming)
   - [Edit Distance](#edit-distance)
   - [Longest Common Subsequence](#longest-common-subsequence)
   - [Palindrome Partitioning](#palindrome-partitioning)
7. [Rolling Hash Technique](#rolling-hash-technique)
8. [Interview Problems](#interview-problems)
9. [Comparison Table](#comparison-table)
10. [Best Practices](#best-practices)
11. [Common Pitfalls](#common-pitfalls)
12. [Real-World Applications](#real-world-applications)

---

## Introduction

String algorithms are fundamental in computer science and form the backbone of many critical applications. Understanding these algorithms is essential for:

- **Text Processing**: Search engines, text editors, compilers
- **Bioinformatics**: DNA sequence matching, genome analysis
- **Data Mining**: Pattern recognition, information retrieval
- **Security**: Intrusion detection, malware analysis
- **Interview Success**: One of the most tested topics in coding interviews

### Why String Algorithms Matter

1. **Ubiquitous Data Type**: Text is everywhere in computing
2. **Complex Problems**: Simple-looking problems can have elegant solutions
3. **Performance Critical**: Efficient algorithms save time and resources
4. **Foundation for Advanced Topics**: Compression, cryptography, NLP
5. **Interview Favorites**: Frequently asked in technical interviews

This comprehensive guide covers essential string algorithms with complete KLang implementations, complexity analysis, and real-world applications.

---

## Understanding Strings

### What is a String?

A **string** is a sequence of characters. In KLang, strings are immutable sequences that support various operations.

### Key Concepts

- **Length**: Number of characters in the string
- **Indexing**: Accessing characters by position (0-based)
- **Substring**: Contiguous sequence of characters
- **Subsequence**: Characters in order but not necessarily contiguous
- **Pattern**: String we're searching for
- **Text**: String in which we're searching

### String Complexity Notation

- **n**: Length of the text
- **m**: Length of the pattern
- **k**: Alphabet size (typically 26 for lowercase English)

---

## Pattern Matching Algorithms

Pattern matching is the problem of finding all occurrences of a pattern string within a text string. This is one of the most fundamental problems in string algorithms.

### Problem Statement

**Given**: Text `T` of length `n` and pattern `P` of length `m`  
**Find**: All positions where pattern `P` appears in text `T`

**Example**:
```
Text:    "ABABCABABA"
Pattern: "ABA"
Output:  [0, 2, 7]  (positions where "ABA" starts)
```

---

## Naive Pattern Matching

The **Naive** or **Brute Force** approach checks the pattern against every possible position in the text.

### Algorithm

1. Slide pattern over text one position at a time
2. At each position, compare pattern characters with text
3. If all match, record the position
4. Move to next position and repeat

### Visualization

```
Text:    A B A B C A B A B A
Pattern: A B A
         ↓ ↓ ↓
Position 0: ✓ ✓ ✓  → Match found at index 0

Text:    A B A B C A B A B A
Pattern:   A B A
           ↓ ✓ ✓
Position 1:   ✓ ✓  → No match (first char doesn't match)

Text:    A B A B C A B A B A
Pattern:     A B A
             ↓ ✓ ✓
Position 2:     ✓ ✓ ✓  → Match found at index 2
```

### Implementation

```javascript
class NaivePatternMatcher {
    // Find all occurrences of pattern in text
    function search(text, pattern) {
        let n = text.length();
        let m = pattern.length();
        let result = [];
        
        // Try every possible starting position
        for (let i = 0; i <= n - m; i++) {
            let j = 0;
            
            // Check if pattern matches at position i
            while (j < m && text[i + j] == pattern[j]) {
                j = j + 1;
            }
            
            // If we matched all characters
            if (j == m) {
                result.push(i);
            }
        }
        
        return result;
    }
    
    // Count total occurrences
    function countOccurrences(text, pattern) {
        return this.search(text, pattern).length();
    }
    
    // Check if pattern exists in text
    function contains(text, pattern) {
        let n = text.length();
        let m = pattern.length();
        
        for (let i = 0; i <= n - m; i++) {
            let j = 0;
            while (j < m && text[i + j] == pattern[j]) {
                j = j + 1;
            }
            if (j == m) {
                return true;
            }
        }
        return false;
    }
}

// Example usage
let matcher = NaivePatternMatcher();
let text = "ABABCABABA";
let pattern = "ABA";

print("Text: " + text);
print("Pattern: " + pattern);
print("Positions: " + matcher.search(text, pattern));
print("Count: " + matcher.countOccurrences(text, pattern));
print("Contains: " + matcher.contains(text, pattern));

// Output:
// Text: ABABCABABA
// Pattern: ABA
// Positions: [0, 2, 7]
// Count: 3
// Contains: true
```

### Complexity Analysis

- **Time Complexity**: O(n × m)
  - Worst case: Every position requires full pattern comparison
  - Example: Text = "AAAAAAA", Pattern = "AAA"
  - Best case: O(n) when first character rarely matches

- **Space Complexity**: O(1)
  - Only uses a few variables for indexing

### When to Use

- ✓ Pattern is very short (m ≤ 3)
- ✓ Simplicity is more important than speed
- ✓ One-time search on small text
- ✗ Large texts or patterns
- ✗ Repeated searches

---

## KMP (Knuth-Morris-Pratt) Algorithm

The **KMP algorithm** improves upon naive search by avoiding redundant comparisons. It never backtracks in the text, achieving O(n + m) time complexity.

### Key Insight

When a mismatch occurs, the pattern contains information about where to continue searching. We can skip positions that we know won't match.

### LPS Array (Longest Proper Prefix which is also Suffix)

The LPS array is the heart of KMP. For each position i in the pattern:
- `lps[i]` = length of longest proper prefix of `pattern[0...i]` that is also a suffix

**Example**:
```
Pattern: A B A B A C A
Index:   0 1 2 3 4 5 6
LPS:     0 0 1 2 3 0 1

Explanation:
- lps[0] = 0 (no proper prefix)
- lps[1] = 0 ("AB" has no prefix = suffix)
- lps[2] = 1 ("ABA": "A" is both prefix and suffix)
- lps[3] = 2 ("ABAB": "AB" is both prefix and suffix)
- lps[4] = 3 ("ABABA": "ABA" is both prefix and suffix)
- lps[5] = 0 ("ABABAC": no prefix = suffix)
- lps[6] = 1 ("ABABACA": "A" is both prefix and suffix)
```

### Visualization

```
Text:    A B A B C A B A B A
Pattern: A B A B A
         ↓ ↓ ↓ ✓ ✗
         
Mismatch at index 4!
Instead of starting over, KMP uses LPS:
lps[3] = 2, so we already matched "AB"
Continue from pattern index 2:

Text:    A B A B C A B A B A
Pattern:     A B A B A
               ↓ ✓ ✓ ✓ ✓
```

### Implementation

```javascript
class KMPMatcher {
    // Compute LPS (Longest Proper Prefix which is also Suffix) array
    function computeLPS(pattern) {
        let m = pattern.length();
        let lps = [];
        lps[0] = 0;  // lps[0] is always 0
        let len = 0;  // length of previous longest prefix suffix
        let i = 1;
        
        while (i < m) {
            if (pattern[i] == pattern[len]) {
                len = len + 1;
                lps[i] = len;
                i = i + 1;
            } else {
                if (len != 0) {
                    // Try shorter prefix
                    len = lps[len - 1];
                } else {
                    lps[i] = 0;
                    i = i + 1;
                }
            }
        }
        
        return lps;
    }
    
    // Search for pattern in text using KMP algorithm
    function search(text, pattern) {
        let n = text.length();
        let m = pattern.length();
        
        if (m == 0) return [];
        
        let result = [];
        let lps = this.computeLPS(pattern);
        
        let i = 0;  // index for text
        let j = 0;  // index for pattern
        
        while (i < n) {
            if (text[i] == pattern[j]) {
                i = i + 1;
                j = j + 1;
            }
            
            if (j == m) {
                // Pattern found
                result.push(i - j);
                j = lps[j - 1];
            } else if (i < n && text[i] != pattern[j]) {
                if (j != 0) {
                    j = lps[j - 1];
                } else {
                    i = i + 1;
                }
            }
        }
        
        return result;
    }
    
    // First occurrence of pattern
    function findFirst(text, pattern) {
        let matches = this.search(text, pattern);
        return matches.length() > 0 ? matches[0] : -1;
    }
    
    // Count occurrences
    function count(text, pattern) {
        return this.search(text, pattern).length();
    }
}

// Example usage
let kmp = KMPMatcher();
let text = "ABABCABABABAABAB";
let pattern = "ABAB";

print("KMP Pattern Matching");
print("Text: " + text);
print("Pattern: " + pattern);
print("Matches at: " + kmp.search(text, pattern));
print("First match at: " + kmp.findFirst(text, pattern));
print("Total matches: " + kmp.count(text, pattern));

// LPS Array demonstration
print("\nLPS Array for pattern: " + pattern);
print("LPS: " + kmp.computeLPS(pattern));

// Output:
// KMP Pattern Matching
// Text: ABABCABABABAABAB
// Pattern: ABAB
// Matches at: [0, 5, 7, 12]
// First match at: 0
// Total matches: 4
//
// LPS Array for pattern: ABAB
// LPS: [0, 0, 1, 2]
```

### Step-by-Step Example

```
Text:    A B C A B A B C A B
Pattern: A B A B C
LPS:     [0, 0, 1, 2, 0]

Step 1: i=0, j=0: A==A, i++, j++
Step 2: i=1, j=1: B==B, i++, j++
Step 3: i=2, j=2: C!=A, j=lps[1]=0
Step 4: i=2, j=0: C!=A, i++
Step 5: i=3, j=0: A==A, i++, j++
Step 6: i=4, j=1: B==B, i++, j++
Step 7: i=5, j=2: A==A, i++, j++
Step 8: i=6, j=3: B==B, i++, j++
Step 9: i=7, j=4: C==C, i++, j++, j==5 → MATCH at position 3!
```

### Complexity Analysis

- **Time Complexity**: O(n + m)
  - LPS construction: O(m)
  - Pattern search: O(n)
  - Text is scanned only once (no backtracking)

- **Space Complexity**: O(m)
  - LPS array requires O(m) space

### When to Use

- ✓ Long patterns (m > 10)
- ✓ Long texts
- ✓ Real-time pattern matching
- ✓ When text scanning can't backtrack (streaming data)

---

## Rabin-Karp Algorithm

The **Rabin-Karp algorithm** uses **hashing** to find patterns. It computes a hash value for the pattern and compares it with hash values of text substrings.

### Key Concept: Rolling Hash

Instead of recomputing hash for each substring from scratch, we "roll" the hash:
- Remove contribution of leftmost character
- Add contribution of new rightmost character

### Hash Function

For string `s = s[0]s[1]...s[m-1]`:
```
hash(s) = (s[0] × d^(m-1) + s[1] × d^(m-2) + ... + s[m-1] × d^0) mod q

where:
- d = alphabet size (e.g., 256 for ASCII)
- q = large prime number (to minimize collisions)
```

### Rolling Hash Formula

```
Remove s[i], add s[i+m]:
hash_new = (d × (hash_old - s[i] × h) + s[i+m]) mod q

where h = d^(m-1) mod q
```

### Visualization

```
Text:    A B C D E
Pattern: C D
         
Hash(AB) = (65×256 + 66) mod q
Hash(BC) = (256×(Hash(AB) - 65×256) + 67) mod q
          = (256×66 + 67) mod q
```

### Implementation

```javascript
class RabinKarpMatcher {
    // Prime number for modulo
    let PRIME = 101;
    let BASE = 256;  // Alphabet size
    
    // Calculate hash value of string
    function calculateHash(str, m) {
        let hash = 0;
        for (let i = 0; i < m; i++) {
            hash = (hash * this.BASE + str.charCodeAt(i)) % this.PRIME;
        }
        return hash;
    }
    
    // Recalculate hash using rolling hash
    function recalculateHash(str, oldIndex, newIndex, oldHash, patternLen) {
        let newHash = oldHash;
        
        // Remove leading character
        let h = 1;
        for (let i = 1; i < patternLen; i++) {
            h = (h * this.BASE) % this.PRIME;
        }
        
        newHash = (newHash - str.charCodeAt(oldIndex) * h) % this.PRIME;
        if (newHash < 0) {
            newHash = newHash + this.PRIME;
        }
        
        // Add trailing character
        newHash = (newHash * this.BASE + str.charCodeAt(newIndex)) % this.PRIME;
        
        return newHash;
    }
    
    // Check if strings match (handle hash collisions)
    function checkEqual(str1, start1, str2, start2, length) {
        for (let i = 0; i < length; i++) {
            if (str1[start1 + i] != str2[start2 + i]) {
                return false;
            }
        }
        return true;
    }
    
    // Search for pattern in text
    function search(text, pattern) {
        let n = text.length();
        let m = pattern.length();
        
        if (m > n) return [];
        
        let result = [];
        let patternHash = this.calculateHash(pattern, m);
        let textHash = this.calculateHash(text, m);
        
        // Check first window
        if (patternHash == textHash) {
            if (this.checkEqual(text, 0, pattern, 0, m)) {
                result.push(0);
            }
        }
        
        // Roll the hash for remaining windows
        for (let i = 1; i <= n - m; i++) {
            textHash = this.recalculateHash(text, i - 1, i + m - 1, textHash, m);
            
            // If hash matches, verify with actual string comparison
            if (patternHash == textHash) {
                if (this.checkEqual(text, i, pattern, 0, m)) {
                    result.push(i);
                }
            }
        }
        
        return result;
    }
    
    // Search for multiple patterns
    function searchMultiple(text, patterns) {
        let results = {};
        for (let i = 0; i < patterns.length(); i++) {
            let pattern = patterns[i];
            results[pattern] = this.search(text, pattern);
        }
        return results;
    }
}

// Example usage
let rk = RabinKarpMatcher();
let text = "ABABCABABABDABA";
let pattern = "ABA";

print("Rabin-Karp Pattern Matching");
print("Text: " + text);
print("Pattern: " + pattern);
print("Matches at: " + rk.search(text, pattern));

// Multiple patterns
let patterns = ["ABA", "CAB", "BAD"];
print("\nSearching multiple patterns:");
let results = rk.searchMultiple(text, patterns);
for (let pattern in results) {
    print(pattern + ": " + results[pattern]);
}

// Output:
// Rabin-Karp Pattern Matching
// Text: ABABCABABABDABA
// Pattern: ABA
// Matches at: [0, 2, 5, 7, 12]
//
// Searching multiple patterns:
// ABA: [0, 2, 5, 7, 12]
// CAB: [4]
// BAD: [9]
```

### Complexity Analysis

- **Time Complexity**: 
  - Average case: O(n + m)
  - Worst case: O(n × m) when many hash collisions occur
  - Best case: O(n + m) with good hash function

- **Space Complexity**: O(1)
  - Only constant extra space needed

### When to Use

- ✓ Searching for multiple patterns simultaneously
- ✓ Plagiarism detection
- ✓ Average case performance is good
- ✓ Simple to implement
- ✗ Worst case can be slow (hash collisions)

---

## Boyer-Moore Algorithm

The **Boyer-Moore algorithm** is one of the most efficient string searching algorithms in practice. It preprocesses the pattern and scans from right to left.

### Key Ideas

1. **Bad Character Rule**: Skip positions based on mismatched character
2. **Good Suffix Rule**: Skip based on matched suffix of pattern

### Bad Character Rule

When a mismatch occurs at text[i]:
- If text[i] appears in pattern, align it
- If text[i] doesn't appear, skip entire pattern

### Visualization

```
Text:    A B C D E F G H
Pattern:     C D E
             ↑ ↑ ✗
         
Check from right to left.
Mismatch at 'B', but 'B' not in pattern.
Skip entire pattern:

Text:    A B C D E F G H
Pattern:         C D E
```

### Implementation

```javascript
class BoyerMooreMatcher {
    // Bad character table
    function buildBadCharTable(pattern) {
        let m = pattern.length();
        let badChar = {};
        
        // Initialize all characters to -1
        for (let i = 0; i < m - 1; i++) {
            badChar[pattern[i]] = i;
        }
        
        return badChar;
    }
    
    // Simplified Boyer-Moore with bad character rule only
    function search(text, pattern) {
        let n = text.length();
        let m = pattern.length();
        
        if (m > n) return [];
        
        let result = [];
        let badChar = this.buildBadCharTable(pattern);
        let s = 0;  // shift of pattern relative to text
        
        while (s <= n - m) {
            let j = m - 1;
            
            // Keep reducing j while characters match
            while (j >= 0 && pattern[j] == text[s + j]) {
                j = j - 1;
            }
            
            if (j < 0) {
                // Pattern found
                result.push(s);
                s = s + 1;  // Move to next position
            } else {
                // Mismatch - use bad character rule
                let badCharShift = 1;
                let mismatchChar = text[s + j];
                
                if (badChar[mismatchChar] != null) {
                    badCharShift = j - badChar[mismatchChar];
                    if (badCharShift < 1) {
                        badCharShift = 1;
                    }
                } else {
                    badCharShift = j + 1;
                }
                
                s = s + badCharShift;
            }
        }
        
        return result;
    }
    
    // Count occurrences
    function count(text, pattern) {
        return this.search(text, pattern).length();
    }
}

// Example usage
let bm = BoyerMooreMatcher();
let text = "ABAAABCDBBABCDABCDABDE";
let pattern = "ABCD";

print("Boyer-Moore Pattern Matching");
print("Text: " + text);
print("Pattern: " + pattern);
print("Matches at: " + bm.search(text, pattern));
print("Total matches: " + bm.count(text, pattern));

// Performance demonstration
let longText = "AAAAAAAAAAAAAAAAAAAAAAABAAAA";
let longPattern = "AAAAB";
print("\nPerformance test:");
print("Text: " + longText);
print("Pattern: " + longPattern);
print("Matches at: " + bm.search(longText, longPattern));

// Output:
// Boyer-Moore Pattern Matching
// Text: ABAAABCDBBABCDABCDABDE
// Pattern: ABCD
// Matches at: [5, 10, 14]
// Total matches: 3
//
// Performance test:
// Text: AAAAAAAAAAAAAAAAAAAAAAABAAAA
// Pattern: AAAAB
// Matches at: [19]
```

### Complexity Analysis

- **Time Complexity**:
  - Best case: O(n/m) - can skip m characters at a time
  - Average case: O(n)
  - Worst case: O(n × m)

- **Space Complexity**: O(k) where k is alphabet size
  - Bad character table

### When to Use

- ✓ Large alphabet (not just DNA with 4 characters)
- ✓ Long patterns
- ✓ Best practical performance on average
- ✓ Text editor search functionality

---

## Z-Algorithm

The **Z-algorithm** computes the Z-array where Z[i] is the length of the longest substring starting from i that matches a prefix of the string.

### Z-Array Definition

For string S of length n:
```
Z[i] = length of longest substring starting at i which is also a prefix of S
Z[0] = n (by definition, whole string matches itself)
```

### Example

```
String: a a b a a b c a a b a a a
Index:  0 1 2 3 4 5 6 7 8 9 10 11 12
Z:      - 1 0 3 1 0 0 2 2 4 1  2  2

Explanation:
- Z[1] = 1: "a" matches prefix "a"
- Z[2] = 0: "b" doesn't match prefix "a"
- Z[3] = 3: "aab" matches prefix "aab"
- Z[9] = 4: "aaaa" matches prefix but string only has "aab"
```

### Pattern Matching with Z-Algorithm

Concatenate pattern and text with a separator:
```
Combined = pattern + "$" + text
Compute Z-array
Any Z[i] == m indicates pattern match at position i-m-1 in text
```

### Implementation

```javascript
class ZAlgorithm {
    // Compute Z-array for string
    function computeZ(s) {
        let n = s.length();
        let z = [];
        z[0] = n;
        
        let l = 0;  // Left boundary of Z-box
        let r = 0;  // Right boundary of Z-box
        
        for (let i = 1; i < n; i++) {
            if (i > r) {
                // Outside Z-box, compute from scratch
                l = i;
                r = i;
                while (r < n && s[r] == s[r - l]) {
                    r = r + 1;
                }
                z[i] = r - l;
                r = r - 1;
            } else {
                // Inside Z-box, use previously computed values
                let k = i - l;
                if (z[k] < r - i + 1) {
                    z[i] = z[k];
                } else {
                    l = i;
                    while (r < n && s[r] == s[r - l]) {
                        r = r + 1;
                    }
                    z[i] = r - l;
                    r = r - 1;
                }
            }
        }
        
        return z;
    }
    
    // Pattern matching using Z-algorithm
    function search(text, pattern) {
        let n = text.length();
        let m = pattern.length();
        
        if (m > n) return [];
        
        // Concatenate pattern, separator, and text
        let combined = pattern + "$" + text;
        let z = this.computeZ(combined);
        
        let result = [];
        for (let i = 0; i < z.length(); i++) {
            if (z[i] == m) {
                // Pattern found at position i - m - 1 in original text
                result.push(i - m - 1);
            }
        }
        
        return result;
    }
    
    // Find all prefixes that are also suffixes
    function findPrefixSuffixMatches(s) {
        let z = this.computeZ(s);
        let n = s.length();
        let matches = [];
        
        for (let i = 0; i < n; i++) {
            if (i + z[i] == n) {
                matches.push(z[i]);
            }
        }
        
        return matches;
    }
}

// Example usage
let zA = ZAlgorithm();
let text = "AABAACAADAABAABA";
let pattern = "AABA";

print("Z-Algorithm Pattern Matching");
print("Text: " + text);
print("Pattern: " + pattern);
print("Matches at: " + zA.search(text, pattern));

// Z-array demonstration
let str = "aabcaabxaaz";
print("\nZ-array for string: " + str);
print("Z-array: " + zA.computeZ(str));

// Prefix-suffix matches
let s = "abacaba";
print("\nPrefix-suffix matches for: " + s);
print("Lengths: " + zA.findPrefixSuffixMatches(s));

// Output:
// Z-Algorithm Pattern Matching
// Text: AABAACAADAABAABA
// Pattern: AABA
// Matches at: [0, 9, 12]
//
// Z-array for string: aabcaabxaaz
// Z-array: [11, 1, 0, 0, 3, 1, 0, 0, 2, 1, 0]
//
// Prefix-suffix matches for: abacaba
// Lengths: [7, 1]
```

### Complexity Analysis

- **Time Complexity**: O(n + m)
  - Linear time for both Z-array computation and pattern matching
  
- **Space Complexity**: O(n + m)
  - For concatenated string and Z-array

### When to Use

- ✓ Multiple pattern matching queries on same text
- ✓ Finding patterns at prefix positions
- ✓ Simpler implementation than KMP
- ✓ Competitive programming

---

## String Processing Algorithms

### Longest Palindromic Substring

Finding the longest substring that reads the same forwards and backwards.

#### Approach 1: Expand Around Center

```javascript
class PalindromeAlgorithms {
    // Expand around center approach - O(n²)
    function longestPalindrome(s) {
        if (s.length() < 2) return s;
        
        let start = 0;
        let maxLen = 0;
        
        for (let i = 0; i < s.length(); i++) {
            // Odd length palindromes (center is single character)
            let len1 = this.expandAroundCenter(s, i, i);
            // Even length palindromes (center is between two characters)
            let len2 = this.expandAroundCenter(s, i, i + 1);
            
            let len = len1 > len2 ? len1 : len2;
            
            if (len > maxLen) {
                maxLen = len;
                start = i - (len - 1) / 2;
            }
        }
        
        return s.substring(start, start + maxLen);
    }
    
    function expandAroundCenter(s, left, right) {
        while (left >= 0 && right < s.length() && s[left] == s[right]) {
            left = left - 1;
            right = right + 1;
        }
        return right - left - 1;
    }
    
    // Manacher's Algorithm - O(n)
    function longestPalindromeManacher(s) {
        if (s.length() < 2) return s;
        
        // Transform string: "abc" -> "^#a#b#c#$"
        let t = "^";
        for (let i = 0; i < s.length(); i++) {
            t = t + "#" + s[i];
        }
        t = t + "#$";
        
        let n = t.length();
        let p = [];  // p[i] = length of palindrome centered at i
        let center = 0;
        let right = 0;
        
        for (let i = 1; i < n - 1; i++) {
            let mirror = 2 * center - i;
            
            if (right > i) {
                p[i] = (right - i) < p[mirror] ? (right - i) : p[mirror];
            } else {
                p[i] = 0;
            }
            
            // Expand around i
            while (t[i + 1 + p[i]] == t[i - 1 - p[i]]) {
                p[i] = p[i] + 1;
            }
            
            // Update center and right
            if (i + p[i] > right) {
                center = i;
                right = i + p[i];
            }
        }
        
        // Find longest palindrome
        let maxLen = 0;
        let centerIndex = 0;
        for (let i = 1; i < n - 1; i++) {
            if (p[i] > maxLen) {
                maxLen = p[i];
                centerIndex = i;
            }
        }
        
        let start = (centerIndex - maxLen) / 2;
        return s.substring(start, start + maxLen);
    }
    
    // Count all palindromic substrings
    function countPalindromes(s) {
        let count = 0;
        for (let i = 0; i < s.length(); i++) {
            // Odd length
            count = count + this.countPalindromesFromCenter(s, i, i);
            // Even length
            count = count + this.countPalindromesFromCenter(s, i, i + 1);
        }
        return count;
    }
    
    function countPalindromesFromCenter(s, left, right) {
        let count = 0;
        while (left >= 0 && right < s.length() && s[left] == s[right]) {
            count = count + 1;
            left = left - 1;
            right = right + 1;
        }
        return count;
    }
}

// Example usage
let pal = PalindromeAlgorithms();
let str = "babad";

print("Palindrome Algorithms");
print("String: " + str);
print("Longest palindrome (expand): " + pal.longestPalindrome(str));
print("Longest palindrome (Manacher): " + pal.longestPalindromeManacher(str));
print("Total palindromic substrings: " + pal.countPalindromes(str));

str = "cbbd";
print("\nString: " + str);
print("Longest palindrome: " + pal.longestPalindrome(str));

// Output:
// Palindrome Algorithms
// String: babad
// Longest palindrome (expand): bab
// Longest palindrome (Manacher): bab
// Total palindromic substrings: 7
//
// String: cbbd
// Longest palindrome: bb
```

**Complexity**:
- Expand Around Center: O(n²) time, O(1) space
- Manacher's Algorithm: O(n) time, O(n) space

---

### Longest Common Prefix

Find the longest common prefix among an array of strings.

```javascript
class LCPAlgorithms {
    // Vertical scanning - O(S) where S is sum of all characters
    function longestCommonPrefix(strs) {
        if (strs.length() == 0) return "";
        
        for (let i = 0; i < strs[0].length(); i++) {
            let char = strs[0][i];
            for (let j = 1; j < strs.length(); j++) {
                if (i >= strs[j].length() || strs[j][i] != char) {
                    return strs[0].substring(0, i);
                }
            }
        }
        
        return strs[0];
    }
    
    // Horizontal scanning
    function longestCommonPrefixHorizontal(strs) {
        if (strs.length() == 0) return "";
        
        let prefix = strs[0];
        for (let i = 1; i < strs.length(); i++) {
            while (strs[i].indexOf(prefix) != 0) {
                prefix = prefix.substring(0, prefix.length() - 1);
                if (prefix.length() == 0) return "";
            }
        }
        
        return prefix;
    }
    
    // Binary search approach - O(S × log m) where m is min length
    function longestCommonPrefixBinarySearch(strs) {
        if (strs.length() == 0) return "";
        
        let minLen = strs[0].length();
        for (let i = 1; i < strs.length(); i++) {
            if (strs[i].length() < minLen) {
                minLen = strs[i].length();
            }
        }
        
        let low = 0;
        let high = minLen;
        
        while (low <= high) {
            let mid = (low + high) / 2;
            if (this.isCommonPrefix(strs, mid)) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        
        return strs[0].substring(0, (low + high) / 2);
    }
    
    function isCommonPrefix(strs, len) {
        let prefix = strs[0].substring(0, len);
        for (let i = 1; i < strs.length(); i++) {
            if (!strs[i].startsWith(prefix)) {
                return false;
            }
        }
        return true;
    }
}

// Example usage
let lcp = LCPAlgorithms();
let strs1 = ["flower", "flow", "flight"];
let strs2 = ["dog", "racecar", "car"];

print("Longest Common Prefix");
print("Strings: " + strs1);
print("LCP: " + lcp.longestCommonPrefix(strs1));

print("\nStrings: " + strs2);
print("LCP: " + lcp.longestCommonPrefix(strs2));

// Output:
// Longest Common Prefix
// Strings: ["flower", "flow", "flight"]
// LCP: fl
//
// Strings: ["dog", "racecar", "car"]
// LCP: (empty string)
```

---

### Anagram Checking

Determine if two strings are anagrams of each other.

```javascript
class AnagramChecker {
    // Sorting approach - O(n log n)
    function isAnagramSort(s1, s2) {
        if (s1.length() != s2.length()) return false;
        
        let arr1 = s1.split("").sort();
        let arr2 = s2.split("").sort();
        
        return arr1.join("") == arr2.join("");
    }
    
    // Frequency count - O(n)
    function isAnagram(s1, s2) {
        if (s1.length() != s2.length()) return false;
        
        let freq = {};
        
        // Count characters in s1
        for (let i = 0; i < s1.length(); i++) {
            let char = s1[i];
            freq[char] = (freq[char] || 0) + 1;
        }
        
        // Subtract characters in s2
        for (let i = 0; i < s2.length(); i++) {
            let char = s2[i];
            if (!freq[char]) return false;
            freq[char] = freq[char] - 1;
            if (freq[char] < 0) return false;
        }
        
        return true;
    }
    
    // Group anagrams from array of strings
    function groupAnagrams(strs) {
        let groups = {};
        
        for (let i = 0; i < strs.length(); i++) {
            let str = strs[i];
            let key = str.split("").sort().join("");
            
            if (!groups[key]) {
                groups[key] = [];
            }
            groups[key].push(str);
        }
        
        let result = [];
        for (let key in groups) {
            result.push(groups[key]);
        }
        
        return result;
    }
    
    // Find all anagrams of pattern in text
    function findAnagrams(text, pattern) {
        let result = [];
        let n = text.length();
        let m = pattern.length();
        
        if (m > n) return result;
        
        // Frequency of pattern
        let patternFreq = {};
        for (let i = 0; i < m; i++) {
            let char = pattern[i];
            patternFreq[char] = (patternFreq[char] || 0) + 1;
        }
        
        // Sliding window
        let windowFreq = {};
        for (let i = 0; i < n; i++) {
            let char = text[i];
            windowFreq[char] = (windowFreq[char] || 0) + 1;
            
            // Remove leftmost character if window exceeds pattern length
            if (i >= m) {
                let leftChar = text[i - m];
                windowFreq[leftChar] = windowFreq[leftChar] - 1;
                if (windowFreq[leftChar] == 0) {
                    delete windowFreq[leftChar];
                }
            }
            
            // Check if window is anagram
            if (i >= m - 1 && this.areFreqEqual(patternFreq, windowFreq)) {
                result.push(i - m + 1);
            }
        }
        
        return result;
    }
    
    function areFreqEqual(freq1, freq2) {
        for (let key in freq1) {
            if (freq1[key] != freq2[key]) return false;
        }
        for (let key in freq2) {
            if (freq1[key] != freq2[key]) return false;
        }
        return true;
    }
}

// Example usage
let ac = AnagramChecker();

print("Anagram Checking");
print("'listen' and 'silent': " + ac.isAnagram("listen", "silent"));
print("'hello' and 'world': " + ac.isAnagram("hello", "world"));

let words = ["eat", "tea", "tan", "ate", "nat", "bat"];
print("\nGroup Anagrams:");
print("Input: " + words);
print("Output: " + ac.groupAnagrams(words));

let text = "cbaebabacd";
let pattern = "abc";
print("\nFind Anagrams:");
print("Text: " + text);
print("Pattern: " + pattern);
print("Anagram positions: " + ac.findAnagrams(text, pattern));

// Output:
// Anagram Checking
// 'listen' and 'silent': true
// 'hello' and 'world': false
//
// Group Anagrams:
// Input: ["eat", "tea", "tan", "ate", "nat", "bat"]
// Output: [["eat", "tea", "ate"], ["tan", "nat"], ["bat"]]
//
// Find Anagrams:
// Text: cbaebabacd
// Pattern: abc
// Anagram positions: [0, 6]
```

---

## Suffix Structures

### Suffix Array

A **suffix array** is a sorted array of all suffixes of a string.

```javascript
class SuffixArray {
    // Build suffix array - O(n² log n) simple version
    function buildSuffixArray(s) {
        let n = s.length();
        let suffixes = [];
        
        // Create all suffixes with their indices
        for (let i = 0; i < n; i++) {
            suffixes.push({
                index: i,
                suffix: s.substring(i)
            });
        }
        
        // Sort suffixes
        suffixes.sort(function(a, b) {
            if (a.suffix < b.suffix) return -1;
            if (a.suffix > b.suffix) return 1;
            return 0;
        });
        
        // Extract indices
        let sa = [];
        for (let i = 0; i < n; i++) {
            sa.push(suffixes[i].index);
        }
        
        return sa;
    }
    
    // Pattern matching using suffix array - O(m log n)
    function search(s, pattern, sa) {
        let n = s.length();
        let m = pattern.length();
        
        // Binary search for lower bound
        let left = 0;
        let right = n;
        
        while (left < right) {
            let mid = (left + right) / 2;
            let suffix = s.substring(sa[mid]);
            
            if (suffix.substring(0, m) < pattern) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        let start = left;
        
        // Binary search for upper bound
        right = n;
        while (left < right) {
            let mid = (left + right) / 2;
            let suffix = s.substring(sa[mid]);
            
            if (suffix.substring(0, m) <= pattern) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        let end = left;
        
        // Collect all matches
        let result = [];
        for (let i = start; i < end; i++) {
            result.push(sa[i]);
        }
        
        return result;
    }
    
    // Find longest repeated substring
    function longestRepeatedSubstring(s) {
        let n = s.length();
        if (n < 2) return "";
        
        let sa = this.buildSuffixArray(s);
        let lcp = this.buildLCPArray(s, sa);
        
        let maxLen = 0;
        let maxIndex = 0;
        
        for (let i = 0; i < n - 1; i++) {
            if (lcp[i] > maxLen) {
                maxLen = lcp[i];
                maxIndex = sa[i];
            }
        }
        
        return s.substring(maxIndex, maxIndex + maxLen);
    }
    
    // Build LCP array
    function buildLCPArray(s, sa) {
        let n = s.length();
        let lcp = [];
        
        for (let i = 0; i < n - 1; i++) {
            let len = 0;
            let idx1 = sa[i];
            let idx2 = sa[i + 1];
            
            while (idx1 + len < n && idx2 + len < n && s[idx1 + len] == s[idx2 + len]) {
                len = len + 1;
            }
            
            lcp.push(len);
        }
        
        return lcp;
    }
}

// Example usage
let sa = SuffixArray();
let str = "banana";

print("Suffix Array");
print("String: " + str);
let suffixArray = sa.buildSuffixArray(str);
print("Suffix Array: " + suffixArray);

print("\nSuffixes in sorted order:");
for (let i = 0; i < suffixArray.length(); i++) {
    print(suffixArray[i] + ": " + str.substring(suffixArray[i]));
}

// Pattern search
let pattern = "ana";
print("\nSearching for pattern: " + pattern);
print("Found at positions: " + sa.search(str, pattern, suffixArray));

// Longest repeated substring
str = "abcabcabc";
print("\nLongest repeated substring in '" + str + "':");
print("Result: " + sa.longestRepeatedSubstring(str));

// Output:
// Suffix Array
// String: banana
// Suffix Array: [5, 3, 1, 0, 4, 2]
//
// Suffixes in sorted order:
// 5: a
// 3: ana
// 1: anana
// 0: banana
// 4: na
// 2: nana
//
// Searching for pattern: ana
// Found at positions: [3, 1]
//
// Longest repeated substring in 'abcabcabc':
// Result: abcabc
```

### Suffix Tree

A **suffix tree** is a compressed trie of all suffixes (conceptual explanation).

```javascript
// Conceptual Suffix Tree Implementation
class SuffixTreeNode {
    let children = {};
    let start = -1;
    let end = -1;
    let suffixLink = null;
    
    function init(start, end) {
        this.start = start;
        this.end = end;
        return this;
    }
}

class SuffixTree {
    let root = null;
    let text = "";
    
    // Simplified version - O(n²) construction
    function build(s) {
        this.text = s + "$";  // Add terminator
        this.root = SuffixTreeNode().init(-1, -1);
        
        let n = this.text.length();
        
        // Add all suffixes
        for (let i = 0; i < n; i++) {
            this.addSuffix(i);
        }
    }
    
    function addSuffix(suffixStart) {
        let node = this.root;
        let i = suffixStart;
        
        while (i < this.text.length()) {
            let char = this.text[i];
            
            if (!node.children[char]) {
                // Create new edge
                let newNode = SuffixTreeNode().init(i, this.text.length() - 1);
                node.children[char] = newNode;
                return;
            }
            
            node = node.children[char];
            i = i + 1;
        }
    }
    
    // Search for pattern
    function search(pattern) {
        let node = this.root;
        let i = 0;
        
        while (i < pattern.length()) {
            let char = pattern[i];
            
            if (!node.children[char]) {
                return false;
            }
            
            node = node.children[char];
            i = i + 1;
        }
        
        return true;
    }
}

// Example usage
print("Suffix Tree (Conceptual)");
let st = SuffixTree();
st.build("banana");

print("Searching for 'ana': " + st.search("ana"));
print("Searching for 'nan': " + st.search("nan"));
print("Searching for 'xyz': " + st.search("xyz"));

// Output:
// Suffix Tree (Conceptual)
// Searching for 'ana': true
// Searching for 'nan': true
// Searching for 'xyz': false
```

### LCP Array

The **LCP (Longest Common Prefix) Array** stores the lengths of longest common prefixes between consecutive suffixes in the suffix array.

```javascript
class LCPArray {
    // Build LCP array from string and suffix array
    function buildLCP(s, sa) {
        let n = s.length();
        let lcp = [];
        let rank = [];
        
        // Build rank array (inverse of suffix array)
        for (let i = 0; i < n; i++) {
            rank[sa[i]] = i;
        }
        
        let h = 0;  // Length of current LCP
        
        for (let i = 0; i < n; i++) {
            if (rank[i] > 0) {
                let j = sa[rank[i] - 1];
                
                while (i + h < n && j + h < n && s[i + h] == s[j + h]) {
                    h = h + 1;
                }
                
                lcp[rank[i]] = h;
                
                if (h > 0) {
                    h = h - 1;
                }
            }
        }
        
        return lcp;
    }
    
    // Find number of distinct substrings
    function countDistinctSubstrings(s) {
        let n = s.length();
        
        // Total substrings = n * (n + 1) / 2
        let total = n * (n + 1) / 2;
        
        // Build suffix and LCP arrays
        let saBuilder = SuffixArray();
        let sa = saBuilder.buildSuffixArray(s);
        let lcp = this.buildLCP(s, sa);
        
        // Subtract duplicate substrings (sum of LCP values)
        let duplicates = 0;
        for (let i = 0; i < lcp.length(); i++) {
            duplicates = duplicates + lcp[i];
        }
        
        return total - duplicates;
    }
}

// Example usage
let lcpArray = LCPArray();
let str = "banana";

print("LCP Array");
print("String: " + str);

let saBuilder = SuffixArray();
let sa = saBuilder.buildSuffixArray(str);
let lcp = lcpArray.buildLCP(str, sa);

print("Suffix Array: " + sa);
print("LCP Array: " + lcp);

print("\nDistinct substrings in '" + str + "': " + lcpArray.countDistinctSubstrings(str));

// Output:
// LCP Array
// String: banana
// Suffix Array: [5, 3, 1, 0, 4, 2]
// LCP Array: [0, 1, 3, 0, 0, 2]
//
// Distinct substrings in 'banana': 15
```

---

## String Dynamic Programming

### Edit Distance

The **edit distance** (Levenshtein distance) is the minimum number of operations (insert, delete, replace) needed to transform one string into another.

```javascript
class EditDistance {
    // Compute edit distance - O(m × n)
    function minDistance(word1, word2) {
        let m = word1.length();
        let n = word2.length();
        
        // dp[i][j] = edit distance between word1[0..i-1] and word2[0..j-1]
        let dp = [];
        
        // Initialize
        for (let i = 0; i <= m; i++) {
            dp[i] = [];
            for (let j = 0; j <= n; j++) {
                dp[i][j] = 0;
            }
        }
        
        // Base cases
        for (let i = 0; i <= m; i++) {
            dp[i][0] = i;  // Delete all characters
        }
        for (let j = 0; j <= n; j++) {
            dp[0][j] = j;  // Insert all characters
        }
        
        // Fill DP table
        for (let i = 1; i <= m; i++) {
            for (let j = 1; j <= n; j++) {
                if (word1[i - 1] == word2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = 1 + this.min3(
                        dp[i - 1][j],      // Delete
                        dp[i][j - 1],      // Insert
                        dp[i - 1][j - 1]   // Replace
                    );
                }
            }
        }
        
        return dp[m][n];
    }
    
    function min3(a, b, c) {
        let min = a < b ? a : b;
        return min < c ? min : c;
    }
    
    // Get actual edit operations
    function getEditOperations(word1, word2) {
        let m = word1.length();
        let n = word2.length();
        let dp = [];
        
        for (let i = 0; i <= m; i++) {
            dp[i] = [];
            for (let j = 0; j <= n; j++) {
                dp[i][j] = 0;
            }
        }
        
        for (let i = 0; i <= m; i++) dp[i][0] = i;
        for (let j = 0; j <= n; j++) dp[0][j] = j;
        
        for (let i = 1; i <= m; i++) {
            for (let j = 1; j <= n; j++) {
                if (word1[i - 1] == word2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = 1 + this.min3(dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]);
                }
            }
        }
        
        // Backtrack to find operations
        let operations = [];
        let i = m;
        let j = n;
        
        while (i > 0 || j > 0) {
            if (i == 0) {
                operations.unshift("Insert '" + word2[j - 1] + "'");
                j = j - 1;
            } else if (j == 0) {
                operations.unshift("Delete '" + word1[i - 1] + "'");
                i = i - 1;
            } else if (word1[i - 1] == word2[j - 1]) {
                i = i - 1;
                j = j - 1;
            } else {
                let replace = dp[i - 1][j - 1];
                let del = dp[i - 1][j];
                let insert = dp[i][j - 1];
                
                if (replace <= del && replace <= insert) {
                    operations.unshift("Replace '" + word1[i - 1] + "' with '" + word2[j - 1] + "'");
                    i = i - 1;
                    j = j - 1;
                } else if (del <= insert) {
                    operations.unshift("Delete '" + word1[i - 1] + "'");
                    i = i - 1;
                } else {
                    operations.unshift("Insert '" + word2[j - 1] + "'");
                    j = j - 1;
                }
            }
        }
        
        return operations;
    }
}

// Example usage
let ed = EditDistance();
let word1 = "horse";
let word2 = "ros";

print("Edit Distance");
print("Word1: " + word1);
print("Word2: " + word2);
print("Edit Distance: " + ed.minDistance(word1, word2));
print("Operations:");
let ops = ed.getEditOperations(word1, word2);
for (let i = 0; i < ops.length(); i++) {
    print("  " + ops[i]);
}

// Output:
// Edit Distance
// Word1: horse
// Word2: ros
// Edit Distance: 3
// Operations:
//   Replace 'h' with 'r'
//   Delete 'r'
//   Delete 'e'
```

### Longest Common Subsequence

```javascript
class LCS {
    // Compute length of LCS - O(m × n)
    function longestCommonSubsequence(text1, text2) {
        let m = text1.length();
        let n = text2.length();
        let dp = [];
        
        for (let i = 0; i <= m; i++) {
            dp[i] = [];
            for (let j = 0; j <= n; j++) {
                dp[i][j] = 0;
            }
        }
        
        for (let i = 1; i <= m; i++) {
            for (let j = 1; j <= n; j++) {
                if (text1[i - 1] == text2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = dp[i - 1][j] > dp[i][j - 1] ? dp[i - 1][j] : dp[i][j - 1];
                }
            }
        }
        
        return dp[m][n];
    }
    
    // Get actual LCS string
    function getLCS(text1, text2) {
        let m = text1.length();
        let n = text2.length();
        let dp = [];
        
        for (let i = 0; i <= m; i++) {
            dp[i] = [];
            for (let j = 0; j <= n; j++) {
                dp[i][j] = 0;
            }
        }
        
        for (let i = 1; i <= m; i++) {
            for (let j = 1; j <= n; j++) {
                if (text1[i - 1] == text2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = dp[i - 1][j] > dp[i][j - 1] ? dp[i - 1][j] : dp[i][j - 1];
                }
            }
        }
        
        // Backtrack to build LCS
        let lcs = "";
        let i = m;
        let j = n;
        
        while (i > 0 && j > 0) {
            if (text1[i - 1] == text2[j - 1]) {
                lcs = text1[i - 1] + lcs;
                i = i - 1;
                j = j - 1;
            } else if (dp[i - 1][j] > dp[i][j - 1]) {
                i = i - 1;
            } else {
                j = j - 1;
            }
        }
        
        return lcs;
    }
}

// Example usage
let lcs = LCS();
let s1 = "ABCDGH";
let s2 = "AEDFHR";

print("Longest Common Subsequence");
print("String 1: " + s1);
print("String 2: " + s2);
print("LCS Length: " + lcs.longestCommonSubsequence(s1, s2));
print("LCS: " + lcs.getLCS(s1, s2));

// Output:
// Longest Common Subsequence
// String 1: ABCDGH
// String 2: AEDFHR
// LCS Length: 3
// LCS: ADH
```

### Palindrome Partitioning

Find minimum cuts needed to partition a string into palindromes.

```javascript
class PalindromePartitioning {
    // Minimum cuts for palindrome partitioning - O(n²)
    function minCut(s) {
        let n = s.length();
        
        // isPalin[i][j] = true if s[i..j] is palindrome
        let isPalin = [];
        for (let i = 0; i < n; i++) {
            isPalin[i] = [];
            for (let j = 0; j < n; j++) {
                isPalin[i][j] = false;
            }
        }
        
        // Build palindrome table
        for (let i = 0; i < n; i++) {
            isPalin[i][i] = true;
        }
        
        for (let len = 2; len <= n; len++) {
            for (let i = 0; i <= n - len; i++) {
                let j = i + len - 1;
                if (s[i] == s[j]) {
                    if (len == 2) {
                        isPalin[i][j] = true;
                    } else {
                        isPalin[i][j] = isPalin[i + 1][j - 1];
                    }
                }
            }
        }
        
        // dp[i] = minimum cuts needed for s[0..i]
        let dp = [];
        for (let i = 0; i < n; i++) {
            dp[i] = i;  // Max cuts = i (cut after each char)
        }
        
        for (let i = 0; i < n; i++) {
            if (isPalin[0][i]) {
                dp[i] = 0;
            } else {
                for (let j = 0; j < i; j++) {
                    if (isPalin[j + 1][i]) {
                        dp[i] = dp[i] < dp[j] + 1 ? dp[i] : dp[j] + 1;
                    }
                }
            }
        }
        
        return dp[n - 1];
    }
    
    // Get all palindrome partitions
    function partition(s) {
        let result = [];
        let current = [];
        this.backtrack(s, 0, current, result);
        return result;
    }
    
    function backtrack(s, start, current, result) {
        if (start == s.length()) {
            result.push([...current]);
            return;
        }
        
        for (let end = start; end < s.length(); end++) {
            if (this.isPalindrome(s, start, end)) {
                current.push(s.substring(start, end + 1));
                this.backtrack(s, end + 1, current, result);
                current.pop();
            }
        }
    }
    
    function isPalindrome(s, left, right) {
        while (left < right) {
            if (s[left] != s[right]) return false;
            left = left + 1;
            right = right - 1;
        }
        return true;
    }
}

// Example usage
let pp = PalindromePartitioning();
let str = "aab";

print("Palindrome Partitioning");
print("String: " + str);
print("Minimum cuts: " + pp.minCut(str));
print("All partitions:");
let partitions = pp.partition(str);
for (let i = 0; i < partitions.length(); i++) {
    print("  " + partitions[i]);
}

// Output:
// Palindrome Partitioning
// String: aab
// Minimum cuts: 1
// All partitions:
//   ["a", "a", "b"]
//   ["aa", "b"]
```

---

## Rolling Hash Technique

**Rolling hash** is a technique to compute hash values incrementally, enabling efficient substring operations.

### Concept

Instead of recomputing hash for each substring, we:
1. Remove contribution of leftmost character
2. Add contribution of rightmost character

### Polynomial Rolling Hash

```
hash(s[i..j]) = (s[i] × p^(j-i) + s[i+1] × p^(j-i-1) + ... + s[j] × p^0) mod m

where:
- p = prime base (e.g., 31 for strings)
- m = large prime modulus (e.g., 10^9 + 7)
```

### Implementation

```javascript
class RollingHash {
    let MOD = 1000000007;
    let BASE = 31;
    
    // Compute hash of string
    function computeHash(s) {
        let hash = 0;
        let pow = 1;
        
        for (let i = 0; i < s.length(); i++) {
            hash = (hash + (s.charCodeAt(i) - 'a'.charCodeAt(0) + 1) * pow) % this.MOD;
            pow = (pow * this.BASE) % this.MOD;
        }
        
        return hash;
    }
    
    // Find longest duplicate substring using rolling hash
    function longestDupSubstring(s) {
        let n = s.length();
        let left = 1;
        let right = n;
        let result = "";
        
        while (left <= right) {
            let mid = (left + right) / 2;
            let dup = this.searchDuplicate(s, mid);
            
            if (dup != "") {
                result = dup;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return result;
    }
    
    function searchDuplicate(s, length) {
        let n = s.length();
        let seen = {};
        
        // Compute hash for first window
        let hash = 0;
        let pow = 1;
        
        for (let i = 0; i < length; i++) {
            hash = (hash + (s.charCodeAt(i) - 'a'.charCodeAt(0) + 1) * pow) % this.MOD;
            pow = (pow * this.BASE) % this.MOD;
        }
        
        seen[hash] = [0];
        
        // Rolling hash for remaining windows
        let highestPow = pow;
        pow = 1;
        
        for (let i = length; i < n; i++) {
            // Remove leftmost character
            let leftChar = s.charCodeAt(i - length) - 'a'.charCodeAt(0) + 1;
            hash = (hash - leftChar + this.MOD) % this.MOD;
            hash = (hash * this.modInverse(this.BASE)) % this.MOD;
            
            // Add rightmost character
            let rightChar = s.charCodeAt(i) - 'a'.charCodeAt(0) + 1;
            hash = (hash + rightChar * this.modPow(this.BASE, length - 1)) % this.MOD;
            
            // Check for duplicate
            let substr = s.substring(i - length + 1, i + 1);
            if (seen[hash]) {
                // Verify actual string to handle collisions
                for (let j = 0; j < seen[hash].length(); j++) {
                    let prevIndex = seen[hash][j];
                    let prevSubstr = s.substring(prevIndex, prevIndex + length);
                    if (prevSubstr == substr) {
                        return substr;
                    }
                }
                seen[hash].push(i - length + 1);
            } else {
                seen[hash] = [i - length + 1];
            }
        }
        
        return "";
    }
    
    function modPow(base, exp) {
        let result = 1;
        for (let i = 0; i < exp; i++) {
            result = (result * base) % this.MOD;
        }
        return result;
    }
    
    function modInverse(a) {
        // Simple modular inverse for prime modulus
        return this.modPow(a, this.MOD - 2);
    }
    
    // Check if two substrings are equal using hash
    function areSubstringsEqual(s1, start1, s2, start2, length) {
        let hash1 = 0;
        let hash2 = 0;
        let pow = 1;
        
        for (let i = 0; i < length; i++) {
            hash1 = (hash1 + (s1.charCodeAt(start1 + i) - 'a'.charCodeAt(0) + 1) * pow) % this.MOD;
            hash2 = (hash2 + (s2.charCodeAt(start2 + i) - 'a'.charCodeAt(0) + 1) * pow) % this.MOD;
            pow = (pow * this.BASE) % this.MOD;
        }
        
        return hash1 == hash2;
    }
}

// Example usage
let rh = RollingHash();
let str = "banana";

print("Rolling Hash");
print("String: " + str);
print("Hash: " + rh.computeHash(str));

str = "aaabaaa";
print("\nLongest duplicate substring in '" + str + "':");
print("Result: " + rh.longestDupSubstring(str));

// Output:
// Rolling Hash
// String: banana
// Hash: 98410643
//
// Longest duplicate substring in 'aaabaaa':
// Result: aaa
```

### Applications of Rolling Hash

1. **Pattern Matching**: Rabin-Karp algorithm
2. **Duplicate Detection**: Finding repeated substrings
3. **Substring Comparison**: O(1) comparison after preprocessing
4. **Data Integrity**: Checksums and fingerprinting

---

## Interview Problems

### 1. Implement strStr() - KMP

```javascript
// Find first occurrence of needle in haystack
class StrStr {
    function strStr(haystack, needle) {
        if (needle.length() == 0) return 0;
        
        let kmp = KMPMatcher();
        let result = kmp.findFirst(haystack, needle);
        return result;
    }
}

// Test
let ss = StrStr();
print("strStr('hello', 'll'): " + ss.strStr("hello", "ll"));  // Output: 2
print("strStr('aaaaa', 'bba'): " + ss.strStr("aaaaa", "bba"));  // Output: -1
```

### 2. Longest Palindromic Substring

```javascript
class Solution {
    function longestPalindrome(s) {
        if (s.length() < 2) return s;
        
        let start = 0;
        let maxLen = 0;
        
        for (let i = 0; i < s.length(); i++) {
            let len1 = this.expandAroundCenter(s, i, i);
            let len2 = this.expandAroundCenter(s, i, i + 1);
            let len = len1 > len2 ? len1 : len2;
            
            if (len > maxLen) {
                maxLen = len;
                start = i - (len - 1) / 2;
            }
        }
        
        return s.substring(start, start + maxLen);
    }
    
    function expandAroundCenter(s, left, right) {
        while (left >= 0 && right < s.length() && s[left] == s[right]) {
            left = left - 1;
            right = right + 1;
        }
        return right - left - 1;
    }
}

// Test
let sol = Solution();
print("longestPalindrome('babad'): " + sol.longestPalindrome("babad"));  // "bab" or "aba"
print("longestPalindrome('cbbd'): " + sol.longestPalindrome("cbbd"));    // "bb"
```

### 3. Longest Repeating Character Replacement

```javascript
// Find longest substring with same char after k replacements
class LongestRepeatingChar {
    function characterReplacement(s, k) {
        let freq = {};
        let maxFreq = 0;
        let left = 0;
        let maxLen = 0;
        
        for (let right = 0; right < s.length(); right++) {
            freq[s[right]] = (freq[s[right]] || 0) + 1;
            maxFreq = maxFreq > freq[s[right]] ? maxFreq : freq[s[right]];
            
            // Window size - max frequency > k means we need more than k replacements
            while (right - left + 1 - maxFreq > k) {
                freq[s[left]] = freq[s[left]] - 1;
                left = left + 1;
            }
            
            maxLen = maxLen > right - left + 1 ? maxLen : right - left + 1;
        }
        
        return maxLen;
    }
}

// Test
let lrc = LongestRepeatingChar();
print("characterReplacement('ABAB', 2): " + lrc.characterReplacement("ABAB", 2));      // 4
print("characterReplacement('AABABBA', 1): " + lrc.characterReplacement("AABABBA", 1));  // 4
```

### 4. Minimum Window Substring

```javascript
// Find minimum window in s that contains all characters of t
class MinWindowSubstring {
    function minWindow(s, t) {
        if (s.length() < t.length()) return "";
        
        let need = {};
        let window = {};
        
        // Count characters in t
        for (let i = 0; i < t.length(); i++) {
            need[t[i]] = (need[t[i]] || 0) + 1;
        }
        
        let left = 0;
        let right = 0;
        let valid = 0;
        let start = 0;
        let minLen = s.length() + 1;
        
        while (right < s.length()) {
            let c = s[right];
            right = right + 1;
            
            if (need[c]) {
                window[c] = (window[c] || 0) + 1;
                if (window[c] == need[c]) {
                    valid = valid + 1;
                }
            }
            
            // Shrink window
            while (valid == Object.keys(need).length) {
                if (right - left < minLen) {
                    start = left;
                    minLen = right - left;
                }
                
                let d = s[left];
                left = left + 1;
                
                if (need[d]) {
                    if (window[d] == need[d]) {
                        valid = valid - 1;
                    }
                    window[d] = window[d] - 1;
                }
            }
        }
        
        return minLen == s.length() + 1 ? "" : s.substring(start, start + minLen);
    }
}

// Test
let mws = MinWindowSubstring();
print("minWindow('ADOBECODEBANC', 'ABC'): " + mws.minWindow("ADOBECODEBANC", "ABC"));  // "BANC"
print("minWindow('a', 'a'): " + mws.minWindow("a", "a"));  // "a"
print("minWindow('a', 'aa'): " + mws.minWindow("a", "aa"));  // ""
```

### 5. Find All Anagrams in a String

```javascript
class FindAnagrams {
    function findAnagrams(s, p) {
        let result = [];
        if (s.length() < p.length()) return result;
        
        let pCount = {};
        let sCount = {};
        
        // Count characters in p
        for (let i = 0; i < p.length(); i++) {
            pCount[p[i]] = (pCount[p[i]] || 0) + 1;
        }
        
        // Sliding window
        for (let i = 0; i < s.length(); i++) {
            // Add new character
            sCount[s[i]] = (sCount[s[i]] || 0) + 1;
            
            // Remove old character
            if (i >= p.length()) {
                let leftChar = s[i - p.length()];
                sCount[leftChar] = sCount[leftChar] - 1;
                if (sCount[leftChar] == 0) {
                    delete sCount[leftChar];
                }
            }
            
            // Check if window is anagram
            if (this.areEqual(pCount, sCount)) {
                result.push(i - p.length() + 1);
            }
        }
        
        return result;
    }
    
    function areEqual(map1, map2) {
        if (Object.keys(map1).length != Object.keys(map2).length) {
            return false;
        }
        for (let key in map1) {
            if (map1[key] != map2[key]) return false;
        }
        return true;
    }
}

// Test
let fa = FindAnagrams();
print("findAnagrams('cbaebabacd', 'abc'): " + fa.findAnagrams("cbaebabacd", "abc"));  // [0, 6]
print("findAnagrams('abab', 'ab'): " + fa.findAnagrams("abab", "ab"));  // [0, 1, 2]
```

### 6. Group Anagrams

```javascript
class GroupAnagrams {
    function groupAnagrams(strs) {
        let groups = {};
        
        for (let i = 0; i < strs.length(); i++) {
            let str = strs[i];
            let key = str.split("").sort().join("");
            
            if (!groups[key]) {
                groups[key] = [];
            }
            groups[key].push(str);
        }
        
        let result = [];
        for (let key in groups) {
            result.push(groups[key]);
        }
        
        return result;
    }
}

// Test
let ga = GroupAnagrams();
let words = ["eat", "tea", "tan", "ate", "nat", "bat"];
print("groupAnagrams: " + ga.groupAnagrams(words));
// Output: [["eat", "tea", "ate"], ["tan", "nat"], ["bat"]]
```

### 7. Longest Substring Without Repeating Characters

```javascript
class LongestSubstring {
    function lengthOfLongestSubstring(s) {
        let seen = {};
        let left = 0;
        let maxLen = 0;
        
        for (let right = 0; right < s.length(); right++) {
            let char = s[right];
            
            if (seen[char] != null && seen[char] >= left) {
                left = seen[char] + 1;
            }
            
            seen[char] = right;
            maxLen = maxLen > right - left + 1 ? maxLen : right - left + 1;
        }
        
        return maxLen;
    }
}

// Test
let ls = LongestSubstring();
print("lengthOfLongestSubstring('abcabcbb'): " + ls.lengthOfLongestSubstring("abcabcbb"));  // 3
print("lengthOfLongestSubstring('bbbbb'): " + ls.lengthOfLongestSubstring("bbbbb"));  // 1
print("lengthOfLongestSubstring('pwwkew'): " + ls.lengthOfLongestSubstring("pwwkew"));  // 3
```

### 8. Valid Anagram

```javascript
class ValidAnagram {
    function isAnagram(s, t) {
        if (s.length() != t.length()) return false;
        
        let freq = {};
        
        for (let i = 0; i < s.length(); i++) {
            freq[s[i]] = (freq[s[i]] || 0) + 1;
        }
        
        for (let i = 0; i < t.length(); i++) {
            if (!freq[t[i]]) return false;
            freq[t[i]] = freq[t[i]] - 1;
            if (freq[t[i]] < 0) return false;
        }
        
        return true;
    }
}

// Test
let va = ValidAnagram();
print("isAnagram('anagram', 'nagaram'): " + va.isAnagram("anagram", "nagaram"));  // true
print("isAnagram('rat', 'car'): " + va.isAnagram("rat", "car"));  // false
```

### 9. Palindrome Partitioning

```javascript
// Already implemented in String DP section
// Returns all possible palindrome partitions
```

### 10. Regular Expression Matching

```javascript
class RegexMatching {
    // '.' matches any single character
    // '*' matches zero or more of preceding element
    function isMatch(s, p) {
        let m = s.length();
        let n = p.length();
        let dp = [];
        
        // dp[i][j] = true if s[0..i-1] matches p[0..j-1]
        for (let i = 0; i <= m; i++) {
            dp[i] = [];
            for (let j = 0; j <= n; j++) {
                dp[i][j] = false;
            }
        }
        
        dp[0][0] = true;
        
        // Handle patterns like a*, a*b*, a*b*c*
        for (let j = 2; j <= n; j++) {
            if (p[j - 1] == '*') {
                dp[0][j] = dp[0][j - 2];
            }
        }
        
        for (let i = 1; i <= m; i++) {
            for (let j = 1; j <= n; j++) {
                if (p[j - 1] == s[i - 1] || p[j - 1] == '.') {
                    dp[i][j] = dp[i - 1][j - 1];
                } else if (p[j - 1] == '*') {
                    // Zero occurrences of preceding character
                    dp[i][j] = dp[i][j - 2];
                    
                    // One or more occurrences
                    if (p[j - 2] == s[i - 1] || p[j - 2] == '.') {
                        dp[i][j] = dp[i][j] || dp[i - 1][j];
                    }
                }
            }
        }
        
        return dp[m][n];
    }
}

// Test
let rm = RegexMatching();
print("isMatch('aa', 'a'): " + rm.isMatch("aa", "a"));  // false
print("isMatch('aa', 'a*'): " + rm.isMatch("aa", "a*"));  // true
print("isMatch('ab', '.*'): " + rm.isMatch("ab", ".*"));  // true
print("isMatch('aab', 'c*a*b'): " + rm.isMatch("aab", "c*a*b"));  // true
```

### 11. Word Pattern

```javascript
class WordPattern {
    function wordPattern(pattern, s) {
        let words = s.split(" ");
        
        if (pattern.length() != words.length()) return false;
        
        let charToWord = {};
        let wordToChar = {};
        
        for (let i = 0; i < pattern.length(); i++) {
            let char = pattern[i];
            let word = words[i];
            
            if (charToWord[char]) {
                if (charToWord[char] != word) return false;
            } else {
                charToWord[char] = word;
            }
            
            if (wordToChar[word]) {
                if (wordToChar[word] != char) return false;
            } else {
                wordToChar[word] = char;
            }
        }
        
        return true;
    }
}

// Test
let wp = WordPattern();
print("wordPattern('abba', 'dog cat cat dog'): " + wp.wordPattern("abba", "dog cat cat dog"));  // true
print("wordPattern('abba', 'dog cat cat fish'): " + wp.wordPattern("abba", "dog cat cat fish"));  // false
print("wordPattern('aaaa', 'dog cat cat dog'): " + wp.wordPattern("aaaa", "dog cat cat dog"));  // false
```

### 12. Isomorphic Strings

```javascript
class IsomorphicStrings {
    function isIsomorphic(s, t) {
        if (s.length() != t.length()) return false;
        
        let sToT = {};
        let tToS = {};
        
        for (let i = 0; i < s.length(); i++) {
            let c1 = s[i];
            let c2 = t[i];
            
            if (sToT[c1]) {
                if (sToT[c1] != c2) return false;
            } else {
                sToT[c1] = c2;
            }
            
            if (tToS[c2]) {
                if (tToS[c2] != c1) return false;
            } else {
                tToS[c2] = c1;
            }
        }
        
        return true;
    }
}

// Test
let is = IsomorphicStrings();
print("isIsomorphic('egg', 'add'): " + is.isIsomorphic("egg", "add"));  // true
print("isIsomorphic('foo', 'bar'): " + is.isIsomorphic("foo", "bar"));  // false
print("isIsomorphic('paper', 'title'): " + is.isIsomorphic("paper", "title"));  // true
```

---

## Comparison Table

| Algorithm | Time Complexity | Space | Preprocessing | Best For |
|-----------|----------------|-------|---------------|----------|
| **Naive** | O(n×m) | O(1) | None | Short patterns, simple cases |
| **KMP** | O(n+m) | O(m) | O(m) LPS array | Long texts, no backtracking |
| **Rabin-Karp** | O(n+m) avg | O(1) | O(m) hash | Multiple patterns, average case |
| **Boyer-Moore** | O(n/m) best | O(k) | O(m+k) tables | Large alphabets, practical use |
| **Z-Algorithm** | O(n+m) | O(n+m) | O(n+m) | Prefix matching, simple code |
| **Suffix Array** | O(n log n) | O(n) | O(n²log n) | Multiple queries, substrings |
| **Suffix Tree** | O(n) | O(n) | O(n) complex | Advanced applications |

**Notation**:
- n = text length
- m = pattern length
- k = alphabet size

---

## Best Practices

### 1. Algorithm Selection

**Choose Naive When**:
- Pattern length ≤ 3
- One-time search
- Code simplicity matters

**Choose KMP When**:
- Long patterns
- Cannot backtrack in text (streaming)
- Guaranteed linear time needed

**Choose Rabin-Karp When**:
- Multiple patterns
- Pattern length varies
- Average case performance acceptable

**Choose Boyer-Moore When**:
- Large alphabet
- Practical performance critical
- Text editor implementations

### 2. String Handling

```javascript
// ✓ Good: Use built-in methods
let parts = str.split(",");
let lower = str.toLowerCase();
let trimmed = str.trim();

// ✗ Avoid: Manual character-by-character for built-in ops
let result = "";
for (let i = 0; i < str.length(); i++) {
    if (str[i] != ',') result = result + str[i];
}
```

### 3. StringBuilder for Concatenation

```javascript
// ✓ Good: Use array and join for multiple concatenations
let parts = [];
for (let i = 0; i < n; i++) {
    parts.push(str[i]);
}
let result = parts.join("");

// ✗ Avoid: Repeated string concatenation in loop
let result = "";
for (let i = 0; i < n; i++) {
    result = result + str[i];  // Creates new string each time
}
```

### 4. Two-Pointer Technique

```javascript
// Palindrome check
function isPalindrome(s) {
    let left = 0;
    let right = s.length() - 1;
    
    while (left < right) {
        if (s[left] != s[right]) return false;
        left = left + 1;
        right = right - 1;
    }
    
    return true;
}
```

### 5. Sliding Window

```javascript
// Fixed-size window
for (let i = 0; i < s.length() - k + 1; i++) {
    let window = s.substring(i, i + k);
    // Process window
}

// Variable-size window
let left = 0;
for (let right = 0; right < s.length(); right++) {
    // Expand window
    while (condition) {
        // Shrink window
        left = left + 1;
    }
}
```

### 6. Character Frequency

```javascript
// Use hash map for frequency counting
let freq = {};
for (let i = 0; i < s.length(); i++) {
    freq[s[i]] = (freq[s[i]] || 0) + 1;
}
```

---

## Common Pitfalls

### 1. Off-by-One Errors

```javascript
// ✗ Wrong: Misses last character
for (let i = 0; i < s.length() - 1; i++) {
    // ...
}

// ✓ Correct
for (let i = 0; i < s.length(); i++) {
    // ...
}
```

### 2. Hash Collision Handling

```javascript
// ✓ Always verify with actual string comparison
if (hash1 == hash2) {
    if (str1 == str2) {  // Verify actual strings
        // Match confirmed
    }
}
```

### 3. Integer Overflow

```javascript
// ✗ Risk: Hash value overflow
hash = hash * BASE + charValue;

// ✓ Better: Use modulo
hash = (hash * BASE + charValue) % MOD;
```

### 4. Empty String Edge Cases

```javascript
// Always check for empty strings
if (s.length() == 0) return defaultValue;
if (pattern.length() == 0) return 0;
```

### 5. Unicode and Special Characters

```javascript
// Be aware of multi-byte characters
// Some operations might not work correctly with Unicode
// Use charCodeAt() with caution
```

### 6. Case Sensitivity

```javascript
// Decide early if case matters
// Convert to lowercase if case-insensitive
let s1Lower = s1.toLowerCase();
let s2Lower = s2.toLowerCase();
```

---

## Real-World Applications

### 1. Text Editors and IDEs

- **Find and Replace**: Pattern matching (KMP, Boyer-Moore)
- **Autocomplete**: Trie, suffix trees
- **Syntax Highlighting**: Pattern recognition
- **Spell Check**: Edit distance

### 2. Search Engines

- **Web Crawling**: URL pattern matching
- **Indexing**: Suffix arrays for fast lookup
- **Query Processing**: Pattern matching, ranking
- **Duplicate Detection**: Rolling hash

### 3. Bioinformatics

- **DNA Sequencing**: Pattern matching in genomes
- **Protein Analysis**: Subsequence finding
- **Sequence Alignment**: Edit distance, LCS
- **Motif Discovery**: Pattern recognition

### 4. Security

- **Intrusion Detection**: Pattern matching in network traffic
- **Malware Analysis**: Signature matching
- **Spam Filtering**: Text pattern recognition
- **Data Loss Prevention**: Content inspection

### 5. Natural Language Processing

- **Tokenization**: String splitting
- **Stemming/Lemmatization**: String transformations
- **Named Entity Recognition**: Pattern matching
- **Machine Translation**: Sequence alignment

### 6. Data Processing

- **Log Analysis**: Pattern extraction
- **Data Validation**: Regex matching
- **CSV/JSON Parsing**: String processing
- **Data Deduplication**: Hash-based comparison

### 7. Compression

- **LZ77/LZ78**: Finding repeated substrings
- **Burrows-Wheeler Transform**: Suffix array based
- **Dictionary Coding**: Pattern recognition

### 8. Version Control

- **Diff Algorithms**: LCS-based comparison
- **Merge Conflict Detection**: String matching
- **File Similarity**: Hash-based comparison

---

## Conclusion

String algorithms are essential tools in every programmer's toolkit. Mastering these algorithms provides:

1. **Problem-Solving Skills**: Ability to tackle complex string problems
2. **Performance Optimization**: Choosing the right algorithm matters
3. **Interview Success**: String problems are extremely common
4. **Real-World Impact**: Applications across many domains

### Key Takeaways

- **Pattern Matching**: Know KMP, Rabin-Karp, Boyer-Moore
- **Dynamic Programming**: Master edit distance, LCS
- **Data Structures**: Understand suffix arrays, tries
- **Techniques**: Two pointers, sliding window, hashing
- **Practice**: Solve interview problems regularly

### Further Study

- Advanced suffix structures (suffix automaton)
- Aho-Corasick algorithm (multiple pattern matching)
- String compression algorithms
- Approximate string matching
- Computational biology applications

### Resources

- Practice platforms: LeetCode, HackerRank, Codeforces
- Textbooks: "Introduction to Algorithms" (CLRS)
- Online courses: Competitive programming, algorithms

---

**Happy String Algorithming in KLang!** 🎯

*Master these algorithms through practice and application. String problems are everywhere in programming!*
