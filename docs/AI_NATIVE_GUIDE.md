# KLang AI-Native Language Guide

> Making AI a first-class citizen in programming

## Overview

KLang is now an **AI-Native Core Language** with built-in support for:
- 🤖 LLM (Large Language Model) integration
- 📊 Vector embeddings and semantic search
- 🧠 Retrieval-Augmented Generation (RAG)
- 🎯 Autonomous AI agents
- 💬 Prompt engineering utilities
- 🌐 HTTP/REST API client
- 🔐 Environment variable management

**All AI features are implemented 100% in KLang** (with minimal C bindings for HTTP/env).

---

## Architecture

```
┌─────────────────────────────────────────┐
│  Pure KLang AI Libraries (stdlib/)      │
│  - ai.kl (core types & utilities)       │
│  - llm.kl (LLM providers)               │
│  - embedding.kl (vector operations)     │
│  - prompt.kl (prompt engineering)       │
│  - rag.kl (RAG systems)                 │
│  - agents.kl (autonomous agents)        │
└─────────────────────────────────────────┘
              ↓
┌─────────────────────────────────────────┐
│  C Runtime Bindings (src/runtime.c)     │
│  - http.get() / http.post()             │
│  - env.get() / env.set()                │
│  - JSON utilities                        │
└─────────────────────────────────────────┘
```

---

## Quick Start

### 1. Environment Setup

```bash
# Set your OpenAI API key
export OPENAI_API_KEY="sk-..."

# Or Anthropic
export ANTHROPIC_API_KEY="sk-ant-..."
```

### 2. Simple LLM Query

```klang
# Simple one-liner
let response = llm("What is the capital of France?")
println(response)
```

### 3. Chat Conversation

```klang
# Create a chat session
let session = new ChatSession("You are a helpful assistant.")

# Add messages
session.addUserMessage("Hi, I'm learning KLang")
let response = llmClient.chat(session)
println(response)

session.addUserMessage("Tell me about AI features")
let response2 = llmClient.chat(session)
println(response2)
```

---

## Core Features

### 🌐 HTTP Client

Built-in HTTP client for making API calls:

```klang
# Simple GET request
let response = http.get("https://api.example.com/data")

# POST request
let body = '{"key": "value"}'
let response = http.post("https://api.example.com/endpoint", body)

# Advanced request with headers
let config = dict()
config.set("url", "https://api.openai.com/v1/chat/completions")
config.set("method", "POST")
config.set("body", jsonBody)

let headers = dict()
headers.set("Authorization", "Bearer " + apiKey)
headers.set("Content-Type", "application/json")
config.set("headers", headers)

let response = http.request(config)
```

### 🔐 Environment Variables

Access environment variables securely:

```klang
# Get environment variable
let apiKey = env.get("OPENAI_API_KEY")

# Check if variable exists
if env.has("API_KEY") {
    let key = env.get("API_KEY")
}

# Set environment variable
env.set("MY_VAR", "my_value")
```

### 🤖 LLM Integration

#### OpenAI

```klang
# Configure client
let config = new LLMConfig("openai")
config.setModel("gpt-4")
config.setTemperature(0.7)
config.setMaxTokens(1000)

let client = new LLMClient(config)

# Simple completion
let response = client.complete("Explain KLang in one sentence")
println(response)
```

#### Anthropic Claude

```klang
let config = new LLMConfig("anthropic")
config.setModel("claude-3-opus-20240229")

let client = new LLMClient(config)
let response = client.complete("What is AI?")
```

#### Local Ollama

```klang
let config = new LLMConfig("ollama")
config.setModel("llama2")

let client = new LLMClient(config)
let response = client.complete("Hello!")
```

### 📊 Vector Embeddings

Create and compare embeddings:

```klang
# Create embedding
let embedding1 = new Embedding([0.1, 0.2, 0.3, 0.4], "Hello world")

# Compare embeddings
let similarity = embedding1.cosineSimilarity(embedding2)
println("Similarity: " + str(similarity))

# Distance metrics
let distance = embedding1.euclideanDistance(embedding2)
let dotProd = embedding1.dotProduct(embedding2)

# Vector operations
let normalized = embedding1.normalize()
let magnitude = embedding1.magnitude()
```

### 🔍 Semantic Search

```klang
# Create embedding store
let store = new EmbeddingStore()

# Add documents with embeddings
store.addText("KLang is fast", embedding1)
store.addText("Python is popular", embedding2)
store.addText("KLang is AI-native", embedding3)

# Search
let queryEmbedding = client.embed("fast programming language")
let results = store.search(queryEmbedding, 3)

# Process results
let i = 0
while i < len(results) {
    println("Result " + str(i + 1) + ": " + str(results[i].similarity))
    i = i + 1
}
```

### 🧠 RAG (Retrieval-Augmented Generation)

Complete RAG system in pure KLang:

```klang
# Create RAG system
let rag = new RAGSystem(llmClient, 500, 3)

# Ingest documents
rag.ingestDocument("doc1", "KLang is a modern programming language...")
rag.ingestDocument("doc2", "AI features include LLM integration...")

# Query the system
let answer = rag.query("What is KLang?")
println(answer)

# Query with citations
let result = rag.queryWithCitations("Tell me about AI features")
println("Answer: " + result.get("answer"))
println("Sources: " + str(len(result.get("sources"))))
```

### 🎯 Prompt Engineering

#### Templates

```klang
let template = new PromptTemplate("Write a {language} function that {task}")

let values = dict()
values.set("language", "KLang")
values.set("task", "sorts an array")

let prompt = template.fill(values)
```

#### Few-Shot Learning

```klang
let fewShot = new FewShotPrompt("Convert English to code:")

fewShot.addExample("print hello", 'println("Hello")')
fewShot.addExample("add 2 and 3", "2 + 3")

let prompt = fewShot.build("loop from 0 to 10")
```

#### Chain-of-Thought

```klang
let cot = new ChainOfThoughtPrompt("Solve: What is 15% of 80?")
cot.addStep("Convert 15% to decimal: 0.15")
cot.addStep("Multiply: 80 * 0.15")
cot.addStep("Result: 12")

let prompt = cot.build()
```

#### RAG Prompts

```klang
let ragPrompt = new RAGPrompt("What features does KLang have?")
ragPrompt.addContext("KLang has OOP support", "features.md")
ragPrompt.addContext("KLang has AI integration", "ai.md")

let prompt = ragPrompt.build()
```

### 🤖 Autonomous Agents

#### Simple Agent

```klang
# Create agent
let agent = new Agent("Assistant", "helpful AI", llmClient)

# Add goals
agent.addGoal("Help the user with their task")
agent.addGoal("Provide accurate information")

# Register actions
let searchAction = new AgentAction("search", "Search for information", fn(args) {
    # Implement search logic
    return "search results"
})

agent.registerAction(searchAction)

# Agent thinks and acts
let thought = agent.think("User asked about KLang")
println(thought)

let result = agent.act("search", "KLang features")
println(result)
```

#### ReAct Agent

```klang
# ReAct: Reasoning + Acting agent
let reactAgent = new ReActAgent("Solver", "problem solver", llmClient)

# Register actions
reactAgent.registerAction(calculateAction)
reactAgent.registerAction(searchAction)

# Solve problem autonomously
let solution = reactAgent.solve("Find the sum of numbers from 1 to 100")
println("Solution: " + solution)
```

#### Task Agent

```klang
let taskAgent = new TaskAgent("TaskBot", llmClient)

taskAgent.addTask("Research KLang documentation")
taskAgent.addTask("Write a summary")
taskAgent.addTask("Create code examples")

taskAgent.executeTasks()

let status = taskAgent.getStatus()
println("Completed: " + str(status.completed))
println("Pending: " + str(status.pending))
```

---

## Advanced Patterns

### Pattern 1: Semantic Caching

```klang
let cache = new SemanticCache(0.95)  # 95% similarity threshold

# Check cache before API call
let cachedResponse = cache.get(queryEmbedding)
if cachedResponse != null {
    return cachedResponse
}

# Make API call and cache result
let response = llmClient.complete(prompt)
cache.set(queryEmbedding, response)
```

### Pattern 2: Document Chunking

```klang
# Chunk by word count with overlap
let chunker = new TextChunker(500, 50)
let chunks = chunker.chunk(longDocument)

# Chunk by lines
let lineChunks = chunker.chunkByLines(document, 100)
```

### Pattern 3: Vector Operations

```klang
# Average multiple embeddings
let embeddings = [emb1, emb2, emb3]
let avgEmbedding = vectorAverage(embeddings)

# Add/subtract vectors
let combined = vectorAdd(vec1, vec2)
let difference = vectorSubtract(vec1, vec2)

# Scale vector
let scaled = vectorScale(vec1, 0.5)
```

### Pattern 4: Multi-Agent Coordination

```klang
# Create multiple specialized agents
let researcher = new Agent("Researcher", "information finder", llmClient)
let analyst = new Agent("Analyst", "data analyzer", llmClient)
let writer = new Agent("Writer", "content creator", llmClient)

# Workflow
let data = researcher.act("search", "KLang AI")
let analysis = analyst.think(data)
let article = writer.act("write", analysis)
```

---

## Best Practices

### 1. API Key Management

```klang
# ✅ Good - Use environment variables
let apiKey = env.get("OPENAI_API_KEY")

# ❌ Bad - Hardcode keys
let apiKey = "sk-1234..."  # Never do this!
```

### 2. Error Handling

```klang
# Check API key exists
if apiKey == null || len(apiKey) == 0 {
    println("Error: API key not found")
    exit()
}

# Check HTTP response
let response = http.get(url)
if response == null {
    println("Error: Request failed")
    return
}
```

### 3. Token Management

```klang
# Estimate tokens before calling API
let tokens = estimateTokens(prompt)
let cost = estimateCost(tokens, "gpt-4")

println("Estimated cost: $" + str(cost))

# Truncate if needed
if tokens > 4000 {
    prompt = truncateToTokenLimit(prompt, 4000)
}
```

### 4. Prompt Optimization

```klang
# Always optimize prompts
let optimized = optimizePrompt(rawPrompt)

# Use templates for reusability
let template = new PromptTemplate(promptString)
let filled = template.fill(variables)
```

---

## API Reference

### Core Classes

#### `AIConfig`
Configuration for AI services.

**Constructor:**
```klang
let config = new AIConfig(apiKey: string, model: string)
```

**Methods:**
- `setTemperature(temp: float)` - Set temperature (0.0 to 2.0)
- `setMaxTokens(tokens: int)` - Set max response tokens
- `getApiKey() -> string` - Get API key
- `getModel() -> string` - Get model name

#### `Message`
Represents a chat message.

**Constructor:**
```klang
let msg = new Message(role: string, content: string)
```

**Methods:**
- `toDict()` - Convert to dictionary
- `toString() -> string` - Convert to string
- `fromDict(dict) -> Message` (static) - Create from dict

#### `ChatSession`
Manages conversation history.

**Constructor:**
```klang
let session = new ChatSession(systemPrompt: string, config)
```

**Methods:**
- `addMessage(role: string, content: string)` - Add message
- `addUserMessage(content: string)` - Add user message
- `addAssistantMessage(content: string)` - Add assistant message
- `getMessages()` - Get all messages
- `getLastMessage()` - Get last message
- `clear()` - Clear history
- `toJSON() -> string` - Export as JSON
- `getMessageCount() -> int` - Get message count

#### `Embedding`
Vector embedding representation.

**Constructor:**
```klang
let emb = new Embedding(vector: list, text: string, model: string)
```

**Methods:**
- `getDimension() -> int` - Get vector dimension
- `cosineSimilarity(other) -> float` - Calculate cosine similarity
- `euclideanDistance(other) -> float` - Calculate Euclidean distance
- `dotProduct(other) -> float` - Calculate dot product
- `magnitude() -> float` - Get vector magnitude
- `normalize() -> Embedding` - Normalize vector

#### `LLMClient`
Client for LLM API interactions.

**Constructor:**
```klang
let client = new LLMClient(config: LLMConfig)
```

**Methods:**
- `complete(prompt: string) -> string` - Single completion
- `chat(session: ChatSession) -> string` - Chat with history
- `embed(text: string) -> Embedding` - Generate embedding

#### `EmbeddingStore`
Store and search embeddings.

**Constructor:**
```klang
let store = new EmbeddingStore()
```

**Methods:**
- `add(embedding, metadata)` - Add embedding
- `addText(text: string, embedding, metadata)` - Add with text
- `search(queryEmbedding, topK: int) -> list` - Semantic search
- `size() -> int` - Get store size
- `clear()` - Clear all embeddings

#### `RAGSystem`
Complete RAG implementation.

**Constructor:**
```klang
let rag = new RAGSystem(llmClient, chunkSize: int, topK: int)
```

**Methods:**
- `ingestDocument(docId: string, content: string, metadata)` - Add document
- `query(question: string) -> string` - Query RAG system
- `queryWithCitations(question: string) -> dict` - Query with sources
- `getDocumentCount() -> int` - Get document count

#### `Agent`
Autonomous AI agent.

**Constructor:**
```klang
let agent = new Agent(name: string, role: string, llmClient)
```

**Methods:**
- `registerAction(action: AgentAction)` - Register action
- `addGoal(goal: string)` - Add goal
- `think(observation: string) -> string` - Reason about observation
- `act(actionName: string, args) -> any` - Execute action
- `run(initialObservation: string, maxSteps: int)` - Autonomous loop

### Prompt Engineering Classes

#### `PromptTemplate`
Template with variable substitution.

```klang
let template = new PromptTemplate("Write a {language} {type}")
let prompt = template.fill(variables)
```

#### `FewShotPrompt`
Few-shot learning prompt builder.

```klang
let fewShot = new FewShotPrompt("Instruction...")
fewShot.addExample("input", "output")
let prompt = fewShot.build("query")
```

#### `ChainOfThoughtPrompt`
Step-by-step reasoning prompts.

```klang
let cot = new ChainOfThoughtPrompt("Problem...")
cot.addStep("Step 1...")
let prompt = cot.build()
```

#### `RAGPrompt`
RAG-specific prompt builder.

```klang
let rag = new RAGPrompt("Question?", "Instruction...")
rag.addContext("Context...", "source")
let prompt = rag.build()
```

### Utility Functions

#### Token Management

```klang
# Count tokens
let count = countTokens(text)

# Estimate cost
let cost = estimateCost(count, "gpt-4")

# Truncate to limit
let truncated = truncateToTokenLimit(text, 4000)
```

#### Prompt Optimization

```klang
# Remove excess whitespace
let optimized = optimizePrompt(prompt)

# Combine multiple prompts
let combined = combinePrompts(prompts, "\n\n")
```

---

## Complete Examples

### Example 1: RAG System

```klang
# Setup
let config = new LLMConfig("openai")
let client = new LLMClient(config)
let rag = new RAGSystem(client, 500, 3)

# Ingest documents
rag.ingestDocument("kb1", "KLang supports OOP with classes and inheritance")
rag.ingestDocument("kb2", "KLang has AI-native features like LLM integration")

# Query
let answer = rag.query("Does KLang support AI?")
println(answer)
```

### Example 2: Chatbot with Memory

```klang
# Create session with system prompt
let session = new ChatSession(
    "You are a KLang programming expert. Help users learn KLang."
)

# Chat loop
let running = true
while running {
    let userInput = input("You: ")
    
    if userInput == "exit" {
        running = false
        break
    }
    
    session.addUserMessage(userInput)
    
    let response = client.chat(session)
    println("Bot: " + response)
}
```

### Example 3: Code Generation Agent

```klang
let agent = new Agent("CodeGen", "code generator", llmClient)

# Define actions
let writeCodeAction = new AgentAction(
    "write_code",
    "Write code to a file",
    fn(args) {
        writeFile(args.filename, args.code)
        return "Code written to " + args.filename
    }
)

agent.registerAction(writeCodeAction)
agent.addGoal("Generate KLang code examples")

# Run agent
agent.run("Create a hello world example", 5)
```

### Example 4: Semantic Search over Files

```klang
# Load documents from files
let filePaths = [
    "./docs/README.md",
    "./docs/LANGUAGE_REFERENCE.md",
    "./docs/OOP_SPEC.md"
]

let rag = buildRAGFromFiles(llmClient, filePaths)

# Query the documentation
let answer = rag.query("How do I create a class in KLang?")
println(answer)
```

---

## Performance Tips

1. **Cache embeddings** - Embeddings are expensive, cache them:
   ```klang
   let cache = new SemanticCache(0.95)
   ```

2. **Batch operations** - Process multiple prompts together:
   ```klang
   let batch = new BatchProcessor(config)
   batch.addPrompt(prompt1)
   batch.addPrompt(prompt2)
   ```

3. **Optimize chunks** - Use appropriate chunk sizes:
   ```klang
   # Smaller chunks = more API calls but better precision
   let rag = new RAGSystem(client, 300, 5)
   ```

4. **Use cheaper models** - For simple tasks:
   ```klang
   config.setModel("gpt-3.5-turbo")  # Cheaper than gpt-4
   ```

---

## Provider Support

| Provider | Status | Models |
|----------|--------|--------|
| OpenAI | ✅ Fully Supported | GPT-4, GPT-3.5-turbo |
| Anthropic | ✅ Fully Supported | Claude 3 Opus, Sonnet, Haiku |
| Ollama | ✅ Supported | Llama 2, Mistral, etc. (local) |
| Custom | ✅ Configurable | Any OpenAI-compatible API |

---

## Roadmap

### Phase 1 (Current) ✅
- [x] HTTP client
- [x] Environment variables
- [x] LLM integration (OpenAI, Anthropic, Ollama)
- [x] Vector embeddings
- [x] Semantic search
- [x] RAG system
- [x] Prompt engineering
- [x] Autonomous agents

### Phase 2 (Planned)
- [ ] Streaming completions
- [ ] Function calling / Tools
- [ ] Image generation
- [ ] Speech-to-text
- [ ] Fine-tuning utilities
- [ ] Model evaluation

### Phase 3 (Future)
- [ ] Local model inference
- [ ] Quantization support
- [ ] GPU acceleration
- [ ] Distributed agents
- [ ] Multi-modal AI

---

## Contributing

Want to extend KLang's AI capabilities?

1. **Add new providers** - Extend `LLMClient` class
2. **Create new agent types** - Extend `Agent` base class
3. **Build AI tools** - Add new stdlib modules
4. **Optimize performance** - Improve chunking, caching

All contributions welcome!

---

## Examples

See `/examples` directory:
- `ai_chatbot_simple.kl` - Basic chatbot
- `ai_rag_demo.kl` - RAG system demo
- `ai_agent_demo.kl` - Autonomous agents
- `ai_prompt_engineering.kl` - Prompt patterns

---

## License

MIT License - Same as KLang

---

## Credits

Built with KLang's powerful runtime and 100% KLang standard library.

**AI features implemented entirely in KLang** - Proving that KLang is truly AI-native! 🚀
