# $1 -> platform: win_cb, linux, linux64, vs2008, osx, osxSL, iphone, all
# $2 -> version number: 006

platform=$1
version=$2

runOSXSLScript=0

if [ "$platform" = "osxSL" ]; then
    platform="osx"
    runOSXSLScript=1
    echo "will make changes for snow leopard"
fi

if [ "$platform" != "win_cb" ] && [ "$platform" != "linux" ] && [ "$platform" != "linux64" ] && [ "$platform" != "vs2008" ] && [ "$platform" != "vs2010" ] && [ "$platform" != "osx" ] && [ "$platform" != "android" ] && [ "$platform" != "iphone" ] && [ "$platform" != "all" ]; then
    echo usage: 
    echo ./create_package.sh platform version
    echo platform:
    echo win_cb, linux, linux64, vs2008, vs2010, osx, android, iphone, all
    exit 1
fi

if [ "$version" == "" ]; then
    echo usage: 
    echo ./create_package.sh platform version
    echo platform:
    echo win_cb, linux, linux64, vs2008, osx, android, all
    exit 1
fi

REPO=git://github.com/openframeworks/openFrameworks.git
REPO_ALIAS=ofmain
BRANCH=master

libsnotinmac="unicap gstappsink glu quicktime videoInput"
libsnotinlinux="quicktime videoInput glut glu"
libsnotinwindows="unicap gstappsink"
libsnotinandroid="glut unicap gstappsink quicktime videoInput fmodex glee rtAudio"
libsnotiniphone="glut unicap gstappsink quicktime videoInput fmodex glee rtAudio"

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
    rm makefile
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
        #delete codeblocks for other platforms and rename
        cp ${current_example}_$platform.cbp $current_example.newcbp
        cp ${current_example}_$platform.workspace $current_example.newworkspace
        rm *.cbp *.workspace
        mv $current_example.newcbp $current_example.cbp
        mv $current_example.newworkspace $current_example.workspace
        perl -pi -e s/${example_name}_${platform}/${example_name}/g $current_example.workspace

        #delete other platform's project files
        if [ "$platform" = "win_cb" ]; then
            deleteMakefiles
        fi
        deleteXcode
        deleteVS2008
        deleteVS2010
	    deleteEclipse
        
    fi

    #osx
    if [ "$platform" = "osx" ]; then
        #delete other platform's project files
        deleteCodeblocks
        deleteMakefiles
        deleteVS2008
        deleteVS2010
	    deleteEclipse
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
	    deleteVS2008
        deleteCodeblocks
        deleteMakefiles
        deleteXcode
	    deleteEclipse
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
        deleteCodeblocks
        deleteMakefiles
        deleteXcode
	    deleteEclipse	
    fi

    #android
    if [ "$platform" = "android" ]; then
        #delete other platform's project files
        deleteCodeblocks
        deleteMakefiles
        deleteXcode
	    deleteVS2008
        deleteVS2010
    fi
}

function createPackage {
    pkg_platform=$1
    pkg_version=$2
    pkg_ofroot=$3
    
    #remove previously created package 
    cd $pkg_ofroot/..
	if [ $runOSXSLScript = 1 ]; then
		rm -Rf of_preRelease_v${pkg_version}_osxSL*
	else
	    rm -Rf of_preRelease_v${pkg_version}_${pkg_platform}.*
		rm -Rf of_preRelease_v${pkg_version}_${pkg_platform}_*
    fi
    echo "creating package $pkg_platform $version in $pkg_ofroot"
    
    #delete other platforms example project files
    cd $pkg_ofroot/apps/examples

    for example in $( find . -maxdepth 1 -mindepth 1 -type d )
    do
        echo deleting $example
        deleteProjectFiles $pkg_platform $pkg_ofroot $example
        cd $pkg_ofroot/apps/examples
    done


    #delete other platforms addons examples project files
    cd $pkg_ofroot/apps/addonsExamples

    for example in $( find . -maxdepth 1 -mindepth 1 -type d )
    do
        deleteProjectFiles $pkg_platform $pkg_ofroot $example
        cd $pkg_ofroot/apps/addonsExamples
    done

	#delete iphone examples in other platforms
	cd $pkg_ofroot/apps

	if [ "$pkg_platform" != "iphone" ]; then 
		rm -Rf iPhoneSpecificExamples
		rm -Rf iPhoneExamples
		rm -Rf iPhoneAddonsExamples
	fi

	#delete android examples in other platforms
	if [ "$pkg_platform" != "android" ]; then 
		rm -Rf androidExamples
	fi

	#delete desktop examples in mobile packages
	if [ "$pkg_platform" == "android" ] || [ "$pkg_platform" == "iphone" ]; then 
		rm -Rf examples
		rm -Rf addonsExamples
	fi 

    #delete other platform libraries
    if [ "$pkg_platform" = "linux" ]; then
        otherplatforms="linux64 osx win_cb vs2008 vs2010 iphone android"
    fi

    if [ "$pkg_platform" = "linux64" ]; then
        otherplatforms="linux osx win_cb vs2008 vs2010 iphone android"
    fi

    if [ "$pkg_platform" = "osx" ]; then
        otherplatforms="linux linux64 win_cb vs2008 vs2010 iphone android"
    fi

    if [ "$pkg_platform" = "win_cb" ]; then
        otherplatforms="linux linux64 osx vs2008 vs2010 iphone android"
    fi

    if [ "$pkg_platform" = "vs2008" ]; then
        otherplatforms="linux linux64 osx win_cb vs2010 iphone android"
    fi

    if [ "$pkg_platform" = "vs2010" ]; then
        otherplatforms="linux linux64 osx win_cb vs2008 iphone android"
    fi

    if [ "$pkg_platform" = "iphone" ]; then
        otherplatforms="linux linux64 win_cb vs2008 vs2010 android"
    fi

    if [ "$pkg_platform" = "android" ]; then
        otherplatforms="linux linux64 osx win_cb vs2008 vs2010 iphone"
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
    elif [ "$pkg_platform" = "iphone" ]; then
        rm -Rf $libsnotiniphone
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

	#delete specific include folders for non-iphone
	if [ "$pkg_platform" != "iphone" ]; then
		rm -Rf $( ls -d */include_iphone )
	fi

	#delete generic includes for libs that has specific ones in android
	if [ "$pkg_platform" == "android" ] || [ "$pkg_platform" == "iphone" ]; then
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
    	rm -Rf win_cb vs2008 vs2010 osx iphone
	fi
    rm create_package.sh

    #delete .svn dirs
    cd $pkg_ofroot
    rm -Rf $(find . -type d -name .svn)
    
    #delete .gitignore 
    cd $pkg_ofroot
    rm -Rf $(find . -name .gitignore)
    
    #delete dev folders
    cd $pkg_ofroot/apps
    rm -Rf devApps
    cd ${pkg_ofroot}/scripts
    rm -Rf dev

	#delete xcode templates in other platforms
	cd $pkg_ofroot
	if [ "$pkg_platform" != "osx" ] && [ "$pkg_platform" != "iphone" ]; then
		rm -Rf "xcode templates"
	fi
	
	cd ${pkg_ofroot}/addons
	#delete ofxAndroid in non android
	if [ "$pkg_platform" != "android" ]; then
		rm -Rf ofxAndroid
	fi
	#delete ofxIphone in non iphone
	if [ "$pkg_platform" != "iphone" ]; then
		rm -Rf ofxiPhone
	fi

	#delete eclipse project
	rm $(find . -name .*project)

	#download and copy OF compiled
	cd $pkg_ofroot/libs/openFrameworksCompiled/lib/${pkg_platform}
    if [ "$pkg_platform" = "win_cb" ]; then
		wget http://openframeworks.cc/git_pkgs/OF_compiled/${pkg_platform}/openFrameworks.lib
		wget http://openframeworks.cc/git_pkgs/OF_compiled/${pkg_platform}/openFrameworksDebug.lib
	fi

    #if snow leopard change 10.4u to 10.5
    if [ $runOSXSLScript = 1 ]; then
        cd $pkg_ofroot
        echo "replacing 10.4u with 10.5 for snow leopard"
        find . -name '*.pbxproj' | xargs perl -pi -e 's/10\.4u/10\.5/g'
        pkg_platform="osxSL"
    fi
    
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
    if [ "$pkg_platform" = "linux" ] || [ "$pkg_platform" = "linux64" ] || [ "$pkg_platform" = "android" ]; then
        mkdir of_preRelease_v${pkg_version}_${pkg_platform}_FAT
        mv openFrameworks/* of_preRelease_v${pkg_version}_${pkg_platform}_FAT
        tar czf of_preRelease_v${pkg_version}_${pkg_platform}_FAT.tar.gz of_preRelease_v${pkg_version}_${pkg_platform}_FAT
        mv of_preRelease_v${pkg_version}_${pkg_platform}_FAT of_preRelease_v${pkg_version}_${pkg_platform}
        rm -Rf of_preRelease_v${pkg_version}_${pkg_platform}/addons of_preRelease_v${pkg_version}_${pkg_platform}/apps/addonsExamples
        tar czf of_preRelease_v${pkg_version}_${pkg_platform}.tar.gz of_preRelease_v${pkg_version}_${pkg_platform}
        rm -Rf of_preRelease_v${pkg_version}_${pkg_platform}
    else
        mkdir of_preRelease_v${pkg_version}_${pkg_platform}_FAT
        mv openFrameworks/* of_preRelease_v${pkg_version}_${pkg_platform}_FAT
        zip -r of_preRelease_v${pkg_version}_${pkg_platform}_FAT.zip of_preRelease_v${pkg_version}_${pkg_platform}_FAT
        mv of_preRelease_v${pkg_version}_${pkg_platform}_FAT of_preRelease_v${pkg_version}_${pkg_platform}        
        rm -Rf of_preRelease_v${pkg_version}_${pkg_platform}/addons of_preRelease_v${pkg_version}_${pkg_platform}/apps/addonsExamples
        zip -r of_preRelease_v${pkg_version}_${pkg_platform}.zip of_preRelease_v${pkg_version}_${pkg_platform}
        rm -Rf of_preRelease_v${pkg_version}_${pkg_platform}
    fi
}


if [ "$platform" = "all" ]; then
    for eachplatform in win_cb linux linux64 vs2008 vs2010 osx 
    do
        cd $packageroot
        mkdir of_preRelease_v${version}_${eachplatform}
        cp -R addons apps export libs other scripts of_preRelease_v${version}_${eachplatform}
        cd of_preRelease_v${version}_${eachplatform}
        createPackage $eachplatform $2 $PWD
    done
    
    cd $packageroot
    echo dir: $PWD
    mkdir of_preRelease_v${version}_all_FAT
    mv addons apps export libs other scripts $packageroot/of_preRelease_v${version}_all_FAT
    tar czf of_preRelease_v$version_all_FAT.tar.gz of_preRelease_v${version}_all_FAT
    mv of_preRelease_v${version}_all_FAT of_preRelease_v${version}_all
    rm -Rf of_preRelease_v${version}_all/addons of_preRelease_v${version}_all/apps/addonsExamples
    tar czf of_preRelease_v$version_all.tar.gz of_preRelease_v$version_all
    rm -Rf of_preRelease_v${version}_all
    mv * $packageroot/..
    #rm -Rf $packageroot
else
    createPackage $platform $version $packageroot
fi

cd $packageroot
git reset --hard

cd $packageroot/.. 
    
