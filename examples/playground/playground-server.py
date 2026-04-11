#!/usr/bin/env python3
"""
KLang Playground Backend API Server

This server provides a REST API for executing KLang code safely in a sandboxed environment.
It handles code execution requests from the playground UI and returns results.

⚠️ SECURITY WARNING:
This is a demonstration/development server only. DO NOT use in production!
- Exposes stack traces to users (for debugging)
- Executes arbitrary code
- No rate limiting or authentication
- Intended for local development/testing only

For production use, implement:
- Sandboxing (Docker, VMs, etc.)
- Rate limiting
- Authentication
- Proper error handling (no stack traces)
- Input validation and sanitization
"""

import os
import sys
import json
import subprocess
import tempfile
import time
import signal
from pathlib import Path
from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)  # Enable CORS for frontend requests

# Configuration
KLANG_BINARY = os.path.join(os.path.dirname(__file__), 'klang')
MAX_EXECUTION_TIME = 10  # seconds
MAX_OUTPUT_SIZE = 10000  # characters


class ExecutionResult:
    """Container for code execution results"""
    def __init__(self):
        self.stdout = ""
        self.stderr = ""
        self.exit_code = 0
        self.execution_time = 0.0
        self.error = None
        self.timeout = False


def execute_klang_code(code, timeout=MAX_EXECUTION_TIME):
    """
    Execute KLang code safely with timeout and resource limits
    
    Args:
        code: The KLang source code to execute
        timeout: Maximum execution time in seconds
        
    Returns:
        ExecutionResult object with execution details
    """
    result = ExecutionResult()
    start_time = time.time()
    
    # Create temporary file for code
    with tempfile.NamedTemporaryFile(mode='w', suffix='.kl', delete=False) as f:
        temp_file = f.name
        f.write(code)
    
    try:
        # Check if KLang binary exists
        if not os.path.exists(KLANG_BINARY):
            result.error = "KLang binary not found. Please build KLang first with 'make'."
            result.exit_code = 1
            return result
        
        # Execute KLang code
        process = subprocess.Popen(
            [KLANG_BINARY, 'run', temp_file],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            preexec_fn=os.setsid if hasattr(os, 'setsid') else None
        )
        
        try:
            # Wait for completion with timeout
            stdout, stderr = process.communicate(timeout=timeout)
            result.stdout = stdout[:MAX_OUTPUT_SIZE]
            result.stderr = stderr[:MAX_OUTPUT_SIZE]
            result.exit_code = process.returncode
            
        except subprocess.TimeoutExpired:
            # Kill process group on timeout
            if hasattr(os, 'killpg'):
                os.killpg(os.getpgid(process.pid), signal.SIGKILL)
            else:
                process.kill()
            
            result.timeout = True
            result.error = f"Execution timeout after {timeout} seconds"
            result.exit_code = -1
            
            # Try to get partial output
            try:
                stdout, stderr = process.communicate(timeout=1)
                result.stdout = stdout[:MAX_OUTPUT_SIZE]
                result.stderr = stderr[:MAX_OUTPUT_SIZE]
            except:
                pass
                
    except Exception as e:
        result.error = str(e)
        result.exit_code = 1
        
    finally:
        # Clean up temp file
        try:
            os.unlink(temp_file)
        except:
            pass
        
        result.execution_time = time.time() - start_time
    
    return result


@app.route('/health', methods=['GET'])
def health_check():
    """Health check endpoint"""
    klang_available = os.path.exists(KLANG_BINARY)
    return jsonify({
        'status': 'ok',
        'klang_available': klang_available,
        'klang_path': KLANG_BINARY
    })


@app.route('/execute', methods=['POST'])
def execute():
    """
    Execute KLang code and return results
    
    Request JSON:
        {
            "code": "println('Hello, World!')",
            "timeout": 10  // optional, default 10 seconds
        }
    
    Response JSON:
        {
            "success": true/false,
            "stdout": "...",
            "stderr": "...",
            "exit_code": 0,
            "execution_time": 0.123,
            "error": null/string,
            "timeout": false
        }
    """
    try:
        # Get request data
        data = request.get_json()
        if not data or 'code' not in data:
            return jsonify({
                'success': False,
                'error': 'Missing "code" parameter'
            }), 400
        
        code = data['code']
        timeout = min(data.get('timeout', MAX_EXECUTION_TIME), MAX_EXECUTION_TIME)
        
        # Validate code
        if not code or not code.strip():
            return jsonify({
                'success': False,
                'error': 'Code cannot be empty'
            }), 400
        
        if len(code) > 100000:  # 100KB limit
            return jsonify({
                'success': False,
                'error': 'Code too large (max 100KB)'
            }), 400
        
        # Execute code
        result = execute_klang_code(code, timeout)
        
        # Build response
        response = {
            'success': result.exit_code == 0 and not result.error,
            'stdout': result.stdout,
            'stderr': result.stderr,
            'exit_code': result.exit_code,
            'execution_time': round(result.execution_time, 3),
            'error': result.error,
            'timeout': result.timeout
        }
        
        return jsonify(response)
        
    except Exception as e:
        return jsonify({
            'success': False,
            'error': f'Server error: {str(e)}'
        }), 500


@app.route('/examples', methods=['GET'])
def get_examples():
    """Get list of available example files"""
    examples_dir = Path(__file__).parent / 'examples'
    examples = []
    
    if examples_dir.exists():
        for file in examples_dir.glob('*.k*'):
            try:
                with open(file, 'r') as f:
                    content = f.read()
                examples.append({
                    'name': file.stem,
                    'filename': file.name,
                    'code': content
                })
            except:
                pass
    
    return jsonify({'examples': examples})


if __name__ == '__main__':
    # Check if KLang binary exists
    if not os.path.exists(KLANG_BINARY):
        print(f"⚠️  Warning: KLang binary not found at {KLANG_BINARY}")
        print("   Please run 'make' to build KLang first.")
        print("   The server will start but code execution will fail.")
        print()
    
    # Check if running in development mode
    debug_mode = os.environ.get('FLASK_DEBUG', '0') == '1'
    
    print("🚀 KLang Playground Server")
    print(f"   KLang Binary: {KLANG_BINARY}")
    print(f"   Max Execution Time: {MAX_EXECUTION_TIME}s")
    print(f"   Debug Mode: {'ON (development)' if debug_mode else 'OFF (production)'}")
    print(f"   Starting server on http://localhost:5000")
    print()
    
    if not debug_mode:
        print("ℹ️  Set FLASK_DEBUG=1 environment variable to enable debug mode")
        print()
    
    app.run(host='0.0.0.0', port=5000, debug=debug_mode)
