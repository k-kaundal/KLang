# Encapsulation in KLang

## Table of Contents
1. [Introduction](#introduction)
2. [What is Encapsulation?](#what-is-encapsulation)
3. [Data Hiding](#data-hiding)
4. [Getters and Setters](#getters-and-setters)
5. [Access Modifiers Role](#access-modifiers-role)
6. [Benefits of Encapsulation](#benefits-of-encapsulation)
7. [Practical Examples](#practical-examples)
8. [Information Hiding](#information-hiding)
9. [Validation in Setters](#validation-in-setters)
10. [Best Practices](#best-practices)
11. [Common Pitfalls](#common-pitfalls)
12. [Interview Problems](#interview-problems)

---

## Introduction

**Encapsulation** is the **first pillar of Object-Oriented Programming** and one of its most fundamental concepts. It is the mechanism of **bundling data (properties) and methods that operate on that data within a single unit (class)**, while **restricting direct access** to some of the object's components.

### Why Encapsulation?

- **Data Protection**: Shield internal state from unauthorized modification
- **Security**: Prevent exposure of sensitive information
- **Maintainability**: Change implementation without affecting external code
- **Data Integrity**: Ensure data remains in valid state through validation
- **Flexibility**: Modify internal representation without breaking client code
- **Abstraction**: Hide complexity and expose only necessary interfaces

### The Core Principle

```
┌─────────────────────────────────────────────────┐
│  PUBLIC INTERFACE (What users see)             │
│  ┌───────────────────────────────────────────┐ │
│  │  public methods                           │ │
│  │  - getBalance()                           │ │
│  │  - deposit(amount)                        │ │
│  │  - withdraw(amount)                       │ │
│  └───────────────────────────────────────────┘ │
│                                                 │
│  ┌───────────────────────────────────────────┐ │
│  │  PRIVATE IMPLEMENTATION (Hidden)          │ │
│  │  - balance (private)                      │ │
│  │  - transactionHistory (private)           │ │
│  │  - validateAmount() (private)             │ │
│  │  - logTransaction() (private)             │ │
│  └───────────────────────────────────────────┘ │
└─────────────────────────────────────────────────┘
```

### Key Concept

**Encapsulation = Data Hiding + Abstraction**

- **Data Hiding**: Keep internal state private
- **Abstraction**: Expose only necessary public methods
- **Controlled Access**: Use getters/setters for validation

---

## What is Encapsulation?

Encapsulation is the practice of **wrapping data and methods together** and **controlling access** to them. It ensures that:

1. **Internal state is hidden** from outside access
2. **Data can only be modified through defined methods**
3. **Validation and constraints are enforced**
4. **Implementation details remain private**

### The Encapsulation Formula

```
Encapsulation = Private Data + Public Interface + Validation
```

### Simple Example

```klang
# ✗ WITHOUT Encapsulation (bad practice)
class BankAccount {
    public let balance: float = 0.0  # Anyone can modify!
}

let account = new BankAccount()
account.balance = -1000.0  # ✗ Invalid state! Negative balance!
account.balance = 999999.0  # ✗ Unauthorized modification!

# ✓ WITH Encapsulation (best practice)
class SecureBankAccount {
    private let balance: float = 0.0  # Hidden from outside
    
    public fn init(initialBalance: float) {
        if initialBalance >= 0.0 {
            this.balance = initialBalance
        }
    }
    
    # Controlled read access
    public fn getBalance() -> float {
        return this.balance
    }
    
    # Controlled write access with validation
    public fn deposit(amount: float) -> bool {
        if amount > 0.0 {
            this.balance = this.balance + amount
            return true
        }
        println("Error: Deposit amount must be positive")
        return false
    }
    
    public fn withdraw(amount: float) -> bool {
        if amount > 0.0 and amount <= this.balance {
            this.balance = this.balance - amount
            return true
        }
        println("Error: Invalid withdrawal amount")
        return false
    }
}

let secureAccount = new SecureBankAccount(1000.0)
# secureAccount.balance = -500.0  # ✗ COMPILE ERROR - private member
secureAccount.deposit(500.0)       # ✓ OK - controlled access
println(secureAccount.getBalance())  # ✓ OK - read-only access
```

### Why This Matters

Without encapsulation:
- ❌ Data can be corrupted
- ❌ Invalid states are possible
- ❌ Security vulnerabilities exist
- ❌ No validation or constraints
- ❌ Difficult to maintain

With encapsulation:
- ✓ Data integrity is maintained
- ✓ Only valid states are possible
- ✓ Security is enforced
- ✓ Validation at every access point
- ✓ Easy to change implementation

---

## Data Hiding

**Data Hiding** is the practice of keeping class data (properties) **private** and providing **controlled access** through public methods.

### The Data Hiding Principle

```
┌──────────────────────────────────────┐
│  Class: Account                      │
│                                      │
│  HIDDEN (Private):                   │
│  ┌────────────────────────────────┐ │
│  │ - balance                      │ │
│  │ - accountNumber                │ │
│  │ - pin                          │ │
│  │ - transactionLog               │ │
│  └────────────────────────────────┘ │
│                                      │
│  VISIBLE (Public):                   │
│  ┌────────────────────────────────┐ │
│  │ + deposit()                    │ │
│  │ + withdraw()                   │ │
│  │ + getBalance()                 │ │
│  │ + transfer()                   │ │
│  └────────────────────────────────┘ │
└──────────────────────────────────────┘
```

### Why Hide Data?

1. **Prevent Unauthorized Modification**
2. **Maintain Data Integrity**
3. **Enable Validation**
4. **Protect Sensitive Information**
5. **Allow Implementation Changes**

### Data Hiding Example

```klang
class CreditCard {
    # Hidden data - cannot be accessed directly
    private let cardNumber: string = ""
    private let cvv: string = ""
    private let pin: string = ""
    private let balance: float = 0.0
    private let creditLimit: float = 5000.0
    private let isLocked: bool = false
    
    public fn init(cardNum: string, cvvCode: string, pinCode: string) {
        this.cardNumber = cardNum
        this.cvv = cvvCode
        this.pin = pinCode
    }
    
    # Provide only masked card number (security)
    public fn getMaskedCardNumber() -> string {
        let length = len(this.cardNumber)
        if length < 4 {
            return "****"
        }
        let lastFour = substr(this.cardNumber, length - 4, 4)
        return "************" + lastFour
    }
    
    # CVV and PIN are NEVER exposed
    # No getter methods for cvv or pin!
    
    # Controlled access to balance
    public fn getAvailableCredit() -> float {
        return this.creditLimit - this.balance
    }
    
    # Validate pin without exposing it
    public fn validatePin(inputPin: string) -> bool {
        return inputPin == this.pin
    }
    
    # Make purchase with validation
    public fn makePurchase(amount: float, inputPin: string) -> bool {
        if this.isLocked {
            println("Error: Card is locked")
            return false
        }
        
        if !this.validatePin(inputPin) {
            println("Error: Invalid PIN")
            this.lockCard()  # Security measure
            return false
        }
        
        if amount > this.getAvailableCredit() {
            println("Error: Insufficient credit")
            return false
        }
        
        this.balance = this.balance + amount
        return true
    }
    
    private fn lockCard() {
        this.isLocked = true
        println("Card locked due to security concern")
    }
}

# Usage
let card = new CreditCard("1234567890123456", "123", "4321")
println(card.getMaskedCardNumber())  # ✓ "************3456"
# println(card.cardNumber)           # ✗ COMPILE ERROR - private
# println(card.cvv)                  # ✗ COMPILE ERROR - private
# println(card.pin)                  # ✗ COMPILE ERROR - private
card.makePurchase(100.0, "4321")     # ✓ Controlled access
```

### Protected Data

Data hiding doesn't mean **no access** - it means **controlled access**:

```klang
class Employee {
    private let salary: float = 0.0
    private let bonus: float = 0.0
    
    # Don't expose raw salary - provide calculated values
    public fn getAnnualCompensation() -> float {
        return (this.salary + this.bonus) * 12.0
    }
    
    # Allow modification only through validated methods
    public fn giveRaise(percentage: float) -> bool {
        if percentage > 0.0 and percentage <= 0.20 {  # Max 20% raise
            this.salary = this.salary * (1.0 + percentage)
            return true
        }
        return false
    }
}
```

---

## Getters and Setters

**Getters and Setters** are public methods that provide **controlled access** to private data. They are the primary mechanism for implementing encapsulation.

### The Getter/Setter Pattern

```
┌─────────────────────────────────────────┐
│  Private Data                           │
│  ┌───────────────────────────────────┐ │
│  │ private let value: type           │ │
│  └───────────────────────────────────┘ │
│           ↑                    ↑        │
│           │                    │        │
│       READ ACCESS         WRITE ACCESS  │
│           │                    │        │
│           ↓                    ↓        │
│  ┌─────────────────┐  ┌──────────────┐ │
│  │ getter()        │  │ setter()     │ │
│  │ return value    │  │ validate     │ │
│  │                 │  │ set value    │ │
│  └─────────────────┘  └──────────────┘ │
└─────────────────────────────────────────┘
```

### Basic Getter/Setter Example

```klang
class Person {
    private let name: string = ""
    private let age: int = 0
    
    # Getter - provides read access
    public fn getName() -> string {
        return this.name
    }
    
    # Setter - provides controlled write access
    public fn setName(newName: string) {
        if len(newName) > 0 {
            this.name = newName
        } else {
            println("Error: Name cannot be empty")
        }
    }
    
    # Getter with transformation
    public fn getAge() -> int {
        return this.age
    }
    
    # Setter with validation
    public fn setAge(newAge: int) {
        if newAge >= 0 and newAge <= 150 {
            this.age = newAge
        } else {
            println("Error: Invalid age")
        }
    }
}

let person = new Person()
person.setName("Alice")       # ✓ OK - valid name
person.setName("")            # ✗ Rejected - empty name
person.setAge(30)             # ✓ OK - valid age
person.setAge(-5)             # ✗ Rejected - invalid age
println(person.getName())     # ✓ Read access through getter
```

### Read-Only Properties

Sometimes you want to allow **reading** but not **writing**:

```klang
class Product {
    private let id: string = ""
    private let createdAt: string = ""
    private let price: float = 0.0
    
    public fn init(productId: string) {
        this.id = productId
        this.createdAt = "2024-01-01"  # Timestamp
    }
    
    # Getter - read-only access to ID
    public fn getId() -> string {
        return this.id
    }
    
    # No setter for ID - it's immutable!
    
    # Getter - read-only access to creation date
    public fn getCreatedAt() -> string {
        return this.createdAt
    }
    
    # No setter for createdAt - it's immutable!
    
    # Getter and Setter for price (mutable)
    public fn getPrice() -> float {
        return this.price
    }
    
    public fn setPrice(newPrice: float) {
        if newPrice > 0.0 {
            this.price = newPrice
        }
    }
}
```

### Computed Properties

Getters can **compute values** instead of just returning stored data:

```klang
class Rectangle {
    private let width: float = 0.0
    private let height: float = 0.0
    
    public fn init(w: float, h: float) {
        this.width = w
        this.height = h
    }
    
    # Computed property - not stored
    public fn getArea() -> float {
        return this.width * this.height
    }
    
    # Computed property
    public fn getPerimeter() -> float {
        return 2.0 * (this.width + this.height)
    }
    
    # Computed property with logic
    public fn isSquare() -> bool {
        return this.width == this.height
    }
}
```

### Setters with Side Effects

Setters can do more than just set a value:

```klang
class TemperatureController {
    private let currentTemp: float = 20.0
    private let targetTemp: float = 20.0
    private let isHeating: bool = false
    private let isCooling: bool = false
    
    public fn getTargetTemp() -> float {
        return this.targetTemp
    }
    
    # Setter with side effects
    public fn setTargetTemp(newTemp: float) {
        if newTemp < 10.0 or newTemp > 30.0 {
            println("Error: Temperature out of range (10-30°C)")
            return
        }
        
        this.targetTemp = newTemp
        
        # Side effects - update heating/cooling state
        if newTemp > this.currentTemp {
            this.isHeating = true
            this.isCooling = false
            println("Heating activated")
        } else if newTemp < this.currentTemp {
            this.isHeating = false
            this.isCooling = true
            println("Cooling activated")
        } else {
            this.isHeating = false
            this.isCooling = false
            println("Temperature optimal")
        }
    }
    
    public fn getCurrentTemp() -> float {
        return this.currentTemp
    }
}
```

### Method Chaining with Setters

Setters can return `this` for fluent interfaces:

```klang
class UserProfile {
    private let username: string = ""
    private let email: string = ""
    private let bio: string = ""
    
    public fn setUsername(name: string) -> UserProfile {
        if len(name) >= 3 {
            this.username = name
        }
        return this
    }
    
    public fn setEmail(mail: string) -> UserProfile {
        if "@" in mail {
            this.email = mail
        }
        return this
    }
    
    public fn setBio(description: string) -> UserProfile {
        this.bio = description
        return this
    }
}

# Method chaining
let profile = new UserProfile()
profile.setUsername("alice")
       .setEmail("alice@example.com")
       .setBio("Software developer")
```

---

## Access Modifiers Role

Access modifiers are the **enforcement mechanism** for encapsulation. They determine **who** can access **what**.

### The Three Levels of Access

```
PUBLIC          │ PROTECTED       │ PRIVATE
────────────────┼─────────────────┼──────────────────
Everywhere      │ Class +         │ Class only
                │ Subclasses      │
────────────────┼─────────────────┼──────────────────
Maximum         │ Moderate        │ Minimum
visibility      │ visibility      │ visibility
────────────────┼─────────────────┼──────────────────
Interface       │ Extension       │ Implementation
members         │ points          │ details
```

### Access Modifier Strategy

```klang
class BankAccount {
    # PRIVATE - internal state (hidden)
    private let accountNumber: string = ""
    private let balance: float = 0.0
    private let transactionLog: array<string> = []
    
    # PROTECTED - for subclasses to extend
    protected let accountType: string = "STANDARD"
    
    # PUBLIC - the interface (visible)
    public fn init(accNum: string, initialBalance: float) {
        this.accountNumber = accNum
        if initialBalance >= 0.0 {
            this.balance = initialBalance
        }
    }
    
    # PUBLIC - main operations
    public fn deposit(amount: float) -> bool {
        if this.validateAmount(amount) {
            this.balance = this.balance + amount
            this.logTransaction("DEPOSIT: " + str(amount))
            return true
        }
        return false
    }
    
    public fn withdraw(amount: float) -> bool {
        if this.validateAmount(amount) and amount <= this.balance {
            this.balance = this.balance - amount
            this.logTransaction("WITHDRAW: " + str(amount))
            return true
        }
        return false
    }
    
    public fn getBalance() -> float {
        return this.balance
    }
    
    # PROTECTED - for subclasses to use
    protected fn getAccountType() -> string {
        return this.accountType
    }
    
    protected fn applyInterest(rate: float) {
        let interest = this.balance * rate
        this.balance = this.balance + interest
    }
    
    # PRIVATE - implementation details
    private fn validateAmount(amount: float) -> bool {
        return amount > 0.0
    }
    
    private fn logTransaction(transaction: string) {
        this.transactionLog.append(transaction)
    }
}

# Subclass can access protected members
class SavingsAccount extends BankAccount {
    private let interestRate: float = 0.05
    
    public fn init(accNum: string, balance: float) {
        super.init(accNum, balance)
        this.accountType = "SAVINGS"  # ✓ Protected member
    }
    
    public fn addMonthlyInterest() {
        this.applyInterest(this.interestRate)  # ✓ Protected method
    }
}

# External code can only access public members
let account = new SavingsAccount("ACC123", 1000.0)
account.deposit(500.0)           # ✓ Public method
println(account.getBalance())    # ✓ Public method
# account.balance = 999999.0     # ✗ ERROR - private
# account.accountType = "FRAUD"  # ✗ ERROR - protected
# account.logTransaction("X")    # ✗ ERROR - private
```

### Choosing the Right Access Level

| Use Case | Access Modifier | Example |
|----------|----------------|---------|
| Public API | `public` | `deposit()`, `getBalance()` |
| Extension points | `protected` | `calculateFee()`, `validate()` |
| Internal state | `private` | `balance`, `password` |
| Helper methods | `private` | `formatDate()`, `hash()` |
| Factory methods | `public` | `create()`, `fromJSON()` |
| Template methods | `protected` | `beforeSave()`, `afterLoad()` |

### The Principle of Least Privilege

**Always use the most restrictive access level possible:**

1. **Default to Private** - Make everything private first
2. **Expose Selectively** - Make public only what's necessary
3. **Use Protected Sparingly** - Only for inheritance needs
4. **Review Regularly** - Can you make something more restrictive?

```klang
class SecureSystem {
    # Start with everything private
    private let secretKey: string = ""
    private let userData: array<string> = []
    
    # Make public only what users need
    public fn authenticate(key: string) -> bool {
        return this.verify(key)
    }
    
    # Keep implementation private
    private fn verify(key: string) -> bool {
        return key == this.secretKey
    }
}
```

---

## Benefits of Encapsulation

### 1. Data Integrity

Encapsulation ensures data remains in a **valid state** at all times.

```klang
class BankAccount {
    private let balance: float = 0.0
    
    # Without encapsulation: balance could be negative!
    # With encapsulation: validation ensures valid state
    
    public fn withdraw(amount: float) -> bool {
        if amount > 0.0 and amount <= this.balance {
            this.balance = this.balance - amount
            return true
        }
        println("Error: Invalid withdrawal")
        return false  # Balance remains valid!
    }
}
```

**Benefits:**
- ✓ No invalid states possible
- ✓ Invariants are maintained
- ✓ Business rules are enforced
- ✓ Data consistency guaranteed

### 2. Flexibility

Change **internal implementation** without breaking **external code**.

```klang
# Version 1: Store temperature in Celsius
class Thermometer {
    private let tempCelsius: float = 0.0
    
    public fn getTemperature() -> float {
        return this.tempCelsius
    }
    
    public fn setTemperature(temp: float) {
        this.tempCelsius = temp
    }
}

# Version 2: Changed to Fahrenheit internally
# Public interface remains the same!
class ThermometerV2 {
    private let tempFahrenheit: float = 32.0  # Changed implementation
    
    # Same public interface
    public fn getTemperature() -> float {
        # Convert to Celsius for backward compatibility
        return (this.tempFahrenheit - 32.0) * 5.0 / 9.0
    }
    
    public fn setTemperature(temp: float) {
        # Convert from Celsius to Fahrenheit
        this.tempFahrenheit = temp * 9.0 / 5.0 + 32.0
    }
}

# Client code doesn't change!
let thermo = new ThermometerV2()
thermo.setTemperature(25.0)  # Still works!
println(thermo.getTemperature())  # Still works!
```

**Benefits:**
- ✓ Refactor without breaking changes
- ✓ Optimize internals freely
- ✓ Change data structures
- ✓ Add caching transparently

### 3. Maintainability

Encapsulation makes code **easier to understand and modify**.

```klang
# Easy to understand: clear public interface
class ShoppingCart {
    private let items: array<string> = []
    private let total: float = 0.0
    
    # Clear interface - easy to use
    public fn addItem(item: string, price: float) {
        this.items.append(item)
        this.total = this.total + price
    }
    
    public fn getTotal() -> float {
        return this.total
    }
    
    public fn getItemCount() -> int {
        return len(this.items)
    }
}

# If we need to add discount logic:
# - Modify private calculateTotal() method
# - Public interface stays the same
# - No client code breaks
```

**Benefits:**
- ✓ Changes are localized
- ✓ Reduced coupling
- ✓ Easier debugging
- ✓ Better testability

### 4. Security

Protect sensitive data from unauthorized access.

```klang
class UserAccount {
    private let username: string = ""
    private let passwordHash: string = ""
    private let email: string = ""
    private let loginAttempts: int = 0
    private let isLocked: bool = false
    
    # Password never exposed!
    # Only validation is allowed
    public fn authenticate(password: string) -> bool {
        if this.isLocked {
            println("Account locked")
            return false
        }
        
        if this.hash(password) == this.passwordHash {
            this.loginAttempts = 0
            return true
        }
        
        this.loginAttempts = this.loginAttempts + 1
        if this.loginAttempts >= 3 {
            this.isLocked = true
            println("Account locked due to failed attempts")
        }
        return false
    }
    
    private fn hash(password: string) -> string {
        return "HASH_" + password  # Simplified
    }
}
```

**Benefits:**
- ✓ Sensitive data hidden
- ✓ No direct manipulation
- ✓ Security policies enforced
- ✓ Audit trails possible

### Benefits Summary

| Benefit | Without Encapsulation | With Encapsulation |
|---------|----------------------|-------------------|
| **Data Integrity** | Invalid states possible | Always valid |
| **Flexibility** | Hard to change internals | Easy to refactor |
| **Maintainability** | Scattered logic | Centralized control |
| **Security** | Direct data access | Controlled access |
| **Coupling** | Tight coupling | Loose coupling |
| **Testing** | Hard to test | Easy to mock |

---

## Practical Examples

### Example 1: Bank Account (Protected Balance)

A comprehensive bank account with full encapsulation and security.

```klang
class BankAccount {
    # Private data - protected from external access
    private let accountNumber: string = ""
    private let accountHolder: string = ""
    private let balance: float = 0.0
    private let transactionHistory: array<string> = []
    private let pin: string = ""
    private let isActive: bool = true
    private let minimumBalance: float = 100.0
    
    public fn init(accNum: string, holder: string, pinCode: string, initialDeposit: float) {
        this.accountNumber = accNum
        this.accountHolder = holder
        this.pin = pinCode
        
        if initialDeposit >= this.minimumBalance {
            this.balance = initialDeposit
            this.logTransaction("INITIAL DEPOSIT: $" + str(initialDeposit))
        } else {
            println("Error: Initial deposit must be at least $" + str(this.minimumBalance))
        }
    }
    
    # Read-only access to account number
    public fn getAccountNumber() -> string {
        return this.accountNumber
    }
    
    # Read-only access to account holder
    public fn getAccountHolder() -> string {
        return this.accountHolder
    }
    
    # Controlled access to balance (requires PIN)
    public fn getBalance(inputPin: string) -> float {
        if !this.validatePin(inputPin) {
            println("Error: Invalid PIN")
            return -1.0
        }
        return this.balance
    }
    
    # Deposit with validation
    public fn deposit(amount: float) -> bool {
        if !this.isActive {
            println("Error: Account is inactive")
            return false
        }
        
        if amount <= 0.0 {
            println("Error: Deposit amount must be positive")
            return false
        }
        
        if amount > 50000.0 {
            println("Warning: Large deposit - verification required")
            return false
        }
        
        this.balance = this.balance + amount
        this.logTransaction("DEPOSIT: $" + str(amount))
        println("Deposited: $" + str(amount))
        return true
    }
    
    # Withdrawal with validation and PIN
    public fn withdraw(amount: float, inputPin: string) -> bool {
        if !this.isActive {
            println("Error: Account is inactive")
            return false
        }
        
        if !this.validatePin(inputPin) {
            println("Error: Invalid PIN")
            return false
        }
        
        if amount <= 0.0 {
            println("Error: Withdrawal amount must be positive")
            return false
        }
        
        let newBalance = this.balance - amount
        if newBalance < this.minimumBalance {
            println("Error: Minimum balance requirement not met")
            return false
        }
        
        this.balance = newBalance
        this.logTransaction("WITHDRAWAL: $" + str(amount))
        println("Withdrawn: $" + str(amount))
        return true
    }
    
    # Transfer to another account
    public fn transfer(toAccount: BankAccount, amount: float, inputPin: string) -> bool {
        if !this.validatePin(inputPin) {
            println("Error: Invalid PIN")
            return false
        }
        
        # Use withdraw for validation
        if this.withdraw(amount, inputPin) {
            if toAccount.deposit(amount) {
                this.logTransaction("TRANSFER OUT: $" + str(amount) + " to " + toAccount.getAccountNumber())
                return true
            } else {
                # Rollback if deposit fails
                this.balance = this.balance + amount
                return false
            }
        }
        return false
    }
    
    # Change PIN (requires old PIN)
    public fn changePin(oldPin: string, newPin: string) -> bool {
        if !this.validatePin(oldPin) {
            println("Error: Invalid current PIN")
            return false
        }
        
        if len(newPin) != 4 {
            println("Error: PIN must be 4 digits")
            return false
        }
        
        this.pin = newPin
        this.logTransaction("PIN CHANGED")
        println("PIN updated successfully")
        return true
    }
    
    # Get transaction history (requires PIN)
    public fn getTransactionHistory(inputPin: string) -> array<string> {
        if !this.validatePin(inputPin) {
            println("Error: Invalid PIN")
            return []
        }
        return this.transactionHistory
    }
    
    # Private helper methods
    private fn validatePin(inputPin: string) -> bool {
        return inputPin == this.pin
    }
    
    private fn logTransaction(transaction: string) {
        let timestamp = "2024-01-01 12:00:00"  # Simplified
        this.transactionHistory.append(timestamp + " - " + transaction)
    }
}

# Usage example
let account1 = new BankAccount("ACC001", "Alice", "1234", 5000.0)
let account2 = new BankAccount("ACC002", "Bob", "5678", 3000.0)

# Valid operations
account1.deposit(500.0)                    # ✓ Deposit successful
println(account1.getBalance("1234"))       # ✓ Shows balance
account1.withdraw(200.0, "1234")           # ✓ Withdrawal successful
account1.transfer(account2, 100.0, "1234") # ✓ Transfer successful

# Invalid operations (protected)
# account1.balance = 999999.0              # ✗ COMPILE ERROR
# println(account1.pin)                    # ✗ COMPILE ERROR
println(account1.getBalance("0000"))       # ✗ Wrong PIN
account1.withdraw(10000.0, "1234")         # ✗ Insufficient funds
```

### Example 2: User Profile (Validated Data)

User profile with comprehensive data validation.

```klang
class UserProfile {
    # Private data with strict validation
    private let userId: string = ""
    private let username: string = ""
    private let email: string = ""
    private let age: int = 0
    private let phoneNumber: string = ""
    private let isVerified: bool = false
    private let createdAt: string = ""
    private let bio: string = ""
    
    public fn init(id: string) {
        this.userId = id
        this.createdAt = "2024-01-01"  # Timestamp
    }
    
    # Read-only user ID
    public fn getUserId() -> string {
        return this.userId
    }
    
    # Username with validation
    public fn getUsername() -> string {
        return this.username
    }
    
    public fn setUsername(name: string) -> bool {
        # Validation rules
        if len(name) < 3 {
            println("Error: Username must be at least 3 characters")
            return false
        }
        
        if len(name) > 20 {
            println("Error: Username cannot exceed 20 characters")
            return false
        }
        
        # Check for valid characters (simplified)
        if " " in name {
            println("Error: Username cannot contain spaces")
            return false
        }
        
        this.username = name
        return true
    }
    
    # Email with validation
    public fn getEmail() -> string {
        return this.email
    }
    
    public fn setEmail(mail: string) -> bool {
        if !this.isValidEmail(mail) {
            println("Error: Invalid email format")
            return false
        }
        
        this.email = mail
        this.isVerified = false  # Require re-verification
        println("Email updated - verification required")
        return true
    }
    
    # Age with validation
    public fn getAge() -> int {
        return this.age
    }
    
    public fn setAge(years: int) -> bool {
        if years < 13 {
            println("Error: Must be at least 13 years old")
            return false
        }
        
        if years > 120 {
            println("Error: Invalid age")
            return false
        }
        
        this.age = years
        return true
    }
    
    # Phone with validation
    public fn getPhoneNumber() -> string {
        # Return masked number for privacy
        if len(this.phoneNumber) > 4 {
            let lastFour = substr(this.phoneNumber, len(this.phoneNumber) - 4, 4)
            return "******" + lastFour
        }
        return "****"
    }
    
    public fn setPhoneNumber(phone: string) -> bool {
        if !this.isValidPhone(phone) {
            println("Error: Invalid phone number format")
            return false
        }
        
        this.phoneNumber = phone
        return true
    }
    
    # Bio with validation
    public fn getBio() -> string {
        return this.bio
    }
    
    public fn setBio(description: string) -> bool {
        if len(description) > 500 {
            println("Error: Bio cannot exceed 500 characters")
            return false
        }
        
        this.bio = description
        return true
    }
    
    # Verification status (read-only)
    public fn isEmailVerified() -> bool {
        return this.isVerified
    }
    
    # Get creation date (read-only)
    public fn getCreatedAt() -> string {
        return this.createdAt
    }
    
    # Get profile summary
    public fn getSummary() -> string {
        let summary = "User: " + this.username + "\n"
        summary = summary + "Email: " + this.email + "\n"
        summary = summary + "Age: " + str(this.age) + "\n"
        summary = summary + "Verified: " + str(this.isVerified)
        return summary
    }
    
    # Private validation methods
    private fn isValidEmail(email: string) -> bool {
        # Simplified email validation
        return "@" in email and "." in email
    }
    
    private fn isValidPhone(phone: string) -> bool {
        # Simplified phone validation
        return len(phone) >= 10 and len(phone) <= 15
    }
}

# Usage
let profile = new UserProfile("USER123")

# Valid operations
profile.setUsername("alice_dev")           # ✓ Valid username
profile.setEmail("alice@example.com")      # ✓ Valid email
profile.setAge(25)                         # ✓ Valid age
profile.setBio("Software developer")       # ✓ Valid bio

# Invalid operations (validation fails)
profile.setUsername("al")                  # ✗ Too short
profile.setEmail("invalid-email")          # ✗ Invalid format
profile.setAge(10)                         # ✗ Too young
profile.setBio("x" * 600)                  # ✗ Too long

# Read operations
println(profile.getUsername())             # ✓ alice_dev
println(profile.getEmail())                # ✓ alice@example.com
println(profile.getPhoneNumber())          # ✓ Masked for privacy

# Private data is protected
# profile.userId = "HACKED"                # ✗ COMPILE ERROR
# profile.isVerified = true                # ✗ COMPILE ERROR
```

### Example 3: Product Inventory (Controlled Stock)

Inventory management with strict stock control.

```klang
class ProductInventory {
    # Private inventory data
    private let productId: string = ""
    private let productName: string = ""
    private let stockQuantity: int = 0
    private let reorderLevel: int = 10
    private let maxCapacity: int = 1000
    private let price: float = 0.0
    private let lastRestocked: string = ""
    
    public fn init(id: string, name: string, initialStock: int, productPrice: float) {
        this.productId = id
        this.productName = name
        this.price = productPrice
        
        if initialStock >= 0 and initialStock <= this.maxCapacity {
            this.stockQuantity = initialStock
            this.lastRestocked = "2024-01-01"
        }
    }
    
    # Read-only product info
    public fn getProductId() -> string {
        return this.productId
    }
    
    public fn getProductName() -> string {
        return this.productName
    }
    
    # Read current stock level
    public fn getStockQuantity() -> int {
        return this.stockQuantity
    }
    
    # Check if item is in stock
    public fn isInStock() -> bool {
        return this.stockQuantity > 0
    }
    
    # Check if reorder is needed
    public fn needsReorder() -> bool {
        return this.stockQuantity <= this.reorderLevel
    }
    
    # Get price
    public fn getPrice() -> float {
        return this.price
    }
    
    # Update price with validation
    public fn setPrice(newPrice: float) -> bool {
        if newPrice <= 0.0 {
            println("Error: Price must be positive")
            return false
        }
        
        if newPrice > 1000000.0 {
            println("Error: Price seems unrealistic")
            return false
        }
        
        this.price = newPrice
        println("Price updated to: $" + str(newPrice))
        return true
    }
    
    # Add stock (restock operation)
    public fn addStock(quantity: int) -> bool {
        if quantity <= 0 {
            println("Error: Quantity must be positive")
            return false
        }
        
        let newStock = this.stockQuantity + quantity
        if newStock > this.maxCapacity {
            println("Error: Would exceed maximum capacity of " + str(this.maxCapacity))
            return false
        }
        
        this.stockQuantity = newStock
        this.lastRestocked = "2024-01-01"  # Update timestamp
        println("Added " + str(quantity) + " units. New stock: " + str(this.stockQuantity))
        
        return true
    }
    
    # Remove stock (sale operation)
    public fn removeStock(quantity: int) -> bool {
        if quantity <= 0 {
            println("Error: Quantity must be positive")
            return false
        }
        
        if quantity > this.stockQuantity {
            println("Error: Insufficient stock. Available: " + str(this.stockQuantity))
            return false
        }
        
        this.stockQuantity = this.stockQuantity - quantity
        println("Removed " + str(quantity) + " units. Remaining: " + str(this.stockQuantity))
        
        # Check if reorder is needed
        if this.needsReorder() {
            println("WARNING: Stock is low! Consider reordering.")
        }
        
        return true
    }
    
    # Reserve items for purchase
    public fn reserve(quantity: int) -> bool {
        if !this.isInStock() {
            println("Error: Product out of stock")
            return false
        }
        
        return this.removeStock(quantity)
    }
    
    # Get total inventory value
    public fn getTotalValue() -> float {
        return float(this.stockQuantity) * this.price
    }
    
    # Get inventory status report
    public fn getStatusReport() -> string {
        let report = "Product: " + this.productName + " (" + this.productId + ")\n"
        report = report + "Stock: " + str(this.stockQuantity) + " units\n"
        report = report + "Price: $" + str(this.price) + "\n"
        report = report + "Total Value: $" + str(this.getTotalValue()) + "\n"
        report = report + "Status: " + (this.isInStock() ? "In Stock" : "Out of Stock") + "\n"
        
        if this.needsReorder() {
            report = report + "⚠️ REORDER NEEDED\n"
        }
        
        return report
    }
}

# Usage
let inventory = new ProductInventory("PROD001", "Laptop", 50, 999.99)

# Valid operations
println(inventory.getProductName())        # ✓ Laptop
println(inventory.getStockQuantity())      # ✓ 50
inventory.removeStock(5)                   # ✓ Sale of 5 units
inventory.addStock(20)                     # ✓ Restock 20 units
println(inventory.getTotalValue())         # ✓ Calculate value

# Invalid operations (validation prevents)
inventory.removeStock(1000)                # ✗ Insufficient stock
inventory.addStock(-10)                    # ✗ Negative quantity
inventory.setPrice(-50.0)                  # ✗ Negative price

# Direct manipulation prevented
# inventory.stockQuantity = 999999         # ✗ COMPILE ERROR
# inventory.price = 0.01                   # ✗ COMPILE ERROR
```

### Example 4: Temperature Sensor (Validated Readings)

Temperature sensor with range validation and calibration.

```klang
class TemperatureSensor {
    # Private sensor data
    private let sensorId: string = ""
    private let currentTemp: float = 0.0
    private let minTemp: float = -50.0  # Minimum measurable temp
    private let maxTemp: float = 150.0  # Maximum measurable temp
    private let calibrationOffset: float = 0.0
    private let unit: string = "C"  # C or F
    private let isActive: bool = true
    private let readingHistory: array<float> = []
    private let maxHistorySize: int = 100
    
    public fn init(id: string, tempUnit: string) {
        this.sensorId = id
        if tempUnit == "F" or tempUnit == "C" {
            this.unit = tempUnit
        }
    }
    
    # Read-only sensor ID
    public fn getSensorId() -> string {
        return this.sensorId
    }
    
    # Get current temperature
    public fn getTemperature() -> float {
        if !this.isActive {
            println("Warning: Sensor is inactive")
            return 0.0
        }
        return this.currentTemp
    }
    
    # Get temperature with unit
    public fn getTemperatureWithUnit() -> string {
        return str(this.getTemperature()) + "°" + this.unit
    }
    
    # Set temperature reading (with validation)
    public fn setTemperature(temp: float) -> bool {
        if !this.isActive {
            println("Error: Sensor is inactive")
            return false
        }
        
        # Apply calibration
        let calibratedTemp = temp + this.calibrationOffset
        
        # Validate range
        if !this.isValidTemperature(calibratedTemp) {
            println("Error: Temperature out of sensor range")
            return false
        }
        
        # Check for sudden changes (might indicate sensor error)
        if len(this.readingHistory) > 0 {
            let lastReading = this.readingHistory[len(this.readingHistory) - 1]
            let change = abs(calibratedTemp - lastReading)
            
            if change > 20.0 {
                println("Warning: Sudden temperature change detected")
            }
        }
        
        this.currentTemp = calibratedTemp
        this.addToHistory(calibratedTemp)
        
        return true
    }
    
    # Get unit (read-only)
    public fn getUnit() -> string {
        return this.unit
    }
    
    # Check if sensor is active
    public fn isOperational() -> bool {
        return this.isActive
    }
    
    # Calibrate sensor
    public fn calibrate(offset: float) -> bool {
        if abs(offset) > 10.0 {
            println("Error: Calibration offset too large")
            return false
        }
        
        this.calibrationOffset = offset
        println("Sensor calibrated with offset: " + str(offset))
        return true
    }
    
    # Get average temperature from history
    public fn getAverageTemperature() -> float {
        if len(this.readingHistory) == 0 {
            return 0.0
        }
        
        let sum: float = 0.0
        let i = 0
        while i < len(this.readingHistory) {
            sum = sum + this.readingHistory[i]
            i = i + 1
        }
        
        return sum / float(len(this.readingHistory))
    }
    
    # Get minimum recorded temperature
    public fn getMinRecorded() -> float {
        if len(this.readingHistory) == 0 {
            return 0.0
        }
        
        let minVal = this.readingHistory[0]
        let i = 1
        while i < len(this.readingHistory) {
            if this.readingHistory[i] < minVal {
                minVal = this.readingHistory[i]
            }
            i = i + 1
        }
        
        return minVal
    }
    
    # Get maximum recorded temperature
    public fn getMaxRecorded() -> float {
        if len(this.readingHistory) == 0 {
            return 0.0
        }
        
        let maxVal = this.readingHistory[0]
        let i = 1
        while i < len(this.readingHistory) {
            if this.readingHistory[i] > maxVal {
                maxVal = this.readingHistory[i]
            }
            i = i + 1
        }
        
        return maxVal
    }
    
    # Reset sensor
    public fn reset() {
        this.currentTemp = 0.0
        this.calibrationOffset = 0.0
        this.readingHistory = []
        println("Sensor reset")
    }
    
    # Private helper methods
    private fn isValidTemperature(temp: float) -> bool {
        return temp >= this.minTemp and temp <= this.maxTemp
    }
    
    private fn addToHistory(temp: float) {
        this.readingHistory.append(temp)
        
        # Keep history size manageable
        if len(this.readingHistory) > this.maxHistorySize {
            # Remove oldest reading (simplified - would use queue in real implementation)
            let newHistory: array<float> = []
            let i = 1
            while i < len(this.readingHistory) {
                newHistory.append(this.readingHistory[i])
                i = i + 1
            }
            this.readingHistory = newHistory
        }
    }
    
    private fn abs(value: float) -> float {
        if value < 0.0 {
            return -value
        }
        return value
    }
}

# Usage
let sensor = new TemperatureSensor("TEMP001", "C")

# Valid operations
sensor.setTemperature(25.0)                # ✓ Set temperature
println(sensor.getTemperature())           # ✓ 25.0
println(sensor.getTemperatureWithUnit())   # ✓ 25.0°C
sensor.calibrate(1.5)                      # ✓ Calibrate sensor

# Take multiple readings
sensor.setTemperature(26.0)
sensor.setTemperature(27.5)
sensor.setTemperature(25.0)

# Get statistics
println(sensor.getAverageTemperature())    # ✓ Average
println(sensor.getMinRecorded())           # ✓ Minimum
println(sensor.getMaxRecorded())           # ✓ Maximum

# Invalid operations
sensor.setTemperature(200.0)               # ✗ Out of range
sensor.calibrate(50.0)                     # ✗ Offset too large

# Protected data
# sensor.currentTemp = 999.9               # ✗ COMPILE ERROR
# sensor.calibrationOffset = 100.0         # ✗ COMPILE ERROR
```

---

## Information Hiding

**Information Hiding** is a key principle of encapsulation that involves **hiding implementation details** from users of a class.

### What to Hide?

```
┌────────────────────────────────────────┐
│  HIDE (Implementation Details)         │
│  ────────────────────────────────────  │
│  • Data structures used internally     │
│  • Algorithms and calculations         │
│  • Helper methods                      │
│  • Internal state management           │
│  • Optimization techniques             │
│  • Caching mechanisms                  │
└────────────────────────────────────────┘

┌────────────────────────────────────────┐
│  EXPOSE (Public Interface)             │
│  ────────────────────────────────────  │
│  • Core functionality                  │
│  • Business operations                 │
│  • Data access methods                 │
│  • Configuration options               │
└────────────────────────────────────────┘
```

### Example: Search Engine

```klang
class SearchEngine {
    # HIDDEN: Internal data structures
    private let index: map<string, array<string>> = {}
    private let documentCache: map<string, string> = {}
    private let searchHistory: array<string> = []
    
    # HIDDEN: Internal algorithms
    private fn buildIndex(documents: array<string>) {
        # Complex indexing algorithm
        # Users don't need to know how this works
    }
    
    private fn calculateRelevance(doc: string, query: string) -> float {
        # Complex relevance scoring
        # Implementation can change without affecting users
        return 0.0
    }
    
    private fn rankResults(results: array<string>) -> array<string> {
        # Ranking algorithm
        return results
    }
    
    # EXPOSED: Simple public interface
    public fn search(query: string) -> array<string> {
        # Users only care about getting results
        # They don't need to know about indexing, ranking, etc.
        
        this.searchHistory.append(query)
        
        let results: array<string> = []
        # Use private methods internally
        # ... search logic ...
        
        return this.rankResults(results)
    }
    
    public fn addDocument(doc: string) {
        # Simple interface for adding documents
        # Hides complexity of indexing
        this.documentCache[doc] = doc
    }
}

# Users only see simple interface
let engine = new SearchEngine()
engine.addDocument("Document 1")
let results = engine.search("query")
# How it works internally is hidden!
```

### Benefits of Information Hiding

1. **Flexibility**: Change implementation without breaking code
2. **Simplicity**: Users see only what they need
3. **Maintainability**: Reduce dependencies
4. **Security**: Sensitive logic remains private

### Anti-Pattern: Leaky Abstraction

```klang
# ✗ BAD: Leaking implementation details
class BadCache {
    public let internalHashMap: map<string, string> = {}
    
    public fn get(key: string) -> string {
        # Users now depend on HashMap structure!
        return this.internalHashMap[key]
    }
}

# ✓ GOOD: Hide implementation
class GoodCache {
    private let storage: map<string, string> = {}
    
    public fn get(key: string) -> string {
        # Can change storage mechanism without breaking code
        if key in this.storage {
            return this.storage[key]
        }
        return ""
    }
    
    public fn set(key: string, value: string) {
        this.storage[key] = value
    }
}
```

---

## Validation in Setters

Setters should **always validate input** before modifying internal state. This is crucial for maintaining data integrity.

### Validation Strategies

#### 1. Range Validation

```klang
class Temperature {
    private let celsius: float = 0.0
    
    public fn setTemperature(temp: float) -> bool {
        # Validate range
        if temp < -273.15 {  # Absolute zero
            println("Error: Temperature below absolute zero")
            return false
        }
        
        if temp > 1000000.0 {
            println("Error: Temperature unrealistically high")
            return false
        }
        
        this.celsius = temp
        return true
    }
}
```

#### 2. Format Validation

```klang
class EmailAccount {
    private let email: string = ""
    
    public fn setEmail(newEmail: string) -> bool {
        # Validate format
        if !("@" in newEmail) {
            println("Error: Email must contain @")
            return false
        }
        
        if !("." in newEmail) {
            println("Error: Email must contain domain")
            return false
        }
        
        let parts = split(newEmail, "@")
        if len(parts) != 2 {
            println("Error: Invalid email format")
            return false
        }
        
        this.email = newEmail
        return true
    }
}
```

#### 3. Business Rule Validation

```klang
class Employee {
    private let salary: float = 0.0
    private let department: string = ""
    
    public fn setSalary(newSalary: float) -> bool {
        # Business rule: salary must be reasonable
        if newSalary < 0.0 {
            println("Error: Salary cannot be negative")
            return false
        }
        
        if newSalary > 1000000.0 {
            println("Error: Salary exceeds maximum limit")
            return false
        }
        
        # Business rule: can't decrease salary by more than 10%
        if newSalary < this.salary * 0.9 {
            println("Error: Salary decrease too large")
            return false
        }
        
        this.salary = newSalary
        return true
    }
}
```

#### 4. Dependency Validation

```klang
class DateRange {
    private let startDate: string = ""
    private let endDate: string = ""
    
    public fn setStartDate(date: string) -> bool {
        # Validate that start is before end
        if this.endDate != "" and date > this.endDate {
            println("Error: Start date must be before end date")
            return false
        }
        
        this.startDate = date
        return true
    }
    
    public fn setEndDate(date: string) -> bool {
        # Validate that end is after start
        if this.startDate != "" and date < this.startDate {
            println("Error: End date must be after start date")
            return false
        }
        
        this.endDate = date
        return true
    }
}
```

#### 5. State Validation

```klang
class Order {
    private let status: string = "PENDING"
    private let items: array<string> = []
    
    public fn setStatus(newStatus: string) -> bool {
        # Validate state transitions
        if this.status == "PENDING" {
            if newStatus == "PROCESSING" or newStatus == "CANCELLED" {
                this.status = newStatus
                return true
            }
        } else if this.status == "PROCESSING" {
            if newStatus == "SHIPPED" or newStatus == "CANCELLED" {
                this.status = newStatus
                return true
            }
        } else if this.status == "SHIPPED" {
            if newStatus == "DELIVERED" {
                this.status = newStatus
                return true
            }
        }
        
        println("Error: Invalid status transition")
        return false
    }
}
```

### Comprehensive Validation Example

```klang
class RegistrationForm {
    private let username: string = ""
    private let password: string = ""
    private let email: string = ""
    private let age: int = 0
    
    public fn setUsername(name: string) -> bool {
        # Multiple validation checks
        if len(name) < 3 {
            println("Error: Username too short (min 3 chars)")
            return false
        }
        
        if len(name) > 20 {
            println("Error: Username too long (max 20 chars)")
            return false
        }
        
        # Check for invalid characters
        if " " in name or "!" in name or "@" in name {
            println("Error: Username contains invalid characters")
            return false
        }
        
        # Check first character is letter
        let firstChar = substr(name, 0, 1)
        if firstChar < "a" or firstChar > "z" {
            println("Error: Username must start with a letter")
            return false
        }
        
        this.username = name
        return true
    }
    
    public fn setPassword(pass: string) -> bool {
        # Password strength validation
        if len(pass) < 8 {
            println("Error: Password too short (min 8 chars)")
            return false
        }
        
        # Check for complexity (simplified)
        let hasNumber = false
        let hasUpper = false
        let hasLower = false
        
        # Would iterate through characters in real implementation
        hasNumber = true  # Simplified
        hasUpper = true
        hasLower = true
        
        if !hasNumber or !hasUpper or !hasLower {
            println("Error: Password must contain uppercase, lowercase, and number")
            return false
        }
        
        this.password = pass
        return true
    }
    
    public fn setEmail(mail: string) -> bool {
        # Email validation
        if !("@" in mail and "." in mail) {
            println("Error: Invalid email format")
            return false
        }
        
        if len(mail) < 5 {
            println("Error: Email too short")
            return false
        }
        
        this.email = mail
        return true
    }
    
    public fn setAge(years: int) -> bool {
        # Age validation with business rules
        if years < 13 {
            println("Error: Must be at least 13 years old")
            return false
        }
        
        if years > 120 {
            println("Error: Invalid age")
            return false
        }
        
        this.age = years
        return true
    }
    
    # Validate entire form
    public fn isValid() -> bool {
        return this.username != "" and
               this.password != "" and
               this.email != "" and
               this.age >= 13
    }
}
```

---

## Best Practices

### 1. Keep Data Private

**Default to private** for all class properties.

```klang
# ✓ GOOD: Private by default
class GoodClass {
    private let data: string = ""
    private let count: int = 0
    
    public fn getData() -> string {
        return this.data
    }
}

# ✗ BAD: Public data
class BadClass {
    public let data: string = ""  # Anyone can modify!
    public let count: int = 0     # No control!
}
```

### 2. Provide Public Interface

Expose **only what's necessary** through public methods.

```klang
class WellEncapsulated {
    # Private data
    private let internalState: int = 0
    private let cache: array<string> = []
    
    # Public interface - minimal and focused
    public fn performOperation() -> bool {
        # Uses private methods internally
        this.validateState()
        this.updateCache()
        return true
    }
    
    public fn getResult() -> int {
        return this.internalState
    }
    
    # Private implementation
    private fn validateState() {
        # Implementation detail
    }
    
    private fn updateCache() {
        # Implementation detail
    }
}
```

### 3. Validate All Inputs

**Every setter should validate** before modifying state.

```klang
class ValidatedClass {
    private let value: int = 0
    
    public fn setValue(newValue: int) -> bool {
        # Always validate
        if newValue < 0 {
            return false
        }
        
        if newValue > 100 {
            return false
        }
        
        this.value = newValue
        return true
    }
}
```

### 4. Use Immutability Where Possible

Make properties **read-only** if they shouldn't change.

```klang
class ImmutableConfig {
    private let configId: string = ""
    private let createdAt: string = ""
    
    public fn init(id: string) {
        this.configId = id
        this.createdAt = "2024-01-01"
    }
    
    # Only getter - no setter!
    public fn getConfigId() -> string {
        return this.configId
    }
    
    # Read-only creation date
    public fn getCreatedAt() -> string {
        return this.createdAt
    }
}
```

### 5. Return Copies, Not References

Prevent external modification of internal collections.

```klang
class SafeCollection {
    private let items: array<string> = []
    
    # ✗ BAD: Returns internal reference
    public fn getItemsBad() -> array<string> {
        return this.items  # Can be modified externally!
    }
    
    # ✓ GOOD: Returns copy
    public fn getItems() -> array<string> {
        let copy: array<string> = []
        let i = 0
        while i < len(this.items) {
            copy.append(this.items[i])
            i = i + 1
        }
        return copy
    }
    
    # ✓ BETTER: Provide specific methods
    public fn getItemAt(index: int) -> string {
        if index >= 0 and index < len(this.items) {
            return this.items[index]
        }
        return ""
    }
    
    public fn getItemCount() -> int {
        return len(this.items)
    }
}
```

### 6. Use Meaningful Method Names

Method names should clearly indicate their purpose.

```klang
class ClearInterface {
    private let isActive: bool = false
    private let status: string = ""
    
    # ✓ GOOD: Clear names
    public fn isActive() -> bool {
        return this.isActive
    }
    
    public fn activate() {
        this.isActive = true
    }
    
    public fn deactivate() {
        this.isActive = false
    }
    
    # ✗ BAD: Unclear names
    public fn get() -> bool {  # Get what?
        return this.isActive
    }
    
    public fn set() {  # Set what to what?
        this.isActive = true
    }
}
```

### 7. Consistent Return Values

Setters should return consistent status indicators.

```klang
class ConsistentReturns {
    private let value: int = 0
    
    # ✓ GOOD: Consistent boolean returns
    public fn setValue(newValue: int) -> bool {
        if newValue < 0 {
            return false  # Indicate failure
        }
        this.value = newValue
        return true  # Indicate success
    }
    
    public fn increment() -> bool {
        if this.value >= 100 {
            return false
        }
        this.value = this.value + 1
        return true
    }
}
```

### 8. Document Your Interface

Add comments to explain public methods.

```klang
class DocumentedClass {
    private let balance: float = 0.0
    
    # Deposits the specified amount into the account.
    # Returns true if successful, false if amount is invalid.
    # @param amount - The amount to deposit (must be positive)
    # @return bool - Success status
    public fn deposit(amount: float) -> bool {
        if amount <= 0.0 {
            return false
        }
        this.balance = this.balance + amount
        return true
    }
}
```

---

## Common Pitfalls

### Pitfall 1: Public Mutable State

```klang
# ✗ BAD: Anyone can break invariants
class BadAccount {
    public let balance: float = 0.0
}

let account = new BadAccount()
account.balance = -1000.0  # ✗ Negative balance!

# ✓ GOOD: Controlled access
class GoodAccount {
    private let balance: float = 0.0
    
    public fn getBalance() -> float {
        return this.balance
    }
    
    public fn withdraw(amount: float) -> bool {
        if amount > 0.0 and amount <= this.balance {
            this.balance = this.balance - amount
            return true
        }
        return false
    }
}
```

### Pitfall 2: No Validation

```klang
# ✗ BAD: No validation
class BadPerson {
    private let age: int = 0
    
    public fn setAge(newAge: int) {
        this.age = newAge  # Can be negative!
    }
}

# ✓ GOOD: With validation
class GoodPerson {
    private let age: int = 0
    
    public fn setAge(newAge: int) -> bool {
        if newAge >= 0 and newAge <= 150 {
            this.age = newAge
            return true
        }
        return false
    }
}
```

### Pitfall 3: Exposing Internal Collections

```klang
# ✗ BAD: Internal array exposed
class BadList {
    private let items: array<string> = []
    
    public fn getItems() -> array<string> {
        return this.items  # External code can modify!
    }
}

# External code can break encapsulation
let list = new BadList()
let items = list.getItems()
# items could be modified externally

# ✓ GOOD: Controlled access
class GoodList {
    private let items: array<string> = []
    
    public fn getItemAt(index: int) -> string {
        if index >= 0 and index < len(this.items) {
            return this.items[index]
        }
        return ""
    }
    
    public fn getItemCount() -> int {
        return len(this.items)
    }
    
    public fn addItem(item: string) {
        this.items.append(item)
    }
}
```

### Pitfall 4: Too Many Getters/Setters

```klang
# ✗ BAD: Getter/setter for everything
class OverExposed {
    private let x: int = 0
    private let y: int = 0
    private let internalCache: array<int> = []
    
    public fn getX() -> int { return this.x }
    public fn setX(val: int) { this.x = val }
    public fn getY() -> int { return this.y }
    public fn setY(val: int) { this.y = val }
    public fn getCache() -> array<int> { return this.internalCache }
    # Why encapsulate if everything is exposed?
}

# ✓ GOOD: Expose only meaningful operations
class WellDesigned {
    private let x: int = 0
    private let y: int = 0
    private let internalCache: array<int> = []
    
    public fn getPosition() -> string {
        return "(" + str(this.x) + ", " + str(this.y) + ")"
    }
    
    public fn moveTo(newX: int, newY: int) {
        this.x = newX
        this.y = newY
    }
    
    # Cache is purely internal - not exposed
}
```

---

## Interview Problems

### Problem 1: Design a Secure Library System

**Task**: Create a `Library` and `Book` class with proper encapsulation for a library management system.

**Requirements**:
- Track book information (title, author, ISBN, available copies)
- Members can borrow and return books
- Track who has borrowed which books
- Maximum 3 books per member
- Books can be reserved
- Due date tracking

**Solution**:

```klang
class Book {
    private let isbn: string = ""
    private let title: string = ""
    private let author: string = ""
    private let totalCopies: int = 0
    private let availableCopies: int = 0
    
    public fn init(bookIsbn: string, bookTitle: string, bookAuthor: string, copies: int) {
        this.isbn = bookIsbn
        this.title = bookTitle
        this.author = bookAuthor
        this.totalCopies = copies
        this.availableCopies = copies
    }
    
    public fn getIsbn() -> string {
        return this.isbn
    }
    
    public fn getTitle() -> string {
        return this.title
    }
    
    public fn getAuthor() -> string {
        return this.author
    }
    
    public fn getAvailableCopies() -> int {
        return this.availableCopies
    }
    
    public fn isAvailable() -> bool {
        return this.availableCopies > 0
    }
    
    public fn borrowCopy() -> bool {
        if this.availableCopies > 0 {
            this.availableCopies = this.availableCopies - 1
            return true
        }
        return false
    }
    
    public fn returnCopy() -> bool {
        if this.availableCopies < this.totalCopies {
            this.availableCopies = this.availableCopies + 1
            return true
        }
        return false
    }
    
    public fn getInfo() -> string {
        return this.title + " by " + this.author + " (ISBN: " + this.isbn + ")"
    }
}

class LibraryMember {
    private let memberId: string = ""
    private let name: string = ""
    private let borrowedBooks: array<string> = []  # Array of ISBNs
    private let maxBooks: int = 3
    
    public fn init(id: string, memberName: string) {
        this.memberId = id
        this.name = memberName
    }
    
    public fn getMemberId() -> string {
        return this.memberId
    }
    
    public fn getName() -> string {
        return this.name
    }
    
    public fn getBorrowedCount() -> int {
        return len(this.borrowedBooks)
    }
    
    public fn canBorrowMore() -> bool {
        return len(this.borrowedBooks) < this.maxBooks
    }
    
    public fn addBorrowedBook(isbn: string) -> bool {
        if this.canBorrowMore() {
            this.borrowedBooks.append(isbn)
            return true
        }
        return false
    }
    
    public fn removeBorrowedBook(isbn: string) -> bool {
        let i = 0
        while i < len(this.borrowedBooks) {
            if this.borrowedBooks[i] == isbn {
                # Remove book (simplified)
                return true
            }
            i = i + 1
        }
        return false
    }
    
    public fn hasBorrowed(isbn: string) -> bool {
        let i = 0
        while i < len(this.borrowedBooks) {
            if this.borrowedBooks[i] == isbn {
                return true
            }
            i = i + 1
        }
        return false
    }
}

class Library {
    private let books: array<Book> = []
    private let members: array<LibraryMember> = []
    private let libraryName: string = ""
    
    public fn init(name: string) {
        this.libraryName = name
    }
    
    public fn getName() -> string {
        return this.libraryName
    }
    
    public fn addBook(book: Book) {
        this.books.append(book)
        println("Book added: " + book.getTitle())
    }
    
    public fn registerMember(member: LibraryMember) {
        this.members.append(member)
        println("Member registered: " + member.getName())
    }
    
    public fn borrowBook(memberId: string, isbn: string) -> bool {
        let member = this.findMember(memberId)
        if member == null {
            println("Error: Member not found")
            return false
        }
        
        if !member.canBorrowMore() {
            println("Error: Maximum book limit reached")
            return false
        }
        
        let book = this.findBook(isbn)
        if book == null {
            println("Error: Book not found")
            return false
        }
        
        if !book.isAvailable() {
            println("Error: Book not available")
            return false
        }
        
        if book.borrowCopy() and member.addBorrowedBook(isbn) {
            println(member.getName() + " borrowed: " + book.getTitle())
            return true
        }
        
        return false
    }
    
    public fn returnBook(memberId: string, isbn: string) -> bool {
        let member = this.findMember(memberId)
        if member == null {
            println("Error: Member not found")
            return false
        }
        
        if !member.hasBorrowed(isbn) {
            println("Error: Member hasn't borrowed this book")
            return false
        }
        
        let book = this.findBook(isbn)
        if book == null {
            println("Error: Book not found")
            return false
        }
        
        if book.returnCopy() and member.removeBorrowedBook(isbn) {
            println(member.getName() + " returned: " + book.getTitle())
            return true
        }
        
        return false
    }
    
    public fn listAvailableBooks() {
        println("Available Books:")
        let i = 0
        while i < len(this.books) {
            if this.books[i].isAvailable() {
                println("- " + this.books[i].getInfo() + 
                       " (" + str(this.books[i].getAvailableCopies()) + " copies)")
            }
            i = i + 1
        }
    }
    
    private fn findBook(isbn: string) -> Book {
        let i = 0
        while i < len(this.books) {
            if this.books[i].getIsbn() == isbn {
                return this.books[i]
            }
            i = i + 1
        }
        return null
    }
    
    private fn findMember(memberId: string) -> LibraryMember {
        let i = 0
        while i < len(this.members) {
            if this.members[i].getMemberId() == memberId {
                return this.members[i]
            }
            i = i + 1
        }
        return null
    }
}

# Usage
let library = new Library("City Library")

let book1 = new Book("ISBN001", "Clean Code", "Robert Martin", 3)
let book2 = new Book("ISBN002", "Design Patterns", "Gang of Four", 2)
library.addBook(book1)
library.addBook(book2)

let member = new LibraryMember("MEM001", "Alice")
library.registerMember(member)

library.borrowBook("MEM001", "ISBN001")
library.listAvailableBooks()
library.returnBook("MEM001", "ISBN001")
```

### Problem 2: Hospital Patient Record System

**Task**: Design a secure patient record system with strict access control.

**Requirements**:
- Store patient information securely
- Medical history must be private
- Only authorized access to sensitive data
- Track doctor assignments
- Prescription management

**Solution**:

```klang
class PatientRecord {
    # Private - sensitive medical data
    private let patientId: string = ""
    private let fullName: string = ""
    private let dateOfBirth: string = ""
    private let bloodType: string = ""
    private let allergies: array<string> = []
    private let medicalHistory: array<string> = []
    private let prescriptions: array<string> = []
    private let assignedDoctorId: string = ""
    private let accessPin: string = ""
    
    public fn init(id: string, name: string, dob: string, pin: string) {
        this.patientId = id
        this.fullName = name
        this.dateOfBirth = dob
        this.accessPin = pin
    }
    
    # Public - non-sensitive info
    public fn getPatientId() -> string {
        return this.patientId
    }
    
    public fn getFullName() -> string {
        return this.fullName
    }
    
    # Controlled access - requires authorization
    public fn getBloodType(authPin: string) -> string {
        if !this.authorize(authPin) {
            println("Error: Unauthorized access")
            return ""
        }
        return this.bloodType
    }
    
    public fn setBloodType(bloodType: string, authPin: string) -> bool {
        if !this.authorize(authPin) {
            return false
        }
        
        # Validate blood type
        if bloodType != "A+" and bloodType != "A-" and 
           bloodType != "B+" and bloodType != "B-" and
           bloodType != "AB+" and bloodType != "AB-" and
           bloodType != "O+" and bloodType != "O-" {
            println("Error: Invalid blood type")
            return false
        }
        
        this.bloodType = bloodType
        return true
    }
    
    public fn addAllergy(allergy: string, authPin: string) -> bool {
        if !this.authorize(authPin) {
            return false
        }
        
        this.allergies.append(allergy)
        println("Allergy added: " + allergy)
        return true
    }
    
    public fn getAllergies(authPin: string) -> array<string> {
        if !this.authorize(authPin) {
            return []
        }
        return this.allergies
    }
    
    public fn addMedicalHistory(entry: string, authPin: string) -> bool {
        if !this.authorize(authPin) {
            return false
        }
        
        let timestamp = "2024-01-01"  # Simplified
        this.medicalHistory.append(timestamp + ": " + entry)
        return true
    }
    
    public fn getMedicalHistory(authPin: string) -> array<string> {
        if !this.authorize(authPin) {
            println("Error: Unauthorized access to medical history")
            return []
        }
        return this.medicalHistory
    }
    
    public fn addPrescription(prescription: string, authPin: string) -> bool {
        if !this.authorize(authPin) {
            return false
        }
        
        this.prescriptions.append(prescription)
        println("Prescription added")
        return true
    }
    
    public fn assignDoctor(doctorId: string, authPin: string) -> bool {
        if !this.authorize(authPin) {
            return false
        }
        
        this.assignedDoctorId = doctorId
        println("Doctor assigned: " + doctorId)
        return true
    }
    
    public fn getAssignedDoctor() -> string {
        return this.assignedDoctorId
    }
    
    # Private authorization
    private fn authorize(pin: string) -> bool {
        return pin == this.accessPin
    }
}

# Usage
let patient = new PatientRecord("P001", "John Doe", "1990-01-01", "1234")

# Public information
println(patient.getPatientId())
println(patient.getFullName())

# Secure operations require PIN
patient.setBloodType("O+", "1234")
patient.addAllergy("Penicillin", "1234")
patient.addMedicalHistory("Annual checkup - healthy", "1234")

# Unauthorized access fails
patient.getBloodType("0000")  # Wrong PIN - denied
```

### Problem 3: Smart Home Security System

**Task**: Create a smart home security system with zones and access control.

**Requirements**:
- Multiple security zones
- Arm/disarm with PIN
- Track breach attempts
- Emergency mode
- Activity logging

**Solution**:

```klang
class SecurityZone {
    private let zoneId: string = ""
    private let zoneName: string = ""
    private let isArmed: bool = false
    private let sensors: array<string> = []
    private let breachCount: int = 0
    
    public fn init(id: string, name: string) {
        this.zoneId = id
        this.zoneName = name
    }
    
    public fn getZoneId() -> string {
        return this.zoneId
    }
    
    public fn getZoneName() -> string {
        return this.zoneName
    }
    
    public fn isArmed() -> bool {
        return this.isArmed
    }
    
    public fn arm() {
        this.isArmed = true
        println("Zone armed: " + this.zoneName)
    }
    
    public fn disarm() {
        this.isArmed = false
        println("Zone disarmed: " + this.zoneName)
    }
    
    public fn addSensor(sensorId: string) {
        this.sensors.append(sensorId)
    }
    
    public fn triggerBreach() {
        if this.isArmed {
            this.breachCount = this.breachCount + 1
            println("⚠️ BREACH DETECTED in " + this.zoneName)
        }
    }
    
    public fn getBreachCount() -> int {
        return this.breachCount
    }
}

class SmartHomeSecurity {
    private let systemPin: string = ""
    private let emergencyPin: string = ""
    private let zones: array<SecurityZone> = []
    private let systemArmed: bool = false
    private let emergencyMode: bool = false
    private let failedAttempts: int = 0
    private let maxFailedAttempts: int = 3
    private let activityLog: array<string> = []
    
    public fn init(pin: string, emergPin: string) {
        this.systemPin = pin
        this.emergencyPin = emergPin
        this.logActivity("System initialized")
    }
    
    public fn addZone(zone: SecurityZone) {
        this.zones.append(zone)
        this.logActivity("Zone added: " + zone.getZoneName())
    }
    
    public fn armSystem(pin: string) -> bool {
        if !this.validatePin(pin) {
            this.failedAttempts = this.failedAttempts + 1
            this.logActivity("Failed arm attempt")
            
            if this.failedAttempts >= this.maxFailedAttempts {
                this.activateEmergencyMode()
            }
            return false
        }
        
        this.failedAttempts = 0
        this.systemArmed = true
        
        # Arm all zones
        let i = 0
        while i < len(this.zones) {
            this.zones[i].arm()
            i = i + 1
        }
        
        this.logActivity("System armed")
        println("✓ System armed successfully")
        return true
    }
    
    public fn disarmSystem(pin: string) -> bool {
        if !this.validatePin(pin) {
            this.failedAttempts = this.failedAttempts + 1
            this.logActivity("Failed disarm attempt")
            
            if this.failedAttempts >= this.maxFailedAttempts {
                this.activateEmergencyMode()
            }
            return false
        }
        
        this.failedAttempts = 0
        this.systemArmed = false
        
        # Disarm all zones
        let i = 0
        while i < len(this.zones) {
            this.zones[i].disarm()
            i = i + 1
        }
        
        this.logActivity("System disarmed")
        println("✓ System disarmed successfully")
        return true
    }
    
    public fn isSystemArmed() -> bool {
        return this.systemArmed
    }
    
    public fn isInEmergencyMode() -> bool {
        return this.emergencyMode
    }
    
    public fn deactivateEmergency(emergPin: string) -> bool {
        if emergPin != this.emergencyPin {
            this.logActivity("Failed emergency deactivation")
            return false
        }
        
        this.emergencyMode = false
        this.failedAttempts = 0
        this.logActivity("Emergency mode deactivated")
        println("Emergency mode deactivated")
        return true
    }
    
    public fn getActivityLog(pin: string) -> array<string> {
        if !this.validatePin(pin) {
            println("Error: Unauthorized access")
            return []
        }
        return this.activityLog
    }
    
    public fn getSystemStatus() -> string {
        let status = "System Status:\n"
        status = status + "Armed: " + str(this.systemArmed) + "\n"
        status = status + "Emergency: " + str(this.emergencyMode) + "\n"
        status = status + "Zones: " + str(len(this.zones))
        return status
    }
    
    private fn validatePin(pin: string) -> bool {
        return pin == this.systemPin
    }
    
    private fn activateEmergencyMode() {
        this.emergencyMode = true
        this.logActivity("⚠️ EMERGENCY MODE ACTIVATED")
        println("⚠️ EMERGENCY MODE - Too many failed attempts!")
    }
    
    private fn logActivity(activity: string) {
        let timestamp = "2024-01-01 12:00:00"  # Simplified
        this.activityLog.append(timestamp + " - " + activity)
    }
}

# Usage
let security = new SmartHomeSecurity("1234", "9999")

let livingRoom = new SecurityZone("Z1", "Living Room")
let bedroom = new SecurityZone("Z2", "Bedroom")

security.addZone(livingRoom)
security.addZone(bedroom)

security.armSystem("1234")    # ✓ System armed
security.disarmSystem("0000") # ✗ Wrong PIN
security.disarmSystem("1234") # ✓ System disarmed

println(security.getSystemStatus())
```

### Problem 4: Cryptocurrency Wallet

**Task**: Design a secure cryptocurrency wallet with transaction validation.

**Requirements**:
- Multiple currency support
- Transaction history
- Balance protection
- Fee calculation
- Address validation

**Solution**:

```klang
class CryptoWallet {
    private let walletId: string = ""
    private let ownerName: string = ""
    private let privateKey: string = ""
    private let balances: map<string, float> = {}  # currency -> amount
    private let transactions: array<string> = []
    private let dailyLimit: float = 10000.0
    private let dailySpent: float = 0.0
    
    public fn init(id: string, owner: string, privKey: string) {
        this.walletId = id
        this.ownerName = owner
        this.privateKey = privKey
        
        # Initialize with zero balances
        this.balances["BTC"] = 0.0
        this.balances["ETH"] = 0.0
        this.balances["USDT"] = 0.0
    }
    
    public fn getWalletId() -> string {
        return this.walletId
    }
    
    public fn getOwnerName() -> string {
        return this.ownerName
    }
    
    public fn getBalance(currency: string, authKey: string) -> float {
        if !this.authenticate(authKey) {
            println("Error: Authentication failed")
            return -1.0
        }
        
        if currency in this.balances {
            return this.balances[currency]
        }
        return 0.0
    }
    
    public fn deposit(currency: string, amount: float, authKey: string) -> bool {
        if !this.authenticate(authKey) {
            return false
        }
        
        if amount <= 0.0 {
            println("Error: Invalid deposit amount")
            return false
        }
        
        if !(currency in this.balances) {
            println("Error: Unsupported currency")
            return false
        }
        
        this.balances[currency] = this.balances[currency] + amount
        this.logTransaction("DEPOSIT: " + str(amount) + " " + currency)
        println("✓ Deposited: " + str(amount) + " " + currency)
        return true
    }
    
    public fn withdraw(currency: string, amount: float, authKey: string) -> bool {
        if !this.authenticate(authKey) {
            return false
        }
        
        if amount <= 0.0 {
            println("Error: Invalid withdrawal amount")
            return false
        }
        
        if !(currency in this.balances) {
            println("Error: Unsupported currency")
            return false
        }
        
        if amount > this.balances[currency] {
            println("Error: Insufficient balance")
            return false
        }
        
        # Check daily limit
        if this.dailySpent + amount > this.dailyLimit {
            println("Error: Daily limit exceeded")
            return false
        }
        
        # Calculate fee (1%)
        let fee = amount * 0.01
        let total = amount + fee
        
        if total > this.balances[currency] {
            println("Error: Insufficient balance including fee")
            return false
        }
        
        this.balances[currency] = this.balances[currency] - total
        this.dailySpent = this.dailySpent + amount
        this.logTransaction("WITHDRAW: " + str(amount) + " " + currency + " (fee: " + str(fee) + ")")
        println("✓ Withdrawn: " + str(amount) + " " + currency)
        return true
    }
    
    public fn transfer(toCurrency: string, toAddress: string, amount: float, authKey: string) -> bool {
        if !this.authenticate(authKey) {
            return false
        }
        
        if !this.isValidAddress(toAddress) {
            println("Error: Invalid recipient address")
            return false
        }
        
        # Use withdraw for validation and fee calculation
        if this.withdraw(toCurrency, amount, authKey) {
            this.logTransaction("TRANSFER: " + str(amount) + " " + toCurrency + " to " + toAddress)
            return true
        }
        
        return false
    }
    
    public fn getTransactionHistory(authKey: string) -> array<string> {
        if !this.authenticate(authKey) {
            println("Error: Authentication failed")
            return []
        }
        return this.transactions
    }
    
    public fn getTotalPortfolioValue(authKey: string) -> float {
        if !this.authenticate(authKey) {
            return -1.0
        }
        
        # Simplified - would use real exchange rates
        let total: float = 0.0
        total = total + this.balances["BTC"] * 50000.0  # Simplified rate
        total = total + this.balances["ETH"] * 3000.0
        total = total + this.balances["USDT"] * 1.0
        
        return total
    }
    
    private fn authenticate(key: string) -> bool {
        if key != this.privateKey {
            this.logTransaction("FAILED AUTH ATTEMPT")
            println("Error: Invalid private key")
            return false
        }
        return true
    }
    
    private fn isValidAddress(address: string) -> bool {
        # Simplified validation
        return len(address) >= 26 and len(address) <= 42
    }
    
    private fn logTransaction(transaction: string) {
        let timestamp = "2024-01-01 12:00:00"
        this.transactions.append(timestamp + " - " + transaction)
    }
}

# Usage
let wallet = new CryptoWallet("WALLET001", "Alice", "SECRET_KEY_123")

wallet.deposit("BTC", 0.5, "SECRET_KEY_123")
wallet.deposit("ETH", 2.0, "SECRET_KEY_123")

println(wallet.getBalance("BTC", "SECRET_KEY_123"))
wallet.withdraw("BTC", 0.1, "SECRET_KEY_123")

println(wallet.getTotalPortfolioValue("SECRET_KEY_123"))

# Invalid operations
wallet.withdraw("BTC", 10.0, "SECRET_KEY_123")  # Insufficient balance
wallet.getBalance("BTC", "WRONG_KEY")           # Authentication fails
```

### Problem 5: Online Exam System

**Task**: Create an online exam system with strict access control and time limits.

**Requirements**:
- Timed exams
- Question bank protection
- Answer submission validation
- Score calculation
- Prevent cheating

**Solution**:

```klang
class Question {
    private let questionId: string = ""
    private let questionText: string = ""
    private let options: array<string> = []
    private let correctAnswer: int = 0
    private let points: int = 1
    
    public fn init(id: string, text: string, opts: array<string>, correct: int) {
        this.questionId = id
        this.questionText = text
        this.options = opts
        this.correctAnswer = correct
        this.points = 1
    }
    
    public fn getQuestionId() -> string {
        return this.questionId
    }
    
    public fn getQuestionText() -> string {
        return this.questionText
    }
    
    public fn getOptions() -> array<string> {
        return this.options
    }
    
    public fn checkAnswer(answer: int) -> bool {
        return answer == this.correctAnswer
    }
    
    public fn getPoints() -> int {
        return this.points
    }
}

class OnlineExam {
    private let examId: string = ""
    private let examTitle: string = ""
    private let questions: array<Question> = []
    private let studentAnswers: map<string, int> = {}  # questionId -> answer
    private let startTime: int = 0
    private let duration: int = 3600  # seconds
    private let isStarted: bool = false
    private let isSubmitted: bool = false
    private let studentId: string = ""
    private let accessCode: string = ""
    
    public fn init(id: string, title: string, code: string, durationSecs: int) {
        this.examId = id
        this.examTitle = title
        this.accessCode = code
        this.duration = durationSecs
    }
    
    public fn getExamId() -> string {
        return this.examId
    }
    
    public fn getExamTitle() -> string {
        return this.examTitle
    }
    
    public fn addQuestion(question: Question) {
        if this.isStarted {
            println("Error: Cannot add questions after exam started")
            return
        }
        this.questions.append(question)
    }
    
    public fn startExam(studId: string, code: string) -> bool {
        if code != this.accessCode {
            println("Error: Invalid access code")
            return false
        }
        
        if this.isStarted {
            println("Error: Exam already started")
            return false
        }
        
        if len(this.questions) == 0 {
            println("Error: No questions in exam")
            return false
        }
        
        this.studentId = studId
        this.isStarted = true
        this.startTime = 0  # Would use actual timestamp
        println("✓ Exam started for student: " + studId)
        println("Duration: " + str(this.duration) + " seconds")
        return true
    }
    
    public fn getQuestion(index: int) -> Question {
        if !this.isStarted {
            println("Error: Exam not started")
            return null
        }
        
        if this.isSubmitted {
            println("Error: Exam already submitted")
            return null
        }
        
        if index >= 0 and index < len(this.questions) {
            return this.questions[index]
        }
        
        return null
    }
    
    public fn answerQuestion(questionId: string, answer: int) -> bool {
        if !this.isStarted {
            println("Error: Exam not started")
            return false
        }
        
        if this.isSubmitted {
            println("Error: Exam already submitted")
            return false
        }
        
        # Validate answer is within range
        if answer < 0 or answer > 3 {
            println("Error: Invalid answer option")
            return false
        }
        
        this.studentAnswers[questionId] = answer
        println("Answer recorded for question: " + questionId)
        return true
    }
    
    public fn submitExam() -> bool {
        if !this.isStarted {
            println("Error: Exam not started")
            return false
        }
        
        if this.isSubmitted {
            println("Error: Exam already submitted")
            return false
        }
        
        # Check if all questions answered
        if len(this.studentAnswers) < len(this.questions) {
            println("Warning: Not all questions answered")
        }
        
        this.isSubmitted = true
        println("✓ Exam submitted successfully")
        return true
    }
    
    public fn getScore() -> int {
        if !this.isSubmitted {
            println("Error: Exam not submitted yet")
            return -1
        }
        
        let score = 0
        let i = 0
        while i < len(this.questions) {
            let question = this.questions[i]
            let questionId = question.getQuestionId()
            
            if questionId in this.studentAnswers {
                let studentAnswer = this.studentAnswers[questionId]
                if question.checkAnswer(studentAnswer) {
                    score = score + question.getPoints()
                }
            }
            i = i + 1
        }
        
        return score
    }
    
    public fn getTotalPoints() -> int {
        let total = 0
        let i = 0
        while i < len(this.questions) {
            total = total + this.questions[i].getPoints()
            i = i + 1
        }
        return total
    }
    
    public fn getResults() -> string {
        if !this.isSubmitted {
            return "Exam not submitted"
        }
        
        let score = this.getScore()
        let total = this.getTotalPoints()
        let percentage = float(score) / float(total) * 100.0
        
        let results = "Exam Results\n"
        results = results + "============\n"
        results = results + "Student: " + this.studentId + "\n"
        results = results + "Exam: " + this.examTitle + "\n"
        results = results + "Score: " + str(score) + "/" + str(total) + "\n"
        results = results + "Percentage: " + str(percentage) + "%\n"
        
        if percentage >= 90.0 {
            results = results + "Grade: A"
        } else if percentage >= 80.0 {
            results = results + "Grade: B"
        } else if percentage >= 70.0 {
            results = results + "Grade: C"
        } else if percentage >= 60.0 {
            results = results + "Grade: D"
        } else {
            results = results + "Grade: F"
        }
        
        return results
    }
}

# Usage
let exam = new OnlineExam("EXAM001", "KLang Fundamentals", "ACCESS123", 1800)

# Add questions
let q1 = new Question("Q1", "What is encapsulation?", 
                     ["Data hiding", "Inheritance", "Polymorphism", "Abstraction"], 0)
let q2 = new Question("Q2", "Which is an access modifier?", 
                     ["var", "private", "function", "class"], 1)
let q3 = new Question("Q3", "What validates data?", 
                     ["Getter", "Setter", "Constructor", "Destructor"], 1)

exam.addQuestion(q1)
exam.addQuestion(q2)
exam.addQuestion(q3)

# Student takes exam
exam.startExam("STUDENT001", "ACCESS123")

# Answer questions
exam.answerQuestion("Q1", 0)  # Correct
exam.answerQuestion("Q2", 1)  # Correct
exam.answerQuestion("Q3", 0)  # Wrong

# Submit
exam.submitExam()

# Get results
println(exam.getResults())
```

### Problem 6: Rate-Limited API Client

**Task**: Create an API client with rate limiting and request validation.

**Requirements**:
- Rate limiting (requests per minute)
- Request queue management
- Authentication token protection
- Response caching
- Error handling

**Solution**:

```klang
class APIClient {
    private let apiKey: string = ""
    private let baseUrl: string = ""
    private let requestCount: int = 0
    private let requestLimit: int = 60  # per minute
    private let requestHistory: array<int> = []
    private let cache: map<string, string> = {}
    private let cacheExpiry: map<string, int> = {}
    private let isAuthenticated: bool = false
    
    public fn init(url: string, key: string) {
        this.baseUrl = url
        this.apiKey = key
    }
    
    public fn authenticate() -> bool {
        if len(this.apiKey) == 0 {
            println("Error: No API key provided")
            return false
        }
        
        # Simplified authentication
        this.isAuthenticated = true
        println("✓ Authenticated successfully")
        return true
    }
    
    public fn makeRequest(endpoint: string) -> string {
        if !this.isAuthenticated {
            println("Error: Not authenticated")
            return ""
        }
        
        # Check rate limit
        if !this.checkRateLimit() {
            println("Error: Rate limit exceeded")
            return ""
        }
        
        # Check cache
        let cachedResponse = this.getFromCache(endpoint)
        if cachedResponse != "" {
            println("✓ Cache hit for: " + endpoint)
            return cachedResponse
        }
        
        # Make request (simplified)
        let response = this.sendRequest(endpoint)
        
        # Cache response
        this.addToCache(endpoint, response)
        
        # Track request
        this.requestCount = this.requestCount + 1
        this.requestHistory.append(0)  # Would use timestamp
        
        return response
    }
    
    public fn getRemainingRequests() -> int {
        return this.requestLimit - this.requestCount
    }
    
    public fn clearCache() {
        this.cache = {}
        this.cacheExpiry = {}
        println("Cache cleared")
    }
    
    private fn checkRateLimit() -> bool {
        # Simplified - would check actual time window
        return this.requestCount < this.requestLimit
    }
    
    private fn getFromCache(key: string) -> string {
        if key in this.cache {
            # Check if expired (simplified)
            return this.cache[key]
        }
        return ""
    }
    
    private fn addToCache(key: string, value: string) {
        this.cache[key] = value
        this.cacheExpiry[key] = 0  # Would use actual expiry time
    }
    
    private fn sendRequest(endpoint: string) -> string {
        # Simplified HTTP request
        let url = this.baseUrl + endpoint
        return "Response from: " + url
    }
}

# Usage
let api = new APIClient("https://api.example.com", "API_KEY_123")
api.authenticate()

let response1 = api.makeRequest("/users")
let response2 = api.makeRequest("/users")  # Cached

println("Remaining requests: " + str(api.getRemainingRequests()))
```

---

## Conclusion

**Encapsulation** is the foundation of secure, maintainable, and robust object-oriented code. By following these principles:

1. **Keep data private** - Protect internal state
2. **Provide public interfaces** - Expose only what's necessary
3. **Validate all inputs** - Ensure data integrity
4. **Hide implementation details** - Allow flexibility
5. **Use getters/setters wisely** - Control access

You create code that is:
- ✓ Secure and protected from misuse
- ✓ Maintainable and easy to refactor
- ✓ Flexible and adaptable to change
- ✓ Reliable and consistent
- ✓ Easy to test and debug

Remember: **Encapsulation is not just about hiding data—it's about creating a well-defined, controlled interface to your objects that ensures they remain in valid states throughout their lifetime.**
