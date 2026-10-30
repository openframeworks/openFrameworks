[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

Android Studio 2025
=======

The Android distribution of openFrameworks is setup to work with Android Studio IDE.

## Installation

Setting up openFrameworks with Android Studio is fairly straightforward. The basic steps are:

- Install the latest version of [Android Studio](https://developer.android.com/studio/index.html) (tested with Android Studio Meerkat | 2024.3.1).

###  Select the following to be installed in SDK Manager (options)
- Android SDK Platform 34. The template compiles and targets API 34. Install a newer platform as well if an example needs it.
- Build Tools 37.0.0
- NDK r30, side by side package `30.0.16248370` (SDK Manager: NDK (Side by side))
- Android SDK Command Line Tools 19.0 (or latest)
- CMake 3.31.6 (or latest)

## Install openFrameworks
- Download openFrameworks 0.12.1 or later from the [download page](http://openframeworks.cc/download/) or from [github](https://github.com/openframeworks/openFrameworks/).
- If you got openFrameworks from git:
	- Run `scripts/android/download_libs.sh` to download the current apothecary `latest` Android libraries (arm64, armv7, and x86_64).
	- Use the Project Generator to generate Android Studio project files for android examples.
- In Android Studio, use **Open an existing Android Studio project** and select an openFrameworks example from the `examples/android` folder.
- It will ask you for NDK location, either put the path in `local.properties` or click *Project Structure* and select the NDK location.
- Let Android Studio download all other dependencies missing automatically.
- Build and run.

### Build and run

Press the Play button next to the `androidEmptyExample` shown in the toolbar. It will build the app and deploy it to your Android device after a few seconds (or prompt you to deploy it to a suitable emulator). If the app runs, congratulations! You have setup openFrameworks.

### Building directly with Gradle
- Can build directly in folder with ./gradlew assembleDebug
- System will need latest version of JDK for this https://www.oracle.com/us/java/technologies/downloads/#jdk23-mac

## Creating new projects

1. Use the Project Generator to generate new projects for Android Studio.
2. Open Android Studio and click **Open an existing Android Studio project**, and point to the newly created project folder.
3. Certain apps need additional permissions, for example access to camera. Add these in the `AndroidManifest.xml` file.

## Troubleshooting

- You may need to adjust the following numbers to match your installed Android Studio and Android SDK. Android Studio should offer to fix these values for you when you open the appropriate build files.

    - The Gradle version specified in `/build.gradle`
    - The `compileSdkVersion`, `buildToolsVersion`, `minSdkVersion`, `targetSdkVersion` values
        in `/addons/ofxAndroid/ofAndroidLib/build.gradle` and `/apps/myApps/androidEmptyExample/build.gradle`

- If you get strange linker errors, check that Android Studio is using NDK `30.0.16248370`. The template, the core Android project, and the GitHub Android job all pin that version. An older side-by-side NDK will not match this tree.

- If your connected device is not recognized by Android Studio, restart adb
  with `adb kill-server && adb start-server`.

- If, when synchronizing a project for the first time, clicking on the console
  messages does not solve the problem regarding a missing dependency of the
  android API (like for example Android API 22), open the sdk from the console,
   select the missing API and install it.

## Rabbit R1

The Rabbit R1 is a small arm64 device with a 320×240 touchscreen and a MediaTek Helio P35. A community Android 13 image can install a normal openFrameworks APK. The stock Rabbit OS is not a build target.

- Generate an example with the Project Generator, then in `ofApp/build.gradle` keep `ndkVersion '30.0.16248370'` and set both `ndk.abiFilters` lines to `'arm64-v8a'`.
- `minSdkVersion 25` is below Android 13, so the package installs.
- Build with `./gradlew assembleDebug`. The debug package is `ofApp-playstore-debug.apk`.
- Sideload with `adb install -r` and that APK. Touch events reach openFrameworks. The scroll wheel does not act as a mouse, and layouts written for a phone will be tight on 320×240.
