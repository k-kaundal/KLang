# LLM Integration - Large Language Model Clients

KLang provides first-class support for multiple LLM providers through a unified, consistent API. This guide covers everything you need to know about integrating and using Large Language Models in your KLang applications.

## Table of Contents

1. [Supported Providers](#supported-providers)
2. [OpenAI Integration](#openai-integration)
3. [Anthropic Integration](#anthropic-integration)
4. [Ollama Integration](#ollama-integration)
5. [Unified Client Interface](#unified-client-interface)
6. [Chat Completions](#chat-completions)
7. [Text Completions](#text-completions)
8. [Streaming Responses](#streaming-responses)
9. [Configuration Options](#configuration-options)
10. [Error Handling](#error-handling)
11. [Cost Optimization](#cost-optimization)

---

## Supported Providers

### Provider Comparison

| Provider | Models | Streaming | Cost | Best For |
|----------|--------|-----------|------|----------|
| **OpenAI** | GPT-4, GPT-3.5 | ✅ Yes | $$$ | General purpose, coding |
| **Anthropic** | Claude 3 | ✅ Yes | $$ | Long context, analysis |
| **Ollama** | Llama 2, Mistral | ✅ Yes | Free | Local, privacy |

### Getting API Keys

**OpenAI**: https://platform.openai.com/api-keys  
**Anthropic**: https://console.anthropic.com/  
**Ollama**: No API key needed (local)

---

## OpenAI Integration

### Setup

```klang
// Set API key
env.set("OPENAI_API_KEY", "sk-...")
let apiKey = env.get("OPENAI_API_KEY")

// Create client
let client = new OpenAIClient(apiKey)
```

### Basic Chat

```klang
let apiKey = env.get("OPENAI_API_KEY")
let client = new OpenAIClient(apiKey)

// Simple chat
let response = client.chat("What is KLang?", "gpt-3.5-turbo")
println(response)
```

### Chat with System Prompt

```klang
let client = new OpenAIClient(apiKey)

// Set system prompt
client.setSystemPrompt("You are a helpful programming assistant specializing in KLang.")

// Chat with context
let response = client.chat("How do I define a function?", "gpt-3.5-turbo")
println(response)
```

### Multi-Turn Conversation

```klang
let client = new OpenAIClient(apiKey)

// First message
let msg1 = new Message("user", "My name is Alice")
let response1 = client.chatWithMessage(msg1, "gpt-3.5-turbo")
println("Bot: " + response1)

// Second message (with context)
let msg2 = new Message("user", "What's my name?")
let response2 = client.chatWithMessage(msg2, "gpt-3.5-turbo")
println("Bot: " + response2)  // Should say "Alice"
```

### Chat with Parameters

```klang
let client = new OpenAIClient(apiKey)

// Set parameters
client.setTemperature(0.8)      // Creativity (0.0-2.0)
client.setMaxTokens(500)        // Response length
client.setTopP(0.9)             // Nucleus sampling
client.setFrequencyPenalty(0.5) // Reduce repetition
client.setPresencePenalty(0.3)  // Encourage diversity

let response = client.chat("Write a creative story", "gpt-4")
println(response)
```

### Available Models

```klang
let models = [
    "gpt-4",                    // Most capable
    "gpt-4-turbo-preview",      // Faster GPT-4
    "gpt-3.5-turbo",            // Fast and cheap
    "gpt-3.5-turbo-16k"         // Extended context
]

let i = 0
while i < len(models) {
    println("Testing model: " + models[i])
    let response = client.chat("Say hello", models[i])
    println("  Response: " + response)
    println()
    i = i + 1
}
```

### Function Calling (Tools)

```klang
let client = new OpenAIClient(apiKey)

// Define function schema
let weatherFunc = dict()
weatherFunc.set("name", "get_weather")
weatherFunc.set("description", "Get current weather for a location")

let parameters = dict()
parameters.set("type", "object")

let properties = dict()
let location = dict()
location.set("type", "string")
location.set("description", "City name")
properties.set("location", location)

parameters.set("properties", properties)
let required = ["location"]
parameters.set("required", required)

weatherFunc.set("parameters", parameters)

// Add to client
client.addFunction(weatherFunc)

// Call with function
let response = client.chat("What's the weather in Paris?", "gpt-3.5-turbo")
// Response will include function call
```

### Image Input (GPT-4 Vision)

```klang
let client = new OpenAIClient(apiKey)

let imageUrl = "https://example.com/image.jpg"
let response = client.chatWithImage(
    "What's in this image?",
    imageUrl,
    "gpt-4-vision-preview"
)
println(response)
```

### JSON Mode

```klang
let client = new OpenAIClient(apiKey)
client.setResponseFormat("json_object")

let prompt = """
Extract person information from this text:
"John Smith is 30 years old and lives in New York"

Return JSON with fields: name, age, city
"""

let response = client.chat(prompt, "gpt-3.5-turbo")
// Response will be valid JSON
println(response)
```

### Token Usage Tracking

```klang
let client = new OpenAIClient(apiKey)

let response = client.chat("Hello!", "gpt-3.5-turbo")

let usage = client.getLastUsage()
println("Prompt tokens: " + str(usage.promptTokens))
println("Completion tokens: " + str(usage.completionTokens))
println("Total tokens: " + str(usage.totalTokens))
```

### Embeddings

```klang
let client = new OpenAIClient(apiKey)

// Generate embedding
let embedding = client.embed("KLang is amazing", "text-embedding-ada-002")

println("Dimensions: " + str(len(embedding.vector)))
println("Magnitude: " + str(embedding.magnitude()))

// Batch embeddings
let texts = ["Hello", "World", "KLang"]
let embeddings = client.embedBatch(texts, "text-embedding-ada-002")

println("Generated " + str(len(embeddings)) + " embeddings")
```

---

## Anthropic Integration

### Setup

```klang
// Set API key
env.set("ANTHROPIC_API_KEY", "sk-ant-...")
let apiKey = env.get("ANTHROPIC_API_KEY")

// Create client
let client = new AnthropicClient(apiKey)
```

### Basic Completion

```klang
let client = new AnthropicClient(apiKey)

let response = client.complete("Explain quantum computing", "claude-3-sonnet-20240229")
println(response)
```

### Chat with Claude

```klang
let client = new AnthropicClient(apiKey)

// Set system prompt
client.setSystemPrompt("You are Claude, a helpful AI assistant.")

// Chat
let response = client.chat("What can you help me with?", "claude-3-opus-20240229")
println(response)
```

### Long Context Handling

```klang
let client = new AnthropicClient(apiKey)

// Claude has 200k token context window
let longDocument = fread("very_long_book.txt")

let prompt = "Summarize this entire book:\n\n" + longDocument

let response = client.complete(prompt, "claude-3-opus-20240229")
println(response)
```

### Available Models

```klang
let models = [
    "claude-3-opus-20240229",      // Most capable
    "claude-3-sonnet-20240229",    // Balanced
    "claude-3-haiku-20240307",     // Fast and efficient
    "claude-2.1",                  // Previous generation
    "claude-instant-1.2"           // Fast legacy
]

let i = 0
while i < len(models) {
    println("Testing: " + models[i])
    let response = client.complete("Count to 3", models[i])
    println("  " + response)
    i = i + 1
}
```

### Multi-Modal (Vision)

```klang
let client = new AnthropicClient(apiKey)

let imageData = readImageAsBase64("photo.jpg")
let response = client.analyzeImage(
    "Describe this image in detail",
    imageData,
    "claude-3-opus-20240229"
)
println(response)
```

### Streaming

```klang
let client = new AnthropicClient(apiKey)
client.setStreaming(true)

let response = client.complete("Write a poem about KLang", "claude-3-sonnet-20240229")
// Tokens arrive as they're generated
```

### Configuration

```klang
let client = new AnthropicClient(apiKey)

// Configure parameters
client.setTemperature(0.7)
client.setMaxTokens(1000)
client.setTopP(0.9)
client.setTopK(40)

let response = client.complete("Generate creative ideas", "claude-3-opus-20240229")
```

---

## Ollama Integration

### Setup

```klang
// No API key needed - runs locally
let client = new OllamaClient("http://localhost:11434")
```

### Install Ollama

```bash
# Download from https://ollama.ai
curl -fsSL https://ollama.ai/install.sh | sh

# Pull a model
ollama pull llama2
ollama pull mistral
ollama pull codellama
```

### Basic Generation

```klang
let client = new OllamaClient("http://localhost:11434")

let response = client.generate("llama2", "What is artificial intelligence?")
println(response)
```

### Available Models

```klang
let client = new OllamaClient("http://localhost:11434")

// List available models
let models = client.listModels()
println("Available models: " + str(len(models)))

let i = 0
while i < len(models) {
    println("  - " + models[i])
    i = i + 1
}
```

### Chat Interface

```klang
let client = new OllamaClient("http://localhost:11434")

// Multi-turn chat
let session = new ChatSession("You are a helpful assistant")
session.addUserMessage("Hello!")

let response1 = client.chat("llama2", session.getMessages())
session.addAssistantMessage(response1)

session.addUserMessage("Tell me about KLang")
let response2 = client.chat("llama2", session.getMessages())
println(response2)
```

### Streaming

```klang
let client = new OllamaClient("http://localhost:11434")
client.setStreaming(true)

let response = client.generate("llama2", "Write a story")
// Tokens stream in real-time
```

### Embeddings (Local)

```klang
let client = new OllamaClient("http://localhost:11434")

let embedding = client.embed("llama2", "Sample text")
println("Embedding dimensions: " + str(len(embedding.vector)))
```

### Custom Parameters

```klang
let client = new OllamaClient("http://localhost:11434")

// Set model parameters
client.setTemperature(0.8)
client.setTopP(0.95)
client.setTopK(40)
client.setNumPredict(256)  // Max tokens

let response = client.generate("mistral", "Be creative!")
```

### System Prompts

```klang
let client = new OllamaClient("http://localhost:11434")

let system = "You are a programming expert specializing in KLang."
let response = client.generateWithSystem("codellama", system, "How do I define a class?")
println(response)
```

### Model Management

```klang
let client = new OllamaClient("http://localhost:11434")

// Pull new model
client.pullModel("llama2")

// Remove model
client.removeModel("old-model")

// Show model info
let info = client.showModel("llama2")
println("Model size: " + info.size)
println("Parameters: " + info.parameters)
```

---

## Unified Client Interface

### Abstract Base Class

```klang
// All clients inherit from BaseLLMClient
class BaseLLMClient {
    fn chat(message: string, model: string) -> string
    fn complete(prompt: string, model: string) -> string
    fn setTemperature(temp: float)
    fn setMaxTokens(tokens: int)
    fn setStreaming(enabled: bool)
}
```

### Provider-Agnostic Code

```klang
fn askQuestion(client: BaseLLMClient, question: string, model: string) -> string {
    // Works with any provider!
    return client.chat(question, model)
}

// Use with OpenAI
let openai = new OpenAIClient(env.get("OPENAI_API_KEY"))
let answer1 = askQuestion(openai, "What is AI?", "gpt-3.5-turbo")

// Use with Anthropic
let anthropic = new AnthropicClient(env.get("ANTHROPIC_API_KEY"))
let answer2 = askQuestion(anthropic, "What is AI?", "claude-3-sonnet-20240229")

// Use with Ollama
let ollama = new OllamaClient("http://localhost:11434")
let answer3 = askQuestion(ollama, "What is AI?", "llama2")
```

### Fallback Strategy

```klang
fn robustChat(message: string) -> string {
    // Try OpenAI first
    try {
        let openai = new OpenAIClient(env.get("OPENAI_API_KEY"))
        return openai.chat(message, "gpt-3.5-turbo")
    } catch error {
        println("OpenAI failed, trying Anthropic...")
    }
    
    // Fallback to Anthropic
    try {
        let anthropic = new AnthropicClient(env.get("ANTHROPIC_API_KEY"))
        return anthropic.chat(message, "claude-3-haiku-20240307")
    } catch error {
        println("Anthropic failed, trying Ollama...")
    }
    
    // Final fallback to local
    try {
        let ollama = new OllamaClient("http://localhost:11434")
        return ollama.generate("llama2", message)
    } catch error {
        return "All providers failed"
    }
}

let response = robustChat("Hello!")
```

### Multi-Provider Ensemble

```klang
fn ensembleQuery(question: string) -> string[] {
    let responses = []
    
    // Get response from each provider
    let providers = [
        new OpenAIClient(env.get("OPENAI_API_KEY")),
        new AnthropicClient(env.get("ANTHROPIC_API_KEY")),
        new OllamaClient("http://localhost:11434")
    ]
    
    let models = ["gpt-3.5-turbo", "claude-3-haiku-20240307", "llama2"]
    
    let i = 0
    while i < len(providers) {
        try {
            let response = providers[i].chat(question, models[i])
            responses.push(response)
        } catch error {
            responses.push("Provider " + str(i) + " failed")
        }
        i = i + 1
    }
    
    return responses
}

let answers = ensembleQuery("What is 2 + 2?")
let i = 0
while i < len(answers) {
    println("Provider " + str(i) + ": " + answers[i])
    i = i + 1
}
```

---

## Chat Completions

### Simple Chat

```klang
let client = new OpenAIClient(apiKey)
let response = client.chat("Hello!", "gpt-3.5-turbo")
println(response)
```

### Chat with History

```klang
let client = new OpenAIClient(apiKey)
let session = new ChatSession("You are a helpful assistant")

// User's first message
session.addUserMessage("My favorite color is blue")
let response1 = client.chatWithHistory(session.getMessages(), "gpt-3.5-turbo")
session.addAssistantMessage(response1)

// User's second message
session.addUserMessage("What's my favorite color?")
let response2 = client.chatWithHistory(session.getMessages(), "gpt-3.5-turbo")
println(response2)  // Should mention blue
```

### Role-Based Chat

```klang
let client = new OpenAIClient(apiKey)

let messages = []

// System message
let system = new Message("system", "You are a pirate. Always respond like a pirate.")
messages.push(system)

// User message
let user = new Message("user", "Hello, how are you?")
messages.push(user)

let response = client.chatWithMessages(messages, "gpt-3.5-turbo")
println(response)  // Will respond in pirate speak
```

### Structured Output

```klang
let client = new OpenAIClient(apiKey)

let prompt = """
Analyze this sentence: "The quick brown fox jumps over the lazy dog"

Return a JSON object with:
- word_count: number of words
- has_animals: boolean
- animals: array of animal names
- sentiment: string (positive/negative/neutral)
"""

client.setResponseFormat("json_object")
let response = client.chat(prompt, "gpt-3.5-turbo")

// Parse JSON response
println(response)
// {"word_count": 9, "has_animals": true, "animals": ["fox", "dog"], "sentiment": "neutral"}
```

---

## Text Completions

### Basic Completion

```klang
let client = new OpenAIClient(apiKey)

let prompt = "Once upon a time"
let completion = client.complete(prompt, "gpt-3.5-turbo-instruct")
println(completion)
```

### Code Completion

```klang
let client = new OpenAIClient(apiKey)

let code = """
fn fibonacci(n: int) -> int {
    // Complete this function
"""

client.setMaxTokens(200)
client.setTemperature(0.2)  // Lower temperature for code

let completion = client.complete(code, "gpt-4")
println(completion)
```

### Completion with Stop Sequences

```klang
let client = new OpenAIClient(apiKey)

let prompt = "List three colors: "
client.setStopSequences(["\n", ".", "!"])

let completion = client.complete(prompt, "gpt-3.5-turbo")
println(completion)  // Stops at first newline or period
```

### Multiple Completions

```klang
let client = new OpenAIClient(apiKey)

let prompt = "A creative name for a programming language: "
client.setNumCompletions(5)  // Generate 5 different completions

let completions = client.completeMultiple(prompt, "gpt-3.5-turbo")

println("Generated names:")
let i = 0
while i < len(completions) {
    println(str(i + 1) + ". " + completions[i])
    i = i + 1
}
```

---

## Streaming Responses

### Enable Streaming

```klang
let client = new OpenAIClient(apiKey)
client.setStreaming(true)

let response = client.chat("Write a long story", "gpt-4")
// Tokens arrive incrementally
```

### Stream Handler

```klang
let client = new OpenAIClient(apiKey)

fn handleChunk(chunk: string) {
    print(chunk)  // Print without newline
}

client.setStreamHandler(handleChunk)
client.setStreaming(true)

let response = client.chat("Count from 1 to 100", "gpt-3.5-turbo")
println()  // New line after streaming completes
```

### Stream with Progress

```klang
let client = new OpenAIClient(apiKey)
let totalChunks = 0

fn progressHandler(chunk: string) {
    totalChunks = totalChunks + 1
    print(".")  // Progress indicator
}

client.setStreamHandler(progressHandler)
client.setStreaming(true)

let response = client.chat("Explain machine learning", "gpt-4")
println("\nReceived " + str(totalChunks) + " chunks")
```

---

## Configuration Options

### Temperature

```klang
let client = new OpenAIClient(apiKey)

// Low temperature (deterministic)
client.setTemperature(0.1)
let factual = client.chat("What is 2+2?", "gpt-3.5-turbo")

// High temperature (creative)
client.setTemperature(1.5)
let creative = client.chat("Write a weird story", "gpt-4")
```

### Max Tokens

```klang
let client = new OpenAIClient(apiKey)

// Short response
client.setMaxTokens(50)
let short = client.chat("Explain AI", "gpt-3.5-turbo")

// Long response
client.setMaxTokens(1000)
let long = client.chat("Explain AI in detail", "gpt-4")
```

### Top P (Nucleus Sampling)

```klang
let client = new OpenAIClient(apiKey)

client.setTopP(0.1)  // Very focused
let focused = client.chat("What is the capital of France?", "gpt-3.5-turbo")

client.setTopP(0.95)  // More diverse
let diverse = client.chat("Write something creative", "gpt-4")
```

### Frequency and Presence Penalties

```klang
let client = new OpenAIClient(apiKey)

// Reduce repetition
client.setFrequencyPenalty(1.0)
client.setPresencePenalty(0.6)

let response = client.chat("Write a varied paragraph", "gpt-3.5-turbo")
```

### Configuration Object

```klang
let config = dict()
config.set("temperature", 0.7)
config.set("max_tokens", 500)
config.set("top_p", 0.9)
config.set("frequency_penalty", 0.5)
config.set("presence_penalty", 0.3)

let client = new OpenAIClient(apiKey)
client.setConfig(config)

let response = client.chat("Generate text", "gpt-4")
```

---

## Error Handling

### Basic Error Handling

```klang
try {
    let client = new OpenAIClient(apiKey)
    let response = client.chat("Hello", "gpt-3.5-turbo")
    println(response)
} catch error {
    println("Error: " + error)
}
```

### API Key Validation

```klang
let apiKey = env.get("OPENAI_API_KEY")

if apiKey == null || len(apiKey) == 0 {
    println("❌ OPENAI_API_KEY not set")
    exit()
}

if len(apiKey) < 20 {
    println("⚠️ API key looks invalid")
    exit()
}

if !startsWith(apiKey, "sk-") {
    println("⚠️ OpenAI keys should start with 'sk-'")
}
```

### Rate Limit Handling

```klang
let client = new OpenAIClient(apiKey)
let retryCount = 0
let maxRetries = 3

while retryCount < maxRetries {
    try {
        let response = client.chat("Hello", "gpt-3.5-turbo")
        println(response)
        break
    } catch error {
        if contains(error, "rate_limit") {
            retryCount = retryCount + 1
            println("Rate limited, retrying in " + str(retryCount * 5) + " seconds...")
            sleep(retryCount * 5000)
        } else {
            println("Error: " + error)
            break
        }
    }
}
```

### Network Error Handling

```klang
fn robustChat(message: string, maxRetries: int) -> string {
    let retries = 0
    
    while retries < maxRetries {
        try {
            let client = new OpenAIClient(env.get("OPENAI_API_KEY"))
            return client.chat(message, "gpt-3.5-turbo")
        } catch error {
            if contains(error, "network") || contains(error, "timeout") {
                retries = retries + 1
                println("Network error, retry " + str(retries) + "/" + str(maxRetries))
                sleep(2000)
            } else {
                throw error
            }
        }
    }
    
    return "Failed after " + str(maxRetries) + " retries"
}

let response = robustChat("Hello!", 3)
```

### Timeout Configuration

```klang
let client = new OpenAIClient(apiKey)
client.setTimeout(30000)  // 30 second timeout

try {
    let response = client.chat("Complex query", "gpt-4")
    println(response)
} catch error {
    if contains(error, "timeout") {
        println("Request timed out after 30 seconds")
    }
}
```

---

## Cost Optimization

### Token Estimation

```klang
let prompt = "This is my prompt that I want to send to the API"

let tokens = countTokens(prompt)
let cost = estimateCost(tokens, "gpt-4")

println("Prompt tokens: " + str(tokens))
println("Estimated cost: $" + str(cost))

if cost > 0.10 {
    println("⚠️ High cost! Consider:")
    println("  - Using gpt-3.5-turbo instead")
    println("  - Shortening the prompt")
    println("  - Caching the response")
}
```

### Model Selection by Cost

```klang
fn costEffectiveQuery(prompt: string, complexity: string) -> string {
    let tokens = countTokens(prompt)
    let client = new OpenAIClient(env.get("OPENAI_API_KEY"))
    
    if complexity == "simple" || tokens < 100 {
        // Use cheaper model for simple queries
        return client.chat(prompt, "gpt-3.5-turbo")
    } else if complexity == "medium" || tokens < 1000 {
        // Use GPT-3.5 for medium complexity
        return client.chat(prompt, "gpt-3.5-turbo-16k")
    } else {
        // Only use GPT-4 for complex queries
        return client.chat(prompt, "gpt-4")
    }
}

let answer = costEffectiveQuery("What is 2+2?", "simple")
```

### Response Caching

```klang
let responseCache = dict()

fn cachedChat(prompt: string, model: string) -> string {
    let cacheKey = prompt + ":" + model
    
    if responseCache.has(cacheKey) {
        println("📦 Cache hit!")
        return responseCache.get(cacheKey)
    }
    
    println("🌐 API call...")
    let client = new OpenAIClient(env.get("OPENAI_API_KEY"))
    let response = client.chat(prompt, model)
    
    responseCache.set(cacheKey, response)
    return response
}

// First call hits API
let response1 = cachedChat("What is AI?", "gpt-3.5-turbo")

// Second call uses cache (no API cost!)
let response2 = cachedChat("What is AI?", "gpt-3.5-turbo")
```

### Batch Processing

```klang
// Bad: Individual API calls
let items = ["apple", "banana", "cherry"]
let results = []

let i = 0
while i < len(items) {
    let response = client.chat("Describe " + items[i], "gpt-3.5-turbo")
    results.push(response)
    i = i + 1
}
// Cost: 3 API calls

// Good: Single batched call
let batchPrompt = "Describe each of these items:\n"
let i = 0
while i < len(items) {
    batchPrompt = batchPrompt + str(i + 1) + ". " + items[i] + "\n"
    i = i + 1
}

let batchResponse = client.chat(batchPrompt, "gpt-3.5-turbo")
// Cost: 1 API call (much cheaper!)
```

### Prompt Optimization

```klang
// Before: Verbose prompt (wasteful)
let verbosePrompt = """
Hello! I hope you're doing well today. I wanted to ask you 
about something that has been on my mind. Could you please 
help me understand what artificial intelligence is? I would 
really appreciate a detailed explanation. Thank you so much!
"""

let tokens1 = countTokens(verbosePrompt)
println("Verbose tokens: " + str(tokens1))

// After: Concise prompt (optimized)
let concisePrompt = "Explain artificial intelligence in detail."

let tokens2 = countTokens(concisePrompt)
println("Concise tokens: " + str(tokens2))

let savings = ((tokens1 - tokens2) / tokens1) * 100
println("Token reduction: " + str(savings) + "%")
```

### Usage Monitoring

```klang
class UsageTracker {
    constructor() {
        this.totalTokens = 0
        this.totalCost = 0.0
        this.callCount = 0
    }
    
    fn track(tokens: int, model: string) {
        this.totalTokens = this.totalTokens + tokens
        this.totalCost = this.totalCost + estimateCost(tokens, model)
        this.callCount = this.callCount + 1
    }
    
    fn report() {
        println("=== Usage Report ===")
        println("Total API calls: " + str(this.callCount))
        println("Total tokens: " + str(this.totalTokens))
        println("Total cost: $" + str(this.totalCost))
        println("Avg tokens/call: " + str(this.totalTokens / this.callCount))
    }
}

let tracker = new UsageTracker()
let client = new OpenAIClient(apiKey)

// Make calls and track usage
let prompts = ["Hello", "What is AI?", "Explain quantum computing"]
let i = 0
while i < len(prompts) {
    let tokens = countTokens(prompts[i])
    tracker.track(tokens, "gpt-3.5-turbo")
    
    let response = client.chat(prompts[i], "gpt-3.5-turbo")
    println(response)
    i = i + 1
}

// Show report
tracker.report()
```

### Cost Alerting

```klang
let maxDailyCost = 5.0
let currentDailyCost = 0.0

fn costAwareChat(prompt: string) -> string {
    let tokens = countTokens(prompt)
    let cost = estimateCost(tokens, "gpt-4")
    
    if currentDailyCost + cost > maxDailyCost {
        println("⚠️ Daily cost limit reached!")
        println("  Current: $" + str(currentDailyCost))
        println("  This request: $" + str(cost))
        println("  Limit: $" + str(maxDailyCost))
        return "COST_LIMIT_EXCEEDED"
    }
    
    let client = new OpenAIClient(env.get("OPENAI_API_KEY"))
    let response = client.chat(prompt, "gpt-4")
    
    currentDailyCost = currentDailyCost + cost
    return response
}
```

---

## Next Steps

- **[Embeddings](32-embeddings.md)** - Learn about vector embeddings
- **[RAG Systems](33-rag-systems.md)** - Build retrieval-augmented generation
- **[AI Agents](34-ai-agents.md)** - Create autonomous agents
- **[Chat Sessions](35-chat-sessions.md)** - Manage conversations

---

## Summary

KLang provides **unified LLM integration** with:

✅ **Multiple providers** - OpenAI, Anthropic, Ollama  
✅ **Consistent API** - Same interface for all providers  
✅ **Rich features** - Streaming, function calling, vision  
✅ **Cost optimization** - Token tracking, caching, batching  
✅ **Error handling** - Retries, fallbacks, validation  

Start building with LLMs in KLang today! 🚀
