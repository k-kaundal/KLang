// Test file for all newly implemented operators

// ===== Test 1: Logical AND (&&) with short-circuit =====
print("=== Test 1: Logical AND (&&) ===");
let a = true && true;
print("true && true:", a);  // Should print true

let b = true && false;
print("true && false:", b);  // Should print false

let c = false && true;
print("false && true:", c);  // Should print false

let d = false && false;
print("false && false:", d);  // Should print false

// Test short-circuit: second expression should not be evaluated
let called = false;
fn side_effect() {
    called = true;
    return true;
}
let e = false && side_effect();
print("Short-circuit test (should be false, called should be false):", e, called);

// ===== Test 2: Logical OR (||) with short-circuit =====
print("\n=== Test 2: Logical OR (||) ===");
let f = true || true;
print("true || true:", f);  // Should print true

let g = true || false;
print("true || false:", g);  // Should print true

let h = false || true;
print("false || true:", h);  // Should print true

let i = false || false;
print("false || false:", i);  // Should print false

// Test short-circuit: second expression should not be evaluated
called = false;
let j = true || side_effect();
print("Short-circuit test (should be true, called should be false):", j, called);

// ===== Test 3: Compound Assignments =====
print("\n=== Test 3: Compound Assignments ===");

// += test
let x = 10;
x += 5;
print("10 += 5:", x);  // Should print 15

// -= test
let y = 20;
y -= 8;
print("20 -= 8:", y);  // Should print 12

// *= test
let z = 4;
z *= 3;
print("4 *= 3:", z);  // Should print 12

// /= test
let w = 50;
w /= 5;
print("50 /= 5:", w);  // Should print 10

// %= test
let v = 17;
v %= 5;
print("17 %= 5:", v);  // Should print 2

// Test with floats
let p = 10.5;
p += 2.3;
print("10.5 += 2.3:", p);  // Should print 12.8

// ===== Test 4: Prefix Increment/Decrement =====
print("\n=== Test 4: Prefix Increment/Decrement ===");

let m = 5;
let n = ++m;
print("m = 5, n = ++m, m:", m, "n:", n);  // m: 6, n: 6

let o = 10;
let p2 = --o;
print("o = 10, p2 = --o, o:", o, "p2:", p2);  // o: 9, p2: 9

// ===== Test 5: Postfix Increment/Decrement =====
print("\n=== Test 5: Postfix Increment/Decrement ===");

let q = 5;
let r = q++;
print("q = 5, r = q++, q:", q, "r:", r);  // q: 6, r: 5

let s = 10;
let t = s--;
print("s = 10, t = s--, s:", s, "t:", t);  // s: 9, t: 10

// ===== Test 6: Optional Chaining (?.) =====
print("\n=== Test 6: Optional Chaining (?.) ===");

let obj = { name: "John", age: 30 };
print("obj.name:", obj.name);  // Should print "John"
print("obj?.name:", obj?.name);  // Should print "John"

let null_obj = null;
print("null_obj?.name:", null_obj?.name);  // Should print null

// ===== Test 7: Nullish Coalescing (??) =====
print("\n=== Test 7: Nullish Coalescing (??) ===");

let val1 = null;
let result1 = val1 ?? "default";
print("null ?? 'default':", result1);  // Should print "default"

let val2 = "value";
let result2 = val2 ?? "default";
print("'value' ?? 'default':", result2);  // Should print "value"

let val3 = 0;
let result3 = val3 ?? "default";
print("0 ?? 'default':", result3);  // Should print 0

// ===== Test 8: Complex Expressions =====
print("\n=== Test 8: Complex Expressions ===");

// Combining multiple operators
let u = 5;
u += 3;
u *= 2;
print("u = 5; u += 3; u *= 2; Result:", u);  // Should print 16

// Logical operators in conditions
let age = 25;
let hasLicense = true;
if (age >= 18 && hasLicense) {
    print("Can drive!");  // Should print
}

// Mixing increment with other operations
let counter = 0;
let total = ++counter + ++counter + ++counter;
print("counter:", counter, "total:", total);  // counter: 3, total: 6

// ===== Test 9: Compound assignments with member access =====
print("\n=== Test 9: Compound assignments with objects ===");

let person = { score: 100 };
person.score += 50;
print("person.score after += 50:", person.score);  // Should print 150

person.score *= 2;
print("person.score after *= 2:", person.score);  // Should print 300

// ===== Test 10: Increment/Decrement with member access =====
print("\n=== Test 10: Increment/Decrement with objects ===");

let game = { lives: 3 };
game.lives++;
print("game.lives after ++:", game.lives);  // Should print 4

game.lives--;
print("game.lives after --:", game.lives);  // Should print 3

print("\n=== All tests completed! ===");
