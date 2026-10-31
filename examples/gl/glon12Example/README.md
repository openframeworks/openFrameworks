# glon12Example

### Learning Objectives

GLon12 runs openFrameworks' OpenGL on Direct3D 12 on Windows, through Mesa's d3d12 driver. Use it where the system OpenGL driver is missing, broken or old: Windows on ARM, virtual machines, remote desktops, CI.

You will learn how to..
* build a project with GLon12
* check at runtime which OpenGL driver and D3D12 adapter you got
* run on WARP, Windows' software D3D12 renderer, with `ofGLFWWindowSettings::glon12Warp`

### Setup

GLon12 is a library you download: projectGenerator > downloads > Modular libraries > glon12 (or apothecary: `TYPE=vs ./apothecary update glon12`).

Generate this example with the projectGenerator renderer (advanced mode, platform vs) set to:
* **GLon12** (`-DOF_USE_GLON12=1`): the GPU, through D3D12
* **GLon12 - WARP** (`-DOF_USE_GLON12=1 -DOF_USE_GLON12_WARP=1`): software D3D12, slow but needs no GPU driver

The build copies `opengl32.dll`, `libgallium_wgl.dll` and `dxil.dll` next to the exe. Without them, or on other platforms, the example runs on the system OpenGL driver.

### Expected Behavior

A grid of 8,000 instanced, pulsing boxes, with an overlay showing `GL_RENDERER`, `GL_VERSION`, the frame rate and which mode is active:
* `GLon12 on the GPU (D3D12)`: renderer `D3D12 (<your GPU>)`
* `GLon12 on WARP (software D3D12)`: renderer `D3D12 (Microsoft Basic Render Driver)`
* `system OpenGL driver (GLon12 not in use)`

Instructions for use:

* Drag to rotate the camera
* Press `w` to restart the app in the other mode (GPU / WARP). The D3D12 adapter is chosen when the first OpenGL context is created, so it can't change while the app runs
* Run `glon12Example.exe --warp` to start on WARP

`LIBGL_ALWAYS_SOFTWARE=false` in the environment forces the GPU even when WARP is requested.

### Other classes used in this file

This Example uses the following classes:

* [ofShader](http://openframeworks.cc/documentation/gl/ofShader/)
* [ofVboMesh](http://openframeworks.cc/documentation/gl/ofVboMesh/)
* [ofEasyCam](http://openframeworks.cc/documentation/3d/ofEasyCam/)
