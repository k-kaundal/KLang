// Test nullish coalescing edge cases

let zero = 0;
print(zero ?? "default");  // Should print 0

let empty = "";
print(empty ?? "default");  // Should print ""

let false_val = false;
print(false_val ?? "default");  // Should print false

let null_val = null;
print(null_val ?? "default");  // Should print "default"
