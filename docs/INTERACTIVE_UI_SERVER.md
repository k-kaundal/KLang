# KLang Interactive Terminal UI Server

## Overview

The KLang Interactive Terminal UI Server provides a **browser-like interactive experience** directly in the terminal. Build full-featured terminal applications with real UI components (buttons, inputs, dropdowns) that respond to mouse clicks and keyboard input in real-time.

## Features

### ✨ Core Capabilities

- **🖱️ Mouse Support**: Click buttons, select items, interact with UI elements
- **⌨️ Keyboard Navigation**: Tab between components, keyboard shortcuts
- **🎨 Rich Components**: Buttons, inputs, checkboxes, selects, radio buttons, modals
- **🔄 Real-time Updates**: Components update instantly based on user interaction
- **📊 State Management**: Built-in state system with two-way data binding
- **🎯 Event System**: Event emitters and handlers for all interactions
- **⚡ High Performance**: 60 FPS rendering with efficient screen updates
- **🎪 Focus Management**: Automatic focus handling and visual feedback

### 🎨 Interactive Components

All components support:
- Mouse events (click, hover, enter, leave)
- Keyboard events (keypress, special keys)
- Focus states with visual feedback
- Enable/disable states
- Custom styling
- Event handlers

## Quick Start

### Simple Button Example

```klang
import "stdlib/ui/server.kl" as server

fn main() {
    # Create UI server
    let app = server.createServer({
        title: "My App",
        width: 80,
        height: 24
    })
    
    # Create a button
    let btn = server.createButton("Click Me!", {
        x: 10,
        y: 5,
        style: "primary"
    })
    
    # Handle click events
    btn.on("click", (e) => {
        println("Button clicked!")
    })
    
    # Add to server and start
    app.addComponent(btn)
    app.start()
}

main()
```

### Input Field Example

```klang
import "stdlib/ui/server.kl" as server

fn main() {
    let app = server.createServer({
        title: "Input Demo"
    })
    
    # Create input
    let input = server.createInput("Enter your name...", {
        x: 10,
        y: 5,
        width: 40
    })
    
    # Handle changes
    input.on("change", (e) => {
        println("Value changed: " + e.data.value)
    })
    
    # Handle submit (Enter key)
    input.on("submit", (e) => {
        println("Submitted: " + e.data.value)
    })
    
    app.addComponent(input)
    app.start()
}

main()
```

## Component Reference

### Button

Interactive button with click events and visual states.

```klang
let button = server.createButton("Label", {
    x: 10,           # X position
    y: 5,            # Y position
    style: "primary" # primary, danger, success, default
})

button.on("click", (e) => {
    # Handle click
})

button.on("focus", (e) => {
    # Handle focus
})

button.on("blur", (e) => {
    # Handle blur (unfocus)
})
```

**Styles:**
- `primary` - Cyan highlighted button
- `danger` - Red button for destructive actions
- `success` - Green button for positive actions
- `default` - Standard button

**Visual States:**
- Normal: `[ Label ]`
- Focused: `> Label <` (bold)
- Hovered: Underlined
- Disabled: Dimmed

### Input

Text input field with real-time editing and cursor control.

```klang
let input = server.createInput("Placeholder text", {
    x: 10,
    y: 5,
    width: 40,
    maxLength: 100,
    type: "text"
})

input.on("change", (e) => {
    # Called on every character typed
    let value = e.data.value
})

input.on("submit", (e) => {
    # Called when Enter is pressed
    let value = e.data.value
})
```

**Keyboard Controls:**
- Type characters to add text
- `Backspace` - Delete character before cursor
- `Delete` - Delete character at cursor
- `ArrowLeft`/`ArrowRight` - Move cursor
- `Home`/`End` - Jump to start/end
- `Enter` - Submit (triggers submit event)

**Properties:**
- `value` - Current input value
- `placeholder` - Placeholder text
- `cursorPos` - Current cursor position
- `maxLength` - Maximum characters allowed

### Checkbox

Toggle checkbox with label.

```klang
let checkbox = server.createCheckbox("Enable feature", {
    x: 10,
    y: 5,
    checked: false
})

checkbox.on("change", (e) => {
    let isChecked = e.data.checked
})
```

**Keyboard Controls:**
- `Space` or `Enter` - Toggle checkbox

**Visual States:**
- Checked: `[✓] Label` (green)
- Unchecked: `[ ] Label`

### Select / Dropdown

Dropdown selection with keyboard navigation.

```klang
let select = server.createSelect(
    ["Option 1", "Option 2", "Option 3"],
    {
        x: 10,
        y: 5,
        selectedIndex: 0
    }
)

select.on("change", (e) => {
    let value = e.data.value
    let index = e.data.index
})

select.on("toggle", (e) => {
    let expanded = e.data.expanded
})
```

**Keyboard Controls:**
- `Enter` or `Space` - Toggle dropdown open/closed
- `ArrowDown` - Select next option (or open if closed)
- `ArrowUp` - Select previous option
- `Escape` - Close dropdown

**Visual States:**
- Closed: `[Selected] ▼`
- Open: `[Selected] ▲` with options below
- Selected option highlighted with `> ` prefix

### Radio Group

Radio button group for single selection.

```klang
let radio = server.createRadioGroup(
    ["Small", "Medium", "Large"],
    {
        x: 10,
        y: 5,
        label: "Size:",
        selectedIndex: 1
    }
)

radio.on("change", (e) => {
    let value = e.data.value
    let index = e.data.index
})
```

**Keyboard Controls:**
- `ArrowDown` - Select next option
- `ArrowUp` - Select previous option
- `Space` or `Enter` - Confirm selection

**Visual States:**
- Selected: `(•) Label` (green)
- Unselected: `( ) Label`

### Modal / Dialog

Modal dialog with buttons.

```klang
let modal = server.createModal(
    "Confirm Action",
    "Are you sure you want to continue?",
    {
        buttons: ["Yes", "No", "Cancel"],
        width: 50,
        height: 10
    }
)

modal.on("buttonClick", (e) => {
    let button = e.data.button
    let index = e.data.index
})

modal.on("cancel", (e) => {
    # Escape key pressed
})

# Show/hide modal
modal.show()
modal.hide()
```

**Keyboard Controls:**
- `ArrowLeft`/`ArrowRight` - Navigate buttons
- `Enter` - Click selected button
- `Escape` - Cancel (hide modal)

## Advanced Usage

### State Management

Components have built-in state management:

```klang
class CustomComponent extends ui.InteractiveComponent {
    constructor(props) {
        super(props)
        this.state = {
            count: 0,
            message: "Hello"
        }
    }
    
    increment() {
        # setState automatically triggers re-render
        this.setState({
            count: this.state.count + 1
        })
    }
    
    render() {
        return "Count: " + str(this.state.count)
    }
}
```

### Custom Components

Create your own interactive components:

```klang
import "stdlib/ui/interactive.kl" as ui

class Counter extends ui.InteractiveComponent {
    constructor(props) {
        super(props)
        this.count = 0
    }
    
    handleClick(x, y) {
        super.handleClick(x, y)
        this.count = this.count + 1
        this.emit("countChange", {count: this.count})
        this.render()
    }
    
    handleKeyPress(key) {
        super.handleKeyPress(key)
        if (key == "+") {
            this.count = this.count + 1
        } else if (key == "-") {
            this.count = this.count - 1
        }
        this.render()
    }
    
    render() {
        return cli.BOLD + "Count: " + str(this.count) + cli.RESET
    }
}
```

### Event System

All components extend `EventEmitter`:

```klang
# Listen to events
component.on("eventName", (e) => {
    # e.type - Event type
    # e.data - Event data
    # e.timestamp - Event timestamp
})

# Remove listener
component.off("eventName", handler)

# Emit custom events
component.emit("customEvent", {myData: "value"})

# Stop event propagation
component.on("click", (e) => {
    e.stopPropagation()
})
```

**Built-in Events:**
- `click` - Mouse click
- `focus` - Component gained focus
- `blur` - Component lost focus
- `mouseenter` - Mouse entered component
- `mouseleave` - Mouse left component
- `keypress` - Key pressed while focused
- `stateChange` - Component state changed
- `change` - Value changed (inputs, checkboxes, etc.)
- `submit` - Form submitted (inputs)

### Layout Management

Position components absolutely:

```klang
# Grid layout
for (let row = 0; row < 3; row++) {
    for (let col = 0; col < 3; col++) {
        let btn = server.createButton("Button", {
            x: 2 + (col * 15),
            y: 2 + (row * 3)
        })
        app.addComponent(btn)
    }
}

# Stacked layout
let y = 5
for (let item of items) {
    let component = createComponent(item, {
        x: 10,
        y: y
    })
    app.addComponent(component)
    y = y + component.height + 1
}
```

### Focus Management

Control component focus:

```klang
# Tab order is automatic based on registration order
# Override with manual focus control:

component.focus()      # Give focus
component.blur()       # Remove focus

# Navigate focus programmatically
app.focusManager.next()      # Move to next component
app.focusManager.previous()  # Move to previous component

# Get currently focused component
let focused = app.focusManager.getFocused()
```

## Server Configuration

### Creating a Server

```klang
let app = server.createServer({
    title: "App Title",     # Title bar text
    width: 80,              # Screen width (columns)
    height: 24,             # Screen height (rows)
    fps: 60                 # Target frames per second
})
```

### Server Methods

```klang
# Add components
app.addComponent(component)

# Remove components
app.removeComponent(component)

# Request re-render
app.requestRender()

# Manual render
app.render()

# Start server (blocking)
app.start()

# Stop server
app.stop()
```

### Server Events

```klang
app.on("start", (e) => {
    # Server started
})

app.on("stop", (e) => {
    # Server stopping
})

app.on("cleanup", (e) => {
    # Server cleanup complete
})
```

## Keyboard Controls

### Global Keys

- `Tab` - Move focus to next component
- `Shift+Tab` - Move focus to previous component
- `Ctrl+C` or `Ctrl+Q` - Exit application

### Component Keys

Each component responds to relevant keys:
- Arrow keys for navigation
- Enter/Space for activation
- Escape for cancellation
- Character keys for text input

## Mouse Support

Mouse features (when enabled):
- **Click** - Activate buttons, toggle checkboxes
- **Hover** - Visual feedback on mouse over
- **Drag** - Select text, move components (custom)

Enable mouse support:
```klang
app.input.enableMouse()
```

## Performance Tips

### Efficient Rendering

```klang
# Use render() sparingly
# Components auto-render on state change

# For manual updates:
app.render()  # Full render

# For partial updates (more efficient):
app.screen.renderDiff()  # Only changed cells
```

### Component Visibility

```klang
# Hide unused components
component.visible = false

# Disable for non-interactive display
component.enabled = false
```

### Frame Rate

```klang
# Lower FPS for less CPU usage
let app = server.createServer({
    fps: 30  # 30 FPS instead of 60
})
```

## Examples

See the `examples/` directory for complete applications:

- **`interactive_ui_server.kl`** - Basic interactive components demo
- **`todo_app_interactive.kl`** - Full-featured todo app with CRUD operations
- **`dashboard_interactive.kl`** - Real-time monitoring dashboard

## CLI Integration

### Serve UI Command

```bash
# Start interactive UI server
klang serve-ui myapp.kl

# With options
klang serve-ui --port=3000 --hot-reload myapp.kl
```

### Development Mode

```bash
# Hot reload on file changes
klang serve-ui --mode=dev --hot-reload myapp.kl
```

## Browser-Like Features

The terminal UI server provides a **browser-like experience**:

1. **Interactive Elements**: Click buttons, type in inputs, select from dropdowns
2. **Focus Management**: Tab navigation like web forms
3. **Visual Feedback**: Hover states, focus indicators
4. **Event Handling**: Click, change, submit events
5. **State Management**: Component state like React
6. **Real-time Updates**: Instant UI updates like SPAs

## Comparison with Web UI

| Feature | Web Browser | Terminal UI Server |
|---------|-------------|-------------------|
| Mouse Support | ✓ | ✓ |
| Keyboard Navigation | ✓ | ✓ |
| Interactive Components | ✓ | ✓ |
| Real-time Updates | ✓ | ✓ |
| Rich Styling | CSS | ANSI + Box Drawing |
| Layout Engine | Flexbox/Grid | Absolute Positioning |
| Event System | DOM Events | EventEmitter |
| State Management | React/Vue | Built-in |

## Best Practices

1. **Component Organization**: Group related components
2. **Event Handlers**: Keep handlers simple and focused
3. **State Management**: Use setState for automatic updates
4. **Performance**: Hide/disable unused components
5. **Keyboard Navigation**: Ensure all features keyboard-accessible
6. **Visual Feedback**: Provide clear focus and hover states
7. **Error Handling**: Handle invalid input gracefully

## Troubleshooting

### Mouse Not Working

```klang
# Ensure mouse is enabled
app.input.enableMouse()
```

### Components Not Responding

```klang
# Check component is enabled
component.enabled = true

# Check component is visible
component.visible = true

# Check component is registered
app.addComponent(component)
```

### Rendering Issues

```klang
# Force full render
app.render()

# Clear and re-render
app.screen.clear()
app.render()
```

## Future Enhancements

- WebSocket support for remote UIs
- Touch gesture support
- Animation system
- Responsive layouts
- Theme system
- Accessibility improvements

## See Also

- [UI Components Documentation](./UI_COMPONENTS.md)
- [Event System Guide](./EVENT_SYSTEM.md)
- [CLI Library](../stdlib/cli.kl)
- [Examples](../examples/)
