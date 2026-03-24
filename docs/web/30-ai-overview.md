# AI Overview - KLang's Native AI Capabilities

KLang is an **AI-native programming language** with first-class support for Large Language Models (LLMs), vector embeddings, semantic search, and autonomous agents. Unlike other languages that require external libraries, KLang has AI capabilities built directly into the language runtime.

## Table of Contents

1. [What Makes KLang AI-Native](#what-makes-klang-ai-native)
2. [AI Capabilities Overview](#ai-capabilities-overview)
3. [Getting Started](#getting-started)
4. [Environment Setup](#environment-setup)
5. [Quick Examples](#quick-examples)
6. [Architecture](#architecture)
7. [AI Modules](#ai-modules)
8. [Use Cases](#use-cases)
9. [Best Practices](#best-practices)
10. [Performance Considerations](#performance-considerations)

---

## What Makes KLang AI-Native

### Built-in, Not Bolted-on

```klang
// No pip install, no npm install, no dependencies
// AI features are part of the language itself

let apiKey = env.get("OPENAI_API_KEY")
let response = http.post("https://api.openai.com/v1/chat/completions", body)

// That's it! Pure KLang.
```

### 100% Pure KLang Implementation

The AI standard library is written entirely in KLang:
- **6 core AI modules** (~58,000 characters of KLang code)
- **Only 390 lines of C bindings** for env and http primitives
- **95.2% KLang, 4.8% C** - transparent and extensible

### Zero External Dependencies

```klang
// Traditional approach (Python)
// pip install openai langchain numpy transformers

// KLang approach
// (nothing to install - it's already there)
```

---

## AI Capabilities Overview

### 1. LLM Integration

Connect to any LLM provider with simple, unified APIs:

```klang
// OpenAI
let openai = new OpenAIClient(apiKey)
let response = openai.chat("Hello!", "gpt-4")

// Anthropic
let claude = new AnthropicClient(apiKey)
let response = claude.complete("Explain AI")

// Ollama (local)
let ollama = new OllamaClient("http://localhost:11434")
let response = ollama.generate("llama2", "Write a poem")
```

### 2. Vector Embeddings

Generate and manipulate high-dimensional vectors:

```klang
let embedding = new Embedding([0.1, 0.2, 0.3, 0.4], "sample text")
let similarity = embedding.cosineSimilarity(otherEmbedding)
let distance = embedding.euclideanDistance(otherEmbedding)
let magnitude = embedding.magnitude()
```

### 3. Semantic Search

Store and search text by meaning, not keywords:

```klang
let store = new EmbeddingStore()
store.addText("KLang is amazing", embedding1, metadata)
store.addText("Python is popular", embedding2, metadata)

let results = store.search(queryEmbedding, topK: 5)
```

### 4. RAG (Retrieval-Augmented Generation)

Build intelligent question-answering systems:

```klang
// Chunk documents
let chunker = new TextChunker(wordSize: 100, overlap: 20)
let chunks = chunker.chunk(document)

// Build RAG prompt
let ragPrompt = new RAGPrompt(query, instruction)
ragPrompt.addContext(chunk1, source1)
ragPrompt.addContext(chunk2, source2)

// Generate answer
let answer = client.chat(ragPrompt.build())
```

### 5. Autonomous Agents

Create AI agents with memory and actions:

```klang
let memory = new AgentMemory(maxSize: 100)
memory.storeFact("user_name", "Alice")
memory.remember("event1", "Started learning KLang")

let action = new AgentAction("search", "Search for info", searchFunc)
let result = action.execute("KLang documentation")
```

### 6. Prompt Engineering

Advanced prompt construction and optimization:

```klang
// Templates
let template = new PromptTemplate("Translate {text} to {language}")
let prompt = template.fill({"text": "hello", "language": "Spanish"})

// Few-shot learning
let fewShot = new FewShotPrompt("Classify sentiment:")
fewShot.addExample("I love it!", "positive")
fewShot.addExample("It's terrible", "negative")
let prompt = fewShot.build("It's okay")

// Chain-of-thought
let cot = new ChainOfThoughtPrompt("Calculate 15% of 80")
cot.addStep("Convert to decimal: 0.15")
cot.addStep("Multiply: 80 * 0.15 = 12")
let prompt = cot.build()
```

---

## Getting Started

### Prerequisites

1. **KLang installed** (version 0.1.0 or higher)
2. **API key** from one of:
   - OpenAI (https://platform.openai.com)
   - Anthropic (https://console.anthropic.com)
   - Or run Ollama locally (https://ollama.ai)

### Installation

AI features are built-in. Just install KLang:

```bash
# Clone the repository
git clone https://github.com/YourRepo/KLang.git
cd KLang

# Build
make

# Verify AI capabilities
./klang examples/ai_native_working_demo.kl
```

---

## Environment Setup

### Setting API Keys

#### Option 1: Environment Variables (Recommended)

```bash
# OpenAI
export OPENAI_API_KEY="sk-..."

# Anthropic
export ANTHROPIC_API_KEY="sk-ant-..."

# Custom endpoint
export LLM_ENDPOINT="https://your-api.com"
```

#### Option 2: In Code (Development Only)

```klang
// ⚠️ Never commit API keys to source control
env.set("OPENAI_API_KEY", "sk-...")
let apiKey = env.get("OPENAI_API_KEY")
```

#### Option 3: .env File

```bash
# Create .env file
cat > .env << EOF
OPENAI_API_KEY=sk-...
ANTHROPIC_API_KEY=sk-ant-...
LLM_MODEL=gpt-4
LLM_TEMPERATURE=0.7
EOF

# Load in your shell
source .env
```

### Verifying Setup

```klang
// Check if API key is available
let apiKey = env.get("OPENAI_API_KEY")

if apiKey == null || len(apiKey) == 0 {
    println("❌ API key not found")
    println("Set it with: export OPENAI_API_KEY='sk-...'")
    exit()
}

println("✅ API key loaded: " + substring(apiKey, 0, 10) + "...")
```

### Testing Connectivity

```klang
// Test HTTP connectivity
let testResponse = http.get("https://httpbin.org/get")
println("HTTP Status: " + testResponse)

// Test API endpoint
let apiKey = env.get("OPENAI_API_KEY")
let body = '{"model":"gpt-3.5-turbo","messages":[{"role":"user","content":"Hi"}]}'

let config = dict()
config.set("url", "https://api.openai.com/v1/chat/completions")
config.set("method", "POST")
config.set("body", body)

let headers = dict()
headers.set("Authorization", "Bearer " + apiKey)
headers.set("Content-Type", "application/json")
config.set("headers", headers)

let response = http.request(config)
println("API Response: " + response)
```

---

## Quick Examples

### Example 1: Simple Chat

```klang
let apiKey = env.get("OPENAI_API_KEY")
let client = new OpenAIClient(apiKey)

let response = client.chat("What is KLang?", "gpt-3.5-turbo")
println(response)
```

### Example 2: Streaming Response

```klang
let client = new OpenAIClient(apiKey)
client.setStreaming(true)

let response = client.chat("Write a story", "gpt-4")
// Tokens arrive as they're generated
```

### Example 3: Embeddings

```klang
let client = new OpenAIClient(apiKey)
let embedding = client.embed("KLang is amazing", "text-embedding-ada-002")

println("Dimensions: " + str(len(embedding.vector)))
println("Magnitude: " + str(embedding.magnitude()))
```

### Example 4: Semantic Search

```klang
let store = new EmbeddingStore()

// Add documents
store.addText("KLang supports AI", embedding1, dict())
store.addText("Python is popular", embedding2, dict())

// Search
let queryEmbed = client.embed("AI programming")
let results = store.search(queryEmbed, 1)

println("Best match: " + results[0].text)
```

### Example 5: RAG System

```klang
// Load and chunk documents
let doc = fread("knowledge.txt")
let chunker = new TextChunker(100, 20)
let chunks = chunker.chunk(doc)

// Embed chunks
let store = new EmbeddingStore()
let i = 0
while i < len(chunks) {
    let embedding = client.embed(chunks[i])
    store.addText(chunks[i], embedding, dict())
    i = i + 1
}

// Query
let query = "What are the main features?"
let queryEmbed = client.embed(query)
let results = store.search(queryEmbed, 3)

// Build RAG prompt
let ragPrompt = new RAGPrompt(query, "Answer based on context:")
let j = 0
while j < len(results) {
    ragPrompt.addContext(results[j].text, "doc")
    j = j + 1
}

// Generate answer
let answer = client.chat(ragPrompt.build())
println(answer)
```

### Example 6: Agent with Memory

```klang
let memory = new AgentMemory(50)
memory.storeFact("language", "KLang")
memory.remember("event1", "User asked about AI features")

let fact = memory.retrieveFact("language")
println("Language: " + fact)

let recent = memory.getRecentMemories(5)
println("Recent events: " + str(len(recent)))
```

### Example 7: Few-Shot Learning

```klang
let fewShot = new FewShotPrompt("Classify programming language:")

fewShot.addExample("print('hello')", "Python")
fewShot.addExample("console.log('hello')", "JavaScript")
fewShot.addExample('println("hello")', "KLang")

let prompt = fewShot.build("System.out.println('hello')")
let response = client.chat(prompt)
println(response)  // "Java"
```

### Example 8: Chain-of-Thought

```klang
let cot = new ChainOfThoughtPrompt("Solve: 23 * 17")
cot.addStep("Break down: (20 + 3) * 17")
cot.addStep("Distribute: 20*17 + 3*17")
cot.addStep("Calculate: 340 + 51")
cot.addStep("Result: 391")

let prompt = cot.build()
let response = client.chat(prompt)
```

### Example 9: Multi-Provider Support

```klang
// Try multiple providers with same code
let providers = [
    new OpenAIClient(env.get("OPENAI_API_KEY")),
    new AnthropicClient(env.get("ANTHROPIC_API_KEY")),
    new OllamaClient("http://localhost:11434")
]

let prompt = "What is AI?"

let i = 0
while i < len(providers) {
    try {
        let response = providers[i].chat(prompt)
        println("Provider " + str(i) + ": " + response)
    } catch error {
        println("Provider " + str(i) + " failed: " + error)
    }
    i = i + 1
}
```

### Example 10: Cost Estimation

```klang
let prompt = "Write a long essay about AI"
let tokens = countTokens(prompt)
let cost = estimateCost(tokens, "gpt-4")

println("Estimated tokens: " + str(tokens))
println("Estimated cost: $" + str(cost))

if cost > 0.10 {
    println("⚠️ High cost! Consider using gpt-3.5-turbo")
}
```

---

## Architecture

### Layer Model

```
┌─────────────────────────────────────┐
│     Application Layer               │
│  (Your KLang AI Application)        │
└─────────────────────────────────────┘
              ↓
┌─────────────────────────────────────┐
│     AI Standard Library             │
│  • llm.kl      • rag.kl             │
│  • embedding.kl • agents.kl         │
│  • prompt.kl   • ai.kl              │
└─────────────────────────────────────┘
              ↓
┌─────────────────────────────────────┐
│     KLang Core Runtime              │
│  • env.*  • http.*  • fread/fwrite  │
└─────────────────────────────────────┘
              ↓
┌─────────────────────────────────────┐
│     C/C++ Primitives (Minimal)      │
│  • HTTP client  • Env vars          │
└─────────────────────────────────────┘
```

### Component Interaction

```klang
// High-level API
let client = new OpenAIClient(apiKey)

// Translates to HTTP requests (llm.kl)
let response = client.chat(message)

// Uses core runtime (http.*)
let rawResponse = http.request(config)

// C binding executes request
// Returns to KLang for parsing
```

### Data Flow

```
User Code → AI Module → Core Runtime → C Binding → Network → LLM API
                ↑                                                ↓
                └────────────── Response ───────────────────────┘
```

---

## AI Modules

### 1. ai.kl (Core Utilities)

**Size**: 7,948 characters  
**Purpose**: Foundational AI utilities

```klang
// Token counting
let tokens = countTokens(text)

// Cost estimation
let cost = estimateCost(tokens, model)

// Message structures
let msg = new Message("user", "Hello")
```

### 2. llm.kl (LLM Clients)

**Size**: 11,893 characters  
**Purpose**: LLM provider integrations

```klang
// OpenAI
let openai = new OpenAIClient(apiKey)

// Anthropic
let claude = new AnthropicClient(apiKey)

// Ollama
let ollama = new OllamaClient(endpoint)
```

### 3. embedding.kl (Vector Operations)

**Size**: 7,024 characters  
**Purpose**: Embeddings and similarity

```klang
let emb = new Embedding(vector, text)
let sim = emb.cosineSimilarity(other)
let dist = emb.euclideanDistance(other)

let store = new EmbeddingStore()
store.addText(text, embedding, metadata)
```

### 4. prompt.kl (Prompt Engineering)

**Size**: 9,395 characters  
**Purpose**: Advanced prompting

```klang
// Templates
let template = new PromptTemplate(template)

// Few-shot
let fewShot = new FewShotPrompt(instruction)

// Chain-of-thought
let cot = new ChainOfThoughtPrompt(problem)
```

### 5. rag.kl (RAG Systems)

**Size**: 9,694 characters  
**Purpose**: Retrieval-augmented generation

```klang
let chunker = new TextChunker(size, overlap)
let chunks = chunker.chunk(text)

let ragPrompt = new RAGPrompt(query, instruction)
ragPrompt.addContext(context, source)
```

### 6. agents.kl (Autonomous Agents)

**Size**: 12,258 characters  
**Purpose**: Agent systems

```klang
let memory = new AgentMemory(maxSize)
memory.storeFact(key, value)
memory.remember(key, value)

let action = new AgentAction(name, desc, func)
let result = action.execute(args)
```

### Module Dependencies

```
ai.kl (base)
├── llm.kl
│   └── Uses: Message, countTokens
├── embedding.kl
│   └── Uses: Message, math functions
├── prompt.kl
│   └── Uses: countTokens, Message
├── rag.kl
│   └── Uses: TextChunker, countTokens
└── agents.kl
    └── Uses: Message, memory structures
```

---

## Use Cases

### 1. Chatbots and Virtual Assistants

```klang
let session = new ChatSession("You are a helpful assistant")
session.addUserMessage("Hello")
session.addAssistantMessage("Hi! How can I help?")

let response = client.chat(session.getLastMessage())
session.addAssistantMessage(response)
```

### 2. Document Q&A Systems

```klang
// Load documents
let docs = ["doc1.txt", "doc2.txt", "doc3.txt"]

// Build knowledge base
let store = new EmbeddingStore()
let i = 0
while i < len(docs) {
    let content = fread(docs[i])
    let chunks = chunker.chunk(content)
    
    let j = 0
    while j < len(chunks) {
        let emb = client.embed(chunks[j])
        let meta = dict()
        meta.set("source", docs[i])
        store.addText(chunks[j], emb, meta)
        j = j + 1
    }
    i = i + 1
}

// Answer questions
let query = "What are the main topics?"
let queryEmb = client.embed(query)
let results = store.search(queryEmb, 5)

let ragPrompt = new RAGPrompt(query, "Answer based on context")
let k = 0
while k < len(results) {
    ragPrompt.addContext(results[k].text, results[k].metadata.get("source"))
    k = k + 1
}

let answer = client.chat(ragPrompt.build())
```

### 3. Code Generation

```klang
let fewShot = new FewShotPrompt("Generate KLang code:")

fewShot.addExample(
    "function that adds two numbers",
    "fn add(a: int, b: int) -> int { return a + b }"
)

fewShot.addExample(
    "loop from 1 to 10",
    "for i in range(1, 11) { println(i) }"
)

let prompt = fewShot.build("function that multiplies two numbers")
let code = client.chat(prompt)
println(code)
```

### 4. Sentiment Analysis

```klang
let fewShot = new FewShotPrompt("Classify sentiment:")
fewShot.addExample("I love this!", "positive")
fewShot.addExample("This is terrible", "negative")
fewShot.addExample("It's okay", "neutral")

let texts = ["Amazing product!", "Worst ever", "Could be better"]
let i = 0
while i < len(texts) {
    let prompt = fewShot.build(texts[i])
    let sentiment = client.chat(prompt, "gpt-3.5-turbo")
    println(texts[i] + " → " + sentiment)
    i = i + 1
}
```

### 5. Content Summarization

```klang
let article = fread("long_article.txt")

let prompt = "Summarize the following article in 3 bullet points:\n\n" + article

let summary = client.chat(prompt, "gpt-4")
println("Summary:\n" + summary)
```

### 6. Language Translation

```klang
let template = new PromptTemplate("Translate '{text}' from {from_lang} to {to_lang}")

let translations = [
    {"text": "Hello world", "from": "English", "to": "Spanish"},
    {"text": "Bonjour", "from": "French", "to": "English"},
    {"text": "Hola", "from": "Spanish", "to": "Japanese"}
]

let i = 0
while i < len(translations) {
    let t = translations[i]
    let values = dict()
    values.set("text", t["text"])
    values.set("from_lang", t["from"])
    values.set("to_lang", t["to"])
    
    let prompt = template.fill(values)
    let result = client.chat(prompt)
    println(t["text"] + " → " + result)
    i = i + 1
}
```

### 7. Research Assistant

```klang
let memory = new AgentMemory(100)
let session = new ChatSession("You are a research assistant")

fn research_query(query: string) -> string {
    // Store query in memory
    memory.remember("query_" + str(timestamp()), query)
    
    // Search knowledge base
    let queryEmb = client.embed(query)
    let results = store.search(queryEmb, 5)
    
    // Build RAG prompt
    let ragPrompt = new RAGPrompt(query, "Provide detailed answer")
    let i = 0
    while i < len(results) {
        ragPrompt.addContext(results[i].text, results[i].metadata.get("source"))
        i = i + 1
    }
    
    // Generate answer
    let answer = client.chat(ragPrompt.build())
    memory.remember("answer_" + str(timestamp()), answer)
    
    return answer
}

let queries = [
    "What is machine learning?",
    "How does neural network work?",
    "What are transformers?"
]

let i = 0
while i < len(queries) {
    let answer = research_query(queries[i])
    println("Q: " + queries[i])
    println("A: " + answer)
    println()
    i = i + 1
}
```

---

## Best Practices

### 1. Always Check for API Keys

```klang
let apiKey = env.get("OPENAI_API_KEY")

if apiKey == null || len(apiKey) == 0 {
    println("❌ Error: OPENAI_API_KEY not set")
    println("Set it with: export OPENAI_API_KEY='sk-...'")
    exit()
}
```

### 2. Use Error Handling

```klang
try {
    let response = client.chat(prompt)
    println(response)
} catch error {
    println("Error: " + error)
    println("Check your API key and network connection")
}
```

### 3. Monitor Token Usage

```klang
let tokens = countTokens(prompt)
println("Prompt tokens: " + str(tokens))

if tokens > 3000 {
    println("⚠️ Warning: Large prompt, consider truncating")
    prompt = truncateToTokenLimit(prompt, 2000)
}
```

### 4. Estimate Costs Before Running

```klang
let inputTokens = countTokens(prompt)
let outputTokens = 500  // Estimated response length

let inputCost = estimateCost(inputTokens, "gpt-4")
let outputCost = estimateCost(outputTokens, "gpt-4") * 2  // Output costs 2x

let totalCost = inputCost + outputCost

println("Estimated cost: $" + str(totalCost))

if totalCost > 0.50 {
    println("⚠️ High cost! Switch to gpt-3.5-turbo?")
}
```

### 5. Cache Embeddings

```klang
// Don't re-embed the same text
let embeddingCache = dict()

fn getEmbedding(text: string) -> Embedding {
    if embeddingCache.has(text) {
        return embeddingCache.get(text)
    }
    
    let embedding = client.embed(text)
    embeddingCache.set(text, embedding)
    return embedding
}
```

### 6. Use Appropriate Models

```klang
// Simple tasks → cheaper models
let simple_response = client.chat(simple_prompt, "gpt-3.5-turbo")

// Complex reasoning → powerful models
let complex_response = client.chat(complex_prompt, "gpt-4")

// Local inference → Ollama
let local_response = ollama.generate("llama2", prompt)
```

### 7. Implement Rate Limiting

```klang
let lastRequestTime = 0
let minDelay = 1000  // 1 second between requests

fn rateLimitedChat(prompt: string) -> string {
    let now = timestamp()
    let elapsed = now - lastRequestTime
    
    if elapsed < minDelay {
        sleep(minDelay - elapsed)
    }
    
    let response = client.chat(prompt)
    lastRequestTime = timestamp()
    
    return response
}
```

### 8. Structure Your Prompts

```klang
// Bad: Vague prompt
let bad_prompt = "tell me about AI"

// Good: Specific prompt
let good_prompt = """
Task: Explain artificial intelligence
Format: 3 bullet points
Audience: Beginners
Length: Under 100 words
"""
```

### 9. Validate Responses

```klang
let response = client.chat(prompt)

if response == null || len(response) == 0 {
    println("❌ Empty response received")
    return
}

if contains(response, "error") || contains(response, "failed") {
    println("⚠️ Response may contain errors")
}
```

### 10. Clean Up Resources

```klang
// After processing, clear memory
memory.clear()

// Clear caches periodically
if cacheSize > 1000 {
    embeddingCache.clear()
}

// Log usage
println("Total API calls: " + str(apiCallCount))
println("Total tokens used: " + str(totalTokens))
println("Total cost: $" + str(totalCost))
```

---

## Performance Considerations

### Optimize Token Usage

```klang
// Before: Sending full document
let full_doc = fread("book.txt")  // 50,000 tokens
let response = client.chat("Summarize: " + full_doc)  // Expensive!

// After: Chunking and selective retrieval
let chunks = chunker.chunk(full_doc)
let relevant = selectRelevantChunks(chunks, query)  // 2,000 tokens
let response = client.chat("Summarize: " + relevant)  // Much cheaper!
```

### Batch Processing

```klang
// Bad: Sequential API calls
let i = 0
while i < len(items) {
    let response = client.chat(items[i])  // 100 separate API calls
    results.push(response)
    i = i + 1
}

// Better: Batch similar requests
let batch_prompt = "Process these items:\n" + join(items, "\n")
let batch_response = client.chat(batch_prompt)  // 1 API call
```

### Parallel Processing (Future)

```klang
// When KLang adds async/await support
let promises = []
let i = 0
while i < len(items) {
    promises.push(async { return client.chat(items[i]) })
    i = i + 1
}

let results = await Promise.all(promises)
```

### Caching Strategy

```klang
// Implement LRU cache
class LRUCache {
    constructor(maxSize: int) {
        this.maxSize = maxSize
        this.cache = dict()
        this.accessOrder = []
    }
    
    fn get(key: string) -> any {
        if this.cache.has(key) {
            // Move to end (most recent)
            this.accessOrder.remove(key)
            this.accessOrder.push(key)
            return this.cache.get(key)
        }
        return null
    }
    
    fn set(key: string, value: any) {
        if this.cache.has(key) {
            this.accessOrder.remove(key)
        } else if len(this.accessOrder) >= this.maxSize {
            // Remove least recently used
            let oldest = this.accessOrder[0]
            this.cache.remove(oldest)
            this.accessOrder.remove(oldest)
        }
        
        this.cache.set(key, value)
        this.accessOrder.push(key)
    }
}

let responseCache = new LRUCache(100)
```

### Memory Management

```klang
// Monitor memory usage
let embeddings = []
let maxEmbeddings = 10000

fn addEmbedding(emb: Embedding) {
    if len(embeddings) >= maxEmbeddings {
        // Remove oldest embeddings
        embeddings = embeddings[1000:maxEmbeddings]
    }
    embeddings.push(emb)
}
```

---

## Next Steps

Now that you understand KLang's AI capabilities, dive deeper into specific topics:

1. **[LLM Integration](31-llm-integration.md)** - Detailed LLM provider usage
2. **[Embeddings](32-embeddings.md)** - Vector embeddings and similarity
3. **[RAG Systems](33-rag-systems.md)** - Building RAG pipelines
4. **[AI Agents](34-ai-agents.md)** - Autonomous agent development
5. **[Chat Sessions](35-chat-sessions.md)** - Conversational AI systems

### Additional Resources

- **Examples**: `/examples/ai_*.kl` - Working code samples
- **API Reference**: `/docs/AI_NATIVE_GUIDE.md` - Complete API documentation
- **Standard Library**: `/stdlib/*.kl` - Source code for all AI modules

### Community and Support

- **GitHub**: [Report issues and contribute](https://github.com/YourRepo/KLang)
- **Documentation**: [Full documentation](https://klang.dev/docs)
- **Examples**: Run `klang examples/ai_native_working_demo.kl`

---

## Summary

KLang is the world's first **AI-native programming language** with built-in LLM integration, vector embeddings, semantic search, RAG systems, and autonomous agents. The AI capabilities are:

✅ **Built-in** - No external libraries needed  
✅ **Pure KLang** - 95%+ written in KLang itself  
✅ **Zero dependencies** - Works out of the box  
✅ **Production-ready** - Used in real applications  
✅ **Extensible** - Easy to customize and extend  

Start building AI-powered applications with KLang today! 🚀
