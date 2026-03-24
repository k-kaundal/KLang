# AI Examples

## Introduction

KLang is the **first programming language with native AI capabilities**. This guide shows you how to build AI-powered applications using KLang's built-in AI features.

## Prerequisites

To use AI features, you'll need:
- An API key from OpenAI, Anthropic, or a local Ollama installation
- Set environment variables: `OPENAI_API_KEY`, `ANTHROPIC_API_KEY`, or use local Ollama

## Simple LLM Usage

### Example 1: Basic LLM Call

```klang
# Set your API key
# export OPENAI_API_KEY="sk-..."

# Simple question answering
let response = llm("What is the capital of France?")
println(response)
# Output: "The capital of France is Paris."

let explanation = llm("Explain recursion in simple terms")
println(explanation)
```

### Example 2: LLM Client Setup

```klang
# Create an LLM client with options
let client = LLMClient("openai", {
    apiKey: env("OPENAI_API_KEY"),
    model: "gpt-4",
    temperature: 0.7,
    max_tokens: 500
})

let answer = client.chat("Write a haiku about programming")
println(answer)
```

### Example 3: Different AI Providers

```klang
# OpenAI
let openai = LLMClient("openai", {
    apiKey: env("OPENAI_API_KEY"),
    model: "gpt-4"
})

# Anthropic Claude
let claude = LLMClient("anthropic", {
    apiKey: env("ANTHROPIC_API_KEY"),
    model: "claude-3-opus"
})

# Local Ollama
let local = LLMClient("ollama", {
    model: "llama2",
    baseURL: "http://localhost:11434"
})

let question = "What is 2 + 2?"
println("OpenAI: " + openai.chat(question))
println("Claude: " + claude.chat(question))
println("Ollama: " + local.chat(question))
```

## Chat Sessions

### Example 4: Multi-turn Conversation

```klang
let chat = ChatSession("openai", {
    apiKey: env("OPENAI_API_KEY"),
    model: "gpt-4"
})

# First message
let response1 = chat.send("My name is Alice")
println("AI: " + response1)

# Follow-up (AI remembers context)
let response2 = chat.send("What's my name?")
println("AI: " + response2)  # "Your name is Alice"

# Continue conversation
let response3 = chat.send("Tell me a joke about programming")
println("AI: " + response3)

# Get conversation history
let history = chat.getHistory()
println("\nConversation had " + str(len(history)) + " messages")
```

### Example 5: AI Assistant

```klang
class AIAssistant {
    constructor(name) {
        this.name = name
        this.chat = ChatSession("openai", {
            apiKey: env("OPENAI_API_KEY"),
            model: "gpt-4",
            system: "You are a helpful assistant named " + name
        })
    }
    
    ask(question) {
        println("\nYou: " + question)
        let response = this.chat.send(question)
        println(this.name + ": " + response)
        return response
    }
    
    reset() {
        this.chat.clear()
        println("Conversation reset")
    }
}

let assistant = AIAssistant("KLang Helper")
assistant.ask("What is KLang?")
assistant.ask("Can you give me a code example?")
assistant.ask("How do I install it?")
```

## Vector Embeddings

### Example 6: Text Similarity

```klang
let client = LLMClient("openai", {
    apiKey: env("OPENAI_API_KEY")
})

# Generate embeddings
let text1 = "The cat is on the mat"
let text2 = "The feline is on the rug"
let text3 = "Python is a programming language"

let emb1 = client.embeddings(text1)
let emb2 = client.embeddings(text2)
let emb3 = client.embeddings(text3)

# Calculate cosine similarity (simplified)
fn cosineSimilarity(a, b) {
    let dot = 0
    let mag_a = 0
    let mag_b = 0
    
    for i in range(len(a)) {
        dot = dot + (a[i] * b[i])
        mag_a = mag_a + (a[i] * a[i])
        mag_b = mag_b + (b[i] * b[i])
    }
    
    return dot / (sqrt(mag_a) * sqrt(mag_b))
}

println("Similarity (text1, text2): " + str(cosineSimilarity(emb1, emb2)))
println("Similarity (text1, text3): " + str(cosineSimilarity(emb1, emb3)))
```

### Example 7: Semantic Search

```klang
class SemanticSearch {
    constructor() {
        this.client = LLMClient("openai", {
            apiKey: env("OPENAI_API_KEY")
        })
        this.documents = []
    }
    
    addDocument(text) {
        let embedding = this.client.embeddings(text)
        this.documents.push({
            text: text,
            embedding: embedding
        })
        println("Added document: " + substr(text, 0, 50) + "...")
    }
    
    search(query, top_k) {
        let query_embedding = this.client.embeddings(query)
        
        # Calculate similarities
        let results = this.documents.map(doc => {
            return {
                text: doc.text,
                score: this.similarity(query_embedding, doc.embedding)
            }
        })
        
        # Sort by score
        results = results.sort((a, b) => b.score - a.score)
        
        # Return top k
        return results.slice(0, top_k)
    }
    
    similarity(a, b) {
        # Cosine similarity
        let dot = 0
        for i in range(len(a)) {
            dot = dot + (a[i] * b[i])
        }
        return dot  # Simplified
    }
}

let search = SemanticSearch()
search.addDocument("KLang is an AI-native programming language")
search.addDocument("Python is a popular programming language")
search.addDocument("The weather is nice today")

let results = search.search("programming languages", 2)
println("\nSearch Results:")
results.forEach(r => {
    println("Score: " + str(r.score) + " - " + r.text)
})
```

## RAG Systems

### Example 8: Simple RAG

```klang
let rag = RAG({
    provider: "openai",
    apiKey: env("OPENAI_API_KEY")
})

# Add knowledge base
rag.addDocument("KLang supports classes, inheritance, and polymorphism")
rag.addDocument("KLang has built-in HTTP client for API calls")
rag.addDocument("KLang uses reference counting and garbage collection")

# Query with context
let answer = rag.query("Does KLang support OOP?")
println("Answer: " + answer)

let answer2 = rag.query("How does KLang handle memory?")
println("Answer: " + answer2)
```

### Example 9: Document QA System

```klang
class DocumentQA {
    constructor() {
        this.rag = RAG({
            provider: "openai",
            apiKey: env("OPENAI_API_KEY"),
            model: "gpt-4"
        })
        this.doc_count = 0
    }
    
    loadDocument(content, title) {
        # Split into chunks (simplified)
        let chunks = split(content, "\n\n")
        
        chunks.forEach(chunk => {
            if len(trim(chunk)) > 0 {
                this.rag.addDocument(chunk, {
                    source: title,
                    doc_id: this.doc_count
                })
            }
        })
        
        this.doc_count = this.doc_count + 1
        println("Loaded: " + title)
    }
    
    ask(question) {
        println("\nQ: " + question)
        let answer = this.rag.query(question, {
            top_k: 3,
            include_sources: true
        })
        println("A: " + answer)
        return answer
    }
}

let qa = DocumentQA()

# Load documentation
qa.loadDocument("KLang supports variables with let, const, and var keywords", 
                "Variables Guide")
qa.loadDocument("Functions can be declared with fn keyword or arrow syntax",
                "Functions Guide")

# Ask questions
qa.ask("How do I declare variables?")
qa.ask("What is arrow syntax?")
```

## AI Agents

### Example 10: Simple Task Agent

```klang
let agent = Agent({
    name: "TaskHelper",
    provider: "openai",
    apiKey: env("OPENAI_API_KEY"),
    instructions: "You are a helpful task management assistant"
})

let response = agent.run("Create a list of 5 steps to learn a new programming language")
println(response)
```

### Example 11: Code Review Agent

```klang
class CodeReviewer {
    constructor() {
        this.agent = Agent({
            name: "CodeReviewer",
            provider: "openai",
            apiKey: env("OPENAI_API_KEY"),
            model: "gpt-4",
            instructions: "You are an expert code reviewer. Provide constructive feedback."
        })
    }
    
    review(code, language) {
        let prompt = "Review this " + language + " code:\n\n" + code
        return this.agent.run(prompt)
    }
}

let reviewer = CodeReviewer()

let code = `
fn factorial(n) {
    if n <= 1 { return 1 }
    return n * factorial(n - 1)
}
`

let feedback = reviewer.review(code, "KLang")
println("Code Review Feedback:")
println(feedback)
```

### Example 12: Content Generator

```klang
class ContentGenerator {
    constructor() {
        this.client = LLMClient("openai", {
            apiKey: env("OPENAI_API_KEY"),
            model: "gpt-4",
            temperature: 0.8
        })
    }
    
    generateBlogPost(topic, keywords) {
        let prompt = "Write a blog post about '" + topic + "'. " +
                     "Include these keywords: " + join(keywords, ", ")
        
        return this.client.chat(prompt)
    }
    
    generateSummary(text, max_words) {
        let prompt = "Summarize this text in " + str(max_words) + 
                     " words or less:\n\n" + text
        
        return this.client.chat(prompt)
    }
}

let generator = ContentGenerator()

# Generate blog post
let post = generator.generateBlogPost(
    "AI in Software Development",
    ["automation", "productivity", "innovation"]
)
println("Generated Blog Post:")
println(post)

# Generate summary
let long_text = "KLang is a modern programming language that combines traditional programming concepts with AI-native capabilities..."
let summary = generator.generateSummary(long_text, 20)
println("\nSummary:")
println(summary)
```

## Practical AI Applications

### Example 13: Smart Chatbot

```klang
class SmartChatbot {
    constructor(name, personality) {
        this.name = name
        this.session = ChatSession("openai", {
            apiKey: env("OPENAI_API_KEY"),
            model: "gpt-4",
            system: "You are " + name + ". " + personality
        })
        this.message_count = 0
    }
    
    chat(message) {
        this.message_count = this.message_count + 1
        println("\nYou: " + message)
        
        let response = this.session.send(message)
        println(this.name + ": " + response)
        
        return response
    }
    
    stats() {
        println("\nChat Statistics:")
        println("Messages exchanged: " + str(this.message_count))
    }
}

let bot = SmartChatbot("KBot", "You are a friendly and helpful programming assistant")

bot.chat("Hi! What is KLang?")
bot.chat("Can you show me a code example?")
bot.chat("How do I use classes?")
bot.stats()
```

### Example 14: Email Classifier

```klang
class EmailClassifier {
    constructor() {
        this.client = LLMClient("openai", {
            apiKey: env("OPENAI_API_KEY"),
            model: "gpt-4"
        })
    }
    
    classify(email_text) {
        let prompt = "Classify this email as spam, important, or normal. " +
                     "Respond with only one word.\n\nEmail: " + email_text
        
        let category = lowercase(trim(this.client.chat(prompt)))
        return category
    }
    
    extractAction(email_text) {
        let prompt = "What action is required for this email? " +
                     "Respond with one of: reply, forward, archive, delete\n\n" +
                     "Email: " + email_text
        
        return lowercase(trim(this.client.chat(prompt)))
    }
}

let classifier = EmailClassifier()

let emails = [
    "Meeting tomorrow at 10 AM. Please confirm attendance.",
    "You won $1,000,000! Click here now!!!",
    "Quarterly report attached for your review."
]

emails.forEach(email => {
    let category = classifier.classify(email)
    let action = classifier.extractAction(email)
    
    println("\nEmail: " + substr(email, 0, 40) + "...")
    println("Category: " + category)
    println("Action: " + action)
})
```

### Example 15: Code Generator

```klang
class CodeGenerator {
    constructor() {
        this.client = LLMClient("openai", {
            apiKey: env("OPENAI_API_KEY"),
            model: "gpt-4"
        })
    }
    
    generate(description, language) {
        let prompt = "Generate " + language + " code for: " + description +
                     "\n\nProvide only the code, no explanations."
        
        return this.client.chat(prompt)
    }
    
    explainCode(code) {
        let prompt = "Explain what this code does:\n\n" + code
        return this.client.chat(prompt)
    }
}

let codegen = CodeGenerator()

# Generate function
let code = codegen.generate("a function to check if a number is prime", "KLang")
println("Generated Code:")
println(code)

# Explain code
let explanation = codegen.explainCode(code)
println("\nExplanation:")
println(explanation)
```

## RAG Applications

### Example 16: Knowledge Base QA

```klang
class KnowledgeBase {
    constructor(name) {
        this.name = name
        this.rag = RAG({
            provider: "openai",
            apiKey: env("OPENAI_API_KEY"),
            chunk_size: 500
        })
        this.documents = []
    }
    
    addKnowledge(text, metadata) {
        this.rag.addDocument(text, metadata)
        this.documents.push({text: text, metadata: metadata})
        println("Added: " + metadata.title)
    }
    
    query(question) {
        return this.rag.query(question, {
            top_k: 3,
            include_sources: true
        })
    }
    
    stats() {
        println("\nKnowledge Base: " + this.name)
        println("Documents: " + str(len(this.documents)))
    }
}

let kb = KnowledgeBase("KLang Documentation")

# Add documentation
kb.addKnowledge(
    "KLang supports classes with constructors and methods. " +
    "Use the class keyword to define a class.",
    {title: "Classes", category: "OOP"}
)

kb.addKnowledge(
    "Variables in KLang can be declared with let, const, or var. " +
    "let creates mutable variables, const creates immutable ones.",
    {title: "Variables", category: "Basics"}
)

kb.addKnowledge(
    "KLang has built-in LLM support with the llm() function " +
    "and LLMClient class for advanced usage.",
    {title: "AI Features", category: "AI"}
)

# Query the knowledge base
let answer1 = kb.query("How do I create a class?")
println("\nQ: How do I create a class?")
println("A: " + answer1)

let answer2 = kb.query("What are the variable types?")
println("\nQ: What are the variable types?")
println("A: " + answer2)

kb.stats()
```

### Example 17: Document Search Engine

```klang
class DocumentSearch {
    constructor() {
        this.rag = RAG({
            provider: "openai",
            apiKey: env("OPENAI_API_KEY")
        })
        this.index = []
    }
    
    indexDocument(title, content) {
        # Split into paragraphs
        let paragraphs = split(content, "\n\n")
        
        paragraphs.forEach((para, idx) => {
            if len(trim(para)) > 0 {
                this.rag.addDocument(para, {
                    title: title,
                    paragraph: idx
                })
                this.index.push({
                    title: title,
                    paragraph: idx,
                    text: para
                })
            }
        })
        
        println("Indexed: " + title + " (" + str(len(paragraphs)) + " paragraphs)")
    }
    
    search(query) {
        let results = this.rag.query(query, {
            top_k: 5,
            include_sources: true,
            include_scores: true
        })
        
        return results
    }
}

let search_engine = DocumentSearch()

search_engine.indexDocument("Getting Started", 
    "KLang is easy to learn.\n\nIt has clean syntax.\n\nYou can start coding immediately.")

search_engine.indexDocument("Advanced Features",
    "KLang supports OOP.\n\nIt has async/await.\n\nAI integration is built-in.")

let results = search_engine.search("How do I start learning?")
println("\nSearch Results:")
println(results)
```

## AI Agents

### Example 18: Task Automation Agent

```klang
class TaskAgent {
    constructor(name) {
        this.name = name
        this.agent = Agent({
            name: name,
            provider: "openai",
            apiKey: env("OPENAI_API_KEY"),
            instructions: "You are a task automation assistant. " +
                         "Break down complex tasks into steps."
        })
    }
    
    planTask(task_description) {
        let prompt = "Break down this task into clear steps:\n" + task_description
        return this.agent.run(prompt)
    }
    
    suggestTools(task) {
        let prompt = "What tools or technologies would help with: " + task
        return this.agent.run(prompt)
    }
}

let agent = TaskAgent("TaskPlanner")

let plan = agent.planTask("Build a REST API for a todo application")
println("Task Plan:")
println(plan)

let tools = agent.suggestTools("Build a REST API")
println("\nSuggested Tools:")
println(tools)
```

### Example 19: Data Analyzer Agent

```klang
class DataAnalyzer {
    constructor() {
        this.agent = Agent({
            name: "DataAnalyzer",
            provider: "openai",
            apiKey: env("OPENAI_API_KEY"),
            model: "gpt-4",
            instructions: "You are a data analysis expert. Provide insights."
        })
    }
    
    analyze(data_description, data) {
        let prompt = "Analyze this " + data_description + ":\n\n" + str(data) +
                     "\n\nProvide key insights and patterns."
        
        return this.agent.run(prompt)
    }
    
    recommend(data, goal) {
        let prompt = "Based on this data: " + str(data) +
                     "\n\nWhat actions would help achieve: " + goal
        
        return this.agent.run(prompt)
    }
}

let analyzer = DataAnalyzer()

let sales_data = [
    {month: "Jan", sales: 10000},
    {month: "Feb", sales: 12000},
    {month: "Mar", sales: 9000},
    {month: "Apr", sales: 15000}
]

let insights = analyzer.analyze("monthly sales data", sales_data)
println("Analysis:")
println(insights)

let recommendations = analyzer.recommend(sales_data, "increase revenue")
println("\nRecommendations:")
println(recommendations)
```

## Real-World Applications

### Example 20: AI-Powered Support Bot

```klang
class SupportBot {
    constructor(company_name) {
        this.company = company_name
        this.kb = RAG({
            provider: "openai",
            apiKey: env("OPENAI_API_KEY")
        })
        this.chat = ChatSession("openai", {
            apiKey: env("OPENAI_API_KEY"),
            model: "gpt-4",
            system: "You are a support agent for " + company_name
        })
    }
    
    loadFAQ(faqs) {
        faqs.forEach(faq => {
            let doc = "Q: " + faq.question + "\nA: " + faq.answer
            this.kb.addDocument(doc)
        })
        println("Loaded " + str(len(faqs)) + " FAQs")
    }
    
    handleQuery(user_query) {
        println("\nUser: " + user_query)
        
        # Search knowledge base
        let context = this.kb.query(user_query, {top_k: 2})
        
        # Generate response with context
        let prompt = "Context: " + context + "\n\nUser question: " + user_query
        let response = this.chat.send(prompt)
        
        println("Bot: " + response)
        return response
    }
}

let bot = SupportBot("KLang Inc")

# Load FAQ
bot.loadFAQ([
    {question: "How do I install KLang?", 
     answer: "Run: git clone repo, then make"},
    {question: "What is KLang?",
     answer: "An AI-native programming language"},
    {question: "How do I use AI features?",
     answer: "Use llm() function or LLMClient class"}
])

# Handle user queries
bot.handleQuery("How can I get started?")
bot.handleQuery("What AI features are available?")
```

### Example 21: Smart Code Assistant

```klang
class CodeAssistant {
    constructor() {
        this.chat = ChatSession("openai", {
            apiKey: env("OPENAI_API_KEY"),
            model: "gpt-4",
            system: "You are an expert KLang programmer"
        })
    }
    
    help(topic) {
        return this.chat.send("Explain " + topic + " in KLang with example")
    }
    
    debug(code, error) {
        let prompt = "This KLang code has an error:\n\n" + code +
                     "\n\nError: " + error +
                     "\n\nHow do I fix it?"
        return this.chat.send(prompt)
    }
    
    optimize(code) {
        let prompt = "Optimize this KLang code:\n\n" + code
        return this.chat.send(prompt)
    }
}

let assistant = CodeAssistant()

# Get help
let help = assistant.help("closures")
println("Help on closures:")
println(help)

# Debug code
let buggy_code = "let x = 10\nprintn(x)"  # typo: printn
let fix = assistant.debug(buggy_code, "Unknown function: printn")
println("\nDebug suggestion:")
println(fix)
```

### Example 22: Multi-Agent System

```klang
class ResearchAgent {
    constructor() {
        this.agent = Agent({
            name: "Researcher",
            provider: "openai",
            apiKey: env("OPENAI_API_KEY"),
            instructions: "Research topics and provide detailed information"
        })
    }
    
    research(topic) {
        return this.agent.run("Research: " + topic)
    }
}

class WriterAgent {
    constructor() {
        this.agent = Agent({
            name: "Writer",
            provider: "openai",
            apiKey: env("OPENAI_API_KEY"),
            instructions: "Write clear and engaging content"
        })
    }
    
    write(research, format) {
        return this.agent.run("Write a " + format + " based on:\n" + research)
    }
}

# Multi-agent workflow
let researcher = ResearchAgent()
let writer = WriterAgent()

let topic = "Benefits of functional programming"
println("Topic: " + topic)

# Step 1: Research
let research = researcher.research(topic)
println("\nResearch findings:")
println(research)

# Step 2: Write article
let article = writer.write(research, "blog post")
println("\nGenerated Article:")
println(article)
```

## Combining AI with Traditional Code

### Example 23: Smart Recommendation System

```klang
class RecommendationSystem {
    constructor() {
        this.client = LLMClient("openai", {
            apiKey: env("OPENAI_API_KEY")
        })
        this.embeddings_cache = {}
    }
    
    getEmbedding(text) {
        # Cache embeddings
        if text in this.embeddings_cache {
            return this.embeddings_cache[text]
        }
        
        let embedding = this.client.embeddings(text)
        this.embeddings_cache[text] = embedding
        return embedding
    }
    
    recommend(user_preferences, items, top_n) {
        # Get embedding for user preferences
        let pref_embedding = this.getEmbedding(user_preferences)
        
        # Calculate similarity for each item
        let scored_items = items.map(item => {
            let item_embedding = this.getEmbedding(item.description)
            let score = this.similarity(pref_embedding, item_embedding)
            
            return {
                item: item,
                score: score
            }
        })
        
        # Sort by score and return top N
        scored_items = scored_items.sort((a, b) => b.score - a.score)
        return scored_items.slice(0, top_n)
    }
    
    similarity(a, b) {
        # Simplified cosine similarity
        let dot = 0
        for i in range(len(a)) {
            dot = dot + (a[i] * b[i])
        }
        return dot
    }
}

let recommender = RecommendationSystem()

let user_prefs = "I like science fiction movies with space exploration"

let movies = [
    {title: "Interstellar", description: "Space exploration and time travel"},
    {title: "The Godfather", description: "Crime family drama"},
    {title: "Gravity", description: "Astronaut survival in space"},
    {title: "Inception", description: "Dreams and reality manipulation"}
]

let recommendations = recommender.recommend(user_prefs, movies, 3)

println("Recommendations based on: " + user_prefs)
println("\nTop recommendations:")
recommendations.forEach((rec, idx) => {
    println(str(idx + 1) + ". " + rec.item.title + 
            " (Score: " + str(rec.score) + ")")
})
```

## Tips & Best Practices

### AI API Usage

```klang
# Always handle API errors
fn safeAICall(prompt) {
    let client = LLMClient("openai", {
        apiKey: env("OPENAI_API_KEY")
    })
    
    let response = client.chat(prompt)
    
    if response == null or len(response) == 0 {
        return "AI service unavailable"
    }
    
    return response
}

# Cache responses to save API calls
let response_cache = {}

fn cachedAICall(prompt) {
    if prompt in response_cache {
        println("Using cached response")
        return response_cache[prompt]
    }
    
    let response = llm(prompt)
    response_cache[prompt] = response
    return response
}
```

### Rate Limiting

```klang
class RateLimitedClient {
    constructor() {
        this.client = LLMClient("openai", {
            apiKey: env("OPENAI_API_KEY")
        })
        this.call_count = 0
        this.max_calls = 10
    }
    
    chat(message) {
        if this.call_count >= this.max_calls {
            return "Rate limit reached. Please try again later."
        }
        
        this.call_count = this.call_count + 1
        return this.client.chat(message)
    }
    
    reset() {
        this.call_count = 0
        println("Rate limit reset")
    }
}
```

## Next Steps

- [Project Examples](63-examples-projects.md) - Complete AI projects
- [AI Overview](30-ai-overview.md) - Detailed AI features guide
- [LLM Integration](31-llm-integration.md) - Advanced LLM usage
- [RAG Systems](33-rag-systems.md) - Building RAG applications

---

**Build intelligent applications with KLang's AI capabilities!** 🤖
