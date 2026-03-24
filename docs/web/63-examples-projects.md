# Project Examples

## Introduction

This guide presents complete, real-world projects you can build with KLang. Each project demonstrates multiple concepts working together to create practical applications.

## Project 1: CLI Todo Application

A feature-complete command-line todo list manager.

```klang
# todo_app.kl - Complete Todo Application

class Todo {
    static next_id = 1
    
    constructor(title, priority) {
        this.id = Todo.next_id
        Todo.next_id = Todo.next_id + 1
        this.title = title
        this.priority = priority or 2  # Default: medium
        this.completed = false
        this.created_at = "2024-01-15"  # Simplified
    }
    
    complete() {
        this.completed = true
    }
    
    priorityLabel() {
        if this.priority == 1 {
            return "HIGH"
        } else if this.priority == 2 {
            return "MEDIUM"
        }
        return "LOW"
    }
}

class TodoApp {
    constructor() {
        this.todos = []
        this.filename = "todos.json"
        this.load()
    }
    
    add(title, priority) {
        let todo = Todo(title, priority)
        this.todos.push(todo)
        this.save()
        println("✓ Added: " + title)
    }
    
    list(filter) {
        let filtered = this.todos
        
        if filter == "pending" {
            filtered = filtered.filter(t => !t.completed)
        } else if filter == "completed" {
            filtered = filtered.filter(t => t.completed)
        }
        
        if len(filtered) == 0 {
            println("No tasks found")
            return
        }
        
        println("\n" + uppercase(filter) + " TASKS:")
        println("-" .repeat(50))
        
        filtered.forEach(todo => {
            let checkbox = todo.completed ? "✓" : " "
            let priority = "[" + todo.priorityLabel() + "]"
            
            println("[" + checkbox + "] " + str(todo.id) + ". " + 
                    todo.title + " " + priority)
        })
        
        println("-" .repeat(50))
        println("Total: " + str(len(filtered)))
    }
    
    complete(id) {
        let todo = this.todos.find(t => t.id == id)
        if todo != null {
            todo.complete()
            this.save()
            println("✓ Completed: " + todo.title)
        } else {
            println("✗ Task not found")
        }
    }
    
    delete(id) {
        let original_length = len(this.todos)
        this.todos = this.todos.filter(t => t.id != id)
        
        if len(this.todos) < original_length {
            this.save()
            println("✓ Deleted task #" + str(id))
        } else {
            println("✗ Task not found")
        }
    }
    
    save() {
        let data = json_stringify(this.todos)
        write_file(this.filename, data)
    }
    
    load() {
        if file_exists(this.filename) {
            let data = read_file(this.filename)
            # In real implementation, parse JSON
            println("Loaded todos from " + this.filename)
        }
    }
    
    stats() {
        let total = len(this.todos)
        let completed = this.todos.filter(t => t.completed)
        let pending = total - len(completed)
        let high_priority = this.todos.filter(t => t.priority == 1)
        
        println("\n=== Statistics ===")
        println("Total tasks: " + str(total))
        println("Completed: " + str(len(completed)))
        println("Pending: " + str(pending))
        println("High priority: " + str(len(high_priority)))
    }
}

# CLI Interface
fn main() {
    let app = TodoApp()
    
    println("=== KLang Todo App ===")
    println("Commands: add, list, complete, delete, stats, exit")
    println("")
    
    # Example usage (in real app, read from stdin)
    app.add("Learn KLang basics", 1)
    app.add("Build a project", 1)
    app.add("Write documentation", 2)
    app.add("Deploy application", 2)
    
    app.list("all")
    app.complete(1)
    app.list("pending")
    app.stats()
}

main()
```

## Project 2: Weather Dashboard

Fetch and display weather data from an API.

```klang
# weather_app.kl - Weather Dashboard

class WeatherAPI {
    constructor(api_key) {
        this.api_key = api_key
        this.base_url = "https://api.openweathermap.org/data/2.5"
    }
    
    getCurrentWeather(city) {
        let url = this.base_url + "/weather?q=" + city + 
                  "&appid=" + this.api_key + "&units=metric"
        
        let response = fetch(url)
        return json_parse(response)
    }
    
    getForecast(city, days) {
        let url = this.base_url + "/forecast?q=" + city +
                  "&appid=" + this.api_key + "&units=metric&cnt=" + str(days * 8)
        
        let response = fetch(url)
        return json_parse(response)
    }
}

class WeatherDashboard {
    constructor(api_key) {
        this.api = WeatherAPI(api_key)
        this.cache = {}
    }
    
    displayCurrent(city) {
        println("\n=== Current Weather for " + city + " ===")
        
        let weather = this.api.getCurrentWeather(city)
        
        println("Temperature: " + str(weather.main.temp) + "°C")
        println("Feels like: " + str(weather.main.feels_like) + "°C")
        println("Humidity: " + str(weather.main.humidity) + "%")
        println("Description: " + weather.weather[0].description)
        println("Wind: " + str(weather.wind.speed) + " m/s")
    }
    
    displayForecast(city, days) {
        println("\n=== " + str(days) + "-Day Forecast for " + city + " ===")
        
        let forecast = this.api.getForecast(city, days)
        
        # Group by day
        let daily = {}
        forecast.list.forEach(item => {
            let date = item.dt_txt.split(" ")[0]
            if !(date in daily) {
                daily[date] = {
                    temps: [],
                    descriptions: []
                }
            }
            daily[date].temps.push(item.main.temp)
            daily[date].descriptions.push(item.weather[0].description)
        })
        
        # Display daily summary
        for date of daily {
            let day_data = daily[date]
            let avg_temp = day_data.temps.reduce((a, b) => a + b, 0) / len(day_data.temps)
            let main_desc = day_data.descriptions[0]
            
            println(date + ": " + str(round(avg_temp)) + "°C, " + main_desc)
        }
    }
    
    compareLocations(cities) {
        println("\n=== Weather Comparison ===")
        println("-" .repeat(60))
        
        cities.forEach(city => {
            let weather = this.api.getCurrentWeather(city)
            let temp = weather.main.temp
            let desc = weather.weather[0].description
            
            println(city.padEnd(15, " ") + ": " + 
                    str(temp).padEnd(5, " ") + "°C  " + desc)
        })
    }
}

fn main() {
    # You need an API key from openweathermap.org
    let api_key = env("OPENWEATHER_API_KEY")
    
    if api_key == null {
        println("Error: Set OPENWEATHER_API_KEY environment variable")
        return
    }
    
    let dashboard = WeatherDashboard(api_key)
    
    dashboard.displayCurrent("London")
    dashboard.displayForecast("London", 3)
    dashboard.compareLocations(["London", "Paris", "New York", "Tokyo"])
}

main()
```

## Project 3: Blog Engine

A simple static blog generator.

```klang
# blog_engine.kl - Static Blog Generator

class Post {
    constructor(title, content, author, tags) {
        this.id = this.generateId(title)
        this.title = title
        this.content = content
        this.author = author
        this.tags = tags or []
        this.date = "2024-01-15"  # Simplified
        this.slug = this.createSlug(title)
    }
    
    generateId(title) {
        # Simple hash of title
        return len(title) * 123  # Simplified
    }
    
    createSlug(title) {
        return lowercase(trim(title)).replaceAll(" ", "-")
    }
    
    preview(max_words) {
        let words = split(this.content, " ")
        if len(words) <= max_words {
            return this.content
        }
        
        let preview_words = words.slice(0, max_words)
        return join(preview_words, " ") + "..."
    }
}

class BlogEngine {
    constructor(blog_name, author) {
        this.blog_name = blog_name
        this.default_author = author
        this.posts = []
        this.output_dir = "blog_output"
    }
    
    addPost(title, content, tags) {
        let post = Post(title, content, this.default_author, tags)
        this.posts.push(post)
        println("✓ Added post: " + title)
        return post
    }
    
    getPostsByTag(tag) {
        return this.posts.filter(p => p.tags.includes(tag))
    }
    
    generateHTML(post) {
        let html = "<!DOCTYPE html>\n<html>\n<head>\n"
        html = html + "  <title>" + post.title + " - " + this.blog_name + "</title>\n"
        html = html + "</head>\n<body>\n"
        html = html + "  <h1>" + post.title + "</h1>\n"
        html = html + "  <p><em>By " + post.author + " on " + post.date + "</em></p>\n"
        html = html + "  <div>" + post.content + "</div>\n"
        
        if len(post.tags) > 0 {
            html = html + "  <p>Tags: " + join(post.tags, ", ") + "</p>\n"
        }
        
        html = html + "</body>\n</html>"
        return html
    }
    
    generateIndex() {
        let html = "<!DOCTYPE html>\n<html>\n<head>\n"
        html = html + "  <title>" + this.blog_name + "</title>\n"
        html = html + "</head>\n<body>\n"
        html = html + "  <h1>" + this.blog_name + "</h1>\n"
        
        this.posts.forEach(post => {
            html = html + "  <div>\n"
            html = html + "    <h2><a href='" + post.slug + ".html'>" + post.title + "</a></h2>\n"
            html = html + "    <p>" + post.preview(30) + "</p>\n"
            html = html + "  </div>\n"
        })
        
        html = html + "</body>\n</html>"
        return html
    }
    
    build() {
        println("\n=== Building Blog ===")
        
        # Generate post pages
        this.posts.forEach(post => {
            let html = this.generateHTML(post)
            let filename = this.output_dir + "/" + post.slug + ".html"
            write_file(filename, html)
            println("Generated: " + filename)
        })
        
        # Generate index
        let index_html = this.generateIndex()
        write_file(this.output_dir + "/index.html", index_html)
        println("Generated: index.html")
        
        println("\n✓ Blog built successfully!")
        println("Posts: " + str(len(this.posts)))
    }
}

fn main() {
    let blog = BlogEngine("My Tech Blog", "Alice")
    
    blog.addPost(
        "Getting Started with KLang",
        "KLang is an amazing AI-native programming language. In this post, we'll explore its features...",
        ["programming", "klang", "tutorial"]
    )
    
    blog.addPost(
        "Building Web Applications",
        "Learn how to build scalable web applications using modern techniques...",
        ["web", "programming", "tutorial"]
    )
    
    blog.addPost(
        "AI in Software Development",
        "Artificial Intelligence is transforming how we write code...",
        ["ai", "programming", "future"]
    )
    
    blog.build()
}

main()
```

## Project 4: AI-Powered Chatbot

An intelligent chatbot with conversation memory.

```klang
# chatbot.kl - AI Chatbot with Memory

class Chatbot {
    constructor(name, personality) {
        this.name = name
        this.session = ChatSession("openai", {
            apiKey: env("OPENAI_API_KEY"),
            model: "gpt-4",
            system: "You are " + name + ". " + personality
        })
        this.conversation_log = []
    }
    
    chat(user_message) {
        # Log user message
        this.conversation_log.push({
            role: "user",
            message: user_message,
            timestamp: "2024-01-15 10:00:00"
        })
        
        # Get AI response
        let response = this.session.send(user_message)
        
        # Log bot response
        this.conversation_log.push({
            role: "bot",
            message: response,
            timestamp: "2024-01-15 10:00:01"
        })
        
        return response
    }
    
    displayConversation() {
        println("\n=== Conversation History ===")
        this.conversation_log.forEach(entry => {
            let prefix = entry.role == "user" ? "You" : this.name
            println(prefix + ": " + entry.message)
        })
    }
    
    saveConversation(filename) {
        let data = json_stringify(this.conversation_log)
        write_file(filename, data)
        println("✓ Conversation saved to " + filename)
    }
    
    clearHistory() {
        this.session.clear()
        this.conversation_log = []
        println("✓ Conversation history cleared")
    }
    
    stats() {
        let total = len(this.conversation_log)
        let user_msgs = this.conversation_log.filter(e => e.role == "user")
        let bot_msgs = this.conversation_log.filter(e => e.role == "bot")
        
        println("\n=== Statistics ===")
        println("Total messages: " + str(total))
        println("Your messages: " + str(len(user_msgs)))
        println("Bot messages: " + str(len(bot_msgs)))
    }
}

class ChatbotApp {
    constructor() {
        this.bot = null
        this.running = false
    }
    
    start() {
        println("=== AI Chatbot ===")
        println("Commands: /exit, /clear, /save, /stats")
        println("")
        
        # Create bot
        this.bot = Chatbot("KBot", "You are a helpful and friendly assistant")
        println(this.bot.name + ": Hello! How can I help you today?")
        
        # Example conversation
        this.handleMessage("Hi! What is KLang?")
        this.handleMessage("Can you show me an example?")
        this.handleMessage("How do I install it?")
        
        this.bot.displayConversation()
        this.bot.stats()
    }
    
    handleMessage(message) {
        println("\nYou: " + message)
        
        # Check for commands
        if message == "/exit" {
            this.running = false
            return
        } else if message == "/clear" {
            this.bot.clearHistory()
            return
        } else if message.startsWith("/save") {
            this.bot.saveConversation("conversation.json")
            return
        } else if message == "/stats" {
            this.bot.stats()
            return
        }
        
        # Regular chat
        let response = this.bot.chat(message)
        println(this.bot.name + ": " + response)
    }
}

fn main() {
    let app = ChatbotApp()
    app.start()
}

main()
```

## Project 5: File Organizer

Automatically organize files by type or date.

```klang
# file_organizer.kl - Smart File Organizer

class FileOrganizer {
    constructor(source_dir) {
        this.source_dir = source_dir
        this.file_types = {
            "images": [".jpg", ".jpeg", ".png", ".gif", ".svg"],
            "documents": [".pdf", ".doc", ".docx", ".txt", ".md"],
            "code": [".js", ".py", ".kl", ".java", ".cpp"],
            "archives": [".zip", ".tar", ".gz", ".rar"],
            "videos": [".mp4", ".avi", ".mkv", ".mov"]
        }
        this.stats = {
            moved: 0,
            skipped: 0,
            errors: 0
        }
    }
    
    getFileCategory(filename) {
        let ext = this.getExtension(filename)
        
        for category of this.file_types {
            if this.file_types[category].includes(ext) {
                return category
            }
        }
        
        return "other"
    }
    
    getExtension(filename) {
        let parts = split(filename, ".")
        if len(parts) > 1 {
            return "." + parts[len(parts) - 1]
        }
        return ""
    }
    
    organize() {
        println("=== File Organizer ===")
        println("Source: " + this.source_dir)
        println("")
        
        # In real implementation, list directory files
        let files = [
            "report.pdf",
            "photo.jpg",
            "script.py",
            "video.mp4",
            "document.txt"
        ]
        
        files.forEach(file => {
            this.processFile(file)
        })
        
        this.displayStats()
    }
    
    processFile(filename) {
        let category = this.getFileCategory(filename)
        let target_dir = this.source_dir + "/" + category
        
        println("Moving " + filename + " to " + category + "/")
        
        # In real implementation:
        # 1. Create target directory if needed
        # 2. Move file
        # 3. Handle errors
        
        this.stats.moved = this.stats.moved + 1
    }
    
    displayStats() {
        println("\n=== Statistics ===")
        println("Files moved: " + str(this.stats.moved))
        println("Files skipped: " + str(this.stats.skipped))
        println("Errors: " + str(this.stats.errors))
    }
}

fn main() {
    let organizer = FileOrganizer("./downloads")
    organizer.organize()
}

main()
```

## Project 6: API Client Library

A reusable HTTP client for REST APIs.

```klang
# api_client.kl - REST API Client Library

class APIClient {
    constructor(base_url, auth_token) {
        this.base_url = base_url
        this.auth_token = auth_token
        this.default_headers = {
            "Content-Type": "application/json"
        }
        
        if auth_token != null {
            this.default_headers["Authorization"] = "Bearer " + auth_token
        }
    }
    
    get(endpoint, params) {
        let url = this.buildURL(endpoint, params)
        return this.request("GET", url)
    }
    
    post(endpoint, data) {
        let url = this.buildURL(endpoint, null)
        return this.request("POST", url, data)
    }
    
    put(endpoint, data) {
        let url = this.buildURL(endpoint, null)
        return this.request("PUT", url, data)
    }
    
    delete(endpoint) {
        let url = this.buildURL(endpoint, null)
        return this.request("DELETE", url)
    }
    
    buildURL(endpoint, params) {
        let url = this.base_url + endpoint
        
        if params != null and len(params) > 0 {
            let query = []
            for key of params {
                query.push(key + "=" + params[key])
            }
            url = url + "?" + join(query, "&")
        }
        
        return url
    }
    
    request(method, url, data) {
        println(method + " " + url)
        
        let options = {
            method: method,
            headers: this.default_headers
        }
        
        if data != null {
            options.body = json_stringify(data)
        }
        
        let response = fetch(url, options)
        return json_parse(response)
    }
}

# Example: GitHub API Client
class GitHubClient extends APIClient {
    constructor(token) {
        super("https://api.github.com", token)
    }
    
    getUser(username) {
        return this.get("/users/" + username, null)
    }
    
    getRepos(username) {
        return this.get("/users/" + username + "/repos", null)
    }
    
    createIssue(owner, repo, title, body) {
        return this.post("/repos/" + owner + "/" + repo + "/issues", {
            title: title,
            body: body
        })
    }
}

fn main() {
    let github = GitHubClient(env("GITHUB_TOKEN"))
    
    # Get user info
    let user = github.getUser("octocat")
    println("User: " + user.name)
    println("Followers: " + str(user.followers))
    
    # Get repositories
    let repos = github.getRepos("octocat")
    println("\nRepositories:")
    repos.forEach(repo => {
        println("- " + repo.name + " ⭐ " + str(repo.stargazers_count))
    })
}

main()
```

## Project 7: Data Analyzer

Analyze and visualize data from CSV files.

```klang
# data_analyzer.kl - CSV Data Analyzer

class DataAnalyzer {
    constructor() {
        this.data = []
        this.headers = []
    }
    
    loadCSV(filename) {
        let content = read_file(filename)
        let lines = split(content, "\n")
        
        if len(lines) == 0 {
            println("Error: Empty file")
            return
        }
        
        # Parse headers
        this.headers = split(lines[0], ",")
        
        # Parse data rows
        for i in range(1, len(lines)) {
            let line = trim(lines[i])
            if len(line) > 0 {
                let values = split(line, ",")
                let row = {}
                
                for j in range(len(this.headers)) {
                    row[this.headers[j]] = values[j]
                }
                
                this.data.push(row)
            }
        }
        
        println("✓ Loaded " + str(len(this.data)) + " rows")
    }
    
    summary() {
        println("\n=== Data Summary ===")
        println("Rows: " + str(len(this.data)))
        println("Columns: " + str(len(this.headers)))
        println("Headers: " + join(this.headers, ", "))
    }
    
    getColumn(column_name) {
        return this.data.map(row => row[column_name])
    }
    
    avg(column_name) {
        let values = this.getColumn(column_name)
        let numbers = values.map(v => float(v))
        let sum = numbers.reduce((a, b) => a + b, 0)
        return sum / len(numbers)
    }
    
    max(column_name) {
        let values = this.getColumn(column_name)
        let numbers = values.map(v => float(v))
        return numbers.reduce((a, b) => max(a, b))
    }
    
    min(column_name) {
        let values = this.getColumn(column_name)
        let numbers = values.map(v => float(v))
        return numbers.reduce((a, b) => min(a, b))
    }
    
    filter(predicate) {
        let filtered = DataAnalyzer()
        filtered.headers = this.headers
        filtered.data = this.data.filter(predicate)
        return filtered
    }
    
    groupBy(column) {
        let groups = {}
        
        this.data.forEach(row => {
            let key = row[column]
            if !(key in groups) {
                groups[key] = []
            }
            groups[key].push(row)
        })
        
        return groups
    }
    
    display(limit) {
        let count = min(limit, len(this.data))
        
        println("\n=== Data (" + str(count) + " rows) ===")
        println(join(this.headers, " | "))
        println("-" .repeat(60))
        
        for i in range(count) {
            let row = this.data[i]
            let values = this.headers.map(h => row[h])
            println(join(values, " | "))
        }
    }
}

fn main() {
    # Example: Analyze sales data
    let analyzer = DataAnalyzer()
    
    # In real usage: analyzer.loadCSV("sales.csv")
    # For demo, simulate data
    analyzer.headers = ["product", "quantity", "price"]
    analyzer.data = [
        {product: "Laptop", quantity: "10", price: "999"},
        {product: "Mouse", quantity: "50", price: "25"},
        {product: "Keyboard", quantity: "30", price: "75"}
    ]
    
    analyzer.summary()
    analyzer.display(10)
    
    println("\nAnalysis:")
    println("Average price: $" + str(analyzer.avg("price")))
    println("Max quantity: " + str(analyzer.max("quantity")))
    
    # Filter expensive products
    let expensive = analyzer.filter(row => float(row.price) > 50)
    println("\nExpensive products: " + str(len(expensive.data)))
}

main()
```

## Project 8: AI-Powered Code Assistant

Generate and explain code using AI.

```klang
# code_assistant.kl - AI Code Assistant

class CodeAssistant {
    constructor() {
        this.client = LLMClient("openai", {
            apiKey: env("OPENAI_API_KEY"),
            model: "gpt-4",
            temperature: 0.2
        })
        this.session = ChatSession("openai", {
            apiKey: env("OPENAI_API_KEY"),
            model: "gpt-4",
            system: "You are an expert programmer. Be concise and practical."
        })
    }
    
    generate(description, language) {
        let prompt = "Generate " + language + " code for:\n\n" + description +
                     "\n\nProvide only code, no explanation."
        
        return this.client.chat(prompt)
    }
    
    explain(code, language) {
        let prompt = "Explain this " + language + " code:\n\n```\n" + 
                     code + "\n```"
        
        return this.client.chat(prompt)
    }
    
    improve(code, language) {
        let prompt = "Improve this " + language + " code:\n\n```\n" +
                     code + "\n```\n\nSuggest improvements for readability, " +
                     "performance, and best practices."
        
        return this.client.chat(prompt)
    }
    
    debug(code, error, language) {
        let prompt = "This " + language + " code has an error:\n\n```\n" +
                     code + "\n```\n\nError: " + error +
                     "\n\nExplain the problem and provide a fix."
        
        return this.client.chat(prompt)
    }
    
    convert(code, from_lang, to_lang) {
        let prompt = "Convert this " + from_lang + " code to " + to_lang + ":\n\n```\n" +
                     code + "\n```"
        
        return this.client.chat(prompt)
    }
    
    chat(message) {
        return this.session.send(message)
    }
}

fn main() {
    let assistant = CodeAssistant()
    
    # Generate code
    println("=== Code Generation ===")
    let code = assistant.generate("a function to calculate fibonacci numbers", "KLang")
    println(code)
    
    # Explain code
    println("\n=== Code Explanation ===")
    let explanation = assistant.explain(code, "KLang")
    println(explanation)
    
    # Improve code
    println("\n=== Code Improvement ===")
    let suggestions = assistant.improve(code, "KLang")
    println(suggestions)
    
    # Interactive chat
    println("\n=== Interactive Help ===")
    let help1 = assistant.chat("How do I handle errors in KLang?")
    println("Q: How do I handle errors?")
    println("A: " + help1)
    
    let help2 = assistant.chat("Show me an example")
    println("\nQ: Show me an example")
    println("A: " + help2)
}

main()
```

## Running the Projects

Each project is self-contained. To run:

```bash
# Todo App
klang run todo_app.kl

# Weather Dashboard (requires API key)
export OPENWEATHER_API_KEY="your_key"
klang run weather_app.kl

# Blog Engine
klang run blog_engine.kl

# AI Chatbot (requires API key)
export OPENAI_API_KEY="your_key"
klang run chatbot.kl

# File Organizer
klang run file_organizer.kl

# API Client
klang run api_client.kl

# Data Analyzer
klang run data_analyzer.kl

# Code Assistant (requires API key)
export OPENAI_API_KEY="your_key"
klang run code_assistant.kl
```

## Key Concepts Used

Each project demonstrates:
- **Todo App**: Classes, file I/O, data persistence
- **Weather Dashboard**: HTTP requests, JSON parsing, data display
- **Blog Engine**: File generation, templates, data transformation
- **AI Chatbot**: Chat sessions, conversation memory, AI integration
- **File Organizer**: File operations, categorization, statistics
- **API Client**: HTTP methods, inheritance, reusable libraries
- **Data Analyzer**: CSV parsing, data aggregation, filtering
- **Code Assistant**: AI code generation, multi-turn conversations

## Next Steps

- Extend these projects with additional features
- Combine multiple projects into larger systems
- Add error handling and validation
- Implement persistent storage
- Create web interfaces
- Deploy to production

## Resources

- [Basic Examples](60-examples-basic.md) - Fundamental concepts
- [OOP Examples](61-examples-oop.md) - Object-oriented patterns
- [AI Examples](62-examples-ai.md) - AI-powered applications
- [Full Documentation](../README.md) - Complete guides

---

**Build amazing projects with KLang!** 🚀
