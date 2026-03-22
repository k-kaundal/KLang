// test_func_debug.k
import {greet} from "./func_module.k"
println("Loaded greet function")
let result = greet("World")
println("Called greet, result type: " + typeof(result))
println("Result: " + result)
