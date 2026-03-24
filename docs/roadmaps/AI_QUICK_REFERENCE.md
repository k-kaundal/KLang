# KLang AI-Native Quick Reference

## 🚀 Quick Start

### Setup
```bash
# Set API key
export OPENAI_API_KEY="sk-..."

# Build KLang
make

# Run AI program
./klang run myai.kl
```

---

## 📦 Built-in Functions

### Environment Variables
```klang
env.get("VAR_NAME")           # Get variable (returns null if not exists)
env.set("VAR_NAME", "value")  # Set variable
env.has("VAR_NAME")           # Check if exists (returns true/false)
```

### HTTP Client
```klang
http.get("https://api.example.com")                    # GET request
http.post("https://api.example.com", body)             # POST request
http.request(configDict)                               # Advanced request with headers
```

---

## 🤖 AI Classes (stdlib)

### Core (ai.kl)
```klang
# Configuration
let config = new AIConfig(apiKey, "gpt-4")
config.setTemperature(0.7)
config.setMaxTokens(1000)

# Messages
let msg = new Message("user", "Hello")
msg.toDict()
msg.toString()

# Chat Session
let session = new ChatSession("You are helpful")
session.addUserMessage("Hi")
session.addAssistantMessage("Hello!")
session.getMessages()
session.toJSON()

# Embeddings
let emb = new Embedding([0.1, 0.2, 0.3], "text")
emb.cosineSimilarity(otherEmb)
emb.euclideanDistance(otherEmb)
emb.magnitude()
emb.normalize()
```

### LLM (llm.kl)
```klang
# Quick query
let answer = llm("What is AI?")

# Full client
let config = new LLMConfig("openai", apiKey)
config.setModel("gpt-4")

let client = new LLMClient(config)
let response = client.complete("Explain KLang")

# Chat
let answer = client.chat(session)

# Embeddings
let embedding = client.embed("some text")
```

### Embeddings (embedding.kl)
```klang
# Store
let store = new EmbeddingStore()
store.add(embedding, metadata)
store.addText("doc", embedding, metadata)
let results = store.search(queryEmb, topK=5)

# Chunking
let chunker = new TextChunker(chunkSize=500, overlap=50)
let chunks = chunker.chunk(text)

# Vector ops
vectorAdd(vec1, vec2)
vectorSubtract(vec1, vec2)
vectorScale(vec, 0.5)
vectorAverage([vec1, vec2, vec3])
```

### Prompts (prompt.kl)
```klang
# Template
let tmpl = new PromptTemplate("Write a {lang} {type}")
let prompt = tmpl.fill(valuesDict)

# Few-shot
let fs = new FewShotPrompt("Convert:")
fs.addExample("input", "output")
let prompt = fs.build("query")

# Chain-of-thought
let cot = new ChainOfThoughtPrompt("Problem")
cot.addStep("Step 1")
let prompt = cot.build()

# RAG prompt
let rag = new RAGPrompt("Question?")
rag.addContext("Context...", "source")
let prompt = rag.build()

# System prompts
SystemPrompts.helpful()
SystemPrompts.codeAssistant()
SystemPrompts.custom("role", "traits")

# Utils
optimizePrompt(prompt)
estimateTokens(prompt)
truncateToTokenLimit(prompt, 4000)
```

### RAG (rag.kl)
```klang
# Setup
let rag = new RAGSystem(client, chunkSize=500, topK=3)

# Ingest
rag.ingestDocument("id", "content", metadata)

# Query
let answer = rag.query("question")

# With citations
let result = rag.queryWithCitations("question")
result.get("answer")
result.get("sources")

# Semantic cache
let cache = new SemanticCache(0.95)
let cached = cache.get(queryEmb)
cache.set(queryEmb, response)

# From files
let rag = buildRAGFromFiles(client, ["/path/to/doc.txt"])
```

### Agents (agents.kl)
```klang
# Memory
let memory = new AgentMemory(maxShortTerm=10)
memory.storeFact("key", "value")
memory.remember("event", "data")
let fact = memory.retrieveFact("key")
let recent = memory.getRecentMemories(5)

# Actions
let action = new AgentAction("name", "description", handlerFn)
action.execute(args)

# Agent
let agent = new Agent("name", "role", llmClient)
agent.registerAction(action)
agent.addGoal("goal")
let thought = agent.think("observation")
let result = agent.act("actionName", args)
agent.run("initial observation", maxSteps=10)

# ReAct
let react = new ReActAgent("name", "role", llmClient)
react.registerAction(action)
let solution = react.solve("problem")

# Task agent
let taskAgent = new TaskAgent("name", llmClient)
taskAgent.addTask("task 1")
taskAgent.executeTasks()
let status = taskAgent.getStatus()
```

---

## 🎯 Common Patterns

### Pattern 1: Simple Query
```klang
let apiKey = env.get("OPENAI_API_KEY")
let answer = llm("What is 2+2?", apiKey)
println(answer)
```

### Pattern 2: Chatbot
```klang
let config = new LLMConfig("openai")
let client = new LLMClient(config)
let session = new ChatSession("You are helpful")

session.addUserMessage("Hi!")
let response = client.chat(session)
println(response)
```

### Pattern 3: RAG System
```klang
let client = new LLMClient(config)
let rag = new RAGSystem(client, 500, 3)

rag.ingestDocument("doc1", "KLang is...")
let answer = rag.query("What is KLang?")
println(answer)
```

### Pattern 4: Semantic Search
```klang
let store = new EmbeddingStore()
store.addText("doc1", emb1)
store.addText("doc2", emb2)

let results = store.search(queryEmb, 5)
```

### Pattern 5: Prompt Engineering
```klang
let template = new PromptTemplate("Write {lang} code for {task}")
let values = dict()
values.set("lang", "KLang")
values.set("task", "sorting")
let prompt = template.fill(values)
```

### Pattern 6: Autonomous Agent
```klang
let agent = new Agent("Assistant", "helper", client)
agent.addGoal("Complete task")

let searchAction = new AgentAction("search", "Search info", searchFn)
agent.registerAction(searchAction)

agent.run("User needs help", 5)
```

---

## 📊 Providers

| Provider | Config | Models |
|----------|--------|--------|
| OpenAI | `new LLMConfig("openai", key)` | gpt-4, gpt-3.5-turbo |
| Anthropic | `new LLMConfig("anthropic", key)` | claude-3-opus, claude-3-sonnet |
| Ollama | `new LLMConfig("ollama")` | llama2, mistral, etc. |

---

## 🔐 Security

```klang
# ✅ Good - Use environment variables
let apiKey = env.get("OPENAI_API_KEY")

# ❌ Bad - Hardcode keys
let apiKey = "sk-..."  # NEVER DO THIS
```

---

## 💰 Cost Management

```klang
let tokens = countTokens(prompt)
let cost = estimateCost(tokens, "gpt-4")
println("Cost: $" + str(cost))

if tokens > 4000 {
    prompt = truncateToTokenLimit(prompt, 4000)
}
```

---

## 📚 Import Modules

```klang
# When module system is ready:
import {AIConfig, Message, ChatSession, Embedding} from "./stdlib/ai.kl"
import {LLMClient, LLMConfig} from "./stdlib/llm.kl"
import {EmbeddingStore, TextChunker} from "./stdlib/embedding.kl"
import {PromptTemplate, FewShotPrompt} from "./stdlib/prompt.kl"
import {RAGSystem, VectorDB} from "./stdlib/rag.kl"
import {Agent, ReActAgent, AgentMemory} from "./stdlib/agents.kl"
```

---

## 🎓 Learning Path

1. **Basics** → Environment variables + HTTP
2. **LLM** → Simple completions with `llm()`
3. **Chat** → Multi-turn conversations
4. **Embeddings** → Similarity and search
5. **RAG** → Document Q&A systems
6. **Prompts** → Advanced prompt engineering
7. **Agents** → Autonomous systems

---

## 📖 Full Documentation

- [AI-Native Guide](docs/AI_NATIVE_GUIDE.md) - Complete guide
- [Implementation Details](AI_NATIVE_IMPLEMENTATION.md) - Technical docs
- [Examples](examples/) - Working examples
- [stdlib README](stdlib/README.md) - Module reference

---

**KLang: The first AI-Native Core Language** 🚀
