#!/bin/bash

# Documentation Link Validator for KLang
# This script validates all markdown links in the repository

echo "🔍 KLang Documentation Link Validator"
echo "======================================"
echo ""

REPO_ROOT="/home/runner/work/KLang/KLang"
cd "$REPO_ROOT" || exit 1

BROKEN_LINKS=0
CHECKED_LINKS=0
FILES_WITH_ISSUES=0

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to check if a file exists relative to another file
check_link() {
    local source_file="$1"
    local link_path="$2"
    local source_dir=$(dirname "$source_file")
    
    # Remove anchor tags
    link_path="${link_path%%#*}"
    
    # Skip external links
    if [[ "$link_path" =~ ^https?:// ]]; then
        return 0
    fi
    
    # Resolve relative path
    local full_path
    if [[ "$link_path" = /* ]]; then
        # Absolute path from repo root
        full_path="$REPO_ROOT$link_path"
    else
        # Relative path
        full_path="$source_dir/$link_path"
    fi
    
    # Normalize path
    full_path=$(realpath -m "$full_path" 2>/dev/null)
    
    # Check if file exists
    if [ ! -f "$full_path" ]; then
        echo -e "${RED}  ✗ Broken:${NC} $link_path"
        echo -e "    ${YELLOW}From:${NC} $source_file"
        echo -e "    ${YELLOW}Resolved to:${NC} $full_path"
        echo ""
        return 1
    fi
    
    return 0
}

# Find all markdown files
echo "📝 Scanning markdown files..."
echo ""

FILES=$(find "$REPO_ROOT" -name "*.md" -not -path "*/node_modules/*" -not -path "*/.git/*" -not -path "*/build/*")
FILE_COUNT=$(echo "$FILES" | wc -l)

echo "Found $FILE_COUNT markdown files"
echo ""
echo "Checking links..."
echo ""

while IFS= read -r file; do
    # Extract markdown links: [text](path)
    LINKS=$(grep -oP '\[([^\]]+)\]\(([^\)]+)\)' "$file" 2>/dev/null | grep -oP '\(([^\)]+)\)' | tr -d '()')
    
    if [ -z "$LINKS" ]; then
        continue
    fi
    
    FILE_HAS_ISSUES=0
    
    while IFS= read -r link; do
        if [ -z "$link" ]; then
            continue
        fi
        
        CHECKED_LINKS=$((CHECKED_LINKS + 1))
        
        if ! check_link "$file" "$link"; then
            BROKEN_LINKS=$((BROKEN_LINKS + 1))
            FILE_HAS_ISSUES=1
        fi
    done <<< "$LINKS"
    
    if [ $FILE_HAS_ISSUES -eq 1 ]; then
        FILES_WITH_ISSUES=$((FILES_WITH_ISSUES + 1))
    fi
done <<< "$FILES"

echo ""
echo "======================================"
echo "📊 Summary"
echo "======================================"
echo ""
echo "Files scanned: $FILE_COUNT"
echo "Links checked: $CHECKED_LINKS"
echo ""

if [ $BROKEN_LINKS -eq 0 ]; then
    echo -e "${GREEN}✅ All links are valid!${NC}"
    echo ""
    exit 0
else
    echo -e "${RED}❌ Found $BROKEN_LINKS broken link(s) in $FILES_WITH_ISSUES file(s)${NC}"
    echo ""
    exit 1
fi
