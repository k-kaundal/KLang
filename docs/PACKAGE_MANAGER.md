# KLang Package Manager (kpkg)

The KLang package manager provides a simple and powerful way to manage dependencies in your KLang projects.

## Installation

The package manager is built into the KLang CLI. No separate installation is required.

## Quick Start

### Initialize a Package

```bash
klang pkg init myproject
```

This creates a `package.kl.json` file:

```json
{
  "name": "myproject",
  "version": "1.0.0",
  "description": "",
  "author": "",
  "license": "MIT",
  "main": "main.kl"
}
```

### Install a Package

```bash
klang pkg install packagename@1.0.0
```

Or install the latest version:

```bash
klang pkg install packagename
```

### List Installed Packages

```bash
klang pkg list
```

### Uninstall a Package

```bash
klang pkg uninstall packagename
```

### Update Packages

Update a specific package:

```bash
klang pkg update packagename
```

Update all packages:

```bash
klang pkg update
```

## Package Manifest

The `package.kl.json` file defines your project and its dependencies:

```json
{
  "name": "myapp",
  "version": "1.0.0",
  "description": "My KLang application",
  "author": "Your Name",
  "license": "MIT",
  "main": "main.kl",
  "dependencies": {
    "http-client": "1.2.0",
    "json-parser": "2.0.1"
  }
}
```

### Fields

- **name**: Project name (required)
- **version**: Project version in semver format (required)
- **description**: Project description
- **author**: Author name
- **license**: License type (default: MIT)
- **main**: Entry point file (default: main.kl)
- **dependencies**: Map of package names to versions

## Package Structure

Packages are installed in `.kpkg/packages/` directory:

```
myproject/
├── package.kl.json
├── main.kl
└── .kpkg/
    ├── cache/
    └── packages/
        ├── packagename/
        │   ├── package.kl.json
        │   └── lib/
        └── another-package/
```

## Using Installed Packages

Import packages in your KLang code:

```klang
import packagename

# Use package functions
packagename.someFunction()
```

## Creating Packages

To create a package that others can use:

1. Initialize your package:
```bash
klang pkg init mypackage
```

2. Create your library code in `lib/`:
```
mypackage/
├── package.kl.json
└── lib/
    └── mypackage.kl
```

3. Export your API:
```klang
# lib/mypackage.kl
fn myFunction() {
    println("Hello from mypackage!")
}

export {
    myFunction
}
```

## Versioning

KLang packages follow [Semantic Versioning](https://semver.org/):

- **MAJOR.MINOR.PATCH** (e.g., 1.2.3)
  - MAJOR: Breaking changes
  - MINOR: New features (backward compatible)
  - PATCH: Bug fixes (backward compatible)

Version constraints:
- `1.2.3` - Exact version
- `^1.2.3` - Compatible with 1.x.x (>= 1.2.3, < 2.0.0)
- `~1.2.3` - Compatible with 1.2.x (>= 1.2.3, < 1.3.0)
- `latest` - Latest version

## Package Registry

The package manager supports local packages. A central package registry is planned for future releases.

## Commands Reference

| Command | Description |
|---------|-------------|
| `pkg init <name>` | Initialize a new package |
| `pkg install <pkg[@version]>` | Install a package |
| `pkg uninstall <pkg>` | Uninstall a package |
| `pkg update [pkg]` | Update package(s) |
| `pkg list` | List installed packages |

## Tips

- Use `package.kl.json` to track all dependencies
- Lock versions for production builds
- Keep packages small and focused
- Document your package API
- Test your packages before publishing

## Future Features

- Central package registry
- Package publishing
- Dependency resolution with conflict handling
- Package scripts and lifecycle hooks
- Private package support
