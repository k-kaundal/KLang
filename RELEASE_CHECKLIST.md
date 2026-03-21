# Release Checklist for KLang

This document outlines the complete process for creating and testing releases across all platforms.

## Pre-Release Testing

Before creating a release, ensure all tests pass on all supported platforms:

### 1. Local Testing

#### Linux (x64)
```bash
make clean
make
make test
./test-archive.sh
```

#### macOS (ARM64/Intel)
```bash
make clean
make
make test
./test-archive.sh
```

#### Windows (x64)
```bash
# Using MSYS2 or MinGW
make clean
make
make test
```

### 2. Continuous Integration Testing

All tests must pass on CI before proceeding with release:
- Check GitHub Actions CI workflow status
- Ensure all platforms (Linux, macOS, Windows) pass tests
- Review test output for any warnings

## Release Process

### Step 1: Prepare Release

1. Update version numbers in:
   - `build.sh` (line 125)
   - `quick-install.sh` (line 15)
   - `install.sh` (line 14)

2. Create or update CHANGELOG.md with release notes

3. Commit changes:
   ```bash
   git add .
   git commit -m "Prepare release v0.1.0"
   git push origin main
   ```

### Step 2: Trigger Release Workflow

#### Option A: Create Git Tag (Automatic Release)
```bash
git tag -a v0.1.0 -m "Release version 0.1.0"
git push origin v0.1.0
```

#### Option B: Manual Workflow Dispatch
1. Go to GitHub Actions
2. Select "Release" workflow
3. Click "Run workflow"
4. Enter version number (e.g., 0.1.0)
5. Click "Run workflow"

### Step 3: Monitor Release Build

1. Watch GitHub Actions workflow progress
2. Ensure all platform builds complete successfully:
   - Linux x64 ✓
   - Linux ARM64 ✓
   - macOS x64 (Intel) ✓
   - macOS ARM64 (Apple Silicon) ✓
   - Windows x64 ✓

### Step 4: Verify Release Artifacts

Once the release is created, verify:

1. All platform archives are uploaded
2. Checksums are present for each archive
3. Release notes are correct
4. Archives can be downloaded

### Step 5: Test Downloads

Test the installation on each platform:

#### Linux
```bash
# Test quick install
curl -sSL https://raw.githubusercontent.com/k-kaundal/KLang/main/quick-install.sh | bash

# Or test manual download
wget https://github.com/k-kaundal/KLang/releases/download/v0.1.0/klang-0.1.0-linux-x64.tar.gz
tar -xzf klang-0.1.0-linux-x64.tar.gz
./klang-0.1.0-linux-x64/klang --version
```

#### macOS
```bash
# Test quick install
curl -sSL https://raw.githubusercontent.com/k-kaundal/KLang/main/quick-install.sh | bash

# Or test manual download
curl -LO https://github.com/k-kaundal/KLang/releases/download/v0.1.0/klang-0.1.0-macos-arm64.tar.gz
tar -xzf klang-0.1.0-macos-arm64.tar.gz
./klang-0.1.0-macos-arm64/klang --version
```

#### Windows
```powershell
# Download and extract manually
# Then test:
.\klang-0.1.0-windows-x64\klang.exe --version
```

### Step 6: Verify Checksums

```bash
# Linux/macOS
sha256sum -c klang-0.1.0-linux-x64.tar.gz.sha256

# macOS (alternative)
shasum -a 256 -c klang-0.1.0-macos-arm64.tar.gz.sha256

# Windows (PowerShell)
$hash = (Get-FileHash klang-0.1.0-windows-x64.zip -Algorithm SHA256).Hash
$expected = (Get-Content klang-0.1.0-windows-x64.zip.sha256).Split()[0]
if ($hash -eq $expected) { Write-Host "Checksum verified!" } else { Write-Host "Checksum mismatch!" }
```

## Post-Release

1. Announce the release:
   - Update README.md if needed
   - Notify users/contributors
   - Post on relevant forums/social media

2. Monitor for issues:
   - Check GitHub issues for installation problems
   - Monitor discussion forums
   - Test on different system configurations

## Troubleshooting

### Issue: Tar extraction fails on macOS

**Symptom**: `tar: Error opening archive: Unrecognized archive format`

**Solutions**:
1. Ensure archive is created with POSIX format: `tar --format=posix -czf`
2. Test archive on target platform before release
3. Provide alternative download options (direct binary download)

### Issue: Binary won't execute on target platform

**Symptom**: Permission denied or incompatible binary

**Solutions**:
1. Ensure binary has execute permissions in archive
2. Verify cross-compilation settings
3. Test on actual hardware (not just emulation)

### Issue: Tests fail on specific platform

**Solutions**:
1. Review test logs in GitHub Actions
2. Reproduce locally if possible
3. Fix platform-specific issues
4. Re-run release workflow

## Platform-Specific Notes

### Linux
- Test on multiple distributions (Ubuntu, Debian, CentOS, Arch)
- Consider glibc version compatibility
- ARM64 builds use cross-compilation

### macOS
- Test on both Intel and Apple Silicon
- Ensure compatibility with macOS 10.15+
- Handle notarization for future releases (when applicable)

### Windows
- Test with different shells (CMD, PowerShell, MSYS2)
- Ensure MSVC runtime compatibility
- Consider code signing for future releases

## Rollback Procedure

If a release has critical issues:

1. Mark the release as "Pre-release" on GitHub
2. Add warning notice to release notes
3. Create hotfix branch
4. Test and create new release
5. Update documentation

## Automation Improvements

Future enhancements:
- [ ] Add automatic changelog generation
- [ ] Implement semantic versioning automation
- [ ] Add smoke tests for downloaded binaries
- [ ] Create Docker images for testing
- [ ] Add binary signing for security
- [ ] Implement automatic rollback on failure
