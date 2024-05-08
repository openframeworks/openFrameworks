This is a fork of **Dan Rosser (@danoli3)** own fork of openFrameworks [https://github.com/danoli3/openFrameworks](https://github.com/danoli3/openFrameworks) that I've used to experiment with OF on Android with **modern Android tools**. Dan has indeed rearchitectured the way the OF build for Android can be done.

Default branch is **Android_fixes_thierry**, starting from Dan's **Android_fixes**.

I added :
- a few corrections and completions
- a composite example under (examples/Android) which works on my Android phone 
- the binaries of the initial libs used by OF (boost, freeimage, openCV...)

I have added these binaries for convenience because current **download_libs.sh** under **scripts/android** does not fully work and I had to manually include some the binaries in the right places with the right names. That will need to be fixed.

The composite example includes ofShaderExample, ofSoundPlayerExample, ofSoundStreamExample. It was constructed starting by **manually copying** the following project : **scripts/template/android** under example/android. **ProjectGenerator was not used.**

To make a build, start by **building the openFrameworks lib** for Android by :
- opening the following project in Android Studio : **libs/openFrameworksCompiled/project/android**
- gradle starts to configure the project
- launch **Build / make module android.openframeworksAndroid**
- if gradle stops asking for another version of Gradle JDK do as stated below and sync
- rebuild
- **libopenFrameworksAndroid.so binaries** are created for **arm64-v8a  -  armeabi-v7a  -  x86** in **libs/openFrameworksCompiled/lib/android**

Changing the version of the Gradle JDK : in **Settings / Build, Execution, Deployment / Build Tools / Gradle** (I use the JetBrains 17.0.6 version).

It is then possible to build the example and generate the corresponding APK / bundle by :
- opening  **examples/android/androidCompositeExample** in Android Studio
- in the Android view : select **build / make module androidCompositeExample.ofApp** 
- then launch **build / build bundle / apk / your choice** to **generate an apk or a bundle** in **ofApp/build/output**
- with a connected device you can upload and lauch the app


The version of Android Studio that I have used is **Android Studio Flamingo | 2022.2.1 Patch 1**

That was tested on a Xiaomi MI lite 10 from 2020, runnning **Android 10, SDK 29**.


[openFrameworks](http://openframeworks.cc/)
================

openFrameworks is a C++ toolkit for creative coding.  If you are new to OF, welcome!

- [Grab the Nightly Release (to avoid submodule cloning)](https://github.com/openframeworks/openFrameworks/releases)
- [Setup guides for different Platforms](https://openframeworks.cc/download/)
- [Join Our Slack](https://join.slack.com/t/openframeworks/shared_invite/zt-1r2brqms0-dZMMFZgZhFTgomjJ0vlCjA)
- [Discuss on the Forum](https://forum.openframeworks.cc) 
- [Follow OF on Mastodon](https://fosstodon.org/@openframeworks)

## Build status

* The **master** branch contains the newest, most recently updated code. This code is packaged and available for download in the "Nightly Builds" section of [openframeworks.cc/download](https://openframeworks.cc/download/).
* The **stable** branch contains the code corresponding to the last stable openFrameworks release. This stable code is packaged and available for download at [openframeworks.cc/download](https://openframeworks.cc/download/).

Platform                     | Master branch  | Stable branch
-----------------------------|:---------|:---------
Windows MSYS2        | [![Build status](https://github.com/openframeworks/openFrameworks/workflows/build-msys2/badge.svg)](https://github.com/openframeworks/openFrameworks/actions) | [![Build status](https://github.com/openframeworks/openFrameworks/workflows/build-msys2/badge.svg?branch=stable)](https://github.com/openframeworks/openFrameworks/actions)
Windows Visual Studio  | [![Build status](https://github.com/openframeworks/openFrameworks/workflows/build-vs/badge.svg)](https://github.com/openframeworks/openFrameworks/actions) | [![Build status](https://github.com/openframeworks/openFrameworks/workflows/build-vs/badge.svg?branch=stable)](https://github.com/openframeworks/openFrameworks/actions)
Linux 64 & Arm                    | [![Linux Build Status](https://github.com/openframeworks/openFrameworks/workflows/build-linux64-and-arm/badge.svg)](https://github.com/openframeworks/openFrameworks/actions) | [![Linux Build Status](https://github.com/openframeworks/openFrameworks/workflows/build-linux64-and-arm/badge.svg?branch=stable)](https://github.com/openframeworks/openFrameworks/actions)
Emscripten                   | [![Emscripten Build Status](https://github.com/openframeworks/openFrameworks/workflows/build-emscripten/badge.svg)](https://github.com/openframeworks/openFrameworks/actions) | [![Emscripten Build Status](https://github.com/openframeworks/openFrameworks/workflows/build-emscripten/badge.svg?branch=stable)](https://github.com/openframeworks/openFrameworks/actions) 
macos                        | [![macos Build Status](https://github.com/openframeworks/openFrameworks/workflows/build-macos/badge.svg)](https://github.com/openframeworks/openFrameworks/actions) | [![macos Build Status](https://github.com/openframeworks/openFrameworks/workflows/build-macos/badge.svg?branch=stable)](https://github.com/openframeworks/openFrameworks/actions)
iOS & tvOS                         | [![iOS tvOS Build Status](https://github.com/openframeworks/openFrameworks/workflows/build-ios-tvos/badge.svg)](https://github.com/openframeworks/openFrameworks/actions) | [![iOS tvOS Build Status](https://github.com/openframeworks/openFrameworks/workflows/build-ios-tvos/badge.svg?branch=stable)](https://github.com/openframeworks/openFrameworks/actions)

<!-- Android Arm7                 | [![Android Build Status](https://github.com/openframeworks/openFrameworks/workflows/build-android/badge.svg)](https://github.com/openframeworks/openFrameworks/actions) | [![Android Build Status](https://github.com/openframeworks/openFrameworks/workflows/build-android/badge.svg?branch=stable)](https://github.com/openframeworks/openFrameworks/actions) -->



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
* projectGenerator


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
