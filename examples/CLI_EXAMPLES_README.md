# KLang CLI Examples

This directory contains beautiful command-line interface examples demonstrating the KLang CLI library (`stdlib/cli.kl`).

## 📁 Files

### CLI Library Demo (`cli_demo.kl`)
A comprehensive demonstration of all CLI library features including:
- ✨ Colors and text formatting (16 colors + bright variants)
- 📦 Box drawing (single, double, rounded)
- 📊 Progress bars (simple and colored)
- 📋 Tables with headers
- 🎯 Menus and interactive displays
- ✅ Status messages (success, error, warning, info)
- 📝 Lists (bulleted, numbered, checklists)
- 🎪 Headers and banners
- 🎨 Icons and symbols
- 🔧 Complex dashboard example

**Run the demo:**
```bash
klang run examples/cli_demo.kl
```

**What you'll see:**
The demo showcases 10 different sections, each demonstrating a specific CLI feature set. It's a visual feast of what's possible with the KLang CLI library!

### Task Manager (`cli_task_manager.kl`)
A practical, real-world example of a task management application with:
- ✓ Interactive menu system
- ✓ Add, view, complete, and delete tasks
- ✓ Task priorities (high, medium, low)
- ✓ Beautiful table displays
- ✓ Progress tracking with colored progress bars
- ✓ Statistics dashboard
- ✓ Color-coded status indicators

**Run the task manager:**
```bash
klang run examples/cli_task_manager.kl
```

**Features:**
1. **Add Task** - Create new tasks with priority levels
2. **View Tasks** - See all tasks in a beautiful table
3. **Mark Complete** - Check off completed tasks
4. **Delete Task** - Remove tasks you no longer need
5. **Statistics** - View completion progress with charts
6. **Exit** - Cleanly exit the application

## 🎨 CLI Library Features

The `stdlib/cli.kl` library provides everything you need to create beautiful command-line applications:

### Colors
```klang
println(red("Error message"))
println(green("Success message"))
println(yellow("Warning message"))
println(blue("Info message"))
println(boldGreen("Important success!"))
```

### Boxes
```klang
drawBox(30, 3, "My Box")
drawDoubleBox(30, 3, "Important")
drawRoundBox(30, 3, "Friendly")
boxText("Hello!", 2)
```

### Progress Bars
```klang
progressBar(50, 100, 40, true)       # 50% complete
coloredProgressBar(75, 100, 40, true) # Auto-colors: green at 75%
```

### Tables
```klang
let headers = ["Name", "Status", "Score"]
let rows = [
    ["Alice", "Active", "95"],
    ["Bob", "Inactive", "82"]
]
drawTable(headers, rows)
```

### Status Messages
```klang
success("Build completed!")
error("Failed to connect")
warning("Low disk space")
info("Server running on port 3000")
```

### Menus
```klang
let options = ["Start", "Settings", "Help", "Exit"]
menu("Main Menu", options)
```

### Lists
```klang
bulletList(["Feature 1", "Feature 2", "Feature 3"])
numberedList(["Step 1", "Step 2", "Step 3"])
checklist(["Task 1", "Task 2"], [true, false])
```

## 🚀 Creating Your Own CLI App

Here's a minimal example to get you started:

```klang
# Import CLI utilities (when module system is available)
# For now, use the functions directly from stdlib/cli.kl

fn main() {
    # Clear screen and show header
    clearScreen()
    header("My CLI App", "double")
    
    # Show a menu
    let options = [
        "Option 1",
        "Option 2",
        "Exit"
    ]
    menu("Main Menu", options)
    
    # Get user input
    print(cyan("Choose: "))
    let choice = input()
    
    # Process choice
    if choice == "1" {
        success("You chose option 1!")
    } else if choice == "2" {
        info("You chose option 2!")
    } else {
        warning("Exiting...")
    }
}

main()
```

## 📖 Documentation

For complete documentation on all CLI functions and features, see:
- **[CLI Library Documentation](../docs/CLI_LIBRARY.md)** - Full API reference
- **[Standard Library README](../stdlib/README.md)** - All stdlib modules

## 🎯 Use Cases

The CLI library is perfect for:
- ✅ Task managers and todo lists
- ✅ System monitoring dashboards
- ✅ Build and deployment tools
- ✅ Database query tools
- ✅ DevOps automation scripts
- ✅ Interactive configuration wizards
- ✅ Log viewers and analyzers
- ✅ Package managers and installers

## 🎨 Terminal Compatibility

The CLI library uses ANSI escape codes which work on:
- ✅ Linux terminals (all distributions)
- ✅ macOS Terminal
- ✅ Windows Terminal (Windows 10+)
- ✅ WSL (Windows Subsystem for Linux)
- ✅ Most modern terminal emulators
- ⚠️ Limited support on older Windows Command Prompt

## 💡 Tips

1. **Test in your terminal** - Different terminals may render colors slightly differently
2. **Use consistent styling** - Pick one box style and stick with it
3. **Color coding** - Red for errors, green for success, yellow for warnings
4. **Keep it simple** - Don't overuse colors and decorations
5. **Provide feedback** - Use progress bars and status messages
6. **Handle errors gracefully** - Always show clear error messages

## 🤝 Contributing

Want to add more CLI examples or features? We'd love to see:
- More practical applications (file managers, system monitors, etc.)
- New UI patterns (split views, tabs, popups)
- Animation examples (spinners, loading sequences)
- Advanced layouts (multi-column, nested boxes)
- Theme support (dark mode, light mode, custom colors)

## 📝 License

These examples are part of the KLang project and are released under the MIT License.

---

**Made with ❤️ for the KLang community**

For more information, visit the [KLang GitHub repository](https://github.com/k-kaundal/KLang).
