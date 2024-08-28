DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PARENT_DIR="$(dirname "$DIR")"
MAKEFILE_PATH=$PARENT_DIR/templates/linuxarmv6l/Makefile
cd ${DIR}/../../examples

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
