[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

Android Studio
=======

The Android distribution of openFrameworks is setup to work with Android Studio IDE.

## Installation

Setting up openFrameworks with Android Studio is fairly straightforward. The basic steps are:

- Install latest version of [Android Studio](https://developer.android.com/studio/index.html) (Tested on version 2.2)
- Download openFrameworks 0.10.0 or later from the [download page](http://openframeworks.cc/download/) or from [github](https://github.com/openframeworks/openFrameworks/)
- Download and unzip the Android NDK version r12b: [Windows](https://dl.google.com/android/repository/android-ndk-r12b-windows-x86_64.zip), [Linux](https://dl.google.com/android/repository/android-ndk-r12b-linux-x86_64.zip), [OSX](https://dl.google.com/android/repository/android-ndk-r12b-darwin-x86_64.zip)
- If you got openFrameworks from git: 
	- run `scripts/android/download_libs.sh` to download libraries.
	- Use project generator to generate Android Studio project files for android examples.
- In Android Studio, use **Open an existing Android Studio project** and select an openFrameworks example from the `examples/android` folder
- It will ask you for NDK location, input the location of NDK you downloaded. Notice: Do not select to automatically download the NDK, since the latest version might not work with openFrameworks. If you by accident select to download it automatically, click the *Project Structure* button in the toolbar in the top, and manually change the location.
- Let Android Studio download all other dependencies missing automatically
- Build and run

### Build and run

Press the Play button next to the `androidEmptyExample` shown in the toolbar. With any luck, it should build the app, and momentarily deploy it to your Android device (or prompt you to deploy it on a suitable emulator). If the app runs, congratulations! You have setup openFrameworks.

## Creating new projects
1. Use project generator to generate new projects for Android Studio
2. Open Android Studio and click **Open an existing Android Studio project**, and point to the newly created project folder
3. Certain apps need additional permissions, for example access to camera, add these in the AndroidManifest.xml file

## Troubleshooting

- You may need to adjust the following numbers to match your installed Android Studio and Android SDK. Android Studio should offer to fix these values for you when you open the appropriate build files.

    - The Gradle version specified in `/build.gradle`
    - The `compileSdkVersion`, `buildToolsVersion`, `minSdkVersion`, `targetSdkVersion` values
        in `/addons/ofxAndroid/ofAndroidLib/build.gradle` and `/apps/myApps/androidEmptyExample/build.gradle`

- If you get strange linker errors, check that you are using the 12b version of the NDK. Newer NDKs might work but usually there's some always some fixes that need to be done when moving to a new NDK version.

- If your connected device is not recognized by Android Studio, restart adb
  with `adb kill-server && adb start-server`

- If, when syncronizing a project for the first time, clicking on the console
  messages does not solves the problem regarding a missing dependency of the
  android API (like for example Android API 22), open the sdk from the console,
   select the missing API and install it