[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

Android
=======

The Android distribution of openFrameworks is setup to work with either the Eclipse IDE or experimentally with the newer Android Studio IDE. The projects are currently using a custom toolchain based on Makefiles to compile and install applications.

This information is also online: http://openframeworks.cc/setup/android-eclipse

**Note**: see the FAQ at the bottom of this page if you're having trouble.

The current version of the Android plugin for Eclipse has several problems with projects that mix C++ and Java code, so the projects are currently using a custom toolchain based on makefiles + Ant tasks to compile and install applications. If you are used to Android development in Eclipse, things are a little different. Check the following instructions to know how to install the development environment and compile/install applications.

Right now this is only tested on Linux and OS X. To use it on Windows, check the instructions on this link: http://www.multigesture.net/articles/how-to-setup-openframeworks-for-android-on-windows/

To use it you will need Eclipse, the Android SDK, the Android NDK, the Android Eclipse plugin and the openFrameworks for Android package.

Because of the custom build system openFrameworks uses for Android, you may need to use the exact version of the SDK and NDK specified here. For this release you should use SDK 21 and NDK r8d. Later versions will probably work but it's not guaranteed.

Summary
-------
These instructions go into a lot of important detail, but the main steps are:

- Install Eclipse, Ant and the Android SDK and NDK.
- If you're using OS X, install the Developer Tools.
- Setup the Android Eclipse plugin.
- Download openFrameworks either from the download page, or clone from git.
- Set path variables so openFrameworks knows where SDK and NDK are.
- Import the openFrameworks projects into Eclipse.
- Compile and install one of the Android openFrameworks examples to confirm that everything works.

Installation
------------

**a) Eclipse**: download the C/C++ edition of Eclipse 4.5 (Mars) or later for your platform from here:

http://www.eclipse.org/downloads/packages/eclipse-ide-cc-developers/marsr

You will need Java to use Eclipse, you can download it from java.com.

For Linux, it will probably be in the official repositories. For example, in Ubuntu:

    sudo apt-get install openjdk-7-jdk

or

    sudo apt-get install oracle-java8-installer

**b) Android SDK**: This is the software that allows you to write Android apps. openFrameworks apps are written in C/C++, but you will still need this to interact with the NDK. You can download it from:

http://developer.android.com/sdk/index.html

Uncompress it in any folder on your hard disk. Later you'll need to tell eclipse where to find it.

**c) Android NDK**: This is the C/C++ compiler, headers and libraries for Android. OF 0.9.0 has been tested with the NDK version r10e, newer versions might work but it's not guaranteed.

https://developer.android.com/ndk/downloads/index.html

- OS X: http://dl.google.com/android/ndk/android-ndk-r10e-darwin-x86_64.bin
- Linux 32: http://dl.google.com/android/ndk/android-ndk-r10e-linux-x86.bin
- Linux 64: http://dl.google.com/android/ndk/android-ndk-r10e-linux-x86_64.bin
- Windows 32: http://dl.google.com/android/ndk/android-ndk-r10e-windows-x86.exe
- Windows 64: http://dl.google.com/android/ndk/android-ndk-r10e-windows-x86_64.exe

On Windows, you will also need to install MinGW in order to build openFrameworks. MinGW provides some essential build tools which are not included in the NDK. Follow just the "Installing the MinGW and Msys" instructions on this page: http://www.multigesture.net/articles/how-to-install-mingw-msys-and-eclipse-on-windows/.

**d) openFrameworks for Android package**: Download it from the downloads page:

http://openframeworks.cc/download

You may also check out the openFrameworks source from GitHub (under master branch): http://github.com/openframeworks/openFrameworks

**f) Set the paths for the NDK:**

Edit this file:

    openFrameworks/libs/openFrameworksCompiled/project/android/paths.make

This will tell openFrameworks where to find the android NDK.
If you don't have this file, create it from the paths.make.default template in the same directory.

- Set the values of NDK_ROOT to their install paths

The final file has to look something like:

```
# Default paths.make file.
# Enter the correct paths for your system and save this file as paths.make

NDK_ROOT=/home/arturo/Code/android-ndk-r10e
```

**g) Start Eclipse**: You will see a pop up asking you what workspace to use. Just point it to:
openFrameworks/examples/android.

**h) Android Eclipse plugin**:

There are detailed instructions here: http://developer.android.com/sdk/eclipse-adt.html

To install it, inside Eclipse go to
Help > Install New Software...

Click 'Add...' and enter the following info:

Name: Android SDK  
Location: https://dl-ssl.google.com/android/eclipse/

Press 'OK' and select the new repository in the "Work with:" drop down box in case it's not already selected.

You will see the SDK plugin in the list called "Developer Tools".

Select it and press 'Next' until you get to the "Review Licenses" screen. Check the "I accept the terms of the license" checkbox and press 'Finish'. Eclipse will download and install the Android plugin. Once it finishes press 'Yes' in the popup to restart Eclipse.


**j) Configuring the Android plugin**:

Once we have installed the Android plugin we need to tell it where to find the SDK. In Eclipse go to Window > Preferences > Android (or Eclipse > Preferences for OS X) and set the SDK location by browsing to the folder where you uncompressed the SDK before.

Now Eclipse knows where the SDK is.

Do the same for the path to the NDK.

Next you'll need to install the API files and optionally create an emulator to be able to test programs without uploading to the phone. Press the Android button in the Eclipse toolbar, or go to Window > Android SDK Manager.

First you need to install the SDK platform-tools and API package. Just click on the "Tools" tab and select the box for Android SDK Platform-tools. Then click on the "Android 4.2 (API 17)" tab and select the box for SDK Platform. It's important to use SDK version 4.2 (API 17) since the makefiles are configured for that version. It doesn't matter what version of the Android OS you want to develop for, apps are compiled using SDK 4.2, but they should work on any phone that is at least 2.2.

Once that is done you can create a new virtual device (AVD). Just select a name, the target Android version and a size for the virtual SD card.

**k) Import openFrameworks into Eclipse:**

Now that Eclipse has been completely configured to work with openFrameworks for Android, the last step is to import all the projects in the workspace. Go to
File > Import and select General > Existing projects in the workspace...

Import in this order:

- openFrameworks/libs/openFrameworks
- openFrameworks/addons/ofxAndroid/ofAndroidLib
- openFrameworks/examples/android/androidEmptyExample

You can later import more examples if you want but it's recommended to not have many examples opened at the same time so eclipse works faster

**l) Compile openFrameworks**:

In the "Project Explorer" on the left side of the window, select the openFrameworks project. Choose the Android target in Project > Build Configurations > Set Active, and then click Project > Build Project. You can also do this from the toolbar by switching to the C/C++ perspective and clicking the toolbar button with a hammer.

**m) Enable development in your device:**

Enable USB debugging: Settings > Applications > Development > USB Debug (On Ice Cream Sandwich, this is in Settings > Developer options > USB Debugging). The device needs to be disconnected from the computer while you do this.

**n) Connect the device now:**

If you attempt to run your project and you don't have a device attached, Eclipse will start the Android emulator for you.

**Linux users**: adb needs permissions to access the USB device, follow the instructions here to fix your device permissions:

http://developer.android.com/guide/developing/device.html

**o) Now install and run an example project on the device:**

- Connect the device.
- Check that it is being detected and restart adb server if necessary.
- Select the AndroidRelease target. You can pick a target at Project > Build Configurations > Set Active.

- Press the play button in the toolbar or Run > Run As > Android Application.

**Note:** If you get an error about an obsolete build.xml (or connected with that file), you can safely delete the build.xml file and recreate it using 'android update project -p <path-to-project>'. (The 'android' tool is at SDK_DIR/tools/android.) You may also need to do this for openFrameworks/addons/ofxAndroid/ofAndroidLib.

If everything went OK, the example should start on the device.

Notes
-----

- Data files should go in bin/data. During the build process everything in bin/data will get compressed to a resource in res/raw and then uncompressed and automatically copied to:  
sdcard/cc.openframeworks.appname
before running the app.

    If you have resources that change like XML config files, it's better to generate them from the code since uploading them to the phone will overwrite the configuration

- If there's no SD card in the device, examples that have resources won't work right now.

- Naming of resources is really restrictive in Android, for example you cannot have several resources with the same name even if they have different extensions.

- The AndroidDebug target does a different compilation process of the native code that allows it to detect linker errors that won't be detected when compiling in AndroidRelease mode. It is recommended to compile your application in AndroidDebug mode at least once or if your application crashes before starting. When installing applications on the device or emulator it is recommended to use the AndroidRelease mode since it's faster and the applications will be much smaller. There's also no support for debugging NDK applications in Eclipse, but you could theoretically use the NDK tools to debug an application compiled with AndroidDebug.

- Test your application very often. Even if the last NDK allows for debugging, there's no support for native debugging in Eclipse and setting it up manually with the NDK is pretty hard. When an application crashes the debugger dies too, so it's hard to debug bad memory accesses and similar bugs.

- Use the LogCat view in Eclipse. When programming for Android you cannot see the output of cout or printf, but if you use ofLog you can see its output in the LogCat. To open the view, go to
Window > Show View > Others > Android > LogCat

You can see the output of the compiler in the Console tab and the output of your app in the LogCat one. Everything that is output by openFrameworks through ofLog will have an openFrameworks tag so you can use filters to see only your application's output.

There's a bug in the Android plugin that makes Eclipse build every C/C++ project in your workspace before running any app. You can avoid this by closing projects that you're not currently working on (right-click > Close Project).

Alternatively, you can create a separate workspace for your apps:

- Create a folder inside openFrameworks/apps.

- Open Eclipse and tell it to use this new folder as a workspace. Do the import steps again for the new folder, including openFrameworks, libs, addons but instead of importing all the examples, import only androidEmptyExample to have a template for your new projects.

Creating new applications
-------------------------

You can copy any of the examples and start a new application from there. It's currently far more difficult to create a project from scratch, since the makefiles and project settings contain a lot of details you would need to duplicate.

In Eclipse this is easily done by right-clicking on an existing project, selecting Copy, then right-clicking on the workspace and selecting Paste. A small Copy Project window will pop up for you to pick a new project name and location. For now, project name and directory must have the same name. Let's say your application is called myApp, this must also be the name of your folder.

After you're done copying the project, you'll need to change the name of the application in different places:
- In res/values/strings.xml change app_name value to the name of your application.
- In AndroidManifest.xml change the name of the package from cc.openframeworks.exampleName to cc.openframeworks.myApp  
- in srcJava, select the package cc.openframeworks.exampleName, press F2 to rename it and call it cc.openframeworks.myApp

It's important to keep the package prefix as cc.openframeworks or some things can stop working. This will be fixed in future versions when Eclipse support for native code is better.

FAQ
---

**If the build fails:**

- If it tells you that you're using an obsolete build.xml, delete it and regenerate it using 'android update project -p <path-to-project>'. The build.xml files in the examples directory should not contain anything especially unique.
- Are you including addons? They need to be specified in addons.make, and the case of the letters must match exactly (ie, ofxOpenCv works but ofxOpenCV won't work). This error will probably show up as missing header files or symbols.
- If you're getting a bunch of undeclared reference errors, check which version of the NDK you're using. For this version you should be using NDK r8d.
- If you get 'com.android.sdklib.build.ApkCreationException: Debug Certificate expired on <date>', you have to 'rm ~/.android/debug.keystore'. A new certificate will be generated automatically.


**If the build succeeds but you can't install it on the phone:**

- Make sure you have your project selected in the Project Explorer before you tell it to run as an Android Application.
- If you get a message saying "Activity class ... does not exist.", make sure that its namespace is called cc.openframeworks.your_folder_name_here.OFActivity. This is what the Makefile currently expects. If it does not work even with a correct entry, and you are using an emulator, try using a real device instead.

**If the build succeeds but your app crashes:**

- Check the libs folder. It should be populated with a library during the build. On Linux it is a file that ends with .so. If there is no library, the C++ build process is probably failing somewhere, or it is not being triggered at all. You can test the C++ build process separately using 'make AndroidDebug'. You may also see something like this in your LogCat:


        E/AndroidRuntime(20743): Caused by: java.lang.UnsatisfiedLinkError: Couldn't load OFAndroidApp: findLibrary returned null
        E/AndroidRuntime(20743):     at java.lang.Runtime.loadLibrary(Runtime.java:425)
        E/AndroidRuntime(20743): 	at java.lang.System.loadLibrary(System.java:554)
        E/AndroidRuntime(20743): 	at cc.openframeworks.OFAndroid.<clinit>(OFAndroid.java:535)
        E/AndroidRuntime(20743): 	... 14 more


- The device must have an SD card if you use resources in your openFrameworks app. Note that some devices have an internal SD card, like the Galaxy Tab 10.1.
- Make sure you've declared the appropriate permissions in AndroidManifest.xml (for instance, android.permission.CAMERA for cameras and android.permission.WRITE_EXTERNAL to interact with the SD card, which is necessary if you have resources.)
- Was bin/data accidentally erased by something or other? Does res/raw/your_project_name_resources.zip exist, and does it contain your resources?
