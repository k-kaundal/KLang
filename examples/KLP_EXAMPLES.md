# KLP (KLang Protocol) Examples

This directory contains examples demonstrating the KLP protocol features in KLang.

## Quick Start

### 1. Basic Echo Server

The simplest KLP server that echoes back messages:

```bash
./klang examples/klp_echo_server.klang
```

In another terminal, run the client:

```bash
./klang examples/klp_client_example.klang
```

### 2. Advanced Features

For more advanced features (streaming, concurrency, error handling):

```bash
./klang examples/klp_advanced_server.klang
```

## Examples Overview

### `klp_echo_server.klang`
**Basic server with multiple routes:**
- `/echo` - Echo back messages
- `/greet` - Greet users by name
- `/compute` - Arithmetic operations (add, subtract, multiply, divide)

**Demonstrates:**
- Server creation and routing
- Reading and writing data
- Basic request/response pattern
- Simple error handling

### `klp_client_example.klang`
**Client connecting to echo server:**
- Connects to multiple routes
- Sends requests and receives responses
- Demonstrates proper connection lifecycle

**Demonstrates:**
- Client connection
- Sending/receiving data
- Connection cleanup

### `klp_advanced_server.klang`
**Advanced server features:**
- `/stream` - Streaming data in chunks
- `/json` - Structured data handling
- `/concurrent` - Thread-safe concurrent requests
- `/divide` - Error handling patterns
- `/tensor` - Binary data for AI/ML workloads

**Demonstrates:**
- Chunked streaming
- Concurrent connection handling
- Error responses
- Binary data transfer
- Performance considerations

## API Reference

### Server API

```klang
// Create a server on port
let server = klp.server(port)

// Register a route handler
server.route(path, fn(stream) {
    // Handle requests
    let data = stream.read()
    stream.write(response)
})

// Start server (blocking)
server.start()
```

### Client API

```klang
// Connect to server
let client = klp.connect(host, port, path)

// Send data
client.write(data)

// Receive data
let response = client.read()

// Close connection
client.close()
```

### Stream Operations

```klang
// Read text data
let text = stream.read()

// Write text data
stream.write("Hello, World!")

// For binary/tensor data (AI/ML)
// stream.read_tensor()   // Read binary tensor
// stream.write_tensor()  // Write binary tensor
```

## Protocol Features

The KLP protocol provides several advantages over HTTP:

### 🚀 **Performance**
- **Binary framing** - More efficient than text-based protocols
- **Multiplexing** - Multiple requests over single connection
- **Zero-copy** - Direct memory access (with RDMA)
- **Compression** - Built-in gzip/zstd support

### 🔐 **Security**
- **Quantum-resistant crypto** - Post-quantum algorithms (optional)
- **TLS support** - Standard encryption
- **Hybrid mode** - Classical + quantum crypto

### 🧠 **AI/ML Optimized**
- **Tensor streaming** - Efficient binary tensor transfer
- **Batching** - Multiple tensors in one request
- **GPU direct** - GPU-to-GPU transfer (with CUDA)
- **RDMA support** - Ultra-low latency for HPC

### 📊 **Observability**
- **Stream IDs** - Track multiplexed requests
- **Flow control** - Prevent buffer overflow
- **Backpressure** - Handle slow consumers
- **Metrics** - Built-in performance tracking

## Testing Your Server

### Manual Testing

```bash
# Terminal 1: Start server
./klang examples/klp_echo_server.klang

# Terminal 2: Run client
./klang examples/klp_client_example.klang
```

### Load Testing

For production deployments, test with multiple concurrent clients:

```klang
// concurrent_test.klang
let num_clients = 100
let i = 0

while i < num_clients {
    // Each client in its own "thread"
    let client = klp.connect("localhost", 9000, "/echo")
    client.write("Test message " + i)
    let response = client.read()
    print("Client " + i + ": " + response)
    client.close()
    
    i = i + 1
}
```

## Performance Tuning

### Server-Side

```klang
// Configure server parameters
server.set_max_connections(1000)
server.set_buffer_size(64 * 1024)  // 64KB
server.set_compression("zstd")     // gzip, zstd, or none
server.set_threads(4)              // Worker threads
```

### Client-Side

```klang
// Configure client
client.set_timeout(5000)           // 5 seconds
client.set_compression("zstd")
client.set_buffer_size(64 * 1024)
```

## Deployment

### Development

```bash
# Start server in development mode
./klang examples/klp_echo_server.klang
```

### Production

```bash
# Build optimized binary
make ENABLE_KLP=1 ENABLE_LIBOQS=1

# Run with production settings
./klang -O3 examples/klp_echo_server.klang

# Or compile to native code
./klang --compile examples/klp_echo_server.klang -o echo_server
./echo_server
```

### Docker

```dockerfile
FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    gcc make zlib1g-dev

# Copy KLang
COPY . /klang
WORKDIR /klang

# Build
RUN make ENABLE_KLP=1

# Run server
CMD ["./klang", "examples/klp_echo_server.klang"]
```

## Troubleshooting

### Connection Refused
- Ensure server is running: `ps aux | grep klang`
- Check port is not in use: `netstat -tulpn | grep 9000`
- Verify firewall allows connection

### Slow Performance
- Enable compression: `server.set_compression("zstd")`
- Increase buffer sizes
- Use binary mode for large data
- Consider RDMA for ultra-low latency

### Memory Issues
- Check for connection leaks (always call `client.close()`)
- Monitor with: `top` or `htop`
- Reduce `max_connections` if needed

## Next Steps

1. **Read the docs**: `docs/KLP_ADVANCED.md` for full protocol details
2. **Try benchmarks**: `benchmarks/klp_benchmark.c` for performance testing
3. **Explore features**: Quantum crypto, RDMA, GPU acceleration
4. **Build your app**: Use KLP for your AI/ML or HPC application

## Support

- **Issues**: https://github.com/k-kaundal/KLang/issues
- **Docs**: `docs/` directory
- **Examples**: This directory

## License

Same as KLang main project.
