// test_edge_cases.k - Test edge cases
import {noParams, oneParam, threeParams, returnsFunction} from "./edge_cases.k"

println("Testing edge cases:")

println("noParams(): " + noParams())
println("oneParam('hello'): " + oneParam("hello"))
println("threeParams(1,2,3): " + threeParams(1, 2, 3))

let doubler = returnsFunction()
println("returnsFunction()(5): " + doubler(5))

println("All edge cases passed!")
