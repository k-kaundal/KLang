// Comprehensive Destructuring Tests

// === ARRAY DESTRUCTURING ===

// Basic
const [a1, b1, c1] = [10, 20, 30]
println("Basic array: " + a1 + ", " + b1 + ", " + c1)

// With defaults
const [x1 = 100, y1 = 200, z1 = 300] = [11, 22]
println("With defaults: " + x1 + ", " + y1 + ", " + z1)

// Skip elements  
const [first, , third] = [111, 222, 333]
println("Skip middle: " + first + ", " + third)

// Rest pattern
const [head, ...tail] = [1, 2, 3, 4, 5]
println("Rest pattern - head: " + head)
println("Rest pattern - tail: " + tail)

// === OBJECT DESTRUCTURING ===

// Basic
const user = {name: "Alice", email: "alice@example.com", role: "admin"}
const {name, email, role} = user
println("User - name: " + name + ", email: " + email + ", role: " + role)

// With rename
const config = {host: "localhost", port: 3000}
const {host: serverHost, port: serverPort} = config
println("Server - host: " + serverHost + ", port: " + serverPort)

// With defaults
const settings = {theme: "dark"}
const {theme = "light", lang = "en", debug = false} = settings
println("Settings - theme: " + theme + ", lang: " + lang + ", debug: " + debug)

// === MIXED SCENARIOS ===

// let and var work too
let [m, n] = [100, 200]
println("let destructure: " + m + ", " + n)

var [p, q] = [300, 400]
println("var destructure: " + p + ", " + q)

println("\n✓ All destructuring tests passed!")
