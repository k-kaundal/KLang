# Design Patterns in KLang

## Table of Contents
1. [Introduction](#introduction)
2. [What are Design Patterns?](#what-are-design-patterns)
3. [Creational Patterns](#creational-patterns)
4. [Structural Patterns](#structural-patterns)
5. [Behavioral Patterns](#behavioral-patterns)
6. [Practical Examples](#practical-examples)
7. [Anti-Patterns](#anti-patterns)
8. [Best Practices](#best-practices)
9. [Interview Problems](#interview-problems)

---

## Introduction

**Design patterns** are reusable solutions to commonly occurring problems in software design. They represent best practices refined over decades by expert developers and provide a shared vocabulary for discussing architectural decisions.

### History: The Gang of Four

In 1994, four authors—Erich Gamma, Richard Helm, Ralph Johnson, and John Vlissides (collectively known as the "Gang of Four" or GoF)—published the seminal book *"Design Patterns: Elements of Reusable Object-Oriented Software"*. This book catalogued 23 classic design patterns that have become fundamental to object-oriented programming.

### Why Use Design Patterns?

- **Proven Solutions**: Battle-tested approaches to common problems
- **Common Vocabulary**: Communicate complex ideas quickly ("Let's use the Observer pattern")
- **Best Practices**: Learn from collective experience of expert developers
- **Flexibility**: Build maintainable, extensible systems
- **Code Quality**: Improve structure and reduce complexity

### Real-World Analogy

Think of design patterns like architectural blueprints. Just as architects don't reinvent how to build a door or window for each building, software developers don't need to reinvent solutions for common problems. Patterns provide proven templates that can be adapted to specific needs.

### Pattern Categories

The Gang of Four organized patterns into three categories:

1. **Creational Patterns**: How objects are created
2. **Structural Patterns**: How objects are composed
3. **Behavioral Patterns**: How objects interact and communicate

---

## What are Design Patterns?

A design pattern is NOT:
- A specific piece of code that can be copied
- A library or framework
- A complete design ready for implementation

A design pattern IS:
- A description of how to solve a problem
- A template that can be applied in many situations
- A best practice approach to common design challenges

### Pattern Structure

Each pattern typically includes:

1. **Intent**: What problem does it solve?
2. **Motivation**: Why is it needed?
3. **Structure**: UML diagram showing relationships
4. **Participants**: Classes/objects involved
5. **Implementation**: Code example
6. **Consequences**: Trade-offs and results

---

## Creational Patterns

Creational patterns deal with object creation mechanisms, trying to create objects in a manner suitable to the situation.

### 1. Singleton Pattern

**Problem**: Ensure a class has only one instance and provide a global point of access to it.

**When to Use**:
- Need exactly one instance of a class (e.g., configuration manager, logger)
- Need global access point to that instance
- Instance should be extensible by subclassing

**Structure (ASCII UML)**:
```
┌─────────────────────────┐
│      Singleton          │
├─────────────────────────┤
│ - instance: Singleton   │ (static)
├─────────────────────────┤
│ - init()                │ (private)
│ + getInstance()         │ (static)
│ + operation()           │
└─────────────────────────┘
```

**Implementation**:

```klang
class Logger {
    # Static instance variable
    static let instance: Logger = null
    
    let logFile: string = "app.log"
    let logCount: int = 0
    
    # Private constructor (conceptually - KLang may not enforce private)
    fn init() {
        this.logFile = "app.log"
        this.logCount = 0
        println("Logger initialized")
    }
    
    # Static factory method
    static fn getInstance() -> Logger {
        if Logger.instance == null {
            Logger.instance = new Logger()
        }
        return Logger.instance
    }
    
    fn log(message: string) {
        this.logCount = this.logCount + 1
        println("[LOG " + str(this.logCount) + "] " + message)
    }
    
    fn getLogCount() -> int {
        return this.logCount
    }
}

# Usage
let logger1: Logger = Logger.getInstance()
logger1.log("First message")

let logger2: Logger = Logger.getInstance()
logger2.log("Second message")

# Both reference the same instance
println("Log count: " + str(logger2.getLogCount()))  # Output: 2
```

**Real-World Example: Database Connection Manager**:

```klang
class DatabaseConnection {
    static let instance: DatabaseConnection = null
    
    let connectionString: string = ""
    let isConnected: bool = false
    let queryCount: int = 0
    
    fn init() {
        this.connectionString = "db://localhost:5432/mydb"
        this.isConnected = false
        this.queryCount = 0
    }
    
    static fn getInstance() -> DatabaseConnection {
        if DatabaseConnection.instance == null {
            DatabaseConnection.instance = new DatabaseConnection()
            DatabaseConnection.instance.connect()
        }
        return DatabaseConnection.instance
    }
    
    fn connect() {
        if !this.isConnected {
            println("Connecting to database: " + this.connectionString)
            this.isConnected = true
        }
    }
    
    fn executeQuery(query: string) -> string {
        if this.isConnected {
            this.queryCount = this.queryCount + 1
            println("Executing query: " + query)
            return "Query result"
        }
        return "Not connected"
    }
    
    fn getQueryCount() -> int {
        return this.queryCount
    }
}

# Usage across application
let db1: DatabaseConnection = DatabaseConnection.getInstance()
db1.executeQuery("SELECT * FROM users")

let db2: DatabaseConnection = DatabaseConnection.getInstance()
db2.executeQuery("SELECT * FROM products")

println("Total queries: " + str(db2.getQueryCount()))  # Output: 2
```

**Advantages**:
- Controlled access to sole instance
- Reduced namespace pollution
- Lazy initialization possible
- Easy to extend

**Disadvantages**:
- Global state can make testing difficult
- Can hide dependencies
- Can become a bottleneck in concurrent scenarios

---

### 2. Factory Pattern

**Problem**: Create objects without specifying their exact class. Delegate instantiation logic to subclasses.

**When to Use**:
- Class can't anticipate the type of objects it needs to create
- Class wants subclasses to specify objects to create
- Need to encapsulate object creation logic

**Structure (ASCII UML)**:
```
┌─────────────────┐
│    Creator      │
├─────────────────┤
│ + factoryMethod()│────► creates ────► Product
│ + operation()   │
└─────────────────┘
        △
        │
        │ extends
        │
┌─────────────────┐
│ ConcreteCreator │
├─────────────────┤
│ + factoryMethod()│────► creates ────► ConcreteProduct
└─────────────────┘
```

**Implementation: Shape Factory**:

```klang
# Product interface
class Shape {
    fn draw() {
        println("Drawing shape")
    }
    
    fn getArea() -> float {
        return 0.0
    }
}

# Concrete Products
class Circle extends Shape {
    let radius: float = 0.0
    
    fn init(r: float) {
        this.radius = r
    }
    
    fn draw() {
        println("Drawing Circle with radius: " + str(this.radius))
    }
    
    fn getArea() -> float {
        return 3.14159 * this.radius * this.radius
    }
}

class Rectangle extends Shape {
    let width: float = 0.0
    let height: float = 0.0
    
    fn init(w: float, h: float) {
        this.width = w
        this.height = h
    }
    
    fn draw() {
        println("Drawing Rectangle " + str(this.width) + "x" + str(this.height))
    }
    
    fn getArea() -> float {
        return this.width * this.height
    }
}

class Triangle extends Shape {
    let base: float = 0.0
    let height: float = 0.0
    
    fn init(b: float, h: float) {
        this.base = b
        this.height = h
    }
    
    fn draw() {
        println("Drawing Triangle base:" + str(this.base) + " height:" + str(this.height))
    }
    
    fn getArea() -> float {
        return 0.5 * this.base * this.height
    }
}

# Factory
class ShapeFactory {
    static fn createShape(shapeType: string, param1: float, param2: float) -> Shape {
        if shapeType == "circle" {
            return new Circle(param1)
        } else if shapeType == "rectangle" {
            return new Rectangle(param1, param2)
        } else if shapeType == "triangle" {
            return new Triangle(param1, param2)
        }
        return new Shape()  # Default shape
    }
}

# Usage
let circle: Shape = ShapeFactory.createShape("circle", 5.0, 0.0)
circle.draw()
println("Area: " + str(circle.getArea()))

let rectangle: Shape = ShapeFactory.createShape("rectangle", 4.0, 6.0)
rectangle.draw()
println("Area: " + str(rectangle.getArea()))

let triangle: Shape = ShapeFactory.createShape("triangle", 3.0, 4.0)
triangle.draw()
println("Area: " + str(triangle.getArea()))
```

**Real-World Example: Document Factory**:

```klang
class Document {
    let title: string = ""
    let content: string = ""
    
    fn init(t: string) {
        this.title = t
        this.content = ""
    }
    
    fn open() {
        println("Opening document: " + this.title)
    }
    
    fn save() {
        println("Saving document: " + this.title)
    }
}

class PDFDocument extends Document {
    let pageCount: int = 0
    
    fn init(t: string) {
        super.init(t)
        this.pageCount = 1
    }
    
    fn open() {
        println("Opening PDF document: " + this.title + " (" + str(this.pageCount) + " pages)")
    }
    
    fn exportToPrint() {
        println("Exporting PDF to printer")
    }
}

class WordDocument extends Document {
    let wordCount: int = 0
    
    fn init(t: string) {
        super.init(t)
        this.wordCount = 0
    }
    
    fn open() {
        println("Opening Word document: " + this.title)
    }
    
    fn checkSpelling() {
        println("Running spell check on Word document")
    }
}

class SpreadsheetDocument extends Document {
    let sheetCount: int = 0
    
    fn init(t: string) {
        super.init(t)
        this.sheetCount = 1
    }
    
    fn open() {
        println("Opening Spreadsheet: " + this.title + " (" + str(this.sheetCount) + " sheets)")
    }
    
    fn calculateFormulas() {
        println("Calculating spreadsheet formulas")
    }
}

class DocumentFactory {
    static fn createDocument(docType: string, title: string) -> Document {
        if docType == "pdf" {
            return new PDFDocument(title)
        } else if docType == "word" {
            return new WordDocument(title)
        } else if docType == "spreadsheet" {
            return new SpreadsheetDocument(title)
        }
        return new Document(title)
    }
}

# Usage
let pdf: Document = DocumentFactory.createDocument("pdf", "Report.pdf")
pdf.open()

let word: Document = DocumentFactory.createDocument("word", "Letter.docx")
word.open()

let excel: Document = DocumentFactory.createDocument("spreadsheet", "Budget.xlsx")
excel.open()
```

**Advantages**:
- Loose coupling between creator and products
- Single Responsibility Principle
- Open/Closed Principle (easy to add new products)
- Centralized object creation logic

**Disadvantages**:
- Can lead to many subclasses
- More complex code structure

---

### 3. Builder Pattern

**Problem**: Construct complex objects step by step. Create different representations using the same construction process.

**When to Use**:
- Object creation has many optional parameters
- Want to create different representations of an object
- Construction process must allow different representations
- Avoid "telescoping constructor" anti-pattern

**Structure (ASCII UML)**:
```
┌─────────────┐         ┌─────────────┐
│  Director   │────────►│   Builder   │
└─────────────┘         ├─────────────┤
                        │ + buildPart()│
                        └─────────────┘
                              △
                              │
                              │
                    ┌─────────────────┐
                    │ ConcreteBuilder │
                    ├─────────────────┤
                    │ + buildPart()   │
                    │ + getResult()   │
                    └─────────────────┘
```

**Implementation: Pizza Builder**:

```klang
# Product
class Pizza {
    let size: string = ""
    let crust: string = ""
    let cheese: bool = false
    let pepperoni: bool = false
    let mushrooms: bool = false
    let olives: bool = false
    let onions: bool = false
    
    fn describe() {
        println("Pizza: " + this.size + " with " + this.crust + " crust")
        if this.cheese { println("  - Cheese") }
        if this.pepperoni { println("  - Pepperoni") }
        if this.mushrooms { println("  - Mushrooms") }
        if this.olives { println("  - Olives") }
        if this.onions { println("  - Onions") }
    }
}

# Builder
class PizzaBuilder {
    let pizza: Pizza = null
    
    fn init() {
        this.pizza = new Pizza()
    }
    
    fn setSize(size: string) -> PizzaBuilder {
        this.pizza.size = size
        return this
    }
    
    fn setCrust(crust: string) -> PizzaBuilder {
        this.pizza.crust = crust
        return this
    }
    
    fn addCheese() -> PizzaBuilder {
        this.pizza.cheese = true
        return this
    }
    
    fn addPepperoni() -> PizzaBuilder {
        this.pizza.pepperoni = true
        return this
    }
    
    fn addMushrooms() -> PizzaBuilder {
        this.pizza.mushrooms = true
        return this
    }
    
    fn addOlives() -> PizzaBuilder {
        this.pizza.olives = true
        return this
    }
    
    fn addOnions() -> PizzaBuilder {
        this.pizza.onions = true
        return this
    }
    
    fn build() -> Pizza {
        return this.pizza
    }
}

# Usage with method chaining
let margherita: Pizza = new PizzaBuilder()
    .setSize("Medium")
    .setCrust("Thin")
    .addCheese()
    .build()

margherita.describe()

let supreme: Pizza = new PizzaBuilder()
    .setSize("Large")
    .setCrust("Thick")
    .addCheese()
    .addPepperoni()
    .addMushrooms()
    .addOlives()
    .addOnions()
    .build()

supreme.describe()

let simple: Pizza = new PizzaBuilder()
    .setSize("Small")
    .setCrust("Regular")
    .addCheese()
    .addPepperoni()
    .build()

simple.describe()
```

**Real-World Example: HTTP Request Builder**:

```klang
class HttpRequest {
    let url: string = ""
    let method: string = "GET"
    let headers: array<string> = []
    let body: string = ""
    let timeout: int = 30
    let retries: int = 0
    
    fn execute() {
        println("Executing " + this.method + " request to: " + this.url)
        println("Timeout: " + str(this.timeout) + "s, Retries: " + str(this.retries))
        
        let i: int = 0
        while i < len(this.headers) {
            println("Header: " + this.headers[i])
            i = i + 1
        }
        
        if this.body != "" {
            println("Body: " + this.body)
        }
    }
}

class HttpRequestBuilder {
    let request: HttpRequest = null
    
    fn init() {
        this.request = new HttpRequest()
    }
    
    fn setUrl(url: string) -> HttpRequestBuilder {
        this.request.url = url
        return this
    }
    
    fn setMethod(method: string) -> HttpRequestBuilder {
        this.request.method = method
        return this
    }
    
    fn addHeader(header: string) -> HttpRequestBuilder {
        # In real implementation, would append to array
        this.request.headers = [header]  # Simplified
        return this
    }
    
    fn setBody(body: string) -> HttpRequestBuilder {
        this.request.body = body
        return this
    }
    
    fn setTimeout(timeout: int) -> HttpRequestBuilder {
        this.request.timeout = timeout
        return this
    }
    
    fn setRetries(retries: int) -> HttpRequestBuilder {
        this.request.retries = retries
        return this
    }
    
    fn build() -> HttpRequest {
        return this.request
    }
}

# Usage
let getRequest: HttpRequest = new HttpRequestBuilder()
    .setUrl("https://api.example.com/users")
    .setMethod("GET")
    .addHeader("Authorization: Bearer token123")
    .setTimeout(60)
    .build()

getRequest.execute()

let postRequest: HttpRequest = new HttpRequestBuilder()
    .setUrl("https://api.example.com/users")
    .setMethod("POST")
    .addHeader("Content-Type: application/json")
    .setBody("{\"name\": \"John\", \"age\": 30}")
    .setTimeout(30)
    .setRetries(3)
    .build()

postRequest.execute()
```

**Advantages**:
- Clear, readable object construction
- Immutable objects can be built step-by-step
- Same construction process can create different representations
- Better control over construction process

**Disadvantages**:
- Increases overall code complexity
- Requires creating separate Builder class
- More verbose than simple constructor

---

## Structural Patterns

Structural patterns deal with object composition, creating relationships between objects to form larger structures.

### 4. Adapter Pattern

**Problem**: Make incompatible interfaces work together. Convert the interface of a class into another interface clients expect.

**When to Use**:
- Want to use an existing class but its interface doesn't match
- Need to create a reusable class that cooperates with unrelated classes
- Want to use several existing subclasses but impractical to adapt by subclassing

**Structure (ASCII UML)**:
```
┌─────────────┐         ┌─────────────┐
│   Client    │────────►│   Target    │
└─────────────┘         ├─────────────┤
                        │ + request() │
                        └─────────────┘
                              △
                              │
                              │ implements
                              │
                        ┌─────────────┐         ┌─────────────┐
                        │   Adapter   │────────►│  Adaptee    │
                        ├─────────────┤         ├─────────────┤
                        │ + request() │         │ + specific()│
                        └─────────────┘         └─────────────┘
```

**Implementation: Media Player Adapter**:

```klang
# Target interface
class MediaPlayer {
    fn play(audioType: string, fileName: string) {
        println("Playing audio file")
    }
}

# Adaptee - Advanced player with different interface
class AdvancedMediaPlayer {
    fn playVLC(fileName: string) {
        println("Playing VLC file: " + fileName)
    }
    
    fn playMP4(fileName: string) {
        println("Playing MP4 file: " + fileName)
    }
}

# Adapter - Makes AdvancedMediaPlayer compatible with MediaPlayer
class MediaAdapter extends MediaPlayer {
    let advancedPlayer: AdvancedMediaPlayer = null
    
    fn init(audioType: string) {
        this.advancedPlayer = new AdvancedMediaPlayer()
    }
    
    fn play(audioType: string, fileName: string) {
        if audioType == "vlc" {
            this.advancedPlayer.playVLC(fileName)
        } else if audioType == "mp4" {
            this.advancedPlayer.playMP4(fileName)
        }
    }
}

# Enhanced MediaPlayer using adapter
class AudioPlayer extends MediaPlayer {
    let adapter: MediaAdapter = null
    
    fn play(audioType: string, fileName: string) {
        # Built-in support for mp3
        if audioType == "mp3" {
            println("Playing MP3 file: " + fileName)
        }
        # Use adapter for other formats
        else if audioType == "vlc" || audioType == "mp4" {
            this.adapter = new MediaAdapter(audioType)
            this.adapter.play(audioType, fileName)
        }
        else {
            println("Invalid format: " + audioType)
        }
    }
}

# Usage
let player: AudioPlayer = new AudioPlayer()
player.play("mp3", "song.mp3")
player.play("vlc", "movie.vlc")
player.play("mp4", "video.mp4")
player.play("avi", "clip.avi")  # Unsupported
```

**Real-World Example: Payment Gateway Adapter**:

```klang
# Our standard payment interface
class PaymentProcessor {
    fn processPayment(amount: float) -> bool {
        return false
    }
    
    fn refund(transactionId: string, amount: float) -> bool {
        return false
    }
}

# Third-party payment service (Adaptee)
class StripePaymentService {
    fn charge(amountInCents: int, currency: string) -> string {
        println("Stripe: Charging " + str(amountInCents) + " " + currency)
        return "stripe_tx_12345"
    }
    
    fn reverseCharge(chargeId: string) -> bool {
        println("Stripe: Reversing charge " + chargeId)
        return true
    }
}

# Adapter for Stripe
class StripeAdapter extends PaymentProcessor {
    let stripeService: StripePaymentService = null
    
    fn init() {
        this.stripeService = new StripePaymentService()
    }
    
    fn processPayment(amount: float) -> bool {
        # Convert dollars to cents
        let amountInCents: int = int(amount * 100.0)
        let txId: string = this.stripeService.charge(amountInCents, "USD")
        return txId != ""
    }
    
    fn refund(transactionId: string, amount: float) -> bool {
        return this.stripeService.reverseCharge(transactionId)
    }
}

# Another third-party service (Adaptee)
class PayPalPaymentService {
    fn makePayment(dollars: float) -> int {
        println("PayPal: Processing $" + str(dollars))
        return 67890  # PayPal transaction ID
    }
    
    fn refundPayment(paypalTxId: int) {
        println("PayPal: Refunding transaction " + str(paypalTxId))
    }
}

# Adapter for PayPal
class PayPalAdapter extends PaymentProcessor {
    let paypalService: PayPalPaymentService = null
    
    fn init() {
        this.paypalService = new PayPalPaymentService()
    }
    
    fn processPayment(amount: float) -> bool {
        let txId: int = this.paypalService.makePayment(amount)
        return txId > 0
    }
    
    fn refund(transactionId: string, amount: float) -> bool {
        # Convert string to int for PayPal
        let paypalId: int = 67890  # Would parse transactionId
        this.paypalService.refundPayment(paypalId)
        return true
    }
}

# Usage - Same interface for different payment providers
let stripeProcessor: PaymentProcessor = new StripeAdapter()
stripeProcessor.processPayment(99.99)

let paypalProcessor: PaymentProcessor = new PayPalAdapter()
paypalProcessor.processPayment(49.99)

# Can switch providers easily
let processor: PaymentProcessor = new StripeAdapter()
processor.processPayment(29.99)
```

**Advantages**:
- Makes incompatible interfaces work together
- Increases reusability of existing code
- Follows Single Responsibility Principle
- Follows Open/Closed Principle

**Disadvantages**:
- Increases overall complexity
- Sometimes many adaptations required

---

### 5. Decorator Pattern

**Problem**: Add new functionality to objects dynamically without altering their structure.

**When to Use**:
- Add responsibilities to objects dynamically
- Responsibilities can be withdrawn
- Extension by subclassing is impractical

**Structure (ASCII UML)**:
```
┌─────────────┐
│  Component  │
├─────────────┤
│ + operation()│
└─────────────┘
      △
      │
      ├──────────────┬──────────────┐
      │              │              │
┌─────────────┐ ┌──────────────┐   │
│  Concrete   │ │  Decorator   │◄──┘
│  Component  │ ├──────────────┤
└─────────────┘ │- component   │
                │+ operation() │
                └──────────────┘
                      △
                      │
                ┌─────────────┐
                │  Concrete   │
                │  Decorator  │
                └─────────────┘
```

**Implementation: Coffee Shop**:

```klang
# Component
class Coffee {
    fn getCost() -> float {
        return 2.0
    }
    
    fn getDescription() -> string {
        return "Simple coffee"
    }
}

# Concrete Component
class Espresso extends Coffee {
    fn getCost() -> float {
        return 3.0
    }
    
    fn getDescription() -> string {
        return "Espresso"
    }
}

class Latte extends Coffee {
    fn getCost() -> float {
        return 4.0
    }
    
    fn getDescription() -> string {
        return "Latte"
    }
}

# Decorator base
class CoffeeDecorator extends Coffee {
    let decoratedCoffee: Coffee = null
    
    fn init(coffee: Coffee) {
        this.decoratedCoffee = coffee
    }
    
    fn getCost() -> float {
        return this.decoratedCoffee.getCost()
    }
    
    fn getDescription() -> string {
        return this.decoratedCoffee.getDescription()
    }
}

# Concrete Decorators
class MilkDecorator extends CoffeeDecorator {
    fn getCost() -> float {
        return this.decoratedCoffee.getCost() + 0.5
    }
    
    fn getDescription() -> string {
        return this.decoratedCoffee.getDescription() + ", Milk"
    }
}

class SugarDecorator extends CoffeeDecorator {
    fn getCost() -> float {
        return this.decoratedCoffee.getCost() + 0.2
    }
    
    fn getDescription() -> string {
        return this.decoratedCoffee.getDescription() + ", Sugar"
    }
}

class WhippedCreamDecorator extends CoffeeDecorator {
    fn getCost() -> float {
        return this.decoratedCoffee.getCost() + 0.7
    }
    
    fn getDescription() -> string {
        return this.decoratedCoffee.getDescription() + ", Whipped Cream"
    }
}

class CaramelDecorator extends CoffeeDecorator {
    fn getCost() -> float {
        return this.decoratedCoffee.getCost() + 0.6
    }
    
    fn getDescription() -> string {
        return this.decoratedCoffee.getDescription() + ", Caramel"
    }
}

# Usage - Stack decorators
let coffee1: Coffee = new Espresso()
println(coffee1.getDescription() + " costs $" + str(coffee1.getCost()))

let coffee2: Coffee = new MilkDecorator(new Espresso())
println(coffee2.getDescription() + " costs $" + str(coffee2.getCost()))

let coffee3: Coffee = new WhippedCreamDecorator(
    new CaramelDecorator(
        new MilkDecorator(
            new Latte()
        )
    )
)
println(coffee3.getDescription() + " costs $" + str(coffee3.getCost()))

let coffee4: Coffee = new SugarDecorator(
    new MilkDecorator(
        new Espresso()
    )
)
println(coffee4.getDescription() + " costs $" + str(coffee4.getCost()))
```

**Real-World Example: Text Formatting**:

```klang
class Text {
    let content: string = ""
    
    fn init(text: string) {
        this.content = text
    }
    
    fn render() -> string {
        return this.content
    }
}

class TextDecorator extends Text {
    let wrappedText: Text = null
    
    fn init(text: Text) {
        super.init("")
        this.wrappedText = text
    }
    
    fn render() -> string {
        return this.wrappedText.render()
    }
}

class BoldDecorator extends TextDecorator {
    fn render() -> string {
        return "<b>" + this.wrappedText.render() + "</b>"
    }
}

class ItalicDecorator extends TextDecorator {
    fn render() -> string {
        return "<i>" + this.wrappedText.render() + "</i>"
    }
}

class UnderlineDecorator extends TextDecorator {
    fn render() -> string {
        return "<u>" + this.wrappedText.render() + "</u>"
    }
}

class ColorDecorator extends TextDecorator {
    let color: string = "black"
    
    fn init(text: Text, c: string) {
        super.init(text)
        this.color = c
    }
    
    fn render() -> string {
        return "<span color='" + this.color + "'>" + 
               this.wrappedText.render() + "</span>"
    }
}

# Usage
let simpleText: Text = new Text("Hello, World!")
println(simpleText.render())

let boldText: Text = new BoldDecorator(new Text("Bold text"))
println(boldText.render())

let fancyText: Text = new ColorDecorator(
    new UnderlineDecorator(
        new BoldDecorator(
            new Text("Fancy formatted text")
        )
    ),
    "red"
)
println(fancyText.render())
```

**Advantages**:
- More flexible than static inheritance
- Add/remove responsibilities at runtime
- Combine several decorators for complex effects
- Single Responsibility Principle

**Disadvantages**:
- Many small objects can be created
- Hard to remove specific wrapper from stack
- Order of decorators can matter

---

### 6. Facade Pattern

**Problem**: Provide a unified, simplified interface to a complex subsystem.

**When to Use**:
- Want to provide simple interface to complex system
- Many dependencies between clients and implementation classes
- Want to layer your subsystems

**Structure (ASCII UML)**:
```
┌─────────────┐
│   Client    │
└─────────────┘
       │
       │ uses
       ▼
┌─────────────┐
│   Facade    │──────► ┌──────────┐
├─────────────┤        │ Subsystem│
│ + operation()│       │  Class 1 │
└─────────────┘        └──────────┘
       │
       └──────────────► ┌──────────┐
                        │ Subsystem│
                        │  Class 2 │
                        └──────────┘
```

**Implementation: Home Theater System**:

```klang
# Complex subsystem classes
class DVDPlayer {
    fn on() {
        println("DVD Player: ON")
    }
    
    fn off() {
        println("DVD Player: OFF")
    }
    
    fn play(movie: string) {
        println("DVD Player: Playing '" + movie + "'")
    }
    
    fn stop() {
        println("DVD Player: Stopped")
    }
}

class Projector {
    fn on() {
        println("Projector: ON")
    }
    
    fn off() {
        println("Projector: OFF")
    }
    
    fn setInput(source: string) {
        println("Projector: Input set to " + source)
    }
    
    fn wideScreenMode() {
        println("Projector: Widescreen mode (16x9)")
    }
}

class SoundSystem {
    fn on() {
        println("Sound System: ON")
    }
    
    fn off() {
        println("Sound System: OFF")
    }
    
    fn setVolume(level: int) {
        println("Sound System: Volume set to " + str(level))
    }
    
    fn setSurroundSound() {
        println("Sound System: Surround sound ON")
    }
}

class Lights {
    fn dim(level: int) {
        println("Lights: Dimmed to " + str(level) + "%")
    }
    
    fn on() {
        println("Lights: ON (100%)")
    }
}

# Facade - Simplified interface
class HomeTheaterFacade {
    let dvd: DVDPlayer = null
    let projector: Projector = null
    let sound: SoundSystem = null
    let lights: Lights = null
    
    fn init() {
        this.dvd = new DVDPlayer()
        this.projector = new Projector()
        this.sound = new SoundSystem()
        this.lights = new Lights()
    }
    
    fn watchMovie(movie: string) {
        println("\n=== Get ready to watch a movie ===")
        this.lights.dim(10)
        this.projector.on()
        this.projector.wideScreenMode()
        this.projector.setInput("DVD")
        this.sound.on()
        this.sound.setSurroundSound()
        this.sound.setVolume(5)
        this.dvd.on()
        this.dvd.play(movie)
        println("=================================\n")
    }
    
    fn endMovie() {
        println("\n=== Shutting down theater ===")
        this.dvd.stop()
        this.dvd.off()
        this.sound.off()
        this.projector.off()
        this.lights.on()
        println("============================\n")
    }
}

# Usage - Simple interface to complex system
let homeTheater: HomeTheaterFacade = new HomeTheaterFacade()

homeTheater.watchMovie("The Matrix")
# ... movie plays ...
homeTheater.endMovie()

homeTheater.watchMovie("Inception")
# ... movie plays ...
homeTheater.endMovie()
```

**Real-World Example: Computer Startup Facade**:

```klang
# Subsystem components
class CPU {
    fn freeze() {
        println("CPU: Freezing")
    }
    
    fn jump(position: int) {
        println("CPU: Jumping to position " + str(position))
    }
    
    fn execute() {
        println("CPU: Executing instructions")
    }
}

class Memory {
    fn load(position: int, data: string) {
        println("Memory: Loading '" + data + "' at position " + str(position))
    }
}

class HardDrive {
    fn read(sector: int, size: int) -> string {
        println("HardDrive: Reading " + str(size) + " bytes from sector " + str(sector))
        return "BOOT_DATA"
    }
}

# Facade
class ComputerFacade {
    let cpu: CPU = null
    let memory: Memory = null
    let hardDrive: HardDrive = null
    
    fn init() {
        this.cpu = new CPU()
        this.memory = new Memory()
        this.hardDrive = new HardDrive()
    }
    
    fn start() {
        println("\n=== Starting Computer ===")
        this.cpu.freeze()
        let bootData: string = this.hardDrive.read(0, 1024)
        this.memory.load(0, bootData)
        this.cpu.jump(0)
        this.cpu.execute()
        println("=== Computer Started ===\n")
    }
}

# Usage
let computer: ComputerFacade = new ComputerFacade()
computer.start()  # Simple one-line computer startup!
```

**Advantages**:
- Simplifies complex subsystems
- Reduces dependencies between client and subsystem
- Promotes loose coupling
- Makes subsystem easier to use

**Disadvantages**:
- Facade can become a "god object" coupled to all subsystem classes
- May not provide all functionality of subsystem

---

## Behavioral Patterns

Behavioral patterns deal with object collaboration and the delegation of responsibilities between objects.

### 7. Strategy Pattern

**Problem**: Define a family of algorithms, encapsulate each one, and make them interchangeable.

**When to Use**:
- Many related classes differ only in behavior
- Need different variants of an algorithm
- Algorithm uses data clients shouldn't know about
- Class defines many behaviors as multiple conditional statements

**Structure (ASCII UML)**:
```
┌─────────────┐         ┌──────────────┐
│   Context   │────────►│   Strategy   │
├─────────────┤         ├──────────────┤
│- strategy   │         │ + algorithm()│
│+ execute()  │         └──────────────┘
└─────────────┘               △
                              │
                    ┌─────────┴─────────┐
                    │                   │
            ┌───────────────┐   ┌───────────────┐
            │ConcreteStrategy│   │ConcreteStrategy│
            │       A        │   │       B        │
            └───────────────┘   └───────────────┘
```

**Implementation: Payment Strategy**:

```klang
# Strategy interface
class PaymentStrategy {
    fn pay(amount: float) {
        println("Processing payment of $" + str(amount))
    }
}

# Concrete Strategies
class CreditCardStrategy extends PaymentStrategy {
    let cardNumber: string = ""
    let cvv: string = ""
    
    fn init(number: string, cvv: string) {
        this.cardNumber = number
        this.cvv = cvv
    }
    
    fn pay(amount: float) {
        println("Paying $" + str(amount) + " with Credit Card ending in " + 
                this.cardNumber[12:16])
        println("Processing credit card payment...")
    }
}

class PayPalStrategy extends PaymentStrategy {
    let email: string = ""
    
    fn init(email: string) {
        this.email = email
    }
    
    fn pay(amount: float) {
        println("Paying $" + str(amount) + " with PayPal account " + this.email)
        println("Redirecting to PayPal...")
    }
}

class BitcoinStrategy extends PaymentStrategy {
    let walletAddress: string = ""
    
    fn init(address: string) {
        this.walletAddress = address
    }
    
    fn pay(amount: float) {
        println("Paying $" + str(amount) + " with Bitcoin")
        println("Sending to wallet: " + this.walletAddress)
        println("Waiting for blockchain confirmation...")
    }
}

# Context
class ShoppingCart {
    let items: array<string> = []
    let total: float = 0.0
    let paymentStrategy: PaymentStrategy = null
    
    fn addItem(item: string, price: float) {
        # In real implementation, would append
        this.total = this.total + price
        println("Added: " + item + " ($" + str(price) + ")")
    }
    
    fn setPaymentStrategy(strategy: PaymentStrategy) {
        this.paymentStrategy = strategy
    }
    
    fn checkout() {
        if this.paymentStrategy == null {
            println("Please select a payment method!")
            return
        }
        
        println("\n=== Checkout ===")
        println("Total: $" + str(this.total))
        this.paymentStrategy.pay(this.total)
        println("================\n")
    }
}

# Usage
let cart: ShoppingCart = new ShoppingCart()
cart.addItem("Laptop", 999.99)
cart.addItem("Mouse", 29.99)
cart.addItem("Keyboard", 79.99)

# Pay with credit card
cart.setPaymentStrategy(new CreditCardStrategy("1234567890123456", "123"))
cart.checkout()

# Create another cart
let cart2: ShoppingCart = new ShoppingCart()
cart2.addItem("Book", 19.99)
cart2.addItem("Pen", 2.99)

# Pay with PayPal
cart2.setPaymentStrategy(new PayPalStrategy("user@example.com"))
cart2.checkout()

# Create another cart
let cart3: ShoppingCart = new ShoppingCart()
cart3.addItem("Coffee", 4.99)

# Pay with Bitcoin
cart3.setPaymentStrategy(new BitcoinStrategy("1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa"))
cart3.checkout()
```

**Real-World Example: Sorting Strategy**:

```klang
class SortStrategy {
    fn sort(data: array<int>) {
        println("Sorting data...")
    }
}

class BubbleSortStrategy extends SortStrategy {
    fn sort(data: array<int>) {
        println("Sorting using Bubble Sort")
        println("Good for small datasets, O(n²) complexity")
        # Implementation would go here
    }
}

class QuickSortStrategy extends SortStrategy {
    fn sort(data: array<int>) {
        println("Sorting using Quick Sort")
        println("Good for large datasets, O(n log n) average")
        # Implementation would go here
    }
}

class MergeSortStrategy extends SortStrategy {
    fn sort(data: array<int>) {
        println("Sorting using Merge Sort")
        println("Good for linked lists, O(n log n) guaranteed")
        # Implementation would go here
    }
}

class DataProcessor {
    let strategy: SortStrategy = null
    let data: array<int> = []
    
    fn setStrategy(strategy: SortStrategy) {
        this.strategy = strategy
    }
    
    fn setData(data: array<int>) {
        this.data = data
    }
    
    fn process() {
        println("\nProcessing dataset...")
        if this.strategy != null {
            this.strategy.sort(this.data)
        }
    }
}

# Usage
let processor: DataProcessor = new DataProcessor()
let smallData: array<int> = [5, 2, 8, 1, 9]
let largeData: array<int> = [/* imagine 1000 elements */]

# Small dataset - use Bubble Sort
processor.setData(smallData)
processor.setStrategy(new BubbleSortStrategy())
processor.process()

# Large dataset - use Quick Sort
processor.setData(largeData)
processor.setStrategy(new QuickSortStrategy())
processor.process()
```

**Advantages**:
- Family of algorithms can be swapped at runtime
- Eliminates conditional statements
- Isolates algorithm implementation details
- Open/Closed Principle

**Disadvantages**:
- Clients must be aware of different strategies
- Increases number of objects
- Communication overhead between strategy and context

---

### 8. Observer Pattern

**Problem**: Define a one-to-many dependency so when one object changes state, all dependents are notified.

**When to Use**:
- Change to one object requires changing others
- Object should notify others without knowing who they are
- Implementing event handling systems

**Structure (ASCII UML)**:
```
┌──────────────┐         ┌──────────────┐
│   Subject    │◄────────│   Observer   │
├──────────────┤         ├──────────────┤
│+ attach()    │         │+ update()    │
│+ detach()    │         └──────────────┘
│+ notify()    │               △
└──────────────┘               │
      △                        │
      │                  ┌─────────────┐
      │                  │  Concrete   │
┌─────────────┐          │  Observer   │
│  Concrete   │          └─────────────┘
│  Subject    │
└─────────────┘
```

**Implementation: Newsletter Subscription**:

```klang
# Observer interface
class Observer {
    fn update(message: string) {
        println("Received: " + message)
    }
}

# Concrete Observers
class EmailSubscriber extends Observer {
    let email: string = ""
    
    fn init(email: string) {
        this.email = email
    }
    
    fn update(message: string) {
        println("Sending email to " + this.email + ": " + message)
    }
}

class SMSSubscriber extends Observer {
    let phoneNumber: string = ""
    
    fn init(phone: string) {
        this.phoneNumber = phone
    }
    
    fn update(message: string) {
        println("Sending SMS to " + this.phoneNumber + ": " + message)
    }
}

class PushNotificationSubscriber extends Observer {
    let deviceId: string = ""
    
    fn init(device: string) {
        this.deviceId = device
    }
    
    fn update(message: string) {
        println("Sending push notification to device " + this.deviceId + ": " + message)
    }
}

# Subject
class Newsletter {
    let observers: array<Observer> = []
    let latestNews: string = ""
    
    fn subscribe(observer: Observer) {
        # In real implementation, would append to array
        println("New subscriber added")
    }
    
    fn unsubscribe(observer: Observer) {
        println("Subscriber removed")
    }
    
    fn notify() {
        println("\n=== Broadcasting to all subscribers ===")
        let i: int = 0
        while i < len(this.observers) {
            this.observers[i].update(this.latestNews)
            i = i + 1
        }
        println("=======================================\n")
    }
    
    fn publishNews(news: string) {
        println("\nPublishing news: " + news)
        this.latestNews = news
        this.notify()
    }
}

# Usage
let newsletter: Newsletter = new Newsletter()

let emailSub1: Observer = new EmailSubscriber("alice@example.com")
let emailSub2: Observer = new EmailSubscriber("bob@example.com")
let smsSub: Observer = new SMSSubscriber("+1-555-0123")
let pushSub: Observer = new PushNotificationSubscriber("device_abc123")

newsletter.subscribe(emailSub1)
newsletter.subscribe(emailSub2)
newsletter.subscribe(smsSub)
newsletter.subscribe(pushSub)

newsletter.publishNews("New product launch next week!")
newsletter.publishNews("50% off sale this weekend!")
```

**Real-World Example: Stock Market Ticker**:

```klang
class StockObserver {
    fn update(stock: string, price: float) {
        println("Stock update: " + stock + " = $" + str(price))
    }
}

class InvestorObserver extends StockObserver {
    let name: string = ""
    let threshold: float = 0.0
    
    fn init(name: string, buyThreshold: float) {
        this.name = name
        this.threshold = buyThreshold
    }
    
    fn update(stock: string, price: float) {
        println("\n[" + this.name + "] " + stock + " is now $" + str(price))
        if price < this.threshold {
            println("[" + this.name + "] BUY SIGNAL! Price below $" + str(this.threshold))
        }
    }
}

class DisplayObserver extends StockObserver {
    let displayName: string = ""
    
    fn init(name: string) {
        this.displayName = name
    }
    
    fn update(stock: string, price: float) {
        println("[" + this.displayName + " Display] " + stock + ": $" + str(price))
    }
}

class Stock {
    let symbol: string = ""
    let price: float = 0.0
    let observers: array<StockObserver> = []
    
    fn init(symbol: string, initialPrice: float) {
        this.symbol = symbol
        this.price = initialPrice
    }
    
    fn attach(observer: StockObserver) {
        println("Observer attached to " + this.symbol)
    }
    
    fn detach(observer: StockObserver) {
        println("Observer detached from " + this.symbol)
    }
    
    fn notify() {
        let i: int = 0
        while i < len(this.observers) {
            this.observers[i].update(this.symbol, this.price)
            i = i + 1
        }
    }
    
    fn setPrice(newPrice: float) {
        println("\n=== " + this.symbol + " price changed ===")
        this.price = newPrice
        this.notify()
    }
}

# Usage
let apple: Stock = new Stock("AAPL", 150.0)

let investor1: StockObserver = new InvestorObserver("Warren", 145.0)
let investor2: StockObserver = new InvestorObserver("Elon", 140.0)
let display: StockObserver = new DisplayObserver("Trading Floor")

apple.attach(investor1)
apple.attach(investor2)
apple.attach(display)

apple.setPrice(148.0)
apple.setPrice(143.0)
apple.setPrice(138.0)
```

**Advantages**:
- Loose coupling between subject and observers
- Dynamic relationships (add/remove at runtime)
- Broadcast communication
- Supports Open/Closed Principle

**Disadvantages**:
- Observers notified in random order
- Can cause memory leaks if not properly unsubscribed
- Unexpected updates can cascade

---

### 9. Template Method Pattern

**Problem**: Define the skeleton of an algorithm, deferring some steps to subclasses.

**When to Use**:
- Implement invariant parts of algorithm once, leave varying parts to subclasses
- Common behavior among subclasses should be factored and localized
- Control subclass extensions

**Structure (ASCII UML)**:
```
┌──────────────────┐
│  AbstractClass   │
├──────────────────┤
│+ templateMethod()│ ◄─── Defines algorithm skeleton
│# step1()         │ ◄─── Abstract/Hook methods
│# step2()         │
└──────────────────┘
         △
         │
         │ extends
         │
┌──────────────────┐
│  ConcreteClass   │
├──────────────────┤
│# step1()         │ ◄─── Implements specific steps
│# step2()         │
└──────────────────┘
```

**Implementation: Data Mining**:

```klang
# Abstract class with template method
class DataMiner {
    # Template method - defines algorithm skeleton
    fn mine(path: string) {
        let file: string = this.openFile(path)
        let rawData: string = this.extractData(file)
        let data: string = this.parseData(rawData)
        let analysis: string = this.analyzeData(data)
        this.sendReport(analysis)
        this.closeFile(file)
    }
    
    # Steps to be implemented by subclasses
    fn openFile(path: string) -> string {
        println("Opening file: " + path)
        return "file_handle"
    }
    
    fn extractData(file: string) -> string {
        # Override in subclass
        return "raw_data"
    }
    
    fn parseData(rawData: string) -> string {
        # Override in subclass
        return "parsed_data"
    }
    
    fn analyzeData(data: string) -> string {
        println("Analyzing data...")
        return "analysis_result"
    }
    
    fn sendReport(analysis: string) {
        println("Sending report: " + analysis)
    }
    
    fn closeFile(file: string) {
        println("Closing file: " + file)
    }
}

# Concrete implementation for PDF
class PDFDataMiner extends DataMiner {
    fn extractData(file: string) -> string {
        println("Extracting data from PDF file")
        return "pdf_raw_data"
    }
    
    fn parseData(rawData: string) -> string {
        println("Parsing PDF data format")
        return "pdf_parsed_data"
    }
}

# Concrete implementation for CSV
class CSVDataMiner extends DataMiner {
    fn extractData(file: string) -> string {
        println("Extracting data from CSV file")
        return "csv_raw_data"
    }
    
    fn parseData(rawData: string) -> string {
        println("Parsing CSV data format")
        return "csv_parsed_data"
    }
}

# Concrete implementation for XML
class XMLDataMiner extends DataMiner {
    fn extractData(file: string) -> string {
        println("Extracting data from XML file")
        return "xml_raw_data"
    }
    
    fn parseData(rawData: string) -> string {
        println("Parsing XML data format")
        return "xml_parsed_data"
    }
}

# Usage
println("=== Mining PDF ===")
let pdfMiner: DataMiner = new PDFDataMiner()
pdfMiner.mine("data.pdf")

println("\n=== Mining CSV ===")
let csvMiner: DataMiner = new CSVDataMiner()
csvMiner.mine("data.csv")

println("\n=== Mining XML ===")
let xmlMiner: DataMiner = new XMLDataMiner()
xmlMiner.mine("data.xml")
```

**Real-World Example: Beverage Making**:

```klang
# Abstract beverage class
class Beverage {
    # Template method - same for all beverages
    fn prepare() {
        this.boilWater()
        this.brew()
        this.pourInCup()
        this.addCondiments()
        println("Beverage is ready!\n")
    }
    
    fn boilWater() {
        println("Boiling water...")
    }
    
    # Abstract methods - implemented by subclasses
    fn brew() {
        println("Brewing...")
    }
    
    fn pourInCup() {
        println("Pouring into cup...")
    }
    
    fn addCondiments() {
        println("Adding condiments...")
    }
}

class Tea extends Beverage {
    fn brew() {
        println("Steeping the tea bag")
    }
    
    fn addCondiments() {
        println("Adding lemon")
    }
}

class Coffee extends Beverage {
    fn brew() {
        println("Dripping coffee through filter")
    }
    
    fn addCondiments() {
        println("Adding sugar and milk")
    }
}

class HotChocolate extends Beverage {
    fn brew() {
        println("Mixing hot chocolate powder")
    }
    
    fn addCondiments() {
        println("Adding marshmallows and whipped cream")
    }
}

# Usage
println("=== Making Tea ===")
let tea: Beverage = new Tea()
tea.prepare()

println("=== Making Coffee ===")
let coffee: Beverage = new Coffee()
coffee.prepare()

println("=== Making Hot Chocolate ===")
let hotChocolate: Beverage = new HotChocolate()
hotChocolate.prepare()
```

**Advantages**:
- Code reuse (common algorithm in one place)
- Controlled extensibility
- Inversion of control ("Hollywood Principle")
- Enforces algorithm structure

**Disadvantages**:
- Clients must inherit to implement variations
- Can make code harder to understand
- Violates Liskov Substitution if not careful

---

## Practical Examples

### Example 1: Logger System (Singleton + Strategy)

```klang
# Strategy for log output
class LogOutput {
    fn write(message: string) {
        println(message)
    }
}

class ConsoleOutput extends LogOutput {
    fn write(message: string) {
        println("[CONSOLE] " + message)
    }
}

class FileOutput extends LogOutput {
    let filename: string = ""
    
    fn init(file: string) {
        this.filename = file
    }
    
    fn write(message: string) {
        println("[FILE:" + this.filename + "] " + message)
    }
}

# Singleton Logger with Strategy
class ApplicationLogger {
    static let instance: ApplicationLogger = null
    
    let output: LogOutput = null
    let logLevel: string = "INFO"
    
    fn init() {
        this.output = new ConsoleOutput()
    }
    
    static fn getInstance() -> ApplicationLogger {
        if ApplicationLogger.instance == null {
            ApplicationLogger.instance = new ApplicationLogger()
        }
        return ApplicationLogger.instance
    }
    
    fn setOutput(output: LogOutput) {
        this.output = output
    }
    
    fn setLevel(level: string) {
        this.logLevel = level
    }
    
    fn info(message: string) {
        this.output.write("[INFO] " + message)
    }
    
    fn warning(message: string) {
        this.output.write("[WARNING] " + message)
    }
    
    fn error(message: string) {
        this.output.write("[ERROR] " + message)
    }
}

# Usage
let logger: ApplicationLogger = ApplicationLogger.getInstance()
logger.info("Application started")

# Switch to file output
logger.setOutput(new FileOutput("app.log"))
logger.warning("Low memory")

# Same instance everywhere
let logger2: ApplicationLogger = ApplicationLogger.getInstance()
logger2.error("Connection failed")
```

### Example 2: Plugin System (Factory + Observer)

```klang
# Observer for plugin events
class PluginObserver {
    fn onPluginLoaded(pluginName: string) {
        println("Plugin loaded: " + pluginName)
    }
}

# Base plugin
class Plugin {
    let name: string = ""
    let version: string = "1.0"
    
    fn init(n: string) {
        this.name = n
    }
    
    fn execute() {
        println("Executing plugin: " + this.name)
    }
}

class ImagePlugin extends Plugin {
    fn execute() {
        println("Processing image with " + this.name + " plugin")
    }
}

class AudioPlugin extends Plugin {
    fn execute() {
        println("Processing audio with " + this.name + " plugin")
    }
}

# Factory for plugins
class PluginFactory {
    static fn createPlugin(type: string, name: string) -> Plugin {
        if type == "image" {
            return new ImagePlugin(name)
        } else if type == "audio" {
            return new AudioPlugin(name)
        }
        return new Plugin(name)
    }
}

# Plugin manager with observer
class PluginManager {
    let plugins: array<Plugin> = []
    let observers: array<PluginObserver> = []
    
    fn registerObserver(observer: PluginObserver) {
        # Add observer
        println("Observer registered")
    }
    
    fn loadPlugin(type: string, name: string) {
        let plugin: Plugin = PluginFactory.createPlugin(type, name)
        # Add to plugins array
        
        # Notify observers
        let i: int = 0
        while i < len(this.observers) {
            this.observers[i].onPluginLoaded(name)
            i = i + 1
        }
    }
}

# Usage
let manager: PluginManager = new PluginManager()
let observer: PluginObserver = new PluginObserver()
manager.registerObserver(observer)

manager.loadPlugin("image", "PhotoFilter")
manager.loadPlugin("audio", "EqualizerPro")
```

---

## Anti-Patterns

### 1. God Object Anti-Pattern

**Problem**: One class does too much, knows too much, or controls too much.

```klang
# BAD: God Object
class Application {
    # Database operations
    fn connectToDatabase() { }
    fn executeQuery(query: string) { }
    
    # UI rendering
    fn renderScreen() { }
    fn handleInput() { }
    
    # Business logic
    fn calculateTotal() { }
    fn validateData() { }
    
    # File operations
    fn saveFile() { }
    fn loadFile() { }
    
    # Network operations
    fn sendRequest() { }
    fn receiveResponse() { }
}

# GOOD: Separated responsibilities with patterns
class Database {
    fn connect() { }
    fn query(sql: string) { }
}

class UI {
    fn render() { }
    fn handleInput() { }
}

class BusinessLogic {
    fn calculate() { }
    fn validate() { }
}
```

### 2. Spaghetti Code

**Problem**: Complex and tangled control structure, hard to understand or modify.

```klang
# BAD: Spaghetti code
fn processPayment(type: string, amount: float, discount: bool, premium: bool) {
    if type == "credit" {
        if discount {
            if premium {
                # apply premium discount to credit
            } else {
                # apply normal discount to credit
            }
        } else {
            if premium {
                # no discount but premium credit
            } else {
                # normal credit
            }
        }
    } else if type == "paypal" {
        # repeat similar nested logic
    }
    # More nested conditions...
}

# GOOD: Use Strategy pattern
class PaymentProcessor {
    let strategy: PaymentStrategy = null
    
    fn process(amount: float) {
        this.strategy.pay(amount)
    }
}
```

### 3. Singleton Abuse

**Problem**: Using Singleton for everything, creating global state.

```klang
# BAD: Too many singletons
class ConfigSingleton { }
class DatabaseSingleton { }
class LoggerSingleton { }
class CacheSingleton { }
class SessionSingleton { }
# Everything is a singleton!

# GOOD: Use Singleton only when truly needed
# Use dependency injection for most cases
class Application {
    let config: Config = null
    let database: Database = null
    let logger: Logger = null  # Could be singleton
    
    fn init(config: Config, database: Database, logger: Logger) {
        this.config = config
        this.database = database
        this.logger = logger
    }
}
```

### 4. Premature Pattern Application

**Problem**: Applying patterns before they're needed.

```klang
# BAD: Over-engineering simple code
class AdditionStrategyFactory {
    fn createStrategy() -> MathStrategy {
        return new AdditionStrategy()
    }
}

class AdditionStrategy extends MathStrategy {
    fn execute(a: int, b: int) -> int {
        return a + b
    }
}

# GOOD: Keep it simple when appropriate
fn add(a: int, b: int) -> int {
    return a + b
}
```

---

## Best Practices

### 1. Know When to Use Patterns

**Use patterns when:**
- Problem matches pattern's intent
- Benefits outweigh added complexity
- Team understands the pattern
- Future changes are anticipated

**Don't use patterns when:**
- Simple solution works fine
- Pattern adds unnecessary complexity
- Team unfamiliar with pattern
- YAGNI (You Aren't Gonna Need It)

### 2. Combine Patterns Appropriately

```klang
# Good combination: Singleton + Factory
class ServiceFactory {
    static let instance: ServiceFactory = null
    
    static fn getInstance() -> ServiceFactory {
        if ServiceFactory.instance == null {
            ServiceFactory.instance = new ServiceFactory()
        }
        return ServiceFactory.instance
    }
    
    fn createService(type: string) -> Service {
        if type == "email" {
            return new EmailService()
        } else if type == "sms" {
            return new SMSService()
        }
        return new Service()
    }
}
```

### 3. Favor Composition Over Inheritance

```klang
# Prefer composition (Decorator, Strategy) over deep inheritance
class TextEditor {
    let formatter: TextFormatter = null
    let spellChecker: SpellChecker = null
    
    fn setFormatter(formatter: TextFormatter) {
        this.formatter = formatter
    }
    
    fn setSpellChecker(checker: SpellChecker) {
        this.spellChecker = checker
    }
}
```

### 4. Keep It Simple

**Start simple, refactor to patterns when needed:**

1. Write working code first
2. Identify recurring problems
3. Refactor to appropriate pattern
4. Test thoroughly

### 5. Document Pattern Usage

```klang
# Document which pattern you're using and why
# Singleton Pattern: Ensures single database connection
class DatabaseConnection {
    static let instance: DatabaseConnection = null
    # ...
}
```

---

## Interview Problems

### Problem 1: Design a Notification System

**Question**: Design a notification system that can send notifications via email, SMS, and push notifications. New notification types should be easy to add.

**Solution**: Use Strategy or Factory pattern

```klang
class NotificationStrategy {
    fn send(message: string, recipient: string) {
        println("Sending: " + message)
    }
}

class EmailNotification extends NotificationStrategy {
    fn send(message: string, recipient: string) {
        println("Email to " + recipient + ": " + message)
    }
}

class SMSNotification extends NotificationStrategy {
    fn send(message: string, recipient: string) {
        println("SMS to " + recipient + ": " + message)
    }
}

class NotificationService {
    let strategy: NotificationStrategy = null
    
    fn setStrategy(strategy: NotificationStrategy) {
        this.strategy = strategy
    }
    
    fn notify(message: string, recipient: string) {
        this.strategy.send(message, recipient)
    }
}
```

### Problem 2: Implement a Cache System

**Question**: Implement a cache that can only have one instance across the application.

**Solution**: Singleton pattern

```klang
class Cache {
    static let instance: Cache = null
    
    let data: array<string> = []
    
    fn init() {
        this.data = []
    }
    
    static fn getInstance() -> Cache {
        if Cache.instance == null {
            Cache.instance = new Cache()
        }
        return Cache.instance
    }
    
    fn put(key: string, value: string) {
        println("Caching: " + key + " = " + value)
    }
    
    fn get(key: string) -> string {
        println("Retrieving: " + key)
        return "value"
    }
}
```

### Problem 3: Weather Monitoring System

**Question**: Design a weather station that notifies multiple displays when temperature changes.

**Solution**: Observer pattern

```klang
class Display {
    fn update(temp: float) {
        println("Temperature: " + str(temp))
    }
}

class WeatherStation {
    let temperature: float = 0.0
    let displays: array<Display> = []
    
    fn attach(display: Display) {
        # Add to array
    }
    
    fn setTemperature(temp: float) {
        this.temperature = temp
        this.notifyDisplays()
    }
    
    fn notifyDisplays() {
        let i: int = 0
        while i < len(this.displays) {
            this.displays[i].update(this.temperature)
            i = i + 1
        }
    }
}
```

### Problem 4: Document Converter

**Question**: Create a system to convert documents (Word, PDF) to different formats (HTML, Text).

**Solution**: Factory or Builder pattern

```klang
class DocumentConverter {
    fn convert(doc: string, format: string) -> string {
        return "converted"
    }
}

class ConverterFactory {
    static fn getConverter(sourceType: string, targetType: string) -> DocumentConverter {
        if sourceType == "word" && targetType == "html" {
            return new WordToHTMLConverter()
        } else if sourceType == "pdf" && targetType == "text" {
            return new PDFToTextConverter()
        }
        return new DocumentConverter()
    }
}
```

### Problem 5: Coffee Machine

**Question**: Design a coffee machine that can make different types of coffee with various add-ons (milk, sugar, whipped cream).

**Solution**: Decorator pattern

```klang
class Coffee {
    fn cost() -> float { return 2.0 }
    fn description() -> string { return "Coffee" }
}

class MilkDecorator extends Coffee {
    let coffee: Coffee = null
    
    fn init(c: Coffee) {
        this.coffee = c
    }
    
    fn cost() -> float {
        return this.coffee.cost() + 0.5
    }
    
    fn description() -> string {
        return this.coffee.description() + ", Milk"
    }
}

# Usage: new MilkDecorator(new Coffee())
```

### Problem 6: Shape Drawing System

**Question**: Create a system to draw different shapes without knowing their exact types at compile time.

**Solution**: Factory pattern

```klang
class Shape {
    fn draw() { println("Drawing shape") }
}

class Circle extends Shape {
    fn draw() { println("Drawing circle") }
}

class ShapeFactory {
    static fn createShape(type: string) -> Shape {
        if type == "circle" { return new Circle() }
        return new Shape()
    }
}
```

### Problem 7: Game Character Creation

**Question**: Design a system to create game characters with many optional attributes (weapons, armor, skills, inventory).

**Solution**: Builder pattern

```klang
class Character {
    let name: string = ""
    let health: int = 100
    let weapon: string = "none"
    let armor: string = "none"
}

class CharacterBuilder {
    let character: Character = null
    
    fn init() {
        this.character = new Character()
    }
    
    fn setName(name: string) -> CharacterBuilder {
        this.character.name = name
        return this
    }
    
    fn setWeapon(weapon: string) -> CharacterBuilder {
        this.character.weapon = weapon
        return this
    }
    
    fn build() -> Character {
        return this.character
    }
}

# Usage
let hero: Character = new CharacterBuilder()
    .setName("Warrior")
    .setWeapon("Sword")
    .build()
```

### Problem 8: Stock Trading Alert System

**Question**: Design a system where multiple traders want to be notified when a stock price reaches certain thresholds.

**Solution**: Observer pattern

```klang
class Trader {
    let name: string = ""
    let threshold: float = 0.0
    
    fn init(name: string, threshold: float) {
        this.name = name
        this.threshold = threshold
    }
    
    fn update(price: float) {
        if price <= this.threshold {
            println(this.name + ": BUY at $" + str(price))
        }
    }
}

class StockTicker {
    let traders: array<Trader> = []
    let currentPrice: float = 0.0
    
    fn subscribe(trader: Trader) {
        # Add trader
    }
    
    fn updatePrice(price: float) {
        this.currentPrice = price
        let i: int = 0
        while i < len(this.traders) {
            this.traders[i].update(price)
            i = i + 1
        }
    }
}
```

---

## Summary

Design patterns are powerful tools in object-oriented programming:

- **Creational Patterns** (Singleton, Factory, Builder): Control object creation
- **Structural Patterns** (Adapter, Decorator, Facade): Organize object relationships
- **Behavioral Patterns** (Strategy, Observer, Template Method): Define object interactions

### Key Takeaways

1. **Use patterns judiciously**: Only when they solve real problems
2. **Understand the intent**: Know why, not just how
3. **Keep it simple**: Don't over-engineer
4. **Combine wisely**: Patterns work well together
5. **Refactor to patterns**: Start simple, add patterns as needed

### Further Learning

- Study all 23 GoF patterns
- Practice implementing patterns
- Recognize patterns in existing code
- Understand trade-offs of each pattern
- Learn when NOT to use patterns

Design patterns are a journey, not a destination. Master them through practice and experience!
