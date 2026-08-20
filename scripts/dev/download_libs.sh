#!/usr/bin/env bash
set -e
VER=latest
PLATFORM=""
ARCH=""
OVERWRITE=1
FULL_CLEAN=0
# SHA: VERIFY_SHA=1 (default) check GitHub release asset digests when present
#      VERIFY_SHA=0 (--skip-sha) never verify
#      REQUIRE_SHA=1 (--require-sha) fail if digest missing or tools unavailable
VERIFY_SHA=1
REQUIRE_SHA=0
LEGACY=0
SILENT_ARGS=""
NO_SSL=""
BLEEDING_EDGE=0
DL_VERSION=2.8.2
GCC_VERSION=0
TAG=""
REPO="latest"
SHA_VERIFIED=0
SHA_SKIPPED=0
SHA_FAILED=0
SHA_STATUS="unchecked"

printHelp(){
cat << EOF
    Usage: download_libs.sh [OPTIONS]

    Options:

    -v, --version VERSION       OF version to download the libraries for. Defaults to master
    -p, --platform PLATFORM     Platorm among: android, emscritpen, ios, linux, linux64, linuxarmv6l, linuxarmv7l, msys2, osx, tvos, vs
                                If not specified tries to autodetect the platform.
    -a, --arch ARCH             Architecture:
                                    vs: 64
                                    msys2: 64
                                    android: armv7, arm64, and x86 (if not specified will download all)
                                    linux: 64, arm64, aarch64, armv6l or armv7l
    -n, --no-overwrite          Pure merge: do not delete anything before extract.
                                Default (without -n) only removes libs/<lib>/lib/\$PLATFORM so other
                                platforms (android + ios + macos + emscripten …) can coexist.
    --full-clean                Also remove shared include/ (and bin/ for vs|msys2) for libs being
                                installed. Use for a hard reset of one platform install; not needed
                                for multi-platform side-by-side installs.
    --skip-sha                  Do not verify package SHA-256 against GitHub release digests
    --require-sha               Fail if a package has no digest or local hash tools are missing
    -s, --silent                Silent download progress
    -h, --help                  Shows this message
    -k, --no-ssl                Allow no SSL validation
    -t, --tag                   tag release for libraries
    -g, --gcc-version           GCC Version
EOF
}

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/downloader.sh"

# Resolve apothecary release tag used for package URLs
resolveRepo(){
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        REPO="latest"
    else
        REPO="nightly"
    fi
    #FIXME: remove later, now forcing "latest"
    REPO="latest"
    if [[ $TAG != "" ]] ; then
        REPO="$TAG"
    fi
}

# Shared SHA-256 helpers (GitHub digests + sidecars)
. "$SCRIPT_DIR/sha_verify.sh"

# Apothecary-specific: map REPO tag → digest map, then verify packages in cwd
fetchReleaseDigests(){
    local tag="$1"
    local out="$2"
    fetchGithubReleaseDigests "openframeworks/apothecary" "$tag" "$out"
}

verifyPackageDigests(){
    local pkgs="$1"
    local map
    map=$(mktemp 2>/dev/null || mktemp -t ofdigestmap)
    if ! fetchReleaseDigests "$REPO" "$map"; then
        : > "$map"
    fi
    verifyPackagesAgainstMap "$pkgs" "$map" "GitHub apothecary release digests"
    local rc=$?
    rm -f "$map"
    return $rc
}

download(){
    echo ' -----'
    #echo " Downloading $1"
    # downloader ci.openframeworks.cc/libs/$1 $SILENT_ARGS

    COMMAND=" "
    resolveRepo

    for PKG in $1; do
        COMMAND+="https://github.com/openframeworks/apothecary/releases/download/$REPO/$PKG "
    done
    # echo $COMMAND;
    downloader $COMMAND $SILENT_ARGS $NO_SSL
}

is_raspberry_pi(){
    local model=""
    if [ -f /proc/device-tree/model ]; then
        model="$(tr -d '\0' </proc/device-tree/model 2>/dev/null || true)"
    elif [ -f /sys/firmware/devicetree/base/model ]; then
        model="$(tr -d '\0' </sys/firmware/devicetree/base/model 2>/dev/null || true)"
    fi
    if echo "$model" | grep -qi raspberry; then
        return 0
    fi
    if [ -f /etc/os-release ] && grep -qiE '^(ID=raspbian|PRETTY_NAME=.*Raspberry Pi)' /etc/os-release; then
        return 0
    fi
    return 1
}

# Canonical apothecary / linux2026 makefile paths: lib/linux/<arch>
linux_canonical_lib_subpath(){
    case "$1" in
        x86_64|64|64gcc6|64_gcc6) echo "linux/64" ;;
        arm64) echo "linux/arm64" ;;
        aarch64) echo "linux/aarch64" ;;
        armv6l) echo "linux/armv6l" ;;
        armv7l) echo "linux/armv7l" ;;
        jetson) echo "linux/jetson" ;;
        *) echo "" ;;
    esac
}

linux_legacy_lib_subpath(){
    case "$1" in
        x86_64|64|64gcc6|64_gcc6) echo "linux64" ;;
        arm64) echo "linuxarm64" ;;
        aarch64) echo "linuxaarch64" ;;
        armv6l) echo "linuxarmv6l" ;;
        armv7l) echo "linuxarmv7l" ;;
        *) echo "" ;;
    esac
}

# Keep structured apothecary paths. If only a pre-0.13 flat folder exists,
# promote it so the linux2026 makefile (lib/linux/<arch>) can find it.
normalize_linux_lib_paths(){
    if [ "$PLATFORM" != "linux" ]; then
        return
    fi

    local source_path
    local destination_path
    local destination_subpath
    local legacy_subpath
    local lib_root
    local arch
    local promoted=0

    for arch in 64 arm64 aarch64 armv6l armv7l; do
        destination_subpath=$(linux_canonical_lib_subpath "$arch")
        legacy_subpath=$(linux_legacy_lib_subpath "$arch")
        [ -n "$legacy_subpath" ] || continue
        while IFS= read -r source_path; do
            lib_root="${source_path%/lib/$legacy_subpath}"
            destination_path="${lib_root}/lib/${destination_subpath}"
            if [ -e "$destination_path" ]; then
                echo " Keeping structured [$destination_path] (legacy [$source_path] also present)"
                continue
            fi
            echo " Promoting Linux libraries: [$source_path] -> [$destination_path]"
            mkdir -p "$(dirname "$destination_path")"
            mv "$source_path" "$destination_path"
            promoted=$((promoted + 1))
        done < <(find . -type d -path "*/lib/${legacy_subpath}" -print)
    done

    if [ $promoted -gt 0 ]; then
        echo " Promoted $promoted Linux library path(s) to linux2026 structured paths"
    fi
}

# Latest apothecary msys2 archives ship both a GNU and an MSVC symbol index.
# MinGW ld then reports "file format not recognized; treating as linker script"
# and lld reports "unknown file type". Rebuild with GNU ar/ranlib.
reindex_msys2_static_libs(){
    if [ "$PLATFORM" != "msys2" ]; then
        return
    fi

    local ar_bin ranlib_bin archive tmpdir
    ar_bin=$(command -v ar || true)
    ranlib_bin=$(command -v ranlib || true)
    if [ -z "$ar_bin" ]; then
        echo " No ar on PATH; skip MSYS2 static-lib reindex"
        return
    fi

    echo " Re-indexing MSYS2 static libraries with $(basename "$ar_bin") (GNU ld / lld)"
    while IFS= read -r archive; do
        [ -n "$archive" ] || continue
        case "$archive" in
            *.dll.a) continue ;;
        esac
        local archive_abs tmpdir
        archive_abs="$(cd "$(dirname "$archive")" && pwd)/$(basename "$archive")"
        tmpdir=$(mktemp -d 2>/dev/null || mktemp -d -t ofar)
        if (
            cd "$tmpdir" || exit 1
            "$ar_bin" x "$archive_abs" >/dev/null 2>&1 || exit 1
            objs=""
            for f in *; do
                case "$f" in
                    *.o|*.obj|*.O) objs="$objs $f" ;;
                esac
            done
            if [ -z "$objs" ]; then
                echo "  skip [$archive] (no object members)"
                exit 0
            fi
            # shellcheck disable=SC2086
            "$ar_bin" rcs "$archive_abs" $objs || exit 1
            if [ -n "$ranlib_bin" ]; then
                "$ranlib_bin" "$archive_abs" >/dev/null 2>&1 || true
            fi
            echo "  reindexed [$archive]"
        ); then
            :
        else
            echo "  warning: could not reindex [$archive] (linker may reject it)"
        fi
        rm -rf "$tmpdir"
    done < <(find . -type f -name '*.a' ! -name '*.dll.a' -path '*/lib/msys2/*' -print 2>/dev/null)
}

# trap any script errors and exit
trap 'trapError ${LINENO}' ERR
trap "trapError" SIGINT SIGTERM

trapError() {
    local parent_lineno="$1"
    if [[ "$#" = "3" ]] ; then
        local message="$2"
        local code="${3:-1}"
        echo "Error on or near line ${parent_lineno}: ${message}; exiting with status ${code}"
    else
        local code="${2:-1}"
        echo "Error on or near line ${parent_lineno}; exiting with status ${code}"
    fi

    if [ -e openFrameworksLibs* ]; then
        echo "removing packages"
    	rm openFrameworksLibs*
    fi
    exit "${code}"
}


while [[ $# -gt 0 ]]; do
    key="$1"
    case $key in
        -v|--version)
        VER="$2"
        shift # past argument
        ;;
        -p|--platform)
        PLATFORM="$2"
        shift # past argument
        ;;
        -a|--arch)
        ARCH="$2"
        shift # past argument
        ;;
        -n|--no-overwrite)
        OVERWRITE=0
        ;;
        --full-clean)
        FULL_CLEAN=1
        OVERWRITE=1
        ;;
        --skip-sha)
        VERIFY_SHA=0
        ;;
        --require-sha)
        REQUIRE_SHA=1
        VERIFY_SHA=1
        ;;
        -b|--bleeding-edge)
        BLEEDING_EDGE=1
        ;;
        -s|--silent)
        SILENT_ARGS=1
        ;;
        -k|--no-ssl)
        NO_SSL=1
        ;;
        -m|--msystem)
        MSYSTEM="$2"
        shift # past argument
        ;;
        -g|--gcc-version)
        GCC_VERSION="$2"
        shift # past argument
        ;;
        -t|--tag)
        TAG="$2"
        shift # past argument
        ;;
        -l|--legacy)
        LEGACY=1
        ;;
        -vs|--vs2026)
        VS_2026=1
        ;;
        -h|--help)
        printHelp
        exit 0
        ;;
        *)
        echo "Error: invalid argument: $key"
        printHelp
        # exit 1
        ;;
    esac
    shift # past argument or value
done

if [[ "$TARGET" != "" ]] && [[ "$PLATFORM" == "" ]]; then
    PLATFORM=$TARGET
fi

if [ "$PLATFORM" == "" ]; then
    OS=$(uname)
    if [ "$OS" == "Linux" ]; then
        PLATFORM="linux"
    elif [ "$OS" == "Darwin" ]; then
        PLATFORM="osx"
    elif [ "${OS:0:5}" == "MINGW" ]; then
        PLATFORM="msys2"
    else
        # otherwise we are on windows and will download 32bit msys2
        PLATFORM="msys2"
    fi
fi

# Visual Studio package arch for this machine: 64 | arm64
# (arm64 host also pulls arm64ec — see VS package selection below)
detectHostVsArch(){
    local m pa
    # Windows env (Git Bash / MSYS often set these)
    pa="${PROCESSOR_ARCHITECTURE:-}"
    [[ -n "${PROCESSOR_ARCHITEW6432:-}" ]] && pa="${PROCESSOR_ARCHITEW6432}"
    case "${MSYSTEM:-}" in
        CLANGARM64|clangarm64) echo "arm64"; return 0 ;;
    esac
    case "$(echo "$pa" | tr '[:lower:]' '[:upper:]')" in
        ARM64) echo "arm64"; return 0 ;;
        AMD64|X86) echo "64"; return 0 ;;
    esac
    m=$(uname -m 2>/dev/null || echo "")
    case "$m" in
        aarch64|arm64|ARM64) echo "arm64"; return 0 ;;
        x86_64|amd64|i686|i386|x86) echo "64"; return 0 ;;
    esac
    # uname -s might be MINGW64_NT-… on x64
    case "$(uname -s 2>/dev/null)" in
        *ARM64*|*arm64*) echo "arm64"; return 0 ;;
    esac
    echo "64"
}

if [ "$ARCH" == "" ]; then
    if [ "$PLATFORM" == "linux" ]; then
        ARCH=$(uname -m)
        if is_raspberry_pi; then
            case "$ARCH" in
                aarch64|arm64) ARCH=aarch64 ;;
                armv7l) ARCH=armv7l ;;
                armv6l) ARCH=armv6l ;;
            esac
            echo " Raspberry Pi detected → ARCH=${ARCH}"
        elif [ "$ARCH" == "x86_64" ]; then
            ARCH=64
        elif [ "$ARCH" == "arm64" ] || [ "$ARCH" == "aarch64" ]; then
            ARCH=arm64
        elif [ "$ARCH" == "i686" ] || [ "$ARCH" == "i386" ]; then
            echo "32bit linux is not officially supported anymore but compiling the libraries using the build script in apothecary/scripts should compile all the dependencies without problem"
            exit 1
        fi
    elif [ "$PLATFORM" == "msys2" ]; then
        if [ "$MSYSTEM" == "MINGW64" ] || [ "$MSYSTEM" == "mingw64" ]; then
            ARCH=mingw64
        elif [ "$MSYSTEM" == "CLANGARM64" ] || [ "$MSYSTEM" == "clangarm64" ]; then
            ARCH=clangarm64
        elif [ "$MSYSTEM" == "UCRT64" ]; then
            ARCH=ucrt64
        elif [ "$MSYSTEM" == "CLANG64" ]; then
            ARCH=clang64
        else
            ARCH=clang64
        fi
    elif [ "$PLATFORM" == "vs" ]; then
        # Default: host arch only (not every VS package). Use -a all for 64+arm64+arm64ec.
        ARCH=$(detectHostVsArch)
        echo " VS host arch → ${ARCH} (pass -a all for every VS arch, or -a 64|arm64|arm64ec)"
    fi
    if [ "$PLATFORM" == "osx" ]; then
        ARCH=x86_64
    fi
fi

# Normalize aliases for VS package names
if [ "$PLATFORM" == "vs" ]; then
    case "$ARCH" in
        x64|x86_64|amd64|win64) ARCH=64 ;;
        aarch64|ARM64|clangarm64) ARCH=arm64 ;;
        all|multi|host-all) ARCH=all ;;
        host) ARCH=$(detectHostVsArch) ;;
    esac
fi

if [ "$PLATFORM" == "linux" ]; then
	# Official apothecary Linux archives are the GCC 10 baseline only:
	#   openFrameworksLibs_<tag>_linux_<ARCH>_gcc10.tar.bz2
	# A newer host compiler can still consume them (_GLIBCXX_USE_CXX11_ABI=1).
	OFFICIAL_LINUX_GCC=10
	if [ "$GCC_VERSION" == 0 ]; then
		GCC_VERSION=$OFFICIAL_LINUX_GCC
	elif [ "$GCC_VERSION" != "$OFFICIAL_LINUX_GCC" ]; then
		echo " Official Linux archives are GCC ${OFFICIAL_LINUX_GCC} (requested gcc${GCC_VERSION} is not published). Using gcc${OFFICIAL_LINUX_GCC}."
		GCC_VERSION=$OFFICIAL_LINUX_GCC
	fi
	echo "GCC_VERSION: [$GCC_VERSION]"
	GCC_VERSION="gcc${GCC_VERSION}"
	case "$ARCH" in
		64|x86_64|arm64|aarch64|armv6l|armv7l)
			OPT="_${GCC_VERSION}"
			;;
		jetson|armv8l)
			OPT=""
			;;
		*)
			OPT="_${GCC_VERSION}"
			;;
	esac
fi



SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

if [[ $BLEEDING_EDGE = 1 ]] ; then
    VER=latest
fi

if [[ $TAG != "" ]] && [[ $TAG != "nightly" ]] ; then
    VER="$TAG"
fi

echo " openFrameworks download_libs.sh v$DL_VERSION args=$@"

if [ "$PLATFORM" == "emscripten" ]; then
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        if [[ $ARCH = "" ]] ; then
            ARCH="32"
        fi
        if [[ $ARCH = "64" ]] ; then
            ARCH="_64"
        fi
    fi
fi

if [ "$PLATFORM" == "msys2" ]; then
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}.zip"
    else    
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}.zip"
    fi
elif [ "$PLATFORM" == "vs" ]; then
    if [[ $VS_2026 == 1 ]]; then
        VS_PLATFORM="${PLATFORM}_2026"
    else
        VS_PLATFORM="${PLATFORM}"
    fi
    if [[ $BLEEDING_EDGE = 1 ]]; then
        if [[ $LEGACY == 1 ]]; then
            PKGS="openFrameworksLibs_${VER}_${PLATFORM}_2019_64_1.zip \
                  openFrameworksLibs_${VER}_${PLATFORM}_2019_64_2.zip"
        elif [[ "$ARCH" == "all" ]]; then
            # Explicit multi-arch (CI / packagers)
            PKGS="openFrameworksLibs_${VER}_${VS_PLATFORM}_64_1.zip \
                  openFrameworksLibs_${VER}_${VS_PLATFORM}_64_2.zip \
                  openFrameworksLibs_${VER}_${VS_PLATFORM}_arm64_1.zip \
                  openFrameworksLibs_${VER}_${VS_PLATFORM}_arm64_2.zip \
                  openFrameworksLibs_${VER}_${VS_PLATFORM}_arm64ec_1.zip \
                  openFrameworksLibs_${VER}_${VS_PLATFORM}_arm64ec_2.zip"
        elif [[ "$ARCH" == "arm64" ]]; then
            # Windows on ARM: native arm64 + arm64ec (x64-compat) for Win11 ARM
            echo " VS packages: arm64 + arm64ec (Windows on ARM host)"
            PKGS="openFrameworksLibs_${VER}_${VS_PLATFORM}_arm64_1.zip \
                  openFrameworksLibs_${VER}_${VS_PLATFORM}_arm64_2.zip \
                  openFrameworksLibs_${VER}_${VS_PLATFORM}_arm64ec_1.zip \
                  openFrameworksLibs_${VER}_${VS_PLATFORM}_arm64ec_2.zip"
        elif [[ "$ARCH" == "arm64ec" ]]; then
            PKGS="openFrameworksLibs_${VER}_${VS_PLATFORM}_arm64ec_1.zip \
                  openFrameworksLibs_${VER}_${VS_PLATFORM}_arm64ec_2.zip"
        else
            # x64 host (default)
            PKGS="openFrameworksLibs_${VER}_${VS_PLATFORM}_64_1.zip \
                  openFrameworksLibs_${VER}_${VS_PLATFORM}_64_2.zip"
        fi
    else
        if [[ "$ARCH" == "all" ]]; then
            PKGS="openFrameworksLibs_${VER}_${PLATFORM}_64_1.zip \
                  openFrameworksLibs_${VER}_${PLATFORM}_64_2.zip \
                  openFrameworksLibs_${VER}_${PLATFORM}_64_3.zip \
                  openFrameworksLibs_${VER}_${PLATFORM}_64_4.zip"
        else
            PKGS="openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_1.zip \
                  openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_2.zip \
                  openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_3.zip \
                  openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_4.zip"
        fi
    fi
elif [[ "$PLATFORM" =~ ^(osx|ios|tvos|xros|catos|watchos|macos)$ ]]; then
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}_1.tar.bz2 \
              openFrameworksLibs_${VER}_${PLATFORM}_2.tar.bz2 \
              openFrameworksLibs_${VER}_${PLATFORM}_3.tar.bz2"
    else    
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}1.tar.bz2 \
              openFrameworksLibs_${VER}_${PLATFORM}2.tar.bz2 \
              openFrameworksLibs_${VER}_${PLATFORM}3.tar.bz2 \
              openFrameworksLibs_${VER}_${PLATFORM}4.tar.bz2" 
    fi 
elif [ "$ARCH" == "" ] && [ "$PLATFORM" == "android" ]; then
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}_armv7.tar.bz2 \
              openFrameworksLibs_${VER}_${PLATFORM}_arm64.tar.bz2 \
              openFrameworksLibs_${VER}_${PLATFORM}_x86_64.tar.bz2"
    else
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}armv7.tar.bz2 \
          openFrameworksLibs_${VER}_${PLATFORM}arm64.tar.bz2 \
          openFrameworksLibs_${VER}_${PLATFORM}x86.tar.bz2"
    fi
elif [ "$PLATFORM" == "emscripten" ]; then
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}.tar.bz2"
    else
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}${ARCH}.tar.bz2"
    fi
else # Linux
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}${OPT}.tar.bz2"
    else
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}${ARCH}.tar.bz2"
    fi
fi

cd ../../
mkdir -p libs
cd libs

mkdir -p download
cd download

# IFS=' ' read -r -a PKGS_DATA <<< "$PKGS"
# if [ $OVERWRITE -eq 1 ]; then
#     for ((i = 0; i < ${#PKGS_DATA[@]}; i++)); do
#         FILE_CHECK="${PKGS_DATA[$i]}"
#         # Check if the file exists
#         if [ -e "${FILE_CHECK}" ]; then
#             echo " Removing Prior Download:[${FILE_CHECK}]"
#             # Remove the file or directory
#             rm -rf "${FILE_CHECK}"
#         fi
#     done
# fi

download "${PKGS[@]}"

# Integrity: GitHub release asset digests (sha256:…) when published
if ! verifyPackageDigests "$PKGS"; then
    writeVerifyState "$(pwd)" "$REPO" "$PLATFORM" "$ARCH"
    exit 1
fi
writeVerifyState "$(pwd)" "$REPO" "$PLATFORM" "$ARCH"

cd ../ # back to libs
VALID=1
for PKG in $PKGS; do
    echo " Validate libraries [${PLATFORM}] from [$PKG]"
    if [ ! -f "download/$PKG" ]; then
        echo "Error: File 'download/$PKG' does not exist!" >&2
        VALID=0
    fi
done
if [ $VALID -eq 0 ]; then
    exit 71
fi
libs=("cairo" "curl" "FreeImage" "brotli" "fmod" "freetype" "glew" "glfw" "json" "libpng" "openssl" "pixman" "poco" "rtAudio" "tess2" "uriparser" "utf8" "videoInput" "zlib" "opencv" "ippicv" "assimp" "libxml2" "svgtiny" "fmt")

# Resolve which lib/<name> folder this package uses.
# Apple multi-target packages (macos, and ios/tvos/… wrappers that pass -p macos)
# install under lib/macos/*.xcframework. Plain "osx" may use lib/osx and/or a
# macos-arm64_x86_64 slice — never wipe the whole lib/macos tree for -p osx.
LIB_PLATFORM_DIR="$PLATFORM"
case "$PLATFORM" in
    ios|tvos|xros|catos|watchos|macos) LIB_PLATFORM_DIR="macos" ;;
esac

if [ $OVERWRITE -eq 1 ]; then
    echo " "
    if [ $FULL_CLEAN -eq 1 ]; then
        echo " Full-clean - Removing platform libs + shared include/bin for [$PLATFORM]"
    else
        echo " Platform-clean - Removing prior libs only under lib/[$LIB_PLATFORM_DIR] (other platforms kept)"
    fi
    LINUX_CANONICAL_LIB_SUBPATH=""
    LINUX_LEGACY_LIB_SUBPATH=""
    if [ "$PLATFORM" == "linux" ]; then
        LINUX_CANONICAL_LIB_SUBPATH=$(linux_canonical_lib_subpath "$ARCH")
        LINUX_LEGACY_LIB_SUBPATH=$(linux_legacy_lib_subpath "$ARCH")
    fi
    for ((i=0;i<${#libs[@]};++i)); do
        if [ "$PLATFORM" == "linux" ] && [ -n "$LINUX_CANONICAL_LIB_SUBPATH" ]; then
            if [ -e "${libs[i]}/lib/$LINUX_CANONICAL_LIB_SUBPATH" ]; then
                echo "  Removing: [${libs[i]}/lib/$LINUX_CANONICAL_LIB_SUBPATH]"
                rm -rf "${libs[i]}/lib/$LINUX_CANONICAL_LIB_SUBPATH"
            fi
        elif [ -e "${libs[i]}/lib/$LIB_PLATFORM_DIR" ]; then
            echo "  Removing: [${libs[i]}/lib/$LIB_PLATFORM_DIR]"
            rm -rf "${libs[i]}/lib/$LIB_PLATFORM_DIR"
        fi
        if [ -n "$LINUX_LEGACY_LIB_SUBPATH" ] && [ -e "${libs[i]}/lib/$LINUX_LEGACY_LIB_SUBPATH" ]; then
            echo "  Removing: [${libs[i]}/lib/$LINUX_LEGACY_LIB_SUBPATH]"
            rm -rf "${libs[i]}/lib/$LINUX_LEGACY_LIB_SUBPATH"
        fi
        # Shared include/ + vs|msys2 bin/ only with --full-clean (not for multi-platform installs)
        if [ $FULL_CLEAN -eq 1 ]; then
            if [ "$PLATFORM" == "msys2" ] || [ "$PLATFORM" == "vs" ]; then
                if [ -e "${libs[i]}/bin" ]; then
                    echo "  Removing: [${libs[i]}/bin]"
                    rm -rf "${libs[i]}/bin"
                fi
            fi
            if [ -e "${libs[i]}/include" ]; then
                echo "  Removing: [${libs[i]}/include]"
                rm -rf "${libs[i]}/include"
            fi
        fi
    done
fi

# osx host packages may also refresh the desktop slice inside lib/macos/*.xcframework
# without deleting ios/tvos/… sibling slices in the same xcframework.
if [ "$PLATFORM" == "osx" ]; then
    echo " "
    echo " xcframework - refresh macos host slice (ios/tvos/… slices kept)"
    for ((i=0;i<${#libs[@]};++i)); do
        xcframework_path="${libs[i]}/lib/macos/${libs[i]}.xcframework/macos-arm64_x86_64"
        if [ $OVERWRITE -eq 1 ]; then
            if [ -e "$xcframework_path" ]; then
                echo "  Removing: [$xcframework_path]"
                rm -rf "$xcframework_path"
            fi
        fi
        info_plist_path="${libs[i]}/lib/macos/${libs[i]}.xcframework/Info.plist"
        if [ -e "$info_plist_path" ]; then
            cp "$info_plist_path" "${info_plist_path}.bak"
        fi
    done
elif [ "$PLATFORM" == "macos" ]; then
    # Full lib/macos already cleared above when OVERWRITE=1; only need Info.plist backup if merge
    echo " "
    echo " xcframework - macos multi-target package (lib/macos)"
    for ((i=0;i<${#libs[@]};++i)); do
        info_plist_path="${libs[i]}/lib/macos/${libs[i]}.xcframework/Info.plist"
        if [ -e "$info_plist_path" ]; then
            cp "$info_plist_path" "${info_plist_path}.bak"
        fi
    done
fi

echo " ------ "
for PKG in $PKGS; do
    echo " Uncompressing libraries [${PLATFORM}] from [$PKG]"
    if [ ! -f "download/$PKG" ]; then
        echo "Error: File 'download/$PKG' does not exist!" >&2
        exit 71
    fi

    if [ "$PLATFORM" == "msys2" ] || [ "$PLATFORM" == "vs" ]; then
        unzip -qo download/$PKG
        # rm -r download/$PKG
    else

        # Apothecary linux2026 archives unpack as lib/<name>/lib/linux/<arch>/.
        tar xf download/$PKG > /dev/null 2>&1
        # rm -r download/$PKG
    fi
    echo " Deployed libraries from [download/$PKG] to [/libs]"
done

# Apothecary linux2026 packages already use lib/linux/<arch>. Promote any
# leftover pre-0.13 flat folders (linux64, linuxarmv6l, …) to that layout.
normalize_linux_lib_paths
reindex_msys2_static_libs

if [ "$PLATFORM" == "osx" ]; then
    echo " "
    for ((i=0;i<${#libs[@]};++i)); do
        info_plist_path="${libs[i]}/lib/macos/${libs[i]}.xcframework/Info.plist"
        if [ -e "${info_plist_path}.bak" ]; then
            #echo "  Restoring: [${info_plist_path}.bak] to [${info_plist_path}]"
            mv "${info_plist_path}.bak" "$info_plist_path" 2>/dev/null
        fi
    done
fi

if [[ $BLEEDING_EDGE = 1 ]] ; then
    if [ "$PLATFORM" == "osx" ]; then
        addonslibs=("opencv" "ippicv" "libusb" "assimp" "libxml2" "svgtiny" "poco")
        addons=("ofxOpenCv" "ofxOpenCv" "ofxKinect" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco" )
    elif [ "$PLATFORM" == "vs" ]; then
        addonslibs=("opencv" "ippicv" "libusb" "assimp" "libxml2" "svgtiny" "poco")
        addons=("ofxOpenCv" "ofxOpenCv" "ofxKinect" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco")
    elif [ "$PLATFORM" == "ios" ] || [ "$PLATFORM" == "tvos" ]; then
        addonslibs=("opencv" "ippicv" "assimp" "libxml2" "svgtiny" "poco" )
        addons=("ofxOpenCv" "ofxOpenCv" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco")
    else
        addonslibs=("opencv" "ippicv" "assimp" "libxml2" "svgtiny" "poco")
        addons=("ofxOpenCv" "ofxOpenCv" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco")
    fi
else
    if [ "$PLATFORM" == "osx" ]; then
        addonslibs=("opencv" "ippicv" "libusb" "assimp" "libxml2" "svgtiny" "poco" "openssl")
        addons=("ofxOpenCv" "ofxOpenCv" "ofxKinect" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco" "ofxPoco")
    elif [ "$PLATFORM" == "vs" ]; then
        addonslibs=("opencv" "ippicv" "libusb" "assimp" "libxml2" "svgtiny" "poco")
        addons=("ofxOpenCv" "ofxOpenCv" "ofxKinect" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco")
    elif [ "$PLATFORM" == "ios" ] || [ "$PLATFORM" == "tvos" ]; then
        addonslibs=("opencv" "ippicv" "assimp" "libxml2" "svgtiny" "poco" )
        addons=("ofxOpenCv" "ofxOpenCv" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco")
    else
        addonslibs=("opencv" "ippicv" "assimp" "libxml2" "svgtiny" "poco")
        addons=("ofxOpenCv" "ofxOpenCv" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco")
    fi
fi

echo "   ------ "
if [ "$PLATFORM" == "osx" ]; then
    if [ $OVERWRITE -eq 1 ]; then
        # Refresh desktop slice only — keep ios/tvos/… slices in the same xcframework
        for ((i=0;i<${#addonslibs[@]};++i)); do
            xcframework_path="../addons/${addons[i]}/libs/${addonslibs[i]}/lib/macos/${addonslibs[i]}.xcframework/macos-arm64_x86_64"
            if [ -e "$xcframework_path" ]; then
                echo "  Removing addon host slice: [$xcframework_path]"
                rm -rf "$xcframework_path"
            fi
            info_plist_path="../addons/${addons[i]}/libs/${addonslibs[i]}/lib/macos/${addonslibs[i]}.xcframework/Info.plist"
            if [ -e "$info_plist_path" ]; then
                cp "$info_plist_path" "${info_plist_path}.bak"
            fi
        done
    fi
elif [ "$PLATFORM" == "macos" ]; then
    for ((i=0;i<${#addonslibs[@]};++i)); do
        info_plist_path="../addons/${addons[i]}/libs/${addonslibs[i]}/lib/macos/${addonslibs[i]}.xcframework/Info.plist"
        if [ -e "$info_plist_path" ]; then
            cp "$info_plist_path" "${info_plist_path}.bak"
        fi
    done
fi


if [ $OVERWRITE -eq 1 ]; then
    for ((i=0;i<${#addonslibs[@]};++i)); do
        if [ -e ${addonslibs[i]} ] ; then

            echo " Platform-clean - addon: [${addons[i]} - ${addonslibs[i]}] → lib/$LIB_PLATFORM_DIR"
            if [ -e ../addons/${addons[i]}/libs/${addonslibs[i]}/lib/$LIB_PLATFORM_DIR ]; then
                echo "   Remove binaries: [${addons[i]}/libs/${addonslibs[i]}/lib/$LIB_PLATFORM_DIR]"
                rm -rf ../addons/${addons[i]}/libs/${addonslibs[i]}/lib/$LIB_PLATFORM_DIR
            fi
            if [ $FULL_CLEAN -eq 1 ]; then
                if [ -e ../addons/${addons[i]}/libs/${addonslibs[i]}/bin ]; then
                    echo "   Remove binaries: [${addons[i]}/libs/${addonslibs[i]}/bin]"
                    rm -rf ../addons/${addons[i]}/libs/${addonslibs[i]}/bin
                fi
                if [ -e ../addons/${addons[i]}/libs/${addonslibs[i]}/include ]; then
                    echo "   Remove include: [${addons[i]}/libs/${addonslibs[i]}/include]"
                    rm -rf ../addons/${addons[i]}/libs/${addonslibs[i]}/include
                fi
            fi
        fi
    done
    echo "   ------ "
fi

for ((i=0;i<${#addonslibs[@]};++i)); do
    if [ -e "${addonslibs[i]}" ]; then
        echo "   Deploying [${addonslibs[i]}] to [../addons/${addons[i]}/libs]"
        mkdir -p ../addons/${addons[i]}/libs/${addonslibs[i]}
        if ! command -v rsync &> /dev/null
        then      
            cp -a ${addonslibs[i]}/* ../addons/${addons[i]}/libs/${addonslibs[i]}    
        else
            rsync -azp ${addonslibs[i]}/ ../addons/${addons[i]}/libs/${addonslibs[i]}/
        fi
        rm -rf ${addonslibs[i]}
    fi
done

if [ "$PLATFORM" == "osx" ]; then
    echo " "
    for ((i=0;i<${#addonslibs[@]};++i)); do
        if [ -e ${addonslibs[i]} ] ; then
            info_plist_path="../addons/${addons[i]}/libs/${addonslibs[i]}/lib/macos/${addonslibs[i]}.xcframework/Info.plist"
            if [ -e "${info_plist_path}.bak" ]; then
                mv "${info_plist_path}.bak" "$info_plist_path" 2>/dev/null
            fi
        fi
    done
fi

echo " ------ "
if [[ "$SHA_STATUS" == "verified" ]]; then
    echo " openFrameworks download_libs complete — secure (SHA-256 verified: ${SHA_VERIFIED} package(s))"
elif [[ "$SHA_STATUS" == "skipped" || "$SHA_STATUS" == "no-digest" || "$SHA_STATUS" == "no-remote" || "$SHA_STATUS" == "no-tools" ]]; then
    echo " openFrameworks download_libs complete — integrity not fully verified (${SHA_STATUS})"
else
    echo " openFrameworks download_libs and install complete!"
fi
