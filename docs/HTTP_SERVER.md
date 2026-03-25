# Built-in HTTP Server Guide

## Overview

KLang includes a built-in HTTP server that makes it easy to serve web applications, APIs, and static files without needing external dependencies like Node.js or Python.

## Quick Start

### Serve Static Files

Serve the current directory on port 8080:

```bash
klang serve
```

Serve on a custom port:

```bash
klang serve --port=3000
# or
klang serve -p 3000
```

Serve a specific directory:

```bash
klang serve --dir=./public --port=8080
```

### Enable Hot Reload (Development Mode)

Enable automatic file watching and hot reload:

```bash
klang serve --hot-reload
# or
klang serve --watch
```

Or use development mode (hot reload enabled automatically):

```bash
klang serve --mode=dev
```

## Using HTTP Server in KLang Code

### Basic Server

Create a simple HTTP server in your KLang code:

```klang
import http from "stdlib/net"

let server = http.createServer(8080)

server.get("/", (req, res) => {
    res.send("Hello, World!")
})

server.get("/api/status", (req, res) => {
    res.json({
        status: "ok",
        version: "1.0.0"
    })
})

server.start()
println("Server listening on http://localhost:8080")
```

### Route Handling

Define routes for different HTTP methods:

```klang
import http from "stdlib/net"

let server = http.createServer(3000)

# GET request
server.get("/users", (req, res) => {
    res.json([
        {id: 1, name: "Alice"},
        {id: 2, name: "Bob"}
    ])
})

# POST request
server.post("/users", (req, res) => {
    let user = req.body
    # Save user to database
    res.status(201).json(user)
})

# PUT request
server.put("/users/:id", (req, res) => {
    let id = req.params.id
    # Update user
    res.json({message: "User updated"})
})

# DELETE request
server.delete("/users/:id", (req, res) => {
    let id = req.params.id
    # Delete user
    res.status(204).send()
})

server.start()
```

### Middleware

Add middleware for logging, authentication, etc:

```klang
import http from "stdlib/net"

let server = http.createServer(3000)

# Logging middleware
server.use((req, res, next) => {
    println("[${req.method}] ${req.path}")
    next()
})

# Authentication middleware
server.use((req, res, next) => {
    let token = req.headers["authorization"]
    if (!token) {
        res.status(401).json({error: "Unauthorized"})
        return
    }
    next()
})

# CORS middleware
server.use((req, res, next) => {
    res.setHeader("Access-Control-Allow-Origin", "*")
    res.setHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE")
    next()
})

server.get("/api/data", (req, res) => {
    res.json({data: "secure data"})
})

server.start()
```

### Static File Serving

Serve static files from a directory:

```klang
import http from "stdlib/net"

let server = http.createServer(8080)

# Serve static files from ./public directory
server.static("/", "./public")

# Or serve from multiple directories
server.static("/assets", "./assets")
server.static("/images", "./images")

# API routes
server.get("/api/hello", (req, res) => {
    res.json({message: "Hello from API"})
})

server.start()
```

### Request Object

Access request information:

```klang
server.get("/api/info", (req, res) => {
    println("Method: ${req.method}")
    println("Path: ${req.path}")
    println("Query: ${req.query}")
    println("Headers: ${req.headers}")
    println("Body: ${req.body}")
    
    # Get specific header
    let userAgent = req.getHeader("User-Agent")
    
    # Get query parameter
    let name = req.getQuery("name")
    
    res.send("Request processed")
})
```

### Response Object

Send different types of responses:

```klang
# Send text
server.get("/text", (req, res) => {
    res.send("Plain text response")
})

# Send JSON
server.get("/json", (req, res) => {
    res.json({
        message: "JSON response",
        data: [1, 2, 3]
    })
})

# Send file
server.get("/download", (req, res) => {
    res.sendFile("./files/document.pdf")
})

# Set status code
server.get("/notfound", (req, res) => {
    res.status(404).json({error: "Not found"})
})

# Set headers
server.get("/custom", (req, res) => {
    res.setHeader("X-Custom-Header", "Value")
    res.setHeader("Cache-Control", "no-cache")
    res.send("Response with custom headers")
})

# Redirect
server.get("/old-url", (req, res) => {
    res.redirect("/new-url")
})
```

### Hot Reload (Development Mode)

Enable hot reload for development:

```klang
import http from "stdlib/net"

let server = http.createServer(3000)

# Enable hot reload
if (env("KLANG_BUILD_MODE") == "dev") {
    server.enableHotReload(true)
    server.watchDirectory("./src")
}

server.get("/", (req, res) => {
    res.sendFile("./index.html")
})

server.start()
```

### WebSocket Support

Create WebSocket endpoints:

```klang
import http from "stdlib/net"

let server = http.createServer(8080)

# WebSocket endpoint
server.websocket("/ws", {
    onConnect: (conn) => {
        println("Client connected")
    },
    
    onMessage: (conn, msg) => {
        println("Received: ${msg}")
        conn.send("Echo: ${msg}")
    },
    
    onDisconnect: (conn) => {
        println("Client disconnected")
    }
})

server.start()
```

### Complete Example

A complete REST API with static file serving:

```klang
import http from "stdlib/net"

let server = http.createServer(3000)

# Database (in-memory for this example)
let users = [
    {id: 1, name: "Alice", email: "alice@example.com"},
    {id: 2, name: "Bob", email: "bob@example.com"}
]

# Middleware
server.use((req, res, next) => {
    println("[${new Date()}] ${req.method} ${req.path}")
    next()
})

# Static files
server.static("/", "./public")

# API Routes
server.get("/api/users", (req, res) => {
    res.json(users)
})

server.get("/api/users/:id", (req, res) => {
    let id = parseInt(req.params.id)
    let user = users.find(u => u.id == id)
    
    if (user) {
        res.json(user)
    } else {
        res.status(404).json({error: "User not found"})
    }
})

server.post("/api/users", (req, res) => {
    let newUser = req.body
    newUser.id = users.length + 1
    users.push(newUser)
    res.status(201).json(newUser)
})

server.put("/api/users/:id", (req, res) => {
    let id = parseInt(req.params.id)
    let index = users.findIndex(u => u.id == id)
    
    if (index >= 0) {
        users[index] = {...users[index], ...req.body}
        res.json(users[index])
    } else {
        res.status(404).json({error: "User not found"})
    }
})

server.delete("/api/users/:id", (req, res) => {
    let id = parseInt(req.params.id)
    users = users.filter(u => u.id != id)
    res.status(204).send()
})

# Start server
server.start()
println("Server running on http://localhost:3000")
```

## Command Line Options

| Option | Description | Example |
|--------|-------------|---------|
| `--port=N` or `-p N` | Set server port | `klang serve --port=8080` |
| `--dir=PATH` | Set directory to serve | `klang serve --dir=./public` |
| `--hot-reload` or `--watch` | Enable hot reload | `klang serve --hot-reload` |
| `--mode=MODE` | Set build mode | `klang serve --mode=dev` |

## Environment Variables

```bash
# Port
export HTTP_SERVER_PORT=8080

# Directory
export HTTP_SERVER_DIR=./public

# Hot reload
export HTTP_SERVER_HOT_RELOAD=1
```

## Use Cases

### 1. Development Server

```bash
# Start dev server with hot reload
klang serve --mode=dev --hot-reload --port=3000
```

### 2. Static Site Hosting

```bash
# Serve a static website
klang serve --dir=./dist --port=8080
```

### 3. REST API

Create `api.kl`:
```klang
import http from "stdlib/net"

let server = http.createServer(3000)
server.get("/api/hello", (req, res) => {
    res.json({message: "Hello, World!"})
})
server.start()
```

Run:
```bash
klang run api.kl
```

### 4. Microservice

Create `microservice.kl`:
```klang
import http from "stdlib/net"

let server = http.createServer(8080)

server.get("/health", (req, res) => {
    res.json({status: "healthy"})
})

server.get("/api/v1/data", (req, res) => {
    # Fetch data from database
    res.json({data: []})
})

server.start()
```

## Best Practices

1. **Use middleware for common functionality** (logging, auth, CORS)
2. **Enable hot reload in development** for faster iteration
3. **Use production mode for deployment** for better performance
4. **Implement proper error handling** in route handlers
5. **Set appropriate status codes** in responses
6. **Use environment variables** for configuration

## Performance Tips

- Use `--mode=production` for production deployments
- Enable LTO for better performance: `--lto`
- Serve static files directly without middleware
- Use caching headers for static assets
- Implement request compression

## Troubleshooting

### Port already in use

```bash
# Use a different port
klang serve --port=3001
```

### Permission denied (port < 1024)

```bash
# Use sudo or a port >= 1024
sudo klang serve --port=80
# or
klang serve --port=8080
```

### Hot reload not working

```bash
# Make sure you're in dev mode
klang serve --mode=dev --hot-reload
```

## See Also

- [Build Modes Guide](BUILD_MODES.md)
- [API Reference](API_REFERENCE.md)
- [Network Programming](../stdlib/net.kl)
