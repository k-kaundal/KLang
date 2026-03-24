# Array Methods Reference

## Introduction

KLang arrays come with powerful built-in methods for transformation, filtering, and manipulation. These methods enable functional programming patterns and make array operations concise and expressive.

## Transformation Methods

### `map(callback)`

Transform each element.

```klang
let numbers = [1, 2, 3, 4, 5]
let doubled = numbers.map(x => x * 2)
println(doubled)  # [2, 4, 6, 8, 10]

let names = ["alice", "bob"]
let uppercase = names.map(name => name.toUpperCase())
```

### `filter(callback)`

Keep elements that match condition.

```klang
let numbers = [1, 2, 3, 4, 5, 6]
let evens = numbers.filter(x => x % 2 == 0)
println(evens)  # [2, 4, 6]

let users = [
    {"name": "Alice", "age": 30},
    {"name": "Bob", "age": 17},
    {"name": "Charlie", "age": 25}
]
let adults = users.filter(u => u["age"] >= 18)
```

### `reduce(callback, initial)`

Reduce array to single value.

```klang
let numbers = [1, 2, 3, 4, 5]
let sum = numbers.reduce((acc, x) => acc + x, 0)
println(sum)  # 15

let product = numbers.reduce((acc, x) => acc * x, 1)
println(product)  # 120
```

## Iteration Methods

### `forEach(callback)`

Execute function for each element.

```klang
let fruits = ["apple", "banana", "orange"]
fruits.forEach(fruit => println(fruit))

# With index
fruits.forEach((fruit, i) => {
    println(str(i) + ": " + fruit)
})
```

## Search Methods

### `find(callback)`

Find first matching element.

```klang
let numbers = [1, 2, 3, 4, 5]
let found = numbers.find(x => x > 3)
println(found)  # 4

let users = [
    {"id": 1, "name": "Alice"},
    {"id": 2, "name": "Bob"}
]
let user = users.find(u => u["id"] == 2)
```

### `findIndex(callback)`

Find index of first match.

```klang
let numbers = [10, 20, 30, 40]
let index = numbers.findIndex(x => x >= 30)
println(index)  # 2
```

### `includes(value)`

Check if array contains value.

```klang
let fruits = ["apple", "banana", "orange"]
println(fruits.includes("banana"))  # true
println(fruits.includes("grape"))   # false
```

### `indexOf(value)`

Find index of value.

```klang
let numbers = [10, 20, 30, 20, 40]
println(numbers.indexOf(20))  # 1
println(numbers.indexOf(99))  # -1
```

## Modification Methods

### `push(value)`

Add element to end.

```klang
let arr = [1, 2, 3]
arr.push(4)
println(arr)  # [1, 2, 3, 4]
```

### `pop()`

Remove and return last element.

```klang
let arr = [1, 2, 3]
let last = arr.pop()
println(last)  # 3
println(arr)   # [1, 2]
```

### `shift()`

Remove and return first element.

```klang
let arr = [1, 2, 3]
let first = arr.shift()
println(first)  # 1
println(arr)    # [2, 3]
```

### `unshift(value)`

Add element to beginning.

```klang
let arr = [2, 3]
arr.unshift(1)
println(arr)  # [1, 2, 3]
```

### `splice(start, deleteCount, ...items)`

Remove/insert elements.

```klang
let arr = [1, 2, 3, 4, 5]
arr.splice(2, 2)  # Remove 2 elements at index 2
println(arr)  # [1, 2, 5]

arr.splice(1, 0, 10, 20)  # Insert at index 1
println(arr)  # [1, 10, 20, 2, 5]
```

## Ordering Methods

### `sort(compareFn?)`

Sort array.

```klang
let numbers = [3, 1, 4, 1, 5, 9]
numbers.sort()
println(numbers)  # [1, 1, 3, 4, 5, 9]

# Custom sort
let words = ["banana", "apple", "cherry"]
words.sort((a, b) => a > b ? 1 : -1)
```

### `reverse()`

Reverse array order.

```klang
let arr = [1, 2, 3, 4, 5]
arr.reverse()
println(arr)  # [5, 4, 3, 2, 1]
```

## Combining Methods

### `concat(array)`

Combine arrays.

```klang
let arr1 = [1, 2, 3]
let arr2 = [4, 5, 6]
let combined = arr1.concat(arr2)
println(combined)  # [1, 2, 3, 4, 5, 6]
```

### `slice(start, end?)`

Extract portion of array.

```klang
let arr = [1, 2, 3, 4, 5]
let portion = arr.slice(1, 4)
println(portion)  # [2, 3, 4]

let last_two = arr.slice(-2)
println(last_two)  # [4, 5]
```

### `join(separator)`

Join elements into string.

```klang
let words = ["Hello", "World"]
println(words.join(" "))   # "Hello World"
println(words.join(", "))  # "Hello, World"
```

## Testing Methods

### `every(callback)`

Test if all elements match.

```klang
let numbers = [2, 4, 6, 8]
let all_even = numbers.every(x => x % 2 == 0)
println(all_even)  # true
```

### `some(callback)`

Test if any element matches.

```klang
let numbers = [1, 3, 5, 6, 7]
let has_even = numbers.some(x => x % 2 == 0)
println(has_even)  # true
```

## Method Chaining

Combine methods for powerful transformations:

```klang
let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

let result = numbers
    .filter(x => x % 2 == 0)  # Get evens
    .map(x => x * x)          # Square them
    .filter(x => x > 10)      # Keep > 10
    .reduce((sum, x) => sum + x, 0)  # Sum

println(result)  # 4 + 16 + 36 + 64 + 100 = 220
```

## Complete Reference

| Method | Parameters | Returns | Description |
|--------|-----------|---------|-------------|
| `map` | callback | array | Transform elements |
| `filter` | callback | array | Filter elements |
| `reduce` | callback, initial | any | Reduce to value |
| `forEach` | callback | null | Iterate |
| `find` | callback | element/null | Find first |
| `findIndex` | callback | int | Find index |
| `includes` | value | bool | Contains value |
| `indexOf` | value | int | Index of value |
| `push` | value | null | Add to end |
| `pop` | - | element | Remove from end |
| `shift` | - | element | Remove from start |
| `unshift` | value | null | Add to start |
| `splice` | start, count, ...items | array | Remove/insert |
| `sort` | compareFn? | array | Sort array |
| `reverse` | - | array | Reverse order |
| `concat` | array | array | Combine arrays |
| `slice` | start, end? | array | Extract portion |
| `join` | separator | string | Join to string |
| `every` | callback | bool | All match |
| `some` | callback | bool | Any match |

## Next Steps

- [Objects](15-objects.md) - Working with objects
- [Functions](13-functions.md) - Higher-order functions
- [Examples](60-examples-basic.md) - Array examples

---

**Master array methods for elegant data manipulation!** 🎯
