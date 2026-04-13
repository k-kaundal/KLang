# KLang Terminal UI Engine Documentation

## Overview

The **KLang Terminal UI Engine** is a complete framework for building interactive terminal applications, similar to how HTML/CSS/JavaScript work in web browsers. It provides a declarative, component-based approach to building beautiful CLI applications.

## Architecture

### Core Components

1. **UI Engine** (`stdlib/ui/engine.kl`) - Core application framework
2. **Components** (`stdlib/ui/components.kl`) - Built-in UI components
3. **Layout Engine** (`stdlib/ui/layout.kl`) - Layout management
4. **KLUI Parser** (`stdlib/ui/parser.kl`) - Markup language parser
5. **KLSS System** (`stdlib/ui/styles.kl`) - Styling system

## Getting Started

### Basic Example

```klang
# Create an application
let app = createApp({
    "title": "My First App",
    "width": 80,
    "height": 24
})

# Create a root container
let root = Container({"title": "Welcome", "border": true})

# Add components
let text = Text({"content": "Hello, KLang UI!", "color": "cyan", "bold": true})
root.addChild(text)

let progress = ProgressBar({"label": "Loading", "value": 75})
root.addChild(progress)

# Set root and start
app.setRoot(root)
app.start()
```

## KLUI Markup Language

### Syntax

KLUI is a markup language for defining UI structure declaratively:

```xml
<App title="My Application">
  <Container border="true">
    <Panel title="Dashboard" color="cyan">
      <Text bold="true">Welcome!</Text>
      <ProgressBar label="Progress" value="75" />
    </Panel>
    
    <Spacer size="1" />
    
    <List items="Item 1,Item 2,Item 3" />
  </Container>
</App>
```

### Tags

#### Container Tags
- `<App>` - Application container
- `<Container>` - Generic container
- `<Panel>` - Bordered panel with title
- `<Stack>` - Vertical/horizontal stack layout
- `<Grid>` - Multi-column grid layout
- `<Split>` - Two-pane split layout
- `<Tabs>` - Tabbed interface

#### Content Tags
- `<Text>` - Text content
- `<Button>` - Clickable button
- `<Input>` - Text input field
- `<List>` - List of items
- `<Table>` - Data table
- `<ProgressBar>` - Progress indicator

#### Layout Tags
- `<Spacer>` - Empty space
- `<Divider>` - Horizontal divider line

### Attributes

Common attributes for all components:
- `width` - Component width
- `height` - Component height
- `visible` - Visibility (true/false)
- `id` - Unique identifier
- `class` - CSS-like class name

Component-specific attributes:
- `title` - Title text (Panel, Container)
- `border` - Show border (Container, Panel)
- `color` - Color name (Text, Panel)
- `bold` - Bold text (Text)
- `align` - Text alignment (Text)
- `label` - Label text (ProgressBar, Button)
- `value` - Value (ProgressBar, Input)
- `items` - List items (List)
- `headers` - Table headers (Table)
- `rows` - Table rows (Table)

### Usage

```klang
# Parse KLUI markup
let markup = "<App><Panel title=\"Hello\"><Text>Hi!</Text></Panel></App>"
let root = parseKLUI(markup)

# Render
let output = renderKLUI(root)
print(output)
```

## KLSS Styling System

### Syntax

KLSS is a styling language similar to CSS:

```css
/* Tag selector */
Panel {
    border: true;
    color: cyan;
    padding: 2;
}

/* Class selector */
.primary-button {
    style: primary;
    bold: true;
    color: cyan;
}

/* ID selector */
#main-panel {
    width: 60;
    height: 20;
}
```

### Selectors

1. **Tag Selector** - Matches component type
   ```css
   Button { style: primary; }
   ```

2. **Class Selector** - Matches class attribute
   ```css
   .danger { color: red; }
   ```

3. **ID Selector** - Matches id attribute
   ```css
   #header { height: 3; }
   ```

### Properties

#### Colors
- `color` - Text/border color (red, green, yellow, blue, cyan, magenta, white)

#### Typography
- `bold` - Bold text (true/false)
- `italic` - Italic text (true/false)
- `underline` - Underlined text (true/false)
- `align` - Text alignment (left, center, right)

#### Layout
- `width` - Component width (number)
- `height` - Component height (number)
- `padding` - Internal padding (number)
- `border` - Show border (true/false)

#### Component-specific
- `style` - Button style (default, primary, success, danger)
- `direction` - Stack direction (vertical, horizontal)
- `columns` - Grid columns (number)
- `ratio` - Split ratio (percentage)

### Themes

```klang
# Create a theme
let theme = createDarkTheme()

# Apply to component tree
theme.apply(rootComponent)
```

### Usage

```klang
# Parse KLSS
let css = "Panel { color: cyan; } .btn { style: primary; }"
let stylesheet = parseKLSS(css)

# Apply to components
stylesheet.apply(rootComponent)
```

## Built-in Components

### Container
A generic container for other components.

```klang
let container = Container({
    "title": "My Container",
    "border": true,
    "padding": 1
})
```

### Panel
A bordered panel with a title.

```klang
let panel = Panel({
    "title": "Status",
    "color": "cyan"
})
```

### Text
Display text content.

```klang
let text = Text({
    "content": "Hello World",
    "color": "green",
    "bold": true,
    "align": "center"
})
```

### Button
An interactive button.

```klang
let button = Button({
    "label": "Click Me",
    "style": "primary",
    "onClick": handleClick
})
```

### Input
Text input field.

```klang
let input = Input({
    "placeholder": "Enter text...",
    "value": "",
    "onChange": handleChange
})
```

### List
Display a list of items.

```klang
let list = List({
    "items": ["Item 1", "Item 2", "Item 3"],
    "selectedIndex": 0,
    "showNumbers": true
})
```

### Table
Data table with headers.

```klang
let table = Table({
    "headers": ["Name", "Status", "Count"],
    "rows": [
        ["Alice", "Active", "10"],
        ["Bob", "Inactive", "5"]
    ],
    "columnWidths": [15, 10, 8]
})
```

### ProgressBar
Visual progress indicator.

```klang
let progress = ProgressBar({
    "label": "Loading...",
    "value": 75,
    "showPercent": true
})
```

## Layout System

### Stack
Arrange components vertically or horizontally.

```klang
let stack = Stack({
    "direction": "vertical",
    "spacing": 1,
    "align": "start"
})

stack.addChild(component1)
stack.addChild(component2)
```

### Grid
Multi-column grid layout.

```klang
let grid = Grid({
    "columns": 3,
    "gap": 1
})

grid.addChild(item1)
grid.addChild(item2)
grid.addChild(item3)
```

### Split
Two-pane layout.

```klang
let split = Split({
    "direction": "horizontal",
    "ratio": 50  // 50/50 split
})

split.addChild(leftPane)
split.addChild(rightPane)
```

### Tabs
Tabbed interface.

```klang
let tabs = Tabs({
    "activeTab": 0,
    "tabs": [
        {"title": "Home", "content": homeContent},
        {"title": "Settings", "content": settingsContent}
    ]
})
```

## Component Lifecycle

### Methods

1. **onCreate()** - Called when component is created
2. **onMount()** - Called before first render
3. **onUpdate(prevState)** - Called when state changes
4. **onUnmount()** - Called before component is removed

### State Management

```klang
# Set state
component.setState({"count": 5, "active": true})

# Access state
let count = component.state["count"]
```

### Event Handling

```klang
# Add event handler
component.on("click", fn(data) {
    println("Clicked!")
})

# Emit event
component.emit("click", {})
```

## Application Class

### Creating an App

```klang
let app = createApp({
    "title": "My App",
    "width": 80,
    "height": 24
})
```

### Methods

- `setRoot(component)` - Set root component
- `start()` - Start the application
- `stop()` - Stop the application
- `render()` - Render current state

## Examples

### Example 1: Simple Dashboard

```klang
let app = createApp({"title": "Dashboard", "width": 80, "height": 24})

let root = Container({"title": "System Dashboard", "border": true})

# Stats panel
let stats = Panel({"title": "Stats", "color": "cyan"})
stats.addChild(ProgressBar({"label": "CPU", "value": 65}))
stats.addChild(ProgressBar({"label": "Memory", "value": 78}))
root.addChild(stats)

# Services list
let services = Panel({"title": "Services", "color": "green"})
services.addChild(List({"items": ["Web Server - Running", "Database - Running"]}))
root.addChild(services)

app.setRoot(root)
app.start()
```

### Example 2: Using KLUI/KLSS

```klang
# Define UI in KLUI
let markup = "
<App>
  <Panel class=\"header\" title=\"Todo App\">
    <Text class=\"subtitle\">Manage your tasks</Text>
  </Panel>
  <List id=\"task-list\" />
  <Button class=\"primary-btn\">Add Task</Button>
</App>
"

# Define styles in KLSS
let styles = "
.header { color: green; }
.subtitle { color: yellow; }
#task-list { border: true; }
.primary-btn { style: primary; }
"

# Parse and apply
let root = parseKLUI(markup)
let stylesheet = parseKLSS(styles)
stylesheet.apply(root)

# Create and run app
let app = createApp({"width": 80, "height": 24})
app.setRoot(root)
app.start()
```

## Best Practices

1. **Component Composition** - Build complex UIs from simple components
2. **Separation of Concerns** - Keep structure (KLUI) and styling (KLSS) separate
3. **State Management** - Use component state for dynamic data
4. **Event Handling** - Respond to user interactions
5. **Layouts** - Use layout components for consistent spacing
6. **Themes** - Create themes for consistent styling

## File Organization

Recommended project structure:

```
my-app/
├── app.kl              # Main application file
├── ui/
│   ├── views/
│   │   ├── dashboard.klui
│   │   ├── settings.klui
│   │   └── ...
│   ├── styles/
│   │   ├── theme.klss
│   │   ├── components.klss
│   │   └── ...
│   └── components/
│       ├── custom-button.kl
│       └── ...
└── logic/
    ├── state.kl
    └── handlers.kl
```

## Advanced Features

### Custom Components

```klang
class CustomWidget extends Component {
    fn init(props) {
        super.init(props)
        this.customData = props["data"]
    }
    
    fn render() {
        return "Custom: " + this.customData + "\n"
    }
}
```

### Dynamic Updates

```klang
# Update component state
component.setState({"value": newValue})

# This triggers automatic re-render
```

### Nested Layouts

```klang
let layout = Stack({"direction": "vertical"})

let topRow = Grid({"columns": 2})
topRow.addChild(panel1)
topRow.addChild(panel2)

let bottomRow = Split({"direction": "horizontal", "ratio": 60})
bottomRow.addChild(leftPane)
bottomRow.addChild(rightPane)

layout.addChild(topRow)
layout.addChild(bottomRow)
```

## Performance Tips

1. Minimize unnecessary re-renders
2. Use appropriate layout components
3. Avoid deeply nested component trees
4. Cache static content
5. Batch state updates

## Troubleshooting

### Component not rendering
- Check if component is visible
- Verify parent-child relationships
- Ensure render() method returns string

### Styles not applying
- Check selector syntax
- Verify stylesheet is applied to component tree
- Ensure properties match component expectations

### Layout issues
- Check width/height properties
- Verify layout component configuration
- Test with simpler layouts first

## Future Enhancements

- [ ] Animation support
- [ ] Mouse input handling
- [ ] Keyboard navigation
- [ ] Form validation
- [ ] Data binding
- [ ] Virtual scrolling
- [ ] Component library expansion

## Contributing

To add new components:
1. Extend Component base class
2. Implement render() method
3. Add to component registry
4. Update documentation

## License

Part of the KLang project - MIT License

---

**Build terminal UIs as easily as building web apps!**
