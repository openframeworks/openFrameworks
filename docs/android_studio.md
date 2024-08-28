[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

Android Studio
=======

The Android distribution of openFrameworks is setup to work with Android Studio IDE.

## Installation

Setting up openFrameworks with Android Studio is fairly straightforward. The basic steps are:

- Install the latest version of [Android Studio](https://developer.android.com/studio/index.html) (tested with version 3.0).
- Download openFrameworks 0.10.0 or later from the [download page](http://openframeworks.cc/download/) or from [github](https://github.com/openframeworks/openFrameworks/).
- Download and unzip the Android NDK version r15c: [Windows](https://dl.google.com/android/repository/android-ndk-r15c-windows-x86_64.zip), [Linux](https://dl.google.com/android/repository/android-ndk-r15c-linux-x86_64.zip), [OSX](https://dl.google.com/android/repository/android-ndk-r15c-darwin-x86_64.zip). It doesn't matter where you store the library, you will point Android Studio to this location later. *Notice that openFrameworks only works with version r15c!*
- If you got openFrameworks from git: 
	- Run `scripts/android/download_libs.sh` to download libraries.
	- Use the Project Generator to generate Android Studio project files for android examples.
- In Android Studio, use **Open an existing Android Studio project** and select an openFrameworks example from the `examples/android` folder.
- It will ask you for NDK location, either put the path in `local.properties` or click *Project Structure* and select the NDK location.
- Let Android Studio download all other dependencies missing automatically.
- Build and run.

### Build and run

Press the Play button next to the `androidEmptyExample` shown in the toolbar. With any luck, it should build the app and deploy it to your Android device after a few seconds (or prompt you to deploy it to a suitable emulator). If the app runs, congratulations! You have setup openFrameworks.

## Creating new projects

1. Use the Project Generator to generate new projects for Android Studio.
2. Open Android Studio and click **Open an existing Android Studio project**, and point to the newly created project folder.
3. Certain apps need additional permissions, for example access to camera. Add these in the `AndroidManifest.xml` file.

## Troubleshooting

- You may need to adjust the following numbers to match your installed Android Studio and Android SDK. Android Studio should offer to fix these values for you when you open the appropriate build files.

    - The Gradle version specified in `/build.gradle`
    - The `compileSdkVersion`, `buildToolsVersion`, `minSdkVersion`, `targetSdkVersion` values
        in `/addons/ofxAndroid/ofAndroidLib/build.gradle` and `/apps/myApps/androidEmptyExample/build.gradle`

- If you get strange linker errors, make sure you are using NDK version r15c. Newer NDKs might work but usually require some changes.

- If your connected device is not recognized by Android Studio, restart adb
  with `adb kill-server && adb start-server`.

- If, when synchronizing a project for the first time, clicking on the console
  messages does not solve the problem regarding a missing dependency of the
  android API (like for example Android API 22), open the sdk from the console,
   select the missing API and install it.
