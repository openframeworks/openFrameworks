# $1 -> platform: win_cb, linux, linux64, vs2008, macosx, iphone, all
# $2 -> version number: 006

REPO=git://github.com/openframeworks/openFrameworks.git
BRANCH=master

libsnotinmac="unicap gstappsink glu glut quicktime videoInput"
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
git pull origin $BRANCH


packageroot=$PWD

function deleteProjectFiles {
    #codeblocks
    if [ "$1" = "linux" ] || [ "$1" = "linux64" ] || [ "$1" = "win_cb" ]; then 
        cd $3
        cp ${3}_$1.cbp $example.newcbp
        rm *.cbp
        mv $3.newcbp $3.cbp

        #delete makefiles on windows
        if [ "$1" = "win_cb" ]; then
            rm Makefile_*
        else
            mv Makefile_$1 Makefile
            rm Makefile_*
        fi

        #delete macosx files
        rm -Rf openFrameworks.xcodeproj
        rm openFrameworks-Info.plist

        #delete vs files
        rm *.sln
        rm *.vcproj
        rm *.vcproj.user

    fi



    #macosx
    if [ "$1" = "macosx" ]; then
        cd $3

        #delete codeblock files
        rm *.cbp
        rm *.sh

        #delete makefiles
        rm Makefile_*

        #delete vs files
        rm *.sln
        rm *.vcproj
        rm *.vcproj.user

    fi



    #visual studio
    if [ "$1" = "vs2008" ]; then
        cd $3

        #delete codeblock files
        rm *.cbp
        rm *.sh

        #delete makefiles
        rm Makefile_*

        #delete macosx files
        rm -Rf openFrameworks.xcodeproj
        rm openFrameworks-Info.plist

    fi
}

function createPackage {
    #delete other platforms example project files
    cd $3/apps/examples

    for example in $( ls . )
    do
        deleteProjectFiles $1 $2 $example
        cd $3/apps/examples
    done


    #delete other platforms addons examples project files
    cd $3/apps/addonsExamples

    for example in $( ls . )
    do
        deleteProjectFiles $1 $2 $example
        cd $3/apps/addonsExamples
    done

    #delete other platform libraries
    if [ "$1" = "linux" ]; then
        otherplatforms="linux64 macosx win_cb win_vs2008 iphone"
    fi

    if [ "$1" = "linux64" ]; then
        otherplatforms="linux macosx win_cb win_vs2008 iphone"
    fi

    if [ "$1" = "macosx" ]; then
        otherplatforms="linux linux64 win_cb win_vs2008 iphone"
    fi

    if [ "$1" = "win_cb" ]; then
        otherplatforms="linux linux64 macosx win_vs2008 iphone"
    fi

    if [ "$1" = "vs2008" ]; then
        otherplatforms="linux linux64 macosx win_cb iphone"
    fi

    if [ "$1" = "iphone" ]; then
        otherplatforms="linux linux64 macosx win_cb win_vs2008"
    fi

    #delete libraries for other platforms
    cd $3/libs  
    for lib in $( ls . )
    do
        echo $PWD
        echo deleting $lib/lib
        cd $lib/lib
        rm -Rf $otherplatforms
        cd $3/libs 
    done
    if [ "$1" = "macosx"]; then
        rm -Rf $libsnotinmac
    elif [ "$1" = "linux" ] || [ "$1" = "linux64" ]; then
        rm -Rf $libsnotinlinux
    elif [ "$1" = "win_cb" ] || [ "$1" = "win_vs2008" ]; then
        rm -Rf $libsnotinwindows
    fi

    #delete dynamic libraries for other platforms
    cd $3/export
    rm -Rf $otherplatforms
    if [ "$1" = "macosx" ]; then
        cd ..
        rmdir export
    fi

    #delete scripts
    cd $3/scripts
    if [ "$1" = "linux" ] || [ "$1" = "linux64" ]; then
        rm -Rf win_cb
    elif [ "$1" = "win_cb" ]; then
        rm -Rf linux
    else
        rm -Rf *
    fi
    rm create_package.sh

    #delete .svn dirs
    cd $3
    rm -Rf `find . -type d -name .svn`

    #create compressed package
    cd $3/..
    if [ "$1" = "linux" ] || [ "$1" = "linux64" ]; then
        mkdir of_preRelease_v$2_$1
        mv openFrameworks/* of_preRelease_v$2_$1
        tar czf of_preRelease_v$2_$1_FAT.tar.gz of_preRelease_v$2_$1
        rm -Rf of_preRelease_v${2}_${1}/addons of_preRelease_v${2}_${1}/apps/addonsExamples
        tar czf of_preRelease_v$2_$1.tar.gz of_preRelease_v$2_$1
        rm -Rf of_preRelease_v${2}_${1}
    else
        mkdir of_preRelease_v$2_$1
        mv openFrameworks/* of_preRelease_v$2_$1
        zip -r of_preRelease_v$2_$1_FAT.zip of_preRelease_v$2_$1
        rm -Rf of_preRelease_v${2}_${1}/addons of_preRelease_v${2}_${1}/apps/addonsExamples
        zip -r of_preRelease_v$2_$1.zip of_preRelease_v$2_$1
        rm -Rf of_preRelease_v${2}_${1}
    fi
}



if [ "$1" = "all" ]; then
    for platform in win_cb linux linux64 vs2008 macosx 
    do
        cd $packageroot
        mkdir of_preRelease_v${2}_${platform}
        cp -R addons apps export libs other scripts of_preRelease_v${2}_${platform}
        cd of_preRelease_v${2}_${platform}
        createPackage $platform $2 $PWD
    done
    
    cd $packageroot
    echo dir: $PWD
    mkdir of_preRelease_v${2}_all
    mv addons apps export libs other scripts $packageroot/of_preRelease_v${2}_all

    tar czf of_preRelease_v$2_all_FAT.tar.gz of_preRelease_v${2}_all
    rm -Rf of_preRelease_v${2}_all/addons of_preRelease_v${2}_all/apps/addonsExamples
    tar czf of_preRelease_v$2_all.tar.gz of_preRelease_v$2_all
    rm -Rf of_preRelease_v${2}_all
    mv * $packageroot/..
    #rm -Rf $packageroot
else
    createPackage $1 $2 $packageroot
fi

cd $packageroot
git reset --hard

cd $pacakgeroot/.. 
    
