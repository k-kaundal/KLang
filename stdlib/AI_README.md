# KLang AI Standard Library

## Overview

The KLang AI Standard Library provides comprehensive AI capabilities implemented **100% in KLang**. These modules enable LLM integration, vector embeddings, RAG systems, prompt engineering, and autonomous agents—all native to KLang.

---

## Modules

### 🤖 ai.kl - Core AI Utilities
**Size:** 7,948 characters | **Lines:** 304

Foundation classes for AI functionality.

**Classes:**
- `AIConfig` - AI service configuration
- `Message` - Chat message representation  
- `ChatSession` - Conversation history manager
- `Embedding` - Vector embeddings with similarity operations
- `BatchProcessor` - Batch processing utility

**Functions:**
- `countTokens(text: string) -> int` - Estimate token count
- `estimateCost(tokens: int, model: string) -> float` - Calculate API cost

**Use Cases:**
- Configuration management
- Chat applications
- Vector similarity
- Cost estimation

---

### 💬 llm.kl - LLM Integration
**Size:** 11,893 characters | **Lines:** 370

Complete LLM provider support.

**Classes:**
- `LLMConfig` - Provider configuration (OpenAI, Anthropic, Ollama)
- `LLMClient` - Universal LLM client

**Functions:**
- `llm(prompt: string, apiKey: string) -> string` - Quick LLM query
- `llmStream(prompt: string, callback, apiKey: string)` - Streaming

**Supported Providers:**
- ✅ OpenAI (GPT-4, GPT-3.5-turbo)
- ✅ Anthropic (Claude 3 Opus/Sonnet/Haiku)
- ✅ Ollama (Llama 2, Mistral, local models)

**Use Cases:**
- Text completion
- Multi-turn chat
- Multiple LLM providers
- Local model inference

---

### 📊 embedding.kl - Vector Operations
**Size:** 7,024 characters | **Lines:** 259

Vector embeddings and semantic search.

**Classes:**
- `EmbeddingStore` - Vector storage and search
- `TextChunker` - Document chunking

**Functions:**
- `vectorAdd(vec1, vec2)` - Add vectors
- `vectorSubtract(vec1, vec2)` - Subtract vectors
- `vectorScale(vec, scalar: float)` - Scale vector
- `vectorAverage(vectors)` - Average multiple vectors
- `semanticSearch(query, docs, client, topK)` - End-to-end search

**Use Cases:**
- Semantic search
- Document similarity
- Clustering
- Recommendation systems

---

### 🎯 prompt.kl - Prompt Engineering
**Size:** 9,395 characters | **Lines:** 274

Advanced prompt construction and optimization.

**Classes:**
- `PromptTemplate` - Variable substitution templates
- `FewShotPrompt` - Few-shot learning prompts
- `ChainOfThoughtPrompt` - Step-by-step reasoning
- `RAGPrompt` - RAG-specific prompts
- `ReasoningPrompt` - Multi-step reasoning
- `SystemPrompts` - Pre-built system prompts

**Functions:**
- `optimizePrompt(prompt: string) -> string` - Remove excess whitespace
- `estimateTokens(prompt: string) -> int` - Token estimation
- `truncateToTokenLimit(prompt: string, maxTokens: int) -> string` - Fit to limit
- `combinePrompts(prompts, separator: string)` - Combine prompts
- `createRolePrompt(role: string, task: string) -> string` - Role-based
- `createConstrainedPrompt(task: string, constraints)` - With constraints
- `createFormattedPrompt(task: string, format: string) -> string` - Format spec

**Use Cases:**
- Dynamic prompt generation
- Few-shot learning
- Chain-of-thought reasoning
- Token optimization

---

### 🧠 rag.kl - RAG Systems
**Size:** 9,694 characters | **Lines:** 282

Complete Retrieval-Augmented Generation.

**Classes:**
- `Document` - Document with embeddings and metadata
- `VectorDB` - In-memory vector database
- `RAGSystem` - Complete RAG pipeline
- `SemanticCache` - Semantic query caching

**Functions:**
- `buildRAGFromFiles(llmClient, filePaths)` - Load documents from files

**Use Cases:**
- Question answering over documents
- Knowledge bases
- Context-aware chatbots
- Document search

---

### 🤝 agents.kl - Autonomous Agents
**Size:** 12,258 characters | **Lines:** 362

AI agents with memory and reasoning.

**Classes:**
- `AgentMemory` - Short/long-term memory and facts
- `AgentAction` - Action definition
- `Agent` - Base autonomous agent
- `ReActAgent` - Reasoning + Acting pattern
- `TaskAgent` - Task queue executor

**Use Cases:**
- Autonomous assistants
- Problem solving agents
- Multi-step task execution
- Agent systems

---

## Quick Start

### 1. Simple LLM Query
```klang
let apiKey = env.get("OPENAI_API_KEY")
let answer = llm("What is KLang?", apiKey)
println(answer)
```

### 2. Chat Application
```klang
let config = new LLMConfig("openai")
let client = new LLMClient(config)
let session = new ChatSession("You are helpful")

session.addUserMessage("Hi!")
let response = client.chat(session)
println(response)
```

### 3. RAG System
```klang
let rag = new RAGSystem(llmClient, 500, 3)
rag.ingestDocument("doc1", "KLang is...")
let answer = rag.query("What is KLang?")
```

### 4. Semantic Search
```klang
let store = new EmbeddingStore()
store.addText("doc", embedding)
let results = store.search(queryEmb, 5)
```

### 5. Autonomous Agent
```klang
let agent = new Agent("Bot", "helper", llmClient)
agent.addGoal("Help user")
let thought = agent.think("User needs help")
```

---

## Module Dependencies

```
agents.kl
  └─ Uses: ai.kl (Memory, Message)
  
rag.kl
  └─ Uses: ai.kl (Embedding), llm.kl (LLMClient)
  
llm.kl
  └─ Uses: ai.kl (Message, ChatSession, Embedding)
  └─ Requires: env.*, http.*
  
embedding.kl
  └─ Uses: ai.kl (Embedding)
  
prompt.kl
  └─ Standalone (no dependencies)
  
ai.kl
  └─ Standalone (core module)
```

**Recommendation:** Import in this order if using multiple modules:
1. ai.kl (core)
2. llm.kl (depends on ai)
3. embedding.kl (depends on ai)
4. prompt.kl (standalone)
5. rag.kl (depends on ai, llm)
6. agents.kl (depends on ai)

---

## Examples

See `examples/` directory:
- `ai_chatbot_simple.kl` - Basic chatbot
- `ai_rag_demo.kl` - RAG demonstration
- `ai_agent_demo.kl` - Agent examples
- `ai_prompt_engineering.kl` - Prompt patterns

Test:
- `test_ai_native.kl` - Comprehensive feature test

---

## Documentation

- [AI-Native Guide](../docs/AI_NATIVE_GUIDE.md) - Complete tutorial
- [Quick Reference](../docs/AI_QUICK_REFERENCE.md) - API cheat sheet
- [Implementation Details](../AI_NATIVE_IMPLEMENTATION.md) - Technical docs
- [Migration Guide](../docs/AI_MIGRATION_GUIDE.md) - From Python/JS

---

## Statistics

| Module | Size | Classes | Functions | Purpose |
|--------|------|---------|-----------|---------|
| ai.kl | 7,948 | 5 | 2 | Core utilities |
| llm.kl | 11,893 | 2 | 2 | LLM providers |
| embedding.kl | 7,024 | 2 | 5 | Vector ops |
| prompt.kl | 9,395 | 6 | 7 | Prompt engineering |
| rag.kl | 9,694 | 4 | 1 | RAG systems |
| agents.kl | 12,258 | 5 | 0 | Autonomous agents |
| **Total** | **58,212** | **24** | **17** | **Complete AI stack** |

---

## Implementation

**Language:** 100% KLang (no external libraries)  
**Dependencies:** Only core KLang runtime  
**C Bindings:** 390 lines (env, http)  
**Ratio:** 95.2% KLang, 4.8% C

---

## Testing

Run the comprehensive test suite:
```bash
./klang run test_ai_native.kl
```

All tests should pass:
- ✅ Environment variables
- ✅ HTTP API
- ✅ Module files exist
- ✅ Examples exist
- ✅ Documentation exists

---

## Contributing

Want to extend the AI stdlib?

1. **Add new classes** - Follow existing patterns
2. **Add new functions** - Pure KLang implementation
3. **Optimize algorithms** - Better performance
4. **Add examples** - Show use cases

See [Contributing Guide](../docs/development/CONTRIBUTING.md)

---

## Future Plans

### v0.9
- [ ] Streaming completions (token-by-token)
- [ ] Function calling / Tools API
- [ ] Enhanced JSON parsing
- [ ] Async/await integration

### v1.0
- [ ] Local model inference (llama.cpp)
- [ ] Image generation
- [ ] Speech-to-text
- [ ] Multi-modal AI
- [ ] GPU acceleration

---

## License

MIT License - Same as KLang core

---

## Credits

Designed and implemented by the KLang team.

Inspired by:
- OpenAI API patterns
- LangChain architecture
- AutoGPT agent systems

**Built with KLang, for KLang developers** 🚀

---

**Questions?** Open an issue or discussion on [GitHub](https://github.com/k-kaundal/KLang)
