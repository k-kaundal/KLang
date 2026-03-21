# KLang Windows Installation Script
# PowerShell script for installing KLang on Windows
# Usage: .\install-windows.ps1 [-User] [-System]

param(
    [switch]$User,
    [switch]$System,
    [switch]$Help
)

$ErrorActionPreference = "Stop"

$KLANG_VERSION = "0.1.0"
$INSTALL_DIR_USER = "$env:LOCALAPPDATA\KLang"
$INSTALL_DIR_SYSTEM = "$env:ProgramFiles\KLang"

function Show-Banner {
    Write-Host ""
    Write-Host "╔══════════════════════════════════════════╗" -ForegroundColor Blue
    Write-Host "║                                          ║" -ForegroundColor Blue
    Write-Host "║         KLang Windows Installer          ║" -ForegroundColor Blue
    Write-Host "║                v$KLANG_VERSION                        ║" -ForegroundColor Blue
    Write-Host "║                                          ║" -ForegroundColor Blue
    Write-Host "╚══════════════════════════════════════════╝" -ForegroundColor Blue
    Write-Host ""
}

function Show-Usage {
    Write-Host "Usage: .\install-windows.ps1 [OPTIONS]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -User      Install for current user (default, no admin required)"
    Write-Host "  -System    Install system-wide (requires admin)"
    Write-Host "  -Help      Show this help message"
    Write-Host ""
    Write-Host "Examples:"
    Write-Host "  .\install-windows.ps1              # Install for current user"
    Write-Host "  .\install-windows.ps1 -User        # Install for current user"
    Write-Host "  .\install-windows.ps1 -System      # Install system-wide (run as admin)"
    Write-Host ""
}

function Check-Binary {
    if (-not (Test-Path ".\klang.exe")) {
        Write-Host "Error: klang.exe not found!" -ForegroundColor Red
        Write-Host "Please build KLang first using build.bat"
        exit 1
    }
}

function Install-User {
    Write-Host "Installing for current user: $INSTALL_DIR_USER" -ForegroundColor Blue
    
    # Create installation directory
    New-Item -ItemType Directory -Force -Path "$INSTALL_DIR_USER\bin" | Out-Null
    
    # Copy binary
    Copy-Item "klang.exe" "$INSTALL_DIR_USER\bin\klang.exe" -Force
    
    Write-Host "✓ Installed to $INSTALL_DIR_USER\bin\klang.exe" -ForegroundColor Green
    
    # Add to user PATH
    $userPath = [Environment]::GetEnvironmentVariable("Path", "User")
    $klangBinPath = "$INSTALL_DIR_USER\bin"
    
    if ($userPath -notlike "*$klangBinPath*") {
        Write-Host "Adding KLang to user PATH..." -ForegroundColor Blue
        
        $newPath = if ($userPath) { "$userPath;$klangBinPath" } else { $klangBinPath }
        [Environment]::SetEnvironmentVariable("Path", $newPath, "User")
        
        # Update current session PATH
        $env:Path = "$env:Path;$klangBinPath"
        
        Write-Host "✓ Added to PATH" -ForegroundColor Green
        Write-Host ""
        Write-Host "NOTE: You may need to restart your terminal/PowerShell" -ForegroundColor Yellow
        Write-Host "      for the PATH changes to take effect" -ForegroundColor Yellow
    } else {
        Write-Host "✓ KLang already in PATH" -ForegroundColor Green
    }
    
    # Copy examples and docs if they exist
    if (Test-Path "examples") {
        Copy-Item -Recurse -Force "examples" "$INSTALL_DIR_USER\examples"
        Write-Host "✓ Installed examples to $INSTALL_DIR_USER\examples" -ForegroundColor Green
    }
    
    if (Test-Path "docs") {
        Copy-Item -Recurse -Force "docs" "$INSTALL_DIR_USER\docs"
        Write-Host "✓ Installed documentation to $INSTALL_DIR_USER\docs" -ForegroundColor Green
    }
}

function Install-System {
    # Check if running as administrator
    $isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
    
    if (-not $isAdmin) {
        Write-Host "Error: System installation requires administrator privileges" -ForegroundColor Red
        Write-Host "Please run PowerShell as Administrator and try again" -ForegroundColor Yellow
        exit 1
    }
    
    Write-Host "Installing system-wide: $INSTALL_DIR_SYSTEM" -ForegroundColor Blue
    
    # Create installation directory
    New-Item -ItemType Directory -Force -Path "$INSTALL_DIR_SYSTEM\bin" | Out-Null
    
    # Copy binary
    Copy-Item "klang.exe" "$INSTALL_DIR_SYSTEM\bin\klang.exe" -Force
    
    Write-Host "✓ Installed to $INSTALL_DIR_SYSTEM\bin\klang.exe" -ForegroundColor Green
    
    # Add to system PATH
    $systemPath = [Environment]::GetEnvironmentVariable("Path", "Machine")
    $klangBinPath = "$INSTALL_DIR_SYSTEM\bin"
    
    if ($systemPath -notlike "*$klangBinPath*") {
        Write-Host "Adding KLang to system PATH..." -ForegroundColor Blue
        
        $newPath = if ($systemPath) { "$systemPath;$klangBinPath" } else { $klangBinPath }
        [Environment]::SetEnvironmentVariable("Path", $newPath, "Machine")
        
        # Update current session PATH
        $env:Path = "$env:Path;$klangBinPath"
        
        Write-Host "✓ Added to system PATH" -ForegroundColor Green
        Write-Host ""
        Write-Host "NOTE: You may need to restart your terminal/PowerShell" -ForegroundColor Yellow
        Write-Host "      for the PATH changes to take effect" -ForegroundColor Yellow
    } else {
        Write-Host "✓ KLang already in system PATH" -ForegroundColor Green
    }
    
    # Copy examples and docs if they exist
    if (Test-Path "examples") {
        Copy-Item -Recurse -Force "examples" "$INSTALL_DIR_SYSTEM\examples"
        Write-Host "✓ Installed examples to $INSTALL_DIR_SYSTEM\examples" -ForegroundColor Green
    }
    
    if (Test-Path "docs") {
        Copy-Item -Recurse -Force "docs" "$INSTALL_DIR_SYSTEM\docs"
        Write-Host "✓ Installed documentation to $INSTALL_DIR_SYSTEM\docs" -ForegroundColor Green
    }
}

function Verify-Installation {
    Write-Host ""
    Write-Host "Verifying installation..." -ForegroundColor Blue
    
    $klangPath = Get-Command klang -ErrorAction SilentlyContinue
    
    if ($klangPath) {
        Write-Host "✓ KLang is installed and working" -ForegroundColor Green
        Write-Host "  Location: $($klangPath.Source)"
        Write-Host ""
        Write-Host "Quick start:" -ForegroundColor Green
        Write-Host "  klang repl                # Interactive mode"
        Write-Host "  klang run script.kl       # Run a script"
        Write-Host ""
    } else {
        Write-Host "Installation completed but 'klang' command not found in PATH" -ForegroundColor Yellow
        Write-Host "Please restart your terminal/PowerShell and try again" -ForegroundColor Yellow
        Write-Host ""
        Write-Host "If the issue persists, manually verify PATH contains:" -ForegroundColor Yellow
        if ($User -or (-not $System)) {
            Write-Host "  $INSTALL_DIR_USER\bin" -ForegroundColor Cyan
        } else {
            Write-Host "  $INSTALL_DIR_SYSTEM\bin" -ForegroundColor Cyan
        }
    }
}

# Main execution
Show-Banner

if ($Help) {
    Show-Usage
    exit 0
}

Check-Binary

if ($System) {
    Install-System
} else {
    # Default to user installation
    Install-User
}

Verify-Installation

Write-Host ""
Write-Host "╔══════════════════════════════════════════╗" -ForegroundColor Green
Write-Host "║                                          ║" -ForegroundColor Green
Write-Host "║    KLang Installation Complete! ✓       ║" -ForegroundColor Green
Write-Host "║                                          ║" -ForegroundColor Green
Write-Host "╚══════════════════════════════════════════╝" -ForegroundColor Green
Write-Host ""
Write-Host "Happy coding with KLang! 🚀"
Write-Host ""
