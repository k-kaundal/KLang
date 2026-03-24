# KLang Operators Quick Reference

## Logical Operators

| Operator | Description | Example | Short-circuit? |
|----------|-------------|---------|----------------|
| `&&` | Logical AND | `a && b` | ✅ Yes |
| `||` | Logical OR | `a || b` | ✅ Yes |
| `!` | Logical NOT | `!a` | N/A |

**Short-circuit behavior:**
- `&&`: Returns false immediately if left is false
- `||`: Returns true immediately if left is true

## Arithmetic Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `+` | Addition | `a + b` |
| `-` | Subtraction | `a - b` |
| `*` | Multiplication | `a * b` |
| `/` | Division | `a / b` |
| `%` | Modulo | `a % b` |

## Comparison Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `==` | Equal to | `a == b` |
| `!=` | Not equal to | `a != b` |
| `<` | Less than | `a < b` |
| `>` | Greater than | `a > b` |
| `<=` | Less than or equal | `a <= b` |
| `>=` | Greater than or equal | `a >= b` |

## Assignment Operators

| Operator | Equivalent | Example |
|----------|------------|---------|
| `=` | N/A | `a = 5` |
| `+=` | `a = a + b` | `a += 5` |
| `-=` | `a = a - b` | `a -= 5` |
| `*=` | `a = a * b` | `a *= 5` |
| `/=` | `a = a / b` | `a /= 5` |
| `%=` | `a = a % b` | `a %= 5` |

## Increment/Decrement Operators

| Operator | Type | Behavior | Example |
|----------|------|----------|---------|
| `++x` | Prefix | Increment, then return | `let y = ++x` |
| `x++` | Postfix | Return, then increment | `let y = x++` |
| `--x` | Prefix | Decrement, then return | `let y = --x` |
| `x--` | Postfix | Return, then decrement | `let y = x--` |

## Special Operators

| Operator | Description | Example | Notes |
|----------|-------------|---------|-------|
| `?.` | Optional chaining | `obj?.prop` | Returns null if obj is null |
| `??` | Nullish coalescing | `a ?? b` | Returns b only if a is null |
| `? :` | Ternary | `a ? b : c` | Conditional expression |

## Operator Precedence (High to Low)

1. **Postfix**: `++`, `--`, `()`, `[]`, `.`, `?.`
2. **Prefix**: `++`, `--`, `-`, `!`
3. **Multiplicative**: `*`, `/`, `%`
4. **Additive**: `+`, `-`
5. **Comparison**: `<`, `>`, `<=`, `>=`
6. **Equality**: `==`, `!=`
7. **Logical AND**: `&&`
8. **Logical OR**: `||`
9. **Nullish Coalescing**: `??`
10. **Ternary**: `? :`
11. **Assignment**: `=`, `+=`, `-=`, `*=`, `/=`, `%=`

## Common Patterns

### Safe Property Access
```javascript
let value = obj?.prop ?? "default";
```

### Loop Counters
```javascript
for (let i = 0; i < 10; i++) {
    // Use i++
}

for (let i = 10; i > 0; --i) {
    // Use --i for efficiency in prefix
}
```

### Conditional Assignment
```javascript
let result = condition ? valueIfTrue : valueIfFalse;
```

### Short-circuit Evaluation
```javascript
// Only call function if isReady is true
isReady && doSomething();

// Call function only if cache is null
let value = cache || fetchFromAPI();
```

### Default Values
```javascript
// Old way with ||
let timeout = config.timeout || 3000;  // Problem: 0 would become 3000

// New way with ??
let timeout = config.timeout ?? 3000;  // Correct: 0 stays 0
```

### Accumulator Pattern
```javascript
let sum = 0;
for (let i = 0; i < arr.length; i++) {
    sum += arr[i];
}
```

### Guard Clauses
```javascript
if (!user) return;
if (!user.isActive) return;
// Proceed with active user...

// Or with optional chaining:
if (!user?.isActive) return;
```

## Truthiness

### Falsy Values
- `false`
- `0`
- `null`
- `""` (empty string)

### Truthy Values
- Everything else
- `true`
- Non-zero numbers
- Non-empty strings
- Objects
- Arrays

## Important Notes

1. **`??` vs `||`**:
   - `??` only replaces `null`
   - `||` replaces all falsy values (including 0, false, "")

2. **`?.` vs `.`**:
   - Use `?.` when property might not exist
   - Use `.` for required properties (fails fast if missing)

3. **Prefix vs Postfix**:
   - Prefix: More efficient in standalone statements
   - Postfix: Use when you need the old value

4. **Short-circuit**:
   - Can prevent errors: `obj && obj.method()`
   - Can improve performance: avoid expensive calls

5. **Compound assignments**:
   - More concise: `x += 5` vs `x = x + 5`
   - Reads better for intent
   - Works with arrays and objects

## Examples

```javascript
// Complex condition with short-circuit
if (user && user.isActive && user.age >= 18) {
    // Safe: won't error if user is null
}

// Default value with nullish coalescing
let timeout = config?.timeout ?? 3000;

// Efficient counter
let count = 0;
while (condition) {
    ++count;  // Prefix more efficient
}

// Post-increment in expression
let arr = [1, 2, 3];
let i = 0;
print(arr[i++]);  // Prints 1, i becomes 1

// Multiple compound assignments
let score = 100;
score += 10;  // 110
score *= 2;   // 220
score -= 20;  // 200
score /= 4;   // 50

// Chained optional access
let city = user?.address?.city ?? "Unknown";

// Logical operators in conditions
let canProceed = hasPermission || isAdmin;
let isValid = isLoggedIn && hasVerifiedEmail;
```
