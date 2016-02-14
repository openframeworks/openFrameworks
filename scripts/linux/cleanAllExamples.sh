#!/bin/bash

for category in $( find ../../examples/ -maxdepth 1 -type d )
do
    if [ "$category" = "../../examples/android" -o "$category" = "../../examples/ios" -o "$category" = "../../examples/" ]; then
        continue
    fi

    for example in $( find "$category" -maxdepth 1 -type d | grep -v osx )
    do
        if [ "$example" = "$category" ]; then
            continue
        fi
        echo -----------------------------------------------------------------
        echo cleaning "$example"

        make clean -C "$example"
        rm -rf "${example:?}/obj" 2> /dev/null
        rm -rf "${example:?}/"*.layout 2> /dev/null
        rm -rf "${example:?}/"*.backup 2> /dev/null
        rm -rf "${example:?}/"*.depend 2> /dev/null
        rm "${example:?}/"*~ 2> /dev/null
	if [ -f ${example}.qbs ]; then
	    qbs clean
	fi
	rm "{example:?}/"*.qbs 2> /dev/null
        echo -----------------------------------------------------------------
        echo
    done
done

for category in $( find ../../apps/ -maxdepth 1 -type d )
do
    for example in $( find "$category" -maxdepth 1 -type d | grep -v osx )
    do
        if [ "$example" = "$category" ]; then
            continue
        fi
        echo -----------------------------------------------------------------
        echo cleaning "$example"

        make clean -C "$example"
        rm -rf "${example:?}/obj" 2> /dev/null
        rm -rf "${example:?}/"*.layout 2> /dev/null
        rm -rf "${example:?}/"*.backup 2> /dev/null
        rm -rf "${example:?}/"*.depend 2> /dev/null
        rm "${example:?}/"*~ 2> /dev/null
        if [ -f ${example}.qbs ]; then
            qbs clean
        fi
        rm "{example:?}/"*.qbs 2> /dev/null

        echo -----------------------------------------------------------------
        echo
    done
done
