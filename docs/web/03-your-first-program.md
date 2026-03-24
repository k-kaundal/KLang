# Your First Program

## Building a Complete KLang Application

In this tutorial, we'll build a practical todo list application from scratch. You'll learn fundamental concepts while creating something useful!

## What We'll Build

A command-line todo list manager that can:
- Add tasks
- List all tasks
- Mark tasks as complete
- Filter by status
- Save/load from file

By the end, you'll understand:
- Variables and data structures
- Functions and program organization
- Control flow and conditionals
- Array manipulation
- File I/O
- User interaction

## Prerequisites

Make sure you have KLang installed. Check with:

```bash
klang --version
```

If not installed, see the [Installation Guide](01-installation.md).

## Step 1: Hello World (1 minute)

Let's start simple. Create a file called `todo.kl`:

```klang
# todo.kl - Version 1
println("Welcome to KLang Todo Manager!")
```

Run it:

```bash
klang run todo.kl
```

**Output:**
```
Welcome to KLang Todo Manager!
```

Great! You've created and run your first program.

## Step 2: Store Tasks (5 minutes)

Now let's add the ability to store tasks. We'll use an array of objects:

```klang
# todo.kl - Version 2

println("Welcome to KLang Todo Manager!")
println("")

# Store our tasks in an array
let tasks = [
    {id: 1, title: "Learn KLang basics", done: false},
    {id: 2, title: "Build a todo app", done: false},
    {id: 3, title: "Master arrays", done: true}
]

println("You have " + str(len(tasks)) + " tasks")
```

**Output:**
```
Welcome to KLang Todo Manager!

You have 3 tasks
```

### What's Happening Here?

- `let tasks = [...]` creates a mutable array
- Each task is an object with `id`, `title`, and `done` properties
- `len(tasks)` returns the number of tasks
- `str()` converts the number to a string for concatenation

## Step 3: Display Tasks (10 minutes)

Let's create a function to display all tasks nicely:

```klang
# todo.kl - Version 3

println("Welcome to KLang Todo Manager!")
println("")

let tasks = [
    {id: 1, title: "Learn KLang basics", done: false},
    {id: 2, title: "Build a todo app", done: false},
    {id: 3, title: "Master arrays", done: true}
]

# Function to display all tasks
fn displayTasks() {
    println("=== Your Tasks ===")
    
    if len(tasks) == 0 {
        println("No tasks yet! Add one to get started.")
        return
    }
    
    for i in 0..len(tasks) {
        let task = tasks[i]
        let status = task.done ? "✓" : " "
        let taskNum = i + 1
        println(str(taskNum) + ". [" + status + "] " + task.title)
    }
    
    println("")
}

# Display the tasks
displayTasks()
```

**Output:**
```
Welcome to KLang Todo Manager!

=== Your Tasks ===
1. [ ] Learn KLang basics
2. [ ] Build a todo app
3. [✓] Master arrays

```

### Key Concepts Introduced

- **Function declaration**: `fn displayTasks() { ... }`
- **Conditional logic**: `if len(tasks) == 0 { ... }`
- **Ternary operator**: `task.done ? "✓" : " "`
- **For loop**: `for i in 0..len(tasks) { ... }`
- **Array indexing**: `tasks[i]`
- **Object property access**: `task.done`, `task.title`

## Step 4: Add Tasks (15 minutes)

Now let's add functionality to create new tasks:

```klang
# todo.kl - Version 4

println("Welcome to KLang Todo Manager!")
println("")

let tasks = []
let nextId = 1

# Function to add a task
fn addTask(title) {
    let task = {
        id: nextId,
        title: title,
        done: false
    }
    tasks.push(task)
    nextId = nextId + 1
    println("✓ Added: " + title)
}

# Function to display all tasks
fn displayTasks() {
    println("=== Your Tasks ===")
    
    if len(tasks) == 0 {
        println("No tasks yet! Add one to get started.")
        return
    }
    
    for i in 0..len(tasks) {
        let task = tasks[i]
        let status = task.done ? "✓" : " "
        let taskNum = i + 1
        println(str(taskNum) + ". [" + status + "] " + task.title)
    }
    
    println("")
}

# Add some tasks
addTask("Learn KLang basics")
addTask("Build a todo app")
addTask("Master arrays")

# Display the tasks
displayTasks()
```

**Output:**
```
Welcome to KLang Todo Manager!

✓ Added: Learn KLang basics
✓ Added: Build a todo app
✓ Added: Master arrays
=== Your Tasks ===
1. [ ] Learn KLang basics
2. [ ] Build a todo app
3. [ ] Master arrays

```

### New Concepts

- **Mutable global variables**: `tasks` and `nextId` change as we add tasks
- **Array method**: `tasks.push(task)` adds to the array
- **Object literal creation**: Building task objects dynamically
- **Variable increment**: `nextId = nextId + 1`

## Step 5: Complete Tasks (20 minutes)

Let's add the ability to mark tasks as done:

```klang
# todo.kl - Version 5

println("Welcome to KLang Todo Manager!")
println("")

let tasks = []
let nextId = 1

fn addTask(title) {
    let task = {id: nextId, title: title, done: false}
    tasks.push(task)
    nextId = nextId + 1
    println("✓ Added: " + title)
}

fn displayTasks() {
    println("=== Your Tasks ===")
    if len(tasks) == 0 {
        println("No tasks yet!")
        return
    }
    
    for i in 0..len(tasks) {
        let task = tasks[i]
        let status = task.done ? "✓" : " "
        println(str(i + 1) + ". [" + status + "] " + task.title)
    }
    println("")
}

# Function to complete a task by number
fn completeTask(taskNum) {
    let index = taskNum - 1
    
    if index < 0 || index >= len(tasks) {
        println("✗ Invalid task number: " + str(taskNum))
        return
    }
    
    tasks[index].done = true
    println("✓ Completed: " + tasks[index].title)
}

# Function to get task counts
fn getStats() {
    let total = len(tasks)
    let completed = tasks.filter(t => t.done).length
    let remaining = total - completed
    
    return {
        total: total,
        completed: completed,
        remaining: remaining
    }
}

fn displayStats() {
    let stats = getStats()
    println("Stats: " + str(stats.completed) + "/" + str(stats.total) + " completed, " + str(stats.remaining) + " remaining")
    println("")
}

# Demo the app
addTask("Learn KLang basics")
addTask("Build a todo app")
addTask("Master arrays")
addTask("Add file saving")

println("")
displayTasks()

# Complete some tasks
completeTask(1)
completeTask(3)

println("")
displayTasks()
displayStats()
```

**Output:**
```
Welcome to KLang Todo Manager!

✓ Added: Learn KLang basics
✓ Added: Build a todo app
✓ Added: Master arrays
✓ Added: Add file saving

=== Your Tasks ===
1. [ ] Learn KLang basics
2. [ ] Build a todo app
3. [ ] Master arrays
4. [ ] Add file saving

✓ Completed: Learn KLang basics
✓ Completed: Master arrays

=== Your Tasks ===
1. [✓] Learn KLang basics
2. [ ] Build a todo app
3. [✓] Master arrays
4. [ ] Add file saving

Stats: 2/4 completed, 2 remaining

```

### Advanced Concepts

- **Array filtering**: `tasks.filter(t => t.done)`
- **Arrow functions**: `t => t.done` is a shorthand function
- **Property mutation**: `tasks[index].done = true`
- **Bounds checking**: Validating array indices
- **Complex return values**: Returning objects from functions

## Step 6: Save and Load (30 minutes)

Now let's add persistence with file I/O:

```klang
# todo.kl - Version 6 (Final)

println("Welcome to KLang Todo Manager!")
println("")

let tasks = []
let nextId = 1
const SAVE_FILE = "tasks.txt"

fn addTask(title) {
    let task = {id: nextId, title: title, done: false}
    tasks.push(task)
    nextId = nextId + 1
    println("✓ Added: " + title)
    saveTasks()
}

fn displayTasks() {
    println("=== Your Tasks ===")
    if len(tasks) == 0 {
        println("No tasks yet!")
        return
    }
    
    for i in 0..len(tasks) {
        let task = tasks[i]
        let status = task.done ? "✓" : " "
        println(str(i + 1) + ". [" + status + "] " + task.title)
    }
    println("")
}

fn completeTask(taskNum) {
    let index = taskNum - 1
    
    if index < 0 || index >= len(tasks) {
        println("✗ Invalid task number: " + str(taskNum))
        return
    }
    
    tasks[index].done = true
    println("✓ Completed: " + tasks[index].title)
    saveTasks()
}

fn deleteTask(taskNum) {
    let index = taskNum - 1
    
    if index < 0 || index >= len(tasks) {
        println("✗ Invalid task number: " + str(taskNum))
        return
    }
    
    let task = tasks[index]
    println("✗ Deleted: " + task.title)
    
    # Remove from array
    let newTasks = []
    for i in 0..len(tasks) {
        if i != index {
            newTasks.push(tasks[i])
        }
    }
    tasks = newTasks
    saveTasks()
}

fn saveTasks() {
    # Simple format: each line is "id|title|done"
    let lines = tasks.map(t => str(t.id) + "|" + t.title + "|" + str(t.done))
    let content = lines.join("\n")
    write_file(SAVE_FILE, content)
}

fn loadTasks() {
    if !file_exists(SAVE_FILE) {
        println("No saved tasks found. Starting fresh!")
        return
    }
    
    let content = read_file(SAVE_FILE)
    if len(content) == 0 {
        return
    }
    
    let lines = content.split("\n")
    
    for line of lines {
        if len(line) > 0 {
            let parts = line.split("|")
            if len(parts) == 3 {
                let task = {
                    id: int(parts[0]),
                    title: parts[1],
                    done: parts[2] == "true"
                }
                tasks.push(task)
                
                # Update nextId
                if task.id >= nextId {
                    nextId = task.id + 1
                }
            }
        }
    }
    
    println("✓ Loaded " + str(len(tasks)) + " tasks from " + SAVE_FILE)
}

fn displayStats() {
    let total = len(tasks)
    let completed = tasks.filter(t => t.done)
    let completedCount = len(completed)
    let remaining = total - completedCount
    
    println("Stats: " + str(completedCount) + "/" + str(total) + " completed, " + str(remaining) + " remaining")
    println("")
}

fn showHelp() {
    println("Commands:")
    println("  add <title>      - Add a new task")
    println("  list             - Show all tasks")
    println("  done <number>    - Mark task as complete")
    println("  delete <number>  - Remove a task")
    println("  stats            - Show statistics")
    println("  help             - Show this help")
    println("")
}

# Load existing tasks
loadTasks()

# Demo the application
println("")
showHelp()

# Simulate user actions
addTask("Learn KLang basics")
addTask("Build a todo app")
addTask("Master arrays")

println("")
displayTasks()
displayStats()

# Complete a task
completeTask(1)

println("")
displayTasks()
displayStats()

# Delete a task
deleteTask(3)

println("")
displayTasks()
displayStats()

println("Tasks saved to " + SAVE_FILE)
```

**Output:**
```
Welcome to KLang Todo Manager!

No saved tasks found. Starting fresh!

Commands:
  add <title>      - Add a new task
  list             - Show all tasks
  done <number>    - Mark task as complete
  delete <number>  - Remove a task
  stats            - Show statistics
  help             - Show this help

✓ Added: Learn KLang basics
✓ Added: Build a todo app
✓ Added: Master arrays

=== Your Tasks ===
1. [ ] Learn KLang basics
2. [ ] Build a todo app
3. [ ] Master arrays

Stats: 0/3 completed, 3 remaining

✓ Completed: Learn KLang basics

=== Your Tasks ===
1. [✓] Learn KLang basics
2. [ ] Build a todo app
3. [ ] Master arrays

Stats: 1/3 completed, 2 remaining

✗ Deleted: Master arrays

=== Your Tasks ===
1. [✓] Learn KLang basics
2. [ ] Build a todo app

Stats: 1/2 completed, 1 remaining

Tasks saved to tasks.txt
```

### File I/O Concepts

- **File writing**: `write_file(filename, content)`
- **File reading**: `read_file(filename)`
- **File existence check**: `file_exists(filename)`
- **String methods**: `split()`, `join()`
- **Type conversion**: `int()` to parse strings
- **Data serialization**: Converting objects to strings

## Complete Program Analysis

Let's break down what we've built:

### Data Structures

```klang
# Array of task objects
let tasks = [
    {id: 1, title: "Task name", done: false}
]
```

### Core Functions

1. **addTask(title)** - Creates and stores new tasks
2. **displayTasks()** - Shows formatted task list
3. **completeTask(taskNum)** - Marks tasks as done
4. **deleteTask(taskNum)** - Removes tasks
5. **saveTasks()** - Persists to file
6. **loadTasks()** - Restores from file
7. **displayStats()** - Shows completion statistics

### Key Programming Patterns

#### 1. Input Validation

```klang
if index < 0 || index >= len(tasks) {
    println("✗ Invalid task number")
    return
}
```

#### 2. Array Transformation

```klang
# Convert tasks to strings
let lines = tasks.map(t => str(t.id) + "|" + t.title + "|" + str(t.done))
```

#### 3. Filtering

```klang
# Get only completed tasks
let completed = tasks.filter(t => t.done)
```

#### 4. Object Creation

```klang
let task = {
    id: nextId,
    title: title,
    done: false
}
```

## Enhancements to Try

Ready to make it even better? Try these challenges:

### Easy Challenges

1. **Add priority levels**: Add a `priority` field (low, medium, high)
2. **Color output**: Use ANSI codes for colored task status
3. **Sort tasks**: Add sorting by priority or creation date
4. **Search tasks**: Filter tasks by keyword in title

### Medium Challenges

1. **Due dates**: Add `dueDate` field and show overdue tasks
2. **Categories**: Group tasks by category (work, personal, etc.)
3. **Task editing**: Allow updating task titles
4. **Undo feature**: Keep history of changes

### Advanced Challenges

1. **Multiple lists**: Support multiple todo lists
2. **JSON format**: Use JSON instead of pipe-delimited format
3. **Web API**: Sync tasks with a remote server
4. **Interactive mode**: Build a menu-driven interface

## Common Patterns Used

### Pattern 1: CRUD Operations

**C**reate, **R**ead, **U**pdate, **D**elete:

```klang
# Create
fn addTask(title) { ... }

# Read
fn displayTasks() { ... }

# Update
fn completeTask(taskNum) { ... }

# Delete
fn deleteTask(taskNum) { ... }
```

### Pattern 2: Data Persistence

```klang
# Save state
fn saveTasks() {
    let serialized = serialize(tasks)
    write_file(FILE, serialized)
}

# Load state
fn loadTasks() {
    let data = read_file(FILE)
    tasks = deserialize(data)
}
```

### Pattern 3: Separation of Concerns

- **Data layer**: `tasks` array
- **Business logic**: `addTask`, `completeTask`
- **Presentation**: `displayTasks`, `displayStats`
- **Persistence**: `saveTasks`, `loadTasks`

## Debugging Tips

If something doesn't work:

1. **Check file paths**: Ensure `tasks.txt` is writable
2. **Print intermediate values**: Add `println()` to see what's happening
3. **Test incrementally**: Comment out code to isolate issues
4. **Verify types**: Use `type()` to check variable types

```klang
# Debug example
fn loadTasks() {
    println("DEBUG: Checking for file...")
    if !file_exists(SAVE_FILE) {
        println("DEBUG: File doesn't exist")
        return
    }
    
    let content = read_file(SAVE_FILE)
    println("DEBUG: Read " + str(len(content)) + " characters")
    
    # ... rest of function
}
```

## What You've Learned

Congratulations! You've built a complete application and learned:

✅ **Variables**: const, let, and when to use each  
✅ **Functions**: Declaration, parameters, return values  
✅ **Arrays**: Creation, indexing, methods (map, filter, push)  
✅ **Objects**: Literals, property access, nested structures  
✅ **Control Flow**: if/else, for loops, conditionals  
✅ **File I/O**: Reading, writing, and checking files  
✅ **String Operations**: Concatenation, split, join  
✅ **Type Conversion**: str(), int(), type()  
✅ **Program Structure**: Organizing code with functions  
✅ **Data Persistence**: Saving and loading state  

## Next Steps

Now that you've built your first complete program, dive deeper:

- **[Variables & Types](10-variables-types.md)** - Understand type inference and type annotations
- **[Functions](13-functions.md)** - Learn about closures, arrow functions, and advanced patterns
- **[Arrays](14-arrays.md)** - Master all array methods and transformations
- **[Objects](15-objects.md)** - Deep dive into object-oriented features
- **[Control Flow](12-control-flow.md)** - Advanced loops, switch statements, and more

## Full Source Code

The complete final version is available in the KLang repository:

```bash
# Find example programs
ls examples/

# Try the todo app (if included)
klang run examples/todo_app.kl
```

---

**Congratulations on building your first KLang application!** 🎉 You're now ready to explore more advanced features!
