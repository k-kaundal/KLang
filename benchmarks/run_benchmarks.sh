#!/bin/bash
# Comprehensive benchmark runner for KLang

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m' # No Color

# Directories
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
BENCHMARK_DIR="$SCRIPT_DIR"
REPORTS_DIR="$BENCHMARK_DIR/reports"
LANGUAGE_DIR="$BENCHMARK_DIR/language"
CROSS_LANG_DIR="$BENCHMARK_DIR/cross-language"

# Create reports directory
mkdir -p "$REPORTS_DIR"

echo -e "${CYAN}${BOLD}╔═══════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}${BOLD}║           KLANG BENCHMARK SUITE                      ║${NC}"
echo -e "${CYAN}${BOLD}╚═══════════════════════════════════════════════════════╝${NC}"
echo ""

# Check if klang is built
if [ ! -f "$PROJECT_ROOT/klang" ]; then
    echo -e "${YELLOW}Building KLang...${NC}"
    cd "$PROJECT_ROOT"
    make clean
    make
    echo -e "${GREEN}✓ Build complete${NC}"
    echo ""
fi

# Compile benchmark utilities
echo -e "${YELLOW}Compiling benchmarks...${NC}"
cd "$BENCHMARK_DIR"

# Compile language benchmarks
gcc -O3 -o "$LANGUAGE_DIR/arithmetic_bench" "$LANGUAGE_DIR/arithmetic_bench.c" -lm
gcc -O3 -o "$LANGUAGE_DIR/loop_bench" "$LANGUAGE_DIR/loop_bench.c" -lm
gcc -O3 -o "$LANGUAGE_DIR/function_bench" "$LANGUAGE_DIR/function_bench.c" -lm
gcc -O3 -o "$LANGUAGE_DIR/string_bench" "$LANGUAGE_DIR/string_bench.c" -lm

# Compile memory benchmarks
MEMORY_DIR="$BENCHMARK_DIR/memory"
gcc -O3 -o "$MEMORY_DIR/memory_bench" "$MEMORY_DIR/memory_bench.c" -lm

echo -e "${GREEN}✓ Compilation complete${NC}"
echo ""

# Run language benchmarks
echo -e "${BOLD}${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BOLD}${BLUE}  LANGUAGE FEATURE BENCHMARKS${NC}"
echo -e "${BOLD}${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

cd "$LANGUAGE_DIR"

echo -e "${CYAN}Running arithmetic benchmarks...${NC}"
./arithmetic_bench

echo -e "${CYAN}Running loop benchmarks...${NC}"
./loop_bench

echo -e "${CYAN}Running function benchmarks...${NC}"
./function_bench

echo -e "${CYAN}Running string benchmarks...${NC}"
./string_bench

# Run memory benchmarks
echo ""
echo -e "${BOLD}${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BOLD}${BLUE}  MEMORY BENCHMARKS${NC}"
echo -e "${BOLD}${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

cd "$MEMORY_DIR"
echo -e "${CYAN}Running memory benchmarks...${NC}"
./memory_bench

cd "$LANGUAGE_DIR"

# Run cross-language benchmarks if available
if [ -x "$CROSS_LANG_DIR/fibonacci_compare.sh" ]; then
    echo ""
    echo -e "${BOLD}${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${BOLD}${BLUE}  CROSS-LANGUAGE BENCHMARKS${NC}"
    echo -e "${BOLD}${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo ""
    
    cd "$CROSS_LANG_DIR"
    ./fibonacci_compare.sh
fi

# Generate summary report
echo ""
echo -e "${BOLD}${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BOLD}${BLUE}  BENCHMARK SUMMARY${NC}"
echo -e "${BOLD}${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

# Count JSON reports
REPORT_COUNT=$(find "$REPORTS_DIR" -name "*.json" 2>/dev/null | wc -l)

if [ "$REPORT_COUNT" -gt 0 ]; then
    echo -e "${GREEN}Generated $REPORT_COUNT benchmark reports${NC}"
    echo -e "${CYAN}Reports saved to: $REPORTS_DIR${NC}"
    echo ""
    
    # List all reports
    echo -e "${YELLOW}Available reports:${NC}"
    ls -1 "$REPORTS_DIR"/*.json 2>/dev/null | while read report; do
        echo "  - $(basename "$report")"
    done
else
    echo -e "${YELLOW}No JSON reports generated${NC}"
fi

echo ""
echo -e "${GREEN}${BOLD}✓ All benchmarks completed successfully!${NC}"
echo ""

# Optionally generate markdown report
if command -v python3 &> /dev/null; then
    echo -e "${CYAN}Generating markdown report...${NC}"
    
    cat > "$REPORTS_DIR/generate_report.py" << 'PYTHON_SCRIPT'
import json
import os
from pathlib import Path

def load_json_reports(reports_dir):
    """Load all JSON benchmark reports"""
    reports = []
    for json_file in Path(reports_dir).glob("*.json"):
        try:
            with open(json_file, 'r') as f:
                data = json.load(f)
                reports.append(data)
        except Exception as e:
            print(f"Error loading {json_file}: {e}")
    return reports

def format_number(num):
    """Format number with thousands separator"""
    return f"{num:,.0f}"

def generate_markdown(reports):
    """Generate markdown report"""
    md = []
    
    md.append("# KLang Benchmark Report")
    md.append("")
    md.append(f"Generated: {os.popen('date').read().strip()}")
    md.append("")
    
    # Group by category
    categories = {}
    for report in reports:
        name = report['name']
        category = name.split()[0] if ' ' in name else 'Other'
        if category not in categories:
            categories[category] = []
        categories[category].append(report)
    
    # Generate tables for each category
    for category, cat_reports in sorted(categories.items()):
        md.append(f"## {category} Benchmarks")
        md.append("")
        md.append("| Benchmark | Iterations | Time (ms) | Throughput (Mops/sec) | Mean (ns) | P95 (ns) | P99 (ns) |")
        md.append("|-----------|------------|-----------|----------------------|-----------|----------|----------|")
        
        for report in sorted(cat_reports, key=lambda x: x['name']):
            name = report['name'].replace(f"{category} ", "")
            iterations = format_number(report['iterations'])
            time_ms = f"{report['total_time_ms']:.2f}"
            throughput = f"{report['throughput_mops']:.2f}"
            mean = format_number(report['timing']['mean_ns'])
            p95 = format_number(report['timing']['p95_ns'])
            p99 = format_number(report['timing']['p99_ns'])
            
            md.append(f"| {name} | {iterations} | {time_ms} | {throughput} | {mean} | {p95} | {p99} |")
        
        md.append("")
    
    return "\n".join(md)

if __name__ == "__main__":
    reports_dir = os.path.dirname(os.path.abspath(__file__))
    reports = load_json_reports(reports_dir)
    
    if reports:
        markdown = generate_markdown(reports)
        output_file = os.path.join(reports_dir, "benchmark_summary.md")
        with open(output_file, 'w') as f:
            f.write(markdown)
        print(f"✓ Report generated: {output_file}")
    else:
        print("No reports found")
PYTHON_SCRIPT
    
    python3 "$REPORTS_DIR/generate_report.py"
    echo ""
fi

echo -e "${CYAN}${BOLD}Benchmark run complete!${NC}"
echo ""
