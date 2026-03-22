# KLang Array Methods - Quick Reference

## Priority 1 Methods (Essential)

```javascript
// map - Transform elements
[1, 2, 3].map(x => x * 2)  // [2, 4, 6]

// filter - Select elements
[1, 2, 3, 4, 5].filter(x => x > 2)  // [3, 4, 5]

// reduce - Aggregate to single value
[1, 2, 3, 4, 5].reduce((sum, x) => sum + x, 0)  // 15

// forEach - Iterate over elements
[1, 2, 3].forEach(x => println(str(x)))

// find - Find first match
[1, 2, 3, 4, 5].find(x => x > 3)  // 4

// some - Test if any match
[1, 2, 3, 4, 5].some(x => x % 2 == 0)  // true

// every - Test if all match
[1, 2, 3, 4, 5].every(x => x > 0)  // true
```

## Priority 2 Methods (Additional)

```javascript
// indexOf - Find index
[10, 20, 30].indexOf(20)  // 1

// includes - Check if contains
[1, 2, 3].includes(2)  // true

// push - Add to end (mutates)
let arr = [1, 2]
arr.push(3)  // arr is now [1, 2, 3]

// pop - Remove from end (mutates)
let arr = [1, 2, 3]
arr.pop()  // returns 3, arr is now [1, 2]

// slice - Extract subarray
[1, 2, 3, 4, 5].slice(1, 3)  // [2, 3]

// concat - Combine arrays
[1, 2].concat([3, 4])  // [1, 2, 3, 4]

// join - Array to string
[1, 2, 3].join(", ")  // "1, 2, 3"

// reverse - Reverse order (mutates)
let arr = [1, 2, 3]
arr.reverse()  // arr is now [3, 2, 1]

// sort - Sort array (mutates)
let arr = [3, 1, 2]
arr.sort()  // arr is now [1, 2, 3]
```

## Method Chaining

```javascript
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    .filter(x => x % 2 == 0)
    .map(x => x * 2)
    .reduce((sum, x) => sum + x, 0)
// Result: 60
```

## Callback Parameters

Most methods pass 3 parameters to callbacks:
1. `element` - Current element
2. `index` - Current index
3. `array` - The array being operated on

```javascript
[10, 20, 30].map((val, idx, arr) => val + idx)
// [10, 21, 32]
```

## Immutable vs Mutating

**Immutable** (return new array):
- map, filter, slice, concat

**Mutating** (modify original):
- push, pop, reverse, sort

**No return** (side effects only):
- forEach
