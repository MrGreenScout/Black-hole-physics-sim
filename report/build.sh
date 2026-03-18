#!/usr/bin/env bash
set -euo pipefail

# --- Configuration ---
FILENAME="${1:-}"
BUILDDIR="build"

# --- Validate input ---
if [[ -z "$FILENAME" ]]; then
  echo "Usage: $0 <filename_without_extension>"
  exit 1
fi

if [[ ! -f "${FILENAME}.md" ]]; then
  echo "Error: '${FILENAME}.md' not found."
  exit 1
fi

# --- Build ---
mkdir -p "$BUILDDIR"

pandoc "${FILENAME}.md" \
  --filter pandoc-citeproc \
  --from=markdown+tex_math_single_backslash+tex_math_dollars+raw_tex \
  --to=latex \
  --output="${BUILDDIR}/${FILENAME}.pdf" \
  --pdf-engine=xelatex

echo "Done: ${BUILDDIR}/${FILENAME}.pdf"