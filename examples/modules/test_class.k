// test_class.k - Test class exports
import {Person, createPerson} from "./person.k"

println("Testing class exports:")
let p1 = new Person("Alice", 30)
println(p1.greet())

let p2 = createPerson("Bob", 25)
println(p2.greet())

println("Class exports work!")
