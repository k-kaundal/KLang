#!/usr/bin/env bash
# KLang Plugin Manager CLI

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Configuration
PLUGIN_DIR="${KLANG_PLUGIN_DIR:-$HOME/.klang/plugins}"
REGISTRY_FILE="$PLUGIN_DIR/registry.json"
REGISTRY_URL="https://registry.klang.dev"

# Ensure plugin directory exists
mkdir -p "$PLUGIN_DIR"

# Initialize registry if not exists
if [ ! -f "$REGISTRY_FILE" ]; then
    echo '{"plugins":{},"lastUpdate":0}' > "$REGISTRY_FILE"
fi

# Helper functions
log_info() {
    echo -e "${BLUE}ℹ${NC} $1"
}

log_success() {
    echo -e "${GREEN}✓${NC} $1"
}

log_error() {
    echo -e "${RED}✗${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}⚠${NC} $1"
}

# Plugin commands
cmd_install() {
    local plugin_spec="$1"
    
    if [ -z "$plugin_spec" ]; then
        log_error "Plugin name required"
        echo "Usage: klang plugin install <name>"
        exit 1
    fi
    
    log_info "Installing plugin: $plugin_spec"
    
    # Parse plugin spec (name@version or github:user/repo)
    local plugin_name plugin_version plugin_source
    
    if [[ "$plugin_spec" == github:* ]]; then
        # GitHub source
        plugin_source="${plugin_spec#github:}"
        plugin_name=$(basename "$plugin_source")
        log_info "Installing from GitHub: $plugin_source"
        
        # Clone repository
        local temp_dir=$(mktemp -d)
        if git clone "https://github.com/$plugin_source.git" "$temp_dir" 2>/dev/null; then
            cp -r "$temp_dir" "$PLUGIN_DIR/$plugin_name"
            rm -rf "$temp_dir"
            log_success "Plugin installed: $plugin_name"
        else
            log_error "Failed to clone repository"
            exit 1
        fi
    elif [[ "$plugin_spec" == ./* ]] || [[ "$plugin_spec" == /* ]]; then
        # Local directory
        if [ -d "$plugin_spec" ]; then
            plugin_name=$(basename "$plugin_spec")
            cp -r "$plugin_spec" "$PLUGIN_DIR/$plugin_name"
            log_success "Plugin installed from local directory: $plugin_name"
        else
            log_error "Directory not found: $plugin_spec"
            exit 1
        fi
    else
        # Registry source
        if [[ "$plugin_spec" == *@* ]]; then
            plugin_name="${plugin_spec%@*}"
            plugin_version="${plugin_spec#*@}"
        else
            plugin_name="$plugin_spec"
            plugin_version="latest"
        fi
        
        log_info "Fetching from registry: $plugin_name@$plugin_version"
        log_warn "Plugin registry not yet implemented - use GitHub or local install"
        exit 1
    fi
}

cmd_uninstall() {
    local plugin_name="$1"
    
    if [ -z "$plugin_name" ]; then
        log_error "Plugin name required"
        exit 1
    fi
    
    if [ -d "$PLUGIN_DIR/$plugin_name" ]; then
        rm -rf "$PLUGIN_DIR/$plugin_name"
        log_success "Plugin uninstalled: $plugin_name"
    else
        log_error "Plugin not found: $plugin_name"
        exit 1
    fi
}

cmd_list() {
    log_info "Installed plugins:"
    echo ""
    
    if [ -d "$PLUGIN_DIR" ]; then
        local count=0
        for plugin in "$PLUGIN_DIR"/*/; do
            if [ -d "$plugin" ]; then
                local name=$(basename "$plugin")
                local version="unknown"
                
                # Try to read version from plugin.json
                if [ -f "$plugin/plugin.json" ]; then
                    version=$(grep -o '"version"[[:space:]]*:[[:space:]]*"[^"]*"' "$plugin/plugin.json" | cut -d'"' -f4)
                fi
                
                echo -e "  ${GREEN}•${NC} $name ${YELLOW}($version)${NC}"
                count=$((count + 1))
            fi
        done
        
        if [ $count -eq 0 ]; then
            echo "  No plugins installed"
        fi
    else
        echo "  No plugins installed"
    fi
    echo ""
}

cmd_info() {
    local plugin_name="$1"
    
    if [ -z "$plugin_name" ]; then
        log_error "Plugin name required"
        exit 1
    fi
    
    local plugin_dir="$PLUGIN_DIR/$plugin_name"
    
    if [ ! -d "$plugin_dir" ]; then
        log_error "Plugin not found: $plugin_name"
        exit 1
    fi
    
    log_info "Plugin: $plugin_name"
    echo ""
    
    if [ -f "$plugin_dir/plugin.json" ]; then
        cat "$plugin_dir/plugin.json" | python3 -m json.tool 2>/dev/null || cat "$plugin_dir/plugin.json"
    else
        log_warn "No plugin.json found"
    fi
    echo ""
}

cmd_create() {
    local plugin_name="$1"
    
    if [ -z "$plugin_name" ]; then
        log_error "Plugin name required"
        exit 1
    fi
    
    log_info "Creating plugin: $plugin_name"
    
    # Create plugin directory
    mkdir -p "$plugin_name"/{src,tests,docs}
    
    # Create plugin.json
    cat > "$plugin_name/plugin.json" <<EOF
{
  "name": "$plugin_name",
  "version": "1.0.0",
  "description": "A KLang plugin",
  "author": "Your Name",
  "license": "MIT",
  "klangVersion": ">=0.2.0",
  "type": "library",
  "entry": "src/init.kl",
  "dependencies": {},
  "exports": {},
  "config": {}
}
EOF
    
    # Create init.kl
    cat > "$plugin_name/src/init.kl" <<EOF
# $plugin_name initialization

export class ${plugin_name^}Plugin {
    fn init() {
        println("${plugin_name} initialized!")
    }
}

# Auto-initialize
let instance = ${plugin_name^}Plugin()
instance.init()
EOF
    
    # Create README
    cat > "$plugin_name/README.md" <<EOF
# $plugin_name

KLang plugin description.

## Installation

\`\`\`bash
klang plugin install $plugin_name
\`\`\`

## Usage

\`\`\`klang
import "$plugin_name" as Plugin

# Use plugin
\`\`\`

## License

MIT
EOF
    
    log_success "Plugin created: $plugin_name"
    log_info "Edit $plugin_name/plugin.json and $plugin_name/src/init.kl to customize"
}

cmd_search() {
    local query="$1"
    
    log_info "Searching plugins: $query"
    log_warn "Plugin registry not yet implemented"
}

cmd_update() {
    local plugin_name="$1"
    
    if [ -z "$plugin_name" ]; then
        log_error "Plugin name required"
        exit 1
    fi
    
    log_info "Updating plugin: $plugin_name"
    log_warn "Plugin update not yet implemented"
}

cmd_help() {
    echo "KLang Plugin Manager"
    echo ""
    echo "Usage: klang-plugin <command> [options]"
    echo ""
    echo "Commands:"
    echo "  install <name>    Install a plugin"
    echo "  uninstall <name>  Uninstall a plugin"
    echo "  list              List installed plugins"
    echo "  info <name>       Show plugin information"
    echo "  create <name>     Create a new plugin scaffold"
    echo "  search <query>    Search for plugins"
    echo "  update <name>     Update a plugin"
    echo "  help              Show this help"
    echo ""
    echo "Examples:"
    echo "  klang-plugin install github:user/my-plugin"
    echo "  klang-plugin install ./my-local-plugin"
    echo "  klang-plugin list"
    echo "  klang-plugin create my-new-plugin"
    echo ""
}

# Main command dispatcher
case "${1:-}" in
    install)
        cmd_install "$2"
        ;;
    uninstall)
        cmd_uninstall "$2"
        ;;
    list)
        cmd_list
        ;;
    info)
        cmd_info "$2"
        ;;
    create)
        cmd_create "$2"
        ;;
    search)
        cmd_search "$2"
        ;;
    update)
        cmd_update "$2"
        ;;
    help|--help|-h|"")
        cmd_help
        ;;
    *)
        log_error "Unknown command: $1"
        cmd_help
        exit 1
        ;;
esac
