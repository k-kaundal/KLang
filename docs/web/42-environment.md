# Environment Variables

## Introduction

Environment variables are a crucial part of configuration management in KLang. They allow you to:
- Store sensitive data (API keys, passwords) outside your code
- Configure applications for different environments (dev, staging, production)
- Share configuration across multiple programs

## The `env()` Function

KLang provides the built-in `env()` function to access environment variables.

### Basic Usage

```klang
# Get environment variable
let api_key = env("API_KEY")
println("API Key: " + api_key)

# Check if variable exists
let db_url = env("DATABASE_URL")
if db_url == null {
    println("DATABASE_URL not set!")
}
```

## Setting Environment Variables

### From Command Line

```bash
# Set for single command
API_KEY="your-key" ./klang run app.kl

# Set for session
export API_KEY="your-key"
export DATABASE_URL="postgres://localhost/mydb"
./klang run app.kl
```

### Using .env Files

```bash
# Create .env file
cat > .env << EOF
API_KEY=sk-1234567890
DATABASE_URL=postgres://localhost/mydb
DEBUG=true
PORT=3000
EOF

# Load and run (using script)
export $(cat .env | xargs) && ./klang run app.kl
```

## Common Patterns

### Configuration Management

```klang
# config.kl
class Config {
    constructor() {
        this.api_key = env("API_KEY")
        this.db_url = env("DATABASE_URL")
        this.debug = env("DEBUG") == "true"
        this.port = int(env("PORT") or "3000")
    }
    
    validate() {
        if !this.api_key {
            throw "API_KEY not set"
        }
        if !this.db_url {
            throw "DATABASE_URL not set"
        }
    }
}

let config = Config()
config.validate()
println("Config loaded: port=" + str(config.port))
```

### Default Values

```klang
# Provide defaults for optional variables
let port = env("PORT") or "8080"
let host = env("HOST") or "localhost"
let debug = env("DEBUG") or "false"

println("Server: " + host + ":" + port)
```

### API Key Management

```klang
# Secure API key handling
fn get_api_key(service) {
    let key_name = service + "_API_KEY"
    let key = env(key_name)
    
    if !key {
        throw "Missing API key: " + key_name
    }
    
    return key
}

let openai_key = get_api_key("OPENAI")
let anthropic_key = get_api_key("ANTHROPIC")
```

## Best Practices

### 1. Never Hardcode Secrets

❌ **BAD:**
```klang
let api_key = "sk-1234567890abcdef"  # Never do this!
```

✅ **GOOD:**
```klang
let api_key = env("API_KEY")
if !api_key {
    throw "API_KEY environment variable not set"
}
```

### 2. Validate Required Variables

```klang
fn validate_env() {
    let required = ["API_KEY", "DATABASE_URL", "SECRET_KEY"]
    
    for var_name in required {
        if !env(var_name) {
            throw "Required environment variable not set: " + var_name
        }
    }
}

validate_env()
```

### 3. Use Descriptive Names

```klang
# Use clear, specific names
let stripe_key = env("STRIPE_SECRET_KEY")  # Clear
let key = env("KEY")  # Too vague
```

## Next Steps

- [JSON Support](43-json.md) - Parse JSON configuration
- [HTTP Client](40-http-client.md) - Use API keys with HTTP
- [AI Overview](30-ai-overview.md) - Configure AI services

---

**Keep your secrets safe with environment variables!** 🔐
