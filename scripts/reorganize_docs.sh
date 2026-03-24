#!/bin/bash
# KLang Documentation Reorganization Script
# This script reorganizes 184 markdown files into a clean structure
# Archive 90-100 files, consolidate duplicates, improve navigation

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}╔════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║   KLang Documentation Reorganization Script       ║${NC}"
echo -e "${BLUE}║   Cleaning up 184 files → ~90 essential files     ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════╝${NC}"
echo ""

# Confirm before proceeding
echo -e "${YELLOW}This will:${NC}"
echo "  • Create docs/archive/ and subdirectories"
echo "  • Move 90-100 files to archive/"
echo "  • Reorganize remaining files into user/developer/api structure"
echo "  • Update docs/README.md with new navigation"
echo ""
read -p "Continue? (y/N) " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo -e "${RED}Aborted.${NC}"
    exit 1
fi

echo ""
echo -e "${GREEN}Starting reorganization...${NC}"
echo ""

# ============================================================================
# PHASE 1: Create New Directory Structure
# ============================================================================
echo -e "${BLUE}[Phase 1]${NC} Creating new directory structure..."

mkdir -p docs/archive/root-files
mkdir -p docs/archive/features
mkdir -p docs/archive/implementation
mkdir -p docs/user
mkdir -p docs/guides
mkdir -p docs/api/oop
mkdir -p docs/api/cli
mkdir -p docs/api/ai
mkdir -p docs/api/stdlib
mkdir -p docs/api/operators
mkdir -p docs/developer/pr-templates

echo -e "${GREEN}✓ Directories created${NC}"

# ============================================================================
# PHASE 2: Archive Root-Level Session Notes (40 files)
# ============================================================================
echo -e "${BLUE}[Phase 2]${NC} Archiving root-level session notes..."

# List of files to archive from root
ROOT_ARCHIVE_FILES=(
    "100_PERCENT_ACHIEVEMENT.md"
    "AI_FEATURES_INDEX.md"
    "AI_IMPLEMENTATION_NOTES.md"
    "AI_MISSION_COMPLETE.md"
    "AI_NATIVE_ANNOUNCEMENT.md"
    "AI_NATIVE_IMPLEMENTATION.md"
    "ALL_DOCUMENTATION_INDEX.md"
    "CLI_ANALYSIS_SUMMARY.md"
    "CLI_IMPLEMENTATION_SUMMARY.md"
    "CLOSURE_FIX_SUMMARY.md"
    "COLLECTIONS_IMPLEMENTATION_SUMMARY.md"
    "COMPREHENSIVE_TEST_REPORT.md"
    "C_STYLE_FOR_LOOP_IMPLEMENTATION.md"
    "DEFAULT_PARAMETERS_IMPLEMENTATION.md"
    "DOCUMENTATION_COMPLETENESS.md"
    "DOCUMENTATION_STRUCTURE.md"
    "FILE_IO_IMPLEMENTATION.md"
    "FINAL_STATUS.md"
    "FINAL_SUMMARY.md"
    "FULL_IMPLEMENTATION_COMPLETE.md"
    "IMPLEMENTATION_COMPLETE.md"
    "IMPLEMENTATION_SUMMARY.md"
    "LLVM_BACKEND_SUMMARY.md"
    "LLVM_FINAL_SUMMARY.md"
    "LLVM_IMPLEMENTATION.md"
    "LLVM_STATUS_OLD.md"
    "MISSING_FEATURES.md"
    "OPERATORS_FINAL_REPORT.md"
    "OPERATORS_IMPLEMENTATION.md"
    "OPERATORS_SUMMARY.md"
    "PATH_FIX_SUMMARY.md"
    "PRODUCTION_READINESS_STATUS.md"
    "PROJECT_STRUCTURE.md"
    "QA_VALIDATION_COMPLETE.md"
    "REMAINING_ISSUES_FIXED.md"
    "REORGANIZATION_SUMMARY.md"
    "TEST_SUMMARY.md"
    "VALIDATION_REPORT.md"
)

count=0
for file in "${ROOT_ARCHIVE_FILES[@]}"; do
    if [ -f "$file" ]; then
        mv "$file" "docs/archive/root-files/"
        ((count++))
        echo "  Archived: $file"
    fi
done

echo -e "${GREEN}✓ Archived $count root-level files${NC}"

# ============================================================================
# PHASE 3: Archive docs/features/ Implementation Files (30+ files)
# ============================================================================
echo -e "${BLUE}[Phase 3]${NC} Archiving feature implementation files..."

# Files to archive from docs/features/
FEATURE_ARCHIVE_FILES=(
    "ANALYSIS_COMPLETE.md"
    "ARRAY_METHODS_IMPLEMENTATION.md"
    "ARROW_FUNCTIONS_IMPLEMENTATION.md"
    "ASYNC_AWAIT_IMPLEMENTATION.md"
    "COMPLETION_STATUS_FINAL.md"
    "CROSS_PLATFORM_COMPLETE.md"
    "DESTRUCTURING_IMPLEMENTATION.md"
    "ES6_MODULES_IMPLEMENTATION.md"
    "FEATURE_STATUS_2026.md"
    "FINAL_IMPLEMENTATION_COMPLETE.md"
    "FOR_OF_IMPLEMENTATION.md"
    "GENERATOR_IMPLEMENTATION.md"
    "IMPLEMENTATION_COMPLETE.md"
    "IMPLEMENTATION_NOTES.md"
    "IMPLEMENTATION_REPORT.md"
    "IMPLEMENTATION_STATUS_REPORT.md"
    "IMPLEMENTATION_SUMMARY.md"
    "JS_FEATURES_PHASE1_COMPLETE.md"
    "OOP_IMPLEMENTATION_SUMMARY.md"
    "OOP_RUNTIME_IMPLEMENTATION.md"
    "PHASE1_IMPLEMENTATION_GUIDE.md"
    "PROMISES_IMPLEMENTATION.md"
    "PYTHON_PLANNING_COMPLETE.md"
    "SPREAD_REST_IMPLEMENTATION.md"
    "TERNARY_IMPLEMENTATION_COMPLETE.md"
    "TERNARY_OPERATOR_IMPLEMENTATION.md"
    "TUPLE_IMPLEMENTATION_SUMMARY.md"
    "TUPLE_IMPLEMENTATION_VISUAL.md"
    "VAR_LET_CONST_COMPLETE.md"
    "VAR_LET_CONST_IMPLEMENTATION_SUMMARY.md"
)

count=0
for file in "${FEATURE_ARCHIVE_FILES[@]}"; do
    if [ -f "docs/features/$file" ]; then
        mv "docs/features/$file" "docs/archive/features/"
        ((count++))
        echo "  Archived: docs/features/$file"
    fi
done

echo -e "${GREEN}✓ Archived $count feature implementation files${NC}"

# ============================================================================
# PHASE 4: Archive docs/implementation/ Session Summaries (ALL files)
# ============================================================================
echo -e "${BLUE}[Phase 4]${NC} Archiving session summaries..."

if [ -d "docs/implementation" ]; then
    count=$(find docs/implementation -type f -name "*.md" | wc -l)
    mv docs/implementation/*.md docs/archive/implementation/ 2>/dev/null || true
    rmdir docs/implementation 2>/dev/null || true
    echo -e "${GREEN}✓ Archived $count session summary files${NC}"
else
    echo -e "${YELLOW}! docs/implementation/ not found (already cleaned?)${NC}"
fi

# ============================================================================
# PHASE 5: Archive Duplicate Files from docs/
# ============================================================================
echo -e "${BLUE}[Phase 5]${NC} Archiving duplicate files..."

DUPLICATE_FILES=(
    "docs/OOP_SUMMARY.md"
    "docs/OOP_QUICK_START.md"
    "docs/OOP_PARSING.md"
    "docs/OOP_IMPLEMENTATION_PLAN.md"
    "docs/CLI_PHASE1_GETTING_STARTED.md"
    "docs/CLI_BEFORE_AFTER.md"
    "docs/CLI_ENHANCEMENT_PLAN.md"
    "docs/CLI_DOCUMENTATION_INDEX.md"
    "docs/JS_COMPARISON_INDEX.md"
    "docs/FEATURE_COMPARISON_SUMMARY.md"
)

count=0
for file in "${DUPLICATE_FILES[@]}"; do
    if [ -f "$file" ]; then
        mv "$file" "docs/archive/root-files/"
        ((count++))
        echo "  Archived: $file"
    fi
done

echo -e "${GREEN}✓ Archived $count duplicate files${NC}"

# ============================================================================
# PHASE 6: Reorganize User Documentation
# ============================================================================
echo -e "${BLUE}[Phase 6]${NC} Reorganizing user documentation..."

# Move user-facing docs
[ -f "docs/GETTING_STARTED.md" ] && mv "docs/GETTING_STARTED.md" "docs/user/getting-started.md"
[ -f "docs/USAGE_GUIDE.md" ] && mv "docs/USAGE_GUIDE.md" "docs/user/usage-guide.md"
[ -f "docs/COMPILATION_GUIDE.md" ] && mv "docs/COMPILATION_GUIDE.md" "docs/user/compilation-guide.md"
[ -f "docs/DISTRIBUTION.md" ] && mv "docs/DISTRIBUTION.md" "docs/user/distribution.md"
[ -f "docs/COMPLETE_FEATURE_GUIDE.md" ] && mv "docs/COMPLETE_FEATURE_GUIDE.md" "docs/user/complete-feature-guide.md"
[ -f "docs/JS_TO_KLANG_MIGRATION.md" ] && mv "docs/JS_TO_KLANG_MIGRATION.md" "docs/user/migration-from-javascript.md"
[ -f "docs/JS_KLANG_FEATURE_COMPARISON.md" ] && mv "docs/JS_KLANG_FEATURE_COMPARISON.md" "docs/user/javascript-comparison.md"
[ -f "docs/PATH_TROUBLESHOOTING.md" ] && mv "docs/PATH_TROUBLESHOOTING.md" "docs/user/troubleshooting.md"

# Move advanced guides from root to docs/guides/
[ -f "ADVANCED_CLI_FEATURES.md" ] && mv "ADVANCED_CLI_FEATURES.md" "docs/guides/cli-advanced.md"
[ -f "SCRIPTS_GUIDE.md" ] && mv "SCRIPTS_GUIDE.md" "docs/guides/scripts.md"
[ -f "OPERATORS_QUICK_REFERENCE.md" ] && mv "OPERATORS_QUICK_REFERENCE.md" "docs/api/operators/quick-reference.md"

echo -e "${GREEN}✓ User documentation reorganized${NC}"

# ============================================================================
# PHASE 7: Reorganize API Reference Documentation
# ============================================================================
echo -e "${BLUE}[Phase 7]${NC} Reorganizing API reference..."

# OOP documentation
[ -f "docs/OOP_README.md" ] && mv "docs/OOP_README.md" "docs/api/oop/overview.md"
[ -f "docs/OOP_QUICK_REFERENCE.md" ] && mv "docs/OOP_QUICK_REFERENCE.md" "docs/api/oop/quick-reference.md"
[ -f "docs/OOP_LANGUAGE_SPEC.md" ] && mv "docs/OOP_LANGUAGE_SPEC.md" "docs/api/oop/language-spec.md"

# CLI documentation
[ -f "docs/CLI_QUICKSTART.md" ] && mv "docs/CLI_QUICKSTART.md" "docs/api/cli/quickstart.md"
[ -f "docs/CLI_QUICK_REFERENCE.md" ] && mv "docs/CLI_QUICK_REFERENCE.md" "docs/api/cli/reference.md"
[ -f "docs/CLI_IMPLEMENTATION_GUIDE.md" ] && mv "docs/CLI_IMPLEMENTATION_GUIDE.md" "docs/api/cli/implementation-guide.md"
[ -f "docs/CLI_ARCHITECTURE_DIAGRAM.md" ] && mv "docs/CLI_ARCHITECTURE_DIAGRAM.md" "docs/api/cli/architecture.md"
[ -f "docs/CLI_VISUAL_DEMO.md" ] && mv "docs/CLI_VISUAL_DEMO.md" "docs/api/cli/visual-demo.md"
[ -f "docs/CLI_ROADMAP.md" ] && mv "docs/CLI_ROADMAP.md" "docs/roadmaps/cli-roadmap.md" 2>/dev/null || true

# AI documentation
[ -f "docs/AI_NATIVE_GUIDE.md" ] && mv "docs/AI_NATIVE_GUIDE.md" "docs/api/ai/guide.md"
[ -f "docs/AI_QUICK_REFERENCE.md" ] && mv "docs/AI_QUICK_REFERENCE.md" "docs/api/ai/quick-reference.md"
[ -f "docs/AI_MIGRATION_GUIDE.md" ] && mv "docs/AI_MIGRATION_GUIDE.md" "docs/api/ai/migration.md"
[ -f "docs/AI_ARCHITECTURE.md" ] && mv "docs/AI_ARCHITECTURE.md" "docs/api/ai/architecture.md"

# Standard library
[ -f "docs/ARRAY_METHODS.md" ] && mv "docs/ARRAY_METHODS.md" "docs/api/stdlib/arrays.md"
[ -f "docs/ARRAY_METHODS_QUICK_REF.md" ] && mv "docs/ARRAY_METHODS_QUICK_REF.md" "docs/api/stdlib/arrays-quick-ref.md"
[ -f "docs/FILE_IO_API.md" ] && mv "docs/FILE_IO_API.md" "docs/api/stdlib/file-io.md"

# Core language
[ -f "docs/LANGUAGE_REFERENCE.md" ] && mv "docs/LANGUAGE_REFERENCE.md" "docs/api/language-reference.md"
[ -f "docs/VAR_LET_CONST.md" ] && mv "docs/VAR_LET_CONST.md" "docs/api/variables.md"
[ -f "docs/OBJECT_LITERALS.md" ] && mv "docs/OBJECT_LITERALS.md" "docs/api/objects.md"
[ -f "docs/arrow_functions.md" ] && mv "docs/arrow_functions.md" "docs/api/arrow-functions.md"

echo -e "${GREEN}✓ API reference reorganized${NC}"

# ============================================================================
# PHASE 8: Reorganize Developer Documentation
# ============================================================================
echo -e "${BLUE}[Phase 8]${NC} Reorganizing developer documentation..."

# Move architecture docs
[ -f "docs/architecture/ARCHITECTURE.md" ] && mv "docs/architecture/ARCHITECTURE.md" "docs/developer/architecture.md"
[ -d "docs/architecture" ] && rmdir "docs/architecture" 2>/dev/null || true

# Move LLVM backend docs to developer section
[ -f "docs/LLVM_BACKEND.md" ] && mv "docs/LLVM_BACKEND.md" "docs/developer/llvm-backend.md"

# Move PR templates
[ -f "docs/development/PR_ASYNC_AWAIT.md" ] && mv "docs/development/PR_ASYNC_AWAIT.md" "docs/developer/pr-templates/async-await.md"
[ -f "docs/development/PR_GENERATOR_FUNCTIONS.md" ] && mv "docs/development/PR_GENERATOR_FUNCTIONS.md" "docs/developer/pr-templates/generators.md"
[ -f "docs/development/PR_SPREAD_REST.md" ] && mv "docs/development/PR_SPREAD_REST.md" "docs/developer/pr-templates/spread-rest.md"

# Move contributing and release docs
[ -f "docs/development/CONTRIBUTING.md" ] && mv "docs/development/CONTRIBUTING.md" "docs/developer/contributing.md"
[ -f "docs/guides/RELEASE_CHECKLIST.md" ] && mv "docs/guides/RELEASE_CHECKLIST.md" "docs/developer/release-checklist.md"

# Clean up old development directory
[ -d "docs/development" ] && rmdir "docs/development" 2>/dev/null || true

echo -e "${GREEN}✓ Developer documentation reorganized${NC}"

# ============================================================================
# PHASE 9: Reorganize Roadmap Documentation
# ============================================================================
echo -e "${BLUE}[Phase 9]${NC} Reorganizing roadmaps..."

# Move OOP roadmap to roadmaps directory
[ -f "docs/OOP_ROADMAP.md" ] && mv "docs/OOP_ROADMAP.md" "docs/roadmaps/oop-roadmap.md"

# Move complete roadmap from features to roadmaps
[ -f "docs/features/COMPLETE_ROADMAP_TO_100.md" ] && mv "docs/features/COMPLETE_ROADMAP_TO_100.md" "docs/roadmaps/complete-roadmap.md"

echo -e "${GREEN}✓ Roadmaps reorganized${NC}"

# ============================================================================
# PHASE 10: Clean up empty directories
# ============================================================================
echo -e "${BLUE}[Phase 10]${NC} Cleaning up empty directories..."

# Remove features directory if empty (should be empty now)
[ -d "docs/features" ] && rmdir "docs/features" 2>/dev/null && echo "  Removed empty docs/features/"

echo -e "${GREEN}✓ Cleanup complete${NC}"

# ============================================================================
# PHASE 11: Create Archive README
# ============================================================================
echo -e "${BLUE}[Phase 11]${NC} Creating archive documentation..."

cat > docs/archive/README.md << 'EOF'
# KLang Documentation Archive

This directory contains historical documentation that has been archived during the documentation reorganization.

## What's Here

This archive contains approximately **90-100 markdown files** that are no longer needed for active documentation but are preserved for historical reference.

### Categories

#### Root Files (`root-files/`)
Implementation summaries, session notes, and temporary files that were cluttering the project root. These include:
- Implementation completion reports
- Session summaries
- Feature status reports
- Validation reports
- Test summaries

#### Feature Implementation (`features/`)
Detailed implementation notes for specific features. These were valuable during development but are superseded by user-facing documentation:
- Feature-specific implementation guides
- Implementation status reports
- Technical implementation details
- Development notes

#### Session Summaries (`implementation/`)
Session-by-session development summaries that tracked progress during implementation phases:
- Phase completion reports
- Test reports
- Module fix summaries
- Development session notes

## Why Archived?

These files were archived because:

1. **Not User-Facing**: They were internal development notes, not meant for end users
2. **Superseded**: Information has been consolidated into official documentation
3. **Historical**: Represent point-in-time status that's now outdated
4. **Cluttering Navigation**: Made it hard to find relevant documentation
5. **Maintenance Burden**: Keeping them updated was not practical

## Accessing Archived Content

All archived files are still available in version control. If you need historical context:

```bash
# View archived files
ls docs/archive/

# Search archived content
grep -r "search term" docs/archive/
```

## Documentation Structure

For the current, organized documentation structure, see:
- **User Documentation**: `docs/user/`
- **Developer Documentation**: `docs/developer/`
- **API Reference**: `docs/api/`
- **Guides & Tutorials**: `docs/guides/`
- **Project Roadmaps**: `docs/roadmaps/`

---

**Archived**: March 24, 2026  
**Reason**: Documentation reorganization (184 → 90 files)  
**Impact**: No content deleted, just relocated for better organization
EOF

echo -e "${GREEN}✓ Archive README created${NC}"

# ============================================================================
# PHASE 12: Statistics and Summary
# ============================================================================
echo ""
echo -e "${BLUE}╔════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║           Reorganization Complete! ✓              ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════╝${NC}"
echo ""

# Count files
ROOT_MD_COUNT=$(find . -maxdepth 1 -name "*.md" -type f | wc -l)
ARCHIVE_COUNT=$(find docs/archive -name "*.md" -type f | wc -l)
USER_COUNT=$(find docs/user -name "*.md" -type f 2>/dev/null | wc -l)
GUIDES_COUNT=$(find docs/guides -name "*.md" -type f 2>/dev/null | wc -l)
API_COUNT=$(find docs/api -name "*.md" -type f 2>/dev/null | wc -l)
DEVELOPER_COUNT=$(find docs/developer -name "*.md" -type f 2>/dev/null | wc -l)
ROADMAPS_COUNT=$(find docs/roadmaps -name "*.md" -type f 2>/dev/null | wc -l)

echo -e "${GREEN}Summary:${NC}"
echo "  Root markdown files:        $ROOT_MD_COUNT (target: 2-3)"
echo "  Archived files:             $ARCHIVE_COUNT"
echo "  User documentation:         $USER_COUNT"
echo "  Guides & tutorials:         $GUIDES_COUNT"
echo "  API reference:              $API_COUNT"
echo "  Developer docs:             $DEVELOPER_COUNT"
echo "  Roadmaps:                   $ROADMAPS_COUNT"
echo ""
echo -e "${YELLOW}Next Steps:${NC}"
echo "  1. Review the new structure: ls -R docs/"
echo "  2. Update docs/README.md with new navigation"
echo "  3. Check for broken links: grep -r 'docs/' *.md docs/*.md"
echo "  4. Commit changes: git add . && git commit -m 'docs: reorganize documentation structure'"
echo ""
echo -e "${GREEN}Done! 🎉${NC}"
