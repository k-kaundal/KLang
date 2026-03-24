#!/bin/bash
# KLang Repository Cleanup Script

set -e

echo "🧹 KLang Repository Cleanup"
echo "=========================="
echo ""

# Create archive directories
mkdir -p docs/archive/session_notes
mkdir -p docs/archive/implementation  
mkdir -p docs/archive/old_tests
mkdir -p docs/archive/temp_files

# Move session notes and implementation summaries from root
echo "📦 Archiving session notes and summaries..."
mv *SUMMARY*.md docs/archive/session_notes/ 2>/dev/null || true
mv *SESSION*.md docs/archive/session_notes/ 2>/dev/null || true
mv *FIX*.md docs/archive/session_notes/ 2>/dev/null || true
mv *COMPLETE*.md docs/archive/session_notes/ 2>/dev/null || true
mv *STATUS*.md docs/archive/session_notes/ 2>/dev/null || true
mv *REPORT*.md docs/archive/session_notes/ 2>/dev/null || true
mv *IMPLEMENTATION*.md docs/archive/implementation/ 2>/dev/null || true
mv *MISSION*.md docs/archive/implementation/ 2>/dev/null || true
mv *ANNOUNCEMENT*.md docs/archive/implementation/ 2>/dev/null || true

# Move test files from root
echo "🧪 Archiving test files from root..."
mv test_*.k docs/archive/old_tests/ 2>/dev/null || true
mv test_*.kl docs/archive/old_tests/ 2>/dev/null || true
mv demo_*.k docs/archive/old_tests/ 2>/dev/null || true

# Move duplicate/outdated docs from root
echo "📚 Archiving duplicate documentation..."
mv OPERATORS_*.md docs/archive/old_readmes/ 2>/dev/null || true
mv ADVANCED_*.md docs/archive/old_readmes/ 2>/dev/null || true
mv ALL_*.md docs/archive/old_readmes/ 2>/dev/null || true
mv DOCUMENTATION_*.md docs/archive/old_readmes/ 2>/dev/null || true
mv PROJECT_STRUCTURE.md docs/archive/old_readmes/ 2>/dev/null || true
mv REORGANIZATION_*.md docs/archive/old_readmes/ 2>/dev/null || true
mv SCRIPTS_GUIDE.md docs/archive/old_readmes/ 2>/dev/null || true
mv PATH_*.md docs/archive/old_readmes/ 2>/dev/null || true
mv REMAINING_*.md docs/archive/old_readmes/ 2>/dev/null || true
mv PRODUCTION_*.md docs/archive/old_readmes/ 2>/dev/null || true
mv MISSING_*.md docs/archive/old_readmes/ 2>/dev/null || true
mv VALIDATION_*.md docs/archive/old_readmes/ 2>/dev/null || true
mv AI_FEATURES_INDEX.md docs/archive/old_readmes/ 2>/dev/null || true

# Move docs/implementation to archive
echo "📁 Archiving docs/implementation..."
if [ -d "docs/implementation" ]; then
    mv docs/implementation/* docs/archive/implementation/ 2>/dev/null || true
    rmdir docs/implementation 2>/dev/null || true
fi

# Move docs/features to archive (keeping key files)
echo "📁 Archiving docs/features..."
if [ -d "docs/features" ]; then
    # Keep main feature guide
    cp docs/features/COMPLETE_FEATURE_GUIDE.md docs/ 2>/dev/null || true
    # Archive the rest
    mkdir -p docs/archive/features
    mv docs/features/* docs/archive/features/ 2>/dev/null || true
    rmdir docs/features 2>/dev/null || true
fi

# Archive old roadmap docs
echo "🗺️  Consolidating roadmaps..."
if [ -d "docs/roadmaps" ]; then
    mkdir -p docs/archive/roadmaps
    # Keep executive summaries
    cp docs/roadmaps/EXECUTIVE_SUMMARY.md docs/roadmaps/ 2>/dev/null || true
    # Archive detailed plans
    mv docs/roadmaps/*PLANNING*.md docs/archive/roadmaps/ 2>/dev/null || true
    mv docs/roadmaps/*INDEX*.md docs/archive/roadmaps/ 2>/dev/null || true
fi

# Clean up temp test binaries
echo "🗑️  Removing test binaries..."
rm -f test_llvm_* 2>/dev/null || true
rm -f tests/test_for_c_style 2>/dev/null || true

echo ""
echo "✅ Cleanup complete!"
echo ""
echo "Summary:"
echo "  - Session notes archived"
echo "  - Test files archived"
echo "  - Duplicate docs archived"
echo "  - Implementation notes archived"
echo ""
echo "Root directory is now clean!"
