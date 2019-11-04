[openFrameworks](http://openframeworks.cc/)
================

openFrameworks is a C++ toolkit for creative coding.  If you are new to OF, welcome!

[![Slack Status](https://ofslack.herokuapp.com/badge.svg)](https://ofslack.herokuapp.com)

## Build status

* The **master** branch contains the newest, most recently updated code. This code is packaged and available for download in the "Nightly Builds" section of [openframeworks.cc/download](openframeworks.cc/download).
* The **stable** branch contains the code corresponding to the last stable openFrameworks release. This stable code is packaged and available for download at [openframeworks.cc/download](openframeworks.cc/download).

Platform                     | Master branch  | Stable branch
-----------------------------|:---------|:---------
Windows MSYS2 32bits         | [![Build status](https://appveyor-matrix-badges.herokuapp.com/repos/arturoc/openFrameworks/branch/master/1)](https://ci.appveyor.com/project/arturoc/openFrameworks/branch/master) | [![Build status](https://appveyor-matrix-badges.herokuapp.com/repos/arturoc/openFrameworks/branch/stable/1)](https://ci.appveyor.com/project/arturoc/openFrameworks/branch/stable)
Windows MSYS2 64bits         | [![Build status](https://appveyor-matrix-badges.herokuapp.com/repos/arturoc/openFrameworks/branch/master/2)](https://ci.appveyor.com/project/arturoc/openFrameworks/branch/master) | N/A
Windows Visual Studio 32bits | [![Build status](https://appveyor-matrix-badges.herokuapp.com/repos/arturoc/openFrameworks/branch/master/3)](https://ci.appveyor.com/project/arturoc/openFrameworks/branch/master) | [![Build status](https://appveyor-matrix-badges.herokuapp.com/repos/arturoc/openFrameworks/branch/stable/2)](https://ci.appveyor.com/project/arturoc/openFrameworks/branch/stable)
Windows Visual Studio 64bits | [![Build status](https://appveyor-matrix-badges.herokuapp.com/repos/arturoc/openFrameworks/branch/master/4)](https://ci.appveyor.com/project/arturoc/openFrameworks/branch/master) | [![Build status](https://appveyor-matrix-badges.herokuapp.com/repos/arturoc/openFrameworks/branch/stable/3)](https://ci.appveyor.com/project/arturoc/openFrameworks/branch/stable)
Linux 64                     | [![Linux 64 Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=TARGET="linux64"&label=build&branch=master)](https://travis-ci.org/openframeworks/openFrameworks) | [![Linux 64 Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=TARGET="linux64"&label=build&branch=stable)](https://travis-ci.org/openframeworks/openFrameworks)
Linux armv6l                 | [![Linux armv6l Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=TARGET="linuxarmv6l"&label=build&branch=master)](https://travis-ci.org/openframeworks/openFrameworks) | [![Linux armv6l Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=TARGET="linuxarmv6l"&label=build&branch=stable)](https://travis-ci.org/openframeworks/openFrameworks)
Linux armv7l                 | [![Linux armv7l Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=TARGET="linuxarmv7l"&label=build&branch=master)](https://travis-ci.org/openframeworks/openFrameworks) | [![Linux armv7l Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=TARGET="linuxarmv7l"&label=build&branch=stable)](https://travis-ci.org/openframeworks/openFrameworks)
Emscripten                   | [![Emscripten Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=TARGET="emscripten"&label=build&branch=master)](https://travis-ci.org/openframeworks/openFrameworks) | [![Emscripten Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=TARGET="emscripten"&label=build&branch=stable)](https://travis-ci.org/openframeworks/openFrameworks)
macos                        | [![macos Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=TARGET="osx"&label=build&branch=master)](https://travis-ci.org/openframeworks/openFrameworks) | [![macos Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=TARGET="osx"&label=build&branch=stable)](https://travis-ci.org/openframeworks/openFrameworks)
macos makefiles              | [![macos makefiles Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=OPT="makefiles"&label=build&branch=master)](https://travis-ci.org/openframeworks/openFrameworks) | [![macos makefiles Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=OPT="makefiles"&label=build&branch=stable)](https://travis-ci.org/openframeworks/openFrameworks)
iOS                          | [![iOS Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=TARGET="ios"&label=build&branch=master)](https://travis-ci.org/openframeworks/openFrameworks) | [![iOS Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=TARGET="ios"&label=build&branch=stable)](https://travis-ci.org/openframeworks/openFrameworks)
tvos                         | [![tvos Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=TARGET="tvos"&label=build&branch=master)](https://travis-ci.org/openframeworks/openFrameworks) | [![tvos Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=TARGET="tvos"&label=build&branch=stable)](https://travis-ci.org/openframeworks/openFrameworks)
Android Arm7                 | [![Android Arm7 Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=GRADLE_TARGET="compileArm7DebugSources"&label=build&branch=master)](https://travis-ci.org/openframeworks/openFrameworks) | [![Android Arm7 Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=GRADLE_TARGET="compileArm7DebugSources"&label=build&branch=stable)](https://travis-ci.org/openframeworks/openFrameworks)
Android X86                  | [![Android X86 Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=GRADLE_TARGET="compileX86DebugSources"&label=build&branch=master)](https://travis-ci.org/openframeworks/openFrameworks) | [![Android X86 Build Status](http://badges.herokuapp.com/travis/openframeworks/openFrameworks?env=GRADLE_TARGET="compileX86DebugSources"&label=build&branch=stable)](https://travis-ci.org/openframeworks/openFrameworks)


## folder structure

This release of OF comes with several folders:

* addons
* apps
* docs
* examples
* export (on some systems)
* libs
* other
* scripts
* project generator


`docs` has some documentation around OF usage, per platform things to consider, etc. You should definitely take a look in there; for example, if you are on OSX, read the osx.md.   `apps` and `examples` are where projects go -- `examples` contains a variety of projects that show you how to use OF, and `apps` is where your own projects will go.  `libs` contains the libraries that OF uses, including the openframeworks core itself.  `addons` are for additional functionality that's not part of the core.  `export` is for DLLs and dylibs that need to be put in each compiled project.  The `scripts` folder has the templates and small scripts for automating OF per platform. `project generator` is a GUI based tool for making new projects - this folder is only there in packaged releases.  

One idea that's important is that OF releases are designed to be self-contained.  You can put them anywhere on your hard drive, but it's not possible to mix different releases of OF together, so please keep each release (0.8.0, 0.8.1) separate.  Projects may generally work from release to release, but this is not guaranteed.  Because OF is self-contained, there's extensive use of local file paths (ie, ../../../) throughout OF.  It's important to be aware of how directories are structured.  A common error is to take a project and move it so that it's a level below or above where it used to be compared to the root of OF.  This means that links such as ../../../libs will break.  

## Get involved

The openframeworks forum:

[http://forum.openframeworks.cc/](http://forum.openframeworks.cc/)

is a warm and friendly place.  Please ask or answer a question.  The most important part of this project is that it's a community, more than just a tool, so please join us!  Also, this is free software, and we learn so much about what is hard, what doesn't make sense, what is useful, etc. The most basic questions are acceptable here!  Don't worry, just join the conversation.  Learning in OF is social, it's hard to do it alone, but together we can get far!

Our GitHub site is active:

[https://github.com/openframeworks/openFrameworks](https://github.com/openframeworks/openFrameworks)

if you have bugs or feature requests, consider opening an issue.  If you are a developer and want to help, pull requests are warmly welcome.  Please read the contributing guide for guidelines:

[https://github.com/openframeworks/openFrameworks/blob/master/CONTRIBUTING.md](https://github.com/openframeworks/openFrameworks/blob/master/CONTRIBUTING.md
)

We also have a developer's mailing list, which is useful for discussing issues around the development and future of OF.

## Developers

To grab a copy of openFrameworks for your platform, check the [download page](http://openframeworks.cc/download) on the main site.  

If you are working with the Git repository, the `stable` branch of the OF repository corresponds to the most recent release, with a few important differences:  

1. The release includes a simple openFrameworks project generator.
2. This GitHub repository contains code and libs for all the platforms, but the releases are done on a per-platform basis.
3. This GitHub repository has no project files for the different examples. They are generated automatically for each release using a tool in `apps/projectGenerator/`.
4. There are no external dependencies in this repository, you can download them using the download_libs.sh script for each platform in the particular platform folder inside scripts.

If you want to work with the openFrameworks GitHub repository, you need to download the external dependencies and you should use the project generator to create project files for all the code in `examples/`.  To generate the project files with the project generator enable the 'Advanced Options' in the settings tab, then use 'Update Multiple' to update the projects for the `examples/` folder path in the repo.

To set up the project generator submodule within the OF repo, use the command `git submodule init` then `git submodule update` whilst inside the openFrameworks repo.

For more info on working with the Project Generator, for per-platform readmes, and more information, see the [documentation](docs/table_of_contents.md).

## Versioning

openFrameworks uses [Semantic Versioning](http://semver.org/), although strict adherence will only come into effect at version 1.0.0.
