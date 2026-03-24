# KLang Playground (Temporarily Unavailable)

**Note:** The KLang playground feature is currently undergoing maintenance and improvements. It is temporarily unavailable.

In the meantime, please use the KLang CLI to run and test your code locally.

## Using KLang Locally

### Installation
- **Real Code Execution**: Execute actual KLang code via backend API
- **Professional Code Editor**: CodeMirror with syntax highlighting and line numbers
- **Live Console Output**: See stdout, stderr, and errors in real-time
- **10+ Examples**: Learn from pre-built code snippets
- **Execution Controls**: Run, clear, and share code
- **Status Monitoring**: See execution mode, timing, and exit codes
- **Share Code**: Generate shareable URLs with code embedded
- **Keyboard Shortcuts**: Ctrl/Cmd+Enter to run code

### 🔧 Two Modes of Operation

#### 1. Backend Mode (Real Execution)
When connected to the backend server, the playground executes actual KLang code with full language features:
- ✅ All variables, functions, classes work
- ✅ Array methods (map, filter, reduce)
- ✅ Control flow (loops, conditionals)
- ✅ Error messages and stack traces
- ✅ Real execution timing
- ✅ Full KLang standard library

#### 2. Demo Mode (Simulated)
When backend is unavailable, falls back to simulated execution:
- ✅ Shows basic `println()` output
- ✅ Quick preview without setup
- ✅ Good for learning syntax
- ❌ Limited to simple output statements

## Setup Instructions

### Option 1: Quick Start (Demo Mode)
No setup needed! Just open `playground.html` in your browser to use demo mode.

```bash
# Open in browser
open playground.html
# or
firefox playground.html
```

### Option 2: Full Setup (Backend Mode)

#### Prerequisites
- Python 3.7+
- KLang built and ready (run `make` in repository root)

#### Quick Setup (Recommended)

Use the automated startup script:
```bash
./start-playground.sh
```

This script will:
- Check if KLang is built (builds it if needed)
- Install Python dependencies
- Start the backend server
- Show connection instructions

#### Manual Setup

1. **Install Python dependencies**
```bash
pip install -r playground-requirements.txt
```

2. **Build KLang** (if not already built)
```bash
# Install build dependencies
sudo apt-get install -y build-essential libreadline-dev llvm-16 llvm-16-dev

# Build
make
```

3. **Start the backend server**
```bash
# Production mode (recommended)
python3 playground-server.py

# Development mode with debug enabled
FLASK_DEBUG=1 python3 playground-server.py
```

**Security Note**: Debug mode is disabled by default for security. Only enable it during development.

The server will start on `http://localhost:5000`

4. **Open playground in browser**
```bash
# Open playground.html in your browser
open playground.html
# or
firefox playground.html
```

5. **Connect to backend**
- The playground automatically tries to connect to `http://localhost:5000`
- If not connected, click "Connect" button in settings panel
- Status bar will show "Backend (Real Execution)" when connected

## Usage

### Running Code
1. Write KLang code in the editor
2. Click "▶️ Run Code" button or press `Ctrl+Enter` (or `Cmd+Enter` on Mac)
3. View output in the console below

### Loading Examples
- Click any example in the left sidebar to load it
- Enable "Auto-run on example load" to automatically execute examples
- Examples cover: basics, functions, arrays, objects, loops, classes, and more

### Sharing Code
1. Write your code in the editor
2. Click "🔗 Share" button
3. URL with encoded code is copied to clipboard
4. Share the URL with others

### Settings
- **Backend URL**: Change if running backend on different host/port
- **Timeout**: Maximum execution time (1-30 seconds)
- **Auto-run**: Automatically run code when loading examples

## Console Features

### Output Types
- **Normal output** (white): Regular stdout
- **Error messages** (red): Execution errors and failures
- **Success messages** (green): Successful completion
- **Info messages** (cyan): System info and tips
- **Warning messages** (orange): Warnings
- **Stderr output** (yellow): Standard error stream

### Console Features
- **Timestamps**: Some messages show execution time
- **Scrolling**: Auto-scrolls to latest output
- **Clear button**: Clear console anytime
- **Formatted output**: Preserves formatting and line breaks

## Architecture

```
┌─────────────────┐         HTTP POST          ┌──────────────────┐
│                 │  ──────────────────────→    │                  │
│  playground.html│      /execute              │ playground-server│
│   (Frontend)    │         {code}             │    (Backend)     │
│                 │  ←──────────────────────    │                  │
└─────────────────┘    {stdout, stderr, ...}   └──────────────────┘
                                                        │
                                                        ↓
                                                 ┌──────────────┐
                                                 │    klang     │
                                                 │   (Binary)   │
                                                 └──────────────┘
```

### Frontend (playground.html)
- CodeMirror-based code editor
- Console output display
- Backend API client
- Example management
- URL-based code sharing

### Backend (playground-server.py)
- Flask REST API server
- Code execution with sandboxing
- Timeout and resource limits
- Error handling
- Health check endpoint

## API Endpoints

### `GET /health`
Health check and status

**Response:**
```json
{
  "status": "ok",
  "klang_available": true,
  "klang_path": "/path/to/klang"
}
```

### `POST /execute`
Execute KLang code

**Request:**
```json
{
  "code": "println('Hello, World!')",
  "timeout": 10
}
```

**Response:**
```json
{
  "success": true,
  "stdout": "Hello, World!\n",
  "stderr": "",
  "exit_code": 0,
  "execution_time": 0.123,
  "error": null,
  "timeout": false
}
```

## Security

The backend implements several security measures:
- ⏱️ **Execution timeout** (default 10s, max 30s)
- 📏 **Output size limits** (max 10KB)
- 📦 **Code size limits** (max 100KB)
- 🔒 **Process isolation** with subprocess
- 🛡️ **Temporary file cleanup**

**Note**: This playground is intended for development/learning. For production deployment, add:
- Rate limiting
- User authentication
- Container/VM sandboxing
- Network isolation
- Resource quotas

## Troubleshooting

### Backend won't start
```bash
# Check if port 5000 is in use
lsof -i :5000

# Try different port
# Edit playground-server.py, change port in app.run()
# Update Backend URL in playground settings
```

### KLang binary not found
```bash
# Build KLang
make

# Verify it exists
ls -la klang
./klang --version
```

### Cannot connect to backend
- Ensure backend is running: `python3 playground-server.py`
- Check URL in settings panel matches backend address
- Check browser console for CORS errors
- Try clicking "Connect" button manually

### Demo mode not showing output
- Demo mode only simulates `println()` statements
- For full execution, connect to backend
- Check that code contains `println()` calls

## Development

### Customizing Examples
Edit the `examples` object in `playground.html`:
```javascript
const examples = {
    myexample: `# My Custom Example
println("Custom code here")`,
    // ...
};
```

Add corresponding HTML in sidebar:
```html
<div class="example-item" data-example="myexample">
    <div class="example-title">My Example</div>
    <div class="example-desc">Description</div>
</div>
```

### Customizing Backend
Edit `playground-server.py` to:
- Change ports
- Add authentication
- Implement additional endpoints
- Add logging
- Customize security settings

## Future Enhancements

Potential improvements:
- 🔍 **Debugger**: Breakpoints, step-through execution
- 💾 **Code Persistence**: Save code to browser storage
- 📊 **Variable Inspector**: View variable values during execution
- 🎨 **Syntax Themes**: Multiple editor themes
- 🌐 **Cloud Deployment**: Host on cloud with proper sandboxing
- 📈 **Performance Profiling**: Execution time breakdown
- 🧪 **Test Runner**: Built-in unit testing
- 📚 **More Examples**: Expand example library
- 🔐 **User Accounts**: Save and manage code snippets

## License

Same as KLang - MIT License

## Contributing

Contributions welcome! Areas to improve:
- Better syntax highlighting (custom KLang mode)
- Enhanced error messages
- More examples
- Debugger implementation
- Mobile experience
- Performance optimizations

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.
