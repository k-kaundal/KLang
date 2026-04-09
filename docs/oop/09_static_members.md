# Static Members in KLang

## Table of Contents
1. [Introduction](#introduction)
2. [What are Static Members?](#what-are-static-members)
3. [Static Properties](#static-properties)
4. [Static Methods](#static-methods)
5. [Memory Management](#memory-management)
6. [Accessing Static Members](#accessing-static-members)
7. [Practical Examples](#practical-examples)
8. [Static vs Instance Members](#static-vs-instance-members)
9. [Static Initialization](#static-initialization)
10. [Common Use Cases](#common-use-cases)
11. [Best Practices](#best-practices)
12. [Interview Problems](#interview-problems)

---

## Introduction

**Static members** are class-level properties and methods that belong to the class itself rather than to any specific instance. They are shared across all instances of the class and can be accessed without creating an object.

### Why Static Members?

- **Shared State**: Single copy shared by all instances
- **Utility Functions**: Methods that don't require instance data
- **Constants**: Class-level constants and configuration
- **Memory Efficiency**: Only one copy in memory
- **Factory Methods**: Create instances with different configurations
- **Instance Tracking**: Count or manage instances globally

### Key Concept

```
Instance Members              Static Members
     ↓                              ↓
Per-object data              Class-level data
Each instance has copy       Single shared copy
Accessed via object          Accessed via class name
```

---

## What are Static Members?

Static members exist at the **class level**, not at the instance level. This means:

1. They are **shared** across all instances
2. They can be accessed **without creating an object**
3. They **cannot access** instance (non-static) members directly
4. They exist **as long as the program runs**

### Class-Level vs Instance-Level

```
┌─────────────────────────────────────┐
│         MyClass (Class Level)       │
│                                     │
│  static count = 0                   │ ← One copy for entire class
│  static fn utility() { }            │
└─────────────────────────────────────┘
            ↓                ↓
    ┌──────────────┐  ┌──────────────┐
    │  Instance 1  │  │  Instance 2  │
    │              │  │              │
    │  name = "A"  │  │  name = "B"  │ ← Each has own copy
    └──────────────┘  └──────────────┘
```

### Visual Example

```klang
class Counter {
    # Static member - shared by all instances
    static count = 0
    
    # Instance member - unique to each instance
    id = 0
    
    fn constructor() {
        Counter.count = Counter.count + 1
        this.id = Counter.count
    }
}

let c1 = new Counter()
println(c1.id)           # Output: 1
println(Counter.count)   # Output: 1

let c2 = new Counter()
println(c2.id)           # Output: 2
println(Counter.count)   # Output: 2 (shared across instances)
```

---

## Static Properties

Static properties are **class-level variables** that are shared across all instances of the class.

### Declaring Static Properties

```klang
class Database {
    # Static property - shared configuration
    static maxConnections = 100
    static activeConnections = 0
    static serverUrl = "localhost:5432"
    
    # Instance property - unique per connection
    connectionId = 0
}
```

### Accessing Static Properties

```klang
# Access via class name
println(Database.maxConnections)      # Output: 100
Database.activeConnections = 5
println(Database.activeConnections)   # Output: 5

# Can also access via instance (but not recommended)
let db = new Database()
println(db.maxConnections)            # Works, but use Database.maxConnections instead
```

### Shared State Example

```klang
class BankAccount {
    static totalAccounts = 0
    static totalBalance = 0.0
    
    accountNumber = 0
    balance = 0.0
    
    fn constructor(initialBalance) {
        BankAccount.totalAccounts = BankAccount.totalAccounts + 1
        this.accountNumber = BankAccount.totalAccounts
        this.balance = initialBalance
        BankAccount.totalBalance = BankAccount.totalBalance + initialBalance
    }
    
    fn deposit(amount) {
        this.balance = this.balance + amount
        BankAccount.totalBalance = BankAccount.totalBalance + amount
    }
    
    fn withdraw(amount) {
        if (this.balance >= amount) {
            this.balance = this.balance - amount
            BankAccount.totalBalance = BankAccount.totalBalance - amount
            return true
        }
        return false
    }
}

# Create accounts
let acc1 = new BankAccount(1000.0)
let acc2 = new BankAccount(500.0)
let acc3 = new BankAccount(2000.0)

println("Total Accounts: " + BankAccount.totalAccounts)    # Output: 3
println("Total Balance: " + BankAccount.totalBalance)      # Output: 3500.0

acc1.deposit(500.0)
println("Total Balance: " + BankAccount.totalBalance)      # Output: 4000.0
```

---

## Static Methods

Static methods are **class-level functions** that can be called without creating an instance. They can only access static members directly.

### Declaring Static Methods

```klang
class MathUtils {
    static PI = 3.14159
    
    # Static method - utility function
    static fn abs(x) {
        if (x < 0) {
            return -x
        }
        return x
    }
    
    static fn max(a, b) {
        if (a > b) {
            return a
        }
        return b
    }
    
    static fn min(a, b) {
        if (a < b) {
            return a
        }
        return b
    }
    
    static fn circleArea(radius) {
        return MathUtils.PI * radius * radius
    }
}
```

### Calling Static Methods

```klang
# Call without creating instance
let result1 = MathUtils.abs(-42)
println(result1)  # Output: 42

let result2 = MathUtils.max(10, 25)
println(result2)  # Output: 25

let area = MathUtils.circleArea(5.0)
println(area)     # Output: 78.53975
```

### Static Method Restrictions

```klang
class Example {
    static staticVar = 10
    instanceVar = 20
    
    # Static method can access static members
    static fn staticMethod() {
        println(Example.staticVar)      # ✓ OK
        # println(this.instanceVar)     # ✗ ERROR: Cannot access instance member
        # println(instanceVar)           # ✗ ERROR: Cannot access instance member
    }
    
    # Instance method can access both
    fn instanceMethod() {
        println(Example.staticVar)      # ✓ OK
        println(this.instanceVar)       # ✓ OK
    }
}
```

---

## Memory Management

Static members have a **single copy in memory** regardless of how many instances are created.

### Memory Layout

```
┌────────────────────────────────────────┐
│         CLASS MEMORY AREA              │
│  ┌──────────────────────────────────┐  │
│  │  MyClass.staticVar1 = 100        │  │ ← One copy
│  │  MyClass.staticVar2 = "shared"   │  │ ← One copy
│  │  MyClass.staticMethod()          │  │ ← One copy
│  └──────────────────────────────────┘  │
└────────────────────────────────────────┘
                  ↓
┌────────────────────────────────────────┐
│       INSTANCE MEMORY AREA             │
│  ┌────────────┐  ┌────────────┐        │
│  │ Instance 1 │  │ Instance 2 │        │
│  │ var1 = 10  │  │ var1 = 20  │        │ ← Separate copies
│  │ var2 = "A" │  │ var2 = "B" │        │ ← Separate copies
│  └────────────┘  └────────────┘        │
└────────────────────────────────────────┘
```

### Memory Efficiency Example

```klang
class Logger {
    # Static - only one copy in memory
    static logLevel = "INFO"
    static logCount = 0
    static applicationName = "MyApp"
    
    # Instance - each logger has its own
    moduleName = ""
    
    fn constructor(module) {
        this.moduleName = module
    }
    
    static fn log(message) {
        Logger.logCount = Logger.logCount + 1
        println("[" + Logger.logLevel + "] " + message)
    }
    
    fn logModule(message) {
        Logger.logCount = Logger.logCount + 1
        println("[" + this.moduleName + "] " + message)
    }
}

# Memory: Logger.logLevel, Logger.logCount, Logger.applicationName (3 static variables)
let logger1 = new Logger("Auth")      # Memory: + 1 instance variable (moduleName)
let logger2 = new Logger("Database")  # Memory: + 1 instance variable (moduleName)
let logger3 = new Logger("API")       # Memory: + 1 instance variable (moduleName)

# Total: 3 static variables + 3 instance variables = 6 variables
# If all were instance variables: 3 instances × 6 variables = 18 variables
```

---

## Accessing Static Members

Static members are accessed using the **class name**, not through instances.

### Proper Access Pattern

```klang
class Config {
    static appName = "KLang App"
    static version = "1.0.0"
    static debug = true
    
    static fn getInfo() {
        return Config.appName + " v" + Config.version
    }
}

# ✓ CORRECT: Access via class name
println(Config.appName)
println(Config.version)
let info = Config.getInfo()

# ✗ INCORRECT: Access via instance (works but misleading)
let c = new Config()
println(c.appName)      # Avoid this
```

### From Within the Class

```klang
class Counter {
    static count = 0
    id = 0
    
    fn constructor() {
        # Access static member via class name
        Counter.count = Counter.count + 1
        this.id = Counter.count
    }
    
    static fn resetCount() {
        Counter.count = 0
    }
    
    static fn getCount() {
        return Counter.count
    }
    
    fn getId() {
        return this.id
    }
}
```

### From Child Classes

```klang
class Parent {
    static sharedValue = 100
    
    static fn getShared() {
        return Parent.sharedValue
    }
}

class Child extends Parent {
    fn useParentStatic() {
        # Access parent's static member
        println(Parent.sharedValue)
        println(Parent.getShared())
    }
}

let c = new Child()
c.useParentStatic()  # Output: 100, 100
```

---

## Practical Examples

### Example 1: Instance Counter

```klang
class GameObject {
    static objectCount = 0
    static maxObjects = 1000
    
    id = 0
    name = ""
    
    fn constructor(objectName) {
        if (GameObject.objectCount >= GameObject.maxObjects) {
            println("ERROR: Maximum objects reached!")
            return null
        }
        
        GameObject.objectCount = GameObject.objectCount + 1
        this.id = GameObject.objectCount
        this.name = objectName
        println("Created object #" + this.id + ": " + this.name)
    }
    
    fn destructor() {
        GameObject.objectCount = GameObject.objectCount - 1
        println("Destroyed object #" + this.id + ": " + this.name)
    }
    
    static fn getObjectCount() {
        return GameObject.objectCount
    }
    
    static fn getRemainingSlots() {
        return GameObject.maxObjects - GameObject.objectCount
    }
}

# Usage
let player = new GameObject("Player")
let enemy1 = new GameObject("Enemy1")
let enemy2 = new GameObject("Enemy2")

println("Active objects: " + GameObject.getObjectCount())        # Output: 3
println("Remaining slots: " + GameObject.getRemainingSlots())    # Output: 997
```

### Example 2: Configuration Manager

```klang
class AppConfig {
    # Static configuration shared across application
    static appName = "KLang Application"
    static version = "2.0.1"
    static environment = "production"
    static debugMode = false
    static maxRetries = 3
    static timeout = 30
    
    # Configuration settings
    static databaseUrl = ""
    static apiKey = ""
    
    static fn initialize(env) {
        AppConfig.environment = env
        
        if (env == "development") {
            AppConfig.debugMode = true
            AppConfig.databaseUrl = "localhost:5432"
            AppConfig.timeout = 60
        } else if (env == "production") {
            AppConfig.debugMode = false
            AppConfig.databaseUrl = "prod.server.com:5432"
            AppConfig.timeout = 30
        }
        
        println("Initialized " + AppConfig.appName + " in " + env + " mode")
    }
    
    static fn getFullInfo() {
        return AppConfig.appName + " v" + AppConfig.version + 
               " [" + AppConfig.environment + "]"
    }
    
    static fn isDebug() {
        return AppConfig.debugMode
    }
}

# Usage
AppConfig.initialize("development")
println(AppConfig.getFullInfo())

if (AppConfig.isDebug()) {
    println("Debug mode enabled")
    println("Database: " + AppConfig.databaseUrl)
}
```

### Example 3: String Utility Class

```klang
class StringUtils {
    # No instance variables needed - pure utility class
    
    static fn reverse(str) {
        let result = ""
        let len = str.length()
        let i = len - 1
        
        while (i >= 0) {
            result = result + str.charAt(i)
            i = i - 1
        }
        return result
    }
    
    static fn toUpperCase(str) {
        # Simplified - actual implementation would handle all characters
        return str.toUpper()
    }
    
    static fn toLowerCase(str) {
        return str.toLower()
    }
    
    static fn contains(str, substr) {
        return str.indexOf(substr) >= 0
    }
    
    static fn repeat(str, times) {
        let result = ""
        let i = 0
        while (i < times) {
            result = result + str
            i = i + 1
        }
        return result
    }
    
    static fn trim(str) {
        return str.trim()
    }
    
    static fn capitalize(str) {
        if (str.length() == 0) {
            return str
        }
        let first = str.charAt(0).toUpper()
        let rest = str.substring(1)
        return first + rest
    }
}

# Usage - no need to create instance
println(StringUtils.reverse("hello"))           # Output: olleh
println(StringUtils.repeat("ab", 3))            # Output: ababab
println(StringUtils.capitalize("klang"))        # Output: Klang
println(StringUtils.contains("hello", "ell"))   # Output: true
```

### Example 4: Database Connection Pool

```klang
class DatabasePool {
    # Static pool configuration
    static maxConnections = 10
    static activeConnections = 0
    static availableConnections = 10
    static totalRequests = 0
    
    # Instance connection data
    connectionId = 0
    isActive = false
    
    fn constructor() {
        if (DatabasePool.availableConnections > 0) {
            DatabasePool.activeConnections = DatabasePool.activeConnections + 1
            DatabasePool.availableConnections = DatabasePool.availableConnections - 1
            DatabasePool.totalRequests = DatabasePool.totalRequests + 1
            
            this.connectionId = DatabasePool.totalRequests
            this.isActive = true
            
            println("Connection #" + this.connectionId + " established")
        } else {
            println("ERROR: No available connections")
            this.isActive = false
        }
    }
    
    fn close() {
        if (this.isActive) {
            DatabasePool.activeConnections = DatabasePool.activeConnections - 1
            DatabasePool.availableConnections = DatabasePool.availableConnections + 1
            this.isActive = false
            println("Connection #" + this.connectionId + " closed")
        }
    }
    
    static fn getPoolStatus() {
        println("=== Database Pool Status ===")
        println("Max Connections: " + DatabasePool.maxConnections)
        println("Active: " + DatabasePool.activeConnections)
        println("Available: " + DatabasePool.availableConnections)
        println("Total Requests: " + DatabasePool.totalRequests)
    }
    
    static fn isPoolAvailable() {
        return DatabasePool.availableConnections > 0
    }
}

# Usage
let conn1 = new DatabasePool()
let conn2 = new DatabasePool()
let conn3 = new DatabasePool()

DatabasePool.getPoolStatus()
# Output:
# Max Connections: 10
# Active: 3
# Available: 7
# Total Requests: 3

conn1.close()
DatabasePool.getPoolStatus()
# Output:
# Max Connections: 10
# Active: 2
# Available: 8
# Total Requests: 3
```

### Example 5: Math Utility Class

```klang
class MathHelper {
    static PI = 3.14159265359
    static E = 2.71828182846
    
    static fn abs(x) {
        return x < 0 ? -x : x
    }
    
    static fn max(a, b) {
        return a > b ? a : b
    }
    
    static fn min(a, b) {
        return a < b ? a : b
    }
    
    static fn pow(base, exp) {
        let result = 1
        let i = 0
        while (i < exp) {
            result = result * base
            i = i + 1
        }
        return result
    }
    
    static fn factorial(n) {
        if (n <= 1) {
            return 1
        }
        let result = 1
        let i = 2
        while (i <= n) {
            result = result * i
            i = i + 1
        }
        return result
    }
    
    static fn gcd(a, b) {
        a = MathHelper.abs(a)
        b = MathHelper.abs(b)
        
        while (b != 0) {
            let temp = b
            b = a % b
            a = temp
        }
        return a
    }
    
    static fn isPrime(n) {
        if (n <= 1) {
            return false
        }
        if (n <= 3) {
            return true
        }
        if (n % 2 == 0 || n % 3 == 0) {
            return false
        }
        
        let i = 5
        while (i * i <= n) {
            if (n % i == 0 || n % (i + 2) == 0) {
                return false
            }
            i = i + 6
        }
        return true
    }
    
    static fn circleArea(radius) {
        return MathHelper.PI * radius * radius
    }
    
    static fn circleCircumference(radius) {
        return 2 * MathHelper.PI * radius
    }
}

# Usage
println(MathHelper.abs(-42))           # Output: 42
println(MathHelper.max(10, 25))        # Output: 25
println(MathHelper.pow(2, 10))         # Output: 1024
println(MathHelper.factorial(5))       # Output: 120
println(MathHelper.gcd(48, 18))        # Output: 6
println(MathHelper.isPrime(17))        # Output: true
println(MathHelper.circleArea(5))      # Output: 78.539816...
```

---

## Static vs Instance Members

Understanding the difference between static and instance members is crucial for proper OOP design.

### Comparison Table

| Aspect | Static Members | Instance Members |
|--------|---------------|------------------|
| **Belongs to** | Class itself | Each instance |
| **Memory** | Single copy | One copy per instance |
| **Access** | `ClassName.member` | `object.member` or `this.member` |
| **Lifetime** | Program lifetime | Object lifetime |
| **Creation** | When class loads | When object is created |
| **Can access** | Only static members | Both static and instance members |
| **Use for** | Shared data, utilities | Object-specific data |

### Side-by-Side Example

```klang
class Student {
    # STATIC: Shared by all students
    static schoolName = "KLang University"
    static totalStudents = 0
    static passingGrade = 60
    
    # INSTANCE: Unique to each student
    name = ""
    studentId = 0
    grade = 0
    
    fn constructor(studentName, studentGrade) {
        # Access static via class name
        Student.totalStudents = Student.totalStudents + 1
        
        # Access instance via this
        this.name = studentName
        this.studentId = Student.totalStudents
        this.grade = studentGrade
    }
    
    # STATIC METHOD: Works with class-level data
    static fn getSchoolInfo() {
        return Student.schoolName + " (Total Students: " + Student.totalStudents + ")"
    }
    
    static fn setPassingGrade(grade) {
        Student.passingGrade = grade
    }
    
    # INSTANCE METHOD: Works with object-specific data
    fn isPassing() {
        return this.grade >= Student.passingGrade
    }
    
    fn getStudentInfo() {
        return "ID: " + this.studentId + ", Name: " + this.name + 
               ", Grade: " + this.grade
    }
}

# Static access - no object needed
println(Student.getSchoolInfo())  # Output: KLang University (Total Students: 0)
Student.setPassingGrade(65)

# Instance access - requires objects
let student1 = new Student("Alice", 85)
let student2 = new Student("Bob", 62)
let student3 = new Student("Charlie", 70)

println(student1.getStudentInfo())  # ID: 1, Name: Alice, Grade: 85
println(student2.isPassing())       # false (62 < 65)
println(student3.isPassing())       # true (70 >= 65)

println(Student.getSchoolInfo())    # KLang University (Total Students: 3)
```

### When to Use Each

**Use Static Members when:**
- Data should be **shared** across all instances
- Creating **utility functions** that don't need instance data
- Defining **constants** at class level
- Implementing **factory methods**
- Tracking **class-wide statistics**

**Use Instance Members when:**
- Data is **unique** to each object
- Methods need to work with **object state**
- Each object should have **independent behavior**
- Representing **real-world entities** with individual properties

---

## Static Initialization

Static members are initialized when the class is **first loaded**, before any instances are created.

### Initialization Order

```klang
class InitDemo {
    # 1. Static members initialized first (class load time)
    static staticVar1 = 100
    static staticVar2 = InitDemo.staticVar1 * 2  # 200
    
    # 3. Instance members initialized when object created
    instanceVar1 = 10
    instanceVar2 = this.instanceVar1 * 2  # 20
    
    # 2. Static initialization block (if supported)
    static {
        println("Class loaded - static initialization")
        InitDemo.staticVar1 = 150
    }
    
    # 4. Constructor runs after instance variables
    fn constructor() {
        println("Constructor called")
        println("Static: " + InitDemo.staticVar1)
        println("Instance: " + this.instanceVar1)
    }
}

# Class loads here - static members initialized
let obj1 = new InitDemo()
# Output:
# Class loaded - static initialization
# Constructor called
# Static: 150
# Instance: 10

let obj2 = new InitDemo()
# Output (no static initialization, already loaded):
# Constructor called
# Static: 150
# Instance: 10
```

### Initialization Best Practices

```klang
class Database {
    # Simple initialization
    static maxConnections = 100
    
    # Computed initialization
    static minConnections = Database.maxConnections / 10
    
    # Default values
    static host = "localhost"
    static port = 5432
    static database = "mydb"
    
    static fn getConnectionString() {
        return Database.host + ":" + Database.port + "/" + Database.database
    }
    
    # Initialize method for complex setup
    static fn initialize(host, port, database) {
        Database.host = host
        Database.port = port
        Database.database = database
        println("Database configured: " + Database.getConnectionString())
    }
}

# Simple usage with defaults
println(Database.getConnectionString())  # localhost:5432/mydb

# Custom initialization
Database.initialize("prod.server.com", 3306, "production_db")
# Output: Database configured: prod.server.com:3306/production_db
```

---

## Common Use Cases

### 1. Factory Methods

Factory methods are static methods that create and return instances with specific configurations.

```klang
class Color {
    red = 0
    green = 0
    blue = 0
    
    fn constructor(r, g, b) {
        this.red = r
        this.green = g
        this.blue = b
    }
    
    # Factory methods - static constructors
    static fn createRed() {
        return new Color(255, 0, 0)
    }
    
    static fn createGreen() {
        return new Color(0, 255, 0)
    }
    
    static fn createBlue() {
        return new Color(0, 0, 255)
    }
    
    static fn createWhite() {
        return new Color(255, 255, 255)
    }
    
    static fn createBlack() {
        return new Color(0, 0, 0)
    }
    
    static fn createGray(intensity) {
        return new Color(intensity, intensity, intensity)
    }
    
    static fn fromHex(hexString) {
        # Parse hex string and create color
        # Simplified implementation
        return new Color(255, 255, 255)
    }
    
    fn toString() {
        return "Color(R:" + this.red + ", G:" + this.green + ", B:" + this.blue + ")"
    }
}

# Usage - more readable than new Color(255, 0, 0)
let red = Color.createRed()
let green = Color.createGreen()
let gray = Color.createGray(128)
let custom = Color.fromHex("#FF5733")

println(red.toString())    # Color(R:255, G:0, B:0)
println(gray.toString())   # Color(R:128, G:128, B:128)
```

### 2. Constants

Static members are perfect for class-level constants.

```klang
class HttpStatus {
    # HTTP Status Code Constants
    static OK = 200
    static CREATED = 201
    static ACCEPTED = 202
    static NO_CONTENT = 204
    
    static BAD_REQUEST = 400
    static UNAUTHORIZED = 401
    static FORBIDDEN = 403
    static NOT_FOUND = 404
    
    static INTERNAL_SERVER_ERROR = 500
    static SERVICE_UNAVAILABLE = 503
    
    static fn getMessage(code) {
        if (code == HttpStatus.OK) {
            return "OK"
        } else if (code == HttpStatus.NOT_FOUND) {
            return "Not Found"
        } else if (code == HttpStatus.INTERNAL_SERVER_ERROR) {
            return "Internal Server Error"
        }
        return "Unknown Status"
    }
    
    static fn isSuccess(code) {
        return code >= 200 && code < 300
    }
    
    static fn isError(code) {
        return code >= 400
    }
}

# Usage
let status = HttpStatus.OK
println(status)                           # 200
println(HttpStatus.getMessage(status))    # OK
println(HttpStatus.isSuccess(status))     # true

if (status == HttpStatus.NOT_FOUND) {
    println("Resource not found")
}
```

### 3. Helper Functions

Static methods for utility operations that don't require instance state.

```klang
class ArrayHelper {
    static fn sum(arr) {
        let total = 0
        let i = 0
        while (i < arr.length()) {
            total = total + arr[i]
            i = i + 1
        }
        return total
    }
    
    static fn average(arr) {
        if (arr.length() == 0) {
            return 0
        }
        return ArrayHelper.sum(arr) / arr.length()
    }
    
    static fn max(arr) {
        if (arr.length() == 0) {
            return null
        }
        let maxVal = arr[0]
        let i = 1
        while (i < arr.length()) {
            if (arr[i] > maxVal) {
                maxVal = arr[i]
            }
            i = i + 1
        }
        return maxVal
    }
    
    static fn min(arr) {
        if (arr.length() == 0) {
            return null
        }
        let minVal = arr[0]
        let i = 1
        while (i < arr.length()) {
            if (arr[i] < minVal) {
                minVal = arr[i]
            }
            i = i + 1
        }
        return minVal
    }
    
    static fn reverse(arr) {
        let result = []
        let i = arr.length() - 1
        while (i >= 0) {
            result.push(arr[i])
            i = i - 1
        }
        return result
    }
}

# Usage
let numbers = [5, 2, 8, 1, 9, 3]
println(ArrayHelper.sum(numbers))      # 28
println(ArrayHelper.average(numbers))  # 4.666...
println(ArrayHelper.max(numbers))      # 9
println(ArrayHelper.min(numbers))      # 1
```

### 4. Singleton Pattern

Use static members to implement the Singleton design pattern.

```klang
class GameManager {
    # Static instance holder
    static instance = null
    
    # Instance properties
    score = 0
    level = 1
    playerName = ""
    
    # Private constructor (by convention)
    fn constructor() {
        if (GameManager.instance != null) {
            println("ERROR: GameManager already exists!")
            return null
        }
        this.playerName = "Player"
    }
    
    # Static method to get/create instance
    static fn getInstance() {
        if (GameManager.instance == null) {
            GameManager.instance = new GameManager()
        }
        return GameManager.instance
    }
    
    fn addScore(points) {
        this.score = this.score + points
        println("Score: " + this.score)
    }
    
    fn levelUp() {
        this.level = this.level + 1
        println("Level Up! Now at level " + this.level)
    }
    
    fn getGameState() {
        return "Player: " + this.playerName + 
               ", Level: " + this.level + 
               ", Score: " + this.score
    }
}

# Usage - always get same instance
let game1 = GameManager.getInstance()
game1.addScore(100)
game1.levelUp()

let game2 = GameManager.getInstance()  # Same instance as game1
game2.addScore(50)

println(game1.getGameState())  # Player: Player, Level: 2, Score: 150
println(game2.getGameState())  # Player: Player, Level: 2, Score: 150
println(game1 == game2)        # true (same instance)
```

---

## Best Practices

### 1. ✓ Use Class Name for Static Access

Always access static members through the class name, not through instances.

```klang
class Config {
    static appName = "MyApp"
    
    static fn getVersion() {
        return "1.0.0"
    }
}

# ✓ GOOD: Clear that it's a static member
println(Config.appName)
println(Config.getVersion())

# ✗ BAD: Misleading, looks like instance member
let c = new Config()
println(c.appName)       # Confusing
println(c.getVersion())  # Confusing
```

### 2. ✓ Static for Shared State, Instance for Unique Data

Use static members only when data should truly be shared across all instances.

```klang
class Employee {
    # ✓ GOOD: Company name is shared
    static companyName = "TechCorp"
    static totalEmployees = 0
    
    # ✓ GOOD: Each employee has unique data
    name = ""
    employeeId = 0
    salary = 0.0
    
    fn constructor(empName, empSalary) {
        Employee.totalEmployees = Employee.totalEmployees + 1
        this.employeeId = Employee.totalEmployees
        this.name = empName
        this.salary = empSalary
    }
}
```

### 3. ✓ Use Static Methods for Utilities

Create utility classes with static methods for operations that don't need instance state.

```klang
# ✓ GOOD: Pure utility class
class StringUtils {
    static fn isEmpty(str) {
        return str == null || str.length() == 0
    }
    
    static fn isBlank(str) {
        return StringUtils.isEmpty(str) || str.trim().length() == 0
    }
}

# ✗ BAD: Unnecessary instance creation for utilities
class StringUtils {
    fn isEmpty(str) {  # Shouldn't be instance method
        return str == null || str.length() == 0
    }
}
let utils = new StringUtils()  # Wasteful
utils.isEmpty("test")          # Awkward
```

### 4. ✓ Constants Should Be Static

Define constants as static members.

```klang
# ✓ GOOD: Constants are static
class GameConstants {
    static MAX_PLAYERS = 4
    static DEFAULT_LIVES = 3
    static SCREEN_WIDTH = 800
    static SCREEN_HEIGHT = 600
}

# ✗ BAD: Constants as instance members
class GameConstants {
    MAX_PLAYERS = 4        # Each instance has a copy (wasteful)
    DEFAULT_LIVES = 3
}
```

### 5. ✓ Static Methods Can't Access Instance Members

Remember that static methods can only directly access static members.

```klang
class Example {
    static staticVar = 10
    instanceVar = 20
    
    # ✓ GOOD: Static method uses static members
    static fn getStaticValue() {
        return Example.staticVar
    }
    
    # ✗ BAD: Static method trying to access instance member
    static fn getInstanceValue() {
        # return this.instanceVar  # ERROR: 'this' not available in static context
        # return instanceVar        # ERROR: Cannot access instance member
    }
    
    # ✓ GOOD: Instance method can access both
    fn getBothValues() {
        return Example.staticVar + this.instanceVar  # OK
    }
}
```

### 6. ✓ Use Factory Methods for Complex Creation

Static factory methods provide better control and readability for object creation.

```klang
# ✓ GOOD: Factory methods for different configurations
class Database {
    host = ""
    port = 0
    username = ""
    
    fn constructor(h, p, u) {
        this.host = h
        this.port = p
        this.username = u
    }
    
    static fn createLocal() {
        return new Database("localhost", 5432, "admin")
    }
    
    static fn createProduction() {
        return new Database("prod.server.com", 5432, "prod_user")
    }
    
    static fn createTest() {
        return new Database("test.server.com", 5433, "test_user")
    }
}

# Clear and readable
let localDb = Database.createLocal()
let prodDb = Database.createProduction()
```

### 7. ✓ Document Static Members Clearly

Make it clear which members are static in your documentation and naming.

```klang
class Logger {
    # Static configuration (class-level)
    static globalLogLevel = "INFO"    # Affects all loggers
    static totalLogs = 0              # Counts all logs
    
    # Instance configuration (object-level)
    moduleName = ""                   # Specific to this logger
    localLogCount = 0                 # Counts this logger's logs
    
    fn constructor(module) {
        this.moduleName = module
    }
}
```

---

## Interview Problems

### Problem 1: Instance Counter with Categories

**Question:** Implement a `Vehicle` class that tracks:
- Total number of vehicles created
- Count of vehicles by category (car, truck, bike)
- Each vehicle should have a unique ID

```klang
class Vehicle {
    static totalVehicles = 0
    static carCount = 0
    static truckCount = 0
    static bikeCount = 0
    
    id = 0
    category = ""
    
    fn constructor(vehicleCategory) {
        Vehicle.totalVehicles = Vehicle.totalVehicles + 1
        this.id = Vehicle.totalVehicles
        this.category = vehicleCategory
        
        if (vehicleCategory == "car") {
            Vehicle.carCount = Vehicle.carCount + 1
        } else if (vehicleCategory == "truck") {
            Vehicle.truckCount = Vehicle.truckCount + 1
        } else if (vehicleCategory == "bike") {
            Vehicle.bikeCount = Vehicle.bikeCount + 1
        }
    }
    
    static fn getStatistics() {
        println("=== Vehicle Statistics ===")
        println("Total Vehicles: " + Vehicle.totalVehicles)
        println("Cars: " + Vehicle.carCount)
        println("Trucks: " + Vehicle.truckCount)
        println("Bikes: " + Vehicle.bikeCount)
    }
    
    static fn getMostCommonCategory() {
        let maxCount = Vehicle.carCount
        let category = "car"
        
        if (Vehicle.truckCount > maxCount) {
            maxCount = Vehicle.truckCount
            category = "truck"
        }
        if (Vehicle.bikeCount > maxCount) {
            maxCount = Vehicle.bikeCount
            category = "bike"
        }
        
        return category
    }
}

# Test
let v1 = new Vehicle("car")
let v2 = new Vehicle("car")
let v3 = new Vehicle("truck")
let v4 = new Vehicle("bike")
let v5 = new Vehicle("car")

Vehicle.getStatistics()
# Output:
# Total Vehicles: 5
# Cars: 3
# Trucks: 1
# Bikes: 1

println("Most common: " + Vehicle.getMostCommonCategory())  # car
```

### Problem 2: Currency Converter

**Question:** Create a `CurrencyConverter` utility class with static exchange rates and conversion methods.

```klang
class CurrencyConverter {
    # Static exchange rates (base: USD)
    static USD_TO_EUR = 0.85
    static USD_TO_GBP = 0.73
    static USD_TO_JPY = 110.0
    static USD_TO_INR = 74.5
    
    static fn usdToEur(amount) {
        return amount * CurrencyConverter.USD_TO_EUR
    }
    
    static fn usdToGbp(amount) {
        return amount * CurrencyConverter.USD_TO_GBP
    }
    
    static fn usdToJpy(amount) {
        return amount * CurrencyConverter.USD_TO_JPY
    }
    
    static fn usdToInr(amount) {
        return amount * CurrencyConverter.USD_TO_INR
    }
    
    static fn eurToUsd(amount) {
        return amount / CurrencyConverter.USD_TO_EUR
    }
    
    static fn convert(amount, fromCurrency, toCurrency) {
        # First convert to USD, then to target currency
        let usdAmount = amount
        
        # Convert from source to USD
        if (fromCurrency == "EUR") {
            usdAmount = amount / CurrencyConverter.USD_TO_EUR
        } else if (fromCurrency == "GBP") {
            usdAmount = amount / CurrencyConverter.USD_TO_GBP
        } else if (fromCurrency == "JPY") {
            usdAmount = amount / CurrencyConverter.USD_TO_JPY
        } else if (fromCurrency == "INR") {
            usdAmount = amount / CurrencyConverter.USD_TO_INR
        }
        
        # Convert from USD to target
        if (toCurrency == "USD") {
            return usdAmount
        } else if (toCurrency == "EUR") {
            return usdAmount * CurrencyConverter.USD_TO_EUR
        } else if (toCurrency == "GBP") {
            return usdAmount * CurrencyConverter.USD_TO_GBP
        } else if (toCurrency == "JPY") {
            return usdAmount * CurrencyConverter.USD_TO_JPY
        } else if (toCurrency == "INR") {
            return usdAmount * CurrencyConverter.USD_TO_INR
        }
        
        return usdAmount
    }
}

# Test
println(CurrencyConverter.usdToEur(100))           # 85.0
println(CurrencyConverter.convert(100, "USD", "EUR"))  # 85.0
println(CurrencyConverter.convert(85, "EUR", "GBP"))   # ~62.05
```

### Problem 3: User Authentication System

**Question:** Implement a `UserAuth` class that:
- Tracks logged-in users (static)
- Each user instance has username and session
- Provides static methods to check if user is logged in

```klang
class UserAuth {
    # Static - tracks all logged in users
    static loggedInUsers = []
    static maxSessions = 10
    static totalLoginAttempts = 0
    
    # Instance - individual user data
    username = ""
    sessionId = ""
    isLoggedIn = false
    
    fn constructor(user) {
        this.username = user
        this.isLoggedIn = false
    }
    
    fn login(password) {
        UserAuth.totalLoginAttempts = UserAuth.totalLoginAttempts + 1
        
        if (UserAuth.loggedInUsers.length() >= UserAuth.maxSessions) {
            println("ERROR: Maximum sessions reached")
            return false
        }
        
        # Simplified password check
        if (password == "password123") {
            this.isLoggedIn = true
            this.sessionId = "session_" + UserAuth.totalLoginAttempts
            UserAuth.loggedInUsers.push(this.username)
            println(this.username + " logged in")
            return true
        }
        
        println("Invalid password")
        return false
    }
    
    fn logout() {
        if (this.isLoggedIn) {
            this.isLoggedIn = false
            # Remove from logged in users
            let i = 0
            while (i < UserAuth.loggedInUsers.length()) {
                if (UserAuth.loggedInUsers[i] == this.username) {
                    UserAuth.loggedInUsers.remove(i)
                    break
                }
                i = i + 1
            }
            println(this.username + " logged out")
        }
    }
    
    static fn isUserLoggedIn(username) {
        let i = 0
        while (i < UserAuth.loggedInUsers.length()) {
            if (UserAuth.loggedInUsers[i] == username) {
                return true
            }
            i = i + 1
        }
        return false
    }
    
    static fn getActiveUsers() {
        return UserAuth.loggedInUsers.length()
    }
    
    static fn getLoginStats() {
        println("Total login attempts: " + UserAuth.totalLoginAttempts)
        println("Active users: " + UserAuth.loggedInUsers.length())
    }
}

# Test
let user1 = new UserAuth("alice")
let user2 = new UserAuth("bob")

user1.login("password123")
user2.login("password123")

println(UserAuth.isUserLoggedIn("alice"))  # true
println(UserAuth.getActiveUsers())         # 2

user1.logout()
println(UserAuth.isUserLoggedIn("alice"))  # false
UserAuth.getLoginStats()
```

### Problem 4: Temperature Converter

**Question:** Create a `Temperature` utility class with static conversion methods between Celsius, Fahrenheit, and Kelvin.

```klang
class Temperature {
    static ABSOLUTE_ZERO_C = -273.15
    static ABSOLUTE_ZERO_F = -459.67
    static ABSOLUTE_ZERO_K = 0.0
    
    static fn celsiusToFahrenheit(celsius) {
        return (celsius * 9.0 / 5.0) + 32.0
    }
    
    static fn fahrenheitToCelsius(fahrenheit) {
        return (fahrenheit - 32.0) * 5.0 / 9.0
    }
    
    static fn celsiusToKelvin(celsius) {
        return celsius + 273.15
    }
    
    static fn kelvinToCelsius(kelvin) {
        return kelvin - 273.15
    }
    
    static fn fahrenheitToKelvin(fahrenheit) {
        let celsius = Temperature.fahrenheitToCelsius(fahrenheit)
        return Temperature.celsiusToKelvin(celsius)
    }
    
    static fn kelvinToFahrenheit(kelvin) {
        let celsius = Temperature.kelvinToCelsius(kelvin)
        return Temperature.celsiusToFahrenheit(celsius)
    }
    
    static fn isValidCelsius(temp) {
        return temp >= Temperature.ABSOLUTE_ZERO_C
    }
    
    static fn isValidFahrenheit(temp) {
        return temp >= Temperature.ABSOLUTE_ZERO_F
    }
    
    static fn isValidKelvin(temp) {
        return temp >= Temperature.ABSOLUTE_ZERO_K
    }
    
    static fn getBoilingPoint(scale) {
        if (scale == "C") {
            return 100.0
        } else if (scale == "F") {
            return 212.0
        } else if (scale == "K") {
            return 373.15
        }
        return 0.0
    }
}

# Test
println(Temperature.celsiusToFahrenheit(0))      # 32.0
println(Temperature.fahrenheitToCelsius(32))     # 0.0
println(Temperature.celsiusToKelvin(0))          # 273.15
println(Temperature.isValidCelsius(-300))        # false
println(Temperature.getBoilingPoint("C"))        # 100.0
```

### Problem 5: ID Generator

**Question:** Create an `IDGenerator` class that generates unique IDs with different formats.

```klang
class IDGenerator {
    static nextId = 1
    static prefix = "ID"
    static totalGenerated = 0
    
    static fn generateSimple() {
        let id = IDGenerator.nextId
        IDGenerator.nextId = IDGenerator.nextId + 1
        IDGenerator.totalGenerated = IDGenerator.totalGenerated + 1
        return id
    }
    
    static fn generateWithPrefix() {
        let id = IDGenerator.prefix + "_" + IDGenerator.nextId
        IDGenerator.nextId = IDGenerator.nextId + 1
        IDGenerator.totalGenerated = IDGenerator.totalGenerated + 1
        return id
    }
    
    static fn generatePadded(length) {
        let id = IDGenerator.nextId
        let idStr = "" + id
        
        while (idStr.length() < length) {
            idStr = "0" + idStr
        }
        
        IDGenerator.nextId = IDGenerator.nextId + 1
        IDGenerator.totalGenerated = IDGenerator.totalGenerated + 1
        return IDGenerator.prefix + "_" + idStr
    }
    
    static fn setPrefix(newPrefix) {
        IDGenerator.prefix = newPrefix
    }
    
    static fn reset() {
        IDGenerator.nextId = 1
        IDGenerator.totalGenerated = 0
    }
    
    static fn getStats() {
        println("Next ID: " + IDGenerator.nextId)
        println("Total Generated: " + IDGenerator.totalGenerated)
        println("Current Prefix: " + IDGenerator.prefix)
    }
}

# Test
println(IDGenerator.generateSimple())           # 1
println(IDGenerator.generateSimple())           # 2
println(IDGenerator.generateWithPrefix())       # ID_3
println(IDGenerator.generatePadded(5))          # ID_00004

IDGenerator.setPrefix("USER")
println(IDGenerator.generateWithPrefix())       # USER_5
println(IDGenerator.generatePadded(6))          # USER_000006

IDGenerator.getStats()
# Next ID: 7
# Total Generated: 6
# Current Prefix: USER
```

### Problem 6: Rate Limiter

**Question:** Implement a `RateLimiter` class that tracks API calls and enforces limits.

```klang
class RateLimiter {
    static maxRequestsPerMinute = 60
    static requestCount = 0
    static blockedCount = 0
    static lastResetTime = 0
    
    static fn canMakeRequest() {
        # Simplified: In real implementation, check actual time
        if (RateLimiter.requestCount >= RateLimiter.maxRequestsPerMinute) {
            RateLimiter.blockedCount = RateLimiter.blockedCount + 1
            return false
        }
        return true
    }
    
    static fn makeRequest(endpoint) {
        if (RateLimiter.canMakeRequest()) {
            RateLimiter.requestCount = RateLimiter.requestCount + 1
            println("Request to " + endpoint + " allowed (" + 
                    RateLimiter.requestCount + "/" + 
                    RateLimiter.maxRequestsPerMinute + ")")
            return true
        } else {
            println("Request to " + endpoint + " blocked (rate limit exceeded)")
            return false
        }
    }
    
    static fn resetCounter() {
        println("Resetting rate limiter")
        RateLimiter.requestCount = 0
    }
    
    static fn getStatistics() {
        println("=== Rate Limiter Stats ===")
        println("Requests this minute: " + RateLimiter.requestCount)
        println("Requests blocked: " + RateLimiter.blockedCount)
        println("Limit: " + RateLimiter.maxRequestsPerMinute)
        let remaining = RateLimiter.maxRequestsPerMinute - RateLimiter.requestCount
        println("Remaining: " + remaining)
    }
    
    static fn setLimit(newLimit) {
        RateLimiter.maxRequestsPerMinute = newLimit
        println("Rate limit set to " + newLimit + " requests/minute")
    }
}

# Test
RateLimiter.setLimit(5)

let i = 0
while (i < 7) {
    RateLimiter.makeRequest("/api/data")
    i = i + 1
}

RateLimiter.getStatistics()
# Output:
# Requests this minute: 5
# Requests blocked: 2
# Limit: 5
# Remaining: 0
```

---

## Summary

Static members are powerful features in KLang's OOP system that enable:

1. **Shared State**: Data shared across all instances
2. **Utility Classes**: Functions that don't need instance data
3. **Memory Efficiency**: Single copy for all instances
4. **Factory Methods**: Smart object creation patterns
5. **Class-Level Operations**: Tracking and managing class-wide data

### Key Takeaways

- Static members belong to the **class**, not instances
- Access via **class name** (e.g., `ClassName.staticMember`)
- Static methods can only access **static members** directly
- Perfect for **utilities**, **constants**, and **shared state**
- Initialize once when **class loads**
- Use for **factory methods** and **singleton pattern**

### When to Use Static

✅ **Use Static** for:
- Utility functions (Math, String operations)
- Constants and configuration
- Instance counters
- Factory methods
- Singleton pattern

❌ **Don't Use Static** for:
- Object-specific data
- Methods that need instance state
- Anything that varies per object

---

**Next Topics:**
- [Interfaces](10_interfaces.md) - Coming Soon
- [Encapsulation](11_encapsulation.md) - Coming Soon
- [Composition](12_composition.md) - Coming Soon

---

*Last Updated: [Current Date]*  
*KLang Version: 2.0+*
