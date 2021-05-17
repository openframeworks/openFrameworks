#!/usr/bin/env bash
#echo "Using ccache"
#if [ "$TARGET" == "osx" ] || [ "$TARGET" == "ios" ] || [ "$TARGET" == "tvos" ]; then
#	# CCACHE_PATH="$(dirname $(which ccache gcc | sed -n 2p))"
#	# export PATH="$CCACHE_PATH:$PATH"
#
#    CCACHE=$(which ccache)
#    CLANGXX=$(xcodebuild -find clang++)
#    CLANG=$(xcodebuild -find clang)
#    cat << EOF > $PWD/clang++.sh
#         $CCACHE $CLANGXX "\$@"
#EOF
#    cat << EOF2 > $PWD/clang.sh
#        $CCACHE $CLANG "\$@"
#EOF2
#
#    chmod 755 clang++.sh
#    chmod 755 clang.sh
#
#    export CXX="$PWD/clang++.sh"
#    export CC="$PWD/clang.sh"
#elif [ "$BUILDER" == "MSYS2" ]; then
#    echo "detected msys setting ccache as env var"
#    USE_CCACHE="USE_CCACHE=1"
#else
#	export PATH="/usr/lib/ccache:$PATH"
#fi
