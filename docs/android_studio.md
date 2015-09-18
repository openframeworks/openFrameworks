[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

Android
=======

The Android distribution of openFrameworks is setup to work with either the Eclipse IDE or the newer Android Studio IDE. The projects are currently using a custom toolchain based on Makefiles to compile and install applications.

# Android Studio

Android studio support in openFrameworks is still experimental. The latest version of android studio still doesn't have support for c++ development so even if it's deprecated we still recomend to use eclipse and the ADT plugin. See android-eclipse.md in the docs folder or the corresponding guide in the download page.

## Summary

Setting up openFrameworks with Android Studio is fairly straightforward. The basic steps are:

- Install Android Studio and the Android SDK
- Install the Android NDK (r10b or lower)
- Download openFrameworks from the download page or from git
- In Android Studio, use **File ➞ Import Project** and select the `settings.gradle` file in the root of the openFrameworks directory
- Set the path to the NDK in local.properties (`ndk.dir`)
- Build and run

## Installation

### Install Android Studio and the SDK

Download and install Android Studio from http://tools.android.com/download/studio/stable (you need Android Studio 1.0 or higher). It should come with (or automatically install) a recent SDK, though you can customize the SDK version later from within Android Studio using **Tools ➞ Android ➞ SDK Manager**.

In the event that Android Studio does not come with an SDK, you can install it from http://developer.android.com/sdk/index.html (under "Get the SDK for an existing IDE").

### Install the Android NDK

Download and Install the latest Android NDK for your plattform. Last known working version is r10e.
https://developer.android.com/ndk/downloads/index.html

On Windows, you will also need to install MinGW in order to build openFrameworks. MinGW provides some essential build tools which are not included in the NDK. Follow just the "Installing the MinGW and Msys" instructions on this page: http://www.multigesture.net/articles/how-to-install-mingw-msys-and-eclipse-on-windows/.


### Download openFrameworks 0.9.0 or later

Download it from the downloads page:

http://openframeworks.cc/download

You may also check out the openFrameworks source from GitHub (under master branch): http://github.com/openframeworks/openFrameworks

Or download the latest nightly:
http://openframeworks.cc/download (bottom of the page)

### Import the project

At the Android Studio welcome screen select **Import Project**, or use the **File ➞ Import Project** menu item. Browse to  the `settings.gradle` file in the `libs/openFrameworksCompiled/project/android` directory. Accept all the prompts and wait for Android Studio to set up the project.

At this point you will get an error about the NDK not being configured. Onto the next step...

### Configure the NDK

Edit the file `local.properties` and add a line like this:

    ndk.dir=/path/to/the/ndk

Save and resync the project (either press the "Try Again" link on the "Gradle project sync failed" banner, or use **Tools ➞ Android ➞ Sync Project with Gradle Files**).

You'll have to wait a bit: the first sync will automatically build openFrameworks. If it doesn't work (Gradle sync still fails), try looking at the Troubleshooting tips.  In some cases, clicking on the error in the console window will take you to a solution, such as installing various versions of the Android SDK.  In some cases, you may have to do this several times to solve several missing dependencies.

### Build and run

Press the Play button next to the `androidEmptyExample` shown in the toolbar. With any luck, it should build the app, and momentarily deploy it to your Android device (or prompt you to deploy it on a suitable emulator). If the app runs, congratulations! You have setup openFrameworks.

## Creating new projects

1. Copy the provided example app (make sure to put it in a subdirectory of `apps`, at the same level as the sample)
2. Double-click on `build.gradle` and press "Add Now..."
3. If that doesn't work, add a line to `settings.gradle` including your project manually.
4. Perform a project sync (it should prompt you to do this).

## Creating projects from examples

1. Copy the `build.gradle` file from `androidEmptyExample` into the directory of the example you want to use.
2. Follow steps 2-4 above.

## Troubleshooting

- You may need to adjust the following numbers to match your installed Android Studio and Android SDK. Android Studio should offer to fix these values for you when you open the appropriate build files.

    - The Gradle version specified in `/build.gradle`
    - The `compileSdkVersion`, `buildToolsVersion`, `minSdkVersion`, `targetSdkVersion` values
        in `/addons/ofxAndroid/ofAndroidLib/build.gradle` and `/apps/myApps/androidEmptyExample/build.gradle`

- If you get strange linker errors (e.g. errors about a missing `__srget`), try using the r9d version of the NDK. Newer NDKs (particularly r10c and up) don't work with some versions of OpenFrameworks.
- If you get strange linker errors when running the Android Simulator (e.g. `java.lang.UnsatisfiedLinkError: dalvik.system.PathClassLoader[DexPathList`) make sure that your simulator is using the `armeabi-v7a`, not the `x86` or `x86_64` architecture.  You can check your setup by opening your Android Virtual Device Manager and editing the device (**Tools ➞ Android ➞ AVD Manager** or click the AVD Manager icon in the toolbar).  You may be prompted to download an appropriate system image for `armeabi-v7a`.

