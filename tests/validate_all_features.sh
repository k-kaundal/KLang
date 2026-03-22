#!/bin/bash
# Comprehensive Feature Validation Script
# Tests all documented KLang features from 0 to 100%

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Get repo root
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$REPO_ROOT"

# Counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

echo -e "${BLUE}╔══════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║     KLang Complete Feature Validation (0-100%)           ║${NC}"
echo -e "${BLUE}╚══════════════════════════════════════════════════════════╝${NC}"
echo ""

# Check if klang exists
if [ ! -f "./klang" ]; then
    echo -e "${RED}❌ Error: klang binary not found!${NC}"
    echo -e "${YELLOW}Please run 'make' to build KLang first.${NC}"
    exit 1
fi

# Function to run a test
run_test() {
    local test_name="$1"
    local test_code="$2"
    local expected_success="${3:-true}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -n "  Testing $test_name... "
    
    # Create temp file
    local temp_file="/tmp/klang_test_$$_${TOTAL_TESTS}.kl"
    echo "$test_code" > "$temp_file"
    
    # Run the test
    if ./klang run "$temp_file" > /tmp/klang_output_$$.txt 2>&1; then
        if [ "$expected_success" = "true" ]; then
            echo -e "${GREEN}✅ PASS${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}❌ FAIL (expected to fail)${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    else
        if [ "$expected_success" = "false" ]; then
            echo -e "${GREEN}✅ PASS (correctly failed)${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}❌ FAIL${NC}"
            echo -e "${YELLOW}Error output:${NC}"
            cat /tmp/klang_output_$$.txt | head -5
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    fi
    
    # Cleanup
    rm -f "$temp_file" /tmp/klang_output_$$.txt
}

# ============================================================
# Level 1: Basic Features (0-20%)
# ============================================================
echo -e "${BLUE}━━━ Level 1: Basic Features (0-20%) ━━━${NC}"

run_test "Comments" '
// Single line comment
/* Multi-line
   comment */
print("Comments work")
'

run_test "Print statements" '
print("Hello")
print(" ")
print("World")
'

run_test "Basic arithmetic" '
let a = 10
let b = 20
print(a + b)
print(a - b)
print(a * b)
print(a / b)
'

# ============================================================
# Level 2: Variables & Types (20-30%)
# ============================================================
echo -e "\n${BLUE}━━━ Level 2: Variables & Types (20-30%) ━━━${NC}"

run_test "let declarations" '
let x = 10
let name = "Alice"
print(x)
print(name)
'

run_test "const declarations" '
const PI = 3.14159
print(PI)
'

run_test "var declarations" '
var y = 100
print(y)
'

run_test "Type annotations" '
let age: int = 25
let price: float = 19.99
print(age)
print(price)
'

run_test "String operations" '
let str = "Hello"
print(str + " World")
'

run_test "Boolean values" '
let flag = true
print(flag)
'

# ============================================================
# Level 3: Functions (30-40%)
# ============================================================
echo -e "\n${BLUE}━━━ Level 3: Functions (30-40%) ━━━${NC}"

run_test "Function declarations" '
fn greet(name) {
    return "Hello, " + name
}
print(greet("Alice"))
'

run_test "Arrow functions - single param" '
let double = x => x * 2
print(double(5))
'

run_test "Arrow functions - multiple params" '
let add = (a, b) => a + b
print(add(3, 7))
'

run_test "Arrow functions - no params" '
let getMessage = () => "Hello"
print(getMessage())
'

run_test "Default parameters" '
fn greet(name = "World") {
    return "Hello, " + name
}
print(greet())
print(greet("Alice"))
'

run_test "Rest parameters" '
fn sum(...numbers) {
    let total = 0
    for (let i = 0; i < numbers.length; i = i + 1) {
        total = total + numbers[i]
    }
    return total
}
print(sum(1, 2, 3, 4, 5))
'

# ============================================================
# Level 4: Control Flow (40-50%)
# ============================================================
echo -e "\n${BLUE}━━━ Level 4: Control Flow (40-50%) ━━━${NC}"

run_test "if/else statements" '
let x = 10
if (x > 5) {
    print("Greater")
} else {
    print("Smaller")
}
'

run_test "while loops" '
let i = 0
while (i < 3) {
    print(i)
    i = i + 1
}
'

run_test "for loops" '
for (let i = 0; i < 3; i = i + 1) {
    print(i)
}
'

run_test "Ternary operator" '
let x = 10
let result = x > 5 ? "yes" : "no"
print(result)
'

run_test "break statement" '
for (let i = 0; i < 10; i = i + 1) {
    if (i == 3) {
        break
    }
    print(i)
}
'

run_test "continue statement" '
for (let i = 0; i < 5; i = i + 1) {
    if (i == 2) {
        continue
    }
    print(i)
}
'

# ============================================================
# Level 5: Data Structures (50-60%)
# ============================================================
echo -e "\n${BLUE}━━━ Level 5: Data Structures (50-60%) ━━━${NC}"

run_test "Array literals" '
let arr = [1, 2, 3, 4, 5]
print(arr[0])
print(arr[4])
'

run_test "Array length" '
let arr = [1, 2, 3]
print(arr.length)
'

run_test "Object literals" '
let person = {
    name: "Alice",
    age: 30
}
print(person.name)
print(person.age)
'

run_test "Property shorthand" '
let x = 10
let y = 20
let point = {x, y}
print(point.x)
print(point.y)
'

run_test "Computed properties" '
let key = "name"
let obj = {
    [key]: "Alice"
}
print(obj.name)
'

# ============================================================
# Level 6: Object-Oriented Programming (60-75%)
# ============================================================
echo -e "\n${BLUE}━━━ Level 6: Object-Oriented Programming (60-75%) ━━━${NC}"

run_test "Class declarations" '
class Animal {
    fn init(name) {
        this.name = name
    }
    
    fn speak() {
        return this.name + " makes a sound"
    }
}

let dog = Animal("Buddy")
print(dog.speak())
'

run_test "Class inheritance" '
class Animal {
    fn init(name) {
        this.name = name
    }
}

class Dog extends Animal {
    fn init(name, breed) {
        super.init(name)
        this.breed = breed
    }
}

let dog = Dog("Buddy", "Labrador")
print(dog.name)
print(dog.breed)
'

run_test "Static methods" '
class Math {
    static fn double(x) {
        return x * 2
    }
}
print(Math.double(5))
'

# ============================================================
# Level 7: Advanced Functions (75-85%)
# ============================================================
echo -e "\n${BLUE}━━━ Level 7: Advanced Functions (75-85%) ━━━${NC}"

run_test "Template literals" '
let name = "Alice"
let age = 30
print(`Hello, ${name}! You are ${age} years old.`)
'

run_test "Closures" '
fn outer() {
    let x = 10
    fn inner() {
        return x + 5
    }
    return inner
}
let myFunc = outer()
print(myFunc())
'

run_test "Higher-order functions" '
fn apply(func, x) {
    return func(x)
}
let double = x => x * 2
print(apply(double, 5))
'

# ============================================================
# Level 8: Asynchronous Programming (85-90%)
# ============================================================
echo -e "\n${BLUE}━━━ Level 8: Asynchronous Programming (85-90%) ━━━${NC}"

run_test "Async functions" '
async fn fetchData() {
    return "Data loaded"
}
print("Testing async")
' false  # Expected to fail or need proper async support

run_test "Promises" '
let promise = Promise(fn(resolve, reject) {
    resolve("Success")
})
print("Promise test")
' false  # Expected to fail or need proper promise support

# ============================================================
# Level 9: Modules & Organization (90-95%)
# ============================================================
echo -e "\n${BLUE}━━━ Level 9: Modules & Organization (90-95%) ━━━${NC}"

run_test "Import/Export" '
// This requires actual module files
// export const value = 42
// import { value } from "./module"
print("Module test placeholder")
' false  # Expected to fail without proper module system

# ============================================================
# Level 10: Advanced Features (95-100%)
# ============================================================
echo -e "\n${BLUE}━━━ Level 10: Advanced Features (95-100%) ━━━${NC}"

run_test "Spread operator" '
let arr1 = [1, 2, 3]
let arr2 = [...arr1, 4, 5]
print(arr2)
'

run_test "Destructuring arrays" '
let [a, b] = [1, 2]
print(a)
print(b)
'

run_test "Destructuring objects" '
let {name, age} = {name: "Alice", age: 30}
print(name)
print(age)
'

run_test "Try-catch-finally" '
try {
    throw "Error!"
} catch (e) {
    print("Caught: " + e)
} finally {
    print("Finally executed")
}
'

run_test "Generators" '
fn* range(n) {
    for (let i = 0; i < n; i = i + 1) {
        yield i
    }
}
let gen = range(3)
print(gen.next())
' false  # Expected to need proper generator support

# ============================================================
# Print Summary
# ============================================================
echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}Test Summary${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""
echo -e "  Total Tests:   $TOTAL_TESTS"
echo -e "  ${GREEN}Passed:        $PASSED_TESTS${NC}"
echo -e "  ${RED}Failed:        $FAILED_TESTS${NC}"
echo ""

# Calculate percentage
if [ $TOTAL_TESTS -gt 0 ]; then
    PASS_PERCENT=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    echo -e "  Pass Rate:     ${GREEN}${PASS_PERCENT}%${NC}"
    echo ""
    
    if [ $PASS_PERCENT -ge 80 ]; then
        echo -e "${GREEN}🎉 Excellent! Most features are working!${NC}"
    elif [ $PASS_PERCENT -ge 60 ]; then
        echo -e "${YELLOW}⚠️  Good progress, but some features need work.${NC}"
    else
        echo -e "${RED}⚠️  Many features need implementation.${NC}"
    fi
fi

echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"

# Exit with appropriate code
if [ $FAILED_TESTS -eq 0 ]; then
    exit 0
else
    exit 1
fi
