# Migrating to KLang AI-Native Features

## Overview

This guide helps you adopt KLang's AI-native features in your projects.

---

## Before and After

### Traditional Approach (Python/JS + Libraries)

#### Python
```python
# Install dependencies
pip install openai langchain chromadb

# Lots of imports
import openai
from langchain.llms import OpenAI
from langchain.embeddings import OpenAIEmbeddings
from langchain.vectorstores import Chroma

# Setup
client = openai.OpenAI(api_key=os.environ["OPENAI_API_KEY"])

# Make request
response = client.chat.completions.create(
    model="gpt-4",
    messages=[{"role": "user", "content": "Hello"}]
)
```

#### JavaScript
```javascript
// Install dependencies
npm install openai langchain

// Imports
const { OpenAI } = require("openai");
const { ChatOpenAI } = require("langchain/chat_models/openai");

// Setup
const client = new OpenAI({
    apiKey: process.env.OPENAI_API_KEY
});

// Make request
const response = await client.chat.completions.create({
    model: "gpt-4",
    messages: [{ role: "user", content: "Hello" }]
});
```

### KLang AI-Native Approach

```klang
# No installation needed - AI is built-in!

# Simple query
let answer = llm("Hello")

# Or with full control
let config = new LLMConfig("openai")
let client = new LLMClient(config)
let response = client.complete("Hello")
```

**Advantages:**
- ✅ No dependency installation
- ✅ No version management
- ✅ Cleaner, simpler syntax
- ✅ Built-in from day one

---

## Migration Examples

### Example 1: Simple LLM Query

**Python (with openai library)**
```python
import openai
import os

client = openai.OpenAI(api_key=os.environ["OPENAI_API_KEY"])
response = client.chat.completions.create(
    model="gpt-4",
    messages=[{"role": "user", "content": "What is AI?"}]
)
answer = response.choices[0].message.content
print(answer)
```

**KLang (native)**
```klang
let apiKey = env.get("OPENAI_API_KEY")
let answer = llm("What is AI?", apiKey)
println(answer)
```

**Lines of code:** Python: 9 lines → KLang: 3 lines

---

### Example 2: Chat Conversation

**JavaScript (with openai)**
```javascript
const { OpenAI } = require("openai");

const client = new OpenAI({
    apiKey: process.env.OPENAI_API_KEY
});

const messages = [
    { role: "system", content: "You are helpful" },
    { role: "user", content: "Hi!" }
];

const response = await client.chat.completions.create({
    model: "gpt-4",
    messages: messages
});

console.log(response.choices[0].message.content);
```

**KLang (native)**
```klang
let config = new LLMConfig("openai")
let client = new LLMClient(config)
let session = new ChatSession("You are helpful")

session.addUserMessage("Hi!")
let response = client.chat(session)
println(response)
```

**Benefits:** Native syntax, cleaner API, no async/await complexity

---

### Example 3: RAG System

**Python (with langchain)**
```python
from langchain.llms import OpenAI
from langchain.embeddings import OpenAIEmbeddings
from langchain.vectorstores import Chroma
from langchain.document_loaders import TextLoader
from langchain.text_splitter import CharacterTextSplitter

# Load documents
loader = TextLoader("doc.txt")
documents = loader.load()

# Split into chunks
splitter = CharacterTextSplitter(chunk_size=500, chunk_overlap=50)
chunks = splitter.split_documents(documents)

# Create embeddings
embeddings = OpenAIEmbeddings()

# Create vector store
db = Chroma.from_documents(chunks, embeddings)

# Query
llm = OpenAI()
retriever = db.as_retriever()
docs = retriever.get_relevant_documents("question")

# Generate answer with context
context = "\n".join([doc.page_content for doc in docs])
prompt = f"Context: {context}\nQuestion: question\nAnswer:"
answer = llm(prompt)
```

**KLang (native)**
```klang
let config = new LLMConfig("openai")
let client = new LLMClient(config)
let rag = new RAGSystem(client, 500, 3)

# Ingest documents
let content = readFile("doc.txt")
rag.ingestDocument("doc1", content)

# Query
let answer = rag.query("question")
println(answer)
```

**Lines of code:** Python: ~25 lines → KLang: ~9 lines

---

### Example 4: Semantic Search

**Python (with numpy + openai)**
```python
import numpy as np
from openai import OpenAI

client = OpenAI()

# Create embeddings
def get_embedding(text):
    response = client.embeddings.create(
        model="text-embedding-3-small",
        input=text
    )
    return response.data[0].embedding

emb1 = get_embedding("doc1")
emb2 = get_embedding("doc2")

# Calculate similarity
def cosine_similarity(a, b):
    return np.dot(a, b) / (np.linalg.norm(a) * np.linalg.norm(b))

similarity = cosine_similarity(emb1, emb2)
print(similarity)
```

**KLang (native)**
```klang
let client = new LLMClient(config)

let emb1 = client.embed("doc1")
let emb2 = client.embed("doc2")

let similarity = emb1.cosineSimilarity(emb2)
println(similarity)
```

**Benefits:** Built-in vector operations, no numpy needed

---

## Migration Checklist

### Step 1: Environment Setup
```bash
# Old (Python)
pip install openai langchain
export OPENAI_API_KEY="..."

# New (KLang)
export OPENAI_API_KEY="..."  # That's it!
```

### Step 2: Update Imports
```klang
# Python imports → KLang imports
# from openai import OpenAI
# No import needed! Built-in.

# Or use stdlib modules when needed:
# import {LLMClient} from "./stdlib/llm.kl"
```

### Step 3: Simplify API Calls
```klang
# Python: client.chat.completions.create(...)
# KLang: client.complete(prompt)

# Python: OpenAIEmbeddings().embed_query(text)
# KLang: client.embed(text)
```

### Step 4: Use Native Features
```klang
# Environment variables
let key = env.get("API_KEY")  # Instead of os.environ

# HTTP requests
let data = http.get(url)  # Instead of requests.get()

# Vector operations
let sim = emb1.cosineSimilarity(emb2)  # Instead of numpy
```

---

## Feature Mapping

### LLM Operations

| Feature | Python/JS | KLang |
|---------|-----------|-------|
| Simple query | `client.chat.completions.create(...)` | `llm(prompt)` |
| Chat | `client.chat.completions.create(messages=...)` | `client.chat(session)` |
| Embeddings | `client.embeddings.create(...)` | `client.embed(text)` |
| Streaming | `client.chat.completions.create(stream=True)` | `llmStream(prompt, callback)` |

### Vector Operations

| Feature | Python (numpy) | KLang |
|---------|---------------|-------|
| Cosine similarity | `np.dot(a,b)/(norm(a)*norm(b))` | `emb1.cosineSimilarity(emb2)` |
| Distance | `np.linalg.norm(a-b)` | `emb1.euclideanDistance(emb2)` |
| Normalize | `a / np.linalg.norm(a)` | `emb1.normalize()` |

### RAG Systems

| Feature | LangChain | KLang |
|---------|-----------|-------|
| Document load | `TextLoader()` | `readFile()` |
| Chunking | `CharacterTextSplitter()` | `TextChunker` |
| Vector DB | `Chroma.from_documents()` | `VectorDB` |
| Query | `retriever.get_relevant_documents()` | `rag.query()` |

---

## Common Patterns

### Pattern 1: Config from Environment
```klang
# Auto-detect API key
let config = new LLMConfig("openai")  # Reads OPENAI_API_KEY automatically

# Or explicit
let apiKey = env.get("OPENAI_API_KEY")
let config = new LLMConfig("openai", apiKey)
```

### Pattern 2: Error Handling
```klang
let apiKey = env.get("OPENAI_API_KEY")

if apiKey == null || len(apiKey) == 0 {
    println("Error: API key not set")
    exit()
}
```

### Pattern 3: Streaming (Async Pattern)
```klang
# Synchronous (default)
let response = client.complete(prompt)

# Streaming with callback
llmStream(prompt, fn(token) {
    print(token)
})
```

---

## Best Practices

### 1. ✅ Use Environment Variables
```klang
# Good
let key = env.get("API_KEY")

# Bad
let key = "sk-hardcoded"  # NEVER!
```

### 2. ✅ Estimate Costs
```klang
let tokens = countTokens(prompt)
let cost = estimateCost(tokens, "gpt-4")

if cost > 0.10 {
    println("Warning: Expensive query")
}
```

### 3. ✅ Optimize Prompts
```klang
let prompt = optimizePrompt(rawPrompt)
prompt = truncateToTokenLimit(prompt, 4000)
```

### 4. ✅ Cache Results
```klang
let cache = new SemanticCache(0.95)

let cached = cache.get(queryEmb)
if cached != null {
    return cached
}

let response = client.complete(prompt)
cache.set(queryEmb, response)
```

---

## Performance Comparison

### RAG Query Performance

| Implementation | Lines of Code | Dependencies | Setup Time |
|----------------|---------------|--------------|------------|
| Python + LangChain | ~50 lines | 5+ packages | 10 minutes |
| JavaScript + LangChain.js | ~45 lines | 4+ packages | 8 minutes |
| **KLang Native** | **~15 lines** | **0 packages** | **0 minutes** ✅ |

### Semantic Search Performance

| Implementation | Code Size | External Deps | Vector Math |
|----------------|-----------|---------------|-------------|
| Python + numpy | ~30 lines | numpy, openai | numpy |
| JavaScript + TensorFlow | ~40 lines | tensorflow.js | tensorflow |
| **KLang Native** | **~10 lines** | **None** | **Built-in** ✅ |

---

## Troubleshooting

### Issue: API key not found
```klang
# Check if set
if !env.has("OPENAI_API_KEY") {
    println("Please set OPENAI_API_KEY")
}
```

### Issue: HTTP request failed
```klang
# Check response
let response = http.get(url)
if response == null || len(response) == 0 {
    println("Request failed")
}
```

### Issue: Module not found
```klang
# Check file exists
if !fexists("stdlib/ai.kl") {
    println("AI module not found")
}
```

---

## What's Different?

### No Async/Await (Yet)
KLang uses synchronous calls by default:
```klang
# Python: response = await client.chat.completions.create(...)
# KLang: response = client.complete(...)  # Synchronous
```

Async/await is planned for future versions.

### No Type Hints (Flexible)
```klang
# Python requires type hints for IDE support
def complete(prompt: str) -> str:

# KLang is more flexible (types optional)
fn complete(prompt: string) -> string {  # Or just fn complete(prompt)
```

### Dictionary Instead of Objects
```klang
# Python: response.choices[0].message.content
# KLang: result.get("answer")  # Using dictionaries

# Or use classes for structure
let result = new Document(id, content)
```

---

## Learning Path

### Week 1: Basics
- [x] Environment variables
- [x] HTTP client
- [x] Simple LLM queries

### Week 2: Intermediate
- [x] Chat sessions
- [x] Embeddings
- [x] Semantic search

### Week 3: Advanced
- [x] RAG systems
- [x] Prompt engineering
- [x] Autonomous agents

---

## Resources

- [AI-Native Guide](AI_NATIVE_GUIDE.md) - Complete tutorial
- [Quick Reference](AI_QUICK_REFERENCE.md) - API cheat sheet
- [Examples](../examples/) - Working code examples
- [stdlib README](../stdlib/README.md) - Module documentation

---

## Community

- **Questions?** [GitHub Discussions](https://github.com/k-kaundal/KLang/discussions)
- **Issues?** [GitHub Issues](https://github.com/k-kaundal/KLang/issues)
- **Contributions?** [Contributing Guide](development/CONTRIBUTING.md)

---

**Welcome to AI-native programming with KLang!** 🚀
