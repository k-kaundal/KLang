# File I/O

## Introduction

KLang provides comprehensive file system operations for reading, writing, and manipulating files and directories. This guide covers all file I/O capabilities.

## Table of Contents

- [Reading Files](#reading-files)
- [Writing Files](#writing-files)
- [File Operations](#file-operations)
- [Directory Operations](#directory-operations)
- [Path Manipulation](#path-manipulation)
- [File Metadata](#file-metadata)
- [Advanced Operations](#advanced-operations)
- [Best Practices](#best-practices)

## Reading Files

### Read Entire File as String

```klang
// Read text file
let content = readFile("data.txt");
println(content);

// Read with explicit encoding
let content2 = readFile("data.txt", "utf-8");
println(content2);
```

### Read File Line by Line

```klang
let lines = readLines("data.txt");
for (let line of lines) {
    println(line);
}
```

### Read Binary File

```klang
let bytes = readFile("image.png", "binary");
println("File size:", bytes.length, "bytes");
```

### Read JSON File

```klang
let content = readFile("config.json");
let config = JSON.parse(content);
println("Config:", config);
```

### Read with Error Handling

```klang
try {
    let content = readFile("data.txt");
    println(content);
} catch (error) {
    println("Error reading file:", error);
}
```

### Read Large Files

```klang
// Read file in chunks
fn readLargeFile(filename, chunkSize = 1024) {
    let offset = 0;
    let chunks = [];
    
    while (true) {
        let chunk = readFileChunk(filename, offset, chunkSize);
        if (chunk.length == 0) break;
        
        chunks.push(chunk);
        offset += chunk.length;
    }
    
    return chunks.join("");
}

let content = readLargeFile("large-file.txt", 4096);
```

### Read CSV File

```klang
fn readCSV(filename) {
    let lines = readLines(filename);
    let headers = lines[0].split(",");
    let data = [];
    
    for (let i = 1; i < lines.length; i++) {
        let values = lines[i].split(",");
        let row = {};
        
        for (let j = 0; j < headers.length; j++) {
            row[headers[j]] = values[j];
        }
        
        data.push(row);
    }
    
    return data;
}

let users = readCSV("users.csv");
for (let user of users) {
    println(user.name, user.email);
}
```

### Stream Reading

```klang
// Open file for reading
let file = openFile("data.txt", "r");

while (!file.eof()) {
    let line = file.readLine();
    println(line);
}

file.close();
```

## Writing Files

### Write String to File

```klang
// Write (overwrites existing)
writeFile("output.txt", "Hello, World!\n");

// Append to file
appendFile("output.txt", "More text\n");
```

### Write Multiple Lines

```klang
let lines = [
    "Line 1",
    "Line 2",
    "Line 3"
];

writeFile("output.txt", lines.join("\n"));
```

### Write JSON to File

```klang
let data = {
    name: "John",
    age: 30,
    email: "john@example.com"
};

writeFile("data.json", JSON.stringify(data, null, 2));
```

### Write Binary Data

```klang
let bytes = [0x89, 0x50, 0x4E, 0x47];  // PNG header
writeFile("output.bin", bytes, "binary");
```

### Write CSV File

```klang
fn writeCSV(filename, data, headers) {
    let lines = [headers.join(",")];
    
    for (let row of data) {
        let values = headers.map(h => row[h]);
        lines.push(values.join(","));
    }
    
    writeFile(filename, lines.join("\n"));
}

let users = [
    { name: "John", age: 30, email: "john@example.com" },
    { name: "Jane", age: 25, email: "jane@example.com" }
];

writeCSV("users.csv", users, ["name", "age", "email"]);
```

### Atomic Write

```klang
fn atomicWrite(filename, content) {
    let tempFile = filename + ".tmp";
    
    try {
        writeFile(tempFile, content);
        renameFile(tempFile, filename);
    } catch (error) {
        deleteFile(tempFile);
        throw error;
    }
}

atomicWrite("important.txt", "Critical data");
```

### Buffered Writing

```klang
// Open file for writing
let file = openFile("output.txt", "w");

file.write("Line 1\n");
file.write("Line 2\n");
file.write("Line 3\n");

file.close();
```

### Write with Backup

```klang
fn writeWithBackup(filename, content) {
    if (fileExists(filename)) {
        let backup = filename + ".bak";
        copyFile(filename, backup);
    }
    
    writeFile(filename, content);
}

writeWithBackup("config.txt", "New configuration");
```

## File Operations

### Check if File Exists

```klang
if (fileExists("data.txt")) {
    println("File exists");
} else {
    println("File does not exist");
}
```

### Copy File

```klang
copyFile("source.txt", "destination.txt");
println("File copied");
```

### Move/Rename File

```klang
// Rename in same directory
renameFile("old-name.txt", "new-name.txt");

// Move to different directory
moveFile("file.txt", "backups/file.txt");
```

### Delete File

```klang
if (fileExists("temp.txt")) {
    deleteFile("temp.txt");
    println("File deleted");
}
```

### Get File Size

```klang
let size = fileSize("data.txt");
println("File size:", size, "bytes");

// Format file size
fn formatSize(bytes) {
    if (bytes < 1024) return bytes + " B";
    if (bytes < 1024 * 1024) return (bytes / 1024).toFixed(2) + " KB";
    if (bytes < 1024 * 1024 * 1024) return (bytes / 1024 / 1024).toFixed(2) + " MB";
    return (bytes / 1024 / 1024 / 1024).toFixed(2) + " GB";
}

println("Size:", formatSize(size));
```

### Get File Timestamps

```klang
let stats = fileStats("data.txt");
println("Created:", stats.created);
println("Modified:", stats.modified);
println("Accessed:", stats.accessed);
```

### Set File Permissions

```klang
// Make file read-only
setFilePermissions("data.txt", "r--r--r--");

// Make file executable
setFilePermissions("script.sh", "rwxr-xr-x");
```

### Get File Extension

```klang
fn getExtension(filename) {
    let parts = filename.split(".");
    return parts[parts.length - 1];
}

println(getExtension("data.txt"));  // "txt"
println(getExtension("archive.tar.gz"));  // "gz"
```

### Get File Name Without Extension

```klang
fn getBaseName(filename) {
    let parts = filename.split(".");
    parts.pop();
    return parts.join(".");
}

println(getBaseName("document.pdf"));  // "document"
println(getBaseName("archive.tar.gz"));  // "archive.tar"
```

## Directory Operations

### List Directory Contents

```klang
let files = listDir(".");
for (let file of files) {
    println(file);
}
```

### List with Details

```klang
let entries = listDir(".", { details: true });
for (let entry of entries) {
    println(entry.name, entry.type, entry.size);
}
```

### Recursive Directory Listing

```klang
fn listRecursive(dir) {
    let results = [];
    let entries = listDir(dir);
    
    for (let entry of entries) {
        let path = dir + "/" + entry;
        results.push(path);
        
        if (isDirectory(path)) {
            let subResults = listRecursive(path);
            results = results.concat(subResults);
        }
    }
    
    return results;
}

let allFiles = listRecursive(".");
for (let file of allFiles) {
    println(file);
}
```

### Create Directory

```klang
// Create single directory
createDir("new-folder");

// Create nested directories
createDir("path/to/nested/folder", { recursive: true });
```

### Check if Directory Exists

```klang
if (dirExists("folder")) {
    println("Directory exists");
} else {
    createDir("folder");
}
```

### Delete Directory

```klang
// Delete empty directory
deleteDir("empty-folder");

// Delete directory and contents
deleteDir("folder", { recursive: true });
```

### Copy Directory

```klang
fn copyDir(source, dest) {
    createDir(dest, { recursive: true });
    
    let entries = listDir(source);
    for (let entry of entries) {
        let srcPath = source + "/" + entry;
        let destPath = dest + "/" + entry;
        
        if (isDirectory(srcPath)) {
            copyDir(srcPath, destPath);
        } else {
            copyFile(srcPath, destPath);
        }
    }
}

copyDir("source-folder", "destination-folder");
```

### Get Current Directory

```klang
let cwd = getCurrentDir();
println("Current directory:", cwd);
```

### Change Directory

```klang
changeDir("/path/to/directory");
println("Changed to:", getCurrentDir());
```

### Get Home Directory

```klang
let home = getHomeDir();
println("Home directory:", home);
```

### Get Temp Directory

```klang
let temp = getTempDir();
println("Temp directory:", temp);
```

## Path Manipulation

### Join Paths

```klang
let path = joinPath("folder", "subfolder", "file.txt");
println(path);  // "folder/subfolder/file.txt"
```

### Normalize Path

```klang
let normalized = normalizePath("./folder/../file.txt");
println(normalized);  // "file.txt"
```

### Get Absolute Path

```klang
let absolute = absolutePath("file.txt");
println(absolute);  // "/current/directory/file.txt"
```

### Get Relative Path

```klang
let relative = relativePath("/home/user/documents/file.txt", "/home/user");
println(relative);  // "documents/file.txt"
```

### Split Path

```klang
let parts = splitPath("/home/user/documents/file.txt");
println("Directory:", parts.dir);  // "/home/user/documents"
println("File:", parts.name);  // "file.txt"
println("Extension:", parts.ext);  // ".txt"
```

### Get Directory Name

```klang
let dir = dirname("/home/user/file.txt");
println(dir);  // "/home/user"
```

### Get Base Name

```klang
let base = basename("/home/user/file.txt");
println(base);  // "file.txt"
```

### Is Absolute Path

```klang
println(isAbsolutePath("/home/user"));  // true
println(isAbsolutePath("file.txt"));  // false
```

## File Metadata

### Get File Information

```klang
let info = fileInfo("data.txt");
println("Name:", info.name);
println("Size:", info.size);
println("Type:", info.type);
println("Permissions:", info.permissions);
println("Created:", info.created);
println("Modified:", info.modified);
println("Is file:", info.isFile);
println("Is directory:", info.isDirectory);
```

### Check File Type

```klang
if (isFile("data.txt")) {
    println("It's a file");
}

if (isDirectory("folder")) {
    println("It's a directory");
}

if (isSymlink("link")) {
    println("It's a symbolic link");
}
```

### Get File Hash

```klang
// MD5 hash
let md5 = fileHash("data.txt", "md5");
println("MD5:", md5);

// SHA256 hash
let sha256 = fileHash("data.txt", "sha256");
println("SHA256:", sha256);
```

### Compare Files

```klang
fn filesAreEqual(file1, file2) {
    let content1 = readFile(file1);
    let content2 = readFile(file2);
    return content1 == content2;
}

if (filesAreEqual("file1.txt", "file2.txt")) {
    println("Files are identical");
}
```

## Advanced Operations

### Watch File Changes

```klang
// Watch file for changes
let watcher = watchFile("config.txt", (event) => {
    println("File changed:", event.type);
    
    if (event.type == "modified") {
        let content = readFile("config.txt");
        println("New content:", content);
    }
});

// Stop watching
watcher.stop();
```

### Create Temporary File

```klang
fn createTempFile(prefix = "tmp", suffix = ".txt") {
    let temp = getTempDir();
    let filename = prefix + "_" + Date.now() + suffix;
    let path = joinPath(temp, filename);
    writeFile(path, "");
    return path;
}

let tempFile = createTempFile("myapp");
writeFile(tempFile, "Temporary data");
// ... use temp file ...
deleteFile(tempFile);
```

### File Locking

```klang
// Lock file for exclusive access
let lock = lockFile("data.txt");

try {
    let content = readFile("data.txt");
    // Process content
    writeFile("data.txt", modifiedContent);
} finally {
    lock.release();
}
```

### Memory-Mapped Files

```klang
// Map file to memory for fast access
let mapped = mapFile("large-file.bin");

// Access bytes
let byte = mapped[1000];

// Modify
mapped[1000] = 255;

// Unmap
mapped.unmap();
```

### File Compression

```klang
// Compress file
compressFile("data.txt", "data.txt.gz");

// Decompress file
decompressFile("data.txt.gz", "data.txt");
```

### Search in Files

```klang
fn searchInFile(filename, pattern) {
    let lines = readLines(filename);
    let matches = [];
    
    for (let i = 0; i < lines.length; i++) {
        if (lines[i].includes(pattern)) {
            matches.push({
                line: i + 1,
                text: lines[i]
            });
        }
    }
    
    return matches;
}

let results = searchInFile("log.txt", "ERROR");
for (let match of results) {
    println("Line", match.line + ":", match.text);
}
```

### Find Files

```klang
fn findFiles(dir, pattern) {
    let results = [];
    let entries = listDir(dir);
    
    for (let entry of entries) {
        let path = joinPath(dir, entry);
        
        if (isDirectory(path)) {
            let subResults = findFiles(path, pattern);
            results = results.concat(subResults);
        } else if (entry.includes(pattern)) {
            results.push(path);
        }
    }
    
    return results;
}

let txtFiles = findFiles(".", ".txt");
for (let file of txtFiles) {
    println(file);
}
```

### File Backup System

```klang
class BackupManager {
    constructor(sourceDir, backupDir) {
        this.sourceDir = sourceDir;
        this.backupDir = backupDir;
        
        if (!dirExists(backupDir)) {
            createDir(backupDir, { recursive: true });
        }
    }
    
    backup() {
        let timestamp = new Date().toISOString().replace(/:/g, "-");
        let backupPath = joinPath(this.backupDir, "backup_" + timestamp);
        
        copyDir(this.sourceDir, backupPath);
        println("Backup created:", backupPath);
        
        return backupPath;
    }
    
    listBackups() {
        let backups = listDir(this.backupDir);
        return backups.filter(name => name.startsWith("backup_"));
    }
    
    restore(backupName) {
        let backupPath = joinPath(this.backupDir, backupName);
        
        if (!dirExists(backupPath)) {
            throw "Backup not found: " + backupName;
        }
        
        // Clear source directory
        deleteDir(this.sourceDir, { recursive: true });
        
        // Restore from backup
        copyDir(backupPath, this.sourceDir);
        println("Restored from:", backupName);
    }
    
    cleanup(keepLast = 5) {
        let backups = this.listBackups().sort();
        
        while (backups.length > keepLast) {
            let oldest = backups.shift();
            let path = joinPath(this.backupDir, oldest);
            deleteDir(path, { recursive: true });
            println("Deleted old backup:", oldest);
        }
    }
}

// Usage
let backup = new BackupManager("./data", "./backups");

// Create backup
backup.backup();

// List backups
let backups = backup.listBackups();
println("Backups:", backups);

// Cleanup old backups
backup.cleanup(5);
```

### File Sync

```klang
fn syncDirectories(source, dest) {
    if (!dirExists(dest)) {
        createDir(dest, { recursive: true });
    }
    
    let sourceFiles = listDir(source);
    let destFiles = listDir(dest);
    
    // Copy new/modified files
    for (let file of sourceFiles) {
        let srcPath = joinPath(source, file);
        let destPath = joinPath(dest, file);
        
        if (isFile(srcPath)) {
            if (!fileExists(destPath)) {
                copyFile(srcPath, destPath);
                println("Copied:", file);
            } else {
                let srcModified = fileInfo(srcPath).modified;
                let destModified = fileInfo(destPath).modified;
                
                if (srcModified > destModified) {
                    copyFile(srcPath, destPath);
                    println("Updated:", file);
                }
            }
        } else if (isDirectory(srcPath)) {
            syncDirectories(srcPath, destPath);
        }
    }
    
    // Delete files not in source
    for (let file of destFiles) {
        let srcPath = joinPath(source, file);
        let destPath = joinPath(dest, file);
        
        if (!fileExists(srcPath) && !dirExists(srcPath)) {
            if (isFile(destPath)) {
                deleteFile(destPath);
            } else {
                deleteDir(destPath, { recursive: true });
            }
            println("Deleted:", file);
        }
    }
}

syncDirectories("./source", "./destination");
```

## Best Practices

### 1. Always Handle Errors

```klang
fn safeReadFile(filename, defaultValue = "") {
    try {
        return readFile(filename);
    } catch (error) {
        println("Warning: Could not read file:", error);
        return defaultValue;
    }
}
```

### 2. Close Files Properly

```klang
let file = openFile("data.txt", "r");
try {
    let content = file.read();
    // Process content
} finally {
    file.close();
}
```

### 3. Use Absolute Paths

```klang
// Convert to absolute path
let absolutePath = absolutePath("data.txt");
let content = readFile(absolutePath);
```

### 4. Validate File Operations

```klang
fn ensureFileExists(filename) {
    if (!fileExists(filename)) {
        throw "File not found: " + filename;
    }
}

fn ensureDirExists(dirname) {
    if (!dirExists(dirname)) {
        createDir(dirname, { recursive: true });
    }
}
```

### 5. Use Temp Files for Safety

```klang
fn safeFileUpdate(filename, updateFn) {
    let tempFile = filename + ".tmp";
    
    try {
        let content = readFile(filename);
        let newContent = updateFn(content);
        
        writeFile(tempFile, newContent);
        renameFile(tempFile, filename);
    } catch (error) {
        if (fileExists(tempFile)) {
            deleteFile(tempFile);
        }
        throw error;
    }
}
```

## Common Patterns

### Configuration File Manager

```klang
class ConfigManager {
    constructor(filename) {
        this.filename = filename;
        this.config = this.load();
    }
    
    load() {
        if (fileExists(this.filename)) {
            let content = readFile(this.filename);
            return JSON.parse(content);
        }
        return {};
    }
    
    save() {
        let content = JSON.stringify(this.config, null, 2);
        writeFile(this.filename, content);
    }
    
    get(key, defaultValue = null) {
        return this.config[key] || defaultValue;
    }
    
    set(key, value) {
        this.config[key] = value;
        this.save();
    }
    
    delete(key) {
        delete this.config[key];
        this.save();
    }
}

// Usage
let config = new ConfigManager("config.json");
config.set("apiKey", "abc123");
let apiKey = config.get("apiKey");
```

### Log File Manager

```klang
class Logger {
    constructor(filename) {
        this.filename = filename;
    }
    
    log(level, message) {
        let timestamp = new Date().toISOString();
        let entry = `[${timestamp}] [${level}] ${message}\n`;
        appendFile(this.filename, entry);
    }
    
    info(message) {
        this.log("INFO", message);
    }
    
    error(message) {
        this.log("ERROR", message);
    }
    
    warn(message) {
        this.log("WARN", message);
    }
    
    clear() {
        writeFile(this.filename, "");
    }
}

// Usage
let logger = new Logger("app.log");
logger.info("Application started");
logger.error("Something went wrong");
```

## Next Steps

- Learn about [HTTP Client](40-http-client.md) for network operations
- Explore [JSON](43-json.md) for data serialization
- Check out [Environment Variables](42-environment.md) for configuration
- See [Complete Examples](63-examples-projects.md) for file-based applications

---

*Documentation for KLang File I/O - Version 1.0*
