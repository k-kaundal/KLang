# Server Creation and Code Generation Guide

## Overview

KLang now includes NestJS-inspired server creation and code generation features that make it easy to build modern, well-structured server applications.

## Quick Start

### Create a New Server Project

```bash
# Create a new server project
klang new server my-api

# With git initialization
klang new server my-api --git
```

This creates a complete server project structure:

```
my-api/
├── src/
│   ├── main.kl                  # Application entry point
│   ├── app.kl                   # Application setup
│   ├── controllers/             # Request handlers
│   ├── services/                # Business logic
│   ├── middleware/              # Custom middleware
│   │   └── logger.middleware.kl # Example logger middleware
│   ├── models/                  # Data models
│   ├── routes/                  # Route definitions
│   │   └── api.routes.kl        # Example API routes
│   └── config/                  # Configuration
├── tests/
│   ├── unit/                    # Unit tests
│   └── integration/             # Integration tests
├── public/                      # Static files
│   └── index.html              # Welcome page
├── .env                         # Environment variables
├── .gitignore                   # Git ignore file
├── package.kl.json              # Package manifest
└── README.md                    # Project documentation
```

## Code Generators

KLang provides powerful code generators to scaffold common server components.

### Generate Commands

All commands can use the full name or short alias:
- `klang generate` or `klang g`

### Available Generators

#### 1. Generate a Module (Controller + Service + Routes)

```bash
# Generate a complete module
klang generate module users

# Short form
klang g m users
```

This creates:
- `src/controllers/users.controller.kl`
- `src/services/users.service.kl`
- `src/routes/users.routes.kl`

#### 2. Generate a Controller

```bash
# Generate a controller
klang generate controller products

# Short form
klang g c products
```

Creates a controller with CRUD methods:
- `getAll()` - GET /products
- `getById()` - GET /products/:id
- `create()` - POST /products
- `update()` - PUT /products/:id
- `delete()` - DELETE /products/:id

#### 3. Generate a Service

```bash
# Generate a service
klang generate service products

# Short form
klang g s products
```

Creates a service class with data management methods:
- `findAll()`
- `findById(id)`
- `create(data)`
- `update(id, data)`
- `delete(id)`

#### 4. Generate Middleware

```bash
# Generate middleware
klang generate middleware auth

# Short form
klang g mw auth
```

Creates a middleware function with request/response handling.

#### 5. Generate Routes

```bash
# Generate a route file
klang generate route api

# Short form
klang g r api
```

Creates a route file with a router instance.

#### 6. Generate Guards

```bash
# Generate an authentication guard
klang generate guard auth
```

Creates a guard for authorization/authentication.

#### 7. Generate Interceptors

```bash
# Generate an interceptor
klang generate interceptor logging

# Short form
klang g i logging
```

Creates an interceptor for request/response transformation.

## Example: Building a REST API

### Step 1: Create the Server Project

```bash
klang new server blog-api --git
cd blog-api
```

### Step 2: Generate Modules

```bash
# Generate posts module
klang g module posts

# Generate users module
klang g module users

# Generate comments module
klang g module comments
```

### Step 3: Generate Additional Components

```bash
# Generate authentication middleware
klang g middleware auth

# Generate validation middleware
klang g middleware validate

# Generate logging interceptor
klang g interceptor logging
```

### Step 4: Run the Server

```bash
# Development mode (with hot reload)
klang run src/main.kl --mode=dev

# Production mode
klang run src/main.kl --mode=production
```

## Standard Library Additions

### Server Framework (`stdlib/server.kl`)

The server library provides a complete framework for building web applications:

```klang
import server from "stdlib/server"

let app = server.createApp()

# Middleware
app.use(server.cors())
app.use(server.json())
app.use(server.logger())

# Routes
app.get("/", (req, res) => {
    res.json({ message: "Hello, World!" })
})

# Start server
app.listen(3000)
```

#### Available Middleware

- `server.cors(options)` - CORS middleware
- `server.json(options)` - JSON body parser
- `server.urlencoded(options)` - URL-encoded body parser
- `server.logger(format)` - Request logging
- `server.helmet(options)` - Security headers
- `server.rateLimit(options)` - Rate limiting
- `server.compression(options)` - Response compression
- `server.cookieParser(secret)` - Cookie parsing
- `server.session(options)` - Session management

### Validation Library (`stdlib/validation.kl`)

Comprehensive validation utilities:

```klang
import validation from "stdlib/validation"

# Define validation schema
let userSchema = {
    email: [
        validation.required(),
        validation.email()
    ],
    password: [
        validation.required(),
        validation.string({ minLength: 8, maxLength: 100 })
    ],
    age: [
        validation.number({ min: 18, max: 120 })
    ]
}

# Validate data
let validator = validation.Validator(userSchema)
let result = validator.validate(userData)

if (!result.isValid) {
    println("Validation errors:", result.errors)
}
```

#### Validation Rules

- `required()` - Field is required
- `string(options)` - String validation with min/max length
- `number(options)` - Number validation with min/max
- `email()` - Email format validation
- `url()` - URL format validation
- `pattern(regex)` - Pattern matching
- `enum(values)` - Enumeration validation
- `custom(validator)` - Custom validation function

#### Validation Middleware

```klang
import validation from "stdlib/validation"

let createUserSchema = {
    email: [validation.required(), validation.email()],
    name: [validation.required(), validation.string({ minLength: 2 })]
}

app.post("/users", 
    validation.validateBody(createUserSchema),
    (req, res) => {
        # Request body is validated at this point
        res.json({ message: "User created" })
    }
)
```

### Decorators Library (`stdlib/decorators.kl`)

Decorator patterns for advanced use cases:

```klang
import decorators from "stdlib/decorators"

# Controller decorator
@decorators.Controller("/users")
class UsersController {
    @decorators.Get("/")
    @decorators.UseMiddleware(authMiddleware)
    getAll(req, res) {
        res.json({ users: [] })
    }
    
    @decorators.Post("/")
    @decorators.Validate(userSchema)
    create(req, res) {
        res.status(201).json({ message: "Created" })
    }
}
```

## Best Practices

### 1. Project Structure

Keep your code organized:
- **Controllers**: Handle HTTP requests and responses
- **Services**: Implement business logic
- **Middleware**: Process requests before reaching controllers
- **Routes**: Define API endpoints
- **Models**: Define data structures

### 2. Separation of Concerns

```klang
# Bad: Business logic in controller
class UserController {
    create(req, res) {
        let user = {
            id: generateId(),
            ...req.body,
            createdAt: Date.now()
        }
        database.save(user)
        res.json(user)
    }
}

# Good: Business logic in service
class UserService {
    create(data) {
        let user = {
            id: this.generateId(),
            ...data,
            createdAt: Date.now()
        }
        return this.database.save(user)
    }
}

class UserController {
    constructor(userService) {
        this.userService = userService
    }
    
    create(req, res) {
        let user = this.userService.create(req.body)
        res.status(201).json(user)
    }
}
```

### 3. Error Handling

```klang
# Global error handler
app.useErrorHandler((err, req, res) => {
    println("Error:", err.message)
    
    res.status(err.statusCode || 500).json({
        error: err.message,
        timestamp: Date.now()
    })
})
```

### 4. Validation

Always validate user input:

```klang
import validation from "stdlib/validation"

let schema = {
    email: [validation.required(), validation.email()],
    password: [validation.required(), validation.string({ minLength: 8 })]
}

app.post("/register", 
    validation.validateBody(schema),
    (req, res) => {
        # Validated data
        userService.register(req.body)
        res.json({ success: true })
    }
)
```

## Command Reference

### Project Commands

```bash
# Create new server project
klang new server <name> [--git]

# Create new basic app
klang new app <name> [--git]

# Traditional init (still available)
klang init <name> [--git]
```

### Generator Commands

```bash
# Full names
klang generate module <name>
klang generate controller <name>
klang generate service <name>
klang generate middleware <name>
klang generate route <name>
klang generate guard <name>
klang generate interceptor <name>

# Short aliases
klang g m <name>      # module
klang g c <name>      # controller
klang g s <name>      # service
klang g mw <name>     # middleware
klang g r <name>      # route
klang g i <name>      # interceptor
```

### Development Commands

```bash
# Run in development mode
klang run src/main.kl --mode=dev

# Run with hot reload
klang run src/main.kl --mode=dev --hot-reload

# Run tests
klang test tests/

# Format code
klang fmt src/

# Type check
klang typecheck src/main.kl
```

## Comparison with NestJS

| Feature | NestJS | KLang |
|---------|--------|-------|
| Project Creation | `nest new` | `klang new server` |
| Generate Module | `nest g module` | `klang g module` |
| Generate Controller | `nest g controller` | `klang g controller` |
| Generate Service | `nest g service` | `klang g service` |
| Decorators | TypeScript decorators | Decorator library |
| Dependency Injection | Built-in | Manual (can use decorators) |
| Validation | class-validator | stdlib/validation.kl |
| Middleware | @Injectable() | Function-based |
| Guards | @UseGuards() | Guard generators |
| Interceptors | @UseInterceptors() | Interceptor generators |

## Next Steps

1. **Explore Examples**: Check out the generated code to understand the patterns
2. **Read Documentation**: See [API_REFERENCE.md](API_REFERENCE.md) for detailed API docs
3. **Build Your API**: Start creating your server application
4. **Deploy**: Use production mode for deployment

## Getting Help

```bash
# General help
klang help

# Command-specific help
klang help new
klang help generate

# Show version
klang version
```

For more information, visit the [KLang documentation](../docs/).
