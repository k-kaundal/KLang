# KLang CLI Quick Reference Card

## 🚀 Essential Commands

```bash
klang                    # Start REPL (default)
klang repl              # Start REPL (explicit)
klang run <file>        # Run a program
klang build <file>      # Compile & run via VM
klang check <file>      # Check syntax only
klang info <file>       # Show file stats
klang --help            # Show help
klang --version         # Show version
```

## 📖 Getting Help

```bash
klang --help            # Full help with all commands
klang -h                # Short form
klang help run          # Help for 'run' command
klang help check        # Help for 'check' command
klang help <command>    # Help for any command
```

## 🎯 Common Workflows

### Development
```bash
# 1. Check syntax
klang check myapp.kl

# 2. View stats
klang info myapp.kl

# 3. Run program
klang run myapp.kl
```

### Testing Ideas in REPL
```bash
klang repl
> let x = 10
> x * 2
=> 20
> exit
```

### Batch Processing
```bash
# Check all files
for f in *.kl; do klang check "$f"; done

# Get stats for all files
for f in *.kl; do klang info "$f"; done
```

## ⚙️ Options

```bash
--no-color              # Disable colored output
-h, --help              # Show help
-v, --version           # Show version
```

## 📁 File Extensions

Supported: `.kl`, `.k`, `.klang`

```bash
klang run script.kl     # ✅ Valid
klang run script.k      # ✅ Valid
klang run script.klang  # ✅ Valid
klang run script.txt    # ❌ Invalid
```

## 🎨 Output Colors

- **Green** - Success, commands
- **Red** - Errors
- **Cyan** - Info, headers, prompts
- **Yellow** - Warnings, tips
- **Blue** - Links

Use `--no-color` to disable.

## ⌨️ REPL Commands

```bash
> exit                  # Exit REPL
> quit                  # Exit REPL
> exit(0)               # Exit with code 0
> exit(1)               # Exit with code 1
```

## 💡 Tips

1. **Always check syntax first**: `klang check file.kl`
2. **Use info for insights**: `klang info file.kl`
3. **Get help anytime**: `klang help <command>`
4. **Disable colors in scripts**: `klang --no-color check file.kl`

## 🐛 Troubleshooting

### Problem: Colors don't display
**Solution**: Your terminal may not support ANSI colors. Use `--no-color`:
```bash
klang --no-color --help
```

### Problem: "Unknown command" error
**Solution**: Check available commands:
```bash
klang --help
```

### Problem: "Invalid file extension" error
**Solution**: Use `.kl`, `.k`, or `.klang` extensions:
```bash
klang run script.kl
```

### Problem: Need more info about a command
**Solution**: Use command-specific help:
```bash
klang help run
klang help check
```

## 📚 Documentation

- [Visual Demo](CLI_VISUAL_DEMO.md) - See all features in action
- [Full Documentation](README.md) - Complete docs index
- [Getting Started](GETTING_STARTED.md) - Setup and basics
- [Language Reference](LANGUAGE_REFERENCE.md) - Syntax guide

## 🎓 Learn More

```bash
# Check current version
klang --version

# Get full help
klang --help

# Try the REPL
klang repl

# Run an example
klang run examples/hello.kl
```

---

**Happy coding with KLang! 🚀**

For issues or questions, visit: https://github.com/k-kaundal/KLang
