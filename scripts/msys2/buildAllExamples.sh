#!/bin/bash

export LC_ALL=C

examples_dir=../../examples
examples_prefix="$examples_dir/"

system=msys2
copy_makefiles () {
	cp ../templates/$system/Makefile $1/
	cp ../templates/$system/config.make $1/
}

delete_makefiles () {
	rm  $1/Makefile
	rm  $1/config.make
}

echo_and_log () {
	local str="$*"
    echo $str
	echo $str $'\r' >> $logfile
}

echo_category() {
	local cat=$1
	local category_short=${cat#$examples_prefix}
	echo $'\r'$'\r' >> $logfile
	echo_and_log "================================================================="
	echo -e "\e[1mcategory $category_short\e[0m"
	echo "category $category_short" $'\r' >> $logfile
	echo_and_log "================================================================="
	
}

echo_success() {
	#echo_and_log "[Success]"
	echo -e "\e[32m[Success]\e[0m"
	echo " " $'\r'>> $logfile
	echo "[Success]" $'\r'>> $logfile
	(( example_success++ ))
}

echo_failed() {
	echo -e "\e[31;7m[Failed]\e[0m"
	echo " " $'\r'>> $logfile
	echo "[Failed]" $'\r'>> $logfile
	(( example_fail++ ))
}


MAKEFLAGS=-j$NUMBER_OF_PROCESSORS

#initialize logfile
logfile=$examples_dir/ExamplesBuilder.log
echo "" > $logfile

example_total=0
example_success=0
example_fail=0
example_fail_list=""
example_skipped=0

echo_and_log "Building All Examples"

for category in $( find $examples_dir/ -mindepth 1 -maxdepth 1 -type d )
do
    if [ "$category" = "$examples_dir" -o "$category" = "$examples_dir/android" -o "$category" = "$examples_dir/ios" -o "$category" = "$examples_dir/examples/" ]; then
            continue
    fi
    
	echo_category $category
	
    for example in $( find $category -mindepth 1 -maxdepth 1 -type d | grep -v osx )
    do    
		
		echo_and_log "Building ${example#$examples_prefix}"
		(( example_total++ ))
		#if [ "$example" = "$examples_dir/3d/cameraLensOffsetExample" ]; then
		#		echo "   [Skipped]" >> $logfile
		#	(( example_skipped++ ))
        #    continue
        #fi
		
        
        
        
	    #projectGenerator .
		
		copy_makefiles $example
		#ERROR=$(</tmp/Error)
        make -j8 -C $example 1>>$logfile 2>>$logfile
        ret=$?
        if [ $ret -ne 0 ]; then
			echo_failed
			example_fail_list="$example_fail_list ${example#$examples_prefix}"
		else
			echo_success
        fi
		#delete_makefiles $example
		
		echo_and_log "-----------------------------------------------------------------" 
        echo ""
    done
done


echo 
echo_and_log 
echo_and_log "================================================================="
echo_and_log "Success:$example_success - Skipped:$example_skipped - Failed:$example_fail - total:$example_total"
echo_and_log "================================================================="

if [ $example_fail -gt 0 ]; then 
	echo_and_log "Failed examples :"
	echo_and_log "$example_fail_list"
	echo_and_log "================================================================="
fi
