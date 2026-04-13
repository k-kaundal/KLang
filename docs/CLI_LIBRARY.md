# KLang CLI Library Documentation

## Overview

The **KLang CLI Library** (`stdlib/cli.kl`) provides beautiful command-line interface utilities for creating stunning terminal applications. Built entirely in KLang, it offers colors, box drawing, progress bars, tables, menus, and more!

## Features

- 🎨 **Colors and Text Formatting** - Full ANSI color support with bright colors and styles
- 📦 **Box Drawing** - Single, double, and rounded box characters
- 📊 **Progress Bars** - Simple and colored progress indicators
- 📋 **Tables** - Beautiful table rendering with headers
- 🎯 **Menus** - Interactive menu displays
- ✅ **Status Messages** - Success, error, warning, and info messages
- 📝 **Lists** - Bulleted, numbered, and checklists
- 🎪 **Headers and Banners** - Eye-catching section headers
- 🎨 **Icons and Symbols** - Check marks, arrows, stars, and more
- 🔧 **Utilities** - Text centering, padding, screen control

## Installation

The CLI library is included in the KLang standard library. Simply import it in your KLang programs:

```klang
# In future versions with module system
import cli from "stdlib/cli"

# For now, the library is available globally when installed
```

## Quick Start

### Basic Colors

```klang
# Import CLI utilities
# Colors
println(red("This is red text"))
println(green("This is green text"))
println(blue("This is blue text"))
println(yellow("This is yellow text"))

# Styles
println(bold("Bold text"))
println(italic("Italic text"))
println(underline("Underlined text"))

# Combined
println(boldGreen("Bold green text"))
println(boldRed("Bold red text"))
```

### Drawing Boxes

```klang
# Simple box with title
drawBox(30, 3, "Welcome")

# Double line box
drawDoubleBox(30, 3, "Important")

# Rounded box
drawRoundBox(30, 3, "Info")

# Box around text
boxText("Hello, KLang!", 2)
```

### Progress Bars

```klang
# Simple progress bar
progressBar(50, 100, 40, true)  # 50% complete, 40 chars wide, show percent

# Colored progress bar (auto-colors based on progress)
coloredProgressBar(75, 100, 40, true)  # Green at 75%
coloredProgressBar(25, 100, 40, true)  # Red at 25%
```

### Status Messages

```klang
success("Operation completed successfully!")
error("Failed to connect to database")
warning("Low disk space")
info("Server started on port 3000")

# Step indicators
step(1, 5, "Installing dependencies...")
step(2, 5, "Building application...")
```

### Tables

```klang
let headers = ["Name", "Email", "Role"]
let rows = [
    ["Alice", "alice@example.com", "Admin"],
    ["Bob", "bob@example.com", "User"],
    ["Charlie", "charlie@example.com", "User"]
]

drawTable(headers, rows)
```

### Menus

```klang
let options = [
    "Start Application",
    "Configure Settings",
    "View Logs",
    "Exit"
]

menu("Main Menu", options)
```

### Lists

```klang
# Bulleted list
let items = ["Feature 1", "Feature 2", "Feature 3"]
bulletList(items)

# Numbered list
numberedList(items)

# Checklist
let completed = [true, true, false]
checklist(items, completed)
```

## API Reference

### Colors

#### Text Colors
- `red(text)` - Red text
- `green(text)` - Green text
- `yellow(text)` - Yellow text
- `blue(text)` - Blue text
- `magenta(text)` - Magenta text
- `cyan(text)` - Cyan text
- `white(text)` - White text
- `gray(text)` - Gray text

#### Bright Colors
- `brightRed(text)` - Bright red
- `brightGreen(text)` - Bright green
- `brightYellow(text)` - Bright yellow
- `brightBlue(text)` - Bright blue
- `brightCyan(text)` - Bright cyan

#### Text Styles
- `bold(text)` - Bold text
- `dim(text)` - Dimmed text
- `italic(text)` - Italic text
- `underline(text)` - Underlined text

#### Combined Styles
- `boldGreen(text)` - Bold green
- `boldRed(text)` - Bold red
- `boldYellow(text)` - Bold yellow
- `boldBlue(text)` - Bold blue
- `boldCyan(text)` - Bold cyan

### Box Drawing

- `drawBox(width, height, title)` - Single line box
- `drawDoubleBox(width, height, title)` - Double line box
- `drawRoundBox(width, height, title)` - Rounded corner box
- `boxText(text, padding)` - Simple border around text
- `drawLine(length, char)` - Horizontal line

### Progress Bars

- `progressBar(current, total, width, showPercent)` - Simple progress bar
- `coloredProgressBar(current, total, width, showPercent)` - Auto-colored progress bar

### Tables

- `drawTable(headers, rows)` - Draw a table with headers and rows

### Menus

- `menu(title, options)` - Display a menu with options

### Status Messages

- `success(message)` - Green success message with ✓
- `error(message)` - Red error message with ✗
- `warning(message)` - Yellow warning message with ⚠
- `info(message)` - Cyan info message with ℹ
- `question(message)` - Blue question prompt with ?
- `step(number, total, message)` - Step indicator [n/total]

### Lists

- `bulletList(items)` - Bulleted list with •
- `numberedList(items)` - Numbered list (1., 2., 3., ...)
- `checklist(items, completed)` - Checklist with ✓ and □

### Headers and Banners

- `header(text, style)` - Fancy header (styles: "single", "double", "round")
- `banner(text)` - Large banner with borders
- `section(title)` - Section separator

### Separators

- `separator(width, char)` - Horizontal separator
- `coloredSeparator(width, char, colorCode)` - Colored separator
- `section(title)` - Section title with separator

### Loading

- `loading(message)` - Display loading message
- `loadingComplete()` - Mark loading as complete

### Utilities

- `center(text, width)` - Center text in width
- `pad(text, width, char)` - Pad text to width
- `clearScreen()` - Clear terminal screen
- `moveCursor(row, col)` - Move cursor to position
- `hideCursor()` - Hide cursor
- `showCursor()` - Show cursor

## Constants

### Colors
```klang
BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, GRAY
BRIGHT_RED, BRIGHT_GREEN, BRIGHT_YELLOW, BRIGHT_BLUE, BRIGHT_CYAN, BRIGHT_WHITE
BG_BLACK, BG_RED, BG_GREEN, BG_YELLOW, BG_BLUE, BG_MAGENTA, BG_CYAN, BG_WHITE
```

### Styles
```klang
BOLD, DIM, ITALIC, UNDERLINE, BLINK, REVERSE, HIDDEN, STRIKETHROUGH, RESET
```

### Box Characters
```klang
BOX_SINGLE_HORIZONTAL, BOX_SINGLE_VERTICAL
BOX_SINGLE_TOP_LEFT, BOX_SINGLE_TOP_RIGHT
BOX_SINGLE_BOTTOM_LEFT, BOX_SINGLE_BOTTOM_RIGHT
BOX_DOUBLE_HORIZONTAL, BOX_DOUBLE_VERTICAL
BOX_ROUND_TOP_LEFT, BOX_ROUND_TOP_RIGHT
# ... and more
```

### Icons
```klang
ICON_CHECK (✓), ICON_CROSS (✗), ICON_ARROW_RIGHT (→)
ICON_STAR (★), ICON_HEART (♥), ICON_WARNING (⚠)
ICON_INFO (ℹ), ICON_BULLET (•), ICON_ROCKET (🚀)
# ... and more
```

## Examples

### Example 1: Simple CLI App

```klang
# Display welcome message
header("My KLang App", "double")

# Show info
info("Application started successfully")

# Show progress
loading("Processing data")
# ... do work ...
loadingComplete()

# Show results
success("Data processed successfully!")
```

### Example 2: Build Status Dashboard

```klang
banner("Build Status")

section("Build Progress")
step(1, 4, "Compiling source files...")
coloredProgressBar(100, 100, 40, true)

step(2, 4, "Running tests...")
coloredProgressBar(75, 100, 40, true)

section("Test Results")
let headers = ["Suite", "Passed", "Failed"]
let rows = [
    ["Unit Tests", "123", "0"],
    ["Integration", "45", "2"]
]
drawTable(headers, rows)

success("Build completed!")
```

### Example 3: Interactive Menu

```klang
fn main() {
    let running = true
    
    while running {
        clearScreen()
        header("Main Menu", "round")
        
        let options = [
            "Start Application",
            "Settings",
            "Help",
            "Exit"
        ]
        
        menu("Please select an option", options)
        
        print("Enter choice: ")
        let choice = input()
        
        if choice == "1" {
            success("Starting application...")
        } else if choice == "4" {
            running = false
        }
    }
}
```

## Complete Examples

Two complete example applications are included:

1. **cli_demo.kl** - Comprehensive demonstration of all CLI features
2. **cli_task_manager.kl** - Practical task management application

Run them with:
```bash
klang run examples/cli_demo.kl
klang run examples/cli_task_manager.kl
```

## Best Practices

1. **Use appropriate colors** - Green for success, red for errors, yellow for warnings
2. **Be consistent** - Stick to one box style throughout your app
3. **Provide feedback** - Use progress bars and status messages
4. **Clear organization** - Use headers and sections to organize content
5. **Test in terminal** - Ensure your UI looks good in different terminal sizes

## Platform Support

The CLI library uses ANSI escape codes which are supported on:
- ✅ Linux terminals
- ✅ macOS Terminal
- ✅ Windows Terminal (Windows 10+)
- ✅ WSL (Windows Subsystem for Linux)
- ⚠️ Limited support on older Windows Command Prompt

## Performance

The CLI library is optimized for performance:
- All functions are pure KLang with no external dependencies
- String concatenation is used efficiently
- Minimal memory overhead

## Contributing

Want to add more features to the CLI library? We welcome contributions!

- Add new box styles
- Create more icon sets
- Implement animations
- Add more color schemes
- Create layout utilities

## License

The KLang CLI Library is part of the KLang standard library and is released under the MIT License.

---

**Made with ❤️ for the KLang community**

For more information, visit the [KLang GitHub repository](https://github.com/k-kaundal/KLang).
