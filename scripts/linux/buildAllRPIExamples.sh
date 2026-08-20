DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PARENT_DIR="$(dirname "$DIR")"

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
OFDIR="$(cd "$SCRIPT_DIR/../.." && pwd)"
OFCORE_EXAMPLES_DIR="${OFDIR}/examples"

ARCH="${ARCH:-$(uname -m)}"
case "$ARCH" in
        aarch64|arm64) TEMPLATE=linuxaarch64 ;;
        armv7l) TEMPLATE=linuxarmv7l ;;
        *) TEMPLATE=linuxarmv6l ;;
esac
MAKEFILE_PATH=$PARENT_DIR/templates/${TEMPLATE}/Makefile
cd ${OFCORE_EXAMPLES_DIR}

for category in $(ls -1d *)
do
        if [ $category != "addons" ] && [ $category != "ios" ] && [ $category != "android" ] && [[ -d $category ]]; then
 
                echo "CHANGED TO CATEGORY >"+$category
                cd $category
                for j in $(ls -1d *)
                do
                        echo ">>$j"
                        cd $j
#                       make clean -f $MAKEFILE_PATH
                        make -f $MAKEFILE_PATH
                        ret=$?
                        if [ $ret -ne 0 ];
                        then
                                echo "error compiling: " + $j
                        else
                                echo "successfully compiled :" + $j
                        fi
                        cd ../
                done
                cd ../
        fi
done
