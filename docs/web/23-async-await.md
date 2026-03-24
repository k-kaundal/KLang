# Async/Await

## Mastering Asynchronous Programming in KLang

Asynchronous programming is essential for modern applications. KLang's async/await syntax makes it easy to write asynchronous code that looks and behaves like synchronous code, improving readability and maintainability.

## Why Use Async/Await?

Async/await helps you:
- **Handle Async Operations**: Work with network requests, file I/O, timers naturally
- **Write Readable Code**: Avoid callback hell and pyramid of doom
- **Error Handling**: Use try/catch for async errors
- **Sequential Execution**: Write async code that executes in order
- **Parallel Execution**: Run multiple async operations concurrently
- **Maintain Context**: Preserve scope and this binding

## Promises Fundamentals

### Creating Promises

Promises represent future values:

```klang
# Create a promise
let promise = Promise(fn(resolve, reject) {
    # Async operation
    setTimeout(fn() {
        resolve("Success!")
    }, 1000)
})

# Use the promise
promise.then(fn(result) {
    println(result)  # Success!
})
```

### Promise States

```klang
# Pending promise
let pending = Promise(fn(resolve, reject) {
    # Not yet resolved
})

# Fulfilled promise
let fulfilled = Promise(fn(resolve, reject) {
    resolve("Done")
})

# Rejected promise
let rejected = Promise(fn(resolve, reject) {
    reject("Error")
})

# Check promise results
fulfilled.then(fn(value) {
    println(value)  # Done
})

rejected.catch(fn(error) {
    println("Caught: " + error)  # Caught: Error
})
```

### then() and catch()

```klang
# Chain promises
fetchUserData()
    .then(fn(user) {
        println("Got user: " + user.name)
        return fetchUserPosts(user.id)
    })
    .then(fn(posts) {
        println("Got " + str(posts.length) + " posts")
        return posts[0]
    })
    .then(fn(firstPost) {
        println("First post: " + firstPost.title)
    })
    .catch(fn(error) {
        println("Error: " + error)
    })

# Handle errors at any step
getData()
    .then(processData)
    .then(saveData)
    .catch(handleError)
    .finally(cleanup)
```

### Promise.all()

Run multiple promises in parallel:

```klang
let promises = [
    fetchUser(1),
    fetchUser(2),
    fetchUser(3)
]

Promise.all(promises)
    .then(fn(users) {
        println("Got all users:")
        for let user in users {
            println("  - " + user.name)
        }
    })
    .catch(fn(error) {
        println("One or more requests failed: " + error)
    })

# Practical example
let dataPromises = [
    fetch("/api/users"),
    fetch("/api/posts"),
    fetch("/api/comments")
]

Promise.all(dataPromises)
    .then(fn([users, posts, comments]) {
        println("Users: " + str(users.length))
        println("Posts: " + str(posts.length))
        println("Comments: " + str(comments.length))
    })
```

### Promise.race()

Get result from first completed promise:

```klang
let promises = [
    fetchFromServer1(),
    fetchFromServer2(),
    fetchFromServer3()
]

Promise.race(promises)
    .then(fn(result) {
        println("First server responded: " + result)
    })

# Timeout pattern
Promise.race([
    fetchData(),
    timeout(5000)
])
    .then(fn(result) {
        println("Got result in time: " + result)
    })
    .catch(fn(error) {
        println("Request timed out")
    })
```

## Async/Await Basics

### Async Functions

Declare async functions with `async` keyword:

```klang
# Async function declaration
async fn fetchUserData() {
    let response = await fetch("/api/user")
    return response.data
}

# Arrow function
let getData = async () => {
    let result = await fetchData()
    return result
}

# Method in class
class UserService {
    async fn getUser(id) {
        let user = await fetch("/api/users/" + str(id))
        return user
    }
}
```

### Using await

Wait for promises to resolve:

```klang
async fn getUserInfo(userId) {
    # Wait for user data
    let user = await fetchUser(userId)
    println("Got user: " + user.name)
    
    # Wait for user's posts
    let posts = await fetchUserPosts(userId)
    println("Got " + str(posts.length) + " posts")
    
    # Return combined data
    return {
        user: user,
        posts: posts
    }
}

# Call async function
getUserInfo(123)
    .then(fn(data) {
        println("Done: " + data.user.name)
    })
```

### Sequential vs Parallel Execution

```klang
# Sequential: Wait for each operation
async fn sequential() {
    let user1 = await fetchUser(1)      # Wait
    let user2 = await fetchUser(2)      # Then wait
    let user3 = await fetchUser(3)      # Then wait
    
    return [user1, user2, user3]
    # Total time: ~3 seconds (1s + 1s + 1s)
}

# Parallel: Start all operations at once
async fn parallel() {
    let [user1, user2, user3] = await Promise.all([
        fetchUser(1),
        fetchUser(2),
        fetchUser(3)
    ])
    
    return [user1, user2, user3]
    # Total time: ~1 second (max of all operations)
}

# Mixed: Some sequential, some parallel
async fn mixed() {
    # First get the user
    let user = await fetchUser(1)
    
    # Then get their data in parallel
    let [posts, comments, likes] = await Promise.all([
        fetchUserPosts(user.id),
        fetchUserComments(user.id),
        fetchUserLikes(user.id)
    ])
    
    return {user, posts, comments, likes}
}
```

## Error Handling

### try/catch with async/await

```klang
async fn fetchWithErrorHandling() {
    try {
        let user = await fetchUser(123)
        println("Success: " + user.name)
        return user
    } catch error {
        println("Error fetching user: " + error)
        return null
    }
}

# Multiple operations
async fn fetchMultipleWithErrors() {
    try {
        let user = await fetchUser(123)
        let posts = await fetchUserPosts(user.id)
        let comments = await fetchPostComments(posts[0].id)
        
        return {user, posts, comments}
    } catch error {
        println("Error in chain: " + error)
        return null
    }
}
```

### finally Block

```klang
async fn fetchWithCleanup() {
    let loading = true
    
    try {
        let data = await fetchData()
        println("Got data: " + data)
        return data
    } catch error {
        println("Error: " + error)
        return null
    } finally {
        loading = false
        println("Cleanup complete")
    }
}

# Practical example
async fn updateUser(userId, updates) {
    showLoadingSpinner()
    
    try {
        let user = await fetch("/api/users/" + str(userId), {
            method: "PUT",
            body: JSON.stringify(updates)
        })
        
        showSuccessMessage("User updated!")
        return user
    } catch error {
        showErrorMessage("Update failed: " + error)
        throw error
    } finally {
        hideLoadingSpinner()
    }
}
```

### Handling Multiple Errors

```klang
async fn fetchWithMultipleHandlers() {
    try {
        let response = await fetch("/api/data")
        
        if response.status != 200 {
            throw "HTTP Error: " + str(response.status)
        }
        
        let data = await response.json()
        
        if !data.success {
            throw "API Error: " + data.message
        }
        
        return data.result
    } catch error {
        if error.contains("Network") {
            println("Network error - please check connection")
        } else if error.contains("HTTP") {
            println("Server error - please try again")
        } else {
            println("Unknown error: " + error)
        }
        
        throw error  # Re-throw for caller to handle
    }
}
```

## Real-World Use Cases

### API Client

```klang
class ApiClient {
    fn constructor(baseUrl) {
        this.baseUrl = baseUrl
        this.token = null
    }
    
    async fn login(username, password) {
        try {
            let response = await fetch(this.baseUrl + "/auth/login", {
                method: "POST",
                headers: {"Content-Type": "application/json"},
                body: JSON.stringify({username, password})
            })
            
            let data = await response.json()
            this.token = data.token
            println("Logged in successfully")
            return data
        } catch error {
            println("Login failed: " + error)
            throw error
        }
    }
    
    async fn get(endpoint) {
        let response = await fetch(this.baseUrl + endpoint, {
            headers: {
                "Authorization": "Bearer " + this.token
            }
        })
        
        return await response.json()
    }
    
    async fn post(endpoint, data) {
        let response = await fetch(this.baseUrl + endpoint, {
            method: "POST",
            headers: {
                "Authorization": "Bearer " + this.token,
                "Content-Type": "application/json"
            },
            body: JSON.stringify(data)
        })
        
        return await response.json()
    }
    
    async fn fetchUserProfile(userId) {
        try {
            let [user, posts, followers] = await Promise.all([
                this.get("/users/" + str(userId)),
                this.get("/users/" + str(userId) + "/posts"),
                this.get("/users/" + str(userId) + "/followers")
            ])
            
            return {
                user: user,
                postCount: posts.length,
                followerCount: followers.length
            }
        } catch error {
            println("Error fetching profile: " + error)
            return null
        }
    }
}

# Usage
async fn main() {
    let api = ApiClient("https://api.example.com")
    
    await api.login("user@example.com", "password123")
    let profile = await api.fetchUserProfile(123)
    
    println(profile.user.name + " has " + str(profile.postCount) + " posts")
}

main()
```

### File Operations

```klang
class FileManager {
    async fn readFile(path) {
        try {
            let content = await fs.readFile(path, "utf8")
            println("Read " + str(content.length) + " bytes from " + path)
            return content
        } catch error {
            println("Failed to read " + path + ": " + error)
            throw error
        }
    }
    
    async fn writeFile(path, content) {
        try {
            await fs.writeFile(path, content, "utf8")
            println("Wrote " + str(content.length) + " bytes to " + path)
        } catch error {
            println("Failed to write " + path + ": " + error)
            throw error
        }
    }
    
    async fn copyFile(source, destination) {
        try {
            let content = await this.readFile(source)
            await this.writeFile(destination, content)
            println("Copied " + source + " to " + destination)
        } catch error {
            println("Copy failed: " + error)
            throw error
        }
    }
    
    async fn processMultipleFiles(paths) {
        let results = []
        
        for let path in paths {
            try {
                let content = await this.readFile(path)
                results.push({path: path, success: true, content: content})
            } catch error {
                results.push({path: path, success: false, error: error})
            }
        }
        
        return results
    }
}

# Usage
async fn processFiles() {
    let fm = FileManager()
    
    let results = await fm.processMultipleFiles([
        "/path/to/file1.txt",
        "/path/to/file2.txt",
        "/path/to/file3.txt"
    ])
    
    let successful = results.filter(r => r.success)
    let failed = results.filter(r => !r.success)
    
    println("Processed " + str(successful.length) + " files successfully")
    println("Failed to process " + str(failed.length) + " files")
}
```

### Database Operations

```klang
class Database {
    fn constructor(connectionString) {
        this.connectionString = connectionString
        this.connection = null
    }
    
    async fn connect() {
        try {
            this.connection = await db.connect(this.connectionString)
            println("Connected to database")
        } catch error {
            println("Connection failed: " + error)
            throw error
        }
    }
    
    async fn query(sql, params) {
        if !this.connection {
            throw "Not connected to database"
        }
        
        try {
            let result = await this.connection.query(sql, params)
            return result.rows
        } catch error {
            println("Query failed: " + error)
            throw error
        }
    }
    
    async fn transaction(operations) {
        try {
            await this.query("BEGIN")
            
            let results = []
            for let operation in operations {
                let result = await this.query(operation.sql, operation.params)
                results.push(result)
            }
            
            await this.query("COMMIT")
            println("Transaction completed successfully")
            return results
        } catch error {
            await this.query("ROLLBACK")
            println("Transaction rolled back: " + error)
            throw error
        }
    }
    
    async fn getUserWithPosts(userId) {
        let [user, posts] = await Promise.all([
            this.query("SELECT * FROM users WHERE id = ?", [userId]),
            this.query("SELECT * FROM posts WHERE user_id = ?", [userId])
        ])
        
        return {
            user: user[0],
            posts: posts
        }
    }
}

# Usage
async fn databaseExample() {
    let db = Database("postgresql://localhost/mydb")
    await db.connect()
    
    try {
        let data = await db.getUserWithPosts(123)
        println(data.user.name + " has " + str(data.posts.length) + " posts")
    } finally {
        await db.close()
    }
}
```

### Retry Logic

```klang
async fn retry(fn, maxAttempts, delay) {
    let attempt = 0
    
    while attempt < maxAttempts {
        try {
            let result = await fn()
            return result
        } catch error {
            attempt = attempt + 1
            
            if attempt >= maxAttempts {
                println("Max retries reached")
                throw error
            }
            
            println("Attempt " + str(attempt) + " failed, retrying in " + str(delay) + "ms")
            await sleep(delay)
            delay = delay * 2  # Exponential backoff
        }
    }
}

# Usage
async fn fetchWithRetry() {
    let data = await retry(
        async () => {
            return await fetch("/api/data")
        },
        3,      # Max 3 attempts
        1000    # Initial delay 1 second
    )
    
    return data
}

# More sophisticated retry
class RetryableRequest {
    fn constructor(maxRetries, baseDelay) {
        this.maxRetries = maxRetries
        this.baseDelay = baseDelay
    }
    
    async fn execute(operation) {
        let lastError = null
        
        for let attempt = 0; attempt < this.maxRetries; attempt++ {
            try {
                let result = await operation()
                
                if attempt > 0 {
                    println("Succeeded on attempt " + str(attempt + 1))
                }
                
                return result
            } catch error {
                lastError = error
                
                if attempt < this.maxRetries - 1 {
                    let delay = this.baseDelay * Math.pow(2, attempt)
                    println("Attempt " + str(attempt + 1) + " failed, retrying in " + str(delay) + "ms")
                    await sleep(delay)
                }
            }
        }
        
        throw "All retries failed: " + lastError
    }
}

# Usage
let retrier = RetryableRequest(5, 1000)
let result = await retrier.execute(async () => {
    return await fetch("/api/unstable-endpoint")
})
```

### Parallel Processing with Concurrency Limit

```klang
async fn processWithLimit(items, processor, limit) {
    let results = []
    let executing = []
    
    for let item in items {
        # Create promise for this item
        let promise = processor(item).then(result => {
            # Remove from executing list when done
            let index = executing.indexOf(promise)
            if index != -1 {
                executing.splice(index, 1)
            }
            return result
        })
        
        results.push(promise)
        executing.push(promise)
        
        # Wait if we hit the limit
        if executing.length >= limit {
            await Promise.race(executing)
        }
    }
    
    # Wait for all remaining
    return await Promise.all(results)
}

# Usage
async fn downloadImages(urls) {
    println("Downloading " + str(urls.length) + " images...")
    
    let results = await processWithLimit(
        urls,
        async (url) => {
            println("Downloading: " + url)
            let image = await fetch(url)
            println("Downloaded: " + url)
            return image
        },
        5  # Max 5 concurrent downloads
    )
    
    println("All downloads complete!")
    return results
}
```

## Advanced Patterns

### Async Iteration

```klang
class AsyncIterator {
    fn constructor(items) {
        this.items = items
        this.index = 0
    }
    
    async fn next() {
        if this.index >= this.items.length {
            return {done: true}
        }
        
        # Simulate async operation
        await sleep(100)
        
        let value = this.items[this.index]
        this.index = this.index + 1
        
        return {value: value, done: false}
    }
}

async fn processAsyncIterator(iterator) {
    let result = await iterator.next()
    
    while !result.done {
        println("Processing: " + result.value)
        result = await iterator.next()
    }
}

# Usage
let iterator = AsyncIterator([1, 2, 3, 4, 5])
await processAsyncIterator(iterator)
```

### Cancellable Async Operations

```klang
class CancellableOperation {
    fn constructor() {
        this.cancelled = false
    }
    
    fn cancel() {
        this.cancelled = true
        println("Operation cancelled")
    }
    
    async fn execute(operation) {
        if this.cancelled {
            throw "Operation was cancelled"
        }
        
        try {
            let result = await operation()
            
            if this.cancelled {
                throw "Operation was cancelled"
            }
            
            return result
        } catch error {
            if this.cancelled {
                throw "Operation was cancelled"
            }
            throw error
        }
    }
}

# Usage
let operation = CancellableOperation()

# Start long-running operation
let promise = operation.execute(async () => {
    await sleep(5000)
    return "Done"
})

# Cancel after 2 seconds
setTimeout(() => {
    operation.cancel()
}, 2000)

try {
    let result = await promise
    println(result)
} catch error {
    println("Caught: " + error)  # Operation was cancelled
}
```

### Async Queue

```klang
class AsyncQueue {
    fn constructor() {
        this.queue = []
        this.processing = false
    }
    
    async fn add(task) {
        this.queue.push(task)
        
        if !this.processing {
            await this.process()
        }
    }
    
    async fn process() {
        if this.processing {
            return
        }
        
        this.processing = true
        
        while this.queue.length > 0 {
            let task = this.queue.shift()
            
            try {
                await task()
            } catch error {
                println("Task failed: " + error)
            }
        }
        
        this.processing = false
    }
}

# Usage
let queue = AsyncQueue()

queue.add(async () => {
    println("Task 1 starting")
    await sleep(1000)
    println("Task 1 complete")
})

queue.add(async () => {
    println("Task 2 starting")
    await sleep(500)
    println("Task 2 complete")
})

queue.add(async () => {
    println("Task 3 starting")
    await sleep(750)
    println("Task 3 complete")
})
```

## Common Patterns

### Loading States

```klang
class DataLoader {
    fn constructor() {
        this.loading = false
        this.error = null
        this.data = null
    }
    
    async fn load(fetcher) {
        this.loading = true
        this.error = null
        
        try {
            this.data = await fetcher()
        } catch error {
            this.error = error
        } finally {
            this.loading = false
        }
        
        return this.data
    }
}

# Usage
let loader = DataLoader()
await loader.load(async () => {
    return await fetch("/api/users")
})

if loader.error {
    println("Error: " + loader.error)
} else {
    println("Loaded " + str(loader.data.length) + " users")
}
```

### Debounced Async

```klang
fn debounceAsync(fn, delay) {
    let timeoutId = null
    let pendingPromise = null
    
    return async fn(...args) {
        if timeoutId {
            clearTimeout(timeoutId)
        }
        
        return Promise(fn(resolve, reject) {
            timeoutId = setTimeout(async () => {
                try {
                    let result = await fn(...args)
                    resolve(result)
                } catch error {
                    reject(error)
                }
            }, delay)
        })
    }
}

# Usage
let searchUsers = debounceAsync(async (query) => {
    return await fetch("/api/users/search?q=" + query)
}, 500)

# Only last call within 500ms will execute
await searchUsers("a")
await searchUsers("al")
await searchUsers("ali")
await searchUsers("alic")
let results = await searchUsers("alice")
```

### Timeout Wrapper

```klang
async fn withTimeout(promise, ms) {
    let timeoutPromise = Promise(fn(_, reject) {
        setTimeout(fn() {
            reject("Operation timed out after " + str(ms) + "ms")
        }, ms)
    })
    
    return await Promise.race([promise, timeoutPromise])
}

# Usage
try {
    let data = await withTimeout(
        fetch("/api/slow-endpoint"),
        5000  # 5 second timeout
    )
    println("Got data: " + data)
} catch error {
    println("Error: " + error)
}
```

## Best Practices

### 1. Always Handle Errors

```klang
# Good
async fn fetchData() {
    try {
        let result = await fetch("/api/data")
        return result
    } catch error {
        console.error("Fetch failed:", error)
        return null
    }
}

# Avoid
async fn fetchData() {
    let result = await fetch("/api/data")  # Unhandled errors
    return result
}
```

### 2. Use Promise.all() for Parallel Operations

```klang
# Good: Parallel
async fn loadDashboard() {
    let [users, posts, stats] = await Promise.all([
        fetchUsers(),
        fetchPosts(),
        fetchStats()
    ])
    return {users, posts, stats}
}

# Avoid: Sequential when parallel would work
async fn loadDashboard() {
    let users = await fetchUsers()
    let posts = await fetchPosts()
    let stats = await fetchStats()
    return {users, posts, stats}
}
```

### 3. Don't Forget await

```klang
# Wrong: Missing await
async fn getData() {
    let data = fetchData()  # Returns promise, not data!
    return data.length  # Error!
}

# Correct
async fn getData() {
    let data = await fetchData()
    return data.length
}
```

### 4. Use finally for Cleanup

```klang
# Good
async fn processData() {
    showLoader()
    
    try {
        let data = await fetchData()
        return processData(data)
    } catch error {
        showError(error)
    } finally {
        hideLoader()  # Always executes
    }
}
```

### 5. Be Careful with Loops

```klang
# Sequential (slower but preserves order)
async fn processSequential(items) {
    let results = []
    for let item in items {
        let result = await processItem(item)
        results.push(result)
    }
    return results
}

# Parallel (faster)
async fn processParallel(items) {
    let promises = items.map(item => processItem(item))
    return await Promise.all(promises)
}
```

## Common Mistakes to Avoid

### 1. Not Returning Promises

```klang
# Wrong
async fn getData() {
    fetch("/api/data")  # Missing return/await
}

# Correct
async fn getData() {
    return await fetch("/api/data")
}
```

### 2. Using async Without await

```klang
# Unnecessary async
async fn add(a, b) {
    return a + b  # No async operations
}

# Better
fn add(a, b) {
    return a + b
}
```

### 3. Not Handling Promise Rejections

```klang
# Problematic
async fn main() {
    fetchData()  # Unhandled promise rejection!
}

# Better
async fn main() {
    try {
        await fetchData()
    } catch error {
        console.error(error)
    }
}
```

## Next Steps

Now that you understand async/await, explore these topics:

- **[Promises](17-promises.md)** - Deep dive into promises
- **[Error Handling](16-error-handling.md)** - Advanced error patterns
- **[Arrow Functions](21-arrow-functions.md)** - Async arrow functions
- **[API Integration](35-api-integration.md)** - Working with APIs
- **[Testing Async Code](40-testing.md)** - Test async functions

Ready to write better async code? Start using async/await in your projects today!
