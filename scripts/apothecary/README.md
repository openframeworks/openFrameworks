Apothecary
==========

This is the [OpenFrameworks](http://openframeworks.cc) library apothecary. It mixes formulas and potions to build and update the C/C++ lib dependencies.

It's basically a simple build system using Bash scripting inspired by [Homebrew](http://mxcl.github.io/homebrew/). Individual libraries are separated into formula scripts which abstract the download, build, copy, and clean actions to increase flexibility and maintainability. The main apothecary script processes these formulas, runs the appropriate actions, and puts things "in the right place".

One advantage to this approach is that the libraries and/or library binaries can be removed from the OF core repository and only built for the distribution zip packages. Git users would simply need to build all the libraries after doing a clone, which is not that much of a stretch since they already have to generate project files using the OF Project Generator.

2014 OpenFrameworks team  
2013 Dan Wilcox <danomatika@gmail.com> supported by the CMU [Studio for Creative Inquiry](http://studioforcreativeinquiry.org/)

**<span style="color: #F00">This is currently experimental. See the [Apothecary Lib Support spreadsheet](https://docs.google.com/spreadsheet/ccc?key=0Ao8N9g3ekXTydGtzZUxya0IxMTNkdUNiT05laTJieXc&usp=sharing) for current status of the apothecary lib formulas.</span>**

Requirements
------------

* Bash shell environment with: curl, tar, unzip, patch, sed, Make, automake
* C/C++ compiler: gcc and/or llvm (OSX)
* [CMake](http://www.cmake.org)
* git
* Xcode and the iOS SDK (to compile for iOS)
* Android SDK (to compile for android)

Mac OSX & Linux users already have a Bash environment installed. Windows users are recommended to use Git Bash that comes with [Git for Windows](http://windows.github.com) or [MSysGit](http://msysgit.github.io) or the MinGW shell included with [Codeblocks+MiniGW](http://www.codeblocks.org/downloads/26).

Quick Start
----------

*Note: Dirs are relative to base OF dir path*

Core OF formulas dir : `scripts/apothecary/formulas`<br>
Addon formulas dir: `addons/ofxAddonName/scripts/formulas`

Default build dir: `scripts/apothecary/build`<br>
Default Core OF libs dest dir: `libs`

Update a core library:

	apothecary update libName

Update all core libraries:

	apothecary update core

Update an addon:

	apothecary update addonName

Update all add ons:

	apothecary update addons

Update all core libraries for iOS (on OSX only):

	apothecary -t ios update core
	
Update all core lib dependency libraries:

    apothecary update depends

See the built in help for more info:

	apothecary -h

### Commands

* **update**		download, build, and copy library files
* **download** 		download the library source
* **build** 		build the library
* **prepare**		prepare the library source for building
* **copy** 			copy library files into the libs dir
* **clean** 		clean the library build
* **remove**		remove the library from the build cache
* **remove-lib**	remove the library from the libs dir
* **remove-all**	remove the library from the build cache and libs dir

### Options

* **-t**: specify libary type when building, detects type from OS by default
  * valid types: osx, osx-clang-libc++, linux, linux64, vs, msys2, ios, android

* **-a**: specify architecture, either 32 or 64 (default is 32 bit)
  * note: not currently needed, reserved for future use

* **-b**: set the lib build dir, default: $APOTHECARY_DIR/build

* **-d**: set the compiled libs destination dir,
  * default: OF core libs dir or addons/addonName/libs for addons

* **-v**: verbose mode, print out some extra info while mixing formulas

* **-g**: git mode, prefer using git as much as possible

* **-s**: specify a git-tag to select a custom version of the library
  * note: note: availible only with git option

* **-h**: print the usage guide

### Command/Option Order

All options **must** be given before a command or apothecary will return an error.

Detailed Usage
--------------

*Note: Dirs are relative to base OF dir path*

Your interface to the build system is the `apothecary` script:

	apothecary [options] <command> [<library|addons|libName|addonName>]

You can run it from any location, but it's probably easiest to change to this directory:

	cd scripts/apothecary

### Basic

To build libraries, you invoke the apothecary with the library or addon name. The following downloads, builds, and copies poco for the current platform:

	apothecary update poco

The library name is used to locate a formula script. Core OF lib formulas are kept in the apothecary dir `scripts/apothecary/formulas` and the formula script for poco is `poco.sh`.

The name of the formula script file (minus the extension) is used as the name for libs destination folder. Since poco is a formula found in the Core OF lib formulas, it's copied into `libs/poco`.

For addons, you simply give apothecary the addon name and it looks in `scripts/formulas` inside the addon folder. So running the following:

	apothecary update ofxAssimpModelLoader

processes all of the formula scripts in `addons/ofxAssimpModelLoader/scripts/formulas` which, in this case, is only `assimp.sh`. Libs are then installed into `libs` inside the addon folder, so assimp will end up in `addons/ofxAssimpModelLoader/libs/assimp`.

### Lib/Addon Name Lists

apothecary can also accept Core OF lib & addon names in a list:

	apothecary update poco tess2 ofxAssimpModelLoader

Running a command with "core" as the target processes **all** Core OF libs:

	apothecary update core
	
Running a command with "addons" as the target processes **all** addons:

    apothecary update addons

### Script/Directory Paths

apothecary also accepts paths to formula scripts and directories with formula scripts.

For instance, you can run the `poco.sh` formula script directly

	apothecary update formulas/poco.sh

which is the same as running

	apothecary update poco

Some formulas are more involved and require extra files, so apothecary supports formula directories. A formula directory must have a formula script with the same name as the directory, minus the ".sh" extension. 

cairo, for instance, has a number of dependencies which are implemented as formula scripts:

	cairo/cairo.sh <-- main formula script
	cairo/depends/libpng.sh       
	cairo/depends/pixman.sh
	cairo/depends/pkg-config.sh

When running

	apothecary update cairo

apothecary runs `cairo.sh` in `scripts/apothecary/formulas/cairo` which in turn runs apothecary manually on the formula scripts in the `scripts/apothecary/formulas/cairo/depends` dir.

### Manual Paths

By default, apothecary downloads library sources into `scripts/apothecary/build` and copies libs into `libs` while addons are copied into `addons/ofxAddonName`.

You can override these defaults through the **-b** & **-d** options which set the build dir and libs destination dir respectively.

The following

	apothecary -b ~/build -d ~/libs update poco

downloads the poco source into a `build` dir in the user's home folder and copies the built libs into a `libs/poco` dir, again in the user's home folder.

Returning to cairo, the cairo formula script runs apothecary on it's dependency formulas by setting the build dir inside the cairo build dir. This way, removing cairo from the build cache removes all of it's dependencies as well.

### Errors

If any errors occur while running apothecary, it tells you in <span style="color: #F00">red printouts</span> and exits with a return code of 1 while successful command results are (sometimes) printed in <span style="color: #008000">green</span> and return a 0. This should make it easy to drop apothecary into an existing automated build/scripting system.

### Build Types

OpenFrameworks is cross platform and currently supports the following build types:

* osx: Mac OSX, 32 and 64 bit fat libs
* osx-clang-libc++: Mac OSX, clang/libc++ 32 bit libs
* linux: 32 bit linux
* linux64: 64 bit linux
* vs: Windows, Visual Studio
* msys2: Windows, Msys2 (Mingw32 only)
* ios: iOS (can only build on OSX), arm7, arm7s, & 32 bit simulator fat libs
* android: (requires android sdk)

By default, apothecary detects the build type based on your operating system. You can set the build type manually using the **-t** option:

	apothecary -t ios poco

compiles poco for ios.

Compiled lib binaries are copied into subfolders named using the build type, so the previous command copies the static poco ios libs into `libs/poco/lib/ios`.

### Cleaning & Removing

The **clean** command simply cleans the build files in the lib source directory. This can be thought of as the equivalent to the Makefile `make clean`.

The **remove** command deletes the library source folder from the build directory, **remove-lib** command deletes the library from the lib destination folder, and **remove-all** runs both **remove** and **remove-lib**.

Writing Formulas
----------------

Writing a library formula should be relatively straightforward (hopefully). Begin by copying the library template in `scripts/apothecary/doc/formula_template.sh` which is fully commented.

When running a command on a formula, apothecary essentially loads the script into itself (via the Bash source command) and calls the *download*, *build*, *copy*, and *clean* function callbacks. This is the probably not the best way to do it, but it seemed like the easiest in order to greatly simplify the process of writing formulas as paths, etc can be handled for you.

apothecary and it's formulas are written as Bash shell scripts. See this [quick guide to bash shell scripting](http://www.panix.com/~elflord/unix/bash-tute.html) if you're unfamiliar with Bash.

**It's highly recommended to look at existing formula scripts!**

### Filenames & Directories

apothecary is simple and uses the filename (minus extension) of the formula script when search for formulas. That name is then used when copying the libs into the library destination folder. The prevailing OF convention is to use lowercase names.

**All formula scripts must have the ".sh" extension.**

If you have a more advanced/complicated formula (see cairo & FreeImage), you can use a formula directory which is basically a directory that contains a formula script with the same name (plus ".sh" extension). So for cairo, we have `cairo/cairo.sh` and any other dependency scripts/files are contained within the `cairo` dir.

#### Addons

Addon formulas should be placed in a `scripts/formulas` directory inside the add-on directory. For ofxAssimpModelLoader, this is:

    addons/ofxAssimpModelLoader/scripts/formulas
    
When apothecary detects an addon, it sets the lib install directory to the `libs` directory inside the addon and then runs all formulas found in the addon's formal dir.

### Variables

apothecary shares a number of variables with it's formula scripts when running them including

* `$FORMULA_TYPES`: the build types supported by the formula (**Read/Write**)
* `$FORMULA_DEPENDS`: the formulas dependencies (**Read/Write**) 
* `$FORMULA_DEPENDS_MANUAL`: set to 1 to manually handle dependency commands
* `$OS`, `$TYPE`, & `$ARCH`: the current OS, build type, and architecture
* `$APOTHECARY_DIR`: path to the apothecary script dir
* `$APOTHECARY_SCRIPT`: path to the apothecary script itself
* `$FORMULA_DIR`: path to the current formula's parent dir
* `$BUILD_DIR` & `$LIB_DIR`: current build and lib destination die
* `$BUILD_ROOT_DIR`: path to the local build prefix for auto tools projects
* `$DEPENDS_FORMULA_DIR`: path to the dependency formulas dir
* Xcode dev root & OSX/SDK versions

Look inside the apothecary script for more info.

*Note: $FORMULA_TYPES is the only writable variable. Do not write to any of the other variables or things will probably not work.*

The main variables you will probably use are $FORMULA_TYPE and $TYPE. $FORMULA_TYPE is an array of build types supported by the formula and is used by apothecary to ignore formulas if they don't match the current build type. $TYPE is your bread and butter and you'll use it to determine which build commands to run (ex. make versus MSBuild.exe) and which location to copy to based on the build type.

#### Local Variables

Use local variables inside your formula callbacks wherever possible. **Avoid** any global variables beyond what you see in the formula template. The apothecary convention is that local formulas are in camel-case and global variables are all caps.

Here's how to make a local variable (inside functions):

	local someVar=someValue

If you do need to use global variables and/or export variables, make sure to unset them afterwards:

	# set
	GLOBAL_VAR1="ka"
	GLOBAL_VAR2="foo"
	export CFLAGS="-arch x86_64"

	# do a build or something
		
	# cleanup
	unset GLOBAL_VAR1 GLOBAL_VAR2 CFLAGS

### Build Types & Architecture

The (current) default architecture is 32 bit except for:

* osx: i386 / x86_64 fat lib
* linux64: 64 bit
* ios: armv7 / armv7s / i386 (simulator) fat lib

All libraries should be built as static libraries:

* Unix: .a extension
* Windows: .lib extension

### Paths

apothecary changes to the following paths based on the formula function callback so you can execute commands directly:

* download: build dir
* build, copy, clean: lib src dir

### Downloading

Use `curl -O` to download a library source package from a web link. Some links have redirects (SourceForge), so add the `-L` option to tell curl to follow them: `curl -LO`. Unpack the source package (probably using `tar -xf` or `unzip`) and rename it to the formula name as some libs are named using the version number.

For Github projects, simply clone the repository into a folder matching the formula name:

	git clone https://github.com/someuser/somelib somelib

Make sure to delete the downloaded packages (.tar.gz, .zip, etc).

*Note: Favor stable releases / git tags over bleeding edge / master versions!*

The formula template comes with a $VER variable. It's recommended to use that when downloading/unpacking so as to make version updates easier in the future.

**Note**: Download dependencies in the formula's prepare function since download may not be called when using the -g git option.

### Preparing

Prepare the library build system here. This could be running ./autogen.sh to create a configure script or applying custom patches to fix a build for a particular library. You basically want to run things which only nee to be applied once after downloading here.

#### Fixing/Modifying Build Files

Some library build systems may require modification (I'm looking at you, old FreeImage Makefiles!). You might have to do some string replacement to fix bad paths and/or set architecture variables.

For simple things, you can use `sed` which uses regular expressions to match strings in files:

	sed -i tmp "s|some string|replacement string|" path/to/file/to/fix

The **-i tmp** option tells sed to process the file in place and use the ".tmp" extension for a temporary file.

You can also chain replacements using the **-e** option:

	sed -i tmp 
		-e "s|some string|replacement string|"
		-e "s|another string|another replacement|"
		path/to/file/to/fix

See this [sed quick guide](http://www.grymoire.com/Unix/Sed.html) for more info on sed.

For hairier things, you might have to use `diff` and `patch` which sucks and is more involved, but may be easier then trying to fix *alot* of things via sed.

You generally have to do this by unpacking two copies of the library source tree, working in one until you fix things, then creating a diff by comparing that same file in both original and fixed source trees:

	diff -u somelib/oldfile somelib-fixed/newfile > patchfile.patch

Save the patch into your formula directory and then apply it as part of the build process in your formula script:

	# check if patch was applied before patching
	if if patch -p1 -u -N --dry-run --silent < $FORMULA_DIR/patchfile.patch 2>/dev/null ; then
		patch -p1 -u < $FORMULA_DIR/patchfile.patch
	fi

See the [10 Minute Guide to diff and patch](http://stephenjungels.com/jungels.net/articles/diff-patch-ten-minutes.html) and the FreeImage formula for an example of this.

*Note: If you do manage to fix a broken build system, consider submitting your modifications to the library maintainers. Share the Open Source love, y'all.*

### Building

*Note: Every library will probably be different, so consult it's website and readme on how to build it.*

Use the $TYPE variable to determine which commands you need to run when building the library. For example, some library build systems have different makefiles based on OS type.

#### Cleaning when Cross Compiling

If the library can be cross compiled, you may want to make sure to clean the lib build before running another architecture build. For autotools/Makefile build systems, this would be something like:

	make clean; make 

#### Creating Fat Libs for OSX & iOS

For OSX & iOS, libraries need to be built for multiple architectures, then linked together into a "fat lib". 

You can do this by using one of two methods:

1 build as arch1, clean -> build as arch2, clean -> ... -> then link static libs into a fat lib using `lipo`:

	lipo -c lib-arch1.a lib-arch2.a ... -o lib.a

2 build as arch1, arch2, ... simultaneously -> the linking system automatically creates a fat lib

Both methods work, but one may be easier than the other based on the library's build system. For method one, you probably need to copy the built lib file after each build step and rename it based on the architecture or it may be overwritten by the next build.

*Note: `lipo -info` can tell you the architectures currently in a compiled lib: `lipo -info libsomelib.a`*

#### Build Command/Modification Examples

Here's some great script examples to look to when figuring out how to build a library:

* [roxlu_libs](https://github.com/roxlu/roxlu_libs)
* [Homebrew formulas](https://github.com/mxcl/homebrew/tree/master/Library/Formula) (helpful for OSX builds)

### Copying

The copy formula callback function provides 1 argument, $1, which gives you the lib destination folder. For poco for example, this would be `libs/poco`.

By convention, headers go into `include`, source files into `src`, and libs into `lib`.

As with the building, use the $TYPE variable to determine where to copy your compiled libs:

	# headers
	mkdir -p $1/include
	cp -v include/* $1/include

	# lib
	mkdir -p $1/lib/$TYPE
	if [ "$TYPE" == "osx" ] ; then
		cp -v lib/libsomelib.a lib/somelib.a
	
	elif [ $"$TYPE" == "linux" ] ; then
	...

The library is renamed to remove the "lib" prefix, as per the current OF naming conventions.

*Note: Use the verbose copy command `cp -v` for console feedback.*

### Cleaning

Remove build files from the lib source directory here. For example, with autotools/Makefile projects, you usually run

	make clean

For CMake projects, delete the CMakeCache and any build subdirectories you've created:

	rm -f CMakeCache.txt

These are just a few narrow examples. Consult the library build documentation for more info.

### Dependencies

Simple dependency management is handled by separate formulas in the `formulas/_depends` folder. Dependencies can be formula scripts or formula folders as well as existing main OF lib formula (ex: free type).

A formula that has dependencies should list them in the $FORMULA_DEPENDS array and  they will be handled automatically when the main formula is downloaded, prepared, built, etc. The build and copy commands are called on the dependencies before the main formula is built to make sure the dependency libs are installed so the main formula can find them.

Example:

    FORMULA_DEPENDS=( "pkg-config" "freetype" )

Autotools dependency formulas should be written to install (make install) to the $BUILD_ROOT_DIR via --prefix=$BUILD_ROOT_DIR. This way other formulas have a basic install path to set header and lib search directories.

In some situations, you may need to set environment variables etc before building the dependencies. You can tell apothecary you want to manually handle the dependence commands by setting FORMULA_DEPENDS_MANUAL=1. You can then use the following functions within your formula script:

    # do a command on a given dependency in a separate apothecary run
    # $1 = command
    # $2 = dependency name
    function apothecaryDepend()
    
    # do a command on all formula dependencies in separate apothecary runs
    # $1 = command
    function apothecaryDependencies()
    
Example usage:

    apothecaryDepend build pkg-config
    ...
    apothecaryDependencies clean

**Note:** Main OF lib dependencies are not removed by the parent formula as they might have been built separately.

### Debugging, Commenting, & TODO's 

Bash provides an easy way to debug scripts by `set -x`. Put this command in your formula and any command afterward is printed, including the values of environment variables. Very handy.

**PLEASE PLEASE PLEASE comment your formulas!** Writing build scripts sucks! Don't make it even more hell for people that have to try and maintain what you've done. Note *any* assumptions you're making **especially** regarding things that may change based on newer releases of a library.

If you write a formula but can't finish/test it on all platforms, etc make a note of this using the `echoWarning` print command. This is a function provided by apothecary that prints text in <span style="color: #FF0">yellow (caution, right?)</span>. Start your info string with with "TODO: ":

	if [ "$TYPE" == "msys2" ] then ;
		echoWarning "TODO: build msys2"
	else
	...

Then we at least know what we're missing if the build process fails ...

#### Custom Apothecary Echo Wrappers

There are a number of custom echo wrappers like `echoWarning` which set the console color, etc. Use these for more informative info/debugging print instead of plain old `echo`:

    echoError "somethign bad happend, print in red"
	
	echoWarning "something didn't work, print in yellow"
	
	echoInfo "regular print in white"
	
	echoSuccess "something worked, so print in green"
	
	echoVerbose "for debugging, prints extra info in white when the -v verbose flag is used"
	
Note the verbose `-v` flag. Apothecary will already print out info about what steps it's doing so you don't need to add this to the formula files.

### Additional Tips

If you are using a platform that supports `make`, you might find speedups by setting the following environmental variable before using apothecary:

    export MAKEFLAGS="-j8 -s"

Each time `make` is run within the apothecary scripts, `make` will attempt to run 8 jobs (`-j8`), one on each core simultanously, allowing multi-core processors to be leveraged.    `-s` will attempt to silence the output, making it easier to spot apothecary debug messages.


_Happy Scripting!_
