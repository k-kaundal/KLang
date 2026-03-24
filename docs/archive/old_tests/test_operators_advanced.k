// Test short-circuit evaluation for && and ||

print("=== Short-circuit AND Tests ===");

// Test 1: && should not evaluate right side if left is false
let counter1 = 0;
fn increment1() {
    counter1 = counter1 + 1;
    return true;
}

let result1 = false && increment1();
print(result1);      // false
print(counter1);     // 0 (function should not have been called)

// Test 2: && should evaluate right side if left is true
let counter2 = 0;
fn increment2() {
    counter2 = counter2 + 1;
    return false;
}

let result2 = true && increment2();
print(result2);      // false
print(counter2);     // 1 (function should have been called)

print("=== Short-circuit OR Tests ===");

// Test 3: || should not evaluate right side if left is true
let counter3 = 0;
fn increment3() {
    counter3 = counter3 + 1;
    return false;
}

let result3 = true || increment3();
print(result3);      // true
print(counter3);     // 0 (function should not have been called)

// Test 4: || should evaluate right side if left is false
let counter4 = 0;
fn increment4() {
    counter4 = counter4 + 1;
    return true;
}

let result4 = false || increment4();
print(result4);      // true
print(counter4);     // 1 (function should have been called)

print("=== Compound Assignment with Arrays ===");

let arr = [10, 20, 30];
arr[0] += 5;
print(arr[0]);  // 15

arr[1] *= 2;
print(arr[1]);  // 40

arr[2] -= 10;
print(arr[2]);  // 20

print("=== Increment/Decrement with Arrays ===");

let nums = [5, 10, 15];
nums[0]++;
print(nums[0]);  // 6

nums[1]--;
print(nums[1]);  // 9

print(++nums[2]);  // 16
print(nums[2]);    // 16

print("=== Nested Operators ===");

let val = 5;
val += 3;  // val = 8
val *= 2;  // val = 16
val -= 6;  // val = 10
val /= 2;  // val = 5
val %= 3;  // val = 2
print(val);  // 2

print("=== Logical Operators in Conditions ===");

let a = 10;
let b = 20;
let c = 30;

if (a < b && b < c) {
    print("a < b < c");
}

if (a > b || b < c) {
    print("a > b OR b < c");
}

if ((a < b && b < c) || a > 100) {
    print("Complex condition true");
}

print("=== Nullish Coalescing with Various Types ===");

let null_val = null;
print(null_val ?? 42);  // 42

let zero_val = 0;
print(zero_val ?? 42);  // 0 (0 is not null)

let empty_str = "";
print(empty_str ?? "default");  // "" (empty string is not null)

let false_val = false;
print(false_val ?? true);  // false (false is not null)

print("=== Chaining Optional Access ===");

let data = { user: { name: "Alice" } };
print(data?.user?.name);  // Alice

let data2 = null;
print(data2?.user?.name);  // null

let data3 = { user: null };
print(data3?.user?.name);  // null

print("All advanced tests passed!");
