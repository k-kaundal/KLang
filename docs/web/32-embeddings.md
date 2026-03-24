# Embeddings - Vector Representations and Semantic Search

Vector embeddings transform text into numerical representations that capture semantic meaning. KLang provides built-in support for generating, manipulating, and searching embeddings, enabling powerful semantic search and similarity matching.

## Table of Contents

1. [What are Embeddings](#what-are-embeddings)
2. [Generating Embeddings](#generating-embeddings)
3. [Embedding Class](#embedding-class)
4. [Similarity Metrics](#similarity-metrics)
5. [Embedding Store](#embedding-store)
6. [Semantic Search](#semantic-search)
7. [Vector Operations](#vector-operations)
8. [Clustering and Classification](#clustering-and-classification)
9. [Dimensionality Reduction](#dimensionality-reduction)
10. [Best Practices](#best-practices)
11. [Performance Optimization](#performance-optimization)

---

## What are Embeddings

### Concept

Embeddings convert text into high-dimensional vectors that capture semantic meaning:

```klang
// Text
"cat" → [0.2, 0.8, 0.1, 0.5, ...]  // 1536 dimensions
"dog" → [0.3, 0.7, 0.2, 0.6, ...]  // Similar to cat!
"car" → [0.9, 0.1, 0.8, 0.2, ...]  // Different from cat/dog
```

### Why Embeddings Matter

```klang
// Traditional keyword search fails here:
query = "automobile"
docs = ["I drove my car", "The vehicle was fast"]
// No exact match for "automobile"!

// Semantic search with embeddings succeeds:
queryEmbed = embed("automobile")
doc1Embed = embed("I drove my car")
doc2Embed = embed("The vehicle was fast")

// Both docs have high similarity to query!
similarity1 = cosineSimilarity(queryEmbed, doc1Embed)  // ~0.85
similarity2 = cosineSimilarity(queryEmbed, doc2Embed)  // ~0.82
```

### Properties of Embeddings

```klang
// Semantic similarity
embed("king") - embed("man") + embed("woman") ≈ embed("queen")

// Distance preserves meaning
distance(embed("happy"), embed("joyful")) < distance(embed("happy"), embed("sad"))

// Dimensions capture different aspects
dim_42 might represent "animal-ness"
dim_137 might represent "size"
dim_891 might represent "sentiment"
```

---

## Generating Embeddings

### Using OpenAI

```klang
let apiKey = env.get("OPENAI_API_KEY")
let client = new OpenAIClient(apiKey)

// Generate single embedding
let text = "KLang is an AI-native programming language"
let embedding = client.embed(text, "text-embedding-ada-002")

println("Text: " + text)
println("Dimensions: " + str(len(embedding.vector)))
println("First 5 values: " + str(embedding.vector[0:5]))
```

### Batch Embeddings

```klang
let client = new OpenAIClient(apiKey)

let texts = [
    "Machine learning is a subset of AI",
    "Neural networks are inspired by the brain",
    "Deep learning uses multiple layers",
    "Transformers revolutionized NLP",
    "GPT is a generative pre-trained model"
]

// Generate all embeddings in one API call
let embeddings = client.embedBatch(texts, "text-embedding-ada-002")

println("Generated " + str(len(embeddings)) + " embeddings")

let i = 0
while i < len(embeddings) {
    println(str(i + 1) + ". Dimensions: " + str(len(embeddings[i].vector)))
    i = i + 1
}
```

### Using Local Models (Ollama)

```klang
let ollama = new OllamaClient("http://localhost:11434")

// Generate embedding locally (free!)
let text = "Local embedding generation"
let embedding = ollama.embed("llama2", text)

println("Local embedding dimensions: " + str(len(embedding.vector)))
```

### Custom Embedding Function

```klang
fn getEmbedding(text: string, provider: string) -> Embedding {
    if provider == "openai" {
        let client = new OpenAIClient(env.get("OPENAI_API_KEY"))
        return client.embed(text, "text-embedding-ada-002")
    } else if provider == "ollama" {
        let client = new OllamaClient("http://localhost:11434")
        return client.embed("llama2", text)
    } else {
        println("Unknown provider: " + provider)
        return null
    }
}

let embedding = getEmbedding("Test text", "openai")
```

### Embedding with Metadata

```klang
let text = "KLang supports AI features"
let embedding = client.embed(text, "text-embedding-ada-002")

// Attach metadata
embedding.text = text
embedding.timestamp = timestamp()
embedding.source = "documentation"

println("Text: " + embedding.text)
println("Generated at: " + str(embedding.timestamp))
```

---

## Embedding Class

### Creating Embeddings

```klang
// From vector
let vector = [0.1, 0.2, 0.3, 0.4, 0.5]
let embedding = new Embedding(vector, "sample text")

// From API
let client = new OpenAIClient(apiKey)
let embedding = client.embed("text", "text-embedding-ada-002")
```

### Accessing Properties

```klang
let embedding = new Embedding([0.1, 0.2, 0.3], "test")

println("Vector: " + str(embedding.vector))
println("Text: " + embedding.text)
println("Dimensions: " + str(len(embedding.vector)))
```

### Vector Magnitude

```klang
let embedding = new Embedding([3.0, 4.0], "test")
let magnitude = embedding.magnitude()

println("Magnitude: " + str(magnitude))  // 5.0
// sqrt(3^2 + 4^2) = sqrt(9 + 16) = sqrt(25) = 5
```

### Normalize Embedding

```klang
let embedding = new Embedding([3.0, 4.0], "test")

// Before normalization
println("Original magnitude: " + str(embedding.magnitude()))

// Normalize
let normalized = embedding.normalize()

// After normalization
println("Normalized magnitude: " + str(normalized.magnitude()))  // 1.0
println("Normalized vector: " + str(normalized.vector))
```

### Copy Embedding

```klang
let original = new Embedding([0.1, 0.2, 0.3], "original")
let copy = original.copy()

// Modify copy
copy.vector[0] = 0.9

println("Original: " + str(original.vector[0]))  // 0.1
println("Copy: " + str(copy.vector[0]))          // 0.9
```

---

## Similarity Metrics

### Cosine Similarity

Measures angle between vectors (range: -1 to 1, where 1 = identical):

```klang
let emb1 = new Embedding([0.8, 0.2, 0.1], "cat")
let emb2 = new Embedding([0.7, 0.3, 0.2], "dog")
let emb3 = new Embedding([0.1, 0.9, 0.8], "car")

let sim1 = emb1.cosineSimilarity(emb2)
println("cat vs dog: " + str(sim1))  // ~0.95 (very similar)

let sim2 = emb1.cosineSimilarity(emb3)
println("cat vs car: " + str(sim2))  // ~0.30 (less similar)
```

### Euclidean Distance

Measures straight-line distance (lower = more similar):

```klang
let emb1 = new Embedding([0.0, 0.0], "origin")
let emb2 = new Embedding([3.0, 4.0], "point")

let distance = emb1.euclideanDistance(emb2)
println("Distance: " + str(distance))  // 5.0
```

### Dot Product

Raw similarity score (not normalized):

```klang
let emb1 = new Embedding([1.0, 2.0, 3.0], "a")
let emb2 = new Embedding([2.0, 3.0, 4.0], "b")

let dotProd = emb1.dotProduct(emb2)
println("Dot product: " + str(dotProd))  // 20.0
// (1*2) + (2*3) + (3*4) = 2 + 6 + 12 = 20
```

### Choosing the Right Metric

```klang
fn compareMetrics(emb1: Embedding, emb2: Embedding) {
    let cosine = emb1.cosineSimilarity(emb2)
    let euclidean = emb1.euclideanDistance(emb2)
    let dot = emb1.dotProduct(emb2)
    
    println("Cosine similarity: " + str(cosine))
    println("Euclidean distance: " + str(euclidean))
    println("Dot product: " + str(dot))
    println()
}

// Use cases:
// - Cosine: Best for text similarity (ignores magnitude)
// - Euclidean: Good for spatial relationships
// - Dot product: Fast, but sensitive to magnitude
```

### Similarity Threshold

```klang
fn areSimilar(emb1: Embedding, emb2: Embedding, threshold: float) -> bool {
    let similarity = emb1.cosineSimilarity(emb2)
    return similarity >= threshold
}

let cat = client.embed("cat")
let dog = client.embed("dog")
let car = client.embed("car")

if areSimilar(cat, dog, 0.8) {
    println("cat and dog are similar")
}

if !areSimilar(cat, car, 0.8) {
    println("cat and car are not similar")
}
```

---

## Embedding Store

### Creating a Store

```klang
let store = new EmbeddingStore()
println("Created embedding store")
```

### Adding Embeddings

```klang
let store = new EmbeddingStore()
let client = new OpenAIClient(apiKey)

// Add single embedding
let text1 = "KLang is a programming language"
let emb1 = client.embed(text1)
let meta1 = dict()
meta1.set("category", "tech")
meta1.set("source", "docs")

store.addText(text1, emb1, meta1)

// Add more embeddings
let text2 = "Python is popular for data science"
let emb2 = client.embed(text2)
let meta2 = dict()
meta2.set("category", "tech")
meta2.set("source", "article")

store.addText(text2, emb2, meta2)

println("Store size: " + str(store.size()))
```

### Adding from Document

```klang
let store = new EmbeddingStore()
let client = new OpenAIClient(apiKey)

// Load and chunk document
let doc = fread("knowledge.txt")
let chunker = new TextChunker(100, 20)
let chunks = chunker.chunk(doc)

// Add all chunks to store
let i = 0
while i < len(chunks) {
    let embedding = client.embed(chunks[i])
    let meta = dict()
    meta.set("chunk_id", i)
    meta.set("source", "knowledge.txt")
    
    store.addText(chunks[i], embedding, meta)
    i = i + 1
}

println("Added " + str(store.size()) + " chunks to store")
```

### Bulk Loading

```klang
let store = new EmbeddingStore()
let client = new OpenAIClient(apiKey)

let documents = [
    "First document about AI",
    "Second document about ML",
    "Third document about NLP",
    "Fourth document about CV"
]

// Generate embeddings in batch (more efficient)
let embeddings = client.embedBatch(documents, "text-embedding-ada-002")

// Add all to store
let i = 0
while i < len(documents) {
    let meta = dict()
    meta.set("doc_id", i)
    store.addText(documents[i], embeddings[i], meta)
    i = i + 1
}

println("Loaded " + str(store.size()) + " documents")
```

### Store Statistics

```klang
let store = new EmbeddingStore()
// ... add embeddings ...

println("=== Store Statistics ===")
println("Total embeddings: " + str(store.size()))
println("Dimensions: " + str(store.getDimensions()))
println("Memory usage: ~" + str(store.size() * store.getDimensions() * 8) + " bytes")
```

---

## Semantic Search

### Basic Search

```klang
let store = new EmbeddingStore()
let client = new OpenAIClient(apiKey)

// Build knowledge base
let docs = [
    "KLang is a programming language",
    "Python is used for AI",
    "JavaScript runs in browsers",
    "C++ is fast and efficient"
]

let i = 0
while i < len(docs) {
    let emb = client.embed(docs[i])
    store.addText(docs[i], emb, dict())
    i = i + 1
}

// Search
let query = "Tell me about KLang"
let queryEmb = client.embed(query)
let results = store.search(queryEmb, 2)

println("Query: " + query)
println("\nTop results:")
let j = 0
while j < len(results) {
    println(str(j + 1) + ". " + results[j].text)
    println("   Similarity: " + str(results[j].similarity))
    j = j + 1
}
```

### Search with Filtering

```klang
let store = new EmbeddingStore()
// ... populate store with metadata ...

// Search only in specific category
let query = "programming languages"
let queryEmb = client.embed(query)

fn categoryFilter(metadata: dict) -> bool {
    return metadata.get("category") == "tech"
}

let results = store.searchWithFilter(queryEmb, 5, categoryFilter)
```

### Multi-Query Search

```klang
let queries = [
    "What is machine learning?",
    "How does AI work?",
    "Explain neural networks"
]

let allResults = []

let i = 0
while i < len(queries) {
    let queryEmb = client.embed(queries[i])
    let results = store.search(queryEmb, 3)
    
    println("Query: " + queries[i])
    let j = 0
    while j < len(results) {
        println("  " + results[j].text)
        j = j + 1
    }
    println()
    
    i = i + 1
}
```

### Ranked Retrieval

```klang
fn rankedSearch(store: EmbeddingStore, query: string, k: int) -> SearchResult[] {
    let queryEmb = client.embed(query)
    let results = store.search(queryEmb, k)
    
    // Results are already ranked by similarity
    println("Found " + str(len(results)) + " results:")
    
    let i = 0
    while i < len(results) {
        let rank = i + 1
        let score = results[i].similarity
        let text = results[i].text
        
        println("Rank " + str(rank) + " (score: " + str(score) + "):")
        println("  " + text)
        i = i + 1
    }
    
    return results
}

let results = rankedSearch(store, "AI programming", 5)
```

### Hybrid Search (Keyword + Semantic)

```klang
fn hybridSearch(store: EmbeddingStore, query: string, k: int) -> SearchResult[] {
    // Get semantic results
    let queryEmb = client.embed(query)
    let semanticResults = store.search(queryEmb, k * 2)
    
    // Filter by keyword match
    let finalResults = []
    let keywords = split(toLowerCase(query), " ")
    
    let i = 0
    while i < len(semanticResults) && len(finalResults) < k {
        let text = toLowerCase(semanticResults[i].text)
        let hasKeyword = false
        
        let j = 0
        while j < len(keywords) {
            if contains(text, keywords[j]) {
                hasKeyword = true
                break
            }
            j = j + 1
        }
        
        if hasKeyword || semanticResults[i].similarity > 0.8 {
            finalResults.push(semanticResults[i])
        }
        
        i = i + 1
    }
    
    return finalResults
}
```

---

## Vector Operations

### Addition

```klang
let v1 = new Embedding([1.0, 2.0, 3.0], "a")
let v2 = new Embedding([4.0, 5.0, 6.0], "b")

let sum = v1.add(v2)
println("Sum: " + str(sum.vector))  // [5.0, 7.0, 9.0]
```

### Subtraction

```klang
let king = client.embed("king")
let man = client.embed("man")
let woman = client.embed("woman")

// king - man + woman ≈ queen
let result = king.subtract(man).add(woman)

// Find closest word
let queen = client.embed("queen")
let similarity = result.cosineSimilarity(queen)

println("Similarity to 'queen': " + str(similarity))
```

### Scalar Multiplication

```klang
let v = new Embedding([1.0, 2.0, 3.0], "test")
let scaled = v.multiply(2.5)

println("Original: " + str(v.vector))      // [1.0, 2.0, 3.0]
println("Scaled: " + str(scaled.vector))   // [2.5, 5.0, 7.5]
```

### Element-wise Operations

```klang
let v1 = new Embedding([2.0, 4.0, 6.0], "a")
let v2 = new Embedding([1.0, 2.0, 3.0], "b")

// Element-wise multiplication
let result = []
let i = 0
while i < len(v1.vector) {
    result.push(v1.vector[i] * v2.vector[i])
    i = i + 1
}

let product = new Embedding(result, "product")
println("Element-wise product: " + str(product.vector))  // [2.0, 8.0, 18.0]
```

### Average Embeddings

```klang
fn averageEmbeddings(embeddings: Embedding[]) -> Embedding {
    let n = len(embeddings)
    let dimensions = len(embeddings[0].vector)
    
    let avg = []
    let d = 0
    while d < dimensions {
        let sum = 0.0
        let i = 0
        while i < n {
            sum = sum + embeddings[i].vector[d]
            i = i + 1
        }
        avg.push(sum / n)
        d = d + 1
    }
    
    return new Embedding(avg, "average")
}

// Average multiple document embeddings
let doc1 = client.embed("First paragraph about AI")
let doc2 = client.embed("Second paragraph about AI")
let doc3 = client.embed("Third paragraph about AI")

let avgEmb = averageEmbeddings([doc1, doc2, doc3])
println("Created average embedding")
```

### Weighted Combination

```klang
fn weightedCombination(embeddings: Embedding[], weights: float[]) -> Embedding {
    let dimensions = len(embeddings[0].vector)
    let result = []
    
    let d = 0
    while d < dimensions {
        let weighted = 0.0
        let i = 0
        while i < len(embeddings) {
            weighted = weighted + embeddings[i].vector[d] * weights[i]
            i = i + 1
        }
        result.push(weighted)
        d = d + 1
    }
    
    return new Embedding(result, "weighted")
}

// Combine with different weights
let emb1 = client.embed("Important document")
let emb2 = client.embed("Less important document")

let combined = weightedCombination([emb1, emb2], [0.8, 0.2])
```

---

## Clustering and Classification

### K-Means Clustering

```klang
fn kMeansClustering(embeddings: Embedding[], k: int, iterations: int) -> int[] {
    // Initialize random centroids
    let centroids = []
    let i = 0
    while i < k {
        centroids.push(embeddings[i])
        i = i + 1
    }
    
    let assignments = []
    let iter = 0
    while iter < iterations {
        // Assign each point to nearest centroid
        assignments = []
        let i = 0
        while i < len(embeddings) {
            let minDist = embeddings[i].euclideanDistance(centroids[0])
            let cluster = 0
            
            let c = 1
            while c < k {
                let dist = embeddings[i].euclideanDistance(centroids[c])
                if dist < minDist {
                    minDist = dist
                    cluster = c
                }
                c = c + 1
            }
            
            assignments.push(cluster)
            i = i + 1
        }
        
        // Update centroids (simplified)
        iter = iter + 1
    }
    
    return assignments
}

// Cluster documents
let docs = ["doc1", "doc2", "doc3", "doc4", "doc5"]
let embeddings = []
let i = 0
while i < len(docs) {
    embeddings.push(client.embed(docs[i]))
    i = i + 1
}

let clusters = kMeansClustering(embeddings, 2, 10)
println("Cluster assignments: " + str(clusters))
```

### Classification by Similarity

```klang
class TextClassifier {
    constructor() {
        this.classes = dict()
        this.client = new OpenAIClient(env.get("OPENAI_API_KEY"))
    }
    
    fn addClass(className: string, examples: string[]) {
        let embeddings = []
        let i = 0
        while i < len(examples) {
            let emb = this.client.embed(examples[i])
            embeddings.push(emb)
            i = i + 1
        }
        
        // Store average embedding for class
        let avg = averageEmbeddings(embeddings)
        this.classes.set(className, avg)
    }
    
    fn classify(text: string) -> string {
        let textEmb = this.client.embed(text)
        
        let maxSim = -1.0
        let bestClass = ""
        
        let classNames = this.classes.keys()
        let i = 0
        while i < len(classNames) {
            let classEmb = this.classes.get(classNames[i])
            let sim = textEmb.cosineSimilarity(classEmb)
            
            if sim > maxSim {
                maxSim = sim
                bestClass = classNames[i]
            }
            i = i + 1
        }
        
        return bestClass
    }
}

// Train classifier
let classifier = new TextClassifier()
classifier.addClass("tech", ["computer", "software", "programming"])
classifier.addClass("sports", ["football", "basketball", "soccer"])
classifier.addClass("food", ["pizza", "burger", "pasta"])

// Classify new text
let category = classifier.classify("I love coding")
println("Category: " + category)  // "tech"
```

### Anomaly Detection

```klang
fn detectAnomalies(embeddings: Embedding[], threshold: float) -> int[] {
    // Find average embedding
    let avg = averageEmbeddings(embeddings)
    
    // Find outliers
    let anomalies = []
    let i = 0
    while i < len(embeddings) {
        let sim = embeddings[i].cosineSimilarity(avg)
        
        if sim < threshold {
            anomalies.push(i)
        }
        i = i + 1
    }
    
    return anomalies
}

let texts = [
    "Machine learning is great",
    "AI helps solve problems",
    "Neural networks are powerful",
    "RANDOM SPAM TEXT!!!",  // Anomaly
    "Deep learning advances"
]

let embeddings = []
let i = 0
while i < len(texts) {
    embeddings.push(client.embed(texts[i]))
    i = i + 1
}

let anomalies = detectAnomalies(embeddings, 0.7)
println("Anomalies found at indices: " + str(anomalies))
```

---

## Dimensionality Reduction

### PCA (Simplified)

```klang
fn reduceDimensions(embeddings: Embedding[], targetDim: int) -> Embedding[] {
    // Simplified dimensionality reduction
    // In practice, use proper PCA algorithm
    
    let reduced = []
    let i = 0
    while i < len(embeddings) {
        let newVector = embeddings[i].vector[0:targetDim]
        let newEmb = new Embedding(newVector, embeddings[i].text)
        reduced.push(newEmb)
        i = i + 1
    }
    
    return reduced
}

// Reduce from 1536 to 128 dimensions
let embeddings = [emb1, emb2, emb3]
let reduced = reduceDimensions(embeddings, 128)

println("Original dimensions: " + str(len(embeddings[0].vector)))
println("Reduced dimensions: " + str(len(reduced[0].vector)))
```

### Visualization (2D Projection)

```klang
fn projectTo2D(embeddings: Embedding[]) -> float[][] {
    // Simple projection to first 2 dimensions
    let points = []
    
    let i = 0
    while i < len(embeddings) {
        let point = [embeddings[i].vector[0], embeddings[i].vector[1]]
        points.push(point)
        i = i + 1
    }
    
    return points
}

let embeddings = [
    client.embed("cat"),
    client.embed("dog"),
    client.embed("bird"),
    client.embed("car"),
    client.embed("truck")
]

let points2D = projectTo2D(embeddings)

println("2D Projections:")
let i = 0
while i < len(points2D) {
    println(embeddings[i].text + ": (" + str(points2D[i][0]) + ", " + str(points2D[i][1]) + ")")
    i = i + 1
}
```

---

## Best Practices

### 1. Cache Embeddings

```klang
let embeddingCache = dict()

fn getCachedEmbedding(text: string) -> Embedding {
    if embeddingCache.has(text) {
        return embeddingCache.get(text)
    }
    
    let embedding = client.embed(text)
    embeddingCache.set(text, embedding)
    return embedding
}

// First call: API request
let emb1 = getCachedEmbedding("test")

// Second call: cached (no API cost)
let emb2 = getCachedEmbedding("test")
```

### 2. Normalize Text Before Embedding

```klang
fn normalizeText(text: string) -> string {
    // Remove extra whitespace
    let normalized = trim(text)
    
    // Convert to lowercase
    normalized = toLowerCase(normalized)
    
    // Remove special characters (optional)
    // normalized = removeSpecialChars(normalized)
    
    return normalized
}

let text = "  KLang IS Amazing!  "
let normalized = normalizeText(text)
let embedding = client.embed(normalized)
```

### 3. Batch Embedding Generation

```klang
// Bad: Individual API calls
let embeddings = []
let i = 0
while i < len(texts) {
    embeddings.push(client.embed(texts[i]))
    i = i + 1
}

// Good: Single batch call
let embeddings = client.embedBatch(texts, "text-embedding-ada-002")
```

### 4. Store Metadata with Embeddings

```klang
let text = "Important document"
let embedding = client.embed(text)

let meta = dict()
meta.set("source", "knowledge_base.txt")
meta.set("timestamp", timestamp())
meta.set("category", "tech")
meta.set("author", "Alice")

store.addText(text, embedding, meta)
```

### 5. Use Appropriate Thresholds

```klang
fn intelligentSearch(query: string, minSimilarity: float) -> SearchResult[] {
    let queryEmb = client.embed(query)
    let allResults = store.search(queryEmb, 100)
    
    // Filter by threshold
    let filtered = []
    let i = 0
    while i < len(allResults) {
        if allResults[i].similarity >= minSimilarity {
            filtered.push(allResults[i])
        }
        i = i + 1
    }
    
    return filtered
}

// High-quality matches only
let results = intelligentSearch("AI programming", 0.85)
```

---

## Performance Optimization

### Memory-Efficient Storage

```klang
class CompactEmbeddingStore {
    constructor() {
        this.texts = []
        this.vectors = []
        this.metadata = []
    }
    
    fn add(text: string, embedding: Embedding, meta: dict) {
        this.texts.push(text)
        this.vectors.push(embedding.vector)
        this.metadata.push(meta)
    }
    
    fn search(queryEmb: Embedding, k: int) -> SearchResult[] {
        let scores = []
        
        let i = 0
        while i < len(this.vectors) {
            let emb = new Embedding(this.vectors[i], "")
            let sim = queryEmb.cosineSimilarity(emb)
            scores.push({"index": i, "score": sim})
            i = i + 1
        }
        
        // Sort and return top k
        // (sorting implementation omitted for brevity)
        
        return topK(scores, k)
    }
}
```

### Approximate Nearest Neighbors

```klang
// For large-scale search, use approximate methods
fn approximateSearch(store: EmbeddingStore, queryEmb: Embedding, k: int) -> SearchResult[] {
    // Sample a subset instead of searching all
    let sampleSize = min(1000, store.size())
    let sample = store.randomSample(sampleSize)
    
    // Search only the sample
    return sample.search(queryEmb, k)
}
```

### Parallel Embedding Generation

```klang
// When KLang supports async/await in future:
fn parallelEmbed(texts: string[]) -> Embedding[] {
    // Split into batches
    let batchSize = 100
    let batches = []
    
    let i = 0
    while i < len(texts) {
        let batch = texts[i:min(i + batchSize, len(texts))]
        batches.push(batch)
        i = i + batchSize
    }
    
    // Process batches in parallel
    let allEmbeddings = []
    let i = 0
    while i < len(batches) {
        let embeddings = client.embedBatch(batches[i], "text-embedding-ada-002")
        allEmbeddings = allEmbeddings + embeddings
        i = i + 1
    }
    
    return allEmbeddings
}
```

---

## Next Steps

- **[RAG Systems](33-rag-systems.md)** - Use embeddings for retrieval
- **[AI Agents](34-ai-agents.md)** - Build agents with semantic memory
- **[LLM Integration](31-llm-integration.md)** - Generate embeddings from LLMs

---

## Summary

KLang provides comprehensive embedding support:

✅ **Easy generation** - OpenAI, Ollama, custom providers  
✅ **Rich operations** - Similarity, distance, vector math  
✅ **Semantic search** - Find relevant content by meaning  
✅ **Flexible storage** - Store with metadata and filters  
✅ **Performance** - Caching, batching, optimization  

Start building semantic search with KLang embeddings! 🚀
