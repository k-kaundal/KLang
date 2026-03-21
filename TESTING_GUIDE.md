# How to Test the Archive Fix

This document provides step-by-step instructions for testing the archive format fixes and release automation.

## Prerequisites

- Git installed
- C compiler (gcc or clang)
- make
- For local testing: tar, curl or wget
- For full testing: Access to Linux, macOS, and Windows systems

## Testing Strategy

The fix addresses tar archive format issues on macOS and adds comprehensive cross-platform testing. Testing should verify:

1. Archives can be created correctly on Linux (using GNU tar)
2. Archives can be extracted correctly on macOS (using BSD tar)
3. Archives can be extracted correctly on Linux
4. CI workflows run successfully on all platforms
5. Release workflow creates valid artifacts

## Local Testing (Current System)

### Step 1: Build KLang

```bash
cd /path/to/KLang
make clean
make
```

Expected output: Binary `klang` is created without errors

### Step 2: Test Archive Creation

```bash
./test-archive.sh
```

Expected output:
```
╔══════════════════════════════════════════╗
║    KLang Archive Compatibility Test     ║
╚══════════════════════════════════════════╝

Testing package creation for: klang-0.1.0-<platform>-<arch>

✓ GNU tar detected (or BSD tar on macOS)
✓ Archive is valid
✓ Archive format is gzip compressed
✓ Archive extracted successfully
✓ Binary found in extracted archive
✓ Binary is executable
✓ Binary can be executed
✓ Checksum created

╔══════════════════════════════════════════╗
║       All Tests Passed! ✓                ║
╚══════════════════════════════════════════╝
```

### Step 3: Manual Archive Test

```bash
# Create archive with build script
./build.sh --package

# Verify archive was created
ls -lh dist/

# Test extraction
cd /tmp
tar -xzf /path/to/KLang/dist/klang-0.1.0-*.tar.gz
./klang-0.1.0-*/klang repl  # Type 'exit' to quit
```

### Step 4: Test Installation Script

```bash
# Build first
make clean && make

# Test local install
./install.sh --user

# Verify installation
which klang
klang repl  # Type 'exit' to quit
```

## CI Testing (After PR Merge)

Once the PR is merged, the CI workflow will automatically run.

### Step 1: Monitor CI Workflow

1. Go to: https://github.com/k-kaundal/KLang/actions
2. Find the CI workflow run for your commit
3. Verify all jobs complete successfully:
   - ✓ Test on ubuntu-latest
   - ✓ Test on macos-latest
   - ✓ Test on windows-latest

### Step 2: Review CI Logs

Click on each job and verify:
- Build completes without errors
- Tests pass
- Binary is created

### Step 3: Download Artifacts

1. In the CI workflow run, click on "Artifacts"
2. Download binaries for each platform
3. Test locally:

```bash
# Linux/macOS
chmod +x klang
./klang repl  # Type 'exit' to quit

# Windows (PowerShell)
.\klang.exe repl  # Type 'exit' to quit
```

## Release Testing

### Step 1: Trigger a Test Release

**Option A: Using Manual Workflow Dispatch (Recommended for testing)**

1. Go to: https://github.com/k-kaundal/KLang/actions
2. Click on "Release" workflow
3. Click "Run workflow"
4. Enter version: `0.1.0-test` (use -test suffix for testing)
5. Click "Run workflow"

**Option B: Using Git Tag (Production release)**

```bash
git tag -a v0.1.0 -m "Release version 0.1.0"
git push origin v0.1.0
```

### Step 2: Monitor Release Workflow

Watch the workflow progress:
1. Test jobs run first on all platforms
2. Build jobs run after tests pass
3. Release job creates GitHub release

Expected result: All jobs complete with ✓

### Step 3: Verify Release Artifacts

Go to: https://github.com/k-kaundal/KLang/releases

Verify all these files exist:
- `klang-0.1.0-linux-x64.tar.gz` (+ `.sha256`)
- `klang-0.1.0-linux-arm64.tar.gz` (+ `.sha256`)
- `klang-0.1.0-macos-x64.tar.gz` (+ `.sha256`)
- `klang-0.1.0-macos-arm64.tar.gz` (+ `.sha256`)
- `klang-0.1.0-windows-x64.tar.gz` (+ `.sha256`)
- `klang-0.1.0-windows-x64.zip` (+ `.sha256`)

### Step 4: Test Downloads on Each Platform

#### On Linux (x64)

```bash
# Download
wget https://github.com/k-kaundal/KLang/releases/download/v0.1.0/klang-0.1.0-linux-x64.tar.gz
wget https://github.com/k-kaundal/KLang/releases/download/v0.1.0/klang-0.1.0-linux-x64.tar.gz.sha256

# Verify checksum
sha256sum -c klang-0.1.0-linux-x64.tar.gz.sha256

# Extract
tar -xzf klang-0.1.0-linux-x64.tar.gz

# Test
./klang-0.1.0-linux-x64/klang repl  # Type 'exit' to quit
```

#### On macOS (ARM64 - Apple Silicon) - THE ORIGINAL ISSUE

```bash
# Download
curl -LO https://github.com/k-kaundal/KLang/releases/download/v0.1.0/klang-0.1.0-macos-arm64.tar.gz
curl -LO https://github.com/k-kaundal/KLang/releases/download/v0.1.0/klang-0.1.0-macos-arm64.tar.gz.sha256

# Verify checksum
shasum -a 256 -c klang-0.1.0-macos-arm64.tar.gz.sha256

# Extract (THIS WAS FAILING BEFORE)
tar -xzf klang-0.1.0-macos-arm64.tar.gz

# Test
./klang-0.1.0-macos-arm64/klang repl  # Type 'exit' to quit
```

**Expected result**: No "Unrecognized archive format" error ✓

#### On macOS (x64 - Intel)

```bash
# Download
curl -LO https://github.com/k-kaundal/KLang/releases/download/v0.1.0/klang-0.1.0-macos-x64.tar.gz

# Extract
tar -xzf klang-0.1.0-macos-x64.tar.gz

# Test
```

#### On Windows (x64)

```powershell
# Download using browser or:
Invoke-WebRequest -Uri "https://github.com/k-kaundal/KLang/releases/download/v0.1.0/klang-0.1.0-windows-x64.zip" -OutFile "klang-0.1.0-windows-x64.zip"

# Extract
Expand-Archive -Path klang-0.1.0-windows-x64.zip -DestinationPath .

# Test
.\klang-0.1.0-windows-x64\klang.exe repl  # Type 'exit' to quit
```

### Step 5: Test Quick Install Script

This is what users will typically use:

#### On Linux/macOS

```bash
curl -sSL https://raw.githubusercontent.com/k-kaundal/KLang/main/quick-install.sh | bash
```

Expected behavior:
- Detects platform correctly
- Downloads appropriate archive
- Extracts without errors
- Installs to `~/.klang/bin/`
- Adds to PATH

Verify:
```bash
source ~/.bashrc  # or ~/.zshrc
klang repl  # Type 'exit' to quit
```

## Cross-Platform Verification Checklist

Use this checklist to ensure comprehensive testing:

### Archive Format
- [ ] Archive created on Linux extracts on macOS
- [ ] Archive created on Linux extracts on Windows
- [ ] No "Unrecognized archive format" error on any platform
- [ ] All checksums match

### Binary Functionality
- [ ] Binary runs on Linux x64
- [ ] Binary runs on Linux ARM64 (if available)
- [ ] Binary runs on macOS Intel
- [ ] Binary runs on macOS Apple Silicon
- [ ] Binary runs on Windows x64

### Installation
- [ ] Quick install works on Linux
- [ ] Quick install works on macOS
- [ ] Manual install works on all platforms
- [ ] PATH is set correctly

### CI/CD
- [ ] CI workflow passes on all platforms
- [ ] Release workflow creates all artifacts
- [ ] Release notes are correct
- [ ] Checksums are present and valid

## Troubleshooting

### Issue: tar extraction still fails on macOS

**Check:**
1. Verify the archive was created with POSIX format:
   ```bash
   tar -tzf archive.tar.gz  # Should list contents
   file archive.tar.gz      # Should say "gzip compressed data"
   ```

2. Try extracting with verbose output:
   ```bash
   tar -xzvf archive.tar.gz
   ```

3. Check tar version:
   ```bash
   tar --version  # Should show BSD tar on macOS
   ```

**Fix:**
If using an old archive, recreate with the updated build.sh script.

### Issue: Binary won't execute

**Check:**
1. File permissions:
   ```bash
   ls -l klang
   # Should show: -rwxr-xr-x
   ```

2. If not executable:
   ```bash
   chmod +x klang
   ```

3. Architecture mismatch:
   ```bash
   file klang
   uname -m
   # Should match
   ```

### Issue: CI fails on specific platform

**Actions:**
1. Review the CI logs in GitHub Actions
2. Reproduce locally if possible
3. Check for platform-specific issues in the code
4. Update the CI workflow if needed

## Success Criteria

The fix is successful when:

1. ✅ Archives created on Linux can be extracted on macOS without errors
2. ✅ Archives can be extracted on all supported platforms
3. ✅ CI workflow passes on Linux, macOS, and Windows
4. ✅ Release workflow creates valid artifacts for all platforms
5. ✅ Quick install script works on Linux and macOS
6. ✅ Checksums verify correctly
7. ✅ Users can download and run KLang on any supported platform

## Reporting Results

After testing, please report:

1. Platform tested (OS, architecture)
2. Test results (pass/fail for each step)
3. Any errors encountered
4. Logs or screenshots if issues found

Example:
```
Platform: macOS 14.0 (Apple Silicon)
Archive extraction: ✅ PASS
Binary execution: ✅ PASS
Quick install: ✅ PASS
Notes: No errors, extraction worked perfectly
```

## Next Steps

After successful testing:

1. Merge the PR
2. Create official v0.1.0 release
3. Update documentation with release information
4. Announce the release to users
5. Monitor for any issues reported by users
