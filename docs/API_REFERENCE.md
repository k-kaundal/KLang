# KLang API Reference

## Table of Contents

- [Core Language](#core-language)
- [Standard Library](#standard-library)
- [AI Module](#ai-module)
- [CLI Commands](#cli-commands)

## Core Language

### Built-in Functions

#### I/O Functions

- **`println(value)`** - Print value with newline
- **`print(value)`** - Print value without newline
- **`input(prompt)`** - Read user input with optional prompt

#### Type Conversion

- **`str(value)`** - Convert to string
- **`int(value)`** - Convert to integer
- **`float(value)`** - Convert to floating-point number
- **`bool(value)`** - Convert to boolean

#### Collection Functions

- **`len(collection)`** - Get length of array, string, dict, or set
- **`range(start, end, step?)`** - Create a range iterator
- **`keys(dict)`** - Get keys from dictionary
- **`values(dict)`** - Get values from dictionary

#### Type Checking

- **`typeof(value)`** - Get type name as string
- **`isArray(value)`** - Check if value is an array
- **`isString(value)`** - Check if value is a string
- **`isNumber(value)`** - Check if value is a number
- **`isBoolean(value)`** - Check if value is a boolean
- **`isNull(value)`** - Check if value is null

### Array Methods

All arrays have these methods:

#### Mutating Methods

- **`push(item)`** - Add item to end
- **`pop()`** - Remove and return last item
- **`shift()`** - Remove and return first item
- **`unshift(item)`** - Add item to beginning
- **`splice(start, deleteCount, ...items)`** - Remove/add items
- **`reverse()`** - Reverse array in place
- **`sort(compareFn?)`** - Sort array in place

#### Non-mutating Methods

- **`slice(start?, end?)`** - Get subarray
- **`concat(...arrays)`** - Concatenate arrays
- **`join(separator?)`** - Join elements into string
- **`indexOf(item)`** - Find index of item (-1 if not found)
- **`includes(item)`** - Check if item exists
- **`toString()`** - Convert to string representation

#### Functional Methods

- **`map(fn)`** - Transform each element
- **`filter(fn)`** - Keep elements that pass test
- **`reduce(fn, initial?)`** - Reduce to single value
- **`forEach(fn)`** - Execute function for each element
- **`find(fn)`** - Find first element that passes test
- **`findIndex(fn)`** - Find index of first element that passes test
- **`some(fn)`** - Check if any element passes test
- **`every(fn)`** - Check if all elements pass test

### String Methods

- **`length`** - String length property
- **`charAt(index)`** - Get character at index
- **`substring(start, end?)`** - Get substring
- **`indexOf(substring)`** - Find index of substring
- **`split(separator)`** - Split into array
- **`toUpperCase()`** - Convert to uppercase
- **`toLowerCase()`** - Convert to lowercase
- **`trim()`** - Remove whitespace from both ends

### Dictionary Type

Create with `dict()`:

```klang
let d = dict()
```

Methods:
- **`set(key, value)`** - Set key-value pair
- **`get(key)`** - Get value by key
- **`has(key)`** - Check if key exists
- **`delete(key)`** - Remove key-value pair
- **`keys()`** - Get array of keys
- **`values()`** - Get array of values
- **`clear()`** - Remove all entries
- **`size()`** - Get number of entries

### Set Type

Create with `set()`:

```klang
let s = set()
```

Methods:
- **`add(value)`** - Add value
- **`remove(value)`** - Remove value
- **`has(value)`** - Check if value exists
- **`clear()`** - Remove all values
- **`size()`** - Get number of values
- **`toArray()`** - Convert to array

### Tuple Type

Create with parentheses:

```klang
let t = (1, 2, 3)
```

Features:
- Immutable ordered collection
- Index access: `t[0]`
- Unpacking: `let (x, y) = point`
- Iteration support

## Standard Library

### Environment Module

```klang
import "stdlib/env.kl"
```

- **`env.get(name)`** - Get environment variable
- **`env.set(name, value)`** - Set environment variable
- **`env.has(name)`** - Check if variable exists

### HTTP Module

```klang
import "stdlib/http.kl"
```

- **`http.get(url, headers?)`** - HTTP GET request
- **`http.post(url, body, headers?)`** - HTTP POST request
- **`http.request(config)`** - Generic HTTP request

### Math Module

Built-in math functions:
- `abs(x)` - Absolute value
- `sqrt(x)` - Square root  
- `pow(x, y)` - Power
- `floor(x)` - Floor
- `ceil(x)` - Ceiling
- `round(x)` - Round
- `min(a, b)` - Minimum
- `max(a, b)` - Maximum

## AI Module

### Core AI Types

#### LLMConfig

```klang
let config = new LLMConfig(provider, apiKey, model?, options?)
```

Properties:
- `provider` - "openai", "anthropic", or "ollama"
- `apiKey` - API key (not needed for Ollama)
- `model` - Model name (optional, uses defaults)
- `options` - Additional options (temperature, maxTokens, etc.)

#### LLMClient

```klang
import "stdlib/llm.kl"
let client = new LLMClient(config)
```

Methods:
- **`complete(prompt, options?)`** - Single completion
- **`chat(messages, options?)`** - Chat completion
- **`streamComplete(prompt, onChunk, options?)`** - Streaming completion

#### Message

```klang
let msg = new Message(role, content, metadata?)
```

Properties:
- `role` - "system", "user", or "assistant"
- `content` - Message text
- `metadata` - Optional metadata object

#### ChatSession

```klang
let session = new ChatSession(client, systemPrompt?)
```

Methods:
- **`addMessage(role, content)`** - Add message and get response
- **`getHistory()`** - Get full conversation history
- **`clear()`** - Clear history

### RAG System

```klang
import "stdlib/rag.kl"
let rag = new RAGSystem(llmClient, chunkSize?, topK?)
```

Methods:
- **`ingestDocument(id, text, metadata?)`** - Add document
- **`query(question, options?)`** - Query with retrieval
- **`search(query, k?)`** - Search documents
- **`clear()`** - Clear all documents

### Embedding Operations

```klang
import "stdlib/embedding.kl"
```

Functions:
- **`cosineSimilarity(vec1, vec2)`** - Cosine similarity
- **`dotProduct(vec1, vec2)`** - Dot product
- **`magnitude(vec)`** - Vector magnitude
- **`normalize(vec)`** - Normalize vector
- **`vectorAdd(vec1, vec2)`** - Add vectors
- **`vectorSubtract(vec1, vec2)`** - Subtract vectors

#### EmbeddingStore

```klang
let store = new EmbeddingStore()
```

Methods:
- **`add(id, embedding, metadata?)`** - Add embedding
- **`search(queryEmbedding, k?)`** - Search similar embeddings
- **`get(id)`** - Get embedding by ID
- **`remove(id)`** - Remove embedding
- **`clear()`** - Clear all embeddings

### Agents

```klang
import "stdlib/agents.kl"
```

#### Agent

```klang
let agent = new Agent(name, role, llmClient, tools?)
```

Methods:
- **`think(context)`** - Generate reasoning
- **`act(action, args)`** - Execute action
- **`remember(key, value, term?)`** - Store memory (short/long)
- **`recall(key, term?)`** - Retrieve memory
- **`getMemory()`** - Get all memories

#### ReActAgent

```klang
let agent = new ReActAgent(name, llmClient, tools)
```

Methods:
- **`run(task, maxSteps?)`** - Execute task with reasoning
- **`addTool(name, description, fn)`** - Register tool

## CLI Commands

### Basic Commands

```bash
klang run <file>           # Run a KLang program
klang repl                 # Start interactive REPL
klang check <file>         # Check syntax
klang fmt <file>           # Format code
klang test                 # Run tests
```

### Advanced Commands

```bash
klang init <name>          # Initialize new project
klang build <file>         # Compile (future)
klang doc <file>           # Generate documentation (future)
```

### Options

```bash
--version, -v              # Show version
--help, -h                 # Show help
--verbose                  # Verbose output
--quiet, -q                # Quiet mode
```

## Examples

### Basic Program

```klang
# hello.kl
fn greet(name) {
    return "Hello, " + name + "!"
}

let message = greet("World")
println(message)
```

### Array Processing

```klang
# process.kl
let numbers = [1, 2, 3, 4, 5]

let doubled = numbers.map(x => x * 2)
let evens = numbers.filter(x => x % 2 == 0)
let sum = numbers.reduce((acc, x) => acc + x, 0)

println("Doubled: " + str(doubled))
println("Evens: " + str(evens))
println("Sum: " + str(sum))
```

### AI Chat

```klang
# chat.kl
import "stdlib/ai.kl"
import "stdlib/llm.kl"

let apiKey = env.get("OPENAI_API_KEY")
let config = new LLMConfig("openai", apiKey)
let client = new LLMClient(config)
let session = new ChatSession(client, "You are helpful")

let response = session.addMessage("user", "Tell me about KLang")
println(response.content)
```

---

For more examples, see the [examples/](../examples/) directory.

For detailed guides, see the [User Guide](USER_GUIDE.md).
