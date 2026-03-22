// test_utils.k - Test the utils module

import {isEven, isOdd, max, min} from "./utils.k"

println("Testing utils module:")

println("isEven(4) = " + isEven(4))    // true
println("isEven(7) = " + isEven(7))    // false
println("isOdd(4) = " + isOdd(4))      // false
println("isOdd(7) = " + isOdd(7))      // true

println("max(10, 20) = " + max(10, 20))
println("min(10, 20) = " + min(10, 20))
