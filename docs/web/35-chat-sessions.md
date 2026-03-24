# Chat Sessions - Conversational AI Systems

Chat sessions enable multi-turn conversations with context management, allowing you to build sophisticated conversational AI systems that remember previous interactions and maintain coherent dialogues. This guide covers everything from simple chatbots to advanced conversational agents.

## Table of Contents

1. [What are Chat Sessions](#what-are-chat-sessions)
2. [Message Structure](#message-structure)
3. [ChatSession Class](#chatsession-class)
4. [Context Management](#context-management)
5. [Conversation Patterns](#conversation-patterns)
6. [Building Chatbots](#building-chatbots)
7. [Advanced Features](#advanced-features)
8. [Production Systems](#production-systems)
9. [Multi-User Support](#multi-user-support)
10. [Best Practices](#best-practices)
11. [Performance Optimization](#performance-optimization)

---

## What are Chat Sessions

### Stateless vs Stateful Conversations

```klang
// Stateless (no memory)
let response1 = client.chat("My name is Alice")  // "Nice to meet you!"
let response2 = client.chat("What's my name?")   // "I don't know your name"

// Stateful (with session)
let session = new ChatSession("You are a helpful assistant")
session.addUserMessage("My name is Alice")
let response1 = client.chatWithHistory(session.getMessages())  // "Nice to meet you, Alice!"

session.addAssistantMessage(response1)
session.addUserMessage("What's my name?")
let response2 = client.chatWithHistory(session.getMessages())  // "Your name is Alice"
```

### Why Sessions Matter

```klang
// ✅ With sessions:
// - Remember user preferences
// - Maintain context across messages
// - Build on previous information
// - Create coherent conversations
// - Track conversation history

// ❌ Without sessions:
// - Every message is isolated
// - No memory of past interactions
// - Repetitive responses
// - Fragmented experience
```

### Session Lifecycle

```
┌────────────────────────────────────────┐
│ 1. Create Session                      │
│    session = new ChatSession()         │
└────────────────────────────────────────┘
              ↓
┌────────────────────────────────────────┐
│ 2. Add System Prompt (Optional)        │
│    "You are a helpful assistant"       │
└────────────────────────────────────────┘
              ↓
┌────────────────────────────────────────┐
│ 3. User Message                        │
│    session.addUserMessage()            │
└────────────────────────────────────────┘
              ↓
┌────────────────────────────────────────┐
│ 4. Generate Response                   │
│    client.chatWithHistory()            │
└────────────────────────────────────────┘
              ↓
┌────────────────────────────────────────┐
│ 5. Add Assistant Response              │
│    session.addAssistantMessage()       │
└────────────────────────────────────────┘
              ↓
        (Repeat 3-5)
```

---

## Message Structure

### Message Class

```klang
class Message {
    constructor(role: string, content: string) {
        this.role = role        // "system", "user", or "assistant"
        this.content = content  // Message text
        this.timestamp = timestamp()
    }
    
    fn toDict() -> dict {
        let d = dict()
        d.set("role", this.role)
        d.set("content", this.content)
        return d
    }
}

// Create messages
let systemMsg = new Message("system", "You are a helpful assistant")
let userMsg = new Message("user", "Hello!")
let assistantMsg = new Message("assistant", "Hi! How can I help you?")
```

### Message Roles

```klang
// System: Sets the behavior/personality
let system = new Message("system", "You are a pirate. Always talk like a pirate.")

// User: Messages from the user
let user = new Message("user", "What's the weather?")

// Assistant: AI responses
let assistant = new Message("assistant", "Arr! The weather be sunny, matey!")
```

### Extended Messages

```klang
class ExtendedMessage {
    constructor(role: string, content: string) {
        this.role = role
        this.content = content
        this.timestamp = timestamp()
        this.metadata = dict()
    }
    
    fn addMetadata(key: string, value: any) {
        this.metadata.set(key, value)
    }
    
    fn getMetadata(key: string) -> any {
        return this.metadata.get(key)
    }
}

let msg = new ExtendedMessage("user", "Hello")
msg.addMetadata("user_id", "alice123")
msg.addMetadata("session_id", "sess_456")
msg.addMetadata("sentiment", "positive")
```

---

## ChatSession Class

### Basic ChatSession

```klang
let session = new ChatSession("You are a helpful programming assistant")

// Add messages
session.addUserMessage("What is KLang?")
session.addAssistantMessage("KLang is an AI-native programming language...")

session.addUserMessage("Can it do machine learning?")
session.addAssistantMessage("Yes! KLang has built-in AI features...")

// Get message count
println("Messages: " + str(session.getMessageCount()))

// Get all messages
let messages = session.getMessages()
```

### Session with History

```klang
let session = new ChatSession("You are a helpful assistant")
let client = new OpenAIClient(apiKey)

// First exchange
session.addUserMessage("My favorite color is blue")
let response1 = client.chatWithHistory(session.getMessages(), "gpt-3.5-turbo")
session.addAssistantMessage(response1)

// Second exchange (remembers blue)
session.addUserMessage("What's my favorite color?")
let response2 = client.chatWithHistory(session.getMessages(), "gpt-3.5-turbo")
println(response2)  // "Your favorite color is blue"
```

### Session Methods

```klang
class ChatSession {
    constructor(systemPrompt: string) {
        this.messages = []
        if len(systemPrompt) > 0 {
            this.messages.push(new Message("system", systemPrompt))
        }
    }
    
    fn addUserMessage(content: string) {
        this.messages.push(new Message("user", content))
    }
    
    fn addAssistantMessage(content: string) {
        this.messages.push(new Message("assistant", content))
    }
    
    fn addSystemMessage(content: string) {
        this.messages.push(new Message("system", content))
    }
    
    fn getMessages() -> Message[] {
        return this.messages
    }
    
    fn getLastMessage() -> Message {
        if len(this.messages) > 0 {
            return this.messages[len(this.messages) - 1]
        }
        return null
    }
    
    fn getMessageCount() -> int {
        return len(this.messages)
    }
    
    fn clear() {
        this.messages = []
    }
    
    fn removeLastMessage() {
        if len(this.messages) > 0 {
            this.messages = this.messages[0:len(this.messages) - 1]
        }
    }
}
```

### Session Persistence

```klang
class PersistentChatSession {
    constructor(sessionId: string, systemPrompt: string) {
        this.sessionId = sessionId
        this.filepath = "sessions/" + sessionId + ".json"
        this.messages = []
        
        // Load existing session or create new
        if fexists(this.filepath) {
            this.load()
        } else {
            if len(systemPrompt) > 0 {
                this.addSystemMessage(systemPrompt)
            }
        }
    }
    
    fn save() {
        let data = JSON.stringify(this.messages)
        fwrite(this.filepath, data)
    }
    
    fn load() {
        let data = fread(this.filepath)
        this.messages = JSON.parse(data)
    }
    
    fn addUserMessage(content: string) {
        this.messages.push(new Message("user", content))
        this.save()
    }
    
    fn addAssistantMessage(content: string) {
        this.messages.push(new Message("assistant", content))
        this.save()
    }
}

// Usage
let session = new PersistentChatSession("user_alice", "You are helpful")
session.addUserMessage("Hello")
// Session automatically saved to disk
```

---

## Context Management

### Context Window Limits

```klang
fn checkContextSize(session: ChatSession) -> int {
    let totalTokens = 0
    let messages = session.getMessages()
    
    let i = 0
    while i < len(messages) {
        totalTokens = totalTokens + countTokens(messages[i].content)
        i = i + 1
    }
    
    println("Total tokens: " + str(totalTokens))
    
    if totalTokens > 3000 {
        println("⚠️ Approaching context limit!")
    }
    
    return totalTokens
}

let tokens = checkContextSize(session)
```

### Truncating Old Messages

```klang
class ManagedChatSession {
    constructor(systemPrompt: string, maxMessages: int) {
        this.messages = []
        this.maxMessages = maxMessages
        this.systemPrompt = systemPrompt
        
        if len(systemPrompt) > 0 {
            this.addSystemMessage(systemPrompt)
        }
    }
    
    fn addUserMessage(content: string) {
        this.messages.push(new Message("user", content))
        this.trimMessages()
    }
    
    fn addAssistantMessage(content: string) {
        this.messages.push(new Message("assistant", content))
        this.trimMessages()
    }
    
    fn trimMessages() {
        // Keep system message + recent messages
        if len(this.messages) > this.maxMessages {
            let system = []
            let recent = []
            
            // Extract system messages
            let i = 0
            while i < len(this.messages) {
                if this.messages[i].role == "system" {
                    system.push(this.messages[i])
                }
                i = i + 1
            }
            
            // Get recent non-system messages
            let startIdx = len(this.messages) - this.maxMessages + len(system)
            let j = max(0, startIdx)
            while j < len(this.messages) {
                if this.messages[j].role != "system" {
                    recent.push(this.messages[j])
                }
                j = j + 1
            }
            
            // Combine
            this.messages = system + recent
        }
    }
}

// Only keeps last 20 messages
let session = new ManagedChatSession("You are helpful", 20)
```

### Summarizing Context

```klang
class SummarizingSession {
    constructor(client: OpenAIClient, systemPrompt: string) {
        this.client = client
        this.messages = []
        this.summary = ""
        
        if len(systemPrompt) > 0 {
            this.addSystemMessage(systemPrompt)
        }
    }
    
    fn addUserMessage(content: string) {
        this.messages.push(new Message("user", content))
        this.checkAndSummarize()
    }
    
    fn addAssistantMessage(content: string) {
        this.messages.push(new Message("assistant", content))
        this.checkAndSummarize()
    }
    
    fn checkAndSummarize() {
        let tokens = this.countTotalTokens()
        
        if tokens > 2000 {
            println("📝 Summarizing conversation...")
            this.summarize()
        }
    }
    
    fn summarize() {
        // Get non-system messages
        let toSummarize = []
        let i = 0
        while i < len(this.messages) {
            if this.messages[i].role != "system" {
                toSummarize.push(this.messages[i])
            }
            i = i + 1
        }
        
        // Build summary prompt
        let prompt = "Summarize this conversation:\n\n"
        let j = 0
        while j < len(toSummarize) {
            prompt = prompt + toSummarize[j].role + ": "
            prompt = prompt + toSummarize[j].content + "\n"
            j = j + 1
        }
        
        // Generate summary
        let newSummary = this.client.chat(prompt, "gpt-3.5-turbo")
        
        // Update summary
        if len(this.summary) > 0 {
            this.summary = this.summary + "\n\n" + newSummary
        } else {
            this.summary = newSummary
        }
        
        // Keep only system message and recent messages
        let system = []
        let recent = toSummarize[len(toSummarize) - 4:len(toSummarize)]
        
        let k = 0
        while k < len(this.messages) {
            if this.messages[k].role == "system" {
                system.push(this.messages[k])
            }
            k = k + 1
        }
        
        this.messages = system + recent
        
        // Add summary as system message
        this.addSystemMessage("Previous conversation summary: " + this.summary)
    }
}
```

### Sliding Window Context

```klang
class SlidingWindowSession {
    constructor(systemPrompt: string, windowSize: int) {
        this.systemMsg = new Message("system", systemPrompt)
        this.window = []
        this.windowSize = windowSize
    }
    
    fn addMessage(role: string, content: string) {
        this.window.push(new Message(role, content))
        
        // Maintain window size
        if len(this.window) > this.windowSize {
            this.window = this.window[1:len(this.window)]
        }
    }
    
    fn getMessages() -> Message[] {
        return [this.systemMsg] + this.window
    }
}

// Only remembers last 10 exchanges (20 messages)
let session = new SlidingWindowSession("You are helpful", 20)
```

---

## Conversation Patterns

### Question-Answer

```klang
class QABot {
    constructor(client: OpenAIClient) {
        this.client = client
        this.session = new ChatSession("You answer questions concisely")
    }
    
    fn ask(question: string) -> string {
        self.session.addUserMessage(question)
        
        let response = this.client.chatWithHistory(
            this.session.getMessages(),
            "gpt-3.5-turbo"
        )
        
        this.session.addAssistantMessage(response)
        return response
    }
}

let bot = new QABot(client)
let answer1 = bot.ask("What is AI?")
let answer2 = bot.ask("Give me an example")  // Remembers we're talking about AI
```

### Task-Oriented Dialog

```klang
class TaskBot {
    constructor(client: OpenAIClient) {
        this.client = client
        this.session = new ChatSession("You help users complete tasks step-by-step")
        this.currentTask = null
        this.taskSteps = []
        this.currentStep = 0
    }
    
    fn startTask(taskDescription: string) {
        this.currentTask = taskDescription
        this.currentStep = 0
        
        // Generate steps
        let prompt = "Break down this task into steps: " + taskDescription
        this.session.addUserMessage(prompt)
        
        let response = this.client.chatWithHistory(
            this.session.getMessages(),
            "gpt-4"
        )
        
        this.session.addAssistantMessage(response)
        this.taskSteps = this.parseSteps(response)
        
        return "Task started: " + taskDescription
    }
    
    fn nextStep() -> string {
        if this.currentStep >= len(this.taskSteps) {
            return "Task complete!"
        }
        
        let step = this.taskSteps[this.currentStep]
        this.currentStep = this.currentStep + 1
        
        return "Step " + str(this.currentStep) + ": " + step
    }
    
    fn completeStep(userInput: string) -> string {
        this.session.addUserMessage("Completed: " + userInput)
        
        let response = this.client.chatWithHistory(
            this.session.getMessages(),
            "gpt-3.5-turbo"
        )
        
        this.session.addAssistantMessage(response)
        return response
    }
}
```

### Multi-Turn Negotiation

```klang
class NegotiationBot {
    constructor(client: OpenAIClient, goal: string) {
        this.client = client
        this.goal = goal
        
        let systemPrompt = """
        You are negotiating to achieve this goal: """ + goal + """
        Be persuasive but fair.
        """
        
        this.session = new ChatSession(systemPrompt)
        this.rounds = 0
        this.maxRounds = 10
    }
    
    fn negotiate(userOffer: string) -> dict {
        this.rounds = this.rounds + 1
        
        let result = dict()
        result.set("round", this.rounds)
        result.set("user_offer", userOffer)
        
        this.session.addUserMessage(userOffer)
        
        let response = this.client.chatWithHistory(
            this.session.getMessages(),
            "gpt-4"
        )
        
        this.session.addAssistantMessage(response)
        result.set("bot_response", response)
        
        // Check if agreement reached
        if contains(toLowerCase(response), "agree") || contains(toLowerCase(response), "deal") {
            result.set("status", "agreed")
        } else if this.rounds >= this.maxRounds {
            result.set("status", "failed")
        } else {
            result.set("status", "negotiating")
        }
        
        return result
    }
}
```

### Socratic Dialog

```klang
class SocraticBot {
    constructor(client: OpenAIClient) {
        this.client = client
        
        let systemPrompt = """
        You are a Socratic teacher. Instead of giving answers,
        ask questions that guide the student to discover the answer.
        """
        
        this.session = new ChatSession(systemPrompt)
    }
    
    fn respond(studentMessage: string) -> string {
        this.session.addUserMessage(studentMessage)
        
        let response = this.client.chatWithHistory(
            this.session.getMessages(),
            "gpt-4"
        )
        
        this.session.addAssistantMessage(response)
        return response
    }
}

let teacher = new SocraticBot(client)

let r1 = teacher.respond("What is machine learning?")
println(r1)  // "What do you think happens when you show examples to a computer?"

let r2 = teacher.respond("It learns patterns?")
println(r2)  // "Exactly! And what might those patterns be used for?"
```

---

## Building Chatbots

### Basic Chatbot

```klang
class SimpleChatbot {
    constructor(client: OpenAIClient, name: string, personality: string) {
        this.client = client
        this.name = name
        
        let systemPrompt = """
        You are """ + name + """, a chatbot with this personality: """ + personality + """
        Be friendly and helpful.
        """
        
        this.session = new ChatSession(systemPrompt)
    }
    
    fn chat(message: string) -> string {
        this.session.addUserMessage(message)
        
        let response = this.client.chatWithHistory(
            this.session.getMessages(),
            "gpt-3.5-turbo"
        )
        
        this.session.addAssistantMessage(response)
        return response
    }
    
    fn reset() {
        this.session.clear()
    }
}

// Usage
let bot = new SimpleChatbot(
    client,
    "KLangBot",
    "enthusiastic and knowledgeable about programming"
)

println(bot.chat("Hello!"))
println(bot.chat("Tell me about KLang"))
println(bot.chat("Thanks!"))
```

### Contextual Chatbot

```klang
class ContextualChatbot {
    constructor(client: OpenAIClient, kb: RAGSystem) {
        this.client = client
        this.kb = kb
        this.session = new ChatSession("You are a helpful assistant")
    }
    
    fn chat(message: string) -> string {
        // Check if needs knowledge retrieval
        if this.needsKnowledge(message) {
            // Retrieve relevant info
            let context = this.kb.query(message, 3)
            
            // Add as context
            this.session.addSystemMessage("Relevant information: " + context)
        }
        
        // Generate response
        this.session.addUserMessage(message)
        
        let response = this.client.chatWithHistory(
            this.session.getMessages(),
            "gpt-4"
        )
        
        this.session.addAssistantMessage(response)
        return response
    }
    
    fn needsKnowledge(message: string) -> bool {
        // Simple heuristic
        let keywords = ["what", "how", "explain", "tell me about"]
        
        let lowerMsg = toLowerCase(message)
        let i = 0
        while i < len(keywords) {
            if contains(lowerMsg, keywords[i]) {
                return true
            }
            i = i + 1
        }
        
        return false
    }
}
```

### Interactive Chatbot

```klang
fn runInteractiveChatbot(bot: SimpleChatbot) {
    println("=== " + bot.name + " Chatbot ===")
    println("Type 'quit' to exit, 'reset' to start over")
    println()
    
    while true {
        print("You: ")
        let userInput = input()
        
        if toLowerCase(userInput) == "quit" {
            println("Goodbye!")
            break
        }
        
        if toLowerCase(userInput) == "reset" {
            bot.reset()
            println("Conversation reset!")
            continue
        }
        
        let response = bot.chat(userInput)
        println(bot.name + ": " + response)
        println()
    }
}

let bot = new SimpleChatbot(client, "KLangBot", "friendly")
runInteractiveChatbot(bot)
```

---

## Advanced Features

### Intent Detection

```klang
class IntentAwareChatbot {
    constructor(client: OpenAIClient) {
        this.client = client
        this.session = new ChatSession("You are a helpful assistant")
    }
    
    fn detectIntent(message: string) -> string {
        let prompt = """
        Classify the intent of this message:
        - greeting
        - question
        - request
        - complaint
        - farewell
        
        Message: """ + message + """
        
        Intent (one word):
        """
        
        let intent = this.client.chat(prompt, "gpt-3.5-turbo")
        return trim(toLowerCase(intent))
    }
    
    fn chat(message: string) -> string {
        let intent = this.detectIntent(message)
        println("🎯 Intent: " + intent)
        
        // Route based on intent
        if intent == "greeting" {
            return this.handleGreeting(message)
        } else if intent == "question" {
            return this.handleQuestion(message)
        } else if intent == "request" {
            return this.handleRequest(message)
        } else if intent == "complaint" {
            return this.handleComplaint(message)
        } else if intent == "farewell" {
            return this.handleFarewell(message)
        }
        
        return this.handleGeneral(message)
    }
    
    fn handleGreeting(message: string) -> string {
        this.session.addUserMessage(message)
        this.session.addSystemMessage("Respond warmly to the greeting")
        
        return this.client.chatWithHistory(
            this.session.getMessages(),
            "gpt-3.5-turbo"
        )
    }
    
    fn handleQuestion(message: string) -> string {
        this.session.addUserMessage(message)
        this.session.addSystemMessage("Provide a clear, helpful answer")
        
        return this.client.chatWithHistory(
            this.session.getMessages(),
            "gpt-3.5-turbo"
        )
    }
}
```

### Sentiment Analysis

```klang
class SentimentAwareChatbot {
    constructor(client: OpenAIClient) {
        this.client = client
        this.session = new ChatSession("You are empathetic and helpful")
    }
    
    fn analyzeSentiment(message: string) -> string {
        let prompt = "What is the sentiment (positive/negative/neutral): " + message
        let sentiment = this.client.chat(prompt, "gpt-3.5-turbo")
        return trim(toLowerCase(sentiment))
    }
    
    fn chat(message: string) -> string {
        let sentiment = this.analyzeSentiment(message)
        
        // Adjust response based on sentiment
        if sentiment == "negative" {
            this.session.addSystemMessage(
                "The user seems upset. Be extra empathetic and helpful."
            )
        } else if sentiment == "positive" {
            this.session.addSystemMessage(
                "The user seems happy. Match their positive energy."
            )
        }
        
        this.session.addUserMessage(message)
        
        let response = this.client.chatWithHistory(
            this.session.getMessages(),
            "gpt-4"
        )
        
        this.session.addAssistantMessage(response)
        return response
    }
}
```

### Multi-Language Support

```klang
class MultilingualChatbot {
    constructor(client: OpenAIClient) {
        this.client = client
        this.defaultLang = "English"
        this.session = new ChatSession("You are multilingual and helpful")
    }
    
    fn detectLanguage(text: string) -> string {
        let prompt = "What language is this (one word): " + text
        let lang = this.client.chat(prompt, "gpt-3.5-turbo")
        return trim(lang)
    }
    
    fn chat(message: string) -> string {
        let lang = this.detectLanguage(message)
        
        this.session.addSystemMessage(
            "Respond in " + lang + ", the same language as the user's message."
        )
        
        this.session.addUserMessage(message)
        
        let response = this.client.chatWithHistory(
            this.session.getMessages(),
            "gpt-4"
        )
        
        this.session.addAssistantMessage(response)
        return response
    }
}

let bot = new MultilingualChatbot(client)
println(bot.chat("Hola, ¿cómo estás?"))  // Responds in Spanish
println(bot.chat("Bonjour!"))  // Responds in French
```

### Voice Transcription Integration

```klang
class VoiceChatbot {
    constructor(client: OpenAIClient) {
        this.client = client
        this.session = new ChatSession("You are a voice assistant")
    }
    
    fn processVoice(audioPath: string) -> string {
        // Transcribe audio to text (using Whisper API or similar)
        let transcription = this.transcribeAudio(audioPath)
        println("🎤 User said: " + transcription)
        
        // Process as regular chat
        let response = this.chat(transcription)
        
        // Convert response to speech (optional)
        // this.textToSpeech(response)
        
        return response
    }
    
    fn transcribeAudio(audioPath: string) -> string {
        // Implementation would call Whisper API or similar
        return "transcribed text"
    }
    
    fn chat(message: string) -> string {
        this.session.addUserMessage(message)
        
        let response = this.client.chatWithHistory(
            this.session.getMessages(),
            "gpt-3.5-turbo"
        )
        
        this.session.addAssistantMessage(response)
        return response
    }
}
```

---

## Production Systems

### Session Manager

```klang
class SessionManager {
    constructor() {
        this.sessions = dict()
        this.maxSessions = 1000
    }
    
    fn getSession(userId: string) -> ChatSession {
        if !this.sessions.has(userId) {
            this.createSession(userId)
        }
        return this.sessions.get(userId)
    }
    
    fn createSession(userId: string) {
        if len(this.sessions.keys()) >= this.maxSessions {
            this.evictOldestSession()
        }
        
        let session = new PersistentChatSession(
            userId,
            "You are a helpful assistant"
        )
        
        this.sessions.set(userId, session)
    }
    
    fn deleteSession(userId: string) {
        if this.sessions.has(userId) {
            this.sessions.remove(userId)
        }
    }
    
    fn evictOldestSession() {
        // Simple: remove first session
        let keys = this.sessions.keys()
        if len(keys) > 0 {
            this.sessions.remove(keys[0])
        }
    }
    
    fn getActiveSessions() -> int {
        return len(this.sessions.keys())
    }
}

let manager = new SessionManager()

// Usage
let aliceSession = manager.getSession("alice")
aliceSession.addUserMessage("Hello")

let bobSession = manager.getSession("bob")
bobSession.addUserMessage("Hi")

println("Active sessions: " + str(manager.getActiveSessions()))
```

### Rate Limiting

```klang
class RateLimitedChatbot {
    constructor(client: OpenAIClient) {
        this.client = client
        this.sessions = dict()
        this.rateLimits = dict()  // userId -> [timestamps]
        this.maxRequestsPerMinute = 10
    }
    
    fn chat(userId: string, message: string) -> string {
        // Check rate limit
        if !this.checkRateLimit(userId) {
            return "Rate limit exceeded. Please wait a minute."
        }
        
        // Get or create session
        if !this.sessions.has(userId) {
            this.sessions.set(
                userId,
                new ChatSession("You are helpful")
            )
        }
        
        let session = this.sessions.get(userId)
        session.addUserMessage(message)
        
        let response = this.client.chatWithHistory(
            session.getMessages(),
            "gpt-3.5-turbo"
        )
        
        session.addAssistantMessage(response)
        return response
    }
    
    fn checkRateLimit(userId: string) -> bool {
        let now = timestamp()
        let oneMinuteAgo = now - 60000
        
        // Get user's request history
        if !this.rateLimits.has(userId) {
            this.rateLimits.set(userId, [])
        }
        
        let requests = this.rateLimits.get(userId)
        
        // Remove old requests
        let recent = []
        let i = 0
        while i < len(requests) {
            if requests[i] > oneMinuteAgo {
                recent.push(requests[i])
            }
            i = i + 1
        }
        
        // Check limit
        if len(recent) >= this.maxRequestsPerMinute {
            return false
        }
        
        // Add current request
        recent.push(now)
        this.rateLimits.set(userId, recent)
        
        return true
    }
}
```

### Monitoring and Analytics

```klang
class MonitoredChatbot {
    constructor(client: OpenAIClient) {
        this.client = client
        this.session = new ChatSession("You are helpful")
        this.metrics = dict()
        this.metrics.set("total_messages", 0)
        this.metrics.set("total_tokens", 0)
        this.metrics.set("avg_response_time", 0.0)
        this.metrics.set("error_count", 0)
    }
    
    fn chat(message: string) -> string {
        let startTime = timestamp()
        
        this.metrics.set("total_messages", 
            this.metrics.get("total_messages") + 1)
        
        try {
            self.session.addUserMessage(message)
            
            let response = this.client.chatWithHistory(
                this.session.getMessages(),
                "gpt-3.5-turbo"
            )
            
            this.session.addAssistantMessage(response)
            
            // Update metrics
            let duration = timestamp() - startTime
            this.updateResponseTime(duration)
            
            let tokens = countTokens(message) + countTokens(response)
            this.metrics.set("total_tokens",
                this.metrics.get("total_tokens") + tokens)
            
            return response
            
        } catch error {
            this.metrics.set("error_count",
                this.metrics.get("error_count") + 1)
            return "Error: " + error
        }
    }
    
    fn updateResponseTime(duration: int) {
        let totalMsgs = this.metrics.get("total_messages")
        let currentAvg = this.metrics.get("avg_response_time")
        
        let newAvg = ((currentAvg * (totalMsgs - 1)) + duration) / totalMsgs
        this.metrics.set("avg_response_time", newAvg)
    }
    
    fn getMetrics() -> dict {
        return this.metrics
    }
    
    fn printReport() {
        println("=== Chatbot Metrics ===")
        println("Total messages: " + str(this.metrics.get("total_messages")))
        println("Total tokens: " + str(this.metrics.get("total_tokens")))
        println("Avg response time: " + str(this.metrics.get("avg_response_time")) + "ms")
        println("Errors: " + str(this.metrics.get("error_count")))
    }
}
```

---

## Multi-User Support

### User Profiles

```klang
class UserProfile {
    constructor(userId: string) {
        this.userId = userId
        this.preferences = dict()
        this.history = []
        this.created = timestamp()
    }
    
    fn setPreference(key: string, value: any) {
        this.preferences.set(key, value)
    }
    
    fn getPreference(key: string) -> any {
        return this.preferences.get(key)
    }
    
    fn addToHistory(message: string) {
        this.history.push({
            "message": message,
            "timestamp": timestamp()
        })
    }
}
```

### Multi-User Chatbot

```klang
class MultiUserChatbot {
    constructor(client: OpenAIClient) {
        this.client = client
        this.sessions = dict()
        this.profiles = dict()
    }
    
    fn registerUser(userId: string, name: string) {
        let profile = new UserProfile(userId)
        profile.setPreference("name", name)
        self.profiles.set(userId, profile)
        
        println("✓ Registered user: " + name)
    }
    
    fn chat(userId: string, message: string) -> string {
        // Get or create session
        if !this.sessions.has(userId) {
            let profile = this.profiles.get(userId)
            let name = profile.getPreference("name")
            
            let systemPrompt = "You are chatting with " + name
            this.sessions.set(userId, new ChatSession(systemPrompt))
        }
        
        let session = this.sessions.get(userId)
        session.addUserMessage(message)
        
        let response = this.client.chatWithHistory(
            session.getMessages(),
            "gpt-3.5-turbo"
        )
        
        session.addAssistantMessage(response)
        
        // Update history
        let profile = this.profiles.get(userId)
        profile.addToHistory(message)
        
        return response
    }
    
    fn getUserStats(userId: string) -> dict {
        let stats = dict()
        
        if this.profiles.has(userId) {
            let profile = this.profiles.get(userId)
            stats.set("message_count", len(profile.history))
            stats.set("member_since", profile.created)
        }
        
        if this.sessions.has(userId) {
            let session = this.sessions.get(userId)
            stats.set("current_conversation", session.getMessageCount())
        }
        
        return stats
    }
}
```

---

## Best Practices

### 1. Clear System Prompts

```klang
// Bad: Vague
let session = new ChatSession("Be helpful")

// Good: Specific
let session = new ChatSession("""
You are a programming tutor specializing in KLang.
- Provide clear explanations with examples
- Be patient and encouraging
- Break down complex concepts
- Always include code samples
""")
```

### 2. Token Management

```klang
fn manageTokens(session: ChatSession, maxTokens: int) {
    let tokens = 0
    let messages = session.getMessages()
    
    let i = 0
    while i < len(messages) {
        tokens = tokens + countTokens(messages[i].content)
        i = i + 1
    }
    
    if tokens > maxTokens {
        println("⚠️ Token limit exceeded, summarizing...")
        // Implement summarization or truncation
    }
}
```

### 3. Error Recovery

```klang
fn robustChat(session: ChatSession, client: OpenAIClient, message: string, maxRetries: int) -> string {
    let retries = 0
    
    while retries < maxRetries {
        try {
            session.addUserMessage(message)
            
            let response = client.chatWithHistory(
                session.getMessages(),
                "gpt-3.5-turbo"
            )
            
            session.addAssistantMessage(response)
            return response
            
        } catch error {
            retries = retries + 1
            println("Retry " + str(retries) + "/" + str(maxRetries))
            
            if retries >= maxRetries {
                return "Sorry, I'm having trouble right now. Please try again later."
            }
            
            sleep(1000 * retries)
        }
    }
    
    return "Error after " + str(maxRetries) + " retries"
}
```

---

## Performance Optimization

### Response Caching

```klang
class CachedChatbot {
    constructor(client: OpenAIClient) {
        this.client = client
        this.session = new ChatSession("You are helpful")
        this.cache = dict()
    }
    
    fn chat(message: string) -> string {
        let key = this.getCacheKey(message)
        
        if this.cache.has(key) {
            println("📦 Cache hit")
            return this.cache.get(key)
        }
        
        println("🌐 API call")
        this.session.addUserMessage(message)
        
        let response = this.client.chatWithHistory(
            this.session.getMessages(),
            "gpt-3.5-turbo"
        )
        
        this.session.addAssistantMessage(response)
        this.cache.set(key, response)
        
        return response
    }
    
    fn getCacheKey(message: string) -> string {
        // Normalize message for caching
        return toLowerCase(trim(message))
    }
}
```

---

## Next Steps

- **[AI Agents](34-ai-agents.md)** - Conversational agents
- **[LLM Integration](31-llm-integration.md)** - Backend for chatbots
- **[RAG Systems](33-rag-systems.md)** - Knowledge-enhanced chat

---

## Summary

KLang provides comprehensive chat support:

✅ **Message management** - Clean message handling  
✅ **Context control** - Manage conversation history  
✅ **Production-ready** - Rate limiting, monitoring  
✅ **Multi-user** - Support many users  
✅ **Advanced features** - Intent, sentiment, multilingual  

Build sophisticated conversational AI with KLang! 💬🚀
