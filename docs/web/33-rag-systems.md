# RAG Systems - Retrieval-Augmented Generation

Retrieval-Augmented Generation (RAG) combines the power of semantic search with large language models to create intelligent question-answering systems that can reason over your own documents. This guide covers building production-ready RAG systems in KLang.

## Table of Contents

1. [What is RAG](#what-is-rag)
2. [RAG Architecture](#rag-architecture)
3. [Document Processing](#document-processing)
4. [Text Chunking](#text-chunking)
5. [Building Knowledge Bases](#building-knowledge-bases)
6. [Semantic Retrieval](#semantic-retrieval)
7. [RAG Prompts](#rag-prompts)
8. [Complete RAG Pipeline](#complete-rag-pipeline)
9. [Advanced Techniques](#advanced-techniques)
10. [Production Deployment](#production-deployment)
11. [Performance Optimization](#performance-optimization)

---

## What is RAG

### The Problem

LLMs have limited knowledge:

```klang
// LLM doesn't know your private data
let response = client.chat("What's in our Q3 2024 financial report?")
// Response: "I don't have access to your financial reports"
```

### The Solution: RAG

```klang
// 1. Find relevant information
let query = "What's in our Q3 2024 financial report?"
let relevantDocs = search(query, knowledge_base)

// 2. Add context to prompt
let prompt = "Based on: " + relevantDocs + "\n\nAnswer: " + query

// 3. Generate informed answer
let response = client.chat(prompt)
// Response: "According to the Q3 report, revenue increased 25%..."
```

### How RAG Works

```
┌──────────────────────────────────────────────────┐
│ 1. Document Ingestion                            │
│    Documents → Chunks → Embeddings → Vector DB   │
└──────────────────────────────────────────────────┘
                    ↓
┌──────────────────────────────────────────────────┐
│ 2. Query Processing                              │
│    User Query → Embedding → Semantic Search      │
└──────────────────────────────────────────────────┘
                    ↓
┌──────────────────────────────────────────────────┐
│ 3. Retrieval                                     │
│    Top K Most Relevant Chunks                    │
└──────────────────────────────────────────────────┘
                    ↓
┌──────────────────────────────────────────────────┐
│ 4. Augmentation                                  │
│    Build Prompt with Context + Query             │
└──────────────────────────────────────────────────┘
                    ↓
┌──────────────────────────────────────────────────┐
│ 5. Generation                                    │
│    LLM Generates Answer Based on Context         │
└──────────────────────────────────────────────────┘
```

### Benefits of RAG

```klang
// ✅ Access to private/recent data
// ✅ Reduces hallucinations
// ✅ Traceable sources
// ✅ Cost-effective (vs fine-tuning)
// ✅ Easy to update knowledge
```

---

## RAG Architecture

### Basic RAG System

```klang
class BasicRAG {
    constructor(apiKey: string) {
        this.client = new OpenAIClient(apiKey)
        this.store = new EmbeddingStore()
        this.chunker = new TextChunker(100, 20)
    }
    
    fn ingest(document: string, source: string) {
        // Chunk document
        let chunks = this.chunker.chunk(document)
        
        // Generate embeddings
        let embeddings = this.client.embedBatch(chunks, "text-embedding-ada-002")
        
        // Store in vector DB
        let i = 0
        while i < len(chunks) {
            let meta = dict()
            meta.set("source", source)
            meta.set("chunk_id", i)
            
            this.store.addText(chunks[i], embeddings[i], meta)
            i = i + 1
        }
    }
    
    fn query(question: string, k: int) -> string {
        // Embed query
        let queryEmb = this.client.embed(question)
        
        // Retrieve relevant chunks
        let results = this.store.search(queryEmb, k)
        
        // Build RAG prompt
        let ragPrompt = new RAGPrompt(question, "Answer based on the context:")
        let i = 0
        while i < len(results) {
            ragPrompt.addContext(results[i].text, results[i].metadata.get("source"))
            i = i + 1
        }
        
        // Generate answer
        return this.client.chat(ragPrompt.build(), "gpt-3.5-turbo")
    }
}

// Usage
let rag = new BasicRAG(env.get("OPENAI_API_KEY"))
rag.ingest(fread("docs.txt"), "docs.txt")
let answer = rag.query("What is KLang?", 3)
println(answer)
```

### Component Breakdown

```klang
// 1. Document Loader
fn loadDocuments(paths: string[]) -> dict[] {
    let documents = []
    let i = 0
    while i < len(paths) {
        let content = fread(paths[i])
        let doc = dict()
        doc.set("content", content)
        doc.set("source", paths[i])
        doc.set("timestamp", timestamp())
        documents.push(doc)
        i = i + 1
    }
    return documents
}

// 2. Chunker
let chunker = new TextChunker(wordSize: 100, overlap: 20)

// 3. Embedder
let client = new OpenAIClient(apiKey)

// 4. Vector Store
let store = new EmbeddingStore()

// 5. Retriever
fn retrieve(query: string, k: int) -> SearchResult[] {
    let queryEmb = client.embed(query)
    return store.search(queryEmb, k)
}

// 6. Generator
fn generate(query: string, context: string) -> string {
    let prompt = "Context: " + context + "\n\nQuestion: " + query
    return client.chat(prompt, "gpt-3.5-turbo")
}
```

---

## Document Processing

### Loading Documents

```klang
// Single file
let doc = fread("knowledge.txt")

// Multiple files
let files = ["doc1.txt", "doc2.txt", "doc3.txt"]
let documents = []

let i = 0
while i < len(files) {
    documents.push(fread(files[i]))
    i = i + 1
}
```

### Document Metadata

```klang
fn loadDocumentWithMetadata(path: string) -> dict {
    let doc = dict()
    doc.set("content", fread(path))
    doc.set("path", path)
    doc.set("filename", basename(path))
    doc.set("size", fsize(path))
    doc.set("timestamp", timestamp())
    doc.set("type", getFileExtension(path))
    
    return doc
}

let doc = loadDocumentWithMetadata("important.txt")
println("Loaded: " + doc.get("filename"))
println("Size: " + str(doc.get("size")) + " bytes")
```

### Document Preprocessing

```klang
fn preprocessDocument(text: string) -> string {
    // Remove excessive whitespace
    let cleaned = text
    cleaned = replaceAll(cleaned, "\\s+", " ")
    
    // Remove special characters (optional)
    // cleaned = replaceAll(cleaned, "[^a-zA-Z0-9\\s]", "")
    
    // Normalize line breaks
    cleaned = replaceAll(cleaned, "\\r\\n", "\\n")
    
    return trim(cleaned)
}

let raw = fread("messy_doc.txt")
let clean = preprocessDocument(raw)
```

### Multi-Format Support

```klang
fn loadDocument(path: string) -> string {
    let ext = getFileExtension(path)
    
    if ext == "txt" {
        return fread(path)
    } else if ext == "md" {
        return fread(path)  // Markdown is text
    } else if ext == "json" {
        // Parse JSON and extract text fields
        let json = parseJSON(fread(path))
        return json.get("content")
    } else if ext == "html" {
        // Strip HTML tags (simplified)
        let html = fread(path)
        return stripHTMLTags(html)
    } else {
        println("Unsupported format: " + ext)
        return ""
    }
}
```

### Batch Document Loading

```klang
fn loadAllDocuments(directory: string) -> dict[] {
    let files = listFiles(directory)
    let documents = []
    
    println("Loading " + str(len(files)) + " documents...")
    
    let i = 0
    while i < len(files) {
        let path = directory + "/" + files[i]
        
        if isFile(path) {
            let doc = dict()
            doc.set("content", fread(path))
            doc.set("source", path)
            doc.set("filename", files[i])
            
            documents.push(doc)
            
            if i % 10 == 0 {
                println("  Loaded " + str(i) + " files...")
            }
        }
        
        i = i + 1
    }
    
    println("✓ Loaded " + str(len(documents)) + " documents")
    return documents
}
```

---

## Text Chunking

### Fixed-Size Chunking

```klang
let chunker = new TextChunker(wordSize: 100, overlap: 20)

let text = "This is a long document that needs to be split into chunks..."
let chunks = chunker.chunk(text)

println("Created " + str(len(chunks)) + " chunks")

let i = 0
while i < len(chunks) {
    println("Chunk " + str(i + 1) + ": " + str(len(split(chunks[i], " "))) + " words")
    i = i + 1
}
```

### Sentence-Aware Chunking

```klang
fn sentenceChunker(text: string, sentencesPerChunk: int) -> string[] {
    // Split into sentences
    let sentences = split(text, ".")
    
    let chunks = []
    let i = 0
    while i < len(sentences) {
        let chunk = ""
        let j = 0
        while j < sentencesPerChunk && i + j < len(sentences) {
            chunk = chunk + sentences[i + j] + "."
            j = j + 1
        }
        
        if len(trim(chunk)) > 0 {
            chunks.push(trim(chunk))
        }
        
        i = i + sentencesPerChunk
    }
    
    return chunks
}

let text = "First sentence. Second sentence. Third sentence. Fourth sentence."
let chunks = sentenceChunker(text, 2)

let i = 0
while i < len(chunks) {
    println("Chunk " + str(i + 1) + ": " + chunks[i])
    i = i + 1
}
```

### Paragraph Chunking

```klang
fn paragraphChunker(text: string) -> string[] {
    // Split by double newlines (paragraphs)
    let paragraphs = split(text, "\n\n")
    
    let chunks = []
    let i = 0
    while i < len(paragraphs) {
        let para = trim(paragraphs[i])
        if len(para) > 0 {
            chunks.push(para)
        }
        i = i + 1
    }
    
    return chunks
}

let document = "First paragraph.\n\nSecond paragraph.\n\nThird paragraph."
let chunks = paragraphChunker(document)
```

### Semantic Chunking

```klang
fn semanticChunker(text: string, client: OpenAIClient, maxChunkSize: int) -> string[] {
    // Split into sentences
    let sentences = split(text, ".")
    
    let chunks = []
    let currentChunk = ""
    
    let i = 0
    while i < len(sentences) {
        let sentence = trim(sentences[i])
        
        if len(sentence) == 0 {
            i = i + 1
            continue
        }
        
        // Check if adding this sentence exceeds max size
        let testChunk = currentChunk + " " + sentence + "."
        
        if len(split(testChunk, " ")) > maxChunkSize {
            // Save current chunk
            if len(currentChunk) > 0 {
                chunks.push(trim(currentChunk))
            }
            
            // Start new chunk
            currentChunk = sentence + "."
        } else {
            currentChunk = testChunk
        }
        
        i = i + 1
    }
    
    // Add final chunk
    if len(currentChunk) > 0 {
        chunks.push(trim(currentChunk))
    }
    
    return chunks
}
```

### Chunking with Overlap

```klang
let chunker = new TextChunker(wordSize: 100, overlap: 20)

let text = fread("long_document.txt")
let chunks = chunker.chunk(text)

println("Chunk statistics:")
println("  Total chunks: " + str(len(chunks)))
println("  Avg chunk size: " + str(avgLength(chunks)) + " words")
println("  Overlap: 20 words")

// Overlap ensures context continuity
// chunk[0]: words 1-100
// chunk[1]: words 81-180 (20 word overlap)
// chunk[2]: words 161-260 (20 word overlap)
```

### Optimal Chunk Size

```klang
fn findOptimalChunkSize(document: string) -> int {
    let docLength = len(split(document, " "))
    
    // Rule of thumb: 100-200 words per chunk
    if docLength < 500 {
        return 50  // Smaller chunks for short docs
    } else if docLength < 2000 {
        return 100
    } else if docLength < 10000 {
        return 150
    } else {
        return 200  // Larger chunks for long docs
    }
}

let doc = fread("article.txt")
let chunkSize = findOptimalChunkSize(doc)
let chunker = new TextChunker(chunkSize, chunkSize / 5)  // 20% overlap
let chunks = chunker.chunk(doc)
```

---

## Building Knowledge Bases

### Simple Knowledge Base

```klang
class KnowledgeBase {
    constructor(apiKey: string) {
        this.client = new OpenAIClient(apiKey)
        this.store = new EmbeddingStore()
        this.documents = []
    }
    
    fn addDocument(content: string, source: string) {
        // Store original
        let doc = dict()
        doc.set("content", content)
        doc.set("source", source)
        this.documents.push(doc)
        
        // Chunk
        let chunker = new TextChunker(100, 20)
        let chunks = chunker.chunk(content)
        
        // Embed and store
        let embeddings = this.client.embedBatch(chunks, "text-embedding-ada-002")
        
        let i = 0
        while i < len(chunks) {
            let meta = dict()
            meta.set("source", source)
            meta.set("chunk_id", i)
            meta.set("doc_id", len(this.documents) - 1)
            
            this.store.addText(chunks[i], embeddings[i], meta)
            i = i + 1
        }
    }
    
    fn search(query: string, k: int) -> SearchResult[] {
        let queryEmb = this.client.embed(query)
        return this.store.search(queryEmb, k)
    }
    
    fn size() -> int {
        return this.store.size()
    }
}

// Build knowledge base
let kb = new KnowledgeBase(env.get("OPENAI_API_KEY"))

kb.addDocument("KLang is a programming language", "intro.txt")
kb.addDocument("KLang supports AI features", "features.txt")
kb.addDocument("KLang has built-in embeddings", "embeddings.txt")

println("Knowledge base size: " + str(kb.size()) + " chunks")
```

### Multi-Source Knowledge Base

```klang
let kb = new KnowledgeBase(apiKey)

// Add from files
let files = ["doc1.txt", "doc2.txt", "doc3.txt"]
let i = 0
while i < len(files) {
    let content = fread(files[i])
    kb.addDocument(content, files[i])
    println("✓ Added " + files[i])
    i = i + 1
}

// Add from strings
let manualDocs = [
    "KLang is AI-native",
    "KLang has 43+ stdlib functions",
    "KLang supports OOP"
]

let j = 0
while j < len(manualDocs) {
    kb.addDocument(manualDocs[j], "manual_entry_" + str(j))
    j = j + 1
}

println("Total chunks: " + str(kb.size()))
```

### Persistent Knowledge Base

```klang
class PersistentKB {
    constructor(apiKey: string, dbPath: string) {
        this.client = new OpenAIClient(apiKey)
        this.store = new EmbeddingStore()
        this.dbPath = dbPath
        this.load()
    }
    
    fn save() {
        // Save to disk (simplified)
        let data = this.store.serialize()
        fwrite(this.dbPath, data)
        println("✓ Knowledge base saved to " + this.dbPath)
    }
    
    fn load() {
        if fexists(this.dbPath) {
            let data = fread(this.dbPath)
            this.store.deserialize(data)
            println("✓ Knowledge base loaded from " + this.dbPath)
        } else {
            println("No existing knowledge base found")
        }
    }
    
    fn addDocument(content: string, source: string) {
        // Add document...
        // (implementation similar to KnowledgeBase)
        
        // Auto-save
        this.save()
    }
}

let kb = new PersistentKB(apiKey, "knowledge_base.bin")
```

### Incremental Updates

```klang
let kb = new KnowledgeBase(apiKey)

// Initial load
kb.addDocument(fread("v1.txt"), "v1.txt")

// Later: add new documents without rebuilding
kb.addDocument(fread("v2.txt"), "v2.txt")
kb.addDocument(fread("v3.txt"), "v3.txt")

// Knowledge base grows incrementally
```

### Metadata-Rich Knowledge Base

```klang
fn addDocumentWithMetadata(kb: KnowledgeBase, path: string) {
    let content = fread(path)
    let chunker = new TextChunker(100, 20)
    let chunks = chunker.chunk(content)
    
    let embeddings = kb.client.embedBatch(chunks, "text-embedding-ada-002")
    
    let i = 0
    while i < len(chunks) {
        let meta = dict()
        meta.set("source", path)
        meta.set("chunk_id", i)
        meta.set("timestamp", timestamp())
        meta.set("author", "Alice")
        meta.set("category", "documentation")
        meta.set("language", "en")
        meta.set("version", "1.0")
        
        kb.store.addText(chunks[i], embeddings[i], meta)
        i = i + 1
    }
}
```

---

## Semantic Retrieval

### Basic Retrieval

```klang
let query = "What are KLang's AI features?"
let queryEmb = client.embed(query)
let results = store.search(queryEmb, k: 5)

println("Top 5 results:")
let i = 0
while i < len(results) {
    println(str(i + 1) + ". (score: " + str(results[i].similarity) + ")")
    println("   " + results[i].text)
    println()
    i = i + 1
}
```

### Filtered Retrieval

```klang
fn retrieveFromSource(query: string, source: string, k: int) -> SearchResult[] {
    let queryEmb = client.embed(query)
    let allResults = store.search(queryEmb, k * 3)  // Get more, then filter
    
    let filtered = []
    let i = 0
    while i < len(allResults) && len(filtered) < k {
        if allResults[i].metadata.get("source") == source {
            filtered.push(allResults[i])
        }
        i = i + 1
    }
    
    return filtered
}

let results = retrieveFromSource("AI features", "features.txt", 3)
```

### Reranking Results

```klang
fn rerankResults(results: SearchResult[], query: string) -> SearchResult[] {
    // Rerank based on multiple criteria
    let scored = []
    
    let i = 0
    while i < len(results) {
        let score = results[i].similarity
        
        // Boost if query keywords present
        if contains(toLowerCase(results[i].text), toLowerCase(query)) {
            score = score * 1.2
        }
        
        // Boost recent documents
        let age = timestamp() - results[i].metadata.get("timestamp")
        if age < 86400 {  // Less than 1 day old
            score = score * 1.1
        }
        
        results[i].similarity = score
        scored.push(results[i])
        
        i = i + 1
    }
    
    // Sort by new scores
    return sortByScore(scored)
}
```

### Multi-Query Retrieval

```klang
fn multiQueryRetrieval(queries: string[], k: int) -> SearchResult[] {
    let allResults = []
    let seen = dict()  // Deduplication
    
    let i = 0
    while i < len(queries) {
        let queryEmb = client.embed(queries[i])
        let results = store.search(queryEmb, k)
        
        let j = 0
        while j < len(results) {
            let text = results[j].text
            if !seen.has(text) {
                seen.set(text, true)
                allResults.push(results[j])
            }
            j = j + 1
        }
        
        i = i + 1
    }
    
    return allResults
}

// Query expansion
let originalQuery = "AI features"
let expandedQueries = [
    originalQuery,
    "artificial intelligence capabilities",
    "machine learning functions",
    "LLM integration"
]

let results = multiQueryRetrieval(expandedQueries, 3)
```

---

## RAG Prompts

### Basic RAG Prompt

```klang
let ragPrompt = new RAGPrompt(
    "What is KLang?",
    "Answer the question based on the provided context."
)

ragPrompt.addContext("KLang is a programming language", "intro.txt")
ragPrompt.addContext("KLang supports AI features", "features.txt")

let prompt = ragPrompt.build()
println(prompt)

// Output:
// Context from intro.txt:
// KLang is a programming language
//
// Context from features.txt:
// KLang supports AI features
//
// Answer the question based on the provided context.
// Question: What is KLang?
```

### Structured RAG Prompt

```klang
fn buildStructuredRAGPrompt(query: string, contexts: SearchResult[]) -> string {
    let prompt = "# Context\n\n"
    
    // Add contexts with citations
    let i = 0
    while i < len(contexts) {
        prompt = prompt + "[" + str(i + 1) + "] "
        prompt = prompt + contexts[i].text + "\n"
        prompt = prompt + "Source: " + contexts[i].metadata.get("source") + "\n\n"
        i = i + 1
    }
    
    // Add instruction
    prompt = prompt + "# Instructions\n"
    prompt = prompt + "Answer the question based on the context above. "
    prompt = prompt + "Cite sources using [1], [2], etc.\n\n"
    
    // Add question
    prompt = prompt + "# Question\n"
    prompt = prompt + query + "\n\n"
    prompt = prompt + "# Answer\n"
    
    return prompt
}

let results = store.search(queryEmb, 3)
let prompt = buildStructuredRAGPrompt("What is KLang?", results)
```

### Custom Instructions

```klang
let instructions = """
Answer the question based on the provided context.

Guidelines:
1. Only use information from the context
2. If the answer isn't in the context, say "I don't know"
3. Cite specific parts of the context
4. Be concise and accurate
5. Use bullet points for lists
"""

let ragPrompt = new RAGPrompt(query, instructions)
```

---

## Complete RAG Pipeline

### Full Implementation

```klang
class RAGSystem {
    constructor(apiKey: string) {
        this.client = new OpenAIClient(apiKey)
        this.store = new EmbeddingStore()
        this.chunker = new TextChunker(100, 20)
    }
    
    fn ingestDocuments(paths: string[]) {
        println("Ingesting " + str(len(paths)) + " documents...")
        
        let i = 0
        while i < len(paths) {
            this.ingestDocument(paths[i])
            println("  ✓ " + paths[i])
            i = i + 1
        }
        
        println("✓ Ingestion complete: " + str(this.store.size()) + " chunks")
    }
    
    fn ingestDocument(path: string) {
        // Load
        let content = fread(path)
        
        // Preprocess
        content = preprocessDocument(content)
        
        // Chunk
        let chunks = this.chunker.chunk(content)
        
        // Embed
        let embeddings = this.client.embedBatch(chunks, "text-embedding-ada-002")
        
        // Store
        let i = 0
        while i < len(chunks) {
            let meta = dict()
            meta.set("source", path)
            meta.set("chunk_id", i)
            meta.set("timestamp", timestamp())
            
            this.store.addText(chunks[i], embeddings[i], meta)
            i = i + 1
        }
    }
    
    fn query(question: string, k: int) -> string {
        // Retrieve
        let queryEmb = this.client.embed(question)
        let results = this.store.search(queryEmb, k)
        
        // Build prompt
        let ragPrompt = new RAGPrompt(
            question,
            "Answer based on the context. Cite sources."
        )
        
        let i = 0
        while i < len(results) {
            ragPrompt.addContext(
                results[i].text,
                results[i].metadata.get("source")
            )
            i = i + 1
        }
        
        // Generate
        return this.client.chat(ragPrompt.build(), "gpt-4")
    }
}

// Usage
let rag = new RAGSystem(env.get("OPENAI_API_KEY"))

// Ingest documents
rag.ingestDocuments([
    "docs/intro.txt",
    "docs/features.txt",
    "docs/examples.txt"
])

// Query
let answer = rag.query("What are KLang's main features?", 5)
println("Answer: " + answer)
```

### Production Pipeline

```klang
class ProductionRAG {
    constructor(config: dict) {
        this.client = new OpenAIClient(config.get("api_key"))
        this.store = new EmbeddingStore()
        this.chunkSize = config.get("chunk_size")
        this.overlap = config.get("overlap")
        this.topK = config.get("top_k")
        this.model = config.get("model")
        this.cache = dict()
    }
    
    fn ingest(path: string) {
        // Check if already ingested
        if this.cache.has(path) {
            println("⚠️ Already ingested: " + path)
            return
        }
        
        // Load and validate
        if !fexists(path) {
            println("❌ File not found: " + path)
            return
        }
        
        let content = fread(path)
        if len(content) == 0 {
            println("⚠️ Empty file: " + path)
            return
        }
        
        // Process
        let chunker = new TextChunker(this.chunkSize, this.overlap)
        let chunks = chunker.chunk(content)
        
        // Embed with progress
        println("Embedding " + str(len(chunks)) + " chunks...")
        let embeddings = this.client.embedBatch(chunks, "text-embedding-ada-002")
        
        // Store
        let i = 0
        while i < len(chunks) {
            let meta = dict()
            meta.set("source", path)
            meta.set("chunk_id", i)
            
            this.store.addText(chunks[i], embeddings[i], meta)
            i = i + 1
        }
        
        // Mark as ingested
        this.cache.set(path, true)
        println("✓ Ingested: " + path)
    }
    
    fn query(question: string) -> dict {
        let startTime = timestamp()
        
        // Retrieve
        let queryEmb = this.client.embed(question)
        let results = this.store.search(queryEmb, this.topK)
        
        let retrievalTime = timestamp() - startTime
        
        // Generate
        let ragPrompt = new RAGPrompt(question, "Answer based on context")
        let i = 0
        while i < len(results) {
            ragPrompt.addContext(results[i].text, results[i].metadata.get("source"))
            i = i + 1
        }
        
        let answer = this.client.chat(ragPrompt.build(), this.model)
        
        let totalTime = timestamp() - startTime
        
        // Return with metadata
        let response = dict()
        response.set("answer", answer)
        response.set("sources", extractSources(results))
        response.set("retrieval_time_ms", retrievalTime)
        response.set("total_time_ms", totalTime)
        response.set("num_chunks", len(results))
        
        return response
    }
}

// Configure
let config = dict()
config.set("api_key", env.get("OPENAI_API_KEY"))
config.set("chunk_size", 100)
config.set("overlap", 20)
config.set("top_k", 5)
config.set("model", "gpt-4")

let rag = new ProductionRAG(config)
```

---

## Advanced Techniques

### Query Expansion

```klang
fn expandQuery(query: string, client: OpenAIClient) -> string[] {
    let prompt = "Generate 3 alternative phrasings of: " + query
    let response = client.chat(prompt, "gpt-3.5-turbo")
    
    // Parse response into list
    let alternatives = split(response, "\n")
    
    // Include original
    let expanded = [query]
    let i = 0
    while i < len(alternatives) {
        let alt = trim(alternatives[i])
        if len(alt) > 0 {
            expanded.push(alt)
        }
        i = i + 1
    }
    
    return expanded
}

let originalQuery = "What is machine learning?"
let queries = expandQuery(originalQuery, client)

// Search with all variations
let allResults = []
let i = 0
while i < len(queries) {
    let queryEmb = client.embed(queries[i])
    let results = store.search(queryEmb, 3)
    allResults = allResults + results
    i = i + 1
}
```

### Hypothetical Document Embeddings (HyDE)

```klang
fn hydeRetrieval(query: string, client: OpenAIClient, store: EmbeddingStore, k: int) -> SearchResult[] {
    // Generate hypothetical answer
    let hydePrompt = "Write a detailed answer to: " + query
    let hypotheticalAnswer = client.chat(hydePrompt, "gpt-3.5-turbo")
    
    // Embed hypothetical answer
    let hydeEmb = client.embed(hypotheticalAnswer)
    
    // Search with hypothetical embedding
    return store.search(hydeEmb, k)
}

let results = hydeRetrieval("Explain neural networks", client, store, 5)
```

### Parent Document Retrieval

```klang
// Store both chunks and full documents
class ParentDocRAG {
    constructor(apiKey: string) {
        this.client = new OpenAIClient(apiKey)
        this.chunkStore = new EmbeddingStore()
        this.documents = dict()
    }
    
    fn ingest(content: string, docId: string) {
        // Store full document
        this.documents.set(docId, content)
        
        // Chunk and store chunks
        let chunker = new TextChunker(100, 20)
        let chunks = chunker.chunk(content)
        let embeddings = this.client.embedBatch(chunks, "text-embedding-ada-002")
        
        let i = 0
        while i < len(chunks) {
            let meta = dict()
            meta.set("doc_id", docId)
            meta.set("chunk_id", i)
            
            this.chunkStore.addText(chunks[i], embeddings[i], meta)
            i = i + 1
        }
    }
    
    fn query(question: string, k: int) -> string {
        // Retrieve relevant chunks
        let queryEmb = this.client.embed(question)
        let chunkResults = this.chunkStore.search(queryEmb, k)
        
        // Get parent documents
        let parentDocs = dict()
        let i = 0
        while i < len(chunkResults) {
            let docId = chunkResults[i].metadata.get("doc_id")
            if !parentDocs.has(docId) {
                parentDocs.set(docId, this.documents.get(docId))
            }
            i = i + 1
        }
        
        // Build prompt with full documents
        let ragPrompt = new RAGPrompt(question, "Answer based on documents")
        let docIds = parentDocs.keys()
        let j = 0
        while j < len(docIds) {
            ragPrompt.addContext(parentDocs.get(docIds[j]), docIds[j])
            j = j + 1
        }
        
        return this.client.chat(ragPrompt.build(), "gpt-4")
    }
}
```

### Self-Querying Retrieval

```klang
fn selfQueryRetrieval(query: string, client: OpenAIClient) -> dict {
    // Extract structured query components
    let extractPrompt = """
    Extract from this query:
    - semantic_query: The actual question
    - filters: Any constraints (time, source, category)
    
    Query: """ + query + """
    
    Return JSON.
    """
    
    client.setResponseFormat("json_object")
    let structured = client.chat(extractPrompt, "gpt-3.5-turbo")
    
    // Parse JSON response
    let parsed = parseJSON(structured)
    
    return parsed
}

let query = "What were the AI features added in 2024?"
let structured = selfQueryRetrieval(query, client)

// Use structured query for filtered retrieval
let semanticQuery = structured.get("semantic_query")
let filters = structured.get("filters")
```

---

## Production Deployment

### Error Handling

```klang
class RobustRAG {
    fn query(question: string, k: int) -> dict {
        let response = dict()
        
        try {
            // Embed query
            let queryEmb = this.client.embed(question)
            
            // Retrieve
            let results = this.store.search(queryEmb, k)
            
            if len(results) == 0 {
                response.set("answer", "No relevant information found.")
                response.set("success", false)
                return response
            }
            
            // Generate
            let ragPrompt = new RAGPrompt(question, "Answer based on context")
            let i = 0
            while i < len(results) {
                ragPrompt.addContext(results[i].text, results[i].metadata.get("source"))
                i = i + 1
            }
            
            let answer = this.client.chat(ragPrompt.build(), "gpt-4")
            
            response.set("answer", answer)
            response.set("sources", extractSources(results))
            response.set("success", true)
            
        } catch error {
            println("❌ Error: " + error)
            response.set("answer", "An error occurred.")
            response.set("error", error)
            response.set("success", false)
        }
        
        return response
    }
}
```

### Monitoring and Logging

```klang
class MonitoredRAG {
    constructor(apiKey: string) {
        this.client = new OpenAIClient(apiKey)
        this.store = new EmbeddingStore()
        this.metrics = dict()
        this.metrics.set("total_queries", 0)
        this.metrics.set("successful_queries", 0)
        this.metrics.set("failed_queries", 0)
        this.metrics.set("avg_retrieval_time", 0.0)
        this.metrics.set("avg_generation_time", 0.0)
    }
    
    fn query(question: string, k: int) -> string {
        this.metrics.set("total_queries", this.metrics.get("total_queries") + 1)
        
        let startTime = timestamp()
        
        try {
            // Retrieve
            let queryEmb = this.client.embed(question)
            let results = this.store.search(queryEmb, k)
            let retrievalTime = timestamp() - startTime
            
            // Generate
            let genStart = timestamp()
            let ragPrompt = new RAGPrompt(question, "Answer")
            let i = 0
            while i < len(results) {
                ragPrompt.addContext(results[i].text, results[i].metadata.get("source"))
                i = i + 1
            }
            let answer = this.client.chat(ragPrompt.build(), "gpt-4")
            let generationTime = timestamp() - genStart
            
            // Update metrics
            this.updateMetrics(retrievalTime, generationTime, true)
            
            // Log
            this.log("SUCCESS", question, len(results), retrievalTime + generationTime)
            
            return answer
            
        } catch error {
            this.metrics.set("failed_queries", this.metrics.get("failed_queries") + 1)
            this.log("ERROR", question, 0, timestamp() - startTime)
            return "Error: " + error
        }
    }
    
    fn log(level: string, query: string, numResults: int, timeMs: int) {
        let logEntry = "[" + level + "] "
        logEntry = logEntry + "Query: '" + query + "' "
        logEntry = logEntry + "Results: " + str(numResults) + " "
        logEntry = logEntry + "Time: " + str(timeMs) + "ms"
        
        println(logEntry)
        
        // Write to file
        fappend("rag_logs.txt", logEntry + "\n")
    }
    
    fn getMetrics() -> dict {
        return this.metrics
    }
}
```

---

## Performance Optimization

### Embedding Caching

```klang
class CachedRAG {
    constructor(apiKey: string) {
        this.client = new OpenAIClient(apiKey)
        this.store = new EmbeddingStore()
        this.embeddingCache = dict()
        this.cacheHits = 0
        this.cacheMisses = 0
    }
    
    fn getCachedEmbedding(text: string) -> Embedding {
        if this.embeddingCache.has(text) {
            this.cacheHits = this.cacheHits + 1
            return this.embeddingCache.get(text)
        }
        
        this.cacheMisses = this.cacheMisses + 1
        let embedding = this.client.embed(text)
        this.embeddingCache.set(text, embedding)
        
        return embedding
    }
    
    fn query(question: string, k: int) -> string {
        let queryEmb = this.getCachedEmbedding(question)
        let results = this.store.search(queryEmb, k)
        
        // Build and generate answer
        let ragPrompt = new RAGPrompt(question, "Answer")
        let i = 0
        while i < len(results) {
            ragPrompt.addContext(results[i].text, results[i].metadata.get("source"))
            i = i + 1
        }
        
        return this.client.chat(ragPrompt.build(), "gpt-4")
    }
    
    fn getCacheStats() -> dict {
        let stats = dict()
        stats.set("hits", this.cacheHits)
        stats.set("misses", this.cacheMisses)
        stats.set("hit_rate", this.cacheHits / (this.cacheHits + this.cacheMisses))
        return stats
    }
}
```

### Batch Processing

```klang
fn batchIngest(rag: RAGSystem, paths: string[], batchSize: int) {
    let numBatches = (len(paths) + batchSize - 1) / batchSize
    
    println("Processing " + str(len(paths)) + " files in " + str(numBatches) + " batches")
    
    let i = 0
    while i < len(paths) {
        let batchEnd = min(i + batchSize, len(paths))
        let batch = paths[i:batchEnd]
        
        println("Batch " + str((i / batchSize) + 1) + "/" + str(numBatches))
        
        // Process batch
        let j = 0
        while j < len(batch) {
            rag.ingestDocument(batch[j])
            j = j + 1
        }
        
        i = batchEnd
    }
    
    println("✓ Batch ingestion complete")
}

let files = listFiles("docs/")
batchIngest(rag, files, 10)  // Process 10 at a time
```

### Response Caching

```klang
class CachedResponseRAG {
    constructor(apiKey: string) {
        this.rag = new RAGSystem(apiKey)
        this.responseCache = dict()
    }
    
    fn query(question: string, k: int) -> string {
        // Normalize query for cache key
        let cacheKey = toLowerCase(trim(question))
        
        if this.responseCache.has(cacheKey) {
            println("📦 Using cached response")
            return this.responseCache.get(cacheKey)
        }
        
        println("🌐 Generating new response")
        let answer = this.rag.query(question, k)
        
        this.responseCache.set(cacheKey, answer)
        return answer
    }
}
```

---

## Next Steps

- **[AI Agents](34-ai-agents.md)** - Build agents with RAG
- **[Chat Sessions](35-chat-sessions.md)** - Conversational RAG
- **[Embeddings](32-embeddings.md)** - Deep dive into embeddings

---

## Summary

KLang makes RAG systems easy:

✅ **Complete pipeline** - Ingest, retrieve, generate  
✅ **Flexible chunking** - Multiple strategies  
✅ **Semantic search** - Find relevant information  
✅ **Production-ready** - Error handling, monitoring  
✅ **Optimized** - Caching, batching, efficiency  

Build intelligent Q&A systems with KLang RAG! 🚀
