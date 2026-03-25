# Access Modifiers in KLang

## Table of Contents
1. [Introduction](#introduction)
2. [What are Access Modifiers?](#what-are-access-modifiers)
3. [Types of Access Modifiers](#types-of-access-modifiers)
4. [Default Access](#default-access)
5. [Member Access Control](#member-access-control)
6. [Practical Examples](#practical-examples)
7. [Getter and Setter Methods](#getter-and-setter-methods)
8. [Data Hiding and Encapsulation](#data-hiding-and-encapsulation)
9. [Access Control Tables](#access-control-tables)
10. [Best Practices](#best-practices)
11. [Common Mistakes](#common-mistakes)
12. [Interview Problems](#interview-problems)

---

## Introduction

**Access Modifiers** are keywords that set the accessibility (visibility) of classes, methods, and properties. They are fundamental to **encapsulation**, one of the core principles of Object-Oriented Programming.

### Why Access Modifiers?

- **Security**: Protect sensitive data from unauthorized access
- **Encapsulation**: Hide internal implementation details
- **Maintainability**: Change internals without breaking external code
- **Controlled Access**: Provide controlled interfaces to class members
- **Data Integrity**: Prevent invalid state through validation

### Key Concept

```
┌─────────────────────────────────────────┐
│  Class Boundary                         │
│  ┌─────────────────────────────────┐   │
│  │  Private Members                 │   │
│  │  (Only accessible within class)  │   │
│  └─────────────────────────────────┘   │
│  ┌─────────────────────────────────┐   │
│  │  Protected Members               │   │
│  │  (Class + Subclasses)            │   │
│  └─────────────────────────────────┘   │
│  ┌─────────────────────────────────┐   │
│  │  Public Members                  │   │
│  │  (Accessible everywhere)         │   │
│  └─────────────────────────────────┘   │
└─────────────────────────────────────────┘
```

---

## What are Access Modifiers?

Access modifiers control the **visibility** and **accessibility** of class members (properties and methods). They determine:

1. **Where** a member can be accessed from
2. **Who** can modify or read the data
3. **How** the class interacts with external code

### The Three Pillars of Access Control

```
Public      →  Maximum accessibility
    ↓
Protected   →  Moderate accessibility (inheritance chain)
    ↓
Private     →  Minimum accessibility (internal only)
```

### Simple Example

```klang
class Person {
    # Public - accessible everywhere
    public let name: string = ""
    
    # Private - only within Person class
    private let ssn: string = ""
    
    # Protected - Person class and subclasses
    protected let salary: float = 0.0
    
    public fn init(n: string, s: string, sal: float) {
        this.name = n
        this.ssn = s
        this.salary = sal
    }
    
    # Public method - can be called from anywhere
    public fn getName() -> string {
        return this.name
    }
    
    # Private method - only usable within this class
    private fn validateSSN(ssn: string) -> bool {
        return len(ssn) == 11  # Format: XXX-XX-XXXX
    }
}

let person = new Person("Alice", "123-45-6789", 75000.0)
println(person.name)          # ✓ OK - public
# println(person.ssn)         # ✗ ERROR - private
# println(person.salary)      # ✗ ERROR - protected
println(person.getName())     # ✓ OK - public method
```

---

## Types of Access Modifiers

### 1. Public Modifier

**Public** members are accessible from **anywhere** - within the class, subclasses, and external code.

#### Syntax

```klang
class Example {
    public let data: int = 0
    
    public fn publicMethod() {
        println("Accessible everywhere")
    }
}
```

#### Characteristics

- ✓ Accessible within the class
- ✓ Accessible in subclasses
- ✓ Accessible from external code
- ✓ Forms the public API/interface

#### When to Use Public

- Methods that form the class interface
- Properties meant to be accessed externally
- Constants and configuration values
- Utility methods for external use

#### Example

```klang
class Calculator {
    public let version: string = "1.0"
    
    # Public methods - part of the API
    public fn add(a: int, b: int) -> int {
        return a + b
    }
    
    public fn subtract(a: int, b: int) -> int {
        return a - b
    }
    
    public fn multiply(a: int, b: int) -> int {
        return a * b
    }
}

let calc = new Calculator()
println(calc.version)           # ✓ OK
println(calc.add(5, 3))         # ✓ OK - Output: 8
println(calc.multiply(4, 2))    # ✓ OK - Output: 8
```

---

### 2. Private Modifier

**Private** members are accessible **only within the class** where they are defined. They cannot be accessed from subclasses or external code.

#### Syntax

```klang
class Example {
    private let secret: string = ""
    
    private fn privateMethod() {
        println("Only accessible within this class")
    }
}
```

#### Characteristics

- ✓ Accessible within the class
- ✗ NOT accessible in subclasses
- ✗ NOT accessible from external code
- ✓ Hides implementation details

#### When to Use Private

- Internal implementation details
- Sensitive data (passwords, keys)
- Helper methods not meant for external use
- Data that needs validation before access

#### Example

```klang
class BankAccount {
    private let accountNumber: string = ""
    private let balance: float = 0.0
    private let pin: string = ""
    
    public fn init(accNum: string, initialBalance: float, pinCode: string) {
        this.accountNumber = accNum
        this.balance = initialBalance
        this.pin = pinCode
    }
    
    # Private validation method
    private fn validatePin(inputPin: string) -> bool {
        return this.pin == inputPin
    }
    
    # Private helper method
    private fn updateBalance(amount: float) {
        this.balance = this.balance + amount
    }
    
    # Public interface - uses private members internally
    public fn withdraw(amount: float, inputPin: string) -> bool {
        if !this.validatePin(inputPin) {
            println("Invalid PIN")
            return false
        }
        
        if amount > this.balance {
            println("Insufficient funds")
            return false
        }
        
        this.updateBalance(-amount)
        println("Withdrawal successful")
        return true
    }
    
    public fn getBalance(inputPin: string) -> float {
        if this.validatePin(inputPin) {
            return this.balance
        }
        return -1.0  # Error indicator
    }
}

let account = new BankAccount("ACC-12345", 1000.0, "1234")

# ✓ OK - using public methods
println(account.getBalance("1234"))  # Output: 1000.0
account.withdraw(200.0, "1234")      # Output: Withdrawal successful

# ✗ ERROR - cannot access private members
# println(account.balance)           # Compilation error
# println(account.pin)               # Compilation error
# account.validatePin("1234")        # Compilation error
```

---

### 3. Protected Modifier

**Protected** members are accessible within the class and in all **subclasses**, but not from external code.

#### Syntax

```klang
class Example {
    protected let data: string = ""
    
    protected fn protectedMethod() {
        println("Accessible in class and subclasses")
    }
}
```

#### Characteristics

- ✓ Accessible within the class
- ✓ Accessible in subclasses (inherited)
- ✗ NOT accessible from external code
- ✓ Supports inheritance hierarchy

#### When to Use Protected

- Members shared across inheritance hierarchy
- Methods meant to be overridden
- Shared state between parent and child
- Template method pattern implementations

#### Example

```klang
class User {
    public let username: string = ""
    protected let passwordHash: string = ""
    protected let loginAttempts: int = 0
    private let secretKey: string = "SECRET_KEY_123"
    
    public fn init(user: string, pass: string) {
        this.username = user
        this.passwordHash = this.hashPassword(pass)
        this.loginAttempts = 0
    }
    
    # Protected method - can be used by subclasses
    protected fn hashPassword(password: string) -> string {
        # Simplified hashing (in real code, use proper hashing)
        return "HASH_" + password + "_" + this.secretKey
    }
    
    # Protected method - subclasses can override
    protected fn onLoginSuccess() {
        this.loginAttempts = 0
        println("Login successful for " + this.username)
    }
    
    # Protected method
    protected fn onLoginFailure() {
        this.loginAttempts = this.loginAttempts + 1
        println("Login failed. Attempts: " + str(this.loginAttempts))
    }
    
    # Public method - uses protected members
    public fn login(password: string) -> bool {
        let hash = this.hashPassword(password)
        if hash == this.passwordHash {
            this.onLoginSuccess()
            return true
        } else {
            this.onLoginFailure()
            return false
        }
    }
}

class AdminUser extends User {
    private let adminLevel: int = 0
    
    public fn init(user: string, pass: string, level: int) {
        super.init(user, pass)
        this.adminLevel = level
    }
    
    # Override protected method - we can access it!
    protected fn onLoginSuccess() {
        # Can access protected members from parent
        this.loginAttempts = 0
        println("Admin login successful: " + this.username)
        println("Admin Level: " + str(this.adminLevel))
        self.logAdminAccess()
    }
    
    private fn logAdminAccess() {
        println("Admin access logged")
    }
    
    # Can use protected method from parent
    public fn resetPassword(oldPass: string, newPass: string) -> bool {
        let oldHash = this.hashPassword(oldPass)
        if oldHash == this.passwordHash {
            this.passwordHash = this.hashPassword(newPass)
            println("Password reset successful")
            return true
        }
        return false
    }
}

# Usage
let user = new User("alice", "pass123")
user.login("pass123")            # ✓ OK - public method
# user.hashPassword("test")      # ✗ ERROR - protected method
# user.passwordHash              # ✗ ERROR - protected property

let admin = new AdminUser("admin", "admin123", 5)
admin.login("admin123")          # ✓ OK - Admin login successful
admin.resetPassword("admin123", "newpass456")  # ✓ OK
```

---

## Default Access

When **no access modifier** is specified, KLang uses **public** as the default access level.

### Default Behavior

```klang
class Example {
    # No modifier = public by default
    let data: int = 0           # public
    
    fn method() {                # public
        println("Default access")
    }
}
```

### Explicit vs Implicit

```klang
class ImplicitPublic {
    let value: int = 10          # Implicitly public
    
    fn getValue() -> int {       # Implicitly public
        return this.value
    }
}

class ExplicitPublic {
    public let value: int = 10   # Explicitly public
    
    public fn getValue() -> int {  # Explicitly public
        return this.value
    }
}

# Both work the same way
let implicit = new ImplicitPublic()
println(implicit.value)          # ✓ OK

let explicit = new ExplicitPublic()
println(explicit.value)          # ✓ OK
```

### Best Practice

**Always use explicit access modifiers** for clarity and intent:

```klang
# ✗ Unclear intent
class Bad {
    let data: string = ""
    fn process() { }
}

# ✓ Clear intent
class Good {
    private let data: string = ""
    public fn process() { }
}
```

---

## Member Access Control

Access modifiers apply to both **properties** (data) and **methods** (behavior).

### Property Access

```klang
class DataContainer {
    public let publicData: int = 1          # Accessible everywhere
    protected let protectedData: int = 2    # Class + subclasses
    private let privateData: int = 3        # Only this class
    
    public fn accessAll() {
        println(this.publicData)      # ✓ OK
        println(this.protectedData)   # ✓ OK
        println(this.privateData)     # ✓ OK
    }
}

class SubContainer extends DataContainer {
    public fn accessInherited() {
        println(this.publicData)      # ✓ OK
        println(this.protectedData)   # ✓ OK
        # println(this.privateData)   # ✗ ERROR
    }
}

let container = new DataContainer()
println(container.publicData)      # ✓ OK
# println(container.protectedData) # ✗ ERROR
# println(container.privateData)   # ✗ ERROR
```

### Method Access

```klang
class Service {
    # Public method - external API
    public fn publicAPI() {
        println("Public method called")
        this.protectedHelper()
        this.privateImplementation()
    }
    
    # Protected method - for inheritance
    protected fn protectedHelper() {
        println("Protected helper")
        this.privateImplementation()
    }
    
    # Private method - internal only
    private fn privateImplementation() {
        println("Private implementation")
    }
}

class ExtendedService extends Service {
    public fn doWork() {
        this.publicAPI()          # ✓ OK
        this.protectedHelper()    # ✓ OK
        # this.privateImplementation()  # ✗ ERROR
    }
}

let service = new Service()
service.publicAPI()           # ✓ OK
# service.protectedHelper()   # ✗ ERROR
# service.privateImplementation()  # ✗ ERROR
```

### Mixed Access Example

```klang
class Configuration {
    # Public constants
    public let APP_NAME: string = "MyApp"
    public let VERSION: string = "1.0.0"
    
    # Private configuration
    private let apiKey: string = "sk_live_12345"
    private let databaseUrl: string = "db://localhost"
    
    # Protected settings
    protected let debug: bool = false
    protected let maxRetries: int = 3
    
    # Public getter - controlled access to private data
    public fn getApiKey() -> string {
        if this.isAuthenticated() {
            return this.apiKey
        }
        return ""
    }
    
    # Private validation
    private fn isAuthenticated() -> bool {
        # Check authentication logic
        return true
    }
    
    # Public method
    public fn printInfo() {
        println("App: " + this.APP_NAME)
        println("Version: " + this.VERSION)
        # Can access all members within the class
        println("Debug mode: " + str(this.debug))
    }
}
```

---

## Practical Examples

### Example 1: Bank Account System

A secure banking system with proper access control:

```klang
class BankAccount {
    # Public - account holder's name (non-sensitive)
    public let accountHolder: string = ""
    
    # Private - sensitive financial data
    private let accountNumber: string = ""
    private let balance: float = 0.0
    private let pin: string = ""
    private let transactionHistory: array<string> = []
    
    # Protected - for specialized account types
    protected let accountType: string = "Standard"
    protected let minimumBalance: float = 100.0
    
    public fn init(holder: string, accNum: string, initialDeposit: float, pinCode: string) {
        this.accountHolder = holder
        this.accountNumber = accNum
        this.balance = initialDeposit
        this.pin = pinCode
        this.addTransaction("Account opened with balance: " + str(initialDeposit))
    }
    
    # Private helper - validate PIN
    private fn validatePin(inputPin: string) -> bool {
        return this.pin == inputPin
    }
    
    # Private helper - check if sufficient funds
    private fn hasSufficientFunds(amount: float) -> bool {
        return (this.balance - amount) >= this.minimumBalance
    }
    
    # Private helper - record transaction
    private fn addTransaction(description: string) {
        # In real system, add timestamp
        this.transactionHistory.push(description)
    }
    
    # Protected method - for subclasses to customize
    protected fn calculateFee(transactionType: string) -> float {
        # Standard account has no fees
        return 0.0
    }
    
    # Public API - deposit money
    public fn deposit(amount: float, inputPin: string) -> bool {
        if !this.validatePin(inputPin) {
            println("Error: Invalid PIN")
            return false
        }
        
        if amount <= 0.0 {
            println("Error: Invalid amount")
            return false
        }
        
        this.balance = this.balance + amount
        this.addTransaction("Deposited: " + str(amount))
        println("Deposit successful. New balance: " + str(this.balance))
        return true
    }
    
    # Public API - withdraw money
    public fn withdraw(amount: float, inputPin: string) -> bool {
        if !this.validatePin(inputPin) {
            println("Error: Invalid PIN")
            return false
        }
        
        if amount <= 0.0 {
            println("Error: Invalid amount")
            return false
        }
        
        let fee = this.calculateFee("withdrawal")
        let totalAmount = amount + fee
        
        if !this.hasSufficientFunds(totalAmount) {
            println("Error: Insufficient funds")
            println("Required: " + str(totalAmount) + " (including fee: " + str(fee) + ")")
            return false
        }
        
        this.balance = this.balance - totalAmount
        this.addTransaction("Withdrew: " + str(amount) + " (Fee: " + str(fee) + ")")
        println("Withdrawal successful. New balance: " + str(this.balance))
        return true
    }
    
    # Public API - get balance (with authentication)
    public fn getBalance(inputPin: string) -> float {
        if this.validatePin(inputPin) {
            return this.balance
        }
        println("Error: Invalid PIN")
        return -1.0
    }
    
    # Public API - print statement
    public fn printStatement(inputPin: string) {
        if !this.validatePin(inputPin) {
            println("Error: Invalid PIN")
            return
        }
        
        println("=== Account Statement ===")
        println("Account Holder: " + this.accountHolder)
        println("Account Type: " + this.accountType)
        println("Current Balance: " + str(this.balance))
        println("\nTransaction History:")
        for let i = 0; i < len(this.transactionHistory); i = i + 1 {
            println((i + 1) + ". " + this.transactionHistory[i])
        }
        println("========================")
    }
}

# Premium account with different rules
class PremiumBankAccount extends BankAccount {
    private let rewardsPoints: int = 0
    
    public fn init(holder: string, accNum: string, initialDeposit: float, pinCode: string) {
        super.init(holder, accNum, initialDeposit, pinCode)
        this.accountType = "Premium"      # Can access protected member
        this.minimumBalance = 1000.0      # Can modify protected member
    }
    
    # Override fee calculation - no fees for premium
    protected fn calculateFee(transactionType: string) -> float {
        # Premium accounts have no transaction fees
        return 0.0
    }
    
    # Premium feature - earn rewards
    public fn earnRewards(amount: float) {
        this.rewardsPoints = this.rewardsPoints + int(amount / 10.0)
        println("Rewards earned! Total points: " + str(this.rewardsPoints))
    }
    
    public fn getRewards() -> int {
        return this.rewardsPoints
    }
}

# Usage
println("=== Standard Account ===")
let account = new BankAccount("Alice Smith", "ACC-001", 5000.0, "1234")
account.deposit(500.0, "1234")
account.withdraw(200.0, "1234")
println("Balance: " + str(account.getBalance("1234")))
account.printStatement("1234")

println("\n=== Premium Account ===")
let premium = new PremiumBankAccount("Bob Johnson", "PACC-001", 10000.0, "5678")
premium.deposit(2000.0, "5678")
premium.withdraw(500.0, "5678")
premium.earnRewards(2000.0)
premium.printStatement("5678")

# These would cause errors:
# println(account.balance)         # ✗ private
# println(account.pin)             # ✗ private
# account.validatePin("1234")      # ✗ private method
```

---

### Example 2: User Authentication System

Secure user authentication with proper encapsulation:

```klang
class User {
    # Public - non-sensitive info
    public let username: string = ""
    public let email: string = ""
    
    # Private - sensitive credentials
    private let passwordHash: string = ""
    private let salt: string = ""
    private let secretKey: string = ""
    
    # Protected - for role-based systems
    protected let role: string = "user"
    protected let permissions: array<string> = []
    protected let loginAttempts: int = 0
    protected let isLocked: bool = false
    
    public fn init(user: string, pass: string, mail: string) {
        this.username = user
        this.email = mail
        this.salt = this.generateSalt()
        this.secretKey = this.generateSecretKey()
        this.passwordHash = this.hashPassword(pass)
        this.setupDefaultPermissions()
    }
    
    # Private - security methods
    private fn generateSalt() -> string {
        # In real code, use crypto library
        return "SALT_" + str(random(1000, 9999))
    }
    
    private fn generateSecretKey() -> string {
        # In real code, use crypto library
        return "KEY_" + str(random(10000, 99999))
    }
    
    private fn hashPassword(password: string) -> string {
        # Simplified hashing (use proper crypto in production)
        return "HASH_" + this.salt + "_" + password + "_" + this.secretKey
    }
    
    # Protected - subclasses can override
    protected fn setupDefaultPermissions() {
        this.permissions = ["read", "write_own"]
    }
    
    # Protected - for inheritance
    protected fn onLoginSuccess() {
        this.loginAttempts = 0
        this.isLocked = false
        println("Login successful: " + this.username)
    }
    
    protected fn onLoginFailure() {
        this.loginAttempts = this.loginAttempts + 1
        println("Login failed. Attempts: " + str(this.loginAttempts))
        
        if this.loginAttempts >= 3 {
            this.isLocked = true
            println("Account locked due to multiple failed attempts")
        }
    }
    
    # Public API - authenticate
    public fn authenticate(password: string) -> bool {
        if this.isLocked {
            println("Account is locked. Contact administrator.")
            return false
        }
        
        let hash = this.hashPassword(password)
        if hash == this.passwordHash {
            this.onLoginSuccess()
            return true
        } else {
            this.onLoginFailure()
            return false
        }
    }
    
    # Public API - change password
    public fn changePassword(oldPassword: string, newPassword: string) -> bool {
        if !this.authenticate(oldPassword) {
            return false
        }
        
        this.passwordHash = this.hashPassword(newPassword)
        println("Password changed successfully")
        return true
    }
    
    # Public API - check permission
    public fn hasPermission(permission: string) -> bool {
        for let i = 0; i < len(this.permissions); i = i + 1 {
            if this.permissions[i] == permission {
                return true
            }
        }
        return false
    }
    
    # Public API - display info (no sensitive data)
    public fn displayInfo() {
        println("Username: " + this.username)
        println("Email: " + this.email)
        println("Role: " + this.role)
    }
}

class AdminUser extends User {
    private let adminLevel: int = 0
    
    public fn init(user: string, pass: string, mail: string, level: int) {
        super.init(user, pass, mail)
        this.role = "admin"           # Access protected member
        this.adminLevel = level
    }
    
    # Override to set admin permissions
    protected fn setupDefaultPermissions() {
        # Admins have more permissions
        this.permissions = ["read", "write_own", "write_all", "delete", "admin"]
    }
    
    # Override login success
    protected fn onLoginSuccess() {
        this.loginAttempts = 0
        this.isLocked = false
        println("Admin login successful: " + this.username)
        println("Admin Level: " + str(this.adminLevel))
    }
    
    # Admin-only method
    public fn unlockUser(user: User) {
        if this.hasPermission("admin") {
            user.isLocked = false
            user.loginAttempts = 0
            println("User unlocked by admin")
        }
    }
}

# Usage
let user = new User("alice", "pass123", "alice@example.com")
user.displayInfo()
user.authenticate("pass123")
println("Has write permission: " + str(user.hasPermission("write_own")))

let admin = new AdminUser("admin", "admin123", "admin@example.com", 5)
admin.displayInfo()
admin.authenticate("admin123")
println("Has admin permission: " + str(admin.hasPermission("admin")))
```

---

### Example 3: API Client with Private Implementation

Public interface with hidden implementation details:

```klang
class APIClient {
    # Public - configuration
    public let apiVersion: string = "v1"
    public let timeout: int = 30
    
    # Private - implementation details
    private let baseUrl: string = ""
    private let apiKey: string = ""
    private let rateLimitRemaining: int = 100
    private let lastRequestTime: int = 0
    
    # Protected - for specialized clients
    protected let headers: map<string, string> = {}
    protected let retryCount: int = 3
    
    public fn init(url: string, key: string) {
        this.baseUrl = url
        this.apiKey = key
        this.setupDefaultHeaders()
    }
    
    # Protected - subclasses can override
    protected fn setupDefaultHeaders() {
        this.headers["Content-Type"] = "application/json"
        this.headers["Accept"] = "application/json"
        this.headers["User-Agent"] = "KLang-API-Client/1.0"
    }
    
    # Private - rate limiting
    private fn checkRateLimit() -> bool {
        if this.rateLimitRemaining <= 0 {
            println("Rate limit exceeded. Try again later.")
            return false
        }
        return true
    }
    
    # Private - update rate limit
    private fn updateRateLimit() {
        this.rateLimitRemaining = this.rateLimitRemaining - 1
        this.lastRequestTime = getCurrentTime()  # Pseudo function
    }
    
    # Private - build full URL
    private fn buildUrl(endpoint: string) -> string {
        return this.baseUrl + "/" + this.apiVersion + "/" + endpoint
    }
    
    # Private - add authentication
    private fn addAuthHeader() -> map<string, string> {
        let authHeaders = this.headers
        authHeaders["Authorization"] = "Bearer " + this.apiKey
        return authHeaders
    }
    
    # Protected - for custom request handling
    protected fn handleResponse(statusCode: int, data: string) -> string {
        if statusCode >= 200 && statusCode < 300 {
            return data
        } else if statusCode == 429 {
            return "Error: Rate limit exceeded"
        } else {
            return "Error: Request failed with status " + str(statusCode)
        }
    }
    
    # Public API - GET request
    public fn get(endpoint: string) -> string {
        if !this.checkRateLimit() {
            return "Error: Rate limit"
        }
        
        let url = this.buildUrl(endpoint)
        let headers = this.addAuthHeader()
        
        # Simulated HTTP request
        println("GET " + url)
        println("Headers: " + str(len(headers)) + " headers")
        
        this.updateRateLimit()
        
        # Simulated response
        let statusCode = 200
        let data = "{\"status\": \"success\"}"
        
        return this.handleResponse(statusCode, data)
    }
    
    # Public API - POST request
    public fn post(endpoint: string, body: string) -> string {
        if !this.checkRateLimit() {
            return "Error: Rate limit"
        }
        
        let url = this.buildUrl(endpoint)
        let headers = this.addAuthHeader()
        
        # Simulated HTTP request
        println("POST " + url)
        println("Body: " + body)
        
        this.updateRateLimit()
        
        # Simulated response
        let statusCode = 201
        let data = "{\"status\": \"created\"}"
        
        return this.handleResponse(statusCode, data)
    }
    
    # Public API - get rate limit info
    public fn getRateLimitInfo() -> string {
        return "Rate limit remaining: " + str(this.rateLimitRemaining)
    }
}

class RetryableAPIClient extends APIClient {
    private let maxRetries: int = 5
    
    public fn init(url: string, key: string, retries: int) {
        super.init(url, key)
        this.maxRetries = retries
        this.retryCount = retries  # Can access protected member
    }
    
    # Override to add retry logic
    protected fn handleResponse(statusCode: int, data: string) -> string {
        if statusCode >= 500 && statusCode < 600 {
            println("Server error. Retrying...")
            # Would implement retry logic here
            return "Retrying after server error"
        }
        
        # Use parent's handling for other cases
        return super.handleResponse(statusCode, data)
    }
}

# Usage
let client = new APIClient("https://api.example.com", "sk_live_12345")
println(client.get("users/123"))
println(client.post("users", "{\"name\": \"Alice\"}"))
println(client.getRateLimitInfo())

# Cannot access private members
# println(client.apiKey)          # ✗ ERROR - private
# println(client.baseUrl)         # ✗ ERROR - private
```

---

## Getter and Setter Methods

**Getter and setter methods** provide controlled access to private properties. They allow validation, transformation, and logging when accessing or modifying data.

### Why Use Getters and Setters?

- **Validation**: Ensure data meets requirements
- **Encapsulation**: Hide internal representation
- **Computed Properties**: Calculate values on demand
- **Side Effects**: Log changes, trigger events
- **Read-Only**: Provide getters without setters

### Basic Pattern

```klang
class Person {
    private let name: string = ""
    private let age: int = 0
    
    # Getter for name
    public fn getName() -> string {
        return this.name
    }
    
    # Setter for name with validation
    public fn setName(newName: string) {
        if len(newName) > 0 {
            this.name = newName
        } else {
            println("Error: Name cannot be empty")
        }
    }
    
    # Getter for age
    public fn getAge() -> int {
        return this.age
    }
    
    # Setter for age with validation
    public fn setAge(newAge: int) {
        if newAge >= 0 && newAge <= 150 {
            this.age = newAge
        } else {
            println("Error: Invalid age")
        }
    }
}

let person = new Person()
person.setName("Alice")
person.setAge(30)
println("Name: " + person.getName())
println("Age: " + str(person.getAge()))

person.setAge(-5)   # Output: Error: Invalid age
person.setName("")  # Output: Error: Name cannot be empty
```

### Advanced Example: Product with Validation

```klang
class Product {
    private let id: string = ""
    private let name: string = ""
    private let price: float = 0.0
    private let quantity: int = 0
    private let discount: float = 0.0  # 0-100%
    
    public fn init(productId: string, productName: string) {
        this.id = productId
        this.setName(productName)
    }
    
    # Getter - simple return
    public fn getId() -> string {
        return this.id
    }
    
    # Getter for name
    public fn getName() -> string {
        return this.name
    }
    
    # Setter with validation
    public fn setName(newName: string) {
        if len(newName) >= 3 {
            this.name = newName
            println("Product name updated")
        } else {
            println("Error: Name must be at least 3 characters")
        }
    }
    
    # Getter for price
    public fn getPrice() -> float {
        return this.price
    }
    
    # Setter with validation
    public fn setPrice(newPrice: float) {
        if newPrice > 0.0 {
            this.price = newPrice
            println("Price updated to: " + str(newPrice))
        } else {
            println("Error: Price must be positive")
        }
    }
    
    # Getter with calculation
    public fn getFinalPrice() -> float {
        let discountAmount = this.price * (this.discount / 100.0)
        return this.price - discountAmount
    }
    
    # Getter for quantity
    public fn getQuantity() -> int {
        return this.quantity
    }
    
    # Setter with validation and logging
    public fn setQuantity(newQuantity: int) {
        if newQuantity >= 0 {
            let oldQuantity = this.quantity
            this.quantity = newQuantity
            println("Quantity updated: " + str(oldQuantity) + " -> " + str(newQuantity))
        } else {
            println("Error: Quantity cannot be negative")
        }
    }
    
    # Getter for discount
    public fn getDiscount() -> float {
        return this.discount
    }
    
    # Setter with validation
    public fn setDiscount(newDiscount: float) {
        if newDiscount >= 0.0 && newDiscount <= 100.0 {
            this.discount = newDiscount
            println("Discount set to: " + str(newDiscount) + "%")
        } else {
            println("Error: Discount must be between 0 and 100")
        }
    }
    
    # Read-only computed property
    public fn getTotalValue() -> float {
        return this.getFinalPrice() * float(this.quantity)
    }
    
    # Method to check availability
    public fn isInStock() -> bool {
        return this.quantity > 0
    }
    
    public fn displayInfo() {
        println("Product: " + this.name + " (ID: " + this.id + ")")
        println("Price: $" + str(this.price))
        println("Discount: " + str(this.discount) + "%")
        println("Final Price: $" + str(this.getFinalPrice()))
        println("Quantity: " + str(this.quantity))
        println("Total Value: $" + str(this.getTotalValue()))
        println("In Stock: " + str(this.isInStock()))
    }
}

# Usage
let product = new Product("PROD-001", "Laptop")
product.setPrice(1000.0)
product.setQuantity(50)
product.setDiscount(10.0)
product.displayInfo()

# Try invalid values
product.setPrice(-500.0)    # Error: Price must be positive
product.setQuantity(-10)     # Error: Quantity cannot be negative
product.setDiscount(150.0)   # Error: Discount must be between 0 and 100
```

### Read-Only Properties

```klang
class Configuration {
    private let configVersion: string = "1.0"
    private let createdAt: string = ""
    private let instanceId: string = ""
    
    public fn init() {
        this.createdAt = getCurrentDateTime()  # Pseudo function
        this.instanceId = generateUniqueId()   # Pseudo function
    }
    
    # Read-only getters (no setters)
    public fn getVersion() -> string {
        return this.configVersion
    }
    
    public fn getCreatedAt() -> string {
        return this.createdAt
    }
    
    public fn getInstanceId() -> string {
        return this.instanceId
    }
}

let config = new Configuration()
println("Version: " + config.getVersion())
println("Created: " + config.getCreatedAt())
println("ID: " + config.getInstanceId())
# Cannot modify - no setters provided!
```

---

## Data Hiding and Encapsulation

**Data hiding** (encapsulation) is the practice of keeping an object's internal state private and providing controlled access through public methods.

### Benefits of Data Hiding

1. **Security**: Protect sensitive data
2. **Integrity**: Prevent invalid state
3. **Maintainability**: Change internals without breaking external code
4. **Abstraction**: Hide complexity
5. **Testability**: Mock and test interfaces

### Encapsulation Principles

```
┌──────────────────────────────────────┐
│  Public Interface                     │
│  (What users see and use)            │
├──────────────────────────────────────┤
│  Protected Members                    │
│  (For inheritance hierarchy)         │
├──────────────────────────────────────┤
│  Private Implementation              │
│  (Hidden from outside world)         │
└──────────────────────────────────────┘
```

### Example: Encapsulated Stack

```klang
class Stack {
    private let items: array<int> = []
    private let maxSize: int = 100
    private let top: int = -1
    
    public fn init(size: int) {
        if size > 0 {
            this.maxSize = size
        }
    }
    
    # Public interface - push
    public fn push(item: int) -> bool {
        if this.isFull() {
            println("Error: Stack overflow")
            return false
        }
        
        this.top = this.top + 1
        this.items.push(item)
        return true
    }
    
    # Public interface - pop
    public fn pop() -> int {
        if this.isEmpty() {
            println("Error: Stack underflow")
            return -1
        }
        
        let item = this.items[this.top]
        this.items.pop()
        this.top = this.top - 1
        return item
    }
    
    # Public interface - peek
    public fn peek() -> int {
        if this.isEmpty() {
            println("Error: Stack is empty")
            return -1
        }
        return this.items[this.top]
    }
    
    # Public interface - isEmpty
    public fn isEmpty() -> bool {
        return this.top == -1
    }
    
    # Public interface - isFull
    public fn isFull() -> bool {
        return this.top >= (this.maxSize - 1)
    }
    
    # Public interface - size
    public fn size() -> int {
        return this.top + 1
    }
    
    # Public interface - display
    public fn display() {
        if this.isEmpty() {
            println("Stack is empty")
            return
        }
        
        println("Stack contents (top to bottom):")
        for let i = this.top; i >= 0; i = i - 1 {
            println(str(i) + ": " + str(this.items[i]))
        }
    }
}

# Usage - users don't need to know internal implementation
let stack = new Stack(5)
stack.push(10)
stack.push(20)
stack.push(30)
stack.display()

println("Top element: " + str(stack.peek()))
println("Popped: " + str(stack.pop()))
println("Size: " + str(stack.size()))

# Internal state is hidden and protected
# stack.items[0] = 999        # ✗ ERROR - cannot access private array
# stack.top = 100             # ✗ ERROR - cannot modify private variable
```

### Example: Encapsulated Database Connection

```klang
class DatabaseConnection {
    private let host: string = ""
    private let port: int = 5432
    private let username: string = ""
    private let password: string = ""
    private let database: string = ""
    private let isConnected: bool = false
    private let connectionPool: int = 10
    
    public fn init(h: string, u: string, p: string, db: string) {
        this.host = h
        this.username = u
        this.password = p
        this.database = db
    }
    
    # Private - internal connection logic
    private fn establishConnection() -> bool {
        println("Connecting to " + this.host + ":" + str(this.port))
        println("Database: " + this.database)
        # Actual connection logic would go here
        this.isConnected = true
        return true
    }
    
    # Private - internal disconnection logic
    private fn closeConnection() {
        println("Closing connection to " + this.database)
        this.isConnected = false
    }
    
    # Private - validate credentials
    private fn validateCredentials() -> bool {
        return len(this.username) > 0 && len(this.password) > 0
    }
    
    # Public interface - connect
    public fn connect() -> bool {
        if this.isConnected {
            println("Already connected")
            return true
        }
        
        if !this.validateCredentials() {
            println("Error: Invalid credentials")
            return false
        }
        
        return this.establishConnection()
    }
    
    # Public interface - disconnect
    public fn disconnect() {
        if this.isConnected {
            this.closeConnection()
        }
    }
    
    # Public interface - execute query
    public fn executeQuery(query: string) -> bool {
        if !this.isConnected {
            println("Error: Not connected to database")
            return false
        }
        
        println("Executing query: " + query)
        # Query execution logic
        return true
    }
    
    # Public interface - get connection status
    public fn getStatus() -> string {
        if this.isConnected {
            return "Connected to " + this.database + " at " + this.host
        }
        return "Disconnected"
    }
}

# Usage - clean interface hides complexity
let db = new DatabaseConnection("localhost", "admin", "secret", "myapp")
db.connect()
db.executeQuery("SELECT * FROM users")
println(db.getStatus())
db.disconnect()

# Internal details are hidden
# println(db.password)           # ✗ ERROR - private
# db.isConnected = true          # ✗ ERROR - private
# db.establishConnection()       # ✗ ERROR - private method
```

---

## Access Control Tables

### Comprehensive Access Matrix

| Modifier    | Same Class | Subclass | External Code | Usage |
|------------|-----------|----------|---------------|-------|
| **public** | ✓ Yes | ✓ Yes | ✓ Yes | Public API, interfaces |
| **protected** | ✓ Yes | ✓ Yes | ✗ No | Inheritance support |
| **private** | ✓ Yes | ✗ No | ✗ No | Internal implementation |

### Access by Location

```klang
class Parent {
    public let pubVar: int = 1
    protected let protVar: int = 2
    private let privVar: int = 3
    
    public fn testAccess() {
        println(this.pubVar)     # ✓ OK - same class
        println(this.protVar)    # ✓ OK - same class
        println(this.privVar)    # ✓ OK - same class
    }
}

class Child extends Parent {
    public fn testInheritedAccess() {
        println(this.pubVar)     # ✓ OK - inherited public
        println(this.protVar)    # ✓ OK - inherited protected
        # println(this.privVar)  # ✗ ERROR - private not inherited
    }
}

# External access
let parent = new Parent()
println(parent.pubVar)      # ✓ OK - public
# println(parent.protVar)   # ✗ ERROR - protected
# println(parent.privVar)   # ✗ ERROR - private

let child = new Child()
println(child.pubVar)       # ✓ OK - public
# println(child.protVar)    # ✗ ERROR - protected
# println(child.privVar)    # ✗ ERROR - private
```

### Decision Flow Chart

```
Need to expose member?
    │
    ├── Yes → Will subclasses need it?
    │         │
    │         ├── Yes → Will external code need it?
    │         │         │
    │         │         ├── Yes → Use PUBLIC
    │         │         └── No  → Use PROTECTED
    │         │
    │         └── No  → Use PUBLIC
    │
    └── No  → Use PRIVATE
```

### Common Patterns

| Pattern | Properties | Methods | Reason |
|---------|-----------|---------|--------|
| **Immutable** | private | public getters only | Data integrity |
| **Bean/DTO** | private | public getters/setters | Encapsulation |
| **Service** | private state | public API | Hide implementation |
| **Template** | protected state | protected hooks | Inheritance support |
| **Utility** | private | public static | Stateless operations |

---

## Best Practices

### 1. Start with Most Restrictive

**Always begin with private**, then make members more accessible only when needed:

```klang
# ✓ Good - start private
class GoodPractice {
    private let data: string = ""          # Private by default
    
    public fn getData() -> string {        # Public when needed
        return this.data
    }
}

# ✗ Bad - everything public
class BadPractice {
    public let data: string = ""           # Unnecessarily exposed
    public let internalFlag: bool = false  # Should be private
}
```

### 2. Use Getters/Setters for Validation

**Provide controlled access** with validation:

```klang
class EmailAccount {
    private let email: string = ""
    
    # ✓ Good - validated setter
    public fn setEmail(newEmail: string) {
        if this.isValidEmail(newEmail) {
            this.email = newEmail
        } else {
            println("Error: Invalid email format")
        }
    }
    
    private fn isValidEmail(email: string) -> bool {
        # Simplified validation
        return contains(email, "@") && contains(email, ".")
    }
}

# ✗ Bad - direct access, no validation
class BadEmailAccount {
    public let email: string = ""
    # Anyone can set invalid email!
}
```

### 3. Protect Implementation Details

**Keep implementation private**, expose only interface:

```klang
# ✓ Good - hidden implementation
class GoodCache {
    private let cache: map<string, string> = {}
    private let maxSize: int = 100
    
    public fn get(key: string) -> string {
        return this.cache[key]
    }
    
    public fn set(key: string, value: string) {
        if len(this.cache) < this.maxSize {
            this.cache[key] = value
        }
    }
}

# ✗ Bad - exposed implementation
class BadCache {
    public let cache: map<string, string> = {}  # Can be modified directly!
}
```

### 4. Use Protected for Inheritance

**Protected members** support the inheritance hierarchy:

```klang
class BaseValidator {
    protected let errorMessages: array<string> = []
    
    protected fn addError(message: string) {
        this.errorMessages.push(message)
    }
    
    public fn getErrors() -> array<string> {
        return this.errorMessages
    }
}

class EmailValidator extends BaseValidator {
    public fn validate(email: string) -> bool {
        if !contains(email, "@") {
            this.addError("Email must contain @")  # Use protected method
            return false
        }
        return true
    }
}
```

### 5. Document Access Levels

**Be explicit** about why members have certain access levels:

```klang
class DocumentedClass {
    # Public: Part of the public API
    public let version: string = "1.0"
    
    # Protected: Subclasses may need to override
    protected fn processData() {
        # ...
    }
    
    # Private: Internal implementation detail
    private let cache: map<string, string> = {}
}
```

### 6. Minimize Public Surface

**Expose minimal public API**:

```klang
# ✓ Good - minimal public interface
class MinimalAPI {
    private let data: array<int> = []
    private let sorted: bool = false
    
    # Only expose what's needed
    public fn add(item: int) {
        this.data.push(item)
        this.sorted = false
    }
    
    public fn get(index: int) -> int {
        return this.data[index]
    }
}
```

### 7. Immutability Where Possible

**Read-only properties** prevent unwanted modifications:

```klang
class ImmutableConfig {
    private let configData: map<string, string> = {}
    
    public fn init(data: map<string, string>) {
        this.configData = data
    }
    
    # Only getter, no setter
    public fn get(key: string) -> string {
        return this.configData[key]
    }
    
    # No way to modify config after creation
}
```

---

## Common Mistakes

### Mistake 1: Making Everything Public

```klang
# ✗ WRONG - everything exposed
class BadDesign {
    public let password: string = ""
    public let apiKey: string = ""
    public let internalState: int = 0
    
    public fn helperMethod() {
        # Should be private
    }
}

# ✓ CORRECT - proper encapsulation
class GoodDesign {
    private let password: string = ""
    private let apiKey: string = ""
    private let internalState: int = 0
    
    private fn helperMethod() {
        # Internal helper
    }
    
    public fn authenticate(pass: string) -> bool {
        return pass == this.password
    }
}
```

### Mistake 2: Not Validating Input

```klang
# ✗ WRONG - no validation
class BadAccount {
    public let balance: float = 0.0
    # Anyone can set any value, including negative!
}

# ✓ CORRECT - validated access
class GoodAccount {
    private let balance: float = 0.0
    
    public fn deposit(amount: float) -> bool {
        if amount > 0.0 {
            this.balance = this.balance + amount
            return true
        }
        return false
    }
}
```

### Mistake 3: Using Protected Instead of Private

```klang
# ✗ WRONG - overusing protected
class OverProtected {
    protected let internalCounter: int = 0  # Should be private
    protected fn helperMethod() {           # Should be private
        # Not needed in subclasses
    }
}

# ✓ CORRECT - use private when not needed in subclasses
class WellDesigned {
    private let internalCounter: int = 0
    
    private fn helperMethod() {
        # Only used internally
    }
    
    protected fn extendableMethod() {
        # Subclasses may override
    }
}
```

### Mistake 4: Direct Field Access in Subclasses

```klang
# ✗ PROBLEMATIC - direct field access
class Parent {
    protected let count: int = 0
}

class Child extends Parent {
    public fn increment() {
        this.count = this.count + 1  # Direct access, no validation
    }
}

# ✓ BETTER - use protected methods
class BetterParent {
    private let count: int = 0
    
    protected fn getCount() -> int {
        return this.count
    }
    
    protected fn setCount(newCount: int) {
        if newCount >= 0 {
            this.count = newCount
        }
    }
}

class BetterChild extends BetterParent {
    public fn increment() {
        this.setCount(this.getCount() + 1)  # Controlled access
    }
}
```

---

## Interview Problems

### Problem 1: Design a Secure Password Manager

**Task**: Create a `PasswordManager` class that stores passwords securely with proper access control.

**Requirements**:
- Store multiple username/password pairs
- Passwords should be private and hashed
- Provide methods to add, retrieve, and validate credentials
- Master password to unlock the manager
- Prevent direct access to stored passwords

**Solution**:

```klang
class PasswordManager {
    private let masterPasswordHash: string = ""
    private let credentials: map<string, string> = {}  # username -> password hash
    private let isUnlocked: bool = false
    private let secretSalt: string = "SECURE_SALT_123"
    
    public fn init(masterPassword: string) {
        this.masterPasswordHash = this.hashPassword(masterPassword)
        println("Password manager initialized")
    }
    
    # Private - hash password with salt
    private fn hashPassword(password: string) -> string {
        return "HASH_" + this.secretSalt + "_" + password
    }
    
    # Private - verify master password
    private fn verifyMasterPassword(password: string) -> bool {
        return this.hashPassword(password) == this.masterPasswordHash
    }
    
    # Public - unlock with master password
    public fn unlock(masterPassword: string) -> bool {
        if this.verifyMasterPassword(masterPassword) {
            this.isUnlocked = true
            println("Password manager unlocked")
            return true
        }
        println("Error: Invalid master password")
        return false
    }
    
    # Public - lock the manager
    public fn lock() {
        this.isUnlocked = false
        println("Password manager locked")
    }
    
    # Public - add credential
    public fn addCredential(username: string, password: string) -> bool {
        if !this.isUnlocked {
            println("Error: Manager is locked")
            return false
        }
        
        let passwordHash = this.hashPassword(password)
        this.credentials[username] = passwordHash
        println("Credential added for: " + username)
        return true
    }
    
    # Public - get password (requires unlocked state)
    public fn getPassword(username: string) -> string {
        if !this.isUnlocked {
            println("Error: Manager is locked")
            return ""
        }
        
        if username in this.credentials {
            # Return password (in real system, decrypt here)
            return "***PROTECTED***"  # Don't return actual password
        }
        return ""
    }
    
    # Public - validate credential
    public fn validateCredential(username: string, password: string) -> bool {
        if !this.isUnlocked {
            println("Error: Manager is locked")
            return false
        }
        
        if username in this.credentials {
            let passwordHash = this.hashPassword(password)
            return this.credentials[username] == passwordHash
        }
        return false
    }
    
    # Public - list usernames only (not passwords!)
    public fn listUsernames() -> array<string> {
        if !this.isUnlocked {
            println("Error: Manager is locked")
            return []
        }
        
        let usernames: array<string> = []
        for let key in this.credentials {
            usernames.push(key)
        }
        return usernames
    }
}

# Usage
let manager = new PasswordManager("master123")
manager.unlock("master123")

manager.addCredential("user@example.com", "password123")
manager.addCredential("admin@example.com", "adminPass456")

println("Valid: " + str(manager.validateCredential("user@example.com", "password123")))
println("Invalid: " + str(manager.validateCredential("user@example.com", "wrongpass")))

let users = manager.listUsernames()
println("Stored usernames: " + str(len(users)))

manager.lock()
manager.getPassword("user@example.com")  # Error: locked
```

---

### Problem 2: Implement a Shopping Cart with Access Control

**Task**: Create a `ShoppingCart` class with proper encapsulation.

**Requirements**:
- Private cart items storage
- Public methods to add/remove items
- Protected discount calculation for subclasses
- Cannot modify cart directly
- Calculate totals with tax

**Solution**:

```klang
class CartItem {
    public let productId: string = ""
    public let productName: string = ""
    public let price: float = 0.0
    public let quantity: int = 0
    
    public fn init(id: string, name: string, p: float, qty: int) {
        this.productId = id
        this.productName = name
        this.price = p
        this.quantity = qty
    }
    
    public fn getSubtotal() -> float {
        return this.price * float(this.quantity)
    }
}

class ShoppingCart {
    private let items: array<CartItem> = []
    private let taxRate: float = 0.08  # 8% tax
    protected let discountPercent: float = 0.0
    
    # Private - find item index
    private fn findItemIndex(productId: string) -> int {
        for let i = 0; i < len(this.items); i = i + 1 {
            if this.items[i].productId == productId {
                return i
            }
        }
        return -1
    }
    
    # Protected - calculate discount (subclasses can override)
    protected fn calculateDiscount(subtotal: float) -> float {
        return subtotal * (this.discountPercent / 100.0)
    }
    
    # Protected - calculate tax
    protected fn calculateTax(amount: float) -> float {
        return amount * this.taxRate
    }
    
    # Public - add item
    public fn addItem(productId: string, name: string, price: float, quantity: int) -> bool {
        if price <= 0.0 || quantity <= 0 {
            println("Error: Invalid price or quantity")
            return false
        }
        
        let index = this.findItemIndex(productId)
        if index >= 0 {
            # Item exists, update quantity
            this.items[index].quantity = this.items[index].quantity + quantity
            println("Updated quantity for: " + name)
        } else {
            # New item
            let item = new CartItem(productId, name, price, quantity)
            this.items.push(item)
            println("Added to cart: " + name)
        }
        return true
    }
    
    # Public - remove item
    public fn removeItem(productId: string) -> bool {
        let index = this.findItemIndex(productId)
        if index >= 0 {
            let item = this.items[index]
            println("Removed from cart: " + item.productName)
            this.items.removeAt(index)
            return true
        }
        println("Error: Item not found")
        return false
    }
    
    # Public - get item count
    public fn getItemCount() -> int {
        return len(this.items)
    }
    
    # Public - calculate subtotal
    public fn getSubtotal() -> float {
        let subtotal = 0.0
        for let i = 0; i < len(this.items); i = i + 1 {
            subtotal = subtotal + this.items[i].getSubtotal()
        }
        return subtotal
    }
    
    # Public - get total with tax and discount
    public fn getTotal() -> float {
        let subtotal = this.getSubtotal()
        let discount = this.calculateDiscount(subtotal)
        let afterDiscount = subtotal - discount
        let tax = this.calculateTax(afterDiscount)
        return afterDiscount + tax
    }
    
    # Public - display cart
    public fn displayCart() {
        if len(this.items) == 0 {
            println("Cart is empty")
            return
        }
        
        println("=== Shopping Cart ===")
        for let i = 0; i < len(this.items); i = i + 1 {
            let item = this.items[i]
            println(item.productName + " x" + str(item.quantity) + " = $" + str(item.getSubtotal()))
        }
        
        let subtotal = this.getSubtotal()
        let discount = this.calculateDiscount(subtotal)
        let tax = this.calculateTax(subtotal - discount)
        
        println("--------------------")
        println("Subtotal: $" + str(subtotal))
        if discount > 0.0 {
            println("Discount: -$" + str(discount))
        }
        println("Tax: $" + str(tax))
        println("Total: $" + str(this.getTotal()))
        println("====================")
    }
}

class PremiumShoppingCart extends ShoppingCart {
    private let membershipLevel: int = 1
    
    public fn init(level: int) {
        this.membershipLevel = level
        # Premium members get discount based on level
        if level >= 3 {
            this.discountPercent = 15.0
        } else if level >= 2 {
            this.discountPercent = 10.0
        } else {
            this.discountPercent = 5.0
        }
    }
    
    # Override discount calculation
    protected fn calculateDiscount(subtotal: float) -> float {
        let discount = super.calculateDiscount(subtotal)
        # Bonus discount for large orders
        if subtotal > 1000.0 {
            discount = discount + 50.0
        }
        return discount
    }
}

# Usage
println("=== Regular Cart ===")
let cart = new ShoppingCart()
cart.addItem("P001", "Laptop", 1200.0, 1)
cart.addItem("P002", "Mouse", 25.0, 2)
cart.addItem("P003", "Keyboard", 75.0, 1)
cart.displayCart()

println("\n=== Premium Cart ===")
let premiumCart = new PremiumShoppingCart(3)
premiumCart.addItem("P001", "Laptop", 1200.0, 1)
premiumCart.addItem("P002", "Mouse", 25.0, 2)
premiumCart.displayCart()
```

---

### Problem 3: Bank Account Hierarchy with Access Control

**Task**: Design a banking system with different account types.

**Requirements**:
- Base `Account` class with private balance
- Protected methods for subclasses
- `SavingsAccount` with interest calculation
- `CheckingAccount` with overdraft protection
- Proper access modifiers throughout

**Solution**:

```klang
class Account {
    protected let accountNumber: string = ""
    protected let accountHolder: string = ""
    private let balance: float = 0.0
    private let transactionCount: int = 0
    
    public fn init(accNum: string, holder: string, initialBalance: float) {
        this.accountNumber = accNum
        this.accountHolder = holder
        if initialBalance >= 0.0 {
            this.balance = initialBalance
        }
    }
    
    # Protected - for subclasses to use
    protected fn getBalance() -> float {
        return this.balance
    }
    
    protected fn setBalance(newBalance: float) {
        if newBalance >= 0.0 {
            this.balance = newBalance
        }
    }
    
    protected fn incrementTransactionCount() {
        this.transactionCount = this.transactionCount + 1
    }
    
    # Public API
    public fn deposit(amount: float) -> bool {
        if amount <= 0.0 {
            println("Error: Invalid amount")
            return false
        }
        
        this.balance = this.balance + amount
        this.incrementTransactionCount()
        println("Deposited: $" + str(amount))
        return true
    }
    
    public fn withdraw(amount: float) -> bool {
        if amount <= 0.0 {
            println("Error: Invalid amount")
            return false
        }
        
        if amount > this.balance {
            println("Error: Insufficient funds")
            return false
        }
        
        this.balance = this.balance - amount
        this.incrementTransactionCount()
        println("Withdrawn: $" + str(amount))
        return true
    }
    
    public fn getAccountInfo() -> string {
        return "Account: " + this.accountNumber + ", Holder: " + this.accountHolder
    }
    
    public fn displayBalance() {
        println("Current balance: $" + str(this.balance))
    }
}

class SavingsAccount extends Account {
    private let interestRate: float = 0.02  # 2% annual interest
    private let minimumBalance: float = 100.0
    
    public fn init(accNum: string, holder: string, initialBalance: float, rate: float) {
        super.init(accNum, holder, initialBalance)
        this.interestRate = rate
    }
    
    # Override withdraw with minimum balance check
    public fn withdraw(amount: float) -> bool {
        let currentBalance = this.getBalance()  # Use protected getter
        
        if (currentBalance - amount) < this.minimumBalance {
            println("Error: Cannot go below minimum balance of $" + str(this.minimumBalance))
            return false
        }
        
        # Use parent's withdraw
        return super.withdraw(amount)
    }
    
    # Savings-specific method
    public fn applyInterest() {
        let currentBalance = this.getBalance()
        let interest = currentBalance * this.interestRate
        this.setBalance(currentBalance + interest)  # Use protected setter
        this.incrementTransactionCount()
        println("Interest applied: $" + str(interest))
        println("New balance: $" + str(this.getBalance()))
    }
}

class CheckingAccount extends Account {
    private let overdraftLimit: float = 500.0
    private let transactionFee: float = 0.5
    private let freeTransactions: int = 10
    
    public fn init(accNum: string, holder: string, initialBalance: float, overdraft: float) {
        super.init(accNum, holder, initialBalance)
        this.overdraftLimit = overdraft
    }
    
    # Override withdraw with overdraft
    public fn withdraw(amount: float) -> bool {
        let currentBalance = this.getBalance()
        
        # Check overdraft limit
        if (currentBalance - amount) < -this.overdraftLimit {
            println("Error: Exceeds overdraft limit of $" + str(this.overdraftLimit))
            return false
        }
        
        # Apply transaction fee if over free limit
        let fee = 0.0
        if this.transactionCount >= this.freeTransactions {
            fee = this.transactionFee
            println("Transaction fee: $" + str(fee))
        }
        
        this.setBalance(currentBalance - amount - fee)
        this.incrementTransactionCount()
        println("Withdrawn: $" + str(amount))
        
        if this.getBalance() < 0.0 {
            println("Warning: Account overdrawn by $" + str(-this.getBalance()))
        }
        
        return true
    }
}

# Usage
println("=== Savings Account ===")
let savings = new SavingsAccount("SAV-001", "Alice", 1000.0, 0.03)
savings.displayBalance()
savings.deposit(500.0)
savings.applyInterest()
savings.withdraw(200.0)
savings.displayBalance()

println("\n=== Checking Account ===")
let checking = new CheckingAccount("CHK-001", "Bob", 300.0, 500.0)
checking.displayBalance()
checking.withdraw(400.0)  # Goes into overdraft
checking.displayBalance()
checking.deposit(500.0)
checking.displayBalance()
```

---

### Problem 4: Design a Logging System with Access Levels

**Task**: Create a flexible logging system with different log levels and access control.

**Requirements**:
- Private log storage
- Protected methods for custom loggers
- Public API for logging
- Different log levels (DEBUG, INFO, WARNING, ERROR)
- File and console loggers as subclasses

**Solution**:

```klang
class Logger {
    # Constants for log levels
    public let LEVEL_DEBUG: int = 0
    public let LEVEL_INFO: int = 1
    public let LEVEL_WARNING: int = 2
    public let LEVEL_ERROR: int = 3
    
    private let logs: array<string> = []
    private let currentLevel: int = 1  # Default: INFO
    private let maxLogs: int = 1000
    
    # Protected - for subclasses to access
    protected let loggerName: string = "Logger"
    
    public fn init(name: string) {
        this.loggerName = name
    }
    
    # Private - format log message
    private fn formatLogMessage(level: string, message: string) -> string {
        return "[" + level + "] [" + this.loggerName + "] " + message
    }
    
    # Private - should log based on level
    private fn shouldLog(level: int) -> bool {
        return level >= this.currentLevel
    }
    
    # Private - add to storage
    private fn addToStorage(message: string) {
        if len(this.logs) >= this.maxLogs {
            # Remove oldest log
            this.logs.removeAt(0)
        }
        this.logs.push(message)
    }
    
    # Protected - write log (subclasses override this)
    protected fn writeLog(formattedMessage: string) {
        println(formattedMessage)
    }
    
    # Public - set log level
    public fn setLevel(level: int) {
        if level >= 0 && level <= 3 {
            this.currentLevel = level
            println("Log level set to: " + str(level))
        }
    }
    
    # Public - debug logging
    public fn debug(message: string) {
        if this.shouldLog(this.LEVEL_DEBUG) {
            let formatted = this.formatLogMessage("DEBUG", message)
            this.addToStorage(formatted)
            this.writeLog(formatted)
        }
    }
    
    # Public - info logging
    public fn info(message: string) {
        if this.shouldLog(this.LEVEL_INFO) {
            let formatted = this.formatLogMessage("INFO", message)
            this.addToStorage(formatted)
            this.writeLog(formatted)
        }
    }
    
    # Public - warning logging
    public fn warning(message: string) {
        if this.shouldLog(this.LEVEL_WARNING) {
            let formatted = this.formatLogMessage("WARNING", message)
            this.addToStorage(formatted)
            this.writeLog(formatted)
        }
    }
    
    # Public - error logging
    public fn error(message: string) {
        if this.shouldLog(this.LEVEL_ERROR) {
            let formatted = this.formatLogMessage("ERROR", message)
            this.addToStorage(formatted)
            this.writeLog(formatted)
        }
    }
    
    # Public - get log count
    public fn getLogCount() -> int {
        return len(this.logs)
    }
    
    # Public - get recent logs
    public fn getRecentLogs(count: int) -> array<string> {
        let result: array<string> = []
        let start = len(this.logs) - count
        if start < 0 {
            start = 0
        }
        
        for let i = start; i < len(this.logs); i = i + 1 {
            result.push(this.logs[i])
        }
        return result
    }
}

class FileLogger extends Logger {
    private let filename: string = "app.log"
    
    public fn init(name: string, file: string) {
        super.init(name)
        this.filename = file
    }
    
    # Override to write to file
    protected fn writeLog(formattedMessage: string) {
        # In real implementation, write to file
        println("[FILE: " + this.filename + "] " + formattedMessage)
    }
}

class ColoredConsoleLogger extends Logger {
    # Override to add colors
    protected fn writeLog(formattedMessage: string) {
        # In real implementation, add ANSI color codes
        if contains(formattedMessage, "ERROR") {
            println("🔴 " + formattedMessage)
        } else if contains(formattedMessage, "WARNING") {
            println("🟡 " + formattedMessage)
        } else if contains(formattedMessage, "INFO") {
            println("🔵 " + formattedMessage)
        } else {
            println("⚪ " + formattedMessage)
        }
    }
}

# Usage
println("=== Standard Logger ===")
let logger = new Logger("AppLogger")
logger.setLevel(logger.LEVEL_DEBUG)
logger.debug("This is a debug message")
logger.info("Application started")
logger.warning("Low memory warning")
logger.error("Failed to connect to database")

println("\n=== File Logger ===")
let fileLogger = new FileLogger("FileLogger", "application.log")
fileLogger.info("Logging to file")
fileLogger.error("Critical error occurred")

println("\n=== Colored Console Logger ===")
let colorLogger = new ColoredConsoleLogger("ColorLogger")
colorLogger.info("Colored info message")
colorLogger.warning("Colored warning message")
colorLogger.error("Colored error message")

println("\nTotal logs: " + str(logger.getLogCount()))
```

---

### Problem 5: Implement a Configuration Manager

**Task**: Create a configuration manager with hierarchical access control.

**Requirements**:
- Public read access to config values
- Private storage of sensitive configs
- Protected methods for loading/saving
- Validation when setting values
- Support for different config sources (file, environment)

**Solution**:

```klang
class ConfigManager {
    # Private - configuration storage
    private let config: map<string, string> = {}
    private let sensitiveKeys: array<string> = []
    private let immutableKeys: array<string> = []
    
    # Protected - for subclasses
    protected let configSource: string = "default"
    
    public fn init() {
        this.setupSensitiveKeys()
        this.loadDefaults()
    }
    
    # Private - setup sensitive keys list
    private fn setupSensitiveKeys() {
        this.sensitiveKeys = ["password", "api_key", "secret", "token"]
    }
    
    # Private - check if key is sensitive
    private fn isSensitiveKey(key: string) -> bool {
        for let i = 0; i < len(this.sensitiveKeys); i = i + 1 {
            if contains(key, this.sensitiveKeys[i]) {
                return true
            }
        }
        return false
    }
    
    # Private - check if key is immutable
    private fn isImmutableKey(key: string) -> bool {
        for let i = 0; i < len(this.immutableKeys); i = i + 1 {
            if this.immutableKeys[i] == key {
                return true
            }
        }
        return false
    }
    
    # Protected - load defaults (subclasses can override)
    protected fn loadDefaults() {
        this.config["app_name"] = "MyApp"
        this.config["version"] = "1.0.0"
        this.config["debug"] = "false"
        this.immutableKeys = ["version"]
    }
    
    # Protected - validate value
    protected fn validateValue(key: string, value: string) -> bool {
        # Add validation logic
        if len(value) == 0 {
            return false
        }
        return true
    }
    
    # Public - get config value
    public fn get(key: string) -> string {
        if key in this.config {
            # Hide sensitive values
            if this.isSensitiveKey(key) {
                return "***HIDDEN***"
            }
            return this.config[key]
        }
        return ""
    }
    
    # Public - get config value (with authentication for sensitive)
    public fn getSecure(key: string, authToken: string) -> string {
        # In real system, validate authToken
        if this.verifyAuth(authToken) {
            if key in this.config {
                return this.config[key]
            }
        }
        return ""
    }
    
    # Private - verify authentication
    private fn verifyAuth(token: string) -> bool {
        # Simplified auth check
        return len(token) > 0
    }
    
    # Public - set config value
    public fn set(key: string, value: string) -> bool {
        if this.isImmutableKey(key) {
            println("Error: Cannot modify immutable key: " + key)
            return false
        }
        
        if !this.validateValue(key, value) {
            println("Error: Invalid value for key: " + key)
            return false
        }
        
        this.config[key] = value
        println("Config updated: " + key)
        return true
    }
    
    # Public - check if key exists
    public fn has(key: string) -> bool {
        return key in this.config
    }
    
    # Public - get all keys (hide sensitive)
    public fn getKeys() -> array<string> {
        let keys: array<string> = []
        for let key in this.config {
            if !this.isSensitiveKey(key) {
                keys.push(key)
            }
        }
        return keys
    }
    
    # Public - display configuration
    public fn displayConfig() {
        println("=== Configuration ===")
        println("Source: " + this.configSource)
        for let key in this.config {
            let value = this.get(key)  # Uses public get (hides sensitive)
            println(key + " = " + value)
        }
        println("====================")
    }
}

class FileConfigManager extends ConfigManager {
    private let configFile: string = ""
    
    public fn init(filename: string) {
        this.configFile = filename
        this.configSource = "file:" + filename
        super.init()
        this.loadFromFile()
    }
    
    # Override to load from file
    protected fn loadDefaults() {
        super.loadDefaults()
        # Additional file-based defaults
    }
    
    # Private - load from file
    private fn loadFromFile() {
        println("Loading config from: " + this.configFile)
        # In real implementation, read from file
        this.set("loaded_from", this.configFile)
    }
    
    # Public - save to file
    public fn save() -> bool {
        println("Saving config to: " + this.configFile)
        # In real implementation, write to file
        return true
    }
}

class EnvironmentConfigManager extends ConfigManager {
    public fn init() {
        this.configSource = "environment"
        super.init()
        this.loadFromEnvironment()
    }
    
    # Private - load from environment
    private fn loadFromEnvironment() {
        println("Loading config from environment variables")
        # In real implementation, read from env vars
        this.set("env", "production")
    }
}

# Usage
println("=== Basic Config Manager ===")
let config = new ConfigManager()
config.set("database_url", "localhost:5432")
config.set("max_connections", "100")
println("Database URL: " + config.get("database_url"))
config.displayConfig()

println("\n=== File Config Manager ===")
let fileConfig = new FileConfigManager("config.ini")
fileConfig.set("api_key", "secret_key_123")
println("API Key (public): " + fileConfig.get("api_key"))
println("API Key (secure): " + fileConfig.getSecure("api_key", "auth_token"))
fileConfig.save()

println("\n=== Environment Config Manager ===")
let envConfig = new EnvironmentConfigManager()
envConfig.displayConfig()
```

---

## Summary

Access modifiers are fundamental to writing secure, maintainable object-oriented code in KLang:

### Key Takeaways

1. **Public**: Use for the interface users interact with
2. **Protected**: Use for inheritance hierarchy support
3. **Private**: Use for internal implementation details (default choice)

4. **Always start with private** and only expose what's necessary
5. **Use getters/setters** for controlled, validated access
6. **Hide implementation details** to enable future changes
7. **Document access decisions** for team clarity

### The Golden Rule

> "Make everything as private as possible, and only as public as necessary"

By properly using access modifiers, you create code that is:
- ✓ **Secure** - Sensitive data is protected
- ✓ **Maintainable** - Internal changes don't break external code
- ✓ **Testable** - Clear interfaces for mocking
- ✓ **Understandable** - Clear boundaries and responsibilities

---

**Next Steps**: Practice implementing classes with proper access control. Try refactoring existing code to use appropriate access modifiers. Remember: good encapsulation is the foundation of robust software design!
