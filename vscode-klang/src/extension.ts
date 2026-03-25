import * as vscode from 'vscode';
import * as path from 'path';
import * as fs from 'fs';
import { LanguageClient, LanguageClientOptions, ServerOptions, TransportKind } from 'vscode-languageclient/node';

let client: LanguageClient | undefined;
let diagnosticCollection: vscode.DiagnosticCollection;

export function activate(context: vscode.ExtensionContext) {
    console.log('KLang extension is now active!');

    // Create diagnostic collection for linting
    diagnosticCollection = vscode.languages.createDiagnosticCollection('klang');
    context.subscriptions.push(diagnosticCollection);

    // Register commands
    registerCommands(context);

    // Start Language Server if enabled
    const config = vscode.workspace.getConfiguration('klang');
    if (config.get<boolean>('languageServer.enabled')) {
        activateLanguageServer(context);
    }

    // Set up file watchers and providers
    setupProviders(context);

    // Set up document watchers for linting
    setupLinting(context);
}

export function deactivate(): Thenable<void> | undefined {
    if (diagnosticCollection) {
        diagnosticCollection.dispose();
    }
    if (!client) {
        return undefined;
    }
    return client.stop();
}

function registerCommands(context: vscode.ExtensionContext) {
    // Run current file
    context.subscriptions.push(
        vscode.commands.registerCommand('klang.run', () => {
            const editor = vscode.window.activeTextEditor;
            if (!editor) {
                vscode.window.showErrorMessage('No active editor');
                return;
            }

            const document = editor.document;
            if (!isKLangFile(document)) {
                vscode.window.showErrorMessage('Current file is not a KLang file');
                return;
            }

            runKLangFile(document.uri.fsPath);
        })
    );

    // Build current file
    context.subscriptions.push(
        vscode.commands.registerCommand('klang.build', () => {
            const editor = vscode.window.activeTextEditor;
            if (!editor) {
                vscode.window.showErrorMessage('No active editor');
                return;
            }

            const document = editor.document;
            if (!isKLangFile(document)) {
                vscode.window.showErrorMessage('Current file is not a KLang file');
                return;
            }

            buildKLangFile(document.uri.fsPath);
        })
    );

    // Check syntax
    context.subscriptions.push(
        vscode.commands.registerCommand('klang.check', () => {
            const editor = vscode.window.activeTextEditor;
            if (!editor) {
                vscode.window.showErrorMessage('No active editor');
                return;
            }

            const document = editor.document;
            if (!isKLangFile(document)) {
                vscode.window.showErrorMessage('Current file is not a KLang file');
                return;
            }

            checkKLangFile(document.uri.fsPath);
        })
    );

    // Format document
    context.subscriptions.push(
        vscode.commands.registerCommand('klang.format', () => {
            const editor = vscode.window.activeTextEditor;
            if (!editor) {
                vscode.window.showErrorMessage('No active editor');
                return;
            }

            const document = editor.document;
            if (!isKLangFile(document)) {
                vscode.window.showErrorMessage('Current file is not a KLang file');
                return;
            }

            formatKLangFile(editor);
        })
    );

    // Open REPL
    context.subscriptions.push(
        vscode.commands.registerCommand('klang.repl', () => {
            openKLangREPL();
        })
    );
}

function activateLanguageServer(context: vscode.ExtensionContext) {
    const config = vscode.workspace.getConfiguration('klang');
    const klangPath = config.get<string>('languageServer.path') || 'klang';

    // Check if KLang is available
    if (!isKLangAvailable(klangPath)) {
        vscode.window.showWarningMessage(
            'KLang executable not found. Language Server features will be disabled. ' +
            'Please install KLang or configure the path in settings.'
        );
        return;
    }

    // Start the Language Server
    const serverOptions: ServerOptions = {
        command: klangPath,
        args: ['lsp'],
        options: {
            env: process.env
        }
    };

    const clientOptions: LanguageClientOptions = {
        documentSelector: [{ scheme: 'file', language: 'klang' }],
        synchronize: {
            fileEvents: vscode.workspace.createFileSystemWatcher('**/*.{kl,k,klang}')
        }
    };

    try {
        client = new LanguageClient('klang', 'KLang Language Server', serverOptions, clientOptions);
        client.start();
        context.subscriptions.push({
            dispose: () => {
                if (client) {
                    client.stop();
                }
            }
        });
        console.log('KLang Language Server started successfully');
    } catch (error) {
        console.error('Failed to start KLang Language Server:', error);
        vscode.window.showErrorMessage('Failed to start KLang Language Server. Some features may not work.');
    }
}

function setupProviders(context: vscode.ExtensionContext) {
    // Register formatting provider
    context.subscriptions.push(
        vscode.languages.registerDocumentFormattingEditProvider('klang', {
            provideDocumentFormattingEdits(document) {
                return formatDocument(document);
            }
        })
    );

    // Register range formatting provider
    context.subscriptions.push(
        vscode.languages.registerDocumentRangeFormattingEditProvider('klang', {
            provideDocumentRangeFormattingEdits(document, range) {
                return formatDocumentRange(document, range);
            }
        })
    );

    // Register completion provider
    context.subscriptions.push(
        vscode.languages.registerCompletionItemProvider('klang', {
            provideCompletionItems(document, position) {
                return provideCompletions(document, position);
            }
        }, '.', ':')
    );

    // Hover provider for detailed documentation
    context.subscriptions.push(
        vscode.languages.registerHoverProvider('klang', {
            provideHover(document, position, token) {
                const range = document.getWordRangeAtPosition(position);
                const word = document.getText(range);

                // Enhanced keyword documentation
                const keywords: { [key: string]: string } = {
                    'fn': '**Function Declaration**\n\nDeclares a function in KLang.\n\n**Syntax:**\n```klang\nfn name(param1: Type, param2: Type) -> ReturnType {\n    // body\n}\n```\n\n**Example:**\n```klang\nfn add(a: number, b: number) -> number {\n    return a + b\n}\n```',
                    'async': '**Async Function**\n\nDeclares an asynchronous function that returns a Promise.\n\n**Syntax:**\n```klang\nasync fn name() {\n    await somePromise()\n}\n```',
                    'await': '**Await Expression**\n\nWaits for a Promise to resolve.\n\n**Syntax:**\n```klang\nlet result = await promise\n```',
                    'class': '**Class Declaration**\n\nDeclares a class with properties and methods.\n\n**Syntax:**\n```klang\nclass Name {\n    property: Type\n    \n    fn method() {\n        // ...\n    }\n}\n```',
                    'extends': '**Class Inheritance**\n\nInherits from a parent class.\n\n**Syntax:**\n```klang\nclass Child extends Parent {\n    // ...\n}\n```',
                    'let': '**Variable Declaration**\n\nDeclares a block-scoped mutable variable.\n\n**Syntax:**\n```klang\nlet name: Type = value\n```',
                    'const': '**Constant Declaration**\n\nDeclares a block-scoped constant (immutable).\n\n**Syntax:**\n```klang\nconst name: Type = value\n```',
                    'var': '**Variable Declaration (Function-scoped)**\n\nDeclares a function-scoped variable (legacy).\n\n**Syntax:**\n```klang\nvar name = value\n```',
                    'if': '**Conditional Statement**\n\nExecutes code based on a condition.\n\n**Syntax:**\n```klang\nif condition {\n    // then block\n} else {\n    // else block\n}\n```',
                    'for': '**For Loop**\n\nIterates over a range or collection.\n\n**Syntax:**\n```klang\nfor item in collection {\n    // body\n}\n\nfor i in 0..10 {\n    // body\n}\n```',
                    'while': '**While Loop**\n\nRepeats while a condition is true.\n\n**Syntax:**\n```klang\nwhile condition {\n    // body\n}\n```',
                    'return': '**Return Statement**\n\nReturns a value from a function.\n\n**Syntax:**\n```klang\nreturn value\n```',
                    'new': '**Object Instantiation**\n\nCreates a new instance of a class.\n\n**Syntax:**\n```klang\nlet obj = new ClassName(args)\n```',
                    'this': '**This Reference**\n\nReferences the current object instance within a class.',
                    'import': '**Import Statement**\n\nImports modules, functions, or classes.\n\n**Syntax:**\n```klang\nimport { name } from "module"\nimport * as name from "module"\n```',
                    'export': '**Export Statement**\n\nExports values from a module.\n\n**Syntax:**\n```klang\nexport const name = value\nexport fn functionName() { }\n```',
                    'yield': '**Yield Expression**\n\nYields a value in a generator function.\n\n**Syntax:**\n```klang\nyield value\n```',
                    'static': '**Static Member**\n\nDeclares a static class member (belongs to class, not instance).\n\n**Syntax:**\n```klang\nclass Example {\n    static member = value\n}\n```',
                    'public': '**Public Access Modifier**\n\nMakes a member accessible from anywhere (default).',
                    'private': '**Private Access Modifier**\n\nMakes a member accessible only within the class.',
                    'protected': '**Protected Access Modifier**\n\nMakes a member accessible within the class and subclasses.',
                    'abstract': '**Abstract Declaration**\n\nDeclares an abstract class or method that must be implemented by subclasses.',
                    'interface': '**Interface Declaration**\n\nDeclares an interface for type checking.\n\n**Syntax:**\n```klang\ninterface Name {\n    property: Type\n    method(): ReturnType\n}\n```',
                    'type': '**Type Alias**\n\nDefines a type alias.\n\n**Syntax:**\n```klang\ntype Name = Type\n```',
                    'enum': '**Enum Declaration**\n\nDefines an enumeration.\n\n**Syntax:**\n```klang\nenum Name {\n    Value1,\n    Value2\n}\n```',
                    'break': '**Break Statement**\n\nExits the current loop.',
                    'continue': '**Continue Statement**\n\nSkips to the next iteration of a loop.',
                    'throw': '**Throw Statement**\n\nThrows an exception.\n\n**Syntax:**\n```klang\nthrow new Error("message")\n```',
                    'try': '**Try-Catch Block**\n\nHandles exceptions.\n\n**Syntax:**\n```klang\ntry {\n    // code\n} catch (e) {\n    // handle error\n}\n```'
                };

                if (keywords[word]) {
                    return new vscode.Hover(new vscode.MarkdownString(keywords[word]));
                }

                return null;
            }
        })
    );

    // Document symbol provider for outline
    context.subscriptions.push(
        vscode.languages.registerDocumentSymbolProvider('klang', {
            provideDocumentSymbols(document) {
                const symbols: vscode.DocumentSymbol[] = [];
                const text = document.getText();
                const lines = text.split('\n');

                // Enhanced regex-based symbol detection
                for (let i = 0; i < lines.length; i++) {
                    const line = lines[i];

                    // Function declarations
                    const fnMatch = line.match(/^\s*(async\s+)?fn\s+(\w+)\s*\(/);
                    if (fnMatch) {
                        const name = fnMatch[2];
                        const range = new vscode.Range(i, 0, i, line.length);
                        const symbol = new vscode.DocumentSymbol(
                            name,
                            fnMatch[1] ? 'async function' : '',
                            vscode.SymbolKind.Function,
                            range,
                            range
                        );
                        symbols.push(symbol);
                    }

                    // Class declarations
                    const classMatch = line.match(/^\s*class\s+(\w+)/);
                    if (classMatch) {
                        const name = classMatch[1];
                        const range = new vscode.Range(i, 0, i, line.length);
                        const symbol = new vscode.DocumentSymbol(
                            name,
                            '',
                            vscode.SymbolKind.Class,
                            range,
                            range
                        );
                        symbols.push(symbol);
                    }

                    // Interface declarations
                    const interfaceMatch = line.match(/^\s*interface\s+(\w+)/);
                    if (interfaceMatch) {
                        const name = interfaceMatch[1];
                        const range = new vscode.Range(i, 0, i, line.length);
                        const symbol = new vscode.DocumentSymbol(
                            name,
                            '',
                            vscode.SymbolKind.Interface,
                            range,
                            range
                        );
                        symbols.push(symbol);
                    }

                    // Enum declarations
                    const enumMatch = line.match(/^\s*enum\s+(\w+)/);
                    if (enumMatch) {
                        const name = enumMatch[1];
                        const range = new vscode.Range(i, 0, i, line.length);
                        const symbol = new vscode.DocumentSymbol(
                            name,
                            '',
                            vscode.SymbolKind.Enum,
                            range,
                            range
                        );
                        symbols.push(symbol);
                    }

                    // Variable declarations (let, const, var)
                    const varMatch = line.match(/^\s*(let|const|var)\s+(\w+)/);
                    if (varMatch) {
                        const name = varMatch[2];
                        const kind = varMatch[1] === 'const' ? 
                            vscode.SymbolKind.Constant : 
                            vscode.SymbolKind.Variable;
                        const range = new vscode.Range(i, 0, i, line.length);
                        const symbol = new vscode.DocumentSymbol(
                            name,
                            varMatch[1],
                            kind,
                            range,
                            range
                        );
                        symbols.push(symbol);
                    }
                }

                return symbols;
            }
        })
    );

    // Definition provider
    context.subscriptions.push(
        vscode.languages.registerDefinitionProvider('klang', {
            provideDefinition(document, position, token) {
                const range = document.getWordRangeAtPosition(position);
                const word = document.getText(range);
                
                // Simple definition search in the same file
                const text = document.getText();
                const lines = text.split('\n');
                
                for (let i = 0; i < lines.length; i++) {
                    const line = lines[i];
                    // Check for function, class, or variable definitions
                    const defMatch = line.match(new RegExp(`\\b(fn|class|interface|enum|let|const|var)\\s+${word}\\b`));
                    if (defMatch) {
                        const startPos = new vscode.Position(i, line.indexOf(word));
                        const endPos = new vscode.Position(i, line.indexOf(word) + word.length);
                        return new vscode.Location(document.uri, new vscode.Range(startPos, endPos));
                    }
                }
                
                return null;
            }
        })
    );
}

function isKLangFile(document: vscode.TextDocument): boolean {
    return document.languageId === 'klang' || 
           document.fileName.endsWith('.kl') ||
           document.fileName.endsWith('.k') ||
           document.fileName.endsWith('.klang');
}

function isKLangAvailable(klangPath: string): boolean {
    try {
        const { execSync } = require('child_process');
        execSync(`${klangPath} --version`, { stdio: 'ignore' });
        return true;
    } catch {
        // Check in common locations
        const commonPaths = [
            '/usr/local/bin/klang',
            '/usr/bin/klang',
            path.join(process.env.HOME || '', 'klang', 'klang')
        ];

        for (const p of commonPaths) {
            if (fs.existsSync(p)) {
                return true;
            }
        }

        return false;
    }
}

function runKLangFile(filePath: string) {
    const config = vscode.workspace.getConfiguration('klang');
    const klangPath = config.get<string>('languageServer.path') || 'klang';

    const terminal = vscode.window.createTerminal('KLang Run');
    terminal.show();
    terminal.sendText(`${klangPath} run "${filePath}"`);
}

function buildKLangFile(filePath: string) {
    const config = vscode.workspace.getConfiguration('klang');
    const klangPath = config.get<string>('languageServer.path') || 'klang';

    const terminal = vscode.window.createTerminal('KLang Build');
    terminal.show();
    terminal.sendText(`${klangPath} build "${filePath}"`);
}

function checkKLangFile(filePath: string) {
    const config = vscode.workspace.getConfiguration('klang');
    const klangPath = config.get<string>('languageServer.path') || 'klang';

    const terminal = vscode.window.createTerminal('KLang Check');
    terminal.show();
    terminal.sendText(`${klangPath} check "${filePath}"`);
}

function formatKLangFile(editor: vscode.TextEditor) {
    const document = editor.document;
    const config = vscode.workspace.getConfiguration('klang');
    
    if (!config.get<boolean>('format.enabled')) {
        vscode.window.showInformationMessage('KLang formatting is disabled in settings');
        return;
    }

    const edits = formatDocument(document);
    if (edits && edits.length > 0) {
        const workspaceEdit = new vscode.WorkspaceEdit();
        workspaceEdit.set(document.uri, edits);
        vscode.workspace.applyEdit(workspaceEdit).then(success => {
            if (success) {
                vscode.window.showInformationMessage('Document formatted successfully');
            } else {
                vscode.window.showErrorMessage('Failed to apply formatting');
            }
        });
    }
}

function openKLangREPL() {
    const config = vscode.workspace.getConfiguration('klang');
    const klangPath = config.get<string>('languageServer.path') || 'klang';

    const terminal = vscode.window.createTerminal('KLang REPL');
    terminal.show();
    terminal.sendText(`${klangPath} repl`);
}

// Formatting support
function formatDocument(document: vscode.TextDocument): vscode.TextEdit[] {
    const config = vscode.workspace.getConfiguration('klang');
    const klangPath = config.get<string>('languageServer.path') || 'klang';
    
    if (!isKLangAvailable(klangPath)) {
        return [];
    }

    try {
        const { execSync } = require('child_process');
        const fs = require('fs');
        const os = require('os');
        const tmpDir = os.tmpdir();
        const tmpFile = path.join(tmpDir, `klang-format-${Date.now()}.kl`);

        // Write current document to temp file
        fs.writeFileSync(tmpFile, document.getText());

        // Run formatter (formats in-place)
        execSync(`${klangPath} fmt "${tmpFile}"`, {
            timeout: 5000
        });

        // Read formatted content
        const formatted = fs.readFileSync(tmpFile, 'utf8');

        // Clean up
        fs.unlinkSync(tmpFile);

        // Create edit for the entire document
        const firstLine = document.lineAt(0);
        const lastLine = document.lineAt(document.lineCount - 1);
        const range = new vscode.Range(firstLine.range.start, lastLine.range.end);
        
        return [vscode.TextEdit.replace(range, formatted)];
    } catch (error) {
        console.error('Formatting error:', error);
        return [];
    }
}

function formatDocumentRange(document: vscode.TextDocument, range: vscode.Range): vscode.TextEdit[] {
    // For simplicity, format the entire document for now
    // A more sophisticated implementation would only format the selected range
    return formatDocument(document);
}

// Completion support
function provideCompletions(document: vscode.TextDocument, position: vscode.Position): vscode.CompletionItem[] {
    const completions: vscode.CompletionItem[] = [];

    // Keywords
    const keywords = [
        { label: 'fn', kind: vscode.CompletionItemKind.Keyword, detail: 'Function declaration', 
          insertText: 'fn ${1:name}(${2:params}) {\n\t$0\n}', documentation: 'Declare a new function' },
        { label: 'async', kind: vscode.CompletionItemKind.Keyword, detail: 'Async function', 
          insertText: 'async fn ${1:name}(${2:params}) {\n\t$0\n}', documentation: 'Declare an async function' },
        { label: 'let', kind: vscode.CompletionItemKind.Keyword, detail: 'Variable declaration', 
          insertText: 'let ${1:name} = ${0:value}', documentation: 'Declare a variable' },
        { label: 'const', kind: vscode.CompletionItemKind.Keyword, detail: 'Constant declaration', 
          insertText: 'const ${1:name} = ${0:value}', documentation: 'Declare a constant' },
        { label: 'var', kind: vscode.CompletionItemKind.Keyword, detail: 'Variable declaration (function-scoped)', 
          insertText: 'var ${1:name} = ${0:value}', documentation: 'Declare a variable' },
        { label: 'class', kind: vscode.CompletionItemKind.Keyword, detail: 'Class declaration', 
          insertText: 'class ${1:Name} {\n\t$0\n}', documentation: 'Declare a class' },
        { label: 'interface', kind: vscode.CompletionItemKind.Keyword, detail: 'Interface declaration', 
          insertText: 'interface ${1:Name} {\n\t$0\n}', documentation: 'Declare an interface' },
        { label: 'enum', kind: vscode.CompletionItemKind.Keyword, detail: 'Enum declaration', 
          insertText: 'enum ${1:Name} {\n\t$0\n}', documentation: 'Declare an enum' },
        { label: 'if', kind: vscode.CompletionItemKind.Keyword, detail: 'If statement', 
          insertText: 'if ${1:condition} {\n\t$0\n}', documentation: 'Conditional statement' },
        { label: 'else', kind: vscode.CompletionItemKind.Keyword, detail: 'Else clause', 
          insertText: 'else {\n\t$0\n}', documentation: 'Else clause' },
        { label: 'for', kind: vscode.CompletionItemKind.Keyword, detail: 'For loop', 
          insertText: 'for ${1:item} in ${2:collection} {\n\t$0\n}', documentation: 'For loop' },
        { label: 'while', kind: vscode.CompletionItemKind.Keyword, detail: 'While loop', 
          insertText: 'while ${1:condition} {\n\t$0\n}', documentation: 'While loop' },
        { label: 'return', kind: vscode.CompletionItemKind.Keyword, detail: 'Return statement', 
          insertText: 'return ${0:value}', documentation: 'Return from function' },
        { label: 'await', kind: vscode.CompletionItemKind.Keyword, detail: 'Await expression', 
          insertText: 'await ${0:promise}', documentation: 'Wait for promise' },
        { label: 'yield', kind: vscode.CompletionItemKind.Keyword, detail: 'Yield expression', 
          insertText: 'yield ${0:value}', documentation: 'Yield value in generator' },
        { label: 'import', kind: vscode.CompletionItemKind.Keyword, detail: 'Import statement', 
          insertText: 'import { ${1:name} } from "${0:module}"', documentation: 'Import from module' },
        { label: 'export', kind: vscode.CompletionItemKind.Keyword, detail: 'Export statement', 
          insertText: 'export ${0}', documentation: 'Export from module' },
        { label: 'new', kind: vscode.CompletionItemKind.Keyword, detail: 'New instance', 
          insertText: 'new ${1:ClassName}(${0})', documentation: 'Create new instance' },
        { label: 'this', kind: vscode.CompletionItemKind.Keyword, detail: 'This reference', 
          insertText: 'this', documentation: 'Reference to current object' },
        { label: 'static', kind: vscode.CompletionItemKind.Keyword, detail: 'Static member', 
          insertText: 'static ', documentation: 'Static class member' },
        { label: 'public', kind: vscode.CompletionItemKind.Keyword, detail: 'Public access', 
          insertText: 'public ', documentation: 'Public access modifier' },
        { label: 'private', kind: vscode.CompletionItemKind.Keyword, detail: 'Private access', 
          insertText: 'private ', documentation: 'Private access modifier' },
        { label: 'protected', kind: vscode.CompletionItemKind.Keyword, detail: 'Protected access', 
          insertText: 'protected ', documentation: 'Protected access modifier' },
        { label: 'abstract', kind: vscode.CompletionItemKind.Keyword, detail: 'Abstract declaration', 
          insertText: 'abstract ', documentation: 'Abstract class or method' },
        { label: 'extends', kind: vscode.CompletionItemKind.Keyword, detail: 'Class inheritance', 
          insertText: 'extends ${0:Parent}', documentation: 'Inherit from parent class' },
        { label: 'implements', kind: vscode.CompletionItemKind.Keyword, detail: 'Interface implementation', 
          insertText: 'implements ${0:Interface}', documentation: 'Implement interface' },
        { label: 'break', kind: vscode.CompletionItemKind.Keyword, detail: 'Break statement', 
          insertText: 'break', documentation: 'Exit loop' },
        { label: 'continue', kind: vscode.CompletionItemKind.Keyword, detail: 'Continue statement', 
          insertText: 'continue', documentation: 'Skip to next iteration' },
        { label: 'throw', kind: vscode.CompletionItemKind.Keyword, detail: 'Throw exception', 
          insertText: 'throw ${0:error}', documentation: 'Throw exception' },
        { label: 'try', kind: vscode.CompletionItemKind.Keyword, detail: 'Try-catch block', 
          insertText: 'try {\n\t${1}\n} catch (${2:e}) {\n\t${0}\n}', documentation: 'Exception handling' }
    ];

    // Built-in functions
    const builtins = [
        { label: 'println', kind: vscode.CompletionItemKind.Function, detail: 'Print with newline', 
          insertText: 'println(${0:message})', documentation: 'Print message to console with newline' },
        { label: 'print', kind: vscode.CompletionItemKind.Function, detail: 'Print without newline', 
          insertText: 'print(${0:message})', documentation: 'Print message to console' },
        { label: 'len', kind: vscode.CompletionItemKind.Function, detail: 'Get length', 
          insertText: 'len(${0:collection})', documentation: 'Get length of collection' },
        { label: 'typeof', kind: vscode.CompletionItemKind.Function, detail: 'Get type', 
          insertText: 'typeof(${0:value})', documentation: 'Get type of value' },
        { label: 'assert', kind: vscode.CompletionItemKind.Function, detail: 'Assertion', 
          insertText: 'assert(${1:condition}, ${0:message})', documentation: 'Assert condition is true' }
    ];

    // Types
    const types = [
        { label: 'number', kind: vscode.CompletionItemKind.TypeParameter, detail: 'Number type', 
          insertText: 'number', documentation: 'Numeric type' },
        { label: 'string', kind: vscode.CompletionItemKind.TypeParameter, detail: 'String type', 
          insertText: 'string', documentation: 'String type' },
        { label: 'bool', kind: vscode.CompletionItemKind.TypeParameter, detail: 'Boolean type', 
          insertText: 'bool', documentation: 'Boolean type' },
        { label: 'void', kind: vscode.CompletionItemKind.TypeParameter, detail: 'Void type', 
          insertText: 'void', documentation: 'No return value' },
        { label: 'any', kind: vscode.CompletionItemKind.TypeParameter, detail: 'Any type', 
          insertText: 'any', documentation: 'Any type' },
        { label: 'array', kind: vscode.CompletionItemKind.TypeParameter, detail: 'Array type', 
          insertText: 'array<${0:Type}>', documentation: 'Array type' }
    ];

    // Convert to completion items with snippets
    for (const item of [...keywords, ...builtins, ...types]) {
        const completion = new vscode.CompletionItem(item.label, item.kind);
        completion.detail = item.detail;
        completion.documentation = new vscode.MarkdownString(item.documentation);
        completion.insertText = new vscode.SnippetString(item.insertText);
        completions.push(completion);
    }

    return completions;
}

// Linting support
function setupLinting(context: vscode.ExtensionContext) {
    const config = vscode.workspace.getConfiguration('klang');
    
    if (!config.get<boolean>('lint.enabled')) {
        return;
    }

    // Lint on document open
    context.subscriptions.push(
        vscode.workspace.onDidOpenTextDocument(document => {
            if (isKLangFile(document)) {
                lintDocument(document);
            }
        })
    );

    // Lint on document save
    context.subscriptions.push(
        vscode.workspace.onDidSaveTextDocument(document => {
            if (isKLangFile(document)) {
                lintDocument(document);
            }
        })
    );

    // Lint on document change (with debouncing)
    let lintTimeout: NodeJS.Timeout | undefined;
    context.subscriptions.push(
        vscode.workspace.onDidChangeTextDocument(event => {
            if (isKLangFile(event.document)) {
                if (lintTimeout) {
                    clearTimeout(lintTimeout);
                }
                lintTimeout = setTimeout(() => {
                    lintDocument(event.document);
                }, 500); // Debounce for 500ms
            }
        })
    );

    // Lint currently open documents
    vscode.workspace.textDocuments.forEach(document => {
        if (isKLangFile(document)) {
            lintDocument(document);
        }
    });
}

function lintDocument(document: vscode.TextDocument) {
    const config = vscode.workspace.getConfiguration('klang');
    const klangPath = config.get<string>('languageServer.path') || 'klang';
    
    if (!isKLangAvailable(klangPath)) {
        return;
    }

    try {
        const { execSync } = require('child_process');
        const fs = require('fs');
        const os = require('os');
        const tmpDir = os.tmpdir();
        const tmpFile = path.join(tmpDir, `klang-lint-${Date.now()}.kl`);

        // Write current document to temp file
        fs.writeFileSync(tmpFile, document.getText());

        // Run syntax check
        const output = execSync(`${klangPath} check "${tmpFile}" 2>&1`, {
            timeout: 5000,
            encoding: 'utf8'
        });

        // Clean up
        fs.unlinkSync(tmpFile);

        // Parse output for diagnostics
        const diagnostics = parseLintOutput(output, document);
        diagnosticCollection.set(document.uri, diagnostics);
    } catch (error: any) {
        // The check command returns non-zero on errors, which is expected
        if (error.stdout || error.stderr) {
            const output = error.stdout || error.stderr;
            const diagnostics = parseLintOutput(output, document);
            diagnosticCollection.set(document.uri, diagnostics);
        } else {
            // Clear diagnostics if there's an unexpected error
            diagnosticCollection.set(document.uri, []);
        }
    }
}

function parseLintOutput(output: string, document: vscode.TextDocument): vscode.Diagnostic[] {
    const diagnostics: vscode.Diagnostic[] = [];
    const lines = output.split('\n');

    for (const line of lines) {
        // Parse error format: "filename:line:column: error: message"
        // or "Error at line X: message"
        let match = line.match(/^.*:(\d+):(\d+):\s*(error|warning):\s*(.+)$/i);
        if (!match) {
            match = line.match(/^(error|warning)\s+at\s+line\s+(\d+):\s*(.+)$/i);
            if (match) {
                const severity = match[1].toLowerCase() === 'error' ? 
                    vscode.DiagnosticSeverity.Error : 
                    vscode.DiagnosticSeverity.Warning;
                const lineNum = parseInt(match[2]) - 1; // 0-indexed
                const message = match[3];
                
                // Use actual line length instead of magic number
                const line = document.lineAt(Math.max(0, Math.min(lineNum, document.lineCount - 1)));
                const range = new vscode.Range(
                    new vscode.Position(Math.max(0, lineNum), 0),
                    line.range.end
                );
                
                const diagnostic = new vscode.Diagnostic(range, message, severity);
                diagnostic.source = 'klang';
                diagnostics.push(diagnostic);
            }
            continue;
        }

        const lineNum = parseInt(match[1]) - 1; // 0-indexed
        const column = parseInt(match[2]) - 1; // 0-indexed
        const severity = match[3].toLowerCase() === 'error' ? 
            vscode.DiagnosticSeverity.Error : 
            vscode.DiagnosticSeverity.Warning;
        const message = match[4];

        const range = new vscode.Range(
            new vscode.Position(Math.max(0, lineNum), Math.max(0, column)),
            new vscode.Position(Math.max(0, lineNum), Math.max(0, column) + 1)
        );

        const diagnostic = new vscode.Diagnostic(range, message, severity);
        diagnostic.source = 'klang';
        diagnostics.push(diagnostic);
    }

    return diagnostics;
}
