#!/usr/bin/env bash
echo "Using ccache"
if [ "$TARGET" == "osx" ] || [ "$TARGET" == "ios" ] || [ "$TARGET" == "tvos" ]; then
	# CCACHE_PATH="$(dirname $(which ccache gcc | sed -n 2p))"
	# export PATH="$CCACHE_PATH:$PATH"

    CCACHE=$(which ccache)
    CLANGXX=$(xcodebuild -find clang++)
    CLANG=$(xcodebuild -find clang)
    cat >$PWD/clang++ <<< EOF
         $CCACHE $CLANGXX
EOF
    cat >$PWD/clang <<< EOF2
        $CCACHE $CLANG
EOF2

    chmod 755 clang++
    chmod 755 clang

    export CXX="$PWD/clang++"
    export CC="$PWD/clang"
else
	export PATH="/usr/lib/ccache:$PATH"
fi
