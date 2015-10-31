#!/bin/bash

export LC_ALL=C

examples_dir=../../examples

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
    
	
	echo $'\r'$'\r' >> $logfile
	echo_and_log "================================================================="
	echo_and_log "category $category"
	echo_and_log "================================================================="
    
	
    for example in $( find $category -mindepth 1 -maxdepth 1 -type d | grep -v osx )
    do    
		
		echo "-----------------------------------------------------------------"
		echo_and_log "Building $example"
		(( example_total++ ))
		#if [ "$example" = "$examples_dir/3d/cameraLensOffsetExample" ]; then
		#		echo "   [Skipped]" >> $logfile
		#	(( example_skipped++ ))
        #    continue
        #fi
		
        
        
        
	    #projectGenerator .
		
		copy_makefiles $example
		#ERROR=$(</tmp/Error)
        make -j8 -C $example 2>>$logfile
        ret=$?
        if [ $ret -ne 0 ]; then
			#echo error compiling $example
			echo_and_log "   [Failed]$'\r'" >> $logfile
			(( example_fail++ ))
			example_fail_list="$example_fail_list $example"
		else
			echo "   [Success]$'\r'" >> $logfile
			(( example_success++ ))
			#delete_makefiles $example
        fi
		#delete_makefiles $example
		
		echo_and_log "-----------------------------------------------------------------" 
        echo ""
    done
done


echo_and_log " "
echo_and_log ""
echo_and_log ""
echo_and_log ""
echo_and_log ""

echo_and_log "Examples success:$example_success - skipped:$example_skipped - failed:$example_fail - total:$example_total"
echo_and_log "Failed examples"
echo_and_log $example_fail_list