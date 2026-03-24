# KLang Standard Library

The KLang standard library provides a collection of useful functions for common programming tasks.

## Available Modules

### 🤖 AI-Native Modules (NEW!)

#### ai.kl - Core AI Utilities
Core AI functionality including configuration, messages, chat sessions, and embeddings.

**Classes:**
- `AIConfig` - Configuration for AI services
- `Message` - Chat message representation
- `ChatSession` - Conversation history manager
- `Embedding` - Vector embedding with similarity operations
- `BatchProcessor` - Batch processing for multiple prompts

**Functions:**
- `countTokens(text: string) -> int` - Estimate token count
- `estimateCost(tokens: int, model: string) -> float` - Estimate API cost

#### llm.kl - LLM Integration
Complete LLM provider support with OpenAI, Anthropic, and Ollama.

**Classes:**
- `LLMConfig` - LLM provider configuration
- `LLMClient` - Client for making LLM API calls

**Functions:**
- `llm(prompt: string, apiKey: string) -> string` - Quick LLM query
- `llmStream(prompt: string, callback, apiKey: string)` - Streaming completions

**Supported Providers:**
- OpenAI (GPT-4, GPT-3.5-turbo)
- Anthropic (Claude 3)
- Ollama (local models)

#### embedding.kl - Vector Embeddings
Vector operations and semantic search utilities.

**Classes:**
- `EmbeddingStore` - Store and search embeddings
- `TextChunker` - Document chunking utilities

**Functions:**
- `vectorAdd(vec1, vec2)` - Add two vectors
- `vectorSubtract(vec1, vec2)` - Subtract vectors
- `vectorScale(vec, scalar)` - Scale vector
- `vectorAverage(vectors)` - Average multiple vectors
- `semanticSearch(query, documents, llmClient, topK)` - Semantic document search

#### prompt.kl - Prompt Engineering
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
- `estimateTokens(prompt: string) -> int` - Estimate token count
- `truncateToTokenLimit(prompt: string, maxTokens: int) -> string` - Truncate to fit
- `combinePrompts(prompts, separator: string)` - Combine multiple prompts
- `createRolePrompt(role: string, task: string) -> string` - Create role-based prompt
- `createConstrainedPrompt(task: string, constraints) -> string` - Add constraints
- `createFormattedPrompt(task: string, format: string) -> string` - Specify output format

#### rag.kl - RAG Systems
Complete Retrieval-Augmented Generation implementation.

**Classes:**
- `Document` - Document with embeddings
- `VectorDB` - In-memory vector database
- `RAGSystem` - Complete RAG pipeline
- `SemanticCache` - Cache for similar queries

**Functions:**
- `buildRAGFromFiles(llmClient, filePaths)` - Build RAG from files

#### agents.kl - AI Agents
Autonomous agents with memory and actions.

**Classes:**
- `AgentMemory` - Short and long-term memory
- `AgentAction` - Agent action definition
- `Agent` - Base autonomous agent
- `ReActAgent` - Reasoning + Acting agent
- `TaskAgent` - Task-oriented agent

---

### math.kl - Mathematical Functions

Integer mathematics functions:

- `abs(n: int) -> int` - Absolute value
- `max(a: int, b: int) -> int` - Maximum of two numbers
- `min(a: int, b: int) -> int` - Minimum of two numbers  
- `pow(base: int, exp: int) -> int` - Power function (integer exponent)
- `sqrt(n: int) -> int` - Square root (integer approximation)
- `factorial(n: int) -> int` - Factorial of n
- `gcd(a: int, b: int) -> int` - Greatest common divisor
- `lcm(a: int, b: int) -> int` - Least common multiple
- `isEven(n: int) -> int` - Check if number is even (returns 1 or 0)
- `isOdd(n: int) -> int` - Check if number is odd (returns 1 or 0)
- `clamp(value: int, minVal: int, maxVal: int) -> int` - Clamp value between min and max

### string.kl - String Utilities

String manipulation functions:

- `repeat(s: string, n: int) -> string` - Repeat string n times
- `reverse(s: string) -> string` - Reverse a string
- `isEmpty(s: string) -> int` - Check if string is empty (returns 1 or 0)
- `padLeft(s: string, totalLength: int, padChar: string) -> string` - Pad string on left
- `padRight(s: string, totalLength: int, padChar: string) -> string` - Pad string on right

### io.kl - Input/Output Functions

I/O utility functions:

- `write(value: string)` - Print without newline (alias for print)
- `writeln(value: string)` - Print with newline (alias for println)
- `prompt(message: string) -> string` - Read input with a prompt message
- `printSeparator(length: int, char: string)` - Print a separator line
- `printHeader(title: string)` - Print a formatted header
- `printBlankLines(count: int)` - Print multiple blank lines

### array.kl - Array Utilities

Array manipulation functions:

- `sum(arr: array) -> int` - Sum all elements
- `average(arr: array) -> int` - Average of elements
- `arrayMax(arr: array) -> int` - Find maximum value
- `arrayMin(arr: array) -> int` - Find minimum value
- `count(arr: array, value: int) -> int` - Count occurrences of value
- `contains(arr: array, value: int) -> int` - Check if array contains value (returns 1 or 0)
- `first(arr: array, defaultVal: int) -> int` - Get first element or default
- `last(arr: array, defaultVal: int) -> int` - Get last element or default
- `rangeArray(start: int, end: int) -> array` - Create range array
- `fill(size: int, value: int) -> array` - Create array filled with value

### core.kl - Core Utilities

Core utility functions:

- `identity(value: int) -> int` - Returns the value passed to it
- `constant(value: int, arg: int) -> int` - Always returns the same value
- `inRange(value: int, min: int, max: int) -> int` - Check if value is within range (returns 1 or 0)
- `defaultTo(value: int, defaultValue: int, condition: int) -> int` - Return default if condition is false
- `swap(a: int, b: int) -> array` - Swap two values (returns array [b, a])
- `sign(n: int) -> int` - Sign function (-1, 0, or 1)
- `compare(a: int, b: int) -> int` - Compare two values (-1 if a < b, 0 if equal, 1 if a > b)
- `not(value: int) -> int` - Logical NOT (0 becomes 1, non-zero becomes 0)
- `and(a: int, b: int) -> int` - Logical AND (returns 1 or 0)
- `or(a: int, b: int) -> int` - Logical OR (returns 1 or 0)
- `xor(a: int, b: int) -> int` - Logical XOR (returns 1 or 0)

## Usage

Currently, the standard library files are reference implementations. You can copy functions from these files into your KLang programs.

### Example

```klang
# Copy functions from stdlib/math.kl
fn factorial(n: int) -> int {
    if n <= 1 {
        return 1
    }
    let result = 1
    let i = 2
    while i <= n {
        result = result * i
        i = i + 1
    }
    return result
}

# Use the function
println("Factorial of 5:")
print(factorial(5))  # Output: 120
```

See `examples/stdlib_simple.kl` for a complete example demonstrating various stdlib functions.

## Built-in Functions

The KLang runtime provides several built-in functions that are always available:

### I/O Functions
- `print(value)` - Print value without newline
- `println(value)` - Print value with newline
- `input() -> string` - Read line from stdin

### Type Functions
- `len(collection)` - Get length of string, array, etc.
- `type(value) -> string` - Get type name of value

### Math Functions (Built-in)
- `abs(n)` - Absolute value
- `min(a, b)` - Minimum of two values
- `max(a, b)` - Maximum of two values

### Conversion Functions
- `int(value)` - Convert to integer
- `float(value)` - Convert to float
- `str(value)` - Convert to string

### Array Functions
- `range(n)` - Create range [0, n)
- `range(start, end)` - Create range [start, end)
- `range(start, end, step)` - Create range with step

## Future Enhancements

Planned improvements for the standard library:

- Module import system for easy inclusion of stdlib files
- File I/O operations (read, write, append files)
- JSON parsing and serialization
- Regular expressions
- Date and time functions
- Network/HTTP utilities
- More data structure implementations

## Contributing

When adding new stdlib functions:

1. Keep functions pure and side-effect free when possible
2. Use clear, descriptive names
3. Include parameter and return types
4. Add comprehensive comments
5. Test thoroughly
6. Update this README with new functions

## License

MIT License - Same as KLang
