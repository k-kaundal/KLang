# JavaScript Array Methods in KLang

KLang now supports essential JavaScript array methods! This document describes all available array methods and their usage.

## Table of Contents

- [Priority 1 Methods](#priority-1-methods) (Essential)
  - [map()](#map)
  - [filter()](#filter)
  - [reduce()](#reduce)
  - [forEach()](#foreach)
  - [find()](#find)
  - [some()](#some)
  - [every()](#every)
- [Priority 2 Methods](#priority-2-methods) (Additional)
  - [indexOf()](#indexof)
  - [includes()](#includes)
  - [push()](#push)
  - [pop()](#pop)
  - [slice()](#slice)
  - [concat()](#concat)
  - [join()](#join)
  - [reverse()](#reverse)
  - [sort()](#sort)

---

## Priority 1 Methods

### map()

**Syntax:** `array.map(callback)`

**Description:** Creates a new array with the results of calling a provided function on every element.

**Parameters:**
- `callback(element, index, array)` - Function called for each element

**Returns:** A new array with transformed elements

**Example:**
```javascript
const numbers = [1, 2, 3, 4, 5]
const doubled = numbers.map(x => x * 2)
// doubled = [2, 4, 6, 8, 10]

const withIndex = numbers.map((x, i) => x + i)
// withIndex = [1, 3, 5, 7, 9]
```

---

### filter()

**Syntax:** `array.filter(callback)`

**Description:** Creates a new array with all elements that pass the test implemented by the provided function.

**Parameters:**
- `callback(element, index, array)` - Function to test each element (should return true/false)

**Returns:** A new array containing only elements that pass the test

**Example:**
```javascript
const numbers = [1, 2, 3, 4, 5]
const evens = numbers.filter(x => x % 2 == 0)
// evens = [2, 4]

const greaterThan2 = numbers.filter(x => x > 2)
// greaterThan2 = [3, 4, 5]
```

---

### reduce()

**Syntax:** `array.reduce(callback, initialValue)`

**Description:** Executes a reducer function on each element, resulting in a single output value.

**Parameters:**
- `callback(accumulator, element, index, array)` - Function to execute on each element
- `initialValue` - (Optional) Initial value for accumulator

**Returns:** The accumulated result

**Example:**
```javascript
const numbers = [1, 2, 3, 4, 5]

// Sum all numbers
const sum = numbers.reduce((acc, x) => acc + x, 0)
// sum = 15

// Product without initial value (uses first element)
const product = numbers.reduce((acc, x) => acc * x)
// product = 120

// Build a string
const words = ["Hello", "World"]
const sentence = words.reduce((acc, word) => acc + " " + word, "")
// sentence = " Hello World"
```

---

### forEach()

**Syntax:** `array.forEach(callback)`

**Description:** Executes a provided function once for each array element.

**Parameters:**
- `callback(element, index, array)` - Function to execute for each element

**Returns:** `null` (undefined)

**Example:**
```javascript
const numbers = [1, 2, 3, 4, 5]

// Print each element
numbers.forEach(x => println(str(x)))

// Print with index
numbers.forEach((x, i) => println("[" + str(i) + "] = " + str(x)))
```

---

### find()

**Syntax:** `array.find(callback)`

**Description:** Returns the first element in the array that satisfies the provided testing function.

**Parameters:**
- `callback(element, index, array)` - Function to test each element

**Returns:** The first element that passes the test, or `null` if not found

**Example:**
```javascript
const numbers = [1, 2, 3, 4, 5]

const found = numbers.find(x => x > 3)
// found = 4

const notFound = numbers.find(x => x > 10)
// notFound = null
```

---

### some()

**Syntax:** `array.some(callback)`

**Description:** Tests whether at least one element passes the test implemented by the provided function.

**Parameters:**
- `callback(element, index, array)` - Function to test each element

**Returns:** `true` if at least one element passes the test, `false` otherwise

**Example:**
```javascript
const numbers = [1, 2, 3, 4, 5]

const hasEven = numbers.some(x => x % 2 == 0)
// hasEven = true

const hasNegative = numbers.some(x => x < 0)
// hasNegative = false
```

---

### every()

**Syntax:** `array.every(callback)`

**Description:** Tests whether all elements pass the test implemented by the provided function.

**Parameters:**
- `callback(element, index, array)` - Function to test each element

**Returns:** `true` if all elements pass the test, `false` otherwise

**Example:**
```javascript
const numbers = [1, 2, 3, 4, 5]

const allPositive = numbers.every(x => x > 0)
// allPositive = true

const allEven = numbers.every(x => x % 2 == 0)
// allEven = false
```

---

## Priority 2 Methods

### indexOf()

**Syntax:** `array.indexOf(value)`

**Description:** Returns the first index at which a given element can be found in the array.

**Parameters:**
- `value` - Element to locate in the array

**Returns:** The first index of the element, or `-1` if not found

**Example:**
```javascript
const numbers = [1, 2, 3, 4, 5]

const idx = numbers.indexOf(3)
// idx = 2

const notFound = numbers.indexOf(10)
// notFound = -1
```

---

### includes()

**Syntax:** `array.includes(value)`

**Description:** Determines whether an array includes a certain value.

**Parameters:**
- `value` - The value to search for

**Returns:** `true` if the array contains the value, `false` otherwise

**Example:**
```javascript
const numbers = [1, 2, 3, 4, 5]

const has3 = numbers.includes(3)
// has3 = true

const has10 = numbers.includes(10)
// has10 = false
```

---

### push()

**Syntax:** `array.push(value)`

**Description:** Adds an element to the end of an array (mutates the array).

**Parameters:**
- `value` - The element to add

**Returns:** The new length of the array

**Example:**
```javascript
let numbers = [1, 2, 3]
const newLength = numbers.push(4)
// numbers = [1, 2, 3, 4]
// newLength = 4
```

---

### pop()

**Syntax:** `array.pop()`

**Description:** Removes the last element from an array (mutates the array).

**Returns:** The removed element, or `null` if array is empty

**Example:**
```javascript
let numbers = [1, 2, 3, 4, 5]
const last = numbers.pop()
// last = 5
// numbers = [1, 2, 3, 4]
```

---

### slice()

**Syntax:** `array.slice(start, end)`

**Description:** Returns a shallow copy of a portion of an array into a new array.

**Parameters:**
- `start` - (Optional) Beginning index (default: 0). Negative indices count from the end
- `end` - (Optional) End index (not included) (default: array length). Negative indices count from the end

**Returns:** A new array containing the extracted elements

**Example:**
```javascript
const numbers = [1, 2, 3, 4, 5]

const sliced = numbers.slice(1, 4)
// sliced = [2, 3, 4]

const fromEnd = numbers.slice(-3, -1)
// fromEnd = [3, 4]

const allButFirst = numbers.slice(1)
// allButFirst = [2, 3, 4, 5]
```

---

### concat()

**Syntax:** `array.concat(otherArray)`

**Description:** Merges two arrays into a new array.

**Parameters:**
- `otherArray` - The array to concatenate

**Returns:** A new array containing elements from both arrays

**Example:**
```javascript
const arr1 = [1, 2, 3]
const arr2 = [4, 5, 6]
const combined = arr1.concat(arr2)
// combined = [1, 2, 3, 4, 5, 6]
```

---

### join()

**Syntax:** `array.join(separator)`

**Description:** Joins all elements of an array into a string.

**Parameters:**
- `separator` - (Optional) String to separate elements (default: ",")

**Returns:** A string with all array elements joined

**Example:**
```javascript
const numbers = [1, 2, 3, 4, 5]

const joined = numbers.join(", ")
// joined = "1, 2, 3, 4, 5"

const withArrows = numbers.join(" -> ")
// withArrows = "1 -> 2 -> 3 -> 4 -> 5"
```

---

### reverse()

**Syntax:** `array.reverse()`

**Description:** Reverses the elements of an array in place (mutates the array).

**Returns:** The reversed array (same reference as original)

**Example:**
```javascript
let numbers = [1, 2, 3, 4, 5]
numbers.reverse()
// numbers = [5, 4, 3, 2, 1]
```

---

### sort()

**Syntax:** `array.sort(comparator)`

**Description:** Sorts the elements of an array in place (mutates the array).

**Parameters:**
- `comparator(a, b)` - (Optional) Function to define sort order
  - Return negative number if `a < b`
  - Return 0 if `a == b`
  - Return positive number if `a > b`
- If no comparator provided, sorts numbers/strings in ascending order

**Returns:** The sorted array (same reference as original)

**Example:**
```javascript
let numbers = [3, 1, 4, 1, 5, 9, 2, 6]
numbers.sort()
// numbers = [1, 1, 2, 3, 4, 5, 6, 9]

let values = [5, 2, 8, 1, 9]
values.sort((a, b) => b - a)  // Sort descending
// values = [9, 8, 5, 2, 1]
```

---

## Method Chaining

All methods that return arrays can be chained together:

```javascript
const numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

const result = numbers
    .filter(x => x % 2 == 0)        // [2, 4, 6, 8, 10]
    .map(x => x * 2)                // [4, 8, 12, 16, 20]
    .filter(x => x > 10)            // [12, 16, 20]
    .reduce((sum, x) => sum + x, 0) // 48

// result = 48
```

---

## Performance Notes

- **Immutability**: Most methods (`map`, `filter`, `slice`, `concat`) return new arrays and don't modify the original
- **Mutating Methods**: `push`, `pop`, `reverse`, `sort` modify the original array in place
- **Short-circuiting**: `find`, `some`, and `every` stop iterating once the result is determined

---

## Examples

See `examples/array_methods.k` for comprehensive examples demonstrating all array methods.

See `tests/test_array_methods.k` for test cases covering all functionality.

---

## Implementation Notes

- All array methods support arrow functions as callbacks
- Methods properly handle empty arrays
- String comparisons work with `indexOf` and `includes`
- The `reduce` method can work with or without an initial value
- Negative indices in `slice` count from the end of the array

---

## Compatibility

These array methods are implemented following JavaScript ES6 specifications with minor adaptations for KLang's type system.
