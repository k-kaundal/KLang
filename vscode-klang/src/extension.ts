import * as vscode from 'vscode';
import * as path from 'path';
import * as fs from 'fs';
import { LanguageClient, LanguageClientOptions, ServerOptions } from 'vscode-languageclient/node';

let client: LanguageClient | undefined;

export function activate(context: vscode.ExtensionContext) {
    console.log('KLang extension is now active!');

    // Register commands
    registerCommands(context);

    // Start Language Server if enabled
    const config = vscode.workspace.getConfiguration('klang');
    if (config.get<boolean>('languageServer.enabled')) {
        activateLanguageServer(context);
    }

    // Set up file watchers and providers
    setupProviders(context);
}

export function deactivate(): Thenable<void> | undefined {
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

    // Language Server is not yet implemented in KLang
    // This is prepared for future LSP support
    const serverOptions: ServerOptions = {
        command: klangPath,
        args: ['lsp']
    };

    const clientOptions: LanguageClientOptions = {
        documentSelector: [{ scheme: 'file', language: 'klang' }],
        synchronize: {
            fileEvents: vscode.workspace.createFileSystemWatcher('**/*.{kl,k,klang}')
        }
    };

    // Uncomment when LSP is implemented
    // client = new LanguageClient('klang', 'KLang Language Server', serverOptions, clientOptions);
    // client.start();
}

function setupProviders(context: vscode.ExtensionContext) {
    // Hover provider for basic documentation
    context.subscriptions.push(
        vscode.languages.registerHoverProvider('klang', {
            provideHover(document, position, token) {
                const range = document.getWordRangeAtPosition(position);
                const word = document.getText(range);

                // Simple keyword documentation
                const keywords: { [key: string]: string } = {
                    'fn': 'Declares a function\n\nSyntax: `fn name(params) { body }`',
                    'async': 'Declares an async function that returns a Promise\n\nSyntax: `async fn name() { ... }`',
                    'await': 'Waits for a Promise to resolve\n\nUsage: `await promise`',
                    'class': 'Declares a class\n\nSyntax: `class Name { ... }`',
                    'extends': 'Inherits from a parent class\n\nSyntax: `class Child extends Parent { ... }`',
                    'let': 'Declares a block-scoped variable\n\nSyntax: `let name = value`',
                    'const': 'Declares a block-scoped constant\n\nSyntax: `const name = value`',
                    'var': 'Declares a function-scoped variable\n\nSyntax: `var name = value`',
                    'if': 'Conditional statement\n\nSyntax: `if (condition) { ... }`',
                    'for': 'Loop statement\n\nSyntax: `for (variable of iterable) { ... }`',
                    'while': 'Loop statement\n\nSyntax: `while (condition) { ... }`',
                    'return': 'Returns a value from a function\n\nSyntax: `return value`',
                    'new': 'Creates a new object instance\n\nSyntax: `new ClassName(args)`',
                    'this': 'References the current object instance',
                    'import': 'Imports modules\n\nSyntax: `import { name } from "module"`',
                    'export': 'Exports values from a module\n\nSyntax: `export const name = value`',
                    'yield': 'Yields a value in a generator\n\nSyntax: `yield value`',
                    'static': 'Declares a static class member\n\nSyntax: `static member = value`',
                    'public': 'Public access modifier (default)',
                    'private': 'Private access modifier',
                    'protected': 'Protected access modifier',
                    'abstract': 'Declares abstract class or method'
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

                // Simple regex-based symbol detection
                for (let i = 0; i < lines.length; i++) {
                    const line = lines[i];

                    // Function declarations
                    const fnMatch = line.match(/^\s*(async\s+)?fn\s+(\w+)\s*\(/);
                    if (fnMatch) {
                        const name = fnMatch[2];
                        const range = new vscode.Range(i, 0, i, line.length);
                        const symbol = new vscode.DocumentSymbol(
                            name,
                            '',
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

                    // Variable declarations
                    const varMatch = line.match(/^\s*(let|const|var)\s+(\w+)/);
                    if (varMatch) {
                        const name = varMatch[2];
                        const range = new vscode.Range(i, 0, i, line.length);
                        const symbol = new vscode.DocumentSymbol(
                            name,
                            '',
                            vscode.SymbolKind.Variable,
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
    // Simple formatter - can be enhanced later
    vscode.window.showInformationMessage('KLang formatter is not yet implemented');
}

function openKLangREPL() {
    const config = vscode.workspace.getConfiguration('klang');
    const klangPath = config.get<string>('languageServer.path') || 'klang';

    const terminal = vscode.window.createTerminal('KLang REPL');
    terminal.show();
    terminal.sendText(`${klangPath} repl`);
}
