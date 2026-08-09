#!/usr/bin/env bash
#
# Foundation memory & linking audit (task #44).
#
# 1. Runs the foundation smoke test under Valgrind and fails on any
#    definite or indirect leak. ncurses intentionally keeps allocations
#    "still reachable" after endwin(), so those are not treated as errors.
# 2. Runs ldd over the produced binaries and fails if any dynamic
#    dependency is unresolved, or if the vendored libraries (curl, cJSON)
#    leaked into the dynamic dependency list instead of being statically
#    linked.
#
# Usage: tests/features/foundation_audit.sh [build-dir]
#   build-dir defaults to build/debug.

set -euo pipefail

BUILD_DIR="${1:-build/debug}"
SMOKE_TEST="$BUILD_DIR/test_foundation"
APP_BINARY="$BUILD_DIR/cels_viewer"

fail() {
    echo "AUDIT FAIL: $1" >&2
    exit 1
}

for binary in "$SMOKE_TEST" "$APP_BINARY"; do
    [ -x "$binary" ] || fail "missing binary '$binary' — build first (cmake --preset debug && cmake --build --preset debug)"
done

echo "== Memory audit (Valgrind) =="
command -v valgrind >/dev/null 2>&1 || fail "valgrind is not installed"
valgrind \
    --leak-check=full \
    --show-leak-kinds=definite,indirect \
    --errors-for-leak-kinds=definite,indirect \
    --error-exitcode=42 \
    "$SMOKE_TEST" \
    || fail "valgrind reported errors or leaks in $SMOKE_TEST"
echo "OK: zero definite/indirect leaks across the app lifecycle"

echo
echo "== Linking audit (ldd) =="
for binary in "$APP_BINARY" "$SMOKE_TEST"; do
    deps="$(ldd "$binary")"

    if echo "$deps" | grep -q "not found"; then
        fail "$binary has unresolved dynamic dependencies:
$deps"
    fi

    # Vendored libraries must be statically embedded, not dynamic deps.
    if echo "$deps" | grep -Eq "libcurl|libcjson"; then
        fail "$binary dynamically links a vendored library:
$deps"
    fi

    if ! echo "$deps" | grep -Eq "libncurses|libcurses"; then
        fail "$binary does not link ncurses:
$deps"
    fi

    echo "OK: $binary — all dynamic dependencies resolved, vendored libs static, ncurses linked"
done

echo
echo "Foundation audit passed."
