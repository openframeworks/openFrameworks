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

if [ "$platform" != "win_cb" ] && [ "$platform" != "linux" ] && [ "$platform" != "linux64" ] && [ "$platform" != "vs2008" ] && [ "$platform" != "osx" ] && [ "$platform" != "all" ]; then
    echo usage: 
    echo ./create_package.sh platform version
    echo platform:
    echo win_cb, linux, linux64, vs2008, osx, all
    exit 1
fi

if [ "$version" == "" ]; then
    echo usage: 
    echo ./create_package.sh platform version
    echo platform:
    echo win_cb, linux, linux64, vs2008, osx, all
    exit 1
fi

REPO=git://github.com/openframeworks/openFrameworks.git
BRANCH=master

libsnotinmac="unicap gstappsink glu quicktime videoInput"
libsnotinlinux="quicktime videoInput glut glu"
libsnotinwindows="unicap gstappsink"

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
    git checkout --track -b $BRANCH origin/$BRANCH
fi
git reset --hard
git pull $REPO $BRANCH


packageroot=$PWD

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
        cp ${current_example}_$platform.cbp $current_example.newcbp
        cp ${current_example}_$platform.workspace $current_example.newworkspace
        rm *.cbp *.workspace
        mv $current_example.newcbp $current_example.cbp
        mv $current_example.newworkspace $current_example.workspace
        sed -i s/${example_name}_${platform}/${example_name}/g $current_example.workspace

        #delete makefiles on windows
        if [ "$platform" = "win_cb" ]; then
            rm makefile cb_build_runner.sh *.make
        fi

        #delete osx files
        rm -Rf *.xcodeproj
        rm openFrameworks-Info.plist

        #delete vs files
        rm *.sln
        rm *.vcproj
        rm *.vcproj.user
        
        #delete OFLib project files
        if [ "$platform" = "win_cb" ]; then
            rm -Rf ${oflib_root}/*/linux
            rm -Rf ${oflib_root}/*/linux64
        else
            rm -Rf ${oflib_root}/*/win_cb
        fi
        
        if [ "$platform" = "linux" ]; then
            rm -Rf ${oflib_root}/*/linux64
        elif [ "$plaform" = "linux64" ]; then
            rm -Rf ${oflib_root}/*/linux
        fi
        rm -Rf ${oflib_root}/*/osx
        rm -Rf ${oflib_root}/*/vs2008
        rm -Rf ${oflib_root}/*/iphone
        
    fi



    #osx
    if [ "$platform" = "osx" ]; then

        #delete codeblock files
        rm *.cbp
        rm *.sh
        rm *.workspace

        #delete makefiles
        rm makefile
        rm *.make

        #delete vs files
        rm *.sln
        rm *.vcproj
        rm *.vcproj.user
        
        #delete OFLib project files
        rm -Rf ${oflib_root}/*/vs2008
        rm -Rf ${oflib_root}/*/linux
        rm -Rf ${oflib_root}/*/linux64
        rm -Rf ${oflib_root}/*/win_cb
        rm -Rf ${oflib_root}/*/iphone

    fi



    #visual studio
    if [ "$platform" = "vs2008" ]; then

        #delete codeblock files
        rm *.cbp
        rm *.sh
        rm *.workspace

        #delete makefiles
        rm makefile
        rm *.make

        #delete osx files
        rm -Rf *.xcodeproj
        rm openFrameworks-Info.plist
        
        #delete OFLib project files
        rm -Rf ${oflib_root}/*/osx
        rm -Rf ${oflib_root}/*/linux
        rm -Rf ${oflib_root}/*/linux64
        rm -Rf ${oflib_root}/*/win_cb
        rm -Rf ${oflib_root}/*/iphone
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

    #delete other platform libraries
    if [ "$pkg_platform" = "linux" ]; then
        otherplatforms="linux64 osx win_cb vs2008 iphone"
    fi

    if [ "$pkg_platform" = "linux64" ]; then
        otherplatforms="linux osx win_cb vs2008 iphone"
    fi

    if [ "$pkg_platform" = "osx" ]; then
        otherplatforms="linux linux64 win_cb vs2008 iphone"
    fi

    if [ "$pkg_platform" = "win_cb" ]; then
        otherplatforms="linux linux64 osx vs2008 iphone"
    fi

    if [ "$pkg_platform" = "vs2008" ]; then
        otherplatforms="linux linux64 osx win_cb iphone"
    fi

    if [ "$pkg_platform" = "iphone" ]; then
        otherplatforms="linux linux64 osx win_cb vs2008"
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
    elif [ "$pkg_platform" = "win_cb" ] || [ "$pkg_platform" = "vs2008" ]; then
        rm -Rf $libsnotinwindows
    fi
    
    cd ${pkg_ofroot}/addons
    for lib in $( ls */libs/*/lib/  -d )
    do
        cd ${lib}
        echo $PWD
        echo deleting $lib
        rm -Rf $otherplatforms
        cd $pkg_ofroot/addons
    done

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
    	rm -Rf win_cb vs2008 osx iphone
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

	#download and copy OF compiled
	cd $pkg_ofroot/libs/openFrameworksCompiled/lib/${pkg_platform}
	if [ "$pkg_platform" = "linux" ] || [ "$pkg_platform" = "linux64" ]; then
		wget http://openframeworks.cc/git_pkgs/OF_compiled/${pkg_platform}/libopenFrameworks.a
		wget http://openframeworks.cc/git_pkgs/OF_compiled/${pkg_platform}/libopenFrameworksDebug.a
	elif [ "$pkg_platform" = "win_cb" ]; then
		wget http://openframeworks.cc/git_pkgs/OF_compiled/${pkg_platform}/openFrameworks.lib
		wget http://openframeworks.cc/git_pkgs/OF_compiled/${pkg_platform}/openFrameworksDebug.lib
	elif [ "$pkg_platform" = "vs2008" ]; then
		wget http://www.openframeworks.cc/git_pkgs/OF_compiled/${pkg_platform}/openframeworksLib.lib
		wget http://www.openframeworks.cc/git_pkgs/OF_compiled/${pkg_platform}/openframeworksLibDebug.lib
	elif [ "$pkg_platform" = "osx" ]; then
		if [ $runOSXSLScript = 1 ]; then
			wget http://openframeworks.cc/git_pkgs/OF_compiled/osxSL/openFrameworks.a
			wget http://openframeworks.cc/git_pkgs/OF_compiled/osxSL/openFrameworksDebug.a
			wget http://openframeworks.cc/git_pkgs/OF_compiled/osxSL/openFrameworksUniversal.a
		else
			wget http://openframeworks.cc/git_pkgs/OF_compiled/osx/openFrameworks.a
			wget http://openframeworks.cc/git_pkgs/OF_compiled/osx/openFrameworksDebug.a
			wget http://openframeworks.cc/git_pkgs/OF_compiled/osx/openFrameworksUniversal.a
		fi
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
    
    rm readme.*
    mv readme readme.txt

    #create compressed package
    cd $pkg_ofroot/..
    if [ "$pkg_platform" = "linux" ] || [ "$pkg_platform" = "linux64" ]; then
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
    for eachplatform in win_cb linux linux64 vs2008 osx 
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

cd $pacakgeroot/.. 
    
