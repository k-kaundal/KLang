# Memory Management Bug Analysis

## Summary
KLang has critical memory management bugs causing crashes when running OOP examples. Multiple issues have been identified and partially fixed.

## Bugs Fixed
1. **Double string duplication** in instance field initialization
2. **Double string duplication** in object literals  
3. **Double string duplication** in parent class field inheritance
4. **Missing cleanup** after env_set_local_with_access calls
5. **Incorrect VAL_METHOD cleanup** logic

## Remaining Issues

### Critical: Method Call Ref-Counting Bug
**Symptom**: Calling any method on an object twice causes a crash (double-free or segfault).

**Minimal Test Case**:
```klang
class Test {
    public fn get_value() -> int {
        return 5
    }
}

let obj = new Test()
println(obj.get_value())  // Works
println(obj.get_value())  // CRASHES
```

**Root Cause**: 
The reference counting for objects is unbalanced across method calls. The issue involves:
- NODE_IDENT increments ref_count when accessing variables
- make_method increments ref_count for the receiver
- env_set_local increments ref_count when binding 'this'
- value_free decrements at various points

The current implementation has an imbalance that causes either:
- Under-counting: Object freed too early (segfault)
- Over-counting: Object not freed, then double-freed later

**Technical Details**:
1. When `obj.method()` is evaluated:
   - obj is accessed (NODE_IDENT) - may increment ref_count
   - member access creates VAL_METHOD via make_method() - increments ref_count  
   - Method is called, receiver bound as 'this' - increments ref_count
   - After call, callee is freed - decrements ref_count
   - After member access, obj may be freed - decrements ref_count

2. The gc-agent attempted a fix by:
   - Calling value_free(&obj) at end of member access for objects
   - Skipping value_free for NODE_THIS (doesn't increment)
   
3. This partially works but still crashes on second call

**Recommended Next Steps**:
1. Use valgrind or address sanitizer to trace exact memory corruption
2. Add comprehensive ref_count logging at every increment/decrement
3. Consider simplifying the ref_count model - perhaps don't increment on NODE_IDENT
4. Review similar languages (Python, JavaScript engines) for ref-counting patterns
5. Add unit tests for method calls before fixing

## Files Modified
- `src/interpreter.c`: Multiple fixes for string duplication and ref-counting

## Test Cases
- examples/access_modifiers_demo.kl - FAILS (crashes on cleanup)
- examples/advanced_oop_complete.kl - FAILS (crashes immediately)
- /tmp/test_two_calls_simple.kl - FAILS (minimal repro case)
- /tmp/test_one_call.kl - PASSES (single method call works)
- examples/hello.kl - PASSES (basic functionality works)
