// Simplified test with one value per print

print("=== Logical AND (&&) Tests ===");
print(true && true);      // true
print(true && false);     // false
print(false && true);     // false
print(false && false);    // false

print("=== Logical OR (||) Tests ===");
print(true || true);      // true
print(true || false);     // true
print(false || true);     // true
print(false || false);    // false

print("=== Compound Assignment Tests ===");
let x = 10;
x += 5;
print(x);  // 15

let y = 20;
y -= 8;
print(y);  // 12

let z = 4;
z *= 3;
print(z);  // 12

let w = 50;
w /= 5;
print(w);  // 10

let v = 17;
v %= 5;
print(v);  // 2

print("=== Prefix Increment/Decrement ===");
let m = 5;
print(++m);  // 6
print(m);    // 6

let n = 10;
print(--n);  // 9
print(n);    // 9

print("=== Postfix Increment/Decrement ===");
let p = 5;
print(p++);  // 5
print(p);    // 6

let q = 10;
print(q--);  // 10
print(q);    // 9

print("=== Optional Chaining ===");
let obj = { name: "John" };
print(obj?.name);  // John

let null_obj = null;
print(null_obj?.name);  // null

print("=== Nullish Coalescing ===");
let val1 = null;
print(val1 ?? "default");  // default

let val2 = "value";
print(val2 ?? "default");  // value

print("=== Complex Test ===");
let a = 5;
a += 3;
a *= 2;
print(a);  // 16

let age = 25;
let hasLicense = true;
if (age >= 18 && hasLicense) {
    print("Can drive");
}

print("All tests passed!");
