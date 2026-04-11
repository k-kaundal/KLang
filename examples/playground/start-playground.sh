#!/bin/bash

# KLang Playground Startup Script
# This script sets up and starts the KLang playground

set -e

echo "🚀 KLang Playground Setup"
echo "=========================="
echo ""

# Check if KLang is built
if [ ! -f "./klang" ]; then
    echo "❌ KLang binary not found!"
    echo "   Building KLang..."
    echo ""
    make || {
        echo "❌ Build failed. Please install dependencies first:"
        echo "   sudo apt-get install -y build-essential libreadline-dev llvm-16 llvm-16-dev"
        exit 1
    }
    echo "✅ KLang built successfully"
    echo ""
fi

# Check Python
if ! command -v python3 &> /dev/null; then
    echo "❌ Python 3 not found. Please install Python 3.7+"
    exit 1
fi

echo "✅ Python 3 found: $(python3 --version)"

# Check if pip is installed
if ! command -v pip3 &> /dev/null; then
    echo "⚠️  pip3 not found. Trying pip..."
    if ! command -v pip &> /dev/null; then
        echo "❌ pip not found. Please install pip"
        exit 1
    fi
    PIP_CMD="pip"
else
    PIP_CMD="pip3"
fi

# Install Python dependencies
echo ""
echo "📦 Installing Python dependencies..."
$PIP_CMD install -q -r playground-requirements.txt || {
    echo "⚠️  Failed to install dependencies. Trying without user flag..."
    $PIP_CMD install -r playground-requirements.txt
}

echo "✅ Dependencies installed"
echo ""

# Start the server
echo "🌐 Starting KLang Playground Server..."
echo "   Server will run on: http://localhost:5000"
echo "   Open playground.html in your browser to use it"
echo ""
echo "Press Ctrl+C to stop the server"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""

python3 playground-server.py
