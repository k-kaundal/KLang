# Polymorphism in KLang

## Table of Contents
1. [Introduction](#introduction)
2. [What is Polymorphism?](#what-is-polymorphism)
3. [Types of Polymorphism](#types-of-polymorphism)
4. [Runtime Polymorphism](#runtime-polymorphism)
5. [Dynamic Dispatch](#dynamic-dispatch)
6. [Practical Examples](#practical-examples)
7. [Benefits of Polymorphism](#benefits-of-polymorphism)
8. [Polymorphic Collections](#polymorphic-collections)
9. [Real-World Applications](#real-world-applications)
10. [Design Principles](#design-principles)
11. [Best Practices](#best-practices)
12. [Common Design Patterns](#common-design-patterns)

---

## Introduction

**Polymorphism** (from Greek: "poly" = many, "morph" = forms) is a fundamental object-oriented programming concept that allows objects of different types to be treated through a common interface. It enables a single interface to represent different underlying forms (data types).

### Why Use Polymorphism?

- **Flexibility**: Write code that works with parent types but handles child types correctly
- **Extensibility**: Add new types without modifying existing code
- **Code Reuse**: Write generic code that works with multiple types
- **Maintainability**: Reduce code duplication and simplify updates
- **Abstraction**: Hide implementation details behind common interfaces

### Real-World Analogy

Think of a TV remote control. You use the same "volume up" button regardless of whether you own a Sony, Samsung, or LG TV. Each TV brand implements volume control differently internally, but they all respond to the same command. This is polymorphism in action!

---

## What is Polymorphism?

Polymorphism allows objects of different classes to be treated as objects of a common parent class. The actual method that gets called is determined by the object's runtime type, not its compile-time type.

### Core Concept

```klang
# Parent class defines the interface
class Animal {
    fn speak() {
        println("Some sound")
    }
}

class Dog extends Animal {
    # Override with dog-specific implementation
    fn speak() {
        println("Woof!")
    }
}

class Cat extends Animal {
    # Override with cat-specific implementation
    fn speak() {
        println("Meow!")
    }
}

# Polymorphism in action:
# Reference type is Animal, but actual object types differ
let animal1: Animal = new Dog()
let animal2: Animal = new Cat()

animal1.speak()  # Output: Woof! (calls Dog's speak)
animal2.speak()  # Output: Meow! (calls Cat's speak)
```

The key insight: **same method call, different behavior** based on the actual object type.

---

## Types of Polymorphism

### 1. Compile-Time Polymorphism (Static)

**Method Overloading**: Multiple methods with the same name but different parameters.

```klang
# Note: Method overloading support depends on KLang implementation
class Calculator {
    fn add(a: int, b: int) -> int {
        return a + b
    }
    
    fn add(a: float, b: float) -> float {
        return a + b
    }
    
    fn add(a: int, b: int, c: int) -> int {
        return a + b + c
    }
}
```

*Note: KLang primarily focuses on runtime polymorphism through method overriding.*

### 2. Runtime Polymorphism (Dynamic)

**Method Overriding**: Child class provides a specific implementation of a parent class method.

```klang
class Payment {
    fn process(amount: float) {
        println("Processing payment...")
    }
}

class CreditCardPayment extends Payment {
    fn process(amount: float) {
        println("Processing credit card payment: $" + str(amount))
    }
}

class PayPalPayment extends Payment {
    fn process(amount: float) {
        println("Processing PayPal payment: $" + str(amount))
    }
}
```

Runtime polymorphism is the primary focus of KLang's polymorphic capabilities.

---

## Runtime Polymorphism

Runtime polymorphism is achieved through **method overriding** and **dynamic dispatch**. The actual method that gets executed is determined at runtime based on the object's actual type.

### How It Works

1. **Inheritance**: Child class extends parent class
2. **Method Overriding**: Child class overrides parent method
3. **Upcasting**: Child object referenced as parent type
4. **Dynamic Dispatch**: Correct method called at runtime

### Example: Shape Hierarchy

```klang
class Shape {
    let color: string = "white"
    
    fn init(c: string) {
        this.color = c
    }
    
    fn draw() {
        println("Drawing a generic shape")
    }
    
    fn area() -> float {
        return 0.0
    }
}

class Circle extends Shape {
    let radius: float = 0.0
    
    fn init(c: string, r: float) {
        super.init(c)
        this.radius = r
    }
    
    # Override draw method
    fn draw() {
        println("Drawing a " + this.color + " circle with radius " + str(this.radius))
    }
    
    # Override area method
    fn area() -> float {
        return 3.14159 * this.radius * this.radius
    }
}

class Rectangle extends Shape {
    let width: float = 0.0
    let height: float = 0.0
    
    fn init(c: string, w: float, h: float) {
        super.init(c)
        this.width = w
        this.height = h
    }
    
    # Override draw method
    fn draw() {
        println("Drawing a " + this.color + " rectangle " + str(this.width) + "x" + str(this.height))
    }
    
    # Override area method
    fn area() -> float {
        return this.width * this.height
    }
}

# Polymorphic usage
let shape1: Shape = new Circle("red", 5.0)
let shape2: Shape = new Rectangle("blue", 4.0, 3.0)

shape1.draw()  # Calls Circle's draw()
shape2.draw()  # Calls Rectangle's draw()

println("Area 1: " + str(shape1.area()))  # Calls Circle's area()
println("Area 2: " + str(shape2.area()))  # Calls Rectangle's area()
```

---

## Dynamic Dispatch

**Dynamic dispatch** (also called runtime dispatch or virtual method lookup) is the mechanism that determines which method implementation to call at runtime.

### Virtual Methods Concept

When a method is overridden, the language runtime creates a **virtual method table (vtable)** to track which implementation to call:

```
Shape vtable:          Circle vtable:         Rectangle vtable:
- draw() -> Shape     - draw() -> Circle     - draw() -> Rectangle
- area() -> Shape     - area() -> Circle     - area() -> Rectangle
```

### How Dynamic Dispatch Works

```klang
class Animal {
    fn makeSound() {
        println("Generic animal sound")
    }
    
    fn move() {
        println("Animal is moving")
    }
}

class Bird extends Animal {
    fn makeSound() {
        println("Tweet tweet!")
    }
    
    fn move() {
        println("Bird is flying")
    }
    
    fn buildNest() {
        println("Building a nest")
    }
}

# Dynamic dispatch in action
let animal: Animal = new Bird()

# These calls are dispatched to Bird's implementations
animal.makeSound()  # Output: Tweet tweet! (Bird's version)
animal.move()       # Output: Bird is flying (Bird's version)

# This won't work - buildNest() is not in Animal
# animal.buildNest()  # Error: method not found in Animal type

# To call Bird-specific methods, need Bird reference
let bird: Bird = new Bird()
bird.buildNest()    # Output: Building a nest
```

### Key Points

- Method lookup happens at **runtime**, not compile-time
- The **actual object type** determines which method is called
- Parent reference can only call methods defined in parent class
- Overridden methods automatically use dynamic dispatch

---

## Practical Examples

### Example 1: Animal Hierarchy

```klang
class Animal {
    let name: string = ""
    let species: string = ""
    
    fn init(n: string, s: string) {
        this.name = n
        this.species = s
    }
    
    fn speak() {
        println(this.name + " makes a sound")
    }
    
    fn info() {
        println("Name: " + this.name)
        println("Species: " + this.species)
    }
}

class Dog extends Animal {
    let breed: string = ""
    
    fn init(n: string, b: string) {
        super.init(n, "Canine")
        this.breed = b
    }
    
    fn speak() {
        println(this.name + " barks: Woof! Woof!")
    }
}

class Cat extends Animal {
    let indoor: bool = true
    
    fn init(n: string, ind: bool) {
        super.init(n, "Feline")
        this.indoor = ind
    }
    
    fn speak() {
        println(this.name + " meows: Meow!")
    }
}

class Cow extends Animal {
    let milkProduction: float = 0.0
    
    fn init(n: string, milk: float) {
        super.init(n, "Bovine")
        this.milkProduction = milk
    }
    
    fn speak() {
        println(this.name + " moos: Moo!")
    }
}

# Polymorphic function
fn makeAnimalSpeak(animal: Animal) {
    animal.speak()  # Calls the appropriate speak() method
}

# Usage
let dog = new Dog("Buddy", "Golden Retriever")
let cat = new Cat("Whiskers", true)
let cow = new Cow("Bessie", 25.5)

# All treated as Animals, but behave differently
makeAnimalSpeak(dog)  # Output: Buddy barks: Woof! Woof!
makeAnimalSpeak(cat)  # Output: Whiskers meows: Meow!
makeAnimalSpeak(cow)  # Output: Bessie moos: Moo!
```

### Example 2: Shape Hierarchy with Area Calculations

```klang
class Shape {
    let color: string = "black"
    let filled: bool = false
    
    fn init(c: string, f: bool) {
        this.color = c
        this.filled = f
    }
    
    fn area() -> float {
        return 0.0
    }
    
    fn perimeter() -> float {
        return 0.0
    }
    
    fn describe() {
        println("Shape: " + this.color)
        println("Filled: " + str(this.filled))
        println("Area: " + str(this.area()))
        println("Perimeter: " + str(this.perimeter()))
    }
}

class Circle extends Shape {
    let radius: float = 0.0
    
    fn init(c: string, f: bool, r: float) {
        super.init(c, f)
        this.radius = r
    }
    
    fn area() -> float {
        return 3.14159 * this.radius * this.radius
    }
    
    fn perimeter() -> float {
        return 2.0 * 3.14159 * this.radius
    }
}

class Rectangle extends Shape {
    let width: float = 0.0
    let height: float = 0.0
    
    fn init(c: string, f: bool, w: float, h: float) {
        super.init(c, f)
        this.width = w
        this.height = h
    }
    
    fn area() -> float {
        return this.width * this.height
    }
    
    fn perimeter() -> float {
        return 2.0 * (this.width + this.height)
    }
}

class Triangle extends Shape {
    let base: float = 0.0
    let height: float = 0.0
    let side1: float = 0.0
    let side2: float = 0.0
    
    fn init(c: string, f: bool, b: float, h: float, s1: float, s2: float) {
        super.init(c, f)
        this.base = b
        this.height = h
        this.side1 = s1
        this.side2 = s2
    }
    
    fn area() -> float {
        return 0.5 * this.base * this.height
    }
    
    fn perimeter() -> float {
        return this.base + this.side1 + this.side2
    }
}

# Polymorphic function to calculate total area
fn calculateTotalArea(shapes: array<Shape>) -> float {
    let total: float = 0.0
    let i: int = 0
    
    while i < len(shapes) {
        total = total + shapes[i].area()
        i = i + 1
    }
    
    return total
}

# Usage
let circle = new Circle("red", true, 5.0)
let rect = new Rectangle("blue", false, 4.0, 6.0)
let triangle = new Triangle("green", true, 3.0, 4.0, 5.0, 5.0)

circle.describe()
println("---")
rect.describe()
println("---")
triangle.describe()
```

### Example 3: Payment Processing System

```klang
class Payment {
    let amount: float = 0.0
    let description: string = ""
    
    fn init(amt: float, desc: string) {
        this.amount = amt
        this.description = desc
    }
    
    fn processPayment() {
        println("Processing payment of $" + str(this.amount))
    }
    
    fn getDetails() -> string {
        return "Payment: $" + str(this.amount) + " - " + this.description
    }
}

class CreditCardPayment extends Payment {
    let cardNumber: string = ""
    let cvv: string = ""
    
    fn init(amt: float, desc: string, card: string, c: string) {
        super.init(amt, desc)
        this.cardNumber = card
        this.cvv = c
    }
    
    fn processPayment() {
        println("Processing Credit Card Payment")
        println("Amount: $" + str(this.amount))
        println("Card: ****" + this.cardNumber)  # Show last 4 digits only
        println("Charging credit card...")
        println("Payment successful!")
    }
}

class PayPalPayment extends Payment {
    let email: string = ""
    
    fn init(amt: float, desc: string, e: string) {
        super.init(amt, desc)
        this.email = e
    }
    
    fn processPayment() {
        println("Processing PayPal Payment")
        println("Amount: $" + str(this.amount))
        println("Email: " + this.email)
        println("Redirecting to PayPal...")
        println("Payment successful!")
    }
}

class BankTransferPayment extends Payment {
    let accountNumber: string = ""
    let routingNumber: string = ""
    
    fn init(amt: float, desc: string, acc: string, routing: string) {
        super.init(amt, desc)
        this.accountNumber = acc
        this.routingNumber = routing
    }
    
    fn processPayment() {
        println("Processing Bank Transfer")
        println("Amount: $" + str(this.amount))
        println("Account: " + this.accountNumber)
        println("Initiating transfer...")
        println("Transfer complete!")
    }
}

class CryptoPayment extends Payment {
    let walletAddress: string = ""
    let cryptocurrency: string = ""
    
    fn init(amt: float, desc: string, wallet: string, crypto: string) {
        super.init(amt, desc)
        this.walletAddress = wallet
        this.cryptocurrency = crypto
    }
    
    fn processPayment() {
        println("Processing Cryptocurrency Payment")
        println("Amount: $" + str(this.amount))
        println("Wallet: " + this.walletAddress)
        println("Currency: " + this.cryptocurrency)
        println("Broadcasting transaction...")
        println("Payment confirmed!")
    }
}

# Polymorphic payment processor
class PaymentProcessor {
    fn processAllPayments(payments: array<Payment>) {
        println("=== Processing All Payments ===")
        let i: int = 0
        
        while i < len(payments) {
            println("")
            payments[i].processPayment()  # Polymorphic call
            i = i + 1
        }
    }
    
    fn calculateTotal(payments: array<Payment>) -> float {
        let total: float = 0.0
        let i: int = 0
        
        while i < len(payments) {
            total = total + payments[i].amount
            i = i + 1
        }
        
        return total
    }
}

# Usage
let payment1: Payment = new CreditCardPayment(99.99, "Online Purchase", "1234", "123")
let payment2: Payment = new PayPalPayment(49.50, "Subscription", "user@email.com")
let payment3: Payment = new BankTransferPayment(199.99, "Service Fee", "12345678", "987654")
let payment4: Payment = new CryptoPayment(299.00, "Premium Plan", "0x1234...abcd", "Bitcoin")

let processor = new PaymentProcessor()
let payments: array<Payment> = [payment1, payment2, payment3, payment4]

processor.processAllPayments(payments)
println("")
println("Total: $" + str(processor.calculateTotal(payments)))
```

### Example 4: File System Hierarchy

```klang
class FileSystemEntity {
    let name: string = ""
    let path: string = ""
    let size: int = 0
    
    fn init(n: string, p: string) {
        this.name = n
        this.path = p
    }
    
    fn getInfo() -> string {
        return this.name + " (" + this.path + ")"
    }
    
    fn open() {
        println("Opening: " + this.name)
    }
    
    fn close() {
        println("Closing: " + this.name)
    }
}

class TextFile extends FileSystemEntity {
    let encoding: string = "UTF-8"
    let lineCount: int = 0
    
    fn init(n: string, p: string, lines: int) {
        super.init(n, p)
        this.lineCount = lines
        this.size = lines * 80  # Estimate
    }
    
    fn open() {
        println("Opening text file: " + this.name)
        println("Encoding: " + this.encoding)
        println("Lines: " + str(this.lineCount))
    }
    
    fn read() -> string {
        return "Reading " + str(this.lineCount) + " lines from " + this.name
    }
}

class ImageFile extends FileSystemEntity {
    let width: int = 0
    let height: int = 0
    let format: string = "PNG"
    
    fn init(n: string, p: string, w: int, h: int, fmt: string) {
        super.init(n, p)
        this.width = w
        this.height = h
        this.format = fmt
        this.size = w * h * 4  # Rough estimate
    }
    
    fn open() {
        println("Opening image file: " + this.name)
        println("Format: " + this.format)
        println("Dimensions: " + str(this.width) + "x" + str(this.height))
        println("Size: " + str(this.size) + " bytes")
    }
    
    fn display() {
        println("Displaying image: " + this.name)
    }
}

class VideoFile extends FileSystemEntity {
    let duration: int = 0  # in seconds
    let resolution: string = "1080p"
    let codec: string = "H.264"
    
    fn init(n: string, p: string, dur: int, res: string) {
        super.init(n, p)
        this.duration = dur
        this.resolution = res
        this.size = dur * 1000000  # Rough estimate
    }
    
    fn open() {
        println("Opening video file: " + this.name)
        println("Duration: " + str(this.duration) + " seconds")
        println("Resolution: " + this.resolution)
        println("Codec: " + this.codec)
    }
    
    fn play() {
        println("Playing video: " + this.name)
    }
}

class Directory extends FileSystemEntity {
    let itemCount: int = 0
    
    fn init(n: string, p: string, items: int) {
        super.init(n, p)
        this.itemCount = items
    }
    
    fn open() {
        println("Opening directory: " + this.name)
        println("Contains " + str(this.itemCount) + " items")
    }
    
    fn list() {
        println("Listing contents of: " + this.name)
    }
}

# Polymorphic file manager
fn openMultipleFiles(files: array<FileSystemEntity>) {
    println("=== Opening Multiple Files ===")
    let i: int = 0
    
    while i < len(files) {
        println("")
        files[i].open()  # Polymorphic call - different for each type
        i = i + 1
    }
}

# Usage
let textFile: FileSystemEntity = new TextFile("readme.txt", "/docs/", 100)
let imageFile: FileSystemEntity = new ImageFile("photo.png", "/images/", 1920, 1080, "PNG")
let videoFile: FileSystemEntity = new VideoFile("tutorial.mp4", "/videos/", 300, "1080p")
let directory: FileSystemEntity = new Directory("projects", "/home/user/", 15)

let entities: array<FileSystemEntity> = [textFile, imageFile, videoFile, directory]
openMultipleFiles(entities)
```

---

## Benefits of Polymorphism

### 1. Code Reusability

Write generic code that works with parent types, automatically handling all child types:

```klang
# Single function works with all Shape types
fn printShapeInfo(shape: Shape) {
    println("Area: " + str(shape.area()))
    shape.draw()
}

# Works with any shape
printShapeInfo(new Circle("red", 5.0))
printShapeInfo(new Rectangle("blue", 4.0, 3.0))
printShapeInfo(new Triangle("green", true, 3.0, 4.0, 5.0, 5.0))
```

### 2. Flexibility

Add new types without modifying existing code:

```klang
# Add a new shape type
class Pentagon extends Shape {
    # Implementation...
}

# Existing code works without changes
let newShape: Shape = new Pentagon()
printShapeInfo(newShape)  # Just works!
```

### 3. Extensibility

Extend systems by adding new implementations:

```klang
# Add new payment method
class ApplePayPayment extends Payment {
    fn processPayment() {
        println("Processing Apple Pay...")
    }
}

# PaymentProcessor works without modification
processor.processAllPayments([new ApplePayPayment(59.99, "App Purchase")])
```

### 4. Maintainability

Reduce code duplication and simplify maintenance:

```klang
# Without polymorphism - needs updating for each new type
fn processPaymentOldWay(paymentType: string, amount: float) {
    if paymentType == "credit" {
        println("Processing credit card...")
    } else if paymentType == "paypal" {
        println("Processing PayPal...")
    } else if paymentType == "bank" {
        println("Processing bank transfer...")
    }
    # Must add new if-else for each new payment type!
}

# With polymorphism - no changes needed for new types
fn processPaymentNewWay(payment: Payment) {
    payment.processPayment()  # Works for all current and future Payment types
}
```

---

## Polymorphic Collections

One of the most powerful uses of polymorphism is storing objects of different types in the same collection.

### Arrays of Parent Type

```klang
# Array holds different animal types
class Animal {
    let name: string = ""
    
    fn init(n: string) {
        this.name = n
    }
    
    fn speak() {
        println(this.name + " makes a sound")
    }
}

class Dog extends Animal {
    fn speak() {
        println(this.name + ": Woof!")
    }
}

class Cat extends Animal {
    fn speak() {
        println(this.name + ": Meow!")
    }
}

class Bird extends Animal {
    fn speak() {
        println(this.name + ": Tweet!")
    }
}

# Polymorphic array
let animals: array<Animal> = [
    new Dog("Buddy"),
    new Cat("Whiskers"),
    new Bird("Tweety"),
    new Dog("Max"),
    new Cat("Luna")
]

# Process all animals polymorphically
fn makeAllAnimalsSpe(animals: array<Animal>) {
    let i: int = 0
    while i < len(animals) {
        animals[i].speak()  # Correct method called for each type
        i = i + 1
    }
}

makeAllAnimalsSpeak(animals)
# Output:
# Buddy: Woof!
# Whiskers: Meow!
# Tweety: Tweet!
# Max: Woof!
# Luna: Meow!
```

### Processing Mixed Types

```klang
class Document {
    let title: string = ""
    let author: string = ""
    
    fn init(t: string, a: string) {
        this.title = t
        this.author = a
    }
    
    fn print() {
        println("Printing: " + this.title + " by " + this.author)
    }
    
    fn export() -> string {
        return "Document: " + this.title
    }
}

class PDFDocument extends Document {
    let pageCount: int = 0
    
    fn init(t: string, a: string, pages: int) {
        super.init(t, a)
        this.pageCount = pages
    }
    
    fn print() {
        println("Printing PDF: " + this.title)
        println("Pages: " + str(this.pageCount))
    }
    
    fn export() -> string {
        return "PDF [" + str(this.pageCount) + " pages]: " + this.title
    }
}

class WordDocument extends Document {
    let wordCount: int = 0
    
    fn init(t: string, a: string, words: int) {
        super.init(t, a)
        this.wordCount = words
    }
    
    fn print() {
        println("Printing Word document: " + this.title)
        println("Word count: " + str(this.wordCount))
    }
    
    fn export() -> string {
        return "DOCX [" + str(this.wordCount) + " words]: " + this.title
    }
}

class SpreadsheetDocument extends Document {
    let sheetCount: int = 0
    
    fn init(t: string, a: string, sheets: int) {
        super.init(t, a)
        this.sheetCount = sheets
    }
    
    fn print() {
        println("Printing spreadsheet: " + this.title)
        println("Sheets: " + str(this.sheetCount))
    }
    
    fn export() -> string {
        return "XLSX [" + str(this.sheetCount) + " sheets]: " + this.title
    }
}

# Document management system
class DocumentManager {
    let documents: array<Document> = []
    
    fn addDocument(doc: Document) {
        # Add polymorphic document to collection
        # Note: Array operations may vary in KLang implementation
        println("Added: " + doc.title)
    }
    
    fn printAll() {
        println("=== Printing All Documents ===")
        let i: int = 0
        while i < len(this.documents) {
            this.documents[i].print()  # Polymorphic call
            println("---")
            i = i + 1
        }
    }
    
    fn exportAll() {
        println("=== Exporting All Documents ===")
        let i: int = 0
        while i < len(this.documents) {
            println(this.documents[i].export())  # Polymorphic call
            i = i + 1
        }
    }
}
```

---

## Real-World Applications

### 1. Plugin Systems

```klang
# Plugin interface
class Plugin {
    let name: string = ""
    let version: string = "1.0"
    
    fn init(n: string, v: string) {
        this.name = n
        this.version = v
    }
    
    fn initialize() {
        println("Initializing plugin: " + this.name)
    }
    
    fn execute() {
        println("Executing plugin: " + this.name)
    }
    
    fn cleanup() {
        println("Cleaning up plugin: " + this.name)
    }
}

class ImageProcessingPlugin extends Plugin {
    fn execute() {
        println("Running image processing algorithms...")
        println("Applying filters and transformations...")
    }
}

class DataAnalyticsPlugin extends Plugin {
    fn execute() {
        println("Analyzing data...")
        println("Generating statistics and reports...")
    }
}

class SecurityScanPlugin extends Plugin {
    fn execute() {
        println("Scanning for security vulnerabilities...")
        println("Running malware detection...")
    }
}

# Plugin manager uses polymorphism
class PluginManager {
    fn runPlugin(plugin: Plugin) {
        plugin.initialize()
        plugin.execute()
        plugin.cleanup()
    }
    
    fn runAllPlugins(plugins: array<Plugin>) {
        let i: int = 0
        while i < len(plugins) {
            println("")
            this.runPlugin(plugins[i])
            i = i + 1
        }
    }
}
```

### 2. GUI Frameworks

```klang
class Widget {
    let x: int = 0
    let y: int = 0
    let width: int = 100
    let height: int = 50
    let visible: bool = true
    
    fn init(xPos: int, yPos: int, w: int, h: int) {
        this.x = xPos
        this.y = yPos
        this.width = w
        this.height = h
    }
    
    fn render() {
        println("Rendering widget at (" + str(this.x) + ", " + str(this.y) + ")")
    }
    
    fn handleClick() {
        println("Widget clicked")
    }
}

class Button extends Widget {
    let label: string = "Button"
    
    fn init(xPos: int, yPos: int, text: string) {
        super.init(xPos, yPos, 120, 40)
        this.label = text
    }
    
    fn render() {
        println("Rendering button: [" + this.label + "] at (" + str(this.x) + ", " + str(this.y) + ")")
    }
    
    fn handleClick() {
        println("Button '" + this.label + "' clicked!")
    }
}

class TextBox extends Widget {
    let placeholder: string = "Enter text..."
    let text: string = ""
    
    fn init(xPos: int, yPos: int, ph: string) {
        super.init(xPos, yPos, 200, 30)
        this.placeholder = ph
    }
    
    fn render() {
        println("Rendering textbox at (" + str(this.x) + ", " + str(this.y) + ")")
        if this.text == "" {
            println("  Placeholder: " + this.placeholder)
        } else {
            println("  Text: " + this.text)
        }
    }
    
    fn handleClick() {
        println("TextBox focused for input")
    }
}

class CheckBox extends Widget {
    let label: string = ""
    let checked: bool = false
    
    fn init(xPos: int, yPos: int, text: string) {
        super.init(xPos, yPos, 150, 30)
        this.label = text
    }
    
    fn render() {
        let status: string = "[ ]"
        if this.checked {
            status = "[X]"
        }
        println("Rendering checkbox: " + status + " " + this.label)
    }
    
    fn handleClick() {
        this.checked = !this.checked
        println("CheckBox '" + this.label + "' toggled: " + str(this.checked))
    }
}

# GUI framework uses polymorphism
class Window {
    let widgets: array<Widget> = []
    
    fn addWidget(widget: Widget) {
        println("Adding widget to window")
    }
    
    fn renderAll() {
        println("=== Rendering Window ===")
        let i: int = 0
        while i < len(this.widgets) {
            this.widgets[i].render()
            i = i + 1
        }
    }
    
    fn handleClickAt(x: int, y: int) {
        println("Click detected at (" + str(x) + ", " + str(y) + ")")
        # In real implementation, would check which widget was clicked
    }
}
```

### 3. Database Drivers

```klang
class DatabaseConnection {
    let host: string = ""
    let port: int = 0
    let database: string = ""
    let connected: bool = false
    
    fn init(h: string, p: int, db: string) {
        this.host = h
        this.port = p
        this.database = db
    }
    
    fn connect() {
        println("Connecting to database...")
        this.connected = true
    }
    
    fn disconnect() {
        println("Disconnecting from database...")
        this.connected = false
    }
    
    fn query(sql: string) -> string {
        return "Query results"
    }
}

class MySQLConnection extends DatabaseConnection {
    fn connect() {
        println("Establishing MySQL connection to " + this.host + ":" + str(this.port))
        this.connected = true
    }
    
    fn query(sql: string) -> string {
        println("Executing MySQL query: " + sql)
        return "MySQL result set"
    }
}

class PostgreSQLConnection extends DatabaseConnection {
    fn connect() {
        println("Establishing PostgreSQL connection to " + this.host + ":" + str(this.port))
        this.connected = true
    }
    
    fn query(sql: string) -> string {
        println("Executing PostgreSQL query: " + sql)
        return "PostgreSQL result set"
    }
}

class MongoDBConnection extends DatabaseConnection {
    fn connect() {
        println("Establishing MongoDB connection to " + this.host + ":" + str(this.port))
        this.connected = true
    }
    
    fn query(sql: string) -> string {
        println("Executing MongoDB query: " + sql)
        return "MongoDB documents"
    }
}

# Database abstraction layer
class DatabaseManager {
    fn executeQuery(connection: DatabaseConnection, sql: string) -> string {
        if !connection.connected {
            connection.connect()
        }
        return connection.query(sql)  # Polymorphic - works with any DB
    }
}
```

---

## Design Principles

### 1. Program to Interface, Not Implementation

**Bad Practice:**
```klang
# Tightly coupled to specific implementations
class OrderProcessor {
    let creditCardProcessor: CreditCardPayment = null
    let paypalProcessor: PayPalPayment = null
    
    fn processCreditCard(amount: float) {
        # Logic specific to credit card
    }
    
    fn processPayPal(amount: float) {
        # Logic specific to PayPal
    }
}
```

**Good Practice:**
```klang
# Loosely coupled through parent interface
class OrderProcessor {
    let paymentMethod: Payment = null
    
    fn init(payment: Payment) {
        this.paymentMethod = payment
    }
    
    fn process(amount: float) {
        # Works with any Payment type
        this.paymentMethod.processPayment()
    }
}

# Can easily switch implementations
let processor1 = new OrderProcessor(new CreditCardPayment(...))
let processor2 = new OrderProcessor(new PayPalPayment(...))
let processor3 = new OrderProcessor(new CryptoPayment(...))
```

### 2. Dependency Inversion Principle

High-level modules should depend on abstractions, not concrete implementations.

```klang
# Abstraction (parent class)
class Notifier {
    fn send(message: string) {
        println("Sending notification: " + message)
    }
}

# Concrete implementations
class EmailNotifier extends Notifier {
    let email: string = ""
    
    fn init(e: string) {
        this.email = e
    }
    
    fn send(message: string) {
        println("Sending email to " + this.email + ": " + message)
    }
}

class SMSNotifier extends Notifier {
    let phone: string = ""
    
    fn init(p: string) {
        this.phone = p
    }
    
    fn send(message: string) {
        println("Sending SMS to " + this.phone + ": " + message)
    }
}

class PushNotifier extends Notifier {
    let deviceId: string = ""
    
    fn init(d: string) {
        this.deviceId = d
    }
    
    fn send(message: string) {
        println("Sending push notification to " + this.deviceId + ": " + message)
    }
}

# High-level module depends on abstraction
class NotificationService {
    let notifier: Notifier = null
    
    fn init(n: Notifier) {
        this.notifier = n  # Accepts any Notifier type
    }
    
    fn notify(message: string) {
        this.notifier.send(message)  # Polymorphic call
    }
}

# Easy to change notification method
let service1 = new NotificationService(new EmailNotifier("user@example.com"))
let service2 = new NotificationService(new SMSNotifier("+1234567890"))
let service3 = new NotificationService(new PushNotifier("device123"))

service1.notify("Hello!")  # Sends email
service2.notify("Hello!")  # Sends SMS
service3.notify("Hello!")  # Sends push notification
```

### 3. Open/Closed Principle

Classes should be open for extension but closed for modification.

```klang
# Base class - closed for modification
class Report {
    let title: string = ""
    let data: string = ""
    
    fn init(t: string, d: string) {
        this.title = t
        this.data = d
    }
    
    fn generate() {
        println("Generating report: " + this.title)
    }
}

# Extended through inheritance - open for extension
class PDFReport extends Report {
    fn generate() {
        println("Generating PDF report: " + this.title)
        println("Formatting as PDF...")
    }
}

class ExcelReport extends Report {
    fn generate() {
        println("Generating Excel report: " + this.title)
        println("Creating spreadsheet...")
    }
}

class HTMLReport extends Report {
    fn generate() {
        println("Generating HTML report: " + this.title)
        println("Creating web page...")
    }
}

# Original Report class never modified, but functionality extended
```

---

## Best Practices

### 1. Design for Polymorphism

Use parent types in method signatures:

```klang
# Good: Accepts any Shape
fn displayShape(shape: Shape) {
    shape.draw()
}

# Bad: Specific to Circle only
fn displayCircle(circle: Circle) {
    circle.draw()
}
```

### 2. Use Meaningful Method Names

Overridden methods should have consistent meaning:

```klang
class Vehicle {
    # All vehicles can start
    fn start() {
        println("Vehicle starting...")
    }
}

class Car extends Vehicle {
    # Car's start makes sense
    fn start() {
        println("Car engine starting...")
    }
}

class Bicycle extends Vehicle {
    # Bicycle's start makes sense
    fn start() {
        println("Ready to pedal...")
    }
}
```

### 3. Maintain Consistent Return Types

```klang
class Shape {
    fn area() -> float {  # Returns float
        return 0.0
    }
}

class Circle extends Shape {
    # Good: Returns same type
    fn area() -> float {
        return 3.14159 * this.radius * this.radius
    }
}
```

### 4. Don't Break Parent Contract

Child classes should honor parent class expectations:

```klang
class Stack {
    fn push(item: int) {
        # Adds item to stack
    }
    
    fn pop() -> int {
        # Should return and remove top item
        return 0
    }
}

# Good: Maintains contract
class LimitedStack extends Stack {
    let maxSize: int = 100
    
    fn push(item: int) {
        # Still adds item (with size check)
        if this.size < this.maxSize {
            super.push(item)
        }
    }
}

# Bad: Breaks contract
# class BrokenStack extends Stack {
#     fn pop() {
#         # ERROR: Should return int, not void
#     }
# }
```

### 5. Use Abstract Classes for Interfaces

When methods must be implemented by children:

```klang
abstract class Drawable {
    # Force all children to implement draw
    public abstract fn draw();
    
    # Common functionality
    public fn prepare() {
        println("Preparing to draw...")
    }
}

class Circle extends Drawable {
    # Must implement draw
    public fn draw() {
        println("Drawing circle")
    }
}
```

---

## Common Design Patterns

### 1. Strategy Pattern

Encapsulate algorithms and make them interchangeable:

```klang
# Strategy interface
class SortStrategy {
    fn sort(arr: array<int>) {
        println("Sorting array...")
    }
}

# Concrete strategies
class QuickSort extends SortStrategy {
    fn sort(arr: array<int>) {
        println("Sorting using QuickSort algorithm")
        # QuickSort implementation
    }
}

class MergeSort extends SortStrategy {
    fn sort(arr: array<int>) {
        println("Sorting using MergeSort algorithm")
        # MergeSort implementation
    }
}

class BubbleSort extends SortStrategy {
    fn sort(arr: array<int>) {
        println("Sorting using BubbleSort algorithm")
        # BubbleSort implementation
    }
}

# Context uses strategy polymorphically
class ArraySorter {
    let strategy: SortStrategy = null
    
    fn init(s: SortStrategy) {
        this.strategy = s
    }
    
    fn setStrategy(s: SortStrategy) {
        this.strategy = s
    }
    
    fn performSort(arr: array<int>) {
        this.strategy.sort(arr)  # Polymorphic call
    }
}

# Usage - easily switch strategies
let sorter = new ArraySorter(new QuickSort())
sorter.performSort([5, 2, 8, 1, 9])

sorter.setStrategy(new MergeSort())
sorter.performSort([5, 2, 8, 1, 9])

sorter.setStrategy(new BubbleSort())
sorter.performSort([5, 2, 8, 1, 9])
```

### 2. Factory Pattern

Create objects through a common interface:

```klang
# Product hierarchy
class Animal {
    let name: string = ""
    
    fn init(n: string) {
        this.name = n
    }
    
    fn speak() {
        println("Animal sound")
    }
}

class Dog extends Animal {
    fn speak() {
        println(this.name + ": Woof!")
    }
}

class Cat extends Animal {
    fn speak() {
        println(this.name + ": Meow!")
    }
}

class Bird extends Animal {
    fn speak() {
        println(this.name + ": Tweet!")
    }
}

# Factory
class AnimalFactory {
    fn createAnimal(type: string, name: string) -> Animal {
        if type == "dog" {
            return new Dog(name)
        } else if type == "cat" {
            return new Cat(name)
        } else if type == "bird" {
            return new Bird(name)
        } else {
            return new Animal(name)
        }
    }
}

# Usage - polymorphic creation and usage
let factory = new AnimalFactory()

let animal1: Animal = factory.createAnimal("dog", "Buddy")
let animal2: Animal = factory.createAnimal("cat", "Whiskers")
let animal3: Animal = factory.createAnimal("bird", "Tweety")

animal1.speak()  # Woof!
animal2.speak()  # Meow!
animal3.speak()  # Tweet!
```

### 3. Template Method Pattern

Define algorithm skeleton, let subclasses implement steps:

```klang
class DataProcessor {
    # Template method - defines the algorithm structure
    fn process() {
        this.readData()
        this.processData()
        this.writeData()
    }
    
    fn readData() {
        println("Reading data...")
    }
    
    fn processData() {
        println("Processing data...")
    }
    
    fn writeData() {
        println("Writing data...")
    }
}

class CSVProcessor extends DataProcessor {
    fn readData() {
        println("Reading CSV file...")
    }
    
    fn processData() {
        println("Parsing CSV data...")
    }
    
    fn writeData() {
        println("Writing to CSV file...")
    }
}

class JSONProcessor extends DataProcessor {
    fn readData() {
        println("Reading JSON file...")
    }
    
    fn processData() {
        println("Parsing JSON data...")
    }
    
    fn writeData() {
        println("Writing to JSON file...")
    }
}

class XMLProcessor extends DataProcessor {
    fn readData() {
        println("Reading XML file...")
    }
    
    fn processData() {
        println("Parsing XML data...")
    }
    
    fn writeData() {
        println("Writing to XML file...")
    }
}

# Usage - same process method, different implementations
let csvProc: DataProcessor = new CSVProcessor()
let jsonProc: DataProcessor = new JSONProcessor()
let xmlProc: DataProcessor = new XMLProcessor()

csvProc.process()   # Uses CSV-specific methods
jsonProc.process()  # Uses JSON-specific methods
xmlProc.process()   # Uses XML-specific methods
```

---

## Summary

**Polymorphism** is a cornerstone of object-oriented programming that enables:

- **Multiple forms**: Same interface, different implementations
- **Runtime dispatch**: Method called determined at runtime
- **Flexible code**: Write once, work with many types
- **Extensible systems**: Add new types without modifying existing code
- **Maintainable applications**: Reduce duplication and complexity

### Key Takeaways

1. **Use parent types** in function parameters and return types
2. **Override methods** to provide specialized behavior
3. **Design with abstraction** - program to interfaces, not implementations
4. **Apply design patterns** - Strategy, Factory, Template Method
5. **Follow SOLID principles** - especially Open/Closed and Dependency Inversion
6. **Test polymorphic code** with multiple concrete types
7. **Document expectations** of parent class methods

Polymorphism transforms rigid, type-specific code into flexible, extensible systems that adapt to new requirements with minimal changes.

---

## Next Steps

Continue learning:
- [Abstract Classes](07_abstract_classes.md) - Enforce implementation contracts
- [Interfaces](08_interfaces.md) - Define pure contracts (if supported)
- [Design Patterns](09_design_patterns.md) - Apply polymorphism in patterns
- [Advanced OOP](10_advanced_oop.md) - Master advanced techniques

Happy coding with polymorphism! 🚀
