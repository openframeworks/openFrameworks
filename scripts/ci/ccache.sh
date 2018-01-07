#!/usr/bin/env bash

if [ "$USE_CCACHE" = true ] ; then
        echo "Using ccache"
        if [ "$TARGET" == "osx" ] || [ "$TARGET" == "ios" ] || [ "$TARGET" == "tvos" ]; then
		CCACHE_PATH="$(dirname $(which ccache gcc | sed -n 2p))"
		export PATH="$CCACHE_PATH:$PATH"         
	else
		export PATH="/usr/lib/ccache:$PATH"
	fi
fi
