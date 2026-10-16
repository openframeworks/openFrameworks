#!/usr/bin/env bash
# Platform-clean removes only lib/msys2; shared include/bin need --full-clean.
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

../dev/download_libs.sh -p msys2 -b "$@"
result=$?

# Some msys2 lib packages (clangarm64) ship static libraries without an archive
# symbol index, so the linker can't find anything in them ("undefined symbol").
# llvm-ranlib (in the clang environments) adds the index; it's a no-op for
# archives that already have one.
if [ $result -eq 0 ] && command -v llvm-ranlib >/dev/null 2>&1; then
	OF_ROOT="$( cd "$SCRIPT_DIR/../.." && pwd )"
	find "$OF_ROOT/libs" "$OF_ROOT/addons" -path "*/lib/msys2/*" -name "*.a" -print0 2>/dev/null |
		xargs -0 -r -n 32 llvm-ranlib 2>/dev/null
fi
exit $result
