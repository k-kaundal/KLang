# KLang Plugin System

## Overview

KLang features an extensible plugin system that allows developers to extend the language with custom functionality, new syntax, and integrations.

## Plugin Architecture

### Plugin Types

1. **Syntax Plugins** - Add new syntax constructs
2. **Library Plugins** - Extend standard library
3. **Tool Plugins** - Add development tools
4. **Integration Plugins** - Connect with external services
5. **Compiler Plugins** - Modify compilation process

### Plugin Structure

```
my-plugin/
├── plugin.json          # Plugin manifest
├── src/                 # Plugin source code
│   ├── init.kl         # Initialization code
│   └── lib/            # Plugin libraries
├── tests/              # Plugin tests
├── docs/               # Documentation
└── README.md           # Plugin description
```

## Creating a Plugin

### 1. Plugin Manifest (plugin.json)

```json
{
  "name": "my-awesome-plugin",
  "version": "1.0.0",
  "description": "An awesome KLang plugin",
  "author": "Your Name",
  "license": "MIT",
  "klangVersion": ">=0.2.0",
  "type": "library",
  "entry": "src/init.kl",
  "dependencies": {
    "another-plugin": "^1.0.0"
  },
  "exports": {
    "MyClass": "src/lib/MyClass.kl",
    "myFunction": "src/lib/functions.kl"
  },
  "hooks": {
    "beforeCompile": "src/hooks/beforeCompile.kl",
    "afterCompile": "src/hooks/afterCompile.kl"
  },
  "config": {
    "enableFeature": true,
    "apiEndpoint": "https://api.example.com"
  }
}
```

### 2. Initialization Code (src/init.kl)

```klang
# Plugin initialization
export class MyPlugin {
    fn init() {
        println("MyPlugin initialized!")
        this.registerHooks()
        this.setupConfig()
    }
    
    fn registerHooks() {
        plugin.on("compile:start", this.onCompileStart)
        plugin.on("runtime:start", this.onRuntimeStart)
    }
    
    fn setupConfig() {
        let config = plugin.getConfig()
        this.enabled = config.enableFeature
    }
    
    fn onCompileStart(context) {
        # Hook implementation
        println("Compilation starting...")
    }
    
    fn onRuntimeStart(runtime) {
        # Hook implementation
        println("Runtime starting...")
    }
}

# Auto-initialize when loaded
let instance = MyPlugin()
instance.init()
```

### 3. Plugin API Usage

```klang
# In your KLang code
import "my-awesome-plugin" as MyPlugin

let result = MyPlugin.myFunction()
let obj = MyPlugin.MyClass()
```

## Plugin Manager

### Installation

```bash
# Install from registry
klang plugin install my-awesome-plugin

# Install from GitHub
klang plugin install github:username/my-plugin

# Install from local directory
klang plugin install ./path/to/plugin

# Install specific version
klang plugin install my-awesome-plugin@1.2.0
```

### Management

```bash
# List installed plugins
klang plugin list

# Show plugin info
klang plugin info my-awesome-plugin

# Update plugin
klang plugin update my-awesome-plugin

# Uninstall plugin
klang plugin uninstall my-awesome-plugin

# Enable/disable plugin
klang plugin enable my-awesome-plugin
klang plugin disable my-awesome-plugin

# Search plugins
klang plugin search database

# Publish plugin
klang plugin publish
```

### Configuration

```bash
# Configure plugin
klang plugin config my-awesome-plugin set apiKey "your-key"

# Show plugin config
klang plugin config my-awesome-plugin show
```

## Plugin Registry

### Publishing

1. Create plugin following structure above
2. Test thoroughly
3. Create git repository
4. Tag version: `git tag v1.0.0`
5. Publish: `klang plugin publish`

### Registry Structure

```
~/.klang/plugins/
├── registry.json          # Plugin registry cache
├── my-awesome-plugin/     # Installed plugin
│   ├── plugin.json
│   └── src/
└── another-plugin/
    ├── plugin.json
    └── src/
```

## Plugin API Reference

### Core Plugin API

```klang
# Available in all plugins
namespace plugin {
    # Plugin metadata
    fn getName() -> String
    fn getVersion() -> String
    fn getConfig() -> Object
    
    # Event system
    fn on(event: String, handler: Function)
    fn emit(event: String, data: Any)
    fn off(event: String, handler: Function)
    
    # Logging
    fn log(message: String)
    fn warn(message: String)
    fn error(message: String)
    
    # Storage
    fn getData(key: String) -> Any
    fn setData(key: String, value: Any)
    fn deleteData(key: String)
    
    # HTTP client
    fn http.get(url: String) -> Response
    fn http.post(url: String, data: Object) -> Response
}
```

### Compiler Hooks

```klang
# Available in compiler plugins
namespace compiler {
    # AST manipulation
    fn getAST() -> ASTNode
    fn transformAST(transformer: Function)
    
    # Code generation
    fn addImport(module: String)
    fn injectCode(code: String, position: String)
    
    # Optimization
    fn addOptimization(optimizer: Function)
}
```

### Runtime Hooks

```klang
# Available in runtime plugins
namespace runtime {
    # Execution control
    fn beforeExecution(handler: Function)
    fn afterExecution(handler: Function)
    
    # Memory management
    fn onAlloc(handler: Function)
    fn onFree(handler: Function)
    
    # Debugging
    fn onBreakpoint(handler: Function)
    fn getStackTrace() -> Array
}
```

## Example Plugins

### 1. HTTP Server Plugin

```klang
# plugin.json
{
  "name": "klang-http-server",
  "version": "1.0.0",
  "type": "library",
  "entry": "src/server.kl"
}

# src/server.kl
export class HTTPServer {
    fn __init__(port: Int) {
        this.port = port
        this.routes = {}
    }
    
    fn get(path: String, handler: Function) {
        this.routes[path] = { method: "GET", handler: handler }
    }
    
    fn post(path: String, handler: Function) {
        this.routes[path] = { method: "POST", handler: handler }
    }
    
    fn listen() {
        println("Server listening on port ${this.port}")
        # Implementation using native bindings
    }
}
```

### 2. Database ORM Plugin

```klang
# plugin.json
{
  "name": "klang-orm",
  "version": "1.0.0",
  "type": "library",
  "entry": "src/orm.kl"
}

# src/orm.kl
export class Model {
    fn __init__(tableName: String) {
        this.table = tableName
    }
    
    fn find(id: Int) -> Object {
        # Database query implementation
    }
    
    fn where(conditions: Object) -> Array {
        # Query builder
    }
    
    fn save() {
        # Save to database
    }
}
```

### 3. Testing Framework Plugin

```klang
# plugin.json
{
  "name": "klang-test",
  "version": "1.0.0",
  "type": "tool",
  "entry": "src/test.kl"
}

# src/test.kl
export fn describe(name: String, tests: Function) {
    println("Suite: ${name}")
    tests()
}

export fn it(description: String, test: Function) {
    try {
        test()
        println("  ✓ ${description}")
    } catch (e) {
        println("  ✗ ${description}")
        println("    ${e}")
    }
}

export fn expect(actual: Any) -> Expectation {
    return Expectation(actual)
}

class Expectation {
    fn __init__(actual: Any) {
        this.actual = actual
    }
    
    fn toBe(expected: Any) {
        if (this.actual != expected) {
            throw "Expected ${expected}, got ${this.actual}"
        }
    }
    
    fn toEqual(expected: Any) {
        # Deep equality check
    }
}
```

## Security Considerations

### Plugin Sandboxing

Plugins run in a sandboxed environment with limited access:

- File system: Limited to plugin directory
- Network: Requires explicit permission
- System calls: Restricted
- Memory: Limited allocation

### Permission System

```json
{
  "permissions": {
    "filesystem": ["read", "write"],
    "network": ["http", "https"],
    "system": ["exec"]
  }
}
```

### Code Signing

Plugins can be signed for verification:

```bash
# Sign plugin
klang plugin sign my-plugin

# Verify signature
klang plugin verify my-plugin
```

## Best Practices

1. **Version Compatibility**: Always specify minimum KLang version
2. **Error Handling**: Handle errors gracefully
3. **Documentation**: Provide comprehensive docs
4. **Testing**: Include tests in your plugin
5. **Performance**: Be mindful of performance impact
6. **Security**: Follow security guidelines
7. **Semver**: Use semantic versioning

## Plugin Development Tools

### CLI Tools

```bash
# Create new plugin scaffold
klang plugin create my-plugin

# Validate plugin
klang plugin validate

# Test plugin
klang plugin test

# Bundle plugin
klang plugin bundle

# Publish to registry
klang plugin publish
```

### Testing

```klang
# tests/test_plugin.kl
import "klang-test" as test

test.describe("MyPlugin", () => {
    test.it("should initialize correctly", () => {
        let plugin = MyPlugin()
        test.expect(plugin.isInitialized()).toBe(true)
    })
})
```

## Contributing Plugins

See [CONTRIBUTING.md](../CONTRIBUTING.md) for guidelines on contributing plugins to the official registry.

## Plugin Registry

Official plugin registry: https://plugins.klang.dev

Browse, search, and discover community plugins.

## Support

- Documentation: https://docs.klang.dev/plugins
- Forum: https://forum.klang.dev
- Issues: https://github.com/k-kaundal/KLang/issues

---

**Happy plugin development! 🔌**
