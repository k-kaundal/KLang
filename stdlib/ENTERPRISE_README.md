# KLang Standard Library - Enterprise Modules

Enterprise-grade modules for production-ready applications.

## 📋 Table of Contents

1. [Observability](#observability)
   - [Logging](#logging)
   - [Metrics](#metrics)
   - [Tracing](#tracing)
2. [Resilience](#resilience)
   - [Circuit Breaker](#circuit-breaker)
   - [Rate Limiter](#rate-limiter)
   - [Retry Policies](#retry-policies)
3. [Security](#security)
   - [Audit Logging](#audit-logging)
4. [AI & ML](#ai--ml)
5. [Networking](#networking)
6. [Data](#data)

---

## 🔍 Observability

### Logging

**Module:** `stdlib/logging.kl`

Structured logging with multiple outputs and log levels.

```klang
import { createLogger, LOG_LEVEL_INFO } from "logging"

let logger = createLogger("my-app", LOG_LEVEL_INFO)

logger.info("Application started", { port: 8080 })
logger.error("Database connection failed", { error: err.message })

// With context
logger
    .withContext("userId", "123")
    .withContext("requestId", "abc-456")
    .info("Processing request")
```

**Features:**
- 5 log levels: DEBUG, INFO, WARN, ERROR, FATAL
- JSON or text output
- Multiple targets: console, file, network
- Context propagation
- Performance timing

### Metrics

**Module:** `stdlib/metrics.kl`

Prometheus-compatible metrics collection.

```klang
import { counter, gauge, histogram, exportMetrics } from "metrics"

// Counter: monotonically increasing
let requests = counter("http_requests_total", "HTTP requests", ["method", "status"])
requests.inc({ method: "GET", status: "200" })

// Gauge: can go up or down
let activeUsers = gauge("active_users", "Active users")
activeUsers.set(42)

// Histogram: distribution tracking
let latency = histogram("request_latency_seconds", "Request latency")
latency.observe(0.123)

// Export for Prometheus
println(exportMetrics())
```

**Metric Types:**
- Counter: Counts (requests, errors)
- Gauge: Current values (memory, connections)
- Histogram: Distributions (latency, sizes)
- Summary: Quantiles (p50, p95, p99)

### Tracing

**Module:** `stdlib/tracing.kl`

OpenTelemetry-compatible distributed tracing.

```klang
import { initTracer, startTrace, startSpan, JaegerExporter } from "tracing"

// Initialize with Jaeger
let exporter = new JaegerExporter("http://localhost:14268/api/traces")
initTracer("my-service", exporter)

// Start trace
let trace = startTrace("handle-request")

// Child span
let dbSpan = startSpan("query-database")
// ... do work ...
dbSpan.end()

trace.end()
```

**Features:**
- W3C Trace Context propagation
- Span attributes and events
- Exception recording
- Jaeger/Tempo/Zipkin export
- Automatic decorators

---

## 🛡️ Resilience

### Circuit Breaker

**Module:** `stdlib/circuit_breaker.kl`

Prevent cascading failures.

```klang
import { createCircuitBreaker } from "circuit_breaker"

let breaker = createCircuitBreaker({
    failureThreshold: 5,
    successThreshold: 2,
    timeout: 30000,
    resetTimeout: 60000
})

let result = await breaker.execute(async () => {
    return await callExternalAPI()
})
```

**States:**
- CLOSED: Normal operation
- OPEN: Failing, reject immediately
- HALF_OPEN: Testing recovery

### Rate Limiter

**Module:** `stdlib/rate_limiter.kl`

API protection and rate limiting.

```klang
import { tokenBucket, slidingWindow } from "rate_limiter"

// Token bucket (allows bursts)
let limiter = tokenBucket(100, 10)  // 100 capacity, 10/sec refill

if limiter.tryAllow() {
    // Process request
} else {
    // Rate limited
}

// Sliding window (more accurate)
let apiLimiter = slidingWindow(1000, 60000)  // 1000 req/min
```

**Algorithms:**
- Token Bucket
- Fixed Window
- Sliding Window
- Leaky Bucket

### Retry Policies

**Module:** `stdlib/retry.kl`

Automatic retries with exponential backoff.

```klang
import { retry, RetryStrategies } from "retry"

let result = await retry(
    () => callUnreliableAPI(),
    {
        maxAttempts: 5,
        initialDelay: 1000,
        maxDelay: 30000,
        backoffMultiplier: 2
    }
)

// Or use predefined strategy
import { createRetryPolicy, RetryStrategies } from "retry"
let policy = createRetryPolicy(RetryStrategies.STANDARD)
```

**Strategies:**
- AGGRESSIVE: Fast retries, few attempts
- STANDARD: Balanced approach
- CONSERVATIVE: More attempts, slower
- LINEAR: Fixed delay

---

## 🔐 Security

### Audit Logging

**Module:** `stdlib/audit.kl`

Security and compliance audit trails.

```klang
import { auditAuth, auditAuthz, auditDataAccess, AuditEventType } from "audit"

// Authentication
auditAuth(AuditEventType.AUTH_LOGIN, userId, "success", {
    ip: req.ip,
    userAgent: req.headers["user-agent"]
})

// Authorization
auditAuthz(userId, "/admin/config", "read", "denied")

// Data access
auditDataAccess(userId, "user_table", "read", "success", {
    rowCount: 100
})

// Configuration changes
auditConfigChange(adminId, "rate_limit", 100, 200)
```

**Event Types:**
- Authentication events
- Authorization decisions
- Data access (read/write/delete)
- Configuration changes
- Admin actions
- Security alerts

---

## 🤖 AI & ML

Comprehensive AI modules:

- **llm.kl**: LLM integration (OpenAI, Anthropic, Ollama)
- **agents.kl**: Autonomous AI agents
- **rag.kl**: Retrieval-augmented generation
- **embedding.kl**: Vector embeddings
- **prompt.kl**: Prompt templates

See [AI_README.md](AI_README.md) for details.

---

## 🌐 Networking

- **net.kl**: HTTP client, WebSocket, DNS
- **json.kl**: JSON parsing and serialization

---

## 📦 Data

- **collections.kl**: Maps, sets, queues, deques
- **array.kl**: Array utilities
- **string.kl**: String manipulation
- **datetime.kl**: Date and time operations
- **fs.kl**: File system operations

---

## 📖 Usage Examples

### Complete Observability Stack

```klang
import { createLogger } from "logging"
import { counter, histogram } from "metrics"
import { initTracer, startTrace, JaegerExporter } from "tracing"

// Setup
let logger = createLogger("api-server")
let requestCount = counter("requests_total", "Total requests")
let requestDuration = histogram("request_duration_seconds", "Request duration")
initTracer("api-server", new JaegerExporter("http://localhost:14268/api/traces"))

// Handle request
async fn handleRequest(req) {
    let trace = startTrace("handle-request")
    let startTime = Date.now()
    
    try {
        logger.info("Request received", { path: req.path })
        
        let result = await processRequest(req)
        
        requestCount.inc({ status: "success" })
        trace.setStatus("OK")
        
        return result
    } catch (err) {
        logger.error("Request failed", { error: err.message })
        requestCount.inc({ status: "error" })
        trace.recordException(err)
        throw err
    } finally {
        let duration = (Date.now() - startTime) / 1000
        requestDuration.observe(duration)
        trace.end()
    }
}
```

### Resilient API Client

```klang
import { createCircuitBreaker } from "circuit_breaker"
import { retry } from "retry"
import { slidingWindow } from "rate_limiter"

// Setup
let breaker = createCircuitBreaker({ failureThreshold: 5 })
let limiter = slidingWindow(1000, 60000)

// Resilient call
async fn callAPI(endpoint) {
    // Rate limit
    if !limiter.tryAllow() {
        throw new Error("Rate limit exceeded")
    }
    
    // Circuit breaker + retry
    return await retry(
        async () => {
            return await breaker.execute(async () => {
                return await http.get(endpoint)
            })
        },
        {
            maxAttempts: 3,
            initialDelay: 1000
        }
    )
}
```

---

## 🎯 Best Practices

1. **Always use structured logging**
   ```klang
   logger.info("Order processed", { orderId, amount, duration })
   ```

2. **Instrument critical paths**
   ```klang
   histogram.time(() => criticalOperation())
   ```

3. **Wrap external calls**
   ```klang
   let protected = withCircuitBreaker(externalCall, { timeout: 30000 })
   ```

4. **Rate limit public endpoints**
   ```klang
   @rateLimit(slidingWindow(1000, 60000))
   async fn publicAPI(req) { ... }
   ```

5. **Audit security events**
   ```klang
   auditAuth(AuditEventType.AUTH_LOGIN, userId, "success", { ip: req.ip })
   ```

---

## 📚 Documentation

For comprehensive guides, see:

- [Enterprise Features Guide](../docs/ENTERPRISE_FEATURES.md)
- [API Reference](../docs/API_REFERENCE.md)
- [Performance Guide](../docs/performance.md)

---

## 🤝 Contributing

Enterprise modules follow strict standards:

1. **Performance**: Minimal overhead
2. **Reliability**: Comprehensive error handling
3. **Observability**: Built-in metrics and logging
4. **Documentation**: Examples and API docs
5. **Testing**: Unit and integration tests

See [CONTRIBUTING.md](../CONTRIBUTING.md) for details.
