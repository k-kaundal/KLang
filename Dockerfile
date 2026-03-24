# KLang Docker Image
# Multi-stage build for optimized image size

# Build stage
FROM ubuntu:22.04 AS builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    libreadline-dev \
    llvm-16-dev \
    wget \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /build

# Copy source code
COPY . .

# Build KLang
RUN make clean && make

# Runtime stage
FROM ubuntu:22.04

# Install runtime dependencies only
RUN apt-get update && apt-get install -y \
    libreadline8 \
    llvm-16 \
    curl \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Create klang user (non-root)
RUN useradd -m -s /bin/bash klang

# Copy binary from build stage
COPY --from=builder /build/klang /usr/local/bin/klang

# Copy examples and docs
COPY --from=builder /build/examples /opt/klang/examples
COPY --from=builder /build/docs /opt/klang/docs
COPY --from=builder /build/stdlib /opt/klang/stdlib

# Set permissions
RUN chmod +x /usr/local/bin/klang && \
    chown -R klang:klang /opt/klang

# Switch to non-root user
USER klang
WORKDIR /home/klang

# Set environment variables
ENV KLANG_HOME=/opt/klang
ENV PATH="${PATH}:/usr/local/bin"

# Add labels
LABEL maintainer="KLang Team"
LABEL version="0.2.0"
LABEL description="KLang - The AI-Native Programming Language"
LABEL org.opencontainers.image.source="https://github.com/k-kaundal/KLang"

# Default command
CMD ["klang", "repl"]

# Health check
HEALTHCHECK --interval=30s --timeout=3s --start-period=5s --retries=3 \
    CMD klang --version || exit 1
