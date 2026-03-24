# AI Agents - Autonomous Intelligent Systems

AI agents are autonomous systems that can perceive their environment, make decisions, and take actions to achieve goals. KLang provides built-in support for creating sophisticated agents with memory, tool use, and multi-step reasoning capabilities.

## Table of Contents

1. [What are AI Agents](#what-are-ai-agents)
2. [Agent Architecture](#agent-architecture)
3. [Agent Memory](#agent-memory)
4. [Agent Actions](#agent-actions)
5. [Tool Use](#tool-use)
6. [ReAct Pattern](#react-pattern)
7. [Multi-Step Reasoning](#multi-step-reasoning)
8. [Agent Types](#agent-types)
9. [Multi-Agent Systems](#multi-agent-systems)
10. [Production Agents](#production-agents)
11. [Best Practices](#best-practices)

---

## What are AI Agents

### Traditional vs Agent-Based

```klang
// Traditional: Direct LLM call
let response = client.chat("What's the weather in Paris?")
// Response: "I don't have real-time weather data"

// Agent-Based: Agent uses tools
let agent = new Agent(client)
agent.addTool(weatherTool)

let response = agent.run("What's the weather in Paris?")
// Agent thinks: "I need weather data"
// Agent acts: Calls weatherTool("Paris")
// Response: "The weather in Paris is 72°F and sunny"
```

### Agent Characteristics

```klang
// 1. Autonomy - Makes own decisions
// 2. Reactivity - Responds to environment
// 3. Proactivity - Takes initiative
// 4. Social Ability - Interacts with other agents
// 5. Memory - Remembers past interactions
// 6. Learning - Improves over time
```

### When to Use Agents

```klang
// ✅ Use agents when:
// - Need to use external tools/APIs
// - Require multi-step reasoning
// - Must maintain conversation context
// - Need to handle dynamic situations
// - Want autonomous task completion

// ❌ Don't use agents for:
// - Simple Q&A (use direct LLM)
// - Deterministic workflows (use scripts)
// - Real-time responses (agents can be slow)
```

---

## Agent Architecture

### Basic Agent Structure

```klang
class BasicAgent {
    constructor(client: OpenAIClient) {
        this.client = client
        this.memory = new AgentMemory(50)
        this.tools = []
        this.maxIterations = 10
    }
    
    fn addTool(tool: AgentAction) {
        this.tools.push(tool)
    }
    
    fn think(situation: string) -> string {
        let prompt = "Given situation: " + situation + "\n"
        prompt = prompt + "Available tools: " + this.getToolList() + "\n"
        prompt = prompt + "What should I do next?"
        
        return this.client.chat(prompt, "gpt-4")
    }
    
    fn act(action: string, args: string) -> string {
        let i = 0
        while i < len(this.tools) {
            if this.tools[i].name == action {
                return this.tools[i].execute(args)
            }
            i = i + 1
        }
        return "Unknown action: " + action
    }
    
    fn run(task: string) -> string {
        let iteration = 0
        let result = ""
        
        while iteration < this.maxIterations {
            let thought = this.think(task)
            
            if contains(thought, "DONE") {
                break
            }
            
            // Extract action and args from thought
            let action = this.parseAction(thought)
            let args = this.parseArgs(thought)
            
            result = this.act(action, args)
            this.memory.remember("step_" + str(iteration), result)
            
            iteration = iteration + 1
        }
        
        return result
    }
    
    fn getToolList() -> string {
        let list = ""
        let i = 0
        while i < len(this.tools) {
            list = list + this.tools[i].name + ", "
            i = i + 1
        }
        return list
    }
}
```

### ReAct Agent (Reasoning + Acting)

```klang
class ReActAgent {
    constructor(client: OpenAIClient) {
        this.client = client
        this.memory = new AgentMemory(100)
        this.tools = dict()
        this.maxSteps = 10
        this.verbose = true
    }
    
    fn addTool(name: string, description: string, func: function) {
        let tool = new AgentAction(name, description, func)
        this.tools.set(name, tool)
    }
    
    fn run(task: string) -> string {
        if this.verbose {
            println("🎯 Task: " + task)
            println()
        }
        
        let step = 1
        let context = task
        
        while step <= this.maxSteps {
            if this.verbose {
                println("Step " + str(step) + ":")
            }
            
            // Thought
            let thought = this.generateThought(context)
            if this.verbose {
                println("  💭 Thought: " + thought)
            }
            
            // Check if done
            if contains(thought, "DONE") || contains(thought, "FINAL ANSWER") {
                let answer = this.extractFinalAnswer(thought)
                if this.verbose {
                    println("\n✅ Final Answer: " + answer)
                }
                return answer
            }
            
            // Action
            let action = this.parseAction(thought)
            let actionInput = this.parseActionInput(thought)
            
            if this.verbose {
                println("  🔧 Action: " + action + "(" + actionInput + ")")
            }
            
            // Execute
            let observation = this.executeAction(action, actionInput)
            if this.verbose {
                println("  👁️ Observation: " + observation)
                println()
            }
            
            // Update context
            context = context + "\nStep " + str(step) + ": " + thought
            context = context + "\nObservation: " + observation
            
            // Remember
            this.memory.remember("step_" + str(step), observation)
            
            step = step + 1
        }
        
        return "Max steps reached without completion"
    }
    
    fn generateThought(context: string) -> string {
        let prompt = """
        You are an AI agent. Think step by step.
        
        Available tools:
        """ + this.getToolDescriptions() + """
        
        Current situation:
        """ + context + """
        
        Think about what to do next. Format your response as:
        Thought: [your reasoning]
        Action: [tool name]
        Action Input: [input for tool]
        
        Or if done:
        Thought: [final reasoning]
        FINAL ANSWER: [your answer]
        """
        
        return this.client.chat(prompt, "gpt-4")
    }
    
    fn executeAction(action: string, input: string) -> string {
        if this.tools.has(action) {
            let tool = this.tools.get(action)
            return tool.execute(input)
        }
        return "Error: Unknown action '" + action + "'"
    }
    
    fn getToolDescriptions() -> string {
        let desc = ""
        let toolNames = this.tools.keys()
        
        let i = 0
        while i < len(toolNames) {
            let tool = this.tools.get(toolNames[i])
            desc = desc + "- " + tool.name + ": " + tool.description + "\n"
            i = i + 1
        }
        
        return desc
    }
}
```

### Using the ReAct Agent

```klang
let apiKey = env.get("OPENAI_API_KEY")
let client = new OpenAIClient(apiKey)
let agent = new ReActAgent(client)

// Define tools
fn searchWeb(query: string) -> string {
    return "Found information about " + query
}

fn calculate(expression: string) -> string {
    // Simple calculator (implement actual calculation)
    return "Result: 42"
}

fn getCurrentTime(args: string) -> string {
    return "Current time: 2024-01-15 14:30:00"
}

// Add tools to agent
agent.addTool("search", "Search the web for information", searchWeb)
agent.addTool("calculate", "Perform calculations", calculate)
agent.addTool("get_time", "Get current time", getCurrentTime)

// Run task
let result = agent.run("What time is it and what is 6 * 7?")
println(result)

// Output:
// Step 1:
//   💭 Thought: I need to get the current time first
//   🔧 Action: get_time()
//   👁️ Observation: Current time: 2024-01-15 14:30:00
//
// Step 2:
//   💭 Thought: Now I need to calculate 6 * 7
//   🔧 Action: calculate(6 * 7)
//   👁️ Observation: Result: 42
//
// ✅ Final Answer: It's 14:30:00 and 6 * 7 equals 42
```

---

## Agent Memory

### Short-Term Memory

```klang
let memory = new AgentMemory(maxSize: 50)

// Store facts
memory.storeFact("user_name", "Alice")
memory.storeFact("user_language", "KLang")
memory.storeFact("user_level", "beginner")

// Remember events
memory.remember("event_1", "User asked about variables")
memory.remember("event_2", "User asked about functions")
memory.remember("event_3", "User asked about classes")

// Retrieve
let name = memory.retrieveFact("user_name")
println("User name: " + name)  // "Alice"

// Get recent memories
let recent = memory.getRecentMemories(3)
let i = 0
while i < len(recent) {
    println(recent[i].key + ": " + recent[i].value)
    i = i + 1
}
```

### Long-Term Memory (Persistent)

```klang
class PersistentMemory {
    constructor(storagePath: string) {
        this.path = storagePath
        this.memory = dict()
        this.load()
    }
    
    fn store(key: string, value: any) {
        this.memory.set(key, value)
        this.save()
    }
    
    fn retrieve(key: string) -> any {
        return this.memory.get(key)
    }
    
    fn save() {
        let data = JSON.stringify(this.memory)
        fwrite(this.path, data)
    }
    
    fn load() {
        if fexists(this.path) {
            let data = fread(this.path)
            this.memory = JSON.parse(data)
        }
    }
    
    fn clear() {
        this.memory = dict()
        this.save()
    }
}

let memory = new PersistentMemory("agent_memory.json")
memory.store("last_interaction", timestamp())
memory.store("total_queries", 42)
```

### Semantic Memory (Vector-Based)

```klang
class SemanticMemory {
    constructor(client: OpenAIClient) {
        this.client = client
        this.store = new EmbeddingStore()
    }
    
    fn remember(text: string, metadata: dict) {
        let embedding = this.client.embed(text)
        this.store.addText(text, embedding, metadata)
    }
    
    fn recall(query: string, k: int) -> SearchResult[] {
        let queryEmb = this.client.embed(query)
        return this.store.search(queryEmb, k)
    }
    
    fn relatedMemories(memory: string, k: int) -> SearchResult[] {
        return this.recall(memory, k)
    }
}

let memory = new SemanticMemory(client)

// Store memories
memory.remember("User prefers detailed explanations", dict())
memory.remember("User is learning about AI", dict())
memory.remember("User asked about neural networks", dict())

// Recall relevant memories
let relevant = memory.recall("Tell me about machine learning", 2)
let i = 0
while i < len(relevant) {
    println("Relevant: " + relevant[i].text)
    i = i + 1
}
```

### Episodic Memory (Event History)

```klang
class EpisodicMemory {
    constructor(maxSize: int) {
        this.episodes = []
        this.maxSize = maxSize
    }
    
    fn addEpisode(event: string, context: string, outcome: string) {
        let episode = dict()
        episode.set("event", event)
        episode.set("context", context)
        episode.set("outcome", outcome)
        episode.set("timestamp", timestamp())
        
        this.episodes.push(episode)
        
        // Keep only recent episodes
        if len(this.episodes) > this.maxSize {
            this.episodes = this.episodes[1:this.maxSize + 1]
        }
    }
    
    fn getRecentEpisodes(n: int) -> dict[] {
        let start = max(0, len(this.episodes) - n)
        return this.episodes[start:len(this.episodes)]
    }
    
    fn findSimilarEpisodes(query: string) -> dict[] {
        let similar = []
        
        let i = 0
        while i < len(this.episodes) {
            if contains(this.episodes[i].get("event"), query) {
                similar.push(this.episodes[i])
            }
            i = i + 1
        }
        
        return similar
    }
}

let episodic = new EpisodicMemory(100)

episodic.addEpisode(
    "User asked about variables",
    "Learning basics",
    "Explained with examples"
)

episodic.addEpisode(
    "User made a syntax error",
    "Writing first function",
    "Corrected and explained"
)

let recent = episodic.getRecentEpisodes(5)
```

---

## Agent Actions

### Defining Actions

```klang
// Simple action
fn searchAction(query: string) -> string {
    println("🔍 Searching for: " + query)
    return "Found information about " + query
}

let search = new AgentAction(
    "search",
    "Search for information on the web",
    searchAction
)

// Execute
let result = search.execute("KLang programming")
println(result)
```

### Built-in Actions

```klang
class StandardActions {
    static fn search(query: string) -> string {
        // Web search implementation
        return "Search results for: " + query
    }
    
    static fn calculate(expression: string) -> string {
        // Calculator implementation
        return "Calculation result"
    }
    
    static fn readFile(path: string) -> string {
        if fexists(path) {
            return fread(path)
        }
        return "File not found: " + path
    }
    
    static fn writeFile(path: string, content: string) -> string {
        fwrite(path, content)
        return "Written to " + path
    }
    
    static fn getCurrentTime(args: string) -> string {
        return "Current time: " + str(timestamp())
    }
    
    static fn httpGet(url: string) -> string {
        return http.get(url)
    }
}
```

### Action with Validation

```klang
class ValidatedAction {
    constructor(name: string, description: string, func: function) {
        this.name = name
        this.description = description
        this.func = func
        this.callCount = 0
        this.successCount = 0
        this.failCount = 0
    }
    
    fn execute(args: string) -> string {
        this.callCount = this.callCount + 1
        
        try {
            // Validate args
            if args == null || len(trim(args)) == 0 {
                this.failCount = this.failCount + 1
                return "Error: Empty arguments"
            }
            
            // Execute
            let result = this.func(args)
            this.successCount = this.successCount + 1
            
            return result
            
        } catch error {
            this.failCount = this.failCount + 1
            return "Error executing action: " + error
        }
    }
    
    fn getStats() -> dict {
        let stats = dict()
        stats.set("calls", this.callCount)
        stats.set("success", this.successCount)
        stats.set("failures", this.failCount)
        stats.set("success_rate", this.successCount / this.callCount)
        return stats
    }
}
```

### Async Actions (Future)

```klang
// When KLang adds async support
class AsyncAction {
    constructor(name: string, func: function) {
        this.name = name
        this.func = func
    }
    
    async fn execute(args: string) -> string {
        let result = await this.func(args)
        return result
    }
}
```

---

## Tool Use

### Creating Tools

```klang
class Tool {
    constructor(name: string, description: string, parameters: dict, func: function) {
        this.name = name
        this.description = description
        this.parameters = parameters
        this.func = func
    }
    
    fn getSchema() -> dict {
        let schema = dict()
        schema.set("name", this.name)
        schema.set("description", this.description)
        schema.set("parameters", this.parameters)
        return schema
    }
    
    fn call(args: dict) -> any {
        return this.func(args)
    }
}

// Define a tool
let params = dict()
params.set("type", "object")

let properties = dict()
let location = dict()
location.set("type", "string")
location.set("description", "City name")
properties.set("location", location)

params.set("properties", properties)
params.set("required", ["location"])

fn getWeather(args: dict) -> string {
    let location = args.get("location")
    return "Weather in " + location + ": 72°F, sunny"
}

let weatherTool = new Tool(
    "get_weather",
    "Get current weather for a location",
    params,
    getWeather
)
```

### Tool Registry

```klang
class ToolRegistry {
    constructor() {
        this.tools = dict()
    }
    
    fn register(tool: Tool) {
        this.tools.set(tool.name, tool)
        println("✓ Registered tool: " + tool.name)
    }
    
    fn get(name: string) -> Tool {
        return this.tools.get(name)
    }
    
    fn list() -> string[] {
        return this.tools.keys()
    }
    
    fn getSchemas() -> dict[] {
        let schemas = []
        let names = this.tools.keys()
        
        let i = 0
        while i < len(names) {
            let tool = this.tools.get(names[i])
            schemas.push(tool.getSchema())
            i = i + 1
        }
        
        return schemas
    }
}

let registry = new ToolRegistry()
registry.register(weatherTool)
registry.register(calculatorTool)
registry.register(searchTool)

let toolList = registry.list()
println("Available tools: " + str(toolList))
```

### Function Calling with OpenAI

```klang
let client = new OpenAIClient(apiKey)

// Define function
let weatherFunc = dict()
weatherFunc.set("name", "get_weather")
weatherFunc.set("description", "Get weather for a location")

let parameters = dict()
parameters.set("type", "object")

let properties = dict()
let locationProp = dict()
locationProp.set("type", "string")
properties.set("location", locationProp)

parameters.set("properties", properties)
parameters.set("required", ["location"])

weatherFunc.set("parameters", parameters)

// Add to client
client.addFunction(weatherFunc)

// Query with function calling
let response = client.chat("What's the weather in Tokyo?", "gpt-3.5-turbo")

// Response will include function call
// Parse and execute the function
// Return result to LLM
```

---

## ReAct Pattern

### ReAct Implementation

```klang
fn reactLoop(task: string, agent: ReActAgent, maxSteps: int) -> string {
    let step = 1
    let context = "Task: " + task
    
    while step <= maxSteps {
        println("\n--- Step " + str(step) + " ---")
        
        // Thought: Agent reasons about what to do
        println("Thinking...")
        let thought = agent.think(context)
        println("Thought: " + thought)
        
        // Check if done
        if contains(thought, "DONE") {
            let answer = extractAnswer(thought)
            println("\n✅ Task complete!")
            return answer
        }
        
        // Action: Agent decides on action
        let action = extractAction(thought)
        let actionInput = extractActionInput(thought)
        println("Action: " + action + "(" + actionInput + ")")
        
        // Observation: Execute action and observe result
        let observation = agent.executeAction(action, actionInput)
        println("Observation: " + observation)
        
        // Update context with new information
        context = context + "\n\nStep " + str(step) + ":"
        context = context + "\nThought: " + thought
        context = context + "\nObservation: " + observation
        
        step = step + 1
    }
    
    return "Failed to complete task in " + str(maxSteps) + " steps"
}
```

### ReAct Example

```klang
let client = new OpenAIClient(apiKey)
let agent = new ReActAgent(client)

// Add tools
agent.addTool("search", "Search the web", searchTool)
agent.addTool("calculator", "Do math", calculatorTool)
agent.addTool("get_weather", "Get weather", weatherTool)

// Complex task
let task = """
I need to know:
1. What's the weather in Paris?
2. What's 15% tip on a €50 meal?
3. Find information about the Eiffel Tower
"""

let result = agent.run(task)

// Agent will:
// Step 1: Search for weather in Paris
// Step 2: Calculate 15% of 50
// Step 3: Search for Eiffel Tower info
// Step 4: Synthesize answer
```

---

## Multi-Step Reasoning

### Chain of Thought Agent

```klang
class ChainOfThoughtAgent {
    constructor(client: OpenAIClient) {
        this.client = client
    }
    
    fn solve(problem: string) -> string {
        let prompt = """
        Solve this problem step by step:
        
        Problem: """ + problem + """
        
        Think through each step:
        Step 1: [first step]
        Step 2: [second step]
        ...
        
        Final Answer: [your answer]
        """
        
        return this.client.chat(prompt, "gpt-4")
    }
}

let agent = new ChainOfThoughtAgent(client)

let problem = "If I have 5 apples and buy 3 more, then give 2 away, how many do I have?"
let solution = agent.solve(problem)
println(solution)

// Output:
// Step 1: Start with 5 apples
// Step 2: Buy 3 more: 5 + 3 = 8 apples
// Step 3: Give away 2: 8 - 2 = 6 apples
// Final Answer: 6 apples
```

### Tree of Thoughts

```klang
class TreeOfThoughtsAgent {
    constructor(client: OpenAIClient) {
        this.client = client
        this.branches = 3  // Try 3 approaches
    }
    
    fn solve(problem: string) -> string {
        println("Exploring multiple solution paths...")
        
        let approaches = []
        let i = 0
        while i < this.branches {
            let approach = this.exploreApproach(problem, i + 1)
            approaches.push(approach)
            println("Approach " + str(i + 1) + " evaluated")
            i = i + 1
        }
        
        // Evaluate and choose best
        let best = this.selectBestApproach(approaches)
        return best
    }
    
    fn exploreApproach(problem: string, branchNum: int) -> string {
        let prompt = """
        Solve this problem (approach #""" + str(branchNum) + """):
        
        Problem: """ + problem + """
        
        Try a different strategy for each approach.
        """
        
        return this.client.chat(prompt, "gpt-4")
    }
    
    fn selectBestApproach(approaches: string[]) -> string {
        let prompt = """
        Given these different solutions to the same problem:
        
        """
        
        let i = 0
        while i < len(approaches) {
            prompt = prompt + "\nApproach " + str(i + 1) + ":\n"
            prompt = prompt + approaches[i] + "\n"
            i = i + 1
        }
        
        prompt = prompt + "\nWhich approach is best? Explain why and provide the final answer."
        
        return this.client.chat(prompt, "gpt-4")
    }
}
```

### Iterative Refinement

```klang
class RefinementAgent {
    constructor(client: OpenAIClient) {
        this.client = client
        this.maxIterations = 5
    }
    
    fn refine(task: string) -> string {
        let result = this.client.chat(task, "gpt-4")
        
        let iteration = 1
        while iteration < this.maxIterations {
            println("Iteration " + str(iteration) + "...")
            
            // Critique current result
            let critique = this.critique(result)
            
            if contains(critique, "GOOD ENOUGH") {
                println("Result satisfactory after " + str(iteration) + " iterations")
                break
            }
            
            // Refine based on critique
            result = this.improve(task, result, critique)
            
            iteration = iteration + 1
        }
        
        return result
    }
    
    fn critique(result: string) -> string {
        let prompt = """
        Critique this result:
        
        """ + result + """
        
        What could be improved? If it's good, say "GOOD ENOUGH".
        """
        
        return this.client.chat(prompt, "gpt-4")
    }
    
    fn improve(task: string, currentResult: string, critique: string) -> string {
        let prompt = """
        Original task: """ + task + """
        
        Current result: """ + currentResult + """
        
        Critique: """ + critique + """
        
        Provide an improved version.
        """
        
        return this.client.chat(prompt, "gpt-4")
    }
}
```

---

## Agent Types

### Task Completion Agent

```klang
class TaskAgent {
    constructor(client: OpenAIClient, tools: Tool[]) {
        this.client = client
        this.tools = tools
        this.memory = new AgentMemory(50)
    }
    
    fn execute(task: string) -> dict {
        let result = dict()
        result.set("task", task)
        result.set("status", "in_progress")
        result.set("steps", [])
        
        let completed = false
        let maxSteps = 10
        let step = 0
        
        while !completed && step < maxSteps {
            let thought = this.think(task, result.get("steps"))
            let action = this.parseAction(thought)
            
            if action == "COMPLETE" {
                completed = true
                result.set("status", "completed")
                result.set("output", this.extractOutput(thought))
            } else {
                let observation = this.executeAction(action)
                result.get("steps").push({
                    "thought": thought,
                    "action": action,
                    "observation": observation
                })
            }
            
            step = step + 1
        }
        
        if !completed {
            result.set("status", "failed")
            result.set("reason", "Max steps exceeded")
        }
        
        return result
    }
}
```

### Research Agent

```klang
class ResearchAgent {
    constructor(client: OpenAIClient, knowledgeBase: RAGSystem) {
        this.client = client
        this.kb = knowledgeBase
        this.findings = []
    }
    
    fn research(topic: string, depth: int) -> string {
        println("🔬 Researching: " + topic)
        
        // Generate research questions
        let questions = this.generateQuestions(topic, depth)
        
        // Answer each question
        let answers = []
        let i = 0
        while i < len(questions) {
            println("  ❓ " + questions[i])
            
            let answer = this.kb.query(questions[i], 5)
            answers.push(answer)
            
            println("  ✓ Answered")
            i = i + 1
        }
        
        // Synthesize findings
        let report = this.synthesize(topic, questions, answers)
        
        return report
    }
    
    fn generateQuestions(topic: string, depth: int) -> string[] {
        let prompt = """
        Generate """ + str(depth) + """ research questions about: """ + topic + """
        
        Questions should cover different aspects and go from basic to advanced.
        """
        
        let response = this.client.chat(prompt, "gpt-4")
        return split(response, "\n")
    }
    
    fn synthesize(topic: string, questions: string[], answers: string[]) -> string {
        let prompt = "Synthesize a comprehensive report on: " + topic + "\n\n"
        
        let i = 0
        while i < len(questions) {
            prompt = prompt + "Q" + str(i + 1) + ": " + questions[i] + "\n"
            prompt = prompt + "A: " + answers[i] + "\n\n"
            i = i + 1
        }
        
        prompt = prompt + "Create a well-structured report."
        
        return this.client.chat(prompt, "gpt-4")
    }
}
```

### Conversational Agent

```klang
class ConversationalAgent {
    constructor(client: OpenAIClient) {
        this.client = client
        this.session = new ChatSession("You are a helpful assistant")
        this.personality = "friendly and knowledgeable"
    }
    
    fn chat(message: string) -> string {
        // Add user message
        this.session.addUserMessage(message)
        
        // Generate response with personality
        let systemPrompt = "You are " + this.personality
        this.client.setSystemPrompt(systemPrompt)
        
        let response = this.client.chatWithHistory(
            this.session.getMessages(),
            "gpt-3.5-turbo"
        )
        
        // Add to history
        this.session.addAssistantMessage(response)
        
        return response
    }
    
    fn setPersonality(personality: string) {
        this.personality = personality
    }
    
    fn getConversationSummary() -> string {
        let messages = this.session.getMessages()
        let summary = "Conversation with " + str(len(messages)) + " messages:\n"
        
        let i = 0
        while i < len(messages) {
            summary = summary + messages[i].role + ": "
            summary = summary + substring(messages[i].content, 0, 50) + "...\n"
            i = i + 1
        }
        
        return summary
    }
}

let agent = new ConversationalAgent(client)
agent.setPersonality("a wise teacher")

let response1 = agent.chat("Hello!")
println(response1)

let response2 = agent.chat("Teach me about AI")
println(response2)
```

---

## Multi-Agent Systems

### Agent Communication

```klang
class AgentMessage {
    constructor(from: string, to: string, content: string) {
        this.from = from
        this.to = to
        this.content = content
        this.timestamp = timestamp()
    }
}

class AgentMailbox {
    constructor() {
        this.messages = []
    }
    
    fn send(message: AgentMessage) {
        this.messages.push(message)
    }
    
    fn receive(agentName: string) -> AgentMessage[] {
        let received = []
        let remaining = []
        
        let i = 0
        while i < len(this.messages) {
            if this.messages[i].to == agentName {
                received.push(this.messages[i])
            } else {
                remaining.push(this.messages[i])
            }
            i = i + 1
        }
        
        this.messages = remaining
        return received
    }
}
```

### Collaborative Agents

```klang
class CollaborativeSystem {
    constructor(client: OpenAIClient) {
        this.client = client
        this.agents = dict()
        this.mailbox = new AgentMailbox()
    }
    
    fn addAgent(name: string, role: string) {
        let agent = dict()
        agent.set("name", name)
        agent.set("role", role)
        agent.set("memory", new AgentMemory(50))
        
        this.agents.set(name, agent)
        println("✓ Added agent: " + name + " (" + role + ")")
    }
    
    fn runCollaboration(task: string) -> string {
        println("🤝 Starting collaboration on: " + task)
        
        // Assign tasks to agents
        let agentNames = this.agents.keys()
        
        let results = dict()
        let i = 0
        while i < len(agentNames) {
            let agent = this.agents.get(agentNames[i])
            let subtask = this.assignSubtask(task, agent.get("role"))
            
            println("\n" + agentNames[i] + " working on: " + subtask)
            let result = this.executeSubtask(agentNames[i], subtask)
            results.set(agentNames[i], result)
            
            i = i + 1
        }
        
        // Synthesize results
        return this.synthesizeResults(task, results)
    }
    
    fn assignSubtask(task: string, role: string) -> string {
        let prompt = """
        Main task: """ + task + """
        Agent role: """ + role + """
        
        What specific subtask should this agent handle?
        """
        
        return this.client.chat(prompt, "gpt-3.5-turbo")
    }
    
    fn executeSubtask(agentName: string, subtask: string) -> string {
        let agent = this.agents.get(agentName)
        
        let prompt = """
        You are """ + agent.get("role") + """.
        Complete this task: """ + subtask
        
        return this.client.chat(prompt, "gpt-4")
    }
    
    fn synthesizeResults(task: string, results: dict) -> string {
        let prompt = "Original task: " + task + "\n\nAgent results:\n"
        
        let agentNames = results.keys()
        let i = 0
        while i < len(agentNames) {
            prompt = prompt + "\n" + agentNames[i] + ":\n"
            prompt = prompt + results.get(agentNames[i]) + "\n"
            i = i + 1
        }
        
        prompt = prompt + "\nSynthesize these results into a final answer."
        
        return this.client.chat(prompt, "gpt-4")
    }
}

// Usage
let system = new CollaborativeSystem(client)
system.addAgent("researcher", "research specialist")
system.addAgent("analyst", "data analyst")
system.addAgent("writer", "technical writer")

let result = system.runCollaboration("Create a report on AI trends")
println("\n📝 Final Report:\n" + result)
```

---

## Production Agents

### Error Handling

```klang
class RobustAgent {
    constructor(client: OpenAIClient) {
        this.client = client
        this.maxRetries = 3
        this.timeout = 30000  // 30 seconds
    }
    
    fn execute(task: string) -> dict {
        let result = dict()
        result.set("success", false)
        
        let retries = 0
        while retries < this.maxRetries {
            try {
                let output = this.run(task)
                result.set("success", true)
                result.set("output", output)
                result.set("retries", retries)
                return result
                
            } catch error {
                retries = retries + 1
                println("⚠️ Attempt " + str(retries) + " failed: " + error)
                
                if retries >= this.maxRetries {
                    result.set("error", error)
                    result.set("retries", retries)
                    return result
                }
                
                sleep(1000 * retries)  // Exponential backoff
            }
        }
        
        return result
    }
}
```

### Monitoring and Logging

```klang
class MonitoredAgent {
    constructor(client: OpenAIClient, logPath: string) {
        this.client = client
        this.logPath = logPath
        this.metrics = dict()
        this.metrics.set("total_tasks", 0)
        this.metrics.set("successful_tasks", 0)
        this.metrics.set("failed_tasks", 0)
        this.metrics.set("total_steps", 0)
        this.metrics.set("avg_steps_per_task", 0.0)
    }
    
    fn run(task: string) -> string {
        let startTime = timestamp()
        this.log("INFO", "Starting task: " + task)
        
        this.metrics.set("total_tasks", this.metrics.get("total_tasks") + 1)
        
        try {
            let result = this.executeTask(task)
            
            let duration = timestamp() - startTime
            this.log("SUCCESS", "Completed in " + str(duration) + "ms")
            
            this.metrics.set("successful_tasks", this.metrics.get("successful_tasks") + 1)
            
            return result
            
        } catch error {
            this.log("ERROR", "Failed: " + error)
            this.metrics.set("failed_tasks", this.metrics.get("failed_tasks") + 1)
            throw error
        }
    }
    
    fn log(level: string, message: string) {
        let entry = "[" + level + "] " + str(timestamp()) + " - " + message + "\n"
        println(entry)
        fappend(this.logPath, entry)
    }
    
    fn getMetrics() -> dict {
        return this.metrics
    }
}
```

---

## Best Practices

### 1. Clear Instructions

```klang
// Bad: Vague
let agent = new Agent(client)
let result = agent.run("Do something with data")

// Good: Specific
let instructions = """
Task: Analyze the sales data
Steps:
1. Load data from sales.csv
2. Calculate total revenue
3. Identify top 5 products
4. Generate summary report
"""
let result = agent.run(instructions)
```

### 2. Tool Design

```klang
// Good tool design
fn calculateTool(expression: string) -> string {
    // Validate input
    if len(expression) == 0 {
        return "Error: Empty expression"
    }
    
    // Sanitize (prevent code injection)
    if contains(expression, "eval") || contains(expression, "exec") {
        return "Error: Invalid expression"
    }
    
    // Execute safely
    try {
        let result = evaluate(expression)
        return "Result: " + str(result)
    } catch error {
        return "Error: " + error
    }
}
```

### 3. Memory Management

```klang
class EfficientAgent {
    constructor(client: OpenAIClient) {
        this.client = client
        this.shortTermMemory = new AgentMemory(20)  // Recent context
        this.longTermMemory = new SemanticMemory(client)  // Important facts
    }
    
    fn remember(event: string, important: bool) {
        // Always to short-term
        this.shortTermMemory.remember("event", event)
        
        // Only important to long-term
        if important {
            let meta = dict()
            meta.set("importance", "high")
            this.longTermMemory.remember(event, meta)
        }
    }
}
```

### 4. Testing Agents

```klang
fn testAgent(agent: Agent) {
    println("=== Agent Testing ===")
    
    let testCases = [
        {"input": "What is 2+2?", "expected": "4"},
        {"input": "Search for KLang", "expected": "search"},
        {"input": "Get weather in Paris", "expected": "weather"}
    ]
    
    let passed = 0
    let i = 0
    while i < len(testCases) {
        let test = testCases[i]
        let result = agent.run(test["input"])
        
        if contains(result, test["expected"]) {
            println("✓ Test " + str(i + 1) + " passed")
            passed = passed + 1
        } else {
            println("✗ Test " + str(i + 1) + " failed")
            println("  Expected: " + test["expected"])
            println("  Got: " + result)
        }
        
        i = i + 1
    }
    
    println("\nResults: " + str(passed) + "/" + str(len(testCases)) + " passed")
}
```

---

## Next Steps

- **[Chat Sessions](35-chat-sessions.md)** - Conversational agents
- **[RAG Systems](33-rag-systems.md)** - Knowledge-enhanced agents
- **[LLM Integration](31-llm-integration.md)** - Agent backends

---

## Summary

KLang provides comprehensive agent support:

✅ **Autonomous execution** - Agents make decisions  
✅ **Tool use** - Interact with external systems  
✅ **Memory** - Remember and learn  
✅ **Multi-agent** - Collaborative systems  
✅ **Production-ready** - Error handling, monitoring  

Build intelligent agents with KLang! 🤖🚀
