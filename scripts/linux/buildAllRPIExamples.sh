cd ../../examples
for category in $(ls -1d *)
do
	if [ $category != "addons" ] && [ $category != "ios" ] && [ $category != "android" ]; then
 
		echo "CHANGED TO CATEGORY >"+$category
		cd $category
		for j in $(ls -1d *)
		do
			echo ">>$j"
			cd $j
			cp ../../../scripts/linux/template/linuxarmv6l/Makefile .
			make clean
			make
			ret=$?
			if [ $ret -ne 0 ];
			then
				echo "error compiling: " + $example
			else
				echo "successfully compiled :" + $example
			fi
			cd ../
		done
		cd ../
	fi
done
