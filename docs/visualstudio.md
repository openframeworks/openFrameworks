# Visual Studio

## Prerequisites
- Git - [Git for Windows] (https://git-scm.com/install/windows)

## Visual Studio Installation
------------

Download and install **Visual Studio 2022** or **Visual Studio 2026**.  

We recommend the free **Community** edition.

- [Visual Studio 2022 Community](https://visualstudio.microsoft.com/downloads/)  
- [Visual Studio 2026 Community](https://visualstudio.microsoft.com/downloads/)  

During installation, select the **Desktop development with C++** workload.

## Install openFrameworks
- Download openFrameworks 0.12.1 or later from the [download page](http://openframeworks.cc/download/) or from [github](https://github.com/openframeworks/openFrameworks/).
- If you got openFrameworks from git: https://github.com/openframeworks/openFrameworks
	- Run `scripts/android/download_libs.sh` to download libraries.
	- Use the Project Generator to generate Visual Studio project files for examples.

## Libraries
------------

If missing any libraries or want to update them run the script at **vs/scripts/**
- **download_libs.sh**

## Creating a new openFrameworks project
-------------------------------------

1. Open the **Project Generator** (ofxImGui interface).  
2. Enter your **Project name**.  
3. Choose your **Project path**.  
4. Under **Platforms**, select **Windows (Visual Studio)**.  
5. Under **Template**:  
   - For **Visual Studio 2022** → leave the Template field empty (default)  
   - For **Visual Studio 2026** → select **`vs2026`** from the dropdown  
6. Click **Generate**.

![Project Generator — Visual Studio 2022 (default template)](/images/vs/pg-vs2022.png)

![Project Generator — selecting the vs2026 template](/images/vs/pg-vs2026.png)

## Open and Run your project
----------------------

![Project Generator success screen](/images/vs/pg-success.png)

You can run by clicking Open in IDE in Project Generator or Open Folder to access directory.

Your project solution file is located at:  
`openFrameworks/apps/[yourFolder]/[projectName]/[projectName].sln`


Double-click the `.sln` file to open it in the matching version of Visual Studio (2022 or 2026).


![Visual Studio 2026 loading](/images/vs/vs2026.png)

Press **F5** to build and run in Debug mode.

## Debug vs Release
-------------------

- **Debug** — best while developing (full error info).  
- **Release** — smaller and faster (use when finished).  

Switch between them using the dropdown at the top of Visual Studio.

## Architectures
-------------------
- Targets are x86_64 - x64 - Default Windows
- ARM64 - for arm64 processors (native arm)
- ARM64EC - a mixture of both x64/arm64 for Windows 11

## Start coding
-------------

Hit **Esc** to close the window, then edit your code in `src/ofApp.cpp` and `src/ofApp.h`.

![Visual Studio 2026 Running](/images/vs/vs-2026-example.png)