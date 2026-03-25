# KLang Windows Installer
# PowerShell installation script for Windows

$ErrorActionPreference = "Stop"

# Configuration
$GITHUB_REPO = "k-kaundal/KLang"
$INSTALL_DIR = "$env:LOCALAPPDATA\KLang"
$BIN_DIR = "$INSTALL_DIR\bin"

# Auto-detect latest version from GitHub API or fallback
function Get-LatestVersion {
    param($Repo)
    try {
        $release = Invoke-RestMethod -Uri "https://api.github.com/repos/$Repo/releases/latest" -ErrorAction SilentlyContinue
        if ($release.tag_name) {
            return $release.tag_name
        }
    } catch {}
    
    try {
        $version = Invoke-RestMethod -Uri "https://raw.githubusercontent.com/$Repo/main/VERSION" -ErrorAction SilentlyContinue
        if ($version) {
            return "v$($version.Trim())"
        }
    } catch {}
    
    return "v1.0.0-rc"
}

$KLANG_VERSION = if ($env:KLANG_VERSION) { $env:KLANG_VERSION } else { Get-LatestVersion -Repo $GITHUB_REPO }

# Colors (ANSI support in Windows Terminal)
$GREEN = "`e[32m"
$BLUE = "`e[34m"
$YELLOW = "`e[33m"
$RED = "`e[31m"
$NC = "`e[0m"

function Write-ColorOutput($ForegroundColor) {
    $fc = $host.UI.RawUI.ForegroundColor
    $host.UI.RawUI.ForegroundColor = $ForegroundColor
    if ($args) {
        Write-Output $args
    }
    $host.UI.RawUI.ForegroundColor = $fc
}

function Show-Banner {
    Write-Host ""
    Write-ColorOutput Blue "╔═══════════════════════════════════════════════════════╗"
    Write-ColorOutput Blue "║                                                       ║"
    Write-ColorOutput Blue "║              KLang Windows Installer                  ║"
    Write-ColorOutput Blue "║                                                       ║"
    Write-ColorOutput Blue "╚═══════════════════════════════════════════════════════╝"
    Write-Host ""
}

function Test-Administrator {
    $currentUser = New-Object Security.Principal.WindowsPrincipal([Security.Principal.WindowsIdentity]::GetCurrent())
    return $currentUser.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
}

function Install-Dependencies {
    Write-ColorOutput Blue "Checking dependencies..."
    
    # Check for Git
    try {
        $gitVersion = git --version 2>$null
        Write-ColorOutput Green "✓ Git is installed: $gitVersion"
    }
    catch {
        Write-ColorOutput Yellow "! Git not found"
        Write-ColorOutput Yellow "  Please install Git from: https://git-scm.com/download/win"
        exit 1
    }
    
    # Check for MinGW/MSYS2 or Visual Studio
    $hasCompiler = $false
    
    try {
        $gccVersion = gcc --version 2>$null
        if ($?) {
            Write-ColorOutput Green "✓ GCC is installed"
            $hasCompiler = $true
        }
    }
    catch {}
    
    if (-not $hasCompiler) {
        Write-ColorOutput Yellow "! C compiler not found"
        Write-ColorOutput Yellow "  Please install either:"
        Write-ColorOutput Yellow "    - MSYS2: https://www.msys2.org/"
        Write-ColorOutput Yellow "    - Visual Studio Build Tools"
        exit 1
    }
}

function Install-KLang {
    Write-ColorOutput Blue "Installing KLang to $INSTALL_DIR..."
    
    # Create installation directory
    New-Item -ItemType Directory -Force -Path $BIN_DIR | Out-Null
    
    # Create temporary directory
    $tempDir = New-TemporaryFile | ForEach-Object { Remove-Item $_; New-Item -ItemType Directory -Path $_ }
    
    try {
        # Clone repository
        Write-ColorOutput Blue "Downloading KLang..."
        git clone --depth 1 --branch "v$KLANG_VERSION" `
            "https://github.com/$GITHUB_REPO.git" "$tempDir\klang" 2>&1 | Out-Null
        
        if (-not $?) {
            # Fallback to main branch
            git clone --depth 1 `
                "https://github.com/$GITHUB_REPO.git" "$tempDir\klang" 2>&1 | Out-Null
        }
        
        Set-Location "$tempDir\klang"
        
        # Build KLang
        Write-ColorOutput Blue "Building KLang..."
        & make clean 2>&1 | Out-Null
        & make 2>&1 | Out-Null
        
        if (-not (Test-Path "klang.exe")) {
            throw "Build failed - klang.exe not found"
        }
        
        # Install binary
        Copy-Item "klang.exe" "$BIN_DIR\" -Force
        
        # Copy examples and docs
        if (Test-Path "examples") {
            Copy-Item "examples" "$INSTALL_DIR\" -Recurse -Force
        }
        if (Test-Path "docs") {
            Copy-Item "docs" "$INSTALL_DIR\" -Recurse -Force
        }
        if (Test-Path "stdlib") {
            Copy-Item "stdlib" "$INSTALL_DIR\" -Recurse -Force
        }
        
        Write-ColorOutput Green "✓ KLang installed to $BIN_DIR\klang.exe"
    }
    finally {
        Set-Location $HOME
        Remove-Item -Recurse -Force $tempDir -ErrorAction SilentlyContinue
    }
}

function Add-ToPath {
    Write-ColorOutput Blue "Adding KLang to PATH..."
    
    # Get current user PATH
    $userPath = [Environment]::GetEnvironmentVariable("Path", "User")
    
    # Check if already in PATH
    if ($userPath -like "*$BIN_DIR*") {
        Write-ColorOutput Green "✓ KLang is already in PATH"
        return
    }
    
    # Add to PATH
    $newPath = "$userPath;$BIN_DIR"
    [Environment]::SetEnvironmentVariable("Path", $newPath, "User")
    
    # Update current session PATH
    $env:Path = "$env:Path;$BIN_DIR"
    
    Write-ColorOutput Green "✓ Added KLang to PATH"
    Write-ColorOutput Yellow "  Note: Restart your terminal for changes to take effect"
}

function Verify-Installation {
    Write-Host ""
    Write-ColorOutput Blue "Verifying installation..."
    
    if (Test-Path "$BIN_DIR\klang.exe") {
        try {
            $version = & "$BIN_DIR\klang.exe" --version 2>&1
            Write-ColorOutput Green "✓ KLang installed successfully!"
            Write-ColorOutput Green "  Version: $version"
        }
        catch {
            Write-ColorOutput Red "✗ KLang verification failed"
            exit 1
        }
    }
    else {
        Write-ColorOutput Red "✗ Installation failed - binary not found"
        exit 1
    }
}

function Show-NextSteps {
    Write-Host ""
    Write-ColorOutput Green "╔═══════════════════════════════════════════════════════╗"
    Write-ColorOutput Green "║                                                       ║"
    Write-ColorOutput Green "║            Installation Complete! ✓                   ║"
    Write-ColorOutput Green "║                                                       ║"
    Write-ColorOutput Green "╚═══════════════════════════════════════════════════════╝"
    Write-Host ""
    
    Write-ColorOutput Yellow "Next Steps:"
    Write-Host ""
    Write-ColorOutput Blue "1. Restart your terminal"
    Write-Host ""
    Write-ColorOutput Blue "2. Try KLang:"
    Write-ColorOutput Cyan "   klang --version"
    Write-ColorOutput Cyan "   klang repl"
    Write-ColorOutput Cyan "   klang help"
    Write-Host ""
    Write-ColorOutput Blue "3. Explore examples:"
    Write-ColorOutput Cyan "   cd $INSTALL_DIR\examples"
    Write-Host ""
}

# Main execution
try {
    Show-Banner
    Install-Dependencies
    Install-KLang
    Add-ToPath
    Verify-Installation
    Show-NextSteps
}
catch {
    Write-ColorOutput Red "Installation failed: $_"
    exit 1
}
