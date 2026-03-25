/**
 * Enterprise Microservice Example
 * 
 * Demonstrates:
 * - Structured logging
 * - Prometheus metrics
 * - Distributed tracing
 * - Circuit breaker
 * - Rate limiting
 * - Health checks
 * - Graceful shutdown
 */

import { createLogger, LOG_LEVEL_INFO } from "logging"
import { counter, gauge, histogram, exportMetrics } from "metrics"
import { initTracer, startTrace, startSpan, JaegerExporter } from "tracing"
import { createCircuitBreaker } from "circuit_breaker"
import { slidingWindow } from "rate_limiter"
import { HttpServer } from "net"

// ============================================================================
// Configuration
// ============================================================================

const CONFIG = {
    port: parseInt(getEnv("PORT", "8080")),
    serviceName: "order-service",
    logLevel: LOG_LEVEL_INFO,
    jaegerEndpoint: "http://localhost:14268/api/traces"
}

// ============================================================================
// Observability Setup
// ============================================================================

// Logger
const logger = createLogger(CONFIG.serviceName, CONFIG.logLevel)
logger.info("Starting service", { config: CONFIG })

// Metrics
const requestCounter = counter(
    "http_requests_total",
    "Total HTTP requests",
    ["method", "endpoint", "status"]
)

const requestDuration = histogram(
    "http_request_duration_seconds",
    "HTTP request duration",
    [0.001, 0.005, 0.01, 0.025, 0.05, 0.1, 0.25, 0.5, 1, 2.5, 5, 10],
    ["method", "endpoint"]
)

const activeRequests = gauge(
    "http_active_requests",
    "Number of active HTTP requests"
)

const orderCounter = counter(
    "orders_processed_total",
    "Total orders processed",
    ["status"]
)

// Tracing
const jaegerExporter = new JaegerExporter(CONFIG.jaegerEndpoint)
initTracer(CONFIG.serviceName, jaegerExporter)

// ============================================================================
// Resilience Patterns
// ============================================================================

// Circuit breaker for external payment service
const paymentBreaker = createCircuitBreaker({
    failureThreshold: 5,
    successThreshold: 2,
    timeout: 30000,
    resetTimeout: 60000,
    onOpen: () => {
        logger.error("Payment service circuit breaker OPEN")
    },
    onClose: () => {
        logger.info("Payment service circuit breaker CLOSED")
    }
})

// Rate limiter for public API
const publicApiLimiter = slidingWindow(
    1000,   // 1000 requests
    60000   // per minute
)

// ============================================================================
// Business Logic
// ============================================================================

/**
 * Process order with full observability
 */
async fn processOrder(orderId, userId) {
    // Start trace
    const trace = startTrace("process-order", {
        "order.id": orderId,
        "user.id": userId
    })
    
    try {
        logger.info("Processing order", { orderId, userId })
        
        // Validate order
        const validateSpan = startSpan("validate-order")
        await validateOrder(orderId)
        validateSpan.end()
        
        // Check inventory
        const inventorySpan = startSpan("check-inventory")
        const inStock = await checkInventory(orderId)
        inventorySpan.end()
        
        if !inStock {
            orderCounter.inc({ status: "out_of_stock" })
            throw new Error("Product out of stock")
        }
        
        // Process payment (with circuit breaker)
        const paymentSpan = startSpan("process-payment")
        try {
            const paymentResult = await paymentBreaker.execute(async () => {
                return await processPayment(orderId, userId)
            })
            paymentSpan.setAttribute("payment.id", paymentResult.id)
            paymentSpan.setStatus("OK")
        } catch (err) {
            paymentSpan.recordException(err)
            throw err
        } finally {
            paymentSpan.end()
        }
        
        // Fulfill order
        const fulfillSpan = startSpan("fulfill-order")
        await fulfillOrder(orderId)
        fulfillSpan.end()
        
        // Success
        orderCounter.inc({ status: "success" })
        trace.setStatus("OK")
        
        logger.info("Order processed successfully", {
            orderId,
            userId,
            duration: trace.duration()
        })
        
        return { success: true, orderId }
        
    } catch (err) {
        orderCounter.inc({ status: "error" })
        trace.recordException(err)
        
        logger.error("Order processing failed", {
            orderId,
            userId,
            error: err.message,
            stack: err.stack
        })
        
        throw err
    } finally {
        trace.end()
    }
}

/**
 * Validate order (simulated)
 */
async fn validateOrder(orderId) {
    await sleep(10)
    logger.debug("Order validated", { orderId })
    return true
}

/**
 * Check inventory (simulated)
 */
async fn checkInventory(orderId) {
    await sleep(20)
    logger.debug("Inventory checked", { orderId })
    return true
}

/**
 * Process payment (simulated, can fail)
 */
async fn processPayment(orderId, userId) {
    await sleep(100)
    
    // Simulate 10% failure rate
    if Math.random() < 0.1 {
        throw new Error("Payment gateway timeout")
    }
    
    logger.debug("Payment processed", { orderId, userId })
    return { id: "payment-" + orderId }
}

/**
 * Fulfill order (simulated)
 */
async fn fulfillOrder(orderId) {
    await sleep(50)
    logger.debug("Order fulfilled", { orderId })
    return true
}

// ============================================================================
// HTTP Server
// ============================================================================

const server = new HttpServer()

/**
 * Middleware: Logging and metrics
 */
fn requestMiddleware(req, res, next) {
    const startTime = Date.now()
    activeRequests.inc()
    
    const requestId = randomHex(8)
    req.requestId = requestId
    
    logger
        .withContext("requestId", requestId)
        .info("Request received", {
            method: req.method,
            path: req.path,
            ip: req.ip
        })
    
    // Continue to handler
    next()
    
    // After handler
    const duration = (Date.now() - startTime) / 1000
    activeRequests.dec()
    
    requestCounter.inc({
        method: req.method,
        endpoint: req.path,
        status: String(res.statusCode)
    })
    
    requestDuration.observe(duration, {
        method: req.method,
        endpoint: req.path
    })
    
    logger.info("Request completed", {
        requestId,
        method: req.method,
        path: req.path,
        status: res.statusCode,
        duration
    })
}

server.use(requestMiddleware)

/**
 * POST /api/orders - Create and process order
 */
server.post("/api/orders", async (req, res) => {
    // Rate limiting
    if !publicApiLimiter.tryAllow() {
        logger.warn("Rate limit exceeded", { ip: req.ip })
        return res.status(429).json({
            error: "Too many requests",
            retryAfter: 60
        })
    }
    
    try {
        const { orderId, userId } = req.body
        
        if !orderId || !userId {
            return res.status(400).json({
                error: "Missing orderId or userId"
            })
        }
        
        const result = await processOrder(orderId, userId)
        
        return res.status(200).json(result)
        
    } catch (err) {
        logger.error("Order endpoint error", {
            error: err.message,
            requestId: req.requestId
        })
        
        return res.status(500).json({
            error: "Internal server error",
            requestId: req.requestId
        })
    }
})

/**
 * GET /health - Health check
 */
server.get("/health", (req, res) => {
    const health = {
        status: "healthy",
        service: CONFIG.serviceName,
        timestamp: Date.now(),
        checks: {
            database: "healthy",
            payment_service: paymentBreaker.getState() == "CLOSED" ? "healthy" : "degraded"
        }
    }
    
    return res.status(200).json(health)
})

/**
 * GET /metrics - Prometheus metrics
 */
server.get("/metrics", (req, res) => {
    const metrics = exportMetrics()
    res.setHeader("Content-Type", "text/plain")
    return res.status(200).send(metrics)
})

/**
 * GET /ready - Readiness probe
 */
server.get("/ready", (req, res) => {
    // Check if service is ready to accept traffic
    const ready = paymentBreaker.getState() != "OPEN"
    
    if ready {
        return res.status(200).json({ ready: true })
    } else {
        return res.status(503).json({ ready: false, reason: "Payment service unavailable" })
    }
})

// ============================================================================
// Startup and Shutdown
// ============================================================================

/**
 * Graceful shutdown
 */
fn shutdown() {
    logger.info("Shutdown signal received")
    
    // Stop accepting new requests
    server.stop()
    
    logger.info("Server stopped")
    
    // Close connections
    // db.close()
    // redis.close()
    
    logger.info("Shutdown complete")
    process.exit(0)
}

process.on("SIGTERM", shutdown)
process.on("SIGINT", shutdown)

/**
 * Start server
 */
server.listen(CONFIG.port, () => {
    logger.info("Server started", {
        port: CONFIG.port,
        service: CONFIG.serviceName
    })
})

// ============================================================================
// Periodic Tasks
// ============================================================================

// Report metrics every 30 seconds
setInterval(() => {
    const stats = {
        circuitBreaker: paymentBreaker.getStats(),
        rateLimiter: publicApiLimiter.getStats()
    }
    
    logger.info("Periodic stats", stats)
}, 30000)
