#!/usr/bin/env bash
# Map an openFrameworks tag, or an apothecary tag, to the apothecary release
# whose libraries that version downloads.
#
# The release candidate uses the release. These all print v12.2.0:
#   0.12.2  0.12.2-RC1  v0.12.2-RC1  v12.2.0  v12.2.0-RC1
# A patch release stays itself: v12.1.1 prints v12.1.1.
# latest and nightly are unchanged.

apothecary_release_tag() {
	local raw="${1#v}"
	local core
	core=$(printf '%s\n' "$raw" | sed -E 's/^([0-9]+\.[0-9]+\.[0-9]+).*/\1/')
	if [[ $core =~ ^0\.([0-9]+)\.([0-9]+)$ ]]; then
		printf 'v%s.%s.0\n' "${BASH_REMATCH[1]}" "${BASH_REMATCH[2]}"
		return 0
	fi
	if [[ $core =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
		printf 'v%s\n' "$core"
		return 0
	fi
	printf '%s\n' "$1"
}

if [[ "${BASH_SOURCE[0]}" == "$0" ]]; then
	apothecary_release_tag "$1"
fi
