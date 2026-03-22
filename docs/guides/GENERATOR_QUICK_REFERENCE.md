# Generator Functions - Quick Reference

## Syntax

### Defining a Generator
```javascript
fn* generatorName(params) {
    // function body
    yield value
}
```

### Creating a Generator Object
```javascript
let gen = generatorName(args)
```

### Calling next()
```javascript
let result = gen.next()  // Returns {value: ..., done: ...}
```

## Complete Example

```javascript
// Define generator
fn* countdown() {
    yield 3
    yield 2
    yield 1
}

// Create generator object
let gen = countdown()

// Get values
let r1 = gen.next()
println(r1.value)  // 3
println(r1.done)   // true (current implementation limitation)

let r2 = gen.next()
println(r2.value)  // null (exhausted)
println(r2.done)   // true
```

## Iterator Result Object

Every call to `next()` returns an object with two properties:

- `value`: The yielded value (or `null` if generator is exhausted)
- `done`: `true` if generator is finished, `false` otherwise

## Generator States

1. **SUSPENDED**: Initial state after creation, waiting to be called
2. **RUNNING**: Currently executing
3. **COMPLETED**: No more values to yield

## Current Limitations

1. **Single Yield**: Only the first `yield` works; subsequent calls return the same value
2. **No State Persistence**: Generator re-executes from the beginning each time
3. **No `.return()` or `.throw()`**: Only `.next()` is implemented

## Type Checking

```javascript
fn* gen() { yield 1 }
let g = gen()
println(type(g))  // "generator"
```

## Use Cases

### Simple Counter
```javascript
fn* counter() {
    yield 1
}

let gen = counter()
let result = gen.next()
println(result.value)  // 1
```

### Parameterized Generator
```javascript
fn* echo(message) {
    yield message
}

let gen = echo("Hello")
let result = gen.next()
println(result.value)  // "Hello"
```

### Check if Exhausted
```javascript
fn* once() {
    yield 42
}

let gen = once()
gen.next()  // Get the value

let r = gen.next()  // Call again
if (r.done) {
    println("Generator is exhausted")
}
```

## Comparison with JavaScript

### KLang
```javascript
fn* gen() {
    yield 1
}
let g = gen()
let r = g.next()
println(r.value)
println(r.done)
```

### JavaScript
```javascript
function* gen() {
    yield 1;
}
let g = gen();
let r = g.next();
console.log(r.value);  // 1
console.log(r.done);    // false
```

**Note**: KLang's current implementation marks generators as `done: true` after the first yield, unlike JavaScript which maintains full state between yields.

## Tips

1. **Access fields separately**: Due to println limitations, access `value` and `done` in separate statements
2. **Test for completion**: Always check `.done` before using `.value`
3. **One-time use**: In current implementation, effectively a one-shot operation

## Future Enhancements

- Multiple yield support with state persistence
- `yield*` for generator delegation
- `.return()` and `.throw()` methods
- Integration with `for-of` loops
- Async generators (`async fn*`)
