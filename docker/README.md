# Docker Support for KLang

This directory contains Docker configurations for KLang.

## Quick Start

### Development Container

Build and run the development container:

```bash
docker-compose up -d
```

Or build manually:

```bash
docker build -f docker/Dockerfile.dev -t klang:dev .
docker run -it klang:dev
```

### Production Container

Build production image:

```bash
docker build -f docker/Dockerfile -t klang:latest .
docker run -it klang:latest klang --version
```

## Files

- **Dockerfile** - Production image (minimal, optimized)
- **Dockerfile.dev** - Development image (with tools, debugging)
- **docker-compose.yml** - Multi-container orchestration
- **.dockerignore** - Files to exclude from Docker build

## Usage Examples

### Run KLang REPL in Container

```bash
docker run -it klang:latest klang repl
```

### Execute a Script

```bash
docker run -v $(pwd):/app -w /app klang:latest klang run script.kl
```

### Development Environment

```bash
docker-compose up -d
docker-compose exec klang bash
```

### Build in Container

```bash
docker run -v $(pwd):/workspace -w /workspace klang:dev make
```

## Environment Variables

- `KLANG_NO_COLOR` - Disable colored output
- `KLANG_DEBUG` - Enable debug mode
- `KLANG_PATH` - Module search paths

## Image Sizes

- **Production**: ~200MB (Alpine-based)
- **Development**: ~500MB (includes dev tools)

## Support

For issues, see [GitHub Issues](https://github.com/k-kaundal/KLang/issues).
