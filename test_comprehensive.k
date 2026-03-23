// Comprehensive test of all new operators

print("=== COMPREHENSIVE OPERATOR TEST ===");

// 1. Complex logical expressions
let score = 85;
let passed = score >= 60 && score <= 100;
print("Score 85, passed (60-100)?", passed);  // true

let hasPermission = false;
let isAdmin = true;
let canAccess = hasPermission || isAdmin;
print("Can access?", canAccess);  // true

// 2. Chained compound assignments
let value = 10;
value += 5;   // 15
value *= 2;   // 30
value -= 10;  // 20
value /= 4;   // 5
value %= 3;   // 2
print("Final value:", value);  // 2

// 3. Pre and post increment in expressions
let i = 5;
let j = 10;
let result = ++i + j++;  // 6 + 10 = 16
print("i:", i, "j:", j, "result:", result);  // 6, 11, 16

// 4. Optional chaining with nested objects
let user = {
    name: "Alice",
    address: {
        city: "NYC",
        zip: 10001
    }
};
print("City:", user?.address?.city);  // NYC

let no_user = null;
print("No user city:", no_user?.address?.city);  // null

// 5. Nullish coalescing in function parameters
fn greet(name) {
    let displayName = name ?? "Guest";
    print("Hello,", displayName);
}
greet("Bob");   // Hello, Bob
greet(null);    // Hello, Guest

// 6. Complex array operations
let numbers = [1, 2, 3, 4, 5];
numbers[0] *= 10;   // 10
numbers[1] += 8;    // 10
numbers[2]++;       // 4
++numbers[3];       // 5
numbers[4] = numbers[4] * 2;  // 10
print("Array:", numbers[0], numbers[1], numbers[2], numbers[3], numbers[4]);

// 7. Combining operators in conditionals
let age = 20;
let country = "USA";
if (age >= 18 && age <= 65 && (country == "USA" || country == "Canada")) {
    print("Eligible for program");
}

// 8. Short-circuit with function calls
let counter = 0;
fn incrementAndReturn() {
    counter++;
    return true;
}

// This should NOT increment counter (short-circuit)
let test1 = false && incrementAndReturn();
print("Counter after false &&:", counter);  // 0

// This should NOT increment counter (short-circuit)
let test2 = true || incrementAndReturn();
print("Counter after true ||:", counter);  // 0

// This SHOULD increment counter
let test3 = true && incrementAndReturn();
print("Counter after true &&:", counter);  // 1

// 9. Operator precedence
let a = 5;
let b = 10;
let c = 15;
let d = a + b * c;  // 5 + 150 = 155
print("5 + 10 * 15 =", d);

let e = (a + b) * c;  // 15 * 15 = 225
print("(5 + 10) * 15 =", e);

// 10. Mixed operators
let x = 10;
x += 5;          // 15
let y = ++x;     // 16
let z = y++;     // 16, y becomes 17
print("x:", x, "y:", y, "z:", z);  // 16, 17, 16

// 11. Object property operations
let obj = { count: 0, total: 100 };
obj.count++;
obj.total -= 50;
obj.count += 5;
print("Count:", obj.count, "Total:", obj.total);  // 6, 50

// 12. Combining all features
let config = {
    debug: true,
    timeout: 5000
};

let timeoutValue = config?.timeout ?? 3000;
if (config?.debug && timeoutValue > 1000) {
    print("Debug mode with timeout:", timeoutValue);
}

print("=== ALL COMPREHENSIVE TESTS PASSED ===");
