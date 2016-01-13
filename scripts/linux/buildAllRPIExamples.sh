DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd ${DIR}/../../examples
for category in $(ls -1d *)
do
	if [ $category != "addons" ] && [ $category != "ios" ] && [ $category != "android" ]; then
 
		echo "CHANGED TO CATEGORY >"+$category
		cd $category
		for j in $(ls -1d *)
		do
			echo ">>$j"
			cd $j
			make -f ../../../scripts/templates/linuxarmv6l/Makefile clean
			make -f ../../../scripts/templates/linuxarmv6l/Makefile
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
