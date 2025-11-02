#!/usr/bin/env bash
set -euo pipefail

# Project directories
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${ROOT_DIR}/build"
BENCH="${BUILD_DIR}/bin/lab1"
FLAMEGRAPH_DIR="${BUILD_DIR}/FlameGraph"
RESULTS_DIR="${ROOT_DIR}/results"
PLOTS_DIR="${ROOT_DIR}/plots"

# Configure project
cmake -S "${ROOT_DIR}" -B "${BUILD_DIR}"
cmake --build "${BUILD_DIR}" -j"$(nproc)"
mkdir -p "${RESULTS_DIR}" "${PLOTS_DIR}"

# Run benchmark
OPERATIONS=("Insert" "Find" "Erase")

for op in "${OPERATIONS[@]}"; do
    out="${RESULTS_DIR}/bench_${op,,}.json"

    "${BENCH}" --benchmark_filter="${op}" \
               --benchmark_format=json \
               --benchmark_out="${out}"

    # Use flamegraph
    if command -v perf >/dev/null 2>&1 && [ -d "${FLAMEGRAPH_DIR}" ]; then
        perf record -F 99 -g -- "${BENCH}" --benchmark_filter="${op}"
        perf script | "${FLAMEGRAPH_DIR}/stackcollapse-perf.pl" | \
          "${FLAMEGRAPH_DIR}/flamegraph.pl" --minwidth 0.5 > "${PLOTS_DIR}/flame_${op,,}.svg"
    fi
done

# Draw plots
if command -v python3 >/dev/null 2>&1; then
    python3 "${ROOT_DIR}/draw_plots.py" --results "${RESULTS_DIR}" --out "${PLOTS_DIR}"
fi
