// ===== Theme Toggle =====
const themeToggle = document.getElementById('themeToggle');
const html = document.documentElement;

// Load saved theme
const savedTheme = localStorage.getItem('theme') || 'dark';
html.setAttribute('data-theme', savedTheme);
updateThemeIcon(savedTheme);
updateHighlightTheme(savedTheme);

themeToggle.addEventListener('click', () => {
    const currentTheme = html.getAttribute('data-theme');
    const newTheme = currentTheme === 'dark' ? 'light' : 'dark';
    
    html.setAttribute('data-theme', newTheme);
    localStorage.setItem('theme', newTheme);
    updateThemeIcon(newTheme);
    updateHighlightTheme(newTheme);
});

function updateThemeIcon(theme) {
    const sunIcon = themeToggle.querySelector('.sun-icon');
    const moonIcon = themeToggle.querySelector('.moon-icon');
    
    if (theme === 'dark') {
        sunIcon.style.display = 'block';
        moonIcon.style.display = 'none';
    } else {
        sunIcon.style.display = 'none';
        moonIcon.style.display = 'block';
    }
}

function updateHighlightTheme(theme) {
    const darkTheme = document.getElementById('highlight-theme-dark');
    const lightTheme = document.getElementById('highlight-theme-light');
    
    if (darkTheme && lightTheme) {
        if (theme === 'dark') {
            darkTheme.disabled = false;
            lightTheme.disabled = true;
        } else {
            darkTheme.disabled = true;
            lightTheme.disabled = false;
        }
    }
}

// ===== Mobile Menu =====
const mobileMenuToggle = document.getElementById('mobileMenuToggle');
const navLinks = document.querySelector('.nav-links');

mobileMenuToggle.addEventListener('click', () => {
    const isActive = navLinks.classList.toggle('active');
    
    // Update aria-expanded for accessibility
    mobileMenuToggle.setAttribute('aria-expanded', isActive);
    
    // Animate hamburger icon
    const spans = mobileMenuToggle.querySelectorAll('span');
    spans[0].style.transform = isActive 
        ? 'rotate(45deg) translateY(8px)' 
        : '';
    spans[1].style.opacity = isActive ? '0' : '1';
    spans[2].style.transform = isActive 
        ? 'rotate(-45deg) translateY(-8px)' 
        : '';
});

// Close mobile menu when clicking outside
document.addEventListener('click', (e) => {
    if (!e.target.closest('.nav-links') && !e.target.closest('.mobile-menu-toggle')) {
        navLinks.classList.remove('active');
        mobileMenuToggle.setAttribute('aria-expanded', 'false');
        const spans = mobileMenuToggle.querySelectorAll('span');
        spans[0].style.transform = '';
        spans[1].style.opacity = '1';
        spans[2].style.transform = '';
    }
});

// ===== Syntax Highlighting =====
document.addEventListener('DOMContentLoaded', () => {
    // Highlight all code blocks
    document.querySelectorAll('pre code').forEach((block) => {
        hljs.highlightElement(block);
    });
});

// ===== Examples Tabs =====
const tabButtons = document.querySelectorAll('.tab-button');
const exampleTabs = document.querySelectorAll('.example-tab');

tabButtons.forEach(button => {
    button.addEventListener('click', () => {
        const tabName = button.getAttribute('data-tab');
        
        // Remove active class from all buttons and tabs
        tabButtons.forEach(btn => btn.classList.remove('active'));
        exampleTabs.forEach(tab => tab.classList.remove('active'));
        
        // Add active class to clicked button and corresponding tab
        button.classList.add('active');
        document.getElementById(`tab-${tabName}`).classList.add('active');
    });
});

// ===== AI Chat Widget =====
const aiChatWidget = document.getElementById('aiChatWidget');
const openAiChat = document.getElementById('openAiChat');
const closeChatWidget = document.getElementById('closeChatWidget');
const fabAiChat = document.getElementById('fabAiChat');
const chatInput = document.getElementById('chatInput');
const sendMessage = document.getElementById('sendMessage');
const chatMessages = document.getElementById('chatMessages');

// Open chat
openAiChat.addEventListener('click', () => {
    aiChatWidget.classList.add('active');
    fabAiChat.style.display = 'none';
    chatInput.focus();
});

// Close chat
closeChatWidget.addEventListener('click', () => {
    aiChatWidget.classList.remove('active');
    fabAiChat.style.display = 'flex';
});

// FAB click
fabAiChat.addEventListener('click', () => {
    aiChatWidget.classList.add('active');
    fabAiChat.style.display = 'none';
    chatInput.focus();
});

// Show FAB after delay
setTimeout(() => {
    if (!aiChatWidget.classList.contains('active')) {
        fabAiChat.style.display = 'flex';
    }
}, 3000);

// Send message
sendMessage.addEventListener('click', sendChatMessage);
chatInput.addEventListener('keypress', (e) => {
    if (e.key === 'Enter') {
        sendChatMessage();
    }
});

function sendChatMessage() {
    const message = chatInput.value.trim();
    if (!message) return;
    
    // Add user message
    addChatMessage(message, 'user');
    chatInput.value = '';
    
    // Simulate AI response
    setTimeout(() => {
        const response = getAIResponse(message);
        addChatMessage(response, 'assistant');
    }, 1000);
}

function addChatMessage(text, type) {
    const messageDiv = document.createElement('div');
    messageDiv.className = `chat-message ${type}`;
    
    const avatar = document.createElement('div');
    avatar.className = 'message-avatar';
    avatar.innerHTML = type === 'assistant' 
        ? '<svg width="20" height="20" fill="currentColor" viewBox="0 0 16 16"><path d="M8 16c3.314 0 6-2 6-5.5 0-1.5-.5-3-1.5-4.5"/></svg>'
        : '<svg width="20" height="20" fill="currentColor" viewBox="0 0 16 16"><path d="M11 6a3 3 0 1 1-6 0 3 3 0 0 1 6 0z"/><path d="M0 8a8 8 0 1 1 16 0A8 8 0 0 1 0 8zm8-7a7 7 0 0 0-5.468 11.37C3.242 11.226 4.805 10 8 10s4.757 1.225 5.468 2.37A7 7 0 0 0 8 1z"/></svg>';
    
    const content = document.createElement('div');
    content.className = 'message-content';
    content.innerHTML = `<p>${text}</p>`;
    
    messageDiv.appendChild(avatar);
    messageDiv.appendChild(content);
    chatMessages.appendChild(messageDiv);
    
    // Scroll to bottom
    chatMessages.scrollTop = chatMessages.scrollHeight;
}

function getAIResponse(message) {
    const lowerMessage = message.toLowerCase();
    
    // Simple keyword-based responses
    if (lowerMessage.includes('install') || lowerMessage.includes('setup')) {
        return `To install KLang, you can use our quick installation script:

<pre><code>git clone https://github.com/k-kaundal/KLang.git
cd KLang
make
sudo make install</code></pre>

After installation, verify with: <code>klang --version</code>`;
    }
    
    if (lowerMessage.includes('ai') || lowerMessage.includes('llm')) {
        return `KLang has native AI integration! You can use AI features like:

• <code>ai.llm(prompt)</code> - Call LLM models
• <code>ai.embed(text)</code> - Generate embeddings
• <code>ai.rag(options)</code> - Use RAG systems
• <code>ai.agent(config)</code> - Create AI agents

Check the <a href="docs.html#ai">AI documentation</a> for more details.`;
    }
    
    if (lowerMessage.includes('syntax') || lowerMessage.includes('how to')) {
        return `KLang uses a clean, Python-like syntax with JavaScript features:

<pre><code># Variables
let x = 10
const PI = 3.14159

# Functions
function greet(name) {
    return "Hello, " + name
}

# Arrow functions
let double = x => x * 2

# Classes
class Person {
    constructor(name) {
        this.name = name
    }
}</code></pre>

See the <a href="docs.html#guide">User Guide</a> for comprehensive examples.`;
    }
    
    if (lowerMessage.includes('example') || lowerMessage.includes('code')) {
        return `Here's a quick example combining traditional programming with AI:

<pre><code># Fetch data
let data = http.get("https://api.example.com/data")

# Process with AI
let summary = ai.llm("Summarize: " + data.text)

# Print result
println(summary)</code></pre>

Try the <a href="playground.html">online playground</a> to experiment!`;
    }
    
    if (lowerMessage.includes('performance') || lowerMessage.includes('fast')) {
        return `KLang is built for speed:

• **366M instructions/second** 
• **2.7ns per instruction**
• **10x faster than stack-based VMs**
• Register-based VM with 256 registers
• LLVM JIT compilation support

The VM is production-ready with 100% test pass rate!`;
    }
    
    if (lowerMessage.includes('feature')) {
        return `KLang's key features:

🤖 **AI-Native**: Built-in LLM, RAG, embeddings, agents
⚡ **Fast**: 366M inst/sec, register-based VM
🎯 **Modern**: Arrow functions, async/await, classes
🛡️ **Safe**: Automatic memory management
📦 **Complete**: HTTP, file I/O, JSON, regex
🔧 **DX**: VS Code extension, LSP, REPL

What would you like to know more about?`;
    }
    
    if (lowerMessage.includes('help') || lowerMessage.includes('documentation')) {
        return `I can help you with:

• Installation and setup
• Language syntax and features
• AI integration examples
• API reference
• Best practices
• Troubleshooting

Try asking specific questions like:
- "How do I use AI in KLang?"
- "Show me syntax examples"
- "How do I install KLang?"

Or visit the <a href="docs.html">full documentation</a>.`;
    }
    
    // Default response
    return `Thanks for your question! Here are some helpful resources:

• <a href="docs.html">Documentation</a> - Complete guide and API reference
• <a href="#examples">Examples</a> - See KLang in action
• <a href="playground.html">Playground</a> - Try code online
• <a href="https://github.com/k-kaundal/KLang">GitHub</a> - Source code and issues

You can also ask me specific questions about installation, syntax, AI features, or examples!`;
}

// ===== Code Copy Functionality =====
document.querySelectorAll('.code-action[title="Copy"]').forEach(button => {
    button.addEventListener('click', () => {
        const codeBlock = button.closest('.code-window').querySelector('code');
        const text = codeBlock.textContent;
        
        navigator.clipboard.writeText(text).then(() => {
            // Show feedback
            const originalHTML = button.innerHTML;
            button.innerHTML = '<svg width="14" height="14" fill="currentColor" viewBox="0 0 16 16"><path d="M10.97 4.97a.75.75 0 0 1 1.07 1.05l-3.99 4.99a.75.75 0 0 1-1.08.02L4.324 8.384a.75.75 0 1 1 1.06-1.06l2.094 2.093 3.473-4.425a.267.267 0 0 1 .02-.022z"/></svg>';
            setTimeout(() => {
                button.innerHTML = originalHTML;
            }, 2000);
        });
    });
});

// ===== Code Run Functionality (Demo) =====
document.querySelectorAll('.code-action[title="Run"]').forEach(button => {
    button.addEventListener('click', () => {
        // Show a message (in production, this would execute in playground)
        alert('Code execution coming soon! Try the playground for interactive coding.');
    });
});

// ===== Smooth Scroll =====
document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function (e) {
        const href = this.getAttribute('href');
        if (href === '#') return;
        
        e.preventDefault();
        const target = document.querySelector(href);
        if (target) {
            target.scrollIntoView({
                behavior: 'smooth',
                block: 'start'
            });
            
            // Close mobile menu if open
            navLinks.classList.remove('active');
        }
    });
});

// ===== Scroll Animations =====
const observerOptions = {
    threshold: 0.1,
    rootMargin: '0px 0px -100px 0px'
};

const observer = new IntersectionObserver((entries) => {
    entries.forEach(entry => {
        if (entry.isIntersecting) {
            entry.target.classList.add('fade-in');
        }
    });
}, observerOptions);

// Observe elements for animation
document.querySelectorAll('.feature-card, .quickstart-step, .performance-card, .community-card').forEach(el => {
    observer.observe(el);
});

// ===== Navbar Scroll Effect =====
let lastScroll = 0;
const navbar = document.querySelector('.navbar');

window.addEventListener('scroll', () => {
    const currentScroll = window.pageYOffset;
    
    if (currentScroll <= 0) {
        navbar.style.boxShadow = 'none';
    } else {
        navbar.style.boxShadow = '0 4px 6px -1px rgb(0 0 0 / 0.3)';
    }
    
    lastScroll = currentScroll;
});

// ===== Search Functionality (AI-Powered) =====
function searchDocumentation(query) {
    // This would connect to an AI-powered search in production
    console.log('Searching for:', query);
    
    // Simulate search results
    const results = [
        {
            title: 'Getting Started',
            excerpt: 'Learn how to install and use KLang...',
            url: 'docs.html#getting-started'
        },
        {
            title: 'AI Integration',
            excerpt: 'Use AI features in your KLang programs...',
            url: 'docs.html#ai'
        },
        {
            title: 'API Reference',
            excerpt: 'Complete API documentation...',
            url: 'docs.html#api'
        }
    ];
    
    return results;
}

// ===== Analytics (Optional) =====
function trackEvent(category, action, label) {
    // Track user interactions for analytics
    console.log('Event:', category, action, label);
    
    // In production, send to analytics service
    // gtag('event', action, { category, label });
}

// Track button clicks
document.querySelectorAll('.btn').forEach(btn => {
    btn.addEventListener('click', (e) => {
        const text = e.target.textContent.trim();
        trackEvent('Button', 'Click', text);
    });
});

// Track AI chat usage
sendMessage.addEventListener('click', () => {
    trackEvent('AI Chat', 'Send Message', 'Chat Interaction');
});

// ===== Performance Monitoring =====
window.addEventListener('load', () => {
    // Log page load performance
    const perfData = performance.timing;
    const pageLoadTime = perfData.loadEventEnd - perfData.navigationStart;
    console.log('Page load time:', pageLoadTime, 'ms');
    
    trackEvent('Performance', 'Page Load', `${pageLoadTime}ms`);
});

// ===== Service Worker (PWA) =====
if ('serviceWorker' in navigator) {
    window.addEventListener('load', () => {
        navigator.serviceWorker.register('/sw.js')
            .then(registration => {
                console.log('SW registered:', registration);
            })
            .catch(error => {
                console.log('SW registration failed:', error);
            });
    });
}

// ===== Dynamic Code Examples =====
function updateCodeExample(language, code) {
    // Update code examples dynamically
    const codeBlock = document.querySelector(`[data-language="${language}"] code`);
    if (codeBlock) {
        codeBlock.textContent = code;
        hljs.highlightElement(codeBlock);
    }
}

// ===== AI-Powered Code Suggestions =====
let suggestionTimeout;
chatInput.addEventListener('input', (e) => {
    clearTimeout(suggestionTimeout);
    
    suggestionTimeout = setTimeout(() => {
        const value = e.target.value.toLowerCase();
        
        // Show suggestions based on input
        if (value.length > 3) {
            // In production, this would call an AI model for suggestions
            console.log('Getting suggestions for:', value);
        }
    }, 500);
});

// ===== Keyboard Shortcuts =====
document.addEventListener('keydown', (e) => {
    // Ctrl/Cmd + K to open search
    if ((e.ctrlKey || e.metaKey) && e.key === 'k') {
        e.preventDefault();
        chatInput.focus();
        aiChatWidget.classList.add('active');
        fabAiChat.style.display = 'none';
    }
    
    // Escape to close chat
    if (e.key === 'Escape') {
        aiChatWidget.classList.remove('active');
        fabAiChat.style.display = 'flex';
    }
});

// ===== Copy Code Button for All Code Blocks =====
document.querySelectorAll('pre code').forEach((codeBlock) => {
    const pre = codeBlock.parentElement;
    if (!pre.querySelector('.copy-btn')) {
        const copyBtn = document.createElement('button');
        copyBtn.className = 'copy-btn';
        copyBtn.innerHTML = '📋 Copy';
        copyBtn.style.cssText = `
            position: absolute;
            top: 0.5rem;
            right: 0.5rem;
            padding: 0.25rem 0.75rem;
            background: rgba(99, 102, 241, 0.1);
            border: 1px solid rgba(99, 102, 241, 0.3);
            border-radius: 0.375rem;
            color: #818cf8;
            font-size: 0.875rem;
            cursor: pointer;
            opacity: 0;
            transition: opacity 0.3s;
        `;
        
        pre.style.position = 'relative';
        pre.addEventListener('mouseenter', () => {
            copyBtn.style.opacity = '1';
        });
        pre.addEventListener('mouseleave', () => {
            copyBtn.style.opacity = '0';
        });
        
        copyBtn.addEventListener('click', () => {
            navigator.clipboard.writeText(codeBlock.textContent).then(() => {
                copyBtn.innerHTML = '✓ Copied!';
                setTimeout(() => {
                    copyBtn.innerHTML = '📋 Copy';
                }, 2000);
            });
        });
        
        pre.appendChild(copyBtn);
    }
});

// ===== Initialize =====
console.log('KLang Documentation Website Loaded');
console.log('Version: 1.0.0');
console.log('AI Assistant: Ready');
console.log('Theme:', html.getAttribute('data-theme'));
