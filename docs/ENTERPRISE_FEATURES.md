# Enterprise Features Guide

## Overview

KLang provides comprehensive enterprise-level features for production deployments:

1. **Observability**: Logging, metrics, and distributed tracing
2. **Resilience**: Circuit breakers, rate limiting, retry policies
3. **Performance**: Benchmarking framework and profiling tools
4. **Security**: Audit logging, secrets management
5. **Cloud-Native**: Container, Kubernetes, and serverless support

---

## 📊 Observability

### Logging (`stdlib/logging.kl`)

Structured JSON logging with multiple log levels and output targets.

```klang
import { createLogger, LOG_LEVEL_INFO } from "logging"

let logger = createLogger("my-service", LOG_LEVEL_INFO)

// Simple logging
logger.info("User logged in", { userId: "123", email: "user@example.com" })
logger.error("Database connection failed", { error: err.message })

// With context
logger
    .withContext("requestId", "abc-123")
    .withContext("userId", "user-456")
    .info("Processing request")

// Performance logging
let perf = perfLog(logger, "database-query")
perf.checkpoint("connection-opened")
// ... do work ...
perf.checkpoint("query-executed")
perf.done(true)
```

**Features:**
- Log levels: DEBUG, INFO, WARN, ERROR, FATAL
- JSON or text output formats
- Multiple output targets (console, file, network)
- Context propagation
- Performance timing helpers
- Function tracing decorator

**Configuration:**
```klang
import { setGlobalLevel, setOutputs, setFormat, LOG_LEVEL_DEBUG } from "logging"

setGlobalLevel(LOG_LEVEL_DEBUG)
setOutputs(["console", "file:/var/log/app.log"])
setFormat("json")
```

---

### Metrics (`stdlib/metrics.kl`)

Prometheus-compatible metrics for monitoring application performance.

```klang
import { counter, gauge, histogram } from "metrics"

// Counter: monotonically increasing values
let requestCount = counter(
    "http_requests_total",
    "Total HTTP requests",
    ["method", "status"]
)

requestCount.inc({ method: "GET", status: "200" })
requestCount.add(5, { method: "POST", status: "201" })

// Gauge: values that go up and down
let activeUsers = gauge(
    "active_users",
    "Number of active users"
)

activeUsers.set(42)
activeUsers.inc()  // 43
activeUsers.dec()  // 42

// Histogram: track distribution
let responseTime = histogram(
    "http_response_time_seconds",
    "HTTP response time",
    [0.005, 0.01, 0.025, 0.05, 0.1, 0.25, 0.5, 1, 2.5, 5, 10]
)

responseTime.observe(0.123)

// Time a function
responseTime.time(() => {
    // ... do work ...
})

// Export all metrics (Prometheus format)
import { exportMetrics } from "metrics"
let prometheusOutput = exportMetrics()
println(prometheusOutput)
```

**Metric Types:**
- **Counter**: Monotonically increasing (requests, errors, events)
- **Gauge**: Can increase/decrease (memory usage, active connections)
- **Histogram**: Distribution with buckets (latency, sizes)
- **Summary**: Distribution with quantiles (p50, p95, p99)

**Default Labels:**
```klang
import { setDefaultLabels } from "metrics"

setDefaultLabels({
    service: "my-service",
    environment: "production",
    version: "1.0.0"
})
```

---

### Distributed Tracing (`stdlib/tracing.kl`)

OpenTelemetry-compatible distributed tracing for microservices.

```klang
import { initTracer, startTrace, startSpan, JaegerExporter } from "tracing"

// Initialize tracer with Jaeger export
let exporter = new JaegerExporter("http://localhost:14268/api/traces")
initTracer("my-service", exporter)

// Start a trace (root span)
let trace = startTrace("handle-request", {
    "http.method": "GET",
    "http.url": "/api/users"
})

// Start child span
let dbSpan = startSpan("database-query", {
    "db.system": "postgresql",
    "db.operation": "SELECT"
})

try {
    // ... do work ...
    dbSpan.setStatus("OK")
} catch (err) {
    dbSpan.recordException(err)
} finally {
    dbSpan.end()
}

trace.end()

// Automatic tracing with decorator
import { traced } from "tracing"

@traced
fn processOrder(orderId) {
    // Automatically traced
    return fetchOrder(orderId)
}
```

**Features:**
- W3C Trace Context propagation
- Automatic parent-child relationships
- Span attributes and events
- Exception recording
- Jaeger/Tempo/Zipkin export
- Function decorators

**Trace Context Propagation:**
```klang
// Extract from HTTP header
let context = SpanContext.fromTraceparent(req.headers["traceparent"])

// Create child span with parent context
let span = tracer.startSpan("downstream-call", context)

// Inject into HTTP request
req.headers["traceparent"] = span.context.toTraceparent()
```

---

## 🛡️ Resilience Patterns

### Circuit Breaker (`stdlib/circuit_breaker.kl`)

Prevents cascading failures by stopping requests to failing services.

```klang
import { createCircuitBreaker, withCircuitBreaker } from "circuit_breaker"

// Create circuit breaker
let breaker = createCircuitBreaker({
    failureThreshold: 5,      // Open after 5 failures
    successThreshold: 2,      // Close after 2 successes
    timeout: 30000,           // 30 second timeout per request
    resetTimeout: 60000,      // Try again after 60 seconds
    onOpen: () => {
        logger.warn("Circuit breaker opened!")
    },
    onClose: () => {
        logger.info("Circuit breaker closed")
    }
})

// Use circuit breaker
try {
    let result = await breaker.execute(async () => {
        return await callExternalAPI()
    })
} catch (err) {
    if err.message == "Circuit breaker is OPEN" {
        // Handle circuit open
        return cachedFallbackValue()
    }
    throw err
}

// Or wrap function
let protectedCall = withCircuitBreaker(callExternalAPI, {
    failureThreshold: 5,
    timeout: 30000
})

let result = await protectedCall()

// Get statistics
let stats = breaker.getStats()
println(`State: ${stats.state}`)
println(`Total calls: ${stats.totalCalls}`)
println(`Failure rate: ${stats.failureRate}`)
```

**States:**
- **CLOSED**: Normal operation, requests pass through
- **OPEN**: Service failing, requests rejected immediately
- **HALF_OPEN**: Testing if service recovered

---

### Rate Limiting (`stdlib/rate_limiter.kl`)

Protect APIs and resources from overload.

```klang
import { tokenBucket, fixedWindow, slidingWindow } from "rate_limiter"

// Token bucket: allows bursts
let limiter = tokenBucket(
    100,    // capacity: 100 tokens
    10      // refill: 10 tokens/second
)

if limiter.tryAllow() {
    // Process request
} else {
    // Rate limit exceeded
    throw new Error("Rate limit exceeded")
}

// Fixed window: simple but has boundary issues
let windowLimiter = fixedWindow(
    100,     // max 100 requests
    60000    // per 60 seconds
)

// Sliding window: more accurate
let slidingLimiter = slidingWindow(
    1000,    // max 1000 requests
    60000    // per 60 seconds
)

// Use with async function
try {
    let result = await limiter.execute(async () => {
        return await processRequest()
    })
} catch (err) {
    // Rate limited
    return { error: "Too many requests", retryAfter: 60 }
}

// Rate limiter decorator
import { rateLimit } from "rate_limiter"

let limiter = tokenBucket(100, 10)

@rateLimit(limiter)
async fn apiEndpoint(req) {
    return await processRequest(req)
}
```

**Algorithms:**
- **Token Bucket**: Smooth average rate with bursts
- **Leaky Bucket**: Constant rate output
- **Fixed Window**: Simple counter per time window
- **Sliding Window**: More accurate than fixed window

---

## 📈 Performance Benchmarking

### Benchmark Framework

Comprehensive performance benchmarking tools in `benchmarks/` directory.

**Directory Structure:**
```
benchmarks/
├── language/           # Language feature benchmarks
├── cross-language/     # Comparison with Python, JS, C
├── memory/            # Memory and GC benchmarks
├── framework/         # Benchmark utilities (benchmark.h)
└── reports/           # Generated reports
```

**Running Benchmarks:**
```bash
# Build and run all benchmarks
make bench

# Quick benchmark
make bench-quick

# Build benchmarks only
make benchmarks

# Clean benchmark artifacts
make clean-bench
```

**Benchmark Results:**
- JSON reports in `benchmarks/reports/`
- Markdown summary with tables
- Historical tracking for regression detection

### Creating Custom Benchmarks (C)

```c
#include "framework/benchmark.h"

void my_benchmark_function() {
    // Code to benchmark
    int sum = 0;
    for (int i = 0; i < 1000; i++) {
        sum += i;
    }
}

int main() {
    BenchmarkResult result = benchmark_run(
        "My Benchmark",
        100000,  // iterations
        my_benchmark_function
    );
    
    benchmark_print_result(&result);
    benchmark_save_json(&result, "reports/my_benchmark.json");
    
    return 0;
}
```

**Metrics Provided:**
- Throughput (operations/sec)
- Latency (min/max/mean/median/p95/p99)
- Memory usage (peak, allocations)
- Standard deviation

---

## 🚀 Production Deployment

### Health Checks

Built-in health check support for Kubernetes liveness/readiness probes.

```klang
import { health_check, health_report } from "cloud_native"

// Add component checks
health_check("database", () => {
    return db.ping() ? "HEALTHY" : "UNHEALTHY"
})

health_check("cache", () => {
    return redis.ping() ? "HEALTHY" : "UNHEALTHY"
})

// Generate health report
let report = health_report()
println(JSON.stringify(report))
// {
//   "status": "HEALTHY",
//   "components": {
//     "database": "HEALTHY",
//     "cache": "HEALTHY"
//   }
// }
```

### Graceful Shutdown

Handle shutdown signals properly to finish in-flight requests.

```klang
import { onShutdown } from "runtime"

onShutdown(() => {
    logger.info("Shutdown signal received")
    
    // Stop accepting new requests
    server.stopAccepting()
    
    // Wait for in-flight requests (max 30s)
    server.waitForRequests(30000)
    
    // Close connections
    db.close()
    redis.close()
    
    logger.info("Shutdown complete")
})
```

### Configuration Management

Environment-based configuration for different deployment stages.

```klang
import { getEnv, requireEnv } from "env"

let config = {
    port: parseInt(getEnv("PORT", "8080")),
    dbUrl: requireEnv("DATABASE_URL"),
    logLevel: getEnv("LOG_LEVEL", "info"),
    enableMetrics: getEnv("ENABLE_METRICS", "true") == "true"
}
```

---

## 🔐 Security

### Audit Logging

Track security-relevant events for compliance.

```klang
import { auditLog } from "security"

auditLog("user.login", {
    userId: user.id,
    username: user.name,
    ip: req.ip,
    timestamp: Date.now(),
    success: true
})

auditLog("admin.config.change", {
    adminId: admin.id,
    action: "update_rate_limit",
    oldValue: 100,
    newValue: 200,
    timestamp: Date.now()
})
```

### Secrets Management

Never hardcode secrets in source code.

```klang
import { getSecret } from "secrets"

// Load from environment or secret manager
let apiKey = getSecret("API_KEY")
let dbPassword = getSecret("DB_PASSWORD")

// Use secrets manager (Vault, AWS Secrets Manager)
let secrets = new SecretsManager("vault://localhost:8200")
let dbCreds = await secrets.get("database/credentials")
```

---

## 📚 Best Practices

### 1. Observability

```klang
// Always use structured logging
logger.info("Order processed", {
    orderId: order.id,
    userId: user.id,
    amount: order.total,
    processingTime: elapsed
})

// Instrument critical paths with metrics
let orderProcessingTime = histogram("order_processing_seconds", "Time to process order")
orderProcessingTime.time(() => {
    processOrder(order)
})

// Trace distributed operations
@traced
async fn placeOrder(order) {
    let span = startSpan("validate-order")
    await validateOrder(order)
    span.end()
    
    let paymentSpan = startSpan("process-payment")
    await processPayment(order)
    paymentSpan.end()
}
```

### 2. Resilience

```klang
// Wrap external calls with circuit breaker
let externalAPI = withCircuitBreaker(callExternalAPI, {
    failureThreshold: 5,
    timeout: 30000
})

// Add rate limiting to public endpoints
let rateLimiter = slidingWindow(1000, 60000)

@rateLimit(rateLimiter)
async fn publicEndpoint(req) {
    return await processRequest(req)
}

// Implement retry with exponential backoff
import { retry } from "retry"

let result = await retry(
    () => callUnreliableAPI(),
    {
        maxAttempts: 3,
        initialDelay: 1000,
        maxDelay: 10000,
        backoffMultiplier: 2
    }
)
```

### 3. Performance

```klang
// Run benchmarks in CI/CD
// Fail build if performance regresses > 10%

// Profile hot paths
let profiler = new Profiler()
profiler.start()
// ... code to profile ...
let report = profiler.stop()
profiler.saveFlameGraph("profile.svg")

// Monitor memory usage
let memGauge = gauge("memory_usage_bytes", "Memory usage")
setInterval(() => {
    memGauge.set(getMemoryUsage())
}, 10000)
```

---

## 🎯 Summary

KLang provides enterprise-grade features out of the box:

✅ **Logging**: Structured, multi-output, context-aware  
✅ **Metrics**: Prometheus-compatible, 4 metric types  
✅ **Tracing**: OpenTelemetry distributed tracing  
✅ **Circuit Breaker**: Prevent cascading failures  
✅ **Rate Limiting**: 4 algorithms (token bucket, sliding window, etc.)  
✅ **Benchmarking**: Comprehensive performance testing  
✅ **Cloud-Native**: Kubernetes, containers, serverless  
✅ **Security**: Audit logging, secrets management  

For more information, see:
- [API Reference](API_REFERENCE.md)
- [Performance Guide](performance.md)
- [Deployment Guide](../DEPLOYMENT.md)
