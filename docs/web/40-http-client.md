# HTTP Client

## Introduction

KLang provides powerful built-in HTTP/HTTPS capabilities for making web requests, consuming REST APIs, and handling responses. This guide covers everything you need to work with HTTP in KLang.

## Table of Contents

- [Basic HTTP Requests](#basic-http-requests)
- [HTTP Methods](#http-methods)
- [Headers and Authentication](#headers-and-authentication)
- [Request Body](#request-body)
- [Response Handling](#response-handling)
- [Error Handling](#error-handling)
- [REST API Integration](#rest-api-integration)
- [Advanced Features](#advanced-features)
- [Best Practices](#best-practices)

## Basic HTTP Requests

### Simple GET Request

```klang
// Basic GET request
let response = http.get("https://api.example.com/users");
println(response.body);
```

### GET with URL Parameters

```klang
let params = {
    page: 1,
    limit: 10,
    sort: "name"
};

let url = "https://api.example.com/users?" + http.buildQuery(params);
let response = http.get(url);
println(response.body);
```

### GET with Custom Headers

```klang
let headers = {
    "User-Agent": "KLang/1.0",
    "Accept": "application/json"
};

let response = http.get("https://api.example.com/data", { headers: headers });
println(response.status);
println(response.body);
```

## HTTP Methods

### POST Request

```klang
let data = {
    name: "John Doe",
    email: "john@example.com",
    age: 30
};

let response = http.post("https://api.example.com/users", {
    body: JSON.stringify(data),
    headers: {
        "Content-Type": "application/json"
    }
});

println("Status:", response.status);
println("Response:", response.body);
```

### PUT Request (Update)

```klang
let userId = 123;
let updatedData = {
    name: "Jane Doe",
    email: "jane@example.com"
};

let response = http.put(
    "https://api.example.com/users/" + userId,
    {
        body: JSON.stringify(updatedData),
        headers: { "Content-Type": "application/json" }
    }
);

if (response.status == 200) {
    println("User updated successfully");
}
```

### PATCH Request (Partial Update)

```klang
let userId = 123;
let partialUpdate = {
    email: "newemail@example.com"
};

let response = http.patch(
    "https://api.example.com/users/" + userId,
    {
        body: JSON.stringify(partialUpdate),
        headers: { "Content-Type": "application/json" }
    }
);
```

### DELETE Request

```klang
let userId = 123;
let response = http.delete("https://api.example.com/users/" + userId);

if (response.status == 204) {
    println("User deleted successfully");
}
```

### HEAD Request

```klang
// Get headers without body
let response = http.head("https://api.example.com/large-file");
println("Content-Length:", response.headers["Content-Length"]);
println("Content-Type:", response.headers["Content-Type"]);
```

### OPTIONS Request

```klang
// Check allowed methods
let response = http.options("https://api.example.com/users");
println("Allowed methods:", response.headers["Allow"]);
```

## Headers and Authentication

### Basic Authentication

```klang
let username = "user";
let password = "pass";
let credentials = btoa(username + ":" + password);

let response = http.get("https://api.example.com/protected", {
    headers: {
        "Authorization": "Basic " + credentials
    }
});
```

### Bearer Token Authentication

```klang
let token = "your-jwt-token-here";

let response = http.get("https://api.example.com/protected", {
    headers: {
        "Authorization": "Bearer " + token
    }
});
```

### API Key Authentication

```klang
// API key in header
let response = http.get("https://api.example.com/data", {
    headers: {
        "X-API-Key": "your-api-key-here"
    }
});

// API key in query parameter
let url = "https://api.example.com/data?apikey=your-api-key-here";
let response2 = http.get(url);
```

### Custom Headers

```klang
let headers = {
    "User-Agent": "KLang Bot/1.0",
    "Accept": "application/json",
    "Accept-Language": "en-US",
    "Cache-Control": "no-cache",
    "X-Custom-Header": "custom-value"
};

let response = http.get("https://api.example.com/data", {
    headers: headers
});
```

### OAuth 2.0 Flow

```klang
// Step 1: Get access token
fn getAccessToken(clientId, clientSecret) {
    let tokenUrl = "https://oauth.example.com/token";
    let response = http.post(tokenUrl, {
        body: "grant_type=client_credentials&client_id=" + clientId + 
              "&client_secret=" + clientSecret,
        headers: {
            "Content-Type": "application/x-www-form-urlencoded"
        }
    });
    
    let data = JSON.parse(response.body);
    return data.access_token;
}

// Step 2: Use access token
let token = getAccessToken("my-client-id", "my-client-secret");
let response = http.get("https://api.example.com/data", {
    headers: {
        "Authorization": "Bearer " + token
    }
});
```

## Request Body

### JSON Body

```klang
let user = {
    name: "Alice",
    email: "alice@example.com",
    roles: ["user", "admin"]
};

let response = http.post("https://api.example.com/users", {
    body: JSON.stringify(user),
    headers: {
        "Content-Type": "application/json"
    }
});
```

### Form Data

```klang
let formData = "name=John&email=john@example.com&age=30";

let response = http.post("https://api.example.com/users", {
    body: formData,
    headers: {
        "Content-Type": "application/x-www-form-urlencoded"
    }
});
```

### Multipart Form Data

```klang
// File upload
let boundary = "----KLangFormBoundary" + Math.random();
let body = "--" + boundary + "\r\n" +
           "Content-Disposition: form-data; name=\"file\"; filename=\"document.txt\"\r\n" +
           "Content-Type: text/plain\r\n\r\n" +
           "File contents here\r\n" +
           "--" + boundary + "--";

let response = http.post("https://api.example.com/upload", {
    body: body,
    headers: {
        "Content-Type": "multipart/form-data; boundary=" + boundary
    }
});
```

### Raw Binary Data

```klang
let binaryData = readFile("image.png", "binary");

let response = http.post("https://api.example.com/upload-image", {
    body: binaryData,
    headers: {
        "Content-Type": "image/png"
    }
});
```

## Response Handling

### Response Object Structure

```klang
let response = http.get("https://api.example.com/data");

// Response properties
println("Status:", response.status);        // 200
println("Status Text:", response.statusText); // "OK"
println("Body:", response.body);            // Response body as string
println("Headers:", response.headers);      // Headers object
```

### Parsing JSON Response

```klang
let response = http.get("https://api.example.com/users");

if (response.status == 200) {
    let users = JSON.parse(response.body);
    
    for (let user of users) {
        println(user.name + " - " + user.email);
    }
}
```

### Accessing Response Headers

```klang
let response = http.get("https://api.example.com/data");

// Individual headers
let contentType = response.headers["Content-Type"];
let contentLength = response.headers["Content-Length"];
let rateLimit = response.headers["X-RateLimit-Remaining"];

println("Content-Type:", contentType);
println("Rate Limit Remaining:", rateLimit);

// All headers
for (let key in response.headers) {
    println(key + ":", response.headers[key]);
}
```

### Checking Status Codes

```klang
let response = http.get("https://api.example.com/data");

if (response.status >= 200 && response.status < 300) {
    println("Success!");
} else if (response.status >= 400 && response.status < 500) {
    println("Client error:", response.status);
} else if (response.status >= 500) {
    println("Server error:", response.status);
}
```

### Response Helper Functions

```klang
fn isSuccess(response) {
    return response.status >= 200 && response.status < 300;
}

fn isRedirect(response) {
    return response.status >= 300 && response.status < 400;
}

fn isClientError(response) {
    return response.status >= 400 && response.status < 500;
}

fn isServerError(response) {
    return response.status >= 500 && response.status < 600;
}

// Usage
let response = http.get("https://api.example.com/data");
if (isSuccess(response)) {
    println("Request succeeded");
}
```

## Error Handling

### Try-Catch Error Handling

```klang
try {
    let response = http.get("https://api.example.com/data");
    let data = JSON.parse(response.body);
    println("Data received:", data);
} catch (error) {
    println("Error occurred:", error);
}
```

### Network Error Handling

```klang
fn fetchWithRetry(url, maxRetries = 3) {
    let retries = 0;
    
    while (retries < maxRetries) {
        try {
            let response = http.get(url);
            if (response.status >= 200 && response.status < 300) {
                return response;
            }
            retries += 1;
        } catch (error) {
            println("Attempt " + (retries + 1) + " failed:", error);
            retries += 1;
            
            if (retries < maxRetries) {
                // Wait before retry
                sleep(1000 * retries); // Exponential backoff
            }
        }
    }
    
    throw "Failed after " + maxRetries + " retries";
}

// Usage
try {
    let response = fetchWithRetry("https://api.example.com/data");
    println("Success:", response.body);
} catch (error) {
    println("Final error:", error);
}
```

### HTTP Error Response Handling

```klang
let response = http.post("https://api.example.com/users", {
    body: JSON.stringify({ name: "John" }),
    headers: { "Content-Type": "application/json" }
});

if (response.status == 400) {
    let error = JSON.parse(response.body);
    println("Validation error:", error.message);
    println("Fields:", error.fields);
} else if (response.status == 401) {
    println("Unauthorized - check your credentials");
} else if (response.status == 403) {
    println("Forbidden - insufficient permissions");
} else if (response.status == 404) {
    println("Resource not found");
} else if (response.status == 429) {
    let retryAfter = response.headers["Retry-After"];
    println("Rate limited - retry after " + retryAfter + " seconds");
} else if (response.status >= 500) {
    println("Server error - try again later");
}
```

### Timeout Handling

```klang
let options = {
    timeout: 5000,  // 5 seconds
    headers: { "Accept": "application/json" }
};

try {
    let response = http.get("https://api.example.com/slow-endpoint", options);
    println("Response:", response.body);
} catch (error) {
    if (error.includes("timeout")) {
        println("Request timed out");
    } else {
        println("Other error:", error);
    }
}
```

## REST API Integration

### Complete REST API Client

```klang
class APIClient {
    constructor(baseURL, apiKey) {
        this.baseURL = baseURL;
        this.apiKey = apiKey;
    }
    
    request(method, endpoint, options = {}) {
        let url = this.baseURL + endpoint;
        
        let headers = {
            "Authorization": "Bearer " + this.apiKey,
            "Content-Type": "application/json",
            "Accept": "application/json"
        };
        
        // Merge custom headers
        if (options.headers) {
            for (let key in options.headers) {
                headers[key] = options.headers[key];
            }
        }
        
        let requestOptions = {
            headers: headers
        };
        
        if (options.body) {
            requestOptions.body = JSON.stringify(options.body);
        }
        
        let response;
        if (method == "GET") {
            response = http.get(url, requestOptions);
        } else if (method == "POST") {
            response = http.post(url, requestOptions);
        } else if (method == "PUT") {
            response = http.put(url, requestOptions);
        } else if (method == "PATCH") {
            response = http.patch(url, requestOptions);
        } else if (method == "DELETE") {
            response = http.delete(url, requestOptions);
        }
        
        if (response.status >= 200 && response.status < 300) {
            return JSON.parse(response.body);
        } else {
            throw "HTTP " + response.status + ": " + response.body;
        }
    }
    
    get(endpoint, options = {}) {
        return this.request("GET", endpoint, options);
    }
    
    post(endpoint, body, options = {}) {
        options.body = body;
        return this.request("POST", endpoint, options);
    }
    
    put(endpoint, body, options = {}) {
        options.body = body;
        return this.request("PUT", endpoint, options);
    }
    
    patch(endpoint, body, options = {}) {
        options.body = body;
        return this.request("PATCH", endpoint, options);
    }
    
    delete(endpoint, options = {}) {
        return this.request("DELETE", endpoint, options);
    }
}

// Usage
let client = new APIClient("https://api.example.com", "your-api-key");

try {
    // GET request
    let users = client.get("/users");
    println("Users:", users);
    
    // POST request
    let newUser = client.post("/users", {
        name: "John Doe",
        email: "john@example.com"
    });
    println("Created user:", newUser);
    
    // PUT request
    let updated = client.put("/users/123", {
        name: "Jane Doe",
        email: "jane@example.com"
    });
    
    // DELETE request
    client.delete("/users/123");
    println("User deleted");
} catch (error) {
    println("API Error:", error);
}
```

### GitHub API Integration

```klang
class GitHubAPI {
    constructor(token) {
        this.baseURL = "https://api.github.com";
        this.token = token;
    }
    
    request(method, endpoint, body = null) {
        let url = this.baseURL + endpoint;
        let options = {
            headers: {
                "Authorization": "token " + this.token,
                "Accept": "application/vnd.github.v3+json",
                "User-Agent": "KLang-GitHub-Client"
            }
        };
        
        if (body) {
            options.body = JSON.stringify(body);
        }
        
        let response;
        if (method == "GET") {
            response = http.get(url, options);
        } else if (method == "POST") {
            response = http.post(url, options);
        }
        
        return JSON.parse(response.body);
    }
    
    getUser(username) {
        return this.request("GET", "/users/" + username);
    }
    
    getRepos(username) {
        return this.request("GET", "/users/" + username + "/repos");
    }
    
    createIssue(owner, repo, title, body) {
        return this.request("POST", "/repos/" + owner + "/" + repo + "/issues", {
            title: title,
            body: body
        });
    }
}

// Usage
let github = new GitHubAPI("your-github-token");

let user = github.getUser("octocat");
println("User:", user.name);
println("Bio:", user.bio);

let repos = github.getRepos("octocat");
for (let repo of repos) {
    println(repo.name + " - " + repo.description);
}
```

### Pagination Handling

```klang
fn fetchAllPages(baseUrl, apiKey) {
    let allData = [];
    let page = 1;
    let hasMore = true;
    
    while (hasMore) {
        let url = baseUrl + "?page=" + page + "&per_page=100";
        let response = http.get(url, {
            headers: { "Authorization": "Bearer " + apiKey }
        });
        
        let pageData = JSON.parse(response.body);
        allData = allData.concat(pageData);
        
        // Check if there are more pages
        let linkHeader = response.headers["Link"];
        hasMore = linkHeader && linkHeader.includes('rel="next"');
        page += 1;
    }
    
    return allData;
}

// Usage
let allUsers = fetchAllPages("https://api.example.com/users", "api-key");
println("Total users:", allUsers.length);
```

### Rate Limiting

```klang
class RateLimitedClient {
    constructor(baseURL, apiKey, maxRequests = 60, timeWindow = 60000) {
        this.baseURL = baseURL;
        this.apiKey = apiKey;
        this.maxRequests = maxRequests;
        this.timeWindow = timeWindow;
        this.requests = [];
    }
    
    canMakeRequest() {
        let now = Date.now();
        this.requests = this.requests.filter(time => now - time < this.timeWindow);
        return this.requests.length < this.maxRequests;
    }
    
    waitForSlot() {
        while (!this.canMakeRequest()) {
            sleep(1000);
        }
        this.requests.push(Date.now());
    }
    
    get(endpoint) {
        this.waitForSlot();
        
        let url = this.baseURL + endpoint;
        let response = http.get(url, {
            headers: { "Authorization": "Bearer " + this.apiKey }
        });
        
        return JSON.parse(response.body);
    }
}

// Usage
let client = new RateLimitedClient("https://api.example.com", "api-key", 60, 60000);

for (let i = 0; i < 100; i++) {
    let data = client.get("/data/" + i);
    println("Fetched:", data);
}
```

## Advanced Features

### Request Interceptors

```klang
class HTTPClient {
    constructor(baseURL) {
        this.baseURL = baseURL;
        this.requestInterceptors = [];
        this.responseInterceptors = [];
    }
    
    addRequestInterceptor(fn) {
        this.requestInterceptors.push(fn);
    }
    
    addResponseInterceptor(fn) {
        this.responseInterceptors.push(fn);
    }
    
    get(endpoint, options = {}) {
        // Apply request interceptors
        for (let interceptor of this.requestInterceptors) {
            options = interceptor(options);
        }
        
        let url = this.baseURL + endpoint;
        let response = http.get(url, options);
        
        // Apply response interceptors
        for (let interceptor of this.responseInterceptors) {
            response = interceptor(response);
        }
        
        return response;
    }
}

// Usage
let client = new HTTPClient("https://api.example.com");

// Add logging interceptor
client.addRequestInterceptor((options) => {
    println("[REQUEST]", new Date().toISOString());
    return options;
});

client.addResponseInterceptor((response) => {
    println("[RESPONSE]", response.status);
    return response;
});

let response = client.get("/data");
```

### Caching Responses

```klang
class CachedHTTPClient {
    constructor() {
        this.cache = {};
    }
    
    get(url, ttl = 60000) {
        let now = Date.now();
        
        // Check cache
        if (this.cache[url]) {
            let cached = this.cache[url];
            if (now - cached.timestamp < ttl) {
                println("Cache hit for:", url);
                return cached.response;
            }
        }
        
        // Make request
        let response = http.get(url);
        
        // Store in cache
        this.cache[url] = {
            response: response,
            timestamp: now
        };
        
        return response;
    }
    
    clearCache() {
        this.cache = {};
    }
}

// Usage
let client = new CachedHTTPClient();

// First request - fetches from server
let response1 = client.get("https://api.example.com/data", 30000);

// Second request within TTL - returns cached
let response2 = client.get("https://api.example.com/data", 30000);
```

### Concurrent Requests

```klang
fn fetchConcurrent(urls) {
    let results = [];
    
    // Note: In a real implementation, this would use async/await
    for (let url of urls) {
        try {
            let response = http.get(url);
            results.push({
                url: url,
                success: true,
                data: JSON.parse(response.body)
            });
        } catch (error) {
            results.push({
                url: url,
                success: false,
                error: error
            });
        }
    }
    
    return results;
}

// Usage
let urls = [
    "https://api.example.com/users/1",
    "https://api.example.com/users/2",
    "https://api.example.com/users/3"
];

let results = fetchConcurrent(urls);
for (let result of results) {
    if (result.success) {
        println("Success:", result.url, result.data);
    } else {
        println("Failed:", result.url, result.error);
    }
}
```

### WebSocket Support

```klang
// WebSocket client (conceptual)
let ws = new WebSocket("wss://api.example.com/stream");

ws.onOpen(() => {
    println("Connected");
    ws.send(JSON.stringify({ type: "subscribe", channel: "updates" }));
});

ws.onMessage((message) => {
    let data = JSON.parse(message);
    println("Received:", data);
});

ws.onClose(() => {
    println("Disconnected");
});

ws.onError((error) => {
    println("Error:", error);
});
```

## Best Practices

### 1. Always Handle Errors

```klang
fn safeRequest(url) {
    try {
        let response = http.get(url);
        if (response.status >= 200 && response.status < 300) {
            return JSON.parse(response.body);
        } else {
            throw "HTTP Error: " + response.status;
        }
    } catch (error) {
        println("Request failed:", error);
        return null;
    }
}
```

### 2. Use Environment Variables for Secrets

```klang
// Don't hardcode API keys
// Bad:
let apiKey = "sk-1234567890abcdef";

// Good:
let apiKey = env("API_KEY");
if (!apiKey) {
    throw "API_KEY environment variable not set";
}
```

### 3. Implement Timeouts

```klang
let options = {
    timeout: 10000,  // 10 seconds
    headers: { "Accept": "application/json" }
};

let response = http.get(url, options);
```

### 4. Log Requests for Debugging

```klang
fn loggedRequest(method, url, options) {
    println("[HTTP]", method, url);
    
    let startTime = Date.now();
    let response = http[method.toLowerCase()](url, options);
    let duration = Date.now() - startTime;
    
    println("[HTTP]", response.status, duration + "ms");
    return response;
}
```

### 5. Validate Response Data

```klang
fn fetchUser(userId) {
    let response = http.get("https://api.example.com/users/" + userId);
    let user = JSON.parse(response.body);
    
    // Validate required fields
    if (!user.id || !user.name || !user.email) {
        throw "Invalid user data received";
    }
    
    return user;
}
```

## Common Patterns

### RESTful CRUD Operations

```klang
class ResourceClient {
    constructor(baseURL, resourcePath, apiKey) {
        this.baseURL = baseURL;
        this.resourcePath = resourcePath;
        this.apiKey = apiKey;
    }
    
    getHeaders() {
        return {
            "Authorization": "Bearer " + this.apiKey,
            "Content-Type": "application/json"
        };
    }
    
    // Create
    create(data) {
        let response = http.post(
            this.baseURL + this.resourcePath,
            {
                body: JSON.stringify(data),
                headers: this.getHeaders()
            }
        );
        return JSON.parse(response.body);
    }
    
    // Read (one)
    get(id) {
        let response = http.get(
            this.baseURL + this.resourcePath + "/" + id,
            { headers: this.getHeaders() }
        );
        return JSON.parse(response.body);
    }
    
    // Read (all)
    list(params = {}) {
        let query = http.buildQuery(params);
        let url = this.baseURL + this.resourcePath + "?" + query;
        
        let response = http.get(url, { headers: this.getHeaders() });
        return JSON.parse(response.body);
    }
    
    // Update
    update(id, data) {
        let response = http.put(
            this.baseURL + this.resourcePath + "/" + id,
            {
                body: JSON.stringify(data),
                headers: this.getHeaders()
            }
        );
        return JSON.parse(response.body);
    }
    
    // Delete
    delete(id) {
        let response = http.delete(
            this.baseURL + this.resourcePath + "/" + id,
            { headers: this.getHeaders() }
        );
        return response.status == 204;
    }
}

// Usage
let users = new ResourceClient(
    "https://api.example.com",
    "/users",
    "your-api-key"
);

// Create
let newUser = users.create({ name: "John", email: "john@example.com" });

// Read
let user = users.get(newUser.id);

// List
let allUsers = users.list({ page: 1, limit: 10 });

// Update
users.update(newUser.id, { name: "John Doe" });

// Delete
users.delete(newUser.id);
```

## Next Steps

- Learn about [File I/O](41-file-io.md) for working with local files
- Explore [JSON](43-json.md) for advanced JSON operations
- Check out [Environment Variables](42-environment.md) for configuration
- See [Complete Examples](63-examples-projects.md) for full HTTP applications

---

*Documentation for KLang HTTP Client - Version 1.0*
