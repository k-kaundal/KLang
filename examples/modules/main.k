// main.k - Main file that imports and uses the math module

import {PI, E, add, multiply} from "./math.k"
import square from "./math.k"
import * as Math from "./math.k"

// Test named imports
println("Named imports:")
println("PI = " + PI)
println("E = " + E)
println("add(5, 3) = " + add(5, 3))
println("multiply(4, 7) = " + multiply(4, 7))

// Test default import
println("\nDefault import:")
println("square(5) = " + square(5))
println("square(10) = " + square(10))

// Test namespace import
println("\nNamespace import (Math.*):")
println("Math.PI = " + Math.PI)
println("Math.E = " + Math.E)
println("Math.add(10, 20) = " + Math.add(10, 20))
println("Math.subtract(100, 30) = " + Math.subtract(100, 30))
println("Math.multiply(6, 7) = " + Math.multiply(6, 7))
println("Math.divide(100, 5) = " + Math.divide(100, 5))

// Note: Cannot access Math.default directly since 'default' is a keyword
// Use the default import instead
println("\nUsing default export:")
println("square(8) = " + square(8))
