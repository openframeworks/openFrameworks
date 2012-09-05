#!/bin/bash
# $1 -> platform: win_cb, linux, linux64, vs2008, osx, osxSL, ios, all
# $2 -> version number: 006

platform=$1
version=$2

#runOSXSLScript=0

hostArch=`uname`

#if [ "$platform" = "osxSL" ]; then
#    platform="osx"
#    runOSXSLScript=1
#    echo "will make changes for snow leopard"
#fi

if [ "$platform" != "win_cb" ] && [ "$platform" != "linux" ] && [ "$platform" != "linux64" ] && [ "$platform" != "vs2008" ] && [ "$platform" != "vs2010" ] && [ "$platform" != "osx" ] && [ "$platform" != "android" ] && [ "$platform" != "ios" ] && [ "$platform" != "all" ]; then
    echo usage: 
    echo ./create_package.sh platform version
    echo platform:
    echo win_cb, linux, linux64, vs2008, vs2010, osx, android, ios, all
    exit 1
fi

if [ "$version" == "" ]; then
    echo usage: 
    echo ./create_package.sh platform version
    echo platform:
    echo win_cb, linux, linux64, vs2008, osx, android, all
    exit 1
fi

REPO=https://github.com/openframeworks/openFrameworks
REPO_ALIAS=upstreamhttps
BRANCH=develop

libsnotinmac="unicap gstappsink glu quicktime videoInput"
libsnotinlinux="quicktime videoInput glut glu"
libsnotinwindows="unicap gstappsink"
libsnotinandroid="glut unicap gstappsink quicktime videoInput fmodex glee rtAudio"
libsnotinios="glut unicap gstappsink quicktime videoInput fmodex glee rtAudio"

if [ ! -d openFrameworks/.git ]; then
    git clone $REPO 
    gitfinishedok=$?
    if [ $gitfinishedok -ne 0 ]; then
        echo "Error connecting to github"
        exit
    fi
fi



cd openFrameworks
if [ "$BRANCH" != "master" ]; then
	git remote add $REPO_ALIAS $REPO
	git fetch $REPO_ALIAS
    git checkout --track -b $BRANCH ${REPO_ALIAS}/${BRANCH}
fi
git reset --hard
git pull $REPO $BRANCH


packageroot=$PWD

function deleteCodeblocks {
    #delete codeblock files
    rm *.cbp
    rm *.sh
    rm *.workspace
}

function deleteMakefiles {
    #delete makefiles
    rm Makefile
    rm *.make
    rm cb_build_runner.sh
}

function deleteVS2008 {
    #delete vs2008 files
    rm *.vcproj
    rm *.vcproj.user
    rm *_vs2008.sln
}

function deleteVS2010 {
    #delete vs2010 files
    rm *.vcxproj
    rm *.vcxproj.user
    rm *.vcxproj.filters
    rm *_vs2010.sln
}

function deleteXcode {
    #delete osx files
    rm -Rf *.xcodeproj
    rm openFrameworks-Info.plist
}

function deleteEclipse {
    #delete eclipse project
    rm $(find . -name .*project)
}

function deleteProjectFiles {
    platform=$1
    ofroot=$2
    current_example=$3
    oflib_root=${ofroot}/libs/openFrameworksCompiled    
    example_name=`echo ${current_example} | sed "s/\.\\///"`
    
    cd $current_example
    
    echo "deleting projects for $platform $example in $ofroot"
    echo "oflib in $oflib_root"
    echo "current dir: " `pwd`
    echo "current example $example_name"
    
    #codeblocks
    if [ "$platform" = "linux" ] || [ "$platform" = "linux64" ] || [ "$platform" = "win_cb" ]; then 
        deleteVS2010
	    deleteEclipse
        if [ "$platform" = win_cb ]; then
            rm Makefile
            rm config.make
        fi
    fi

    #osx
    if [ "$platform" = "osx" ]; then
        #delete other platform's project files
        deleteVS2010
	    deleteEclipse
	    deleteMakefiles
    fi

    #visual studio 2010
    if [ "$platform" = "vs2010" ]; then
	    #delete non needed vs files and rename
        mv ${current_example}_$platform.vcxproj $current_example.vcxproj
        mv ${current_example}_$platform.vcxproj.user $current_example.vcxproj.user
        mv ${current_example}_$platform.vcxproj.filters $current_example.vcxproj.filters
        mv ${current_example}_$platform.sln $current_example.sln
        perl -pi -e s/${example_name}_${platform}/${example_name}/g $current_example.sln

        #delete other platform's project files
	    deleteEclipse
	    deleteMakefiles
    fi


    #visual studio 2008
    if [ "$platform" = "vs2008" ]; then
	    #delete non needed vs files and rename
        mv ${current_example}_$platform.vcproj $current_example.vcproj
        mv ${current_example}_$platform.vcproj.user $current_example.vcproj.user
        mv ${current_example}_$platform.sln $current_example.sln
        perl -pi -e s/${example_name}_${platform}/${example_name}/g $current_example.sln

        #delete other platform's project files
	    deleteVS2010
	    deleteEclipse	
	    deleteMakefiles
    fi

    #android
    if [ "$platform" = "android" ]; then
        #delete other platform's project files
        deleteVS2010
    fi
}

function createProjectFiles {
    projectGenerator --allexamples --${pkg_platform}
}

function createPackage {
    pkg_platform=$1
    pkg_version=$2
    pkg_ofroot=$3
    
    #remove previously created package 
    cd $pkg_ofroot/..
	#if [ $runOSXSLScript = 1 ]; then
	#	rm -Rf of_v${pkg_version}_osxSL*
	#else
	    rm -Rf of_v${pkg_version}_${pkg_platform}.*
		rm -Rf of_v${pkg_version}_${pkg_platform}_*
    #fi
    echo "creating package $pkg_platform $version in $pkg_ofroot"
    
    #remove apps folder
    rm -r $pkg_ofroot/apps

	cd $pkg_ofroot/examples

	#delete ios examples in other platforms
	if [ "$pkg_platform" != "ios" ]; then 
		rm -Rf ios
	fi

	#delete android examples in other platforms
	if [ "$pkg_platform" != "android" ]; then 
		rm -Rf android
	fi

	#delete desktop examples in mobile packages
	if [ "$pkg_platform" == "android" ] || [ "$pkg_platform" == "ios" ]; then 
		rm -Rf 3d
		rm -Rf addons
		rm -Rf communication
		rm -Rf empty
		rm -Rf events
		rm -Rf gl
		rm -Rf graphics
		rm -Rf math
		rm -Rf sound
		rm -Rf utils
		rm -Rf video
	fi 
	
	#delete drag & drop and glInfo examples in linux, still not working
	if [ "$pkg_platform" == "linux" ] || [ "$pkg_platform" == "linux64" ]; then
	    rm -Rf utils/dragDropExample
	    rm -Rf gl/glInfoExample
	fi
    
	
	#delete ofxSynth addon, still not stable
	cd $pkg_ofroot
	rm -Rf addons/ofxSynth
	rm -Rf addons/ofxGui
	
    #create project files for platform
    createProjectFiles $pkg_platform $pkg_ofroot
	

    #delete other platform libraries
    if [ "$pkg_platform" = "linux" ]; then
        otherplatforms="linux64 osx win_cb vs2008 vs2010 ios android"
    fi

    if [ "$pkg_platform" = "linux64" ]; then
        otherplatforms="linux osx win_cb vs2008 vs2010 ios android"
    fi

    if [ "$pkg_platform" = "osx" ]; then
        otherplatforms="linux linux64 win_cb vs2008 vs2010 ios android"
    fi

    if [ "$pkg_platform" = "win_cb" ]; then
        otherplatforms="linux linux64 osx vs2008 vs2010 ios android"
    fi

    if [ "$pkg_platform" = "vs2008" ]; then
        otherplatforms="linux linux64 osx win_cb vs2010 ios android"
    fi

    if [ "$pkg_platform" = "vs2010" ]; then
        otherplatforms="linux linux64 osx win_cb vs2008 ios android"
    fi

    if [ "$pkg_platform" = "ios" ]; then
        otherplatforms="linux linux64 win_cb vs2008 vs2010 android"
    fi

    if [ "$pkg_platform" = "android" ]; then
        otherplatforms="linux linux64 osx win_cb vs2008 vs2010 ios"
    fi

    #delete libraries for other platforms
    cd $pkg_ofroot/libs  
    for lib in $( find . -maxdepth 1 -mindepth 1 -type d )
    do
        echo $PWD
        echo deleting $lib/lib
        cd $lib/lib
        rm -Rf $otherplatforms
        cd $pkg_ofroot/libs 
    done
    if [ "$pkg_platform" = "osx" ]; then
        rm -Rf $libsnotinmac
    elif [ "$pkg_platform" = "linux" ] || [ "$pkg_platform" = "linux64" ]; then
        rm -Rf $libsnotinlinux
    elif [ "$pkg_platform" = "win_cb" ] || [ "$pkg_platform" = "vs2008" ] || [ "$pkg_platform" = "vs2010" ]; then
        rm -Rf $libsnotinwindows
    elif [ "$pkg_platform" = "android" ]; then
        rm -Rf $libsnotinandroid
    elif [ "$pkg_platform" = "ios" ]; then
        rm -Rf $libsnotinios
    fi
    
    cd ${pkg_ofroot}/addons
    for lib in $( ls -d */libs/*/lib/ )
    do
        cd ${lib}
        echo $PWD
        echo deleting $lib
        rm -Rf $otherplatforms
        cd $pkg_ofroot/addons
    done

    #delete other platforms OF project files
    cd ${pkg_ofroot}/libs/openFrameworksCompiled/lib
    rm -Rf $otherplatforms
    cd ${pkg_ofroot}/libs/openFrameworksCompiled/project
    rm -Rf $otherplatforms

	cd ${pkg_ofroot}/libs
	#delete specific include folders non-android
	if [ "$pkg_platform" != "android" ]; then
		rm -Rf $( ls -d */include_android )
	fi

	#delete specific include folders for non-ios
	if [ "$pkg_platform" != "ios" ]; then
		rm -Rf $( ls -d */include_ios )
	fi

	#delete generic includes for libs that has specific ones in android
	if [ "$pkg_platform" == "android" ] || [ "$pkg_platform" == "ios" ]; then
		rm -Rf glu/include
	fi

    #delete dynamic libraries for other platforms
    cd $pkg_ofroot/export
    rm -Rf $otherplatforms
    if [ "$pkg_platform" = "osx" ]; then
        cd $pkg_ofroot
        rmdir export
    fi

    #delete scripts
    cd $pkg_ofroot/scripts
	if [ "$pkg_platform" != "linux64" ]; then
    	rm -Rf $otherplatforms
	else
    	rm -Rf win_cb vs2008 vs2010 osx ios
	fi
    rm create_package.sh

    #delete .svn dirs
    cd $pkg_ofroot
    rm -Rf $(find . -type d -name .svn)
    
    #delete .gitignore 
    cd $pkg_ofroot
    rm -Rf $(find . -name .gitignore)
    
    #delete dev folders
    cd ${pkg_ofroot}/scripts
    rm -Rf dev

	#delete xcode templates in other platforms
	cd $pkg_ofroot
	if [ "$pkg_platform" != "osx" ] && [ "$pkg_platform" != "ios" ]; then
		rm -Rf "xcode templates"
	fi
	
	cd ${pkg_ofroot}/addons
	#delete ofxAndroid in non android
	if [ "$pkg_platform" != "android" ]; then
		rm -Rf ofxAndroid
	fi
	#delete ofxiPhone in non ios
	if [ "$pkg_platform" != "ios" ]; then
		rm -Rf ofxiPhone
	fi

	#delete eclipse project
	#rm $(find . -name .*project)

	#download and copy OF compiled
	cd $pkg_ofroot/libs/openFrameworksCompiled/lib/${pkg_platform}
    if [ "$pkg_platform" = "win_cb" ]; then
		wget http://openframeworks.cc/git_pkgs/OF_compiled/${pkg_platform}/openFrameworks.lib
		wget http://openframeworks.cc/git_pkgs/OF_compiled/${pkg_platform}/openFrameworksDebug.lib
	fi

    #if snow leopard change 10.4u to 10.5
    #if [ $runOSXSLScript = 1 ]; then
    #    cd $pkg_ofroot
    #    echo "replacing 10.4u with 10.5 for snow leopard"
    #    find . -name '*.pbxproj' | xargs perl -pi -e 's/10\.4u/10\.5/g'
    #    pkg_platform="osxSL"
    #fi
    
    #choose readme
    cd $pkg_ofroot
    if [ "$platform" = "linux" ] || [ "$platform" = "linux64" ]; then
        mv readme.linux readme
    fi
    
    if [ "$platform" = "vs2008" ]; then
        mv readme.vs2008 readme
    fi
    
    if [ "$platform" = "win_cb" ]; then
        mv readme.win_cb readme
    fi
    
    if [ "$platform" = "osx" ]; then
        mv readme.osx readme
    fi

    if [ "$platform" = "android" ]; then
        mv readme.android readme
    fi
    
    rm readme.*
    mv readme readme.txt

    #create compressed package
    cd $pkg_ofroot/..
    mkdir -p openFrameworks/apps/myApps 
    if [ "$pkg_platform" = "android" ]; then
        cp -r openFrameworks/examples/android/androidEmptyExample openFrameworks/apps/myApps 
    elif [ "$pkg_platform" = "ios" ]; then
        cp -r openFrameworks/examples/ios/emptyExample openFrameworks/apps/myApps 
    fi
    echo "From 0071 onwards examples are now located in the root of OF at examples/" > openFrameworks/apps/_ExamplesMoved.txt
    echo "This folder will remain as a place to work on your own apps." >> openFrameworks/apps/_ExamplesMoved.txt
    echo "Just remember projects in apps/ still need to be two levels deep." >> openFrameworks/apps/_ExamplesMoved.txt 
    echo "" >> openFrameworks/apps/_ExamplesMoved.txt
    echo "So: " >> openFrameworks/apps/_ExamplesMoved.txt
    echo "" >> openFrameworks/apps/_ExamplesMoved.txt
    echo "apps/" >> openFrameworks/apps/_ExamplesMoved.txt
    echo "  mySoundApp/ " >> openFrameworks/apps/_ExamplesMoved.txt
    echo "" >> openFrameworks/apps/_ExamplesMoved.txt
    echo "will not work" >> openFrameworks/apps/_ExamplesMoved.txt
    echo "" >> openFrameworks/apps/_ExamplesMoved.txt
    echo "" >> openFrameworks/apps/_ExamplesMoved.txt
    echo "apps/" >> openFrameworks/apps/_ExamplesMoved.txt
    echo "  soundApps/" >> openFrameworks/apps/_ExamplesMoved.txt
    echo "      mySoundApp/ " >> openFrameworks/apps/_ExamplesMoved.txt
    echo "  miscApps/" >> openFrameworks/apps/_ExamplesMoved.txt
    echo "  experiments/" >> openFrameworks/apps/_ExamplesMoved.txt
    echo "" >> openFrameworks/apps/_ExamplesMoved.txt
    echo "will work " >> openFrameworks/apps/_ExamplesMoved.txt
    
    if [ "$pkg_platform" = "linux" ] || [ "$pkg_platform" = "linux64" ] || [ "$pkg_platform" = "android" ]; then
        mkdir of_v${pkg_version}_${pkg_platform}_release
        mv openFrameworks/* of_v${pkg_version}_${pkg_platform}_release
        tar czf of_v${pkg_version}_${pkg_platform}_release.tar.gz of_v${pkg_version}_${pkg_platform}_release
        rm -Rf of_v${pkg_version}_${pkg_platform}_release
    else
        mkdir of_v${pkg_version}_${pkg_platform}_release
        mv openFrameworks/* of_v${pkg_version}_${pkg_platform}_release
        zip -r of_v${pkg_version}_${pkg_platform}_release.zip of_v${pkg_version}_${pkg_platform}_release > /dev/null
        mv of_v${pkg_version}_${pkg_platform}_release of_v${pkg_version}_${pkg_platform}_release        
        rm -Rf of_v${pkg_version}_${pkg_platform}_release
    fi
}


if [ "$platform" = "all" ]; then
    for eachplatform in win_cb linux linux64 vs2008 vs2010 osx 
    do
        cd $packageroot
        mkdir of_v${version}_${eachplatform}
        cp -R addons apps export libs other scripts of_v${version}_${eachplatform}
        cd of_v${version}_${eachplatform}
        createPackage $eachplatform $2 $PWD
    done
    
    cd $packageroot
    echo dir: $PWD
    mkdir of_v${version}_all
    mv addons apps export libs other scripts $packageroot/of_v${version}_all
    tar czf of_v$version_all_FAT.tar.gz of_v${version}_all
    rm -Rf of_v${version}_all
    mv * $packageroot/..
    #rm -Rf $packageroot
else
    of_root=$(cat ~/.ofprojectgenerator/config)
    echo $packageroot > ~/.ofprojectgenerator/config
    createPackage $platform $version $packageroot
    echo $of_root > ~/.ofprojectgenerator/config
    
fi

cd $packageroot
git reset --hard

cd $packageroot/.. 
    
