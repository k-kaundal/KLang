# JSON Support

## Introduction

KLang has built-in JSON support for parsing and serializing JSON data. This is essential for:
- Working with REST APIs
- Configuration files
- Data storage
- Inter-process communication

## JSON Functions

### `json_parse()` - Parse JSON String

```klang
# Parse JSON string to object
let json_str = '{"name": "Alice", "age": 30}'
let obj = json_parse(json_str)

println(obj["name"])  # "Alice"
println(obj["age"])   # 30
```

### `json_stringify()` - Convert to JSON

```klang
# Convert object to JSON string
let person = {
    "name": "Bob",
    "age": 25,
    "city": "NYC"
}

let json = json_stringify(person)
println(json)  # {"name":"Bob","age":25,"city":"NYC"}
```

## Working with JSON

### Parsing API Responses

```klang
# Fetch and parse JSON from API
let response = fetch("https://api.example.com/users")
let data = json_parse(response)

for user in data["users"] {
    println(user["name"] + " - " + user["email"])
}
```

### Nested JSON Objects

```klang
let json = '{
    "user": {
        "name": "Alice",
        "address": {
            "city": "NYC",
            "zip": "10001"
        }
    }
}'

let data = json_parse(json)
println(data["user"]["address"]["city"])  # "NYC"
```

### JSON Arrays

```klang
let json = '[{"id": 1, "name": "Item 1"}, {"id": 2, "name": "Item 2"}]'
let items = json_parse(json)

for item in items {
    println("ID: " + str(item["id"]) + ", Name: " + item["name"])
}
```

## Common Patterns

### Configuration Loading

```klang
# Load JSON config file
let config_json = read_file("config.json")
let config = json_parse(config_json)

let db_host = config["database"]["host"]
let db_port = config["database"]["port"]
println("DB: " + db_host + ":" + str(db_port))
```

### API Client

```klang
# REST API client with JSON
class APIClient {
    constructor(base_url, api_key) {
        this.base_url = base_url
        this.api_key = api_key
    }
    
    get(endpoint) {
        let url = this.base_url + endpoint
        let response = fetch(url, {
            headers: {"Authorization": "Bearer " + this.api_key}
        })
        return json_parse(response)
    }
    
    post(endpoint, data) {
        let url = this.base_url + endpoint
        let body = json_stringify(data)
        let response = fetch(url, {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
                "Authorization": "Bearer " + this.api_key
            },
            body: body
        })
        return json_parse(response)
    }
}

let api = APIClient("https://api.example.com", env("API_KEY"))
let users = api.get("/users")
println("Found " + str(len(users)) + " users")
```

## Best Practices

### 1. Always Handle Parse Errors

```klang
# Wrap in try-catch (when available)
let json_str = '{"invalid json'
let data = json_parse(json_str)  # May fail!

# Better: validate first
if json_str {
    let data = json_parse(json_str)
}
```

### 2. Pretty Print JSON

```klang
# Format JSON for readability
fn pretty_json(obj) {
    return json_stringify(obj)  # Add formatting when available
}

let data = {"name": "Alice", "age": 30}
println(pretty_json(data))
```

## Next Steps

- [HTTP Client](40-http-client.md) - Use JSON with REST APIs
- [File I/O](41-file-io.md) - Save/load JSON files
- [Built-in Functions](50-built-in-functions.md) - All built-ins

---

**Work with JSON data effortlessly!** 📦
