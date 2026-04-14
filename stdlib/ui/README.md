# KLang Terminal UI Engine

Build beautiful, interactive terminal applications with a web-like development experience!

## 🎯 Overview

The KLang Terminal UI Engine is a complete framework for building terminal user interfaces, bringing the ease and power of web development (HTML/CSS/React) to the command line.

**NEW in v1.1:** Interactive UI Server with full mouse and keyboard support for browser-like terminal applications!

```klang
# Interactive server with real mouse/keyboard events
import "stdlib/ui/server.kl" as server

let app = server.createServer({title: "My App"})
let btn = server.createButton("Click Me!", {x: 10, y: 5})
btn.on("click", () => println("Clicked!"))
app.addComponent(btn)
app.start()

# Or use declarative markup (like HTML)
<App title="My Dashboard">
  <Panel title="Welcome" color="cyan">
    <Text bold="true">Hello, Terminal UI!</Text>
    <ProgressBar label="Loading" value="75" />
  </Panel>
</App>
```

## ✨ Features

### 🖱️ **Interactive UI Server** (NEW!)
- Full mouse support (click, hover, drag)
- Keyboard navigation (Tab, arrows, shortcuts)
- Interactive components (buttons, inputs, selects, checkboxes)
- Real-time event handling
- State management
- 60 FPS rendering

### 🎨 **Declarative UI**
- KLUI markup language (like HTML)
- Component-based architecture
- Familiar syntax for web developers

### 🎭 **Styling System**
- KLSS stylesheet language (like CSS)
- Tag, class, and ID selectors
- Theme support
- Separation of structure and style

### 📦 **Rich Component Library**
- **Containers**: Container, Panel, Stack, Grid, Split, Tabs
- **Content**: Text, Button, Input, List, Table
- **Layout**: Spacer, Divider, ProgressBar
- **All customizable and extensible**

### 🎯 **Layout Engine**
- Stack layouts (vertical/horizontal)
- Grid layouts (multi-column)
- Split layouts (two-pane)
- Tabbed interfaces

### 🔄 **State Management**
- Component state
- Reactive updates
- Automatic re-rendering

### 🎮 **Event System**
- Event handlers
- Event bubbling
- Custom events

## 🚀 Quick Start

### Interactive UI Server (NEW!)

```klang
import "stdlib/ui/server.kl" as server

# Create server
let app = server.createServer({
    title: "Interactive App",
    width: 80,
    height: 24
})

# Add interactive button
let btn = server.createButton("Click Me!", {
    x: 10,
    y: 5,
    style: "primary"
})

btn.on("click", (e) => {
    println("Button clicked!")
})

# Add input field
let input = server.createInput("Enter text...", {
    x: 10,
    y: 8,
    width: 40
})

input.on("submit", (e) => {
    println("Submitted:", e.data.value)
})

app.addComponent(btn)
app.addComponent(input)
app.start()
```

See [Interactive UI Server Guide](../../docs/INTERACTIVE_UI_SERVER.md) for full documentation.

### Installation

The UI Engine is part of the KLang standard library in `stdlib/ui/`:

```
stdlib/ui/
├── engine.kl        # Core framework
├── components.kl    # Built-in components
├── layout.kl        # Layout system
├── parser.kl        # KLUI markup parser
├── styles.kl        # KLSS styling system
├── interactive.kl   # Interactive components (NEW!)
└── server.kl        # UI server with events (NEW!)
```

### Hello World

```klang
# Create an application
let app = createApp({
    "title": "Hello World",
    "width": 60,
    "height": 10
})

# Create UI
let root = Container({"title": "Greeting", "border": true})
let text = Text({"content": "Hello, KLang UI!", "color": "cyan", "bold": true})
root.addChild(text)

# Run
app.setRoot(root)
app.start()
```

### Using KLUI Markup

```klang
let markup = "
<App title=\"My App\">
  <Container border=\"true\">
    <Panel title=\"Dashboard\" color=\"cyan\">
      <Text bold=\"true\">Welcome!</Text>
      <ProgressBar label=\"Progress\" value=\"75\" />
    </Panel>
  </Container>
</App>
"

let root = parseKLUI(markup)
let app = createApp({"width": 80, "height": 24})
app.setRoot(root)
app.start()
```

### Using KLSS Styles

```klang
let styles = "
Panel { border: true; color: cyan; }
.header { color: green; bold: true; }
.primary-btn { style: primary; }
"

let stylesheet = parseKLSS(styles)
stylesheet.apply(rootComponent)
```

## 📚 Examples

### Example 1: Dashboard

```klang
<App title="System Dashboard">
  <Stack direction="vertical" spacing="1">
    <Panel title="Quick Stats" color="cyan">
      <ProgressBar label="CPU Usage" value="65" />
      <ProgressBar label="Memory" value="78" />
    </Panel>
    
    <Grid columns="2" gap="1">
      <Panel title="Services" color="green">
        <List items="Web Server,Database,Cache" />
      </Panel>
      
      <Panel title="Tasks" color="yellow">
        <List items="Deploy,Backup,Update" />
      </Panel>
    </Grid>
  </Stack>
</App>
```

### Example 2: Multi-View App

```klang
<App title="Project Manager">
  <Tabs activeTab="0">
    <Tab title="Dashboard">
      <Panel title="Overview">
        <Text>5 active projects, 12 tasks</Text>
      </Panel>
    </Tab>
    
    <Tab title="Projects">
      <Table 
        headers="Project,Status,Progress"
        rows="Website,Complete,100%|Mobile,Active,60%" />
    </Tab>
    
    <Tab title="Tasks">
      <List items="Task 1,Task 2,Task 3" />
    </Tab>
  </Tabs>
</App>
```

## 🎨 Component Reference

### Core Components

#### Container
```klang
<Container title="My Container" border="true" padding="1">
  <!-- children -->
</Container>
```

#### Panel
```klang
<Panel title="Panel Title" color="cyan">
  <!-- children -->
</Panel>
```

#### Text
```klang
<Text color="green" bold="true" align="center">
  Content here
</Text>
```

#### Button
```klang
<Button label="Click Me" style="primary" onClick="handleClick" />
```

### Layout Components

#### Stack
```klang
<Stack direction="vertical" spacing="1">
  <Component1 />
  <Component2 />
</Stack>
```

#### Grid
```klang
<Grid columns="3" gap="1">
  <Item1 />
  <Item2 />
  <Item3 />
</Grid>
```

#### Split
```klang
<Split direction="horizontal" ratio="60">
  <LeftPane />
  <RightPane />
</Split>
```

#### Tabs
```klang
<Tabs activeTab="0">
  <Tab title="Tab 1">Content 1</Tab>
  <Tab title="Tab 2">Content 2</Tab>
</Tabs>
```

### Data Components

#### List
```klang
<List 
  items="Item 1,Item 2,Item 3"
  selectedIndex="0"
  showNumbers="true" />
```

#### Table
```klang
<Table 
  headers="Name,Status,Count"
  rows="Alice,Active,10|Bob,Inactive,5"
  columnWidths="15,10,8" />
```

#### ProgressBar
```klang
<ProgressBar 
  label="Loading..."
  value="75"
  showPercent="true" />
```

## 🎭 Styling Guide

### Tag Selectors
```css
Panel {
    border: true;
    color: cyan;
}

Button {
    style: primary;
    bold: true;
}
```

### Class Selectors
```css
.header {
    color: green;
    bold: true;
}

.primary-button {
    style: primary;
    color: cyan;
}
```

### ID Selectors
```css
#main-panel {
    width: 60;
    height: 20;
}
```

### Theme Support
```klang
let theme = createDarkTheme()
theme.apply(rootComponent)
```

## 📖 Full Documentation

- [Complete UI Engine Documentation](../docs/UI_ENGINE.md)
- [Component API Reference](../docs/UI_ENGINE.md#built-in-components)
- [KLUI Markup Guide](../docs/UI_ENGINE.md#klui-markup-language)
- [KLSS Styling Guide](../docs/UI_ENGINE.md#klss-styling-system)

## 🎬 Live Demos

Run the example applications:

```bash
# Interactive UI demos (NEW!)
klang run examples/interactive_ui_server.kl
klang run examples/todo_app_interactive.kl
klang run examples/dashboard_interactive.kl
klang run examples/file_browser_interactive.kl

# Declarative UI demos
klang run examples/ui_dashboard_demo.kl

# Multi-view application
klang run examples/ui_multiview_demo.kl

# Markup & styling demo
klang run examples/ui_markup_demo.kl
```

## 🏗️ Architecture

```
┌─────────────────────────────────────┐
│         Your Application            │
├─────────────────────────────────────┤
│   KLUI Markup  │   KLSS Styles      │
├─────────────────────────────────────┤
│         Component System            │
│  (Panel, Button, List, Table, etc) │
├─────────────────────────────────────┤
│         Layout Engine               │
│    (Stack, Grid, Split, Tabs)      │
├─────────────────────────────────────┤
│      Rendering Engine               │
│   (ANSI codes, Terminal output)    │
├─────────────────────────────────────┤
│        Event System                 │
│   (Click, Keyboard, Custom)        │
└─────────────────────────────────────┘
```

## 💡 Why Use KLang UI Engine?

### For Web Developers
- Familiar HTML/CSS-like syntax
- Component-based architecture (like React)
- Declarative UI definition
- Easy to learn

### For CLI Tool Builders
- Build beautiful terminal UIs
- Interactive applications
- Professional dashboards
- User-friendly interfaces

### For KLang Developers
- Native to KLang
- No external dependencies
- Fast and efficient
- Extensible framework

## 🎯 Use Cases

- **Dashboards** - System monitoring, metrics visualization
- **CLI Tools** - Interactive command-line applications
- **Dev Tools** - Build tools, deployment systems
- **Task Managers** - Todo lists, project management
- **Data Viewers** - Log viewers, database tools
- **Forms** - Configuration wizards, data entry
- **Games** - Terminal-based games
- **Anything!** - If it runs in a terminal, you can build it

## 🛠️ Advanced Features

### Custom Components
```klang
class MyWidget extends Component {
    fn render() {
        return "Custom content"
    }
}
```

### State Management
```klang
component.setState({"count": 5})
let value = component.state["count"]
```

### Event Handling
```klang
component.on("click", fn(data) {
    println("Clicked!")
})
```

### Lifecycle Hooks
```klang
fn onCreate() { /* setup */ }
fn onMount() { /* first render */ }
fn onUpdate(prevState) { /* state changed */ }
fn onUnmount() { /* cleanup */ }
```

## 🎨 Theming

```klang
# Create theme
let theme = Theme("my-theme")
theme.setColor("primary", CYAN)
theme.setColor("success", GREEN)

# Apply
theme.apply(rootComponent)
```

## 🚧 Roadmap

- [x] Core engine and component system
- [x] KLUI markup language
- [x] KLSS styling system
- [x] Layout engine
- [x] Basic components
- [x] Documentation
- [x] Mouse input support
- [x] Keyboard navigation
- [x] Interactive components (buttons, inputs, etc.)
- [x] Event system
- [ ] Form validation
- [ ] Animations
- [ ] More components
- [ ] Component library
- [ ] CLI builder tool

## 🤝 Contributing

Want to contribute? We'd love:
- New components
- Layout improvements
- Styling enhancements
- Documentation
- Examples
- Bug fixes

## 📝 License

Part of the KLang project - MIT License

---

**Build terminal UIs as easily as building web apps!** 🚀

*Made with ❤️ by the KLang community*
