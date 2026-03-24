# KLang AI-Native Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                         KLang Application                           │
│                    (Your AI-powered program)                        │
└─────────────────────────────────────────────────────────────────────┘
                                  │
                                  ↓
┌─────────────────────────────────────────────────────────────────────┐
│                   Pure KLang AI Standard Library                    │
│  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━  │
│                                                                     │
│  📦 ai.kl (7.9KB)          📦 llm.kl (11.9KB)                      │
│     • AIConfig                 • LLMConfig                          │
│     • Message                  • LLMClient                          │
│     • ChatSession             • llm() function                      │
│     • Embedding               • Provider support                    │
│     • BatchProcessor          • OpenAI, Anthropic, Ollama          │
│                                                                     │
│  📦 embedding.kl (7.1KB)   📦 prompt.kl (9.4KB)                    │
│     • EmbeddingStore          • PromptTemplate                      │
│     • TextChunker             • FewShotPrompt                       │
│     • Vector operations       • ChainOfThought                      │
│     • Semantic search         • RAGPrompt                           │
│                               • SystemPrompts                       │
│                                                                     │
│  📦 rag.kl (9.8KB)         📦 agents.kl (12.4KB)                   │
│     • Document                • AgentMemory                         │
│     • VectorDB                • AgentAction                         │
│     • RAGSystem               • Agent                               │
│     • SemanticCache           • ReActAgent                          │
│                               • TaskAgent                           │
│                                                                     │
│  Total: 58KB+ of Pure KLang Code (95.2% of implementation)        │
└─────────────────────────────────────────────────────────────────────┘
                                  │
                                  ↓
┌─────────────────────────────────────────────────────────────────────┐
│                KLang Runtime - Built-in Functions                   │
│  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━  │
│                                                                     │
│  🌍 env object          🌐 http object        📄 JSON (planned)    │
│     • env.get()            • http.get()         • parseStream()    │
│     • env.set()            • http.post()                           │
│     • env.has()            • http.request()                        │
│                                                                     │
│  Implementation: 390 lines of C (4.8% of implementation)           │
└─────────────────────────────────────────────────────────────────────┘
                                  │
                                  ↓
┌─────────────────────────────────────────────────────────────────────┐
│                        System Dependencies                          │
│  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━  │
│                                                                     │
│  • curl - HTTP client                                              │
│  • stdlib.h - Environment variables (getenv, setenv)              │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
                                  │
                                  ↓
┌─────────────────────────────────────────────────────────────────────┐
│                         External AI APIs                            │
│  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━  │
│                                                                     │
│  🤖 OpenAI API          🤖 Anthropic API      🤖 Ollama (Local)    │
│     api.openai.com         api.anthropic.com     localhost:11434   │
│     • GPT-4                • Claude 3 Opus       • Llama 2         │
│     • GPT-3.5-turbo        • Claude 3 Sonnet     • Mistral         │
│     • Embeddings           • Claude 3 Haiku      • Custom models   │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

---

## Component Details

### Layer 1: Application (User Code)
- Written in KLang
- Uses AI stdlib classes and functions
- Makes LLM calls, builds RAG systems, creates agents
- No external dependencies needed

### Layer 2: AI Standard Library (Pure KLang)
- **24 classes** providing complete AI toolkit
- **17 utility functions** for common tasks
- 100% readable, modifiable KLang code
- No compiled dependencies
- Users can extend or customize

### Layer 3: Runtime Bindings (Minimal C)
- **3 built-in objects** (env, http, JSON)
- Thin wrappers around system calls
- Only 390 lines of C code
- Provides primitives for stdlib

### Layer 4: System Dependencies
- curl for HTTP (widely available)
- Standard C library for env vars
- No exotic dependencies

### Layer 5: External APIs
- OpenAI, Anthropic, Ollama
- Standard REST APIs
- User controls which provider to use

---

## Data Flow Examples

### Example 1: Simple LLM Query

```
User Code (KLang)
   ↓
   llm("What is AI?", apiKey)
   ↓
LLM Client (llm.kl - Pure KLang)
   ↓
   client.complete(prompt)
   ↓
HTTP Request (runtime - C binding)
   ↓
   http.post(url, jsonBody)
   ↓
curl (system)
   ↓
OpenAI API (external)
   ↓
   Response JSON
   ↓
Extract content (llm.kl - Pure KLang)
   ↓
Return to user
```

### Example 2: RAG Query

```
User Code
   ↓
   rag.query("question")
   ↓
RAG System (rag.kl - Pure KLang)
   ↓
1. Generate query embedding → LLMClient.embed()
2. Search vector DB → VectorDB.search()
3. Build prompt with context → RAGPrompt.build()
4. Generate answer → LLMClient.complete()
   ↓
Return answer
```

### Example 3: Agent Reasoning

```
User Code
   ↓
   agent.think("observation")
   ↓
Agent (agents.kl - Pure KLang)
   ↓
1. Load memory → AgentMemory.getRecentMemories()
2. Build reasoning prompt → Construct with goals + actions
3. Get LLM reasoning → LLMClient.complete()
4. Store thought → AgentMemory.remember()
   ↓
Return thought
```

---

## Design Decisions

### Why 95% KLang, 5% C?

**Advantages:**
- ✅ Users can read and understand the full AI stack
- ✅ Users can modify and extend AI features
- ✅ No dependency hell (no npm/pip)
- ✅ Educational value - learn AI by reading KLang
- ✅ Debugging is easier - step through KLang code
- ✅ Version control - AI features evolve with language

**What's in C:**
- Only system-level primitives (HTTP, env vars)
- Things that need OS-level access
- Performance-critical operations

**What's in KLang:**
- All AI logic and algorithms
- LLM provider integrations
- Vector operations
- RAG systems
- Agent reasoning
- Prompt engineering

### Why Not Use libcurl Directly?

**Current:** Using `popen` + `curl` command  
**Reason:** 
- Simpler implementation (no library linking)
- curl is universally available
- Easy to debug (can see exact curl command)
- Can upgrade to libcurl later if needed

**Trade-off:** Slightly slower (process overhead) but more portable

---

## Performance Characteristics

### Environment Variables
- **O(1)** - Direct system call
- No allocation overhead
- Instant access

### HTTP Requests
- **O(n)** - Network latency + response size
- Uses system curl (optimized)
- Response buffering in memory

### Vector Similarity
- **O(d)** - d = embedding dimension
- Pure KLang math operations
- No external library needed

### Semantic Search
- **O(n·d + n log n)** - n = docs, d = dimension
- Linear similarity calculation
- Sorting for top-K

### RAG Query
- **O(chunks·d + k·d + LLM_latency)**
- Chunk embedding + search + generation
- Dominated by LLM API latency

---

## Security Model

### API Keys
- ✅ Stored in environment variables (not code)
- ✅ Never logged or printed
- ✅ Access controlled via env.get()
- ✅ Cross-platform secure storage

### HTTP Requests
- ✅ HTTPS supported (curl handles SSL)
- ✅ Headers for authentication
- ✅ Request validation
- ⚠️ User responsible for input sanitization

### Memory Safety
- ✅ KLang GC handles memory
- ✅ No buffer overflows in KLang code
- ✅ C bindings use safe string operations

---

## Extensibility

### Adding New LLM Providers

1. Extend `LLMClient` class in `llm.kl`
2. Add provider-specific method
3. Update config detection
4. No C code changes needed!

```klang
# In llm.kl:
private fn completeNewProvider(prompt: string) -> string {
    let url = this.config.baseURL + "/endpoint"
    # ... build request ...
    let response = http.request(config)
    return this.extractContent(response)
}
```

### Adding New Agent Actions

```klang
# User code:
let customAction = new AgentAction(
    "custom_action",
    "Does something custom",
    fn(args) {
        # Your logic here
        return result
    }
)

agent.registerAction(customAction)
```

### Adding New Prompt Types

```klang
# Create new prompt class in prompt.kl:
class CustomPrompt {
    fn init(params) { ... }
    public fn build() -> string { ... }
}
```

---

## Comparison Matrix

| Feature | Python + LangChain | JavaScript + LangChain | **KLang AI-Native** |
|---------|-------------------|------------------------|---------------------|
| **Setup** | pip install | npm install | **Built-in** ✅ |
| **Dependencies** | 10+ packages | 8+ packages | **0 packages** ✅ |
| **Code** | External libs | External libs | **Native** ✅ |
| **Readable** | Partial | Partial | **100%** ✅ |
| **Customizable** | Limited | Limited | **Fully** ✅ |
| **Size** | 50MB+ | 100MB+ | **<500KB** ✅ |
| **Language** | Python | JavaScript | **KLang** ✅ |
| **Implementation** | C/Python | JS/C++ | **95% KLang** ✅ |

---

## Future Architecture

### v0.9 - Enhancements
```
+ Function Calling API
+ Streaming responses (token-by-token)
+ Enhanced JSON parser
+ Async/await integration
```

### v1.0 - Local Inference
```
+ ONNX runtime integration
+ llama.cpp bindings
+ Local model loading
+ GPU acceleration (CUDA/Metal)
```

### v2.0 - Advanced Features
```
+ Multi-modal (image, audio, video)
+ Fine-tuning utilities
+ Model quantization
+ Distributed agents
```

---

## Conclusion

KLang's AI-native architecture is:
- ✅ **Clean** - Clear separation of concerns
- ✅ **Modular** - 6 independent stdlib modules
- ✅ **Extensible** - Easy to add providers, agents, prompts
- ✅ **Portable** - Minimal system dependencies
- ✅ **Transparent** - 95% readable KLang code
- ✅ **Production-ready** - Tested and working

**This architecture makes KLang unique** - AI is not bolted on, it's built in from the ground up, using KLang itself.

---

**Last Updated:** March 23, 2026  
**Version:** 0.8.0  
**Status:** Production Ready 🚀
