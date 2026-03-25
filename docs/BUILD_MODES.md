# Build Modes and Optimization Guide

## Overview

KLang supports multiple build modes and optimization levels to optimize for different use cases - from fast development iteration to maximum production performance.

## Build Modes

KLang provides four build modes, each optimized for different scenarios:

### 1. Debug Mode (`--mode=debug`)

**Purpose:** Maximum debugging information, no optimization
**Use case:** Finding bugs, debugging complex issues

**Settings:**
- Optimization: O0 (none)
- Debug symbols: Enabled
- Assertions: Enabled
- Bounds checking: Enabled
- Hot reload: Disabled
- LTO: Disabled

**Example:**
```bash
klang compile --mode=debug myapp.kl
KLANG_BUILD_MODE=debug klang compile myapp.kl
```

### 2. Development Mode (`--mode=dev`) [DEFAULT]

**Purpose:** Fast compilation with basic optimization
**Use case:** Day-to-day development

**Settings:**
- Optimization: O1 (basic)
- Debug symbols: Enabled
- Assertions: Enabled
- Bounds checking: Enabled
- Hot reload: Enabled
- LTO: Disabled

**Example:**
```bash
klang compile --mode=dev myapp.kl
klang serve --mode=dev --port=3000
```

### 3. Release Mode (`--mode=release`)

**Purpose:** Balanced optimization with safety checks
**Use case:** Pre-production testing, staging deployments

**Settings:**
- Optimization: O2 (default)
- Debug symbols: Enabled
- Assertions: Enabled
- Bounds checking: Enabled
- Hot reload: Disabled
- LTO: Enabled

**Example:**
```bash
klang compile --mode=release myapp.kl
```

### 4. Production Mode (`--mode=production`)

**Purpose:** Maximum performance and smallest binary size
**Use case:** Production deployments

**Settings:**
- Optimization: O3 (aggressive)
- Debug symbols: Stripped
- Assertions: Disabled
- Bounds checking: Disabled
- Hot reload: Disabled
- LTO: Enabled

**Example:**
```bash
klang compile --mode=production myapp.kl
KLANG_BUILD_MODE=production klang compile myapp.kl
```

## Optimization Levels

You can override the optimization level independently of build mode:

### O0 - No Optimization
```bash
klang compile --opt=O0 myapp.kl
klang compile -O0 myapp.kl
```
- Fastest compilation
- Largest binary
- Slowest execution
- Best for debugging

### O1 - Basic Optimization
```bash
klang compile --opt=O1 myapp.kl
klang compile -O1 myapp.kl
```
- Fast compilation
- Moderate binary size
- Good execution speed
- Good for development

### O2 - Default Optimization
```bash
klang compile --opt=O2 myapp.kl
klang compile -O2 myapp.kl
```
- Moderate compilation time
- Smaller binary size
- Fast execution
- Balanced for most uses

### O3 - Aggressive Optimization
```bash
klang compile --opt=O3 myapp.kl
klang compile -O3 myapp.kl
```
- Slowest compilation
- Smallest binary (with --strip)
- Fastest execution
- Best for production

## Advanced Compilation Flags

### Link-Time Optimization (LTO)

Enables whole-program optimization for maximum performance:

```bash
klang compile --lto myapp.kl
klang compile --no-lto myapp.kl
```

Set via environment:
```bash
export KLANG_ENABLE_LTO=1
```

### Strip Debug Symbols

Remove debug information to reduce binary size:

```bash
klang compile --strip myapp.kl
```

Set via environment:
```bash
export KLANG_STRIP_DEBUG=1
```

### Combining Flags

```bash
klang compile --mode=production --opt=O3 --lto --strip myapp.kl
```

## Environment Variables

Configure KLang build settings globally:

```bash
# Build mode
export KLANG_BUILD_MODE=production

# Optimization level
export KLANG_OPT_LEVEL=3

# LTO
export KLANG_ENABLE_LTO=1

# Strip debug symbols
export KLANG_STRIP_DEBUG=1

# C/C++ compatibility features
export KLANG_ENABLE_POINTERS=1
export KLANG_ENABLE_MANUAL_MEMORY=1
export KLANG_ENABLE_INLINE_ASM=1
```

## Configuration Command

View current configuration:

```bash
klang config
```

Output:
```
KLang Configuration:
  Colors enabled:    yes
  Debug mode:        no
  Indent size:       4
  Use tabs:          no
  History size:      1000
  Max line length:   100

Build Configuration:
  Build mode:        production
  Optimization:      O3
  Link-time opt:     yes
  Strip debug:       yes
  Assertions:        no
  Bounds checking:   no
  Hot reload:        no

C/C++ Compatibility:
  Pointers:          no
  Manual memory:     no
  Inline assembly:   no
```

## Performance Comparison

| Mode       | Compile Time | Binary Size | Runtime Speed | Use Case           |
|------------|--------------|-------------|---------------|--------------------|
| Debug      | Fastest      | Largest     | Slowest       | Debugging          |
| Dev        | Very Fast    | Large       | Good          | Development        |
| Release    | Moderate     | Medium      | Fast          | Testing/Staging    |
| Production | Slowest      | Smallest    | Fastest       | Production         |

## Best Practices

### Development Workflow

1. **Local Development:** Use dev mode
   ```bash
   klang serve --mode=dev --hot-reload
   ```

2. **Testing:** Use release mode
   ```bash
   klang compile --mode=release myapp.kl
   ./myapp
   ```

3. **Production:** Use production mode with all optimizations
   ```bash
   klang compile --mode=production --lto --strip myapp.kl
   ```

### CI/CD Pipeline

```bash
# Development builds
if [ "$ENV" = "development" ]; then
    export KLANG_BUILD_MODE=dev
fi

# Staging builds
if [ "$ENV" = "staging" ]; then
    export KLANG_BUILD_MODE=release
fi

# Production builds
if [ "$ENV" = "production" ]; then
    export KLANG_BUILD_MODE=production
    export KLANG_ENABLE_LTO=1
    export KLANG_STRIP_DEBUG=1
fi

klang compile myapp.kl
```

## Benchmarking

Compare different optimization levels:

```bash
# Benchmark with different modes
for mode in debug dev release production; do
    echo "Building with $mode mode..."
    klang compile --mode=$mode bench.kl -o bench_$mode
    time ./bench_$mode
done
```

## Troubleshooting

### Build is too slow
- Use dev mode: `--mode=dev`
- Disable LTO: `--no-lto`

### Binary is too large
- Use production mode: `--mode=production`
- Strip debug symbols: `--strip`
- Enable LTO: `--lto`

### Runtime is too slow
- Use higher optimization: `--opt=O3`
- Enable LTO: `--lto`
- Use production mode: `--mode=production`

### Need to debug
- Use debug mode: `--mode=debug`
- Disable optimizations: `--opt=O0`

## See Also

- [HTTP Server Guide](HTTP_SERVER.md)
- [C/C++ Compatibility Guide](C_CPP_FEATURES.md)
- [Performance Tuning](PERFORMANCE.md)
