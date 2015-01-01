OF 0.9.0
========

#### change key
          + added
          - removed
          / modified

------------------------------------------------------------------------------

CORE
----
### app
	/ setupOpenGL and ofApp___Window use ofWindowMode instead of int
	/ fix exit callbacks to allow for calling of the destructors, and better signal handling

### 3d
	/ ofEasyCam: removes roll rotation when rotating inside the arcball
	/ ofEasyCam: disables mouse events when moving ofxGui or other higher priority events
	/ ofDrawGrid: arguments changed to ```float stepSize, size_t numberOfSteps``` instead of ```float scale, float ticks```
	/ ofDrawGridPlane: arguments changed to ```float stepSize, size_t numberOfSteps``` instead of ```float scale, float ticks```
	/ ofCamera: fix calculations on first frame or before first call to begin

### gl
	+ Programmable lights and materials
	+ New area light type on programmable renderer
	+ Separate model matrix
	+ ofBufferObject wraps GL buffers for easy use, see compute shader
	  and pixel buffer examples for usage
	+ ofGetCurrentNormalMatrix
	+ ofSetOpenGL(ES)Version, allows to set any specific GL version
	+ ofTexture::enableMipmap() auto-generate mipmaps on load - for textures which support
	  mipmaps (that's TEXTURE_2D textures loaded after ofDisableArbTex() )
	+ ofTexture::generateMipmap() generate mipmap after a texture was loaded,
	  if the texture target supports mipmaps (see above).
	+ ofTexture::disableMipmap() disables auto-generation of mipmap for texture
	- removed compressed mipmaps based on deprecated glu methods
	- ofSetTextureWrap() : deprecated (use corresponding ofTexture member method)
	- ofGetUsingCustomTextureWrap() : deprecated
	- ofRestoreTextureWrap() : deprecated
	- ofSetMinMagFilters() : deprecated (use corresponding ofTexture member method)
	- ofGetUsingCustomMinMagFilters() : deprecated
	- ofRestoreMinMagFilters() : deprecated
	+ ofShader: report offending lines on compiling errors
	+ ofGLUtils: better support for half float types
	/ getTextureReference -> getTexture
	/ ofVbo refector: ofVbo will, in programmable renderer, automatically recognise
	  attribute locations 0, 1, 2, 3 as position, color, texture, normal attributes
	  can be set using either using convenience methods e.g.
	  `ofVbo::setTexCoordData(...)` or the more flexible
	  `ofVbo::setAttributeData(ofShader::TEXCOORD_ATTRIBUTE, ...)` which allows
	  for 3d texture coordinates.
	/ Fix ofTexture::readToPixels for non RGBA or 4 aligned formats
### graphics
	+ ofTruetypeFont: kerning and better hinting and spacing
	+ ofDrawBitmapString: can draw any type not only strings
	/ ofImage: can load image via https
	/ getPixelsRef() deprecated and getPixels() now returns ofPixels
	  temporarily ofPixels auto casts to unsigned char* so old code
	  is still compatible
	/ ofPixels::getPixels() -> getData()
	+ ofPixels support for YUV formats, the prefered allocation method
	  changes from channels to ofPixelFormat like:
		pix.allocate(w,h,OF_PIXELS_RGB)
	/ fixed ofSetBackgroundColor
	/ added ofGetBackgroundColor and ofGetBackgroundAuto
	- removed ofbBGColor and ofbBGClear
	/ ofImage::loadImage() -> load()
	/ ofImage::saveImage() -> save()
	+ ofBeginSaveScreenAsSVG

### utils
	/ better timming in fps calculation
	+ ofFpsCounter allows to easily meassure the fps of any element
	+ ofTimer allows to create a precise periodic waitable timer
	+ ofThreadChannel: thread synchronization primitive to make it
	  easier to work with threads avoiding the need for mutexes in
	  most cases. see example/utils/threadChannelExample
	/ ofBuffer::getBinaryBuffer() -> getData()

### video
	/ gstreamer: fix memory leaks when closing a video element
	+ gstreamer: support for YUV formats when using the programmable renderer
	  using OF_PIXELS_NATIVE as pixel format will choose the fastest format
	/ gstreamer: faster reload by default and optional asynchronous load
	/ ofVideoPlayer::loadMovie() -> load()
	/ ofVideoGrabber::initGrabber() -> setup()

PLATFORM SPECIFIC
-----------------

### Emscripten
	+ New platform!

### ofxiOS
	+ xcode 6.0 updates
	+ deployment target moved up to iOS 5.1.1

### android
	+ fix camera stretching artifacts experienced on some devices when using 16:9
	  aspect ratios
	+ remove support for arm5 since no devices seem to have that cpu anymore and it
	  was problematic with certain libraries

### linux and arm linux
	+ opencv libs are now installed in the system, rerun install_dependencies if you
	  are having problems compiling projects that use opencv

### windows
	+ activated high precission timming to fix inaccurate fps calculations

CORE ADDONS
-----------
### ofxOpenCV
	/ updated ofxCvHaarFinder to not use textures on internal image objects, allowing for usage within threads.

OF 0.8.4
========

#### change key
          + added
          - removed
          / modified

------------------------------------------------------------------------------

PLATFORM SPECIFIC
-----------------

###OSX
	/ Fixes for xcode 6

###RPI
	/ Fixes install_dependencies and makefiles



OF 0.8.3
========

#### change key
	  + added
	  - removed
	  / modified

------------------------------------------------------------------------------

PLATFORM SPECIFIC
-----------------
###OSX
	/ ofAppGLFWWindow: Drag drop files were coming in empty
### Windows
	/ ofAppGLFWWindow: Drag drop files were coming in empty


CORE ADDONS
-----------
### ofxKinect
	/ small fixes for CodeBlocks compilation on Windows



OF 0.8.2
========

#### change key
	  + added
	  - removed
	  / modified

------------------------------------------------------------------------------

CORE
----
### 3d
	/ fix of3DPrimitive copy constructor not copying node

### app
	+ ofAppEGLWindow added new methods hasMouse()/hasKeyboard() to provide info on whether mouse/keyboard were detected
	+ ofAppEGLWindow has new method setThreadTimeout allowing adjustable wait time for the thread to join
	/ ofAppGLFWWindow: fix key mappings on non english keyboards
	/ ofAppGLFWWindow: Cleaner exit code for GLFW based apps.
	/ ofAppGLFWWindow: fixes for better Retina support.
	/ Updated GLFW to latest master branch
	/ ofAppGLFWWindow: added patch to handle special keys and cases.
	/ ofAppGLFWWindow: better support for non-english keyboards.
	/ ofAppGLFWWindow: small bugfix for GLFW drag drop callback for retina screens. Closes
	/ ofSystem bugfix for successive calls. Now uses pclose instead of fclose.
### events
	+ fix ofSetFrameRate to be more accurate
	+ add keycode, scancode and unicode codepoint to ofKeyEventArgs
### gl
	+ ofFbo: added new method to attach an external texture
	+ ofFbo: fix regression on min/max filter settings
### Graphics
	/ ofImage: convert format when loading different image types
	/ Fix ofSetupPerspective not using passed width and height
	/ of3DPrimitives: fix copy constructor
### Math
	/ fix ofQuaternion setOrientation and getEulerOrientation
	/ ofMatrix: add case to catch invalid matrix queries
	+ ofGetCurrentOrientationMatrix(): query current orientation matrix state (supported by ofGLProgrammableRenderer, ofGLRenderer)
### Utils
	/ ofThread now uses Class name for logging channel (was thread name)
	/ ofURLFileLoader: better shutdown, don't shutdown if it wasn't initialized + wait thread
	/ ofXml: fixes crash on empty document
	/ ofSystem: fix for second and later calls failing
	+ ofThread argument for tryJoin time.
	/ ofThread overall, documentation and example fixes
	/ ofThread change to infinite wait.
	/ ofParameterGroup: fix to allow nested ofParameterGroups
	/ ofParameterGroup: memory leak fix
	/ ofThread: detach thread on stop if it's not being waited.
	/ ofParameter(Group) fix polymorphism on parameter groups

PLATFORM SPECIFIC
-----------------
### Android
	/ fix package generation, was missing paths.make
	/ fix 32 bits ndk path
	/ update project files to latest SDK/ADT (20140321)
	/ fixed shader in shader example
	/ disable x86 in assimp and opencv examples
	/ shader and assimp examples reload gl resources when the app resumes
### iOS
	+ implement setSpeed() for video player
	+ ofxiOSGetDeviceInfo for querying major / minor versions, device type and device string
	+ move to cleaner NSAutoreleasePool for iOS projects.
	/ iOS-VideoGrabber memory related fixes
	/ ofxiOSVideoPlayer: setSpeed functionality.
	+ ofxiOSSoundPlayer: added getAVSoundPlayer() method
	/ bugfix for ofxiOSImagePicker crashing on exit.
	/ ofxiOSImagePicker: switch to using ofPixels.
### Linux
	/ video: fixes for videos which pixels have padding
	/ video: close pipeline properly by sending and waiting for EOS event
	/ system: fix dialog boxes
	/ install & make: detect and use gtk 3 instead of 2 if it's available
###OSX
	/ Reduce warnings on OSX
	/ ofxOsc: Fix for issue with UDP multicast socket on OSX.
### Windows
	/ ofAppGLFWWindow: Correct fullscreen multi monitor display on Windows


CORE ADDONS
-----------
### ofxAssimpModelLoader
	/ fix to be able to put loaders on vectors
	/ use shared_ptr for assimp scene
### ofxGui
	+ ofxSlider: added get/setMin/Max
	/ ofxGui: fixed for mouse registration issues with ofxGui. ofxPanel was registering for mouse events 3 times on setup! - ofxButton was registering for events in setup and registering again in the destructor. - ofxSlider was never unregistering mouse events.
### ofxKinect
	/ bugfix to fix Kinect not being able to re-open if device was not correctly shutdown.
	/ bugfix for Kinect IR image crashing. Now sets the correct image sizes.
### ofxNetwork
	/ fix port reuse on osx
### ofxOsc
	+ added blob support. ability to send ofBuffers as blobs. Note: max data size limited by network buffer sizes.



OF 0.8.1
========

#### change key
	  + added
	  - removed
	  / modified

------------------------------------------------------------------------------

```
_____/\\\\\\\______________/\\\\\\\\\_______________/\\\_
 ___/\\\/////\\\__________/\\\///////\\\_________/\\\\\\\_
  __/\\\____\//\\\________\/\\\_____\/\\\________\/////\\\_
   _\/\\\_____\/\\\________\///\\\\\\\\\/_____________\/\\\_
    _\/\\\_____\/\\\_________/\\\///////\\\____________\/\\\_
     _\/\\\_____\/\\\________/\\\______\//\\\___________\/\\\_
      _\//\\\____/\\\________\//\\\______/\\\____________\/\\\_
       __\///\\\\\\\/____/\\\__\///\\\\\\\\\/____/\\\_____\/\\\_
        ____\///////_____\///_____\/////////_____\///______\///_

```
MAJOR CHANGES
-------------
	/ "testApp" changed to "ofApp" everywhere.

DEPRECATIONS & REMOVALS
-----------------------
	- ofxOpenCv: removed deprecated ofxCvMain.h. Use ofxOpenCv.h instead.

CORE
----
### 3d
    + ofGetCurrentMatrix() - returns current OpenGL matrix state (modelView-, projection- or textureMatrix) for ofGLProgrammableRenderer, ofGLRenderer.
### App
	/ updated GLFW to latest master (20131204)
	- delete temporary buffer in ofAppGlutWindow
	+ GLFW now detects when an OF app has been launched as Retina
	/ Fixed unset variables in ofMouseEventArgs
	- remove early, unnecessary ESC check
	/ ofxAppEGLWindow fix
### Communication
	/ use binary mode (don't convert breaklines)
### Events
### Graphics
	/ ofCairoRenderer: fix for moveTo when drawing ofPaths
	/ ofDrawBox: on wireframe mode was recreating vertices on every call
	/ fix ofImageType retrieval issue in ofTexture.readToPixels()
	+ ofColor: added setHueAngle and getHueAngle
	/ Fix for (ofColor == and != operators ignoring alpha)
### Math
        / ofMatrix3x3: fix incorrect multiplication
	+ Added read-only access to current Matrix Stack
	/ fix critical ModelViewMatrix issue when ofFbo::begin(setupScreen==false)
### GL
	/ ofMaterial: fixed back emmisive material
	/ Fix point sprites in openGL 3+
        / ofShader: #pragma include directive can now deal with relative paths for include files
	/ Fix for mesh resolution parameters
	/ Fix for the drawing of ofDrawArrow
	/ fix critical issue w/ ofVbo shallow copies (custom attributes)
        / ofVbo custom attribute stride
        / fix ofShader include path behaviour
	/ Modify ofVbo bind method to allow 2d vertices
	/ Enabling GL_PROGRAM_POINT_SIZE in ofGLProgrammableRenderer
	- remove setName from ofMesh
	/ Assign allocated flag to true after allocating offscreenGWorldPixels
	/ Fix gl programmable renderer of draw axis
	/ fixed ofMaterial for opengl ES1
	/ check if currentShader exists - prevents segfault
	/ add more sophisticated openGL capability checks
	+ displacement map example
### Sound
### Types
### Utils
	+ ofXml(const& string) constructor added
	/ Fixed loadDialogBrowseCallback to return UNICODE string
	/ ofXml addressing seg errors when clearing
	/ tiny log fix in ofSerial
	+ ofXml get attribute to work with paths && non-paths
### Video

PLATFORM SPECIFIC
-----------------
### Android
	/ fix for texture reload
	/ change to work with NDK r9b by default
	/ zip uncompress moved to java
	/ several fixes in pause / resume workflow
	/ fix camera not working on some devices + use external texture, should be faster
	/ camera: add recording hint for fastest fps on newest devices
	+ support for x86
	/ Android building script error when building from Windows
	/ Bug fixed, Compile error in ofxAndroidSoundPlayer
### Mac OS X
	/ tr1/memory compile issue on OS X 10.9
	/ use cc.openframeworks.ofapp as identifier on osx
	/ ES3 + xcode5 fix for missing GL_STENCIL_INDEX
### iOS
	/ iOSVideoGrabber memory leak fixes
	/ iOS Example Fixes
	+ added iosStoryboardExample
	/ iOS-Storyboard Example Fix for <iOS7
	/ Fix iOS SoundEngine memory leak
	/ Use correct audio category for routing audio to speaker
	+ Enabled pointsprites for OpenGLES
	/ ofxiOSUIImageToOFTexture fix inside ofxiOSExtras
	/ fixed depth issue with ios assimp example
	/ texture cache fix for ofxiOSVideoPlayer
	/ ios movie player example fix
	/ ios feature for getting / setting clipboard string
### Linux
	/ alsa as default instead of pulseaudio lower latency
	/ makefile fixes for better eclipse parsing
	/ videoplayer: fix alpha formats for gstreamer 0.10
	+ install_dependencies installs libusb so ofxKinect compiles
	- removed portaudio soundstream and all dependencies
	/ Fix install_dependencies.sh for Fedora
	/ project generator simple/linux: compile in release mode instead of debug
### Windows
	/ fixed disable vertical sync through update in glfw
	/ fix for ofGetFrameRate() going to infinity at high fps on windows
	/ replaced poco 1.4.6 libs with 1.4.3p libs for vs2012
	/ Changes to ofGstVideo files to compile with 0.10 gstreamer and fixup file names in Win32
	/ Improve Dialog Win32
### Linux Arm
	/ Fbo with ofClear now works properly
	/ Fbo/Shader behavior now consistent with other platforms (can be used in update and draw)
	/ Shaders used in draw() no longer cause GL Errors and corrupt display


CORE ADDONS
-----------
### ofxAssimpModelLoader
	/ Remove ofMesh setName() in ofxAssimpModelLoader
### ofxKinect
	+ Support for k4w and 1473 kinect with motor / accel / led control
	/ Bugfix for disconnecting 1473 and K4W devices on OS X
	/ Fixes typo in ofxCvImage that broke getRoiPixelsRef
	+ added Windows driver install info to kinectExample header
### ofxGui
	+ added ofParameter in the setup routine of ofxGuiGroup

PROJECT GENERATOR
-----------------

	/ PG recompiled against 0.8.1
	+ retina enabled for OSX
	+ added support for adding frameworks on OSX


EXAMPLES
--------

OF 0.8.0
========

#### change key
	  + added
	  - removed
	  / modified

------------------------------------------------------------------------------


MAJOR CHANGES
-------------
	+ programmable GL renderer (ability to use OpenGL 3+ / GLES 2.0)
	 linux support for ARM devices (e.g. Raspberry Pi)
	+ ofXml, a full xml parser. NOTE: ofXML is still in development, more convenience functions will be added in future releases.
	+ ofParameter, allows for datatypes to have names, default values and ranges, check out the examples!
	+ 4x anti-aliasing is now on by default (osx/linux/win)
	+ ofxKinect and ofxGui as core addons
	+ new module-based logging system
	+ apps now get special keys, shift/super/alt/ctrl etc
	+ debug and release icons for all apps
	+ textures and text are now correctly flipped when in ofCamera view. NOTE: this might mean that some projects which are manually flipping textures might see those textures inverted
	+ orientation affects ofCamera. NOTE: old projects correcting this manually will break.
	+ OpenGL ES 2.0 support on ofxiOS
	+ support for Visual Studio 2012 (drop support for earlier VS versions)
	/ changed default window manager from GLUT to GLFW
	/ vsync and alpha blending now on by default
	/ new documentation structure and now written in Markdown
	/ ofxiPhone completely renamed to ofxiOS

DEPRECATIONS & REMOVALS
-----------------------
removed functions which were deprecated in 0072:

	- ofVec*f::squareLength()
	- ofVideoGrabber::grabFrame()
	- ofVideoPlayer::idleMovie()
	- ofPolyline::addVertexes
	- ofGraphics: ofVertexes and ofCurveVertexes
	- ofQTKitPlayer::bind() and ofQTKitPlayer::unbind()
	- ofAppiPhoneWindow::enableRetinaSupport(), isRetinaSupported(), isDepthEnabled()

deprecated in this release:

	/ ofBox, ofCone, ofSphere deprecated in favour of ofDrawBox and ofDrawSphere
	/ ofxiPhoneSetOrientation and ofxiPhoneGetOrientation -> ofSet/GetOrientation
	/ ofxOpenALSoundPlayer
	/ ofSetupScreenPerspective(), ofSetupScreenOrtho() don't accept orientation and vflip parameters anymore, use ofSetOrientation() to specify them
	/ ofPath::set/getArcResolution -> set/getCircleResolution


CORE
----
### 3d

	of3dPrimitive
                + ofPlanePrimitive
                + ofSpherePrimitive
                + ofIcoSpherePrimitive
                + ofCylinderPrimitive
                + ofConePrimitive
                + ofBoxPrimitive

	ofMesh
                + ofMesh::setColorForIndices( int startIndex, int endIndex, ofColor color )
                + ofMesh::getMeshForIndices( int startIndex, int endIndex, int startVertIndex, int endVertIndex );
                + ofMesh::mergeDuplicateVertices()
                + vector<ofMeshFace> & ofMesh::getUniqueFaces()
                + vector<ofVec3f> ofMesh::getFaceNormals( bool perVertex )
                + ofMesh::setFromTriangles( const vector<ofMeshFace>& tris, bool bUseFaceNormal )
                + smoothNormals( float angle )
                + get meshes for primitives: plane, sphere, icosphere, cylinder, cone, box
                + ofMeshFace class with additional functions like getFaceNormal()
                / fixed bug where it was assumed the count of vertices / normals / indices / colours / texCoords was the same


	ofNode
                / ofNode::setParent() and clearParent() now accept a flag which allow nodes to maintain their global position

        ofCamera
        	+ ofCamera::setVFlipped/ofCamera::isVFlipped

### App
        + ofAppGLFWWindow::setMultiDisplayFullscreen(bool)
        + ofEnable/DisableAntiAliasing that works at runtime
        + ofGetOrientation() and ofSetOrientation()
        + OF_KEY_TAB
        / fixed bug where ofAppGlutWindow in fullscreen would report incorrect window size on the first frame

### Communication
        + HTTPS support

### Events
        + ofAddListener now has an optional param for listener priority
        + event listeners can now return a bool to indicate whether an event should be marked as attended
        + support for modificator keys: control, alt and shift (only glfw)

### Graphics

	of3DGraphics
                + normalized texture coords by default
                + added get / set functions for setting resolutions for new primitives
                + ofDrawPlane
                + ofDrawIcoSphere
                + ofDrawCylinder
                + ofDrawCone
                / functions like ofBox and ofSphere merged into of3dGraphics

	ofPolyline
                + resample a single point anywhere along an ofPolyline
                + cache and get normals, lengths, curvature, and rotation vector at any point
                + find the nearest index at a given length along the path
                + insert a vertex at any given point
                + required utils and mods to make the above possible

	ofTrueTypeFont
                + support for iso8859-1 on utf-8 encoded files
                + loading of system fonts by name
                + defaults for system fonts (OF_TTF_SANS, OF_TTF_SERIF and OF_TTF_MONO)

        + ofGetBackground()
        + ofColor::blueSteel
        + ofShortPixelsRef and ofFloatPixelsRef
        + ofPixels::setColor(ofColor) and ofImage::setColor(ofColor)
        / ofColor fix for colors wrapping.
        / ofPath refactored to be easier to integrate into other graphics libraries
        / ofDrawBitmapStringHighlight now modifies less GL state
        / ofDrawBitmapString now properly interprets tabs
        / fixed bug where ofTrueTypeFont::drawStringAsShapes didn't respect spaceSize and letterSpacing
        / fixed getHex and setHex for ofFloatColor and ofShortColor
        / fixed infinite recursion bug in ofCairoRenderer::background(float)
        / ofBox, ofCone, ofSphere deprecated in favour of ofDrawBox and ofDrawSphere
### Math
        + 1D interpolation functions: ofCosine|Cubic|Catmull|HermiteInterpolate
        + generic wrapping function float ofWrap(float value, float from, float to)
        / ofWrapDegrees, ofWrapRadians only wrap the new ofWrap, don't enforce a 360Â°/2Pi cycle anymore.
        / ofxVectorMath now included in core (as ofVectorMath)

### GL
        + ability to draw instanced geometry using ofVbo::drawInstanced()
        + ofSetDepthTest(), ofEnableDepthTest(), ofDisableDepthTest()
        + ofEnableAntiAliasing() / ofDisableAntiAliasing(), to toggle antialiasing on runtime. NOTE: needs to be enabled in the window.
        + ofShader support for #pragma includes
        + ofVbo support for VAOs
        / ofVbo and ofVboMesh accessors marked const
        / ofTexture::loadData will now reallocate if necessary
        / fixed ofLight global position and orientation issues when parented
        / fixed some private members of the gl renderer not being initialized
        / fixed the logic of vflip, handedness and orientation

### Sound
        + ofSoundStream: getters for sampleRate, bufferSize and numChannels

### Types
        + ofScopedLock (typedef for Poco::FastMutex::ScopedLock)
        + ofBaseHasShortPixels and ofBaseShortImage
        / typedefs that create compilation errors in C++11 now avoided

### Utils
        + ofGetVersionMajor(), ofGetVersionMinor(), ofGetVersionPatch()
        + ofSerial::isInitialized()
        / ofLog level & module header now more compact & readable, fixed bug where the default "OF" module was being printed to log files
        / ofFile now creates missing directories when saving
        / ofDirectory listings now properly interpret numerical file names (so "2.jpg" will show up before "10.jpg")
        / fixed bug where ofURLFileLoader wouldn't unlock mutex when request pool is empty
        / ofThread now waits during destructor, with a timeout of 10 seconds

### Video
        / ofVideoGrabber::listDevices() now returns a vector<ofVideoDevice>

PLATFORM SPECIFIC
-----------------
### Android
        + support for choosing GLES renderer at runtime
        + ofxAndroidVibrator
        + ofxAndroidVideoPlayer
        / fixed bug where app width and height weren't set before calling setup()
        / fixed bug in onTouchMoved where historical event calls were being given incorrect arguments

### Mac OS X
        + default app icons
        + makefile support
        / Xcode now shows more compilation warnings
        / template project file has been updated
        / file dialogs now resolve alias files

### iOS
        + launch images and icons for iPhone5 and retina iPad
        + ofxiOSSoundStream::setMixWithOtherApps() allows other apps' sound to play in the background
        + ofiPhoneVideoPlayer::setFrame, getCurrentFrame, getTotalNumFrames, etc
        + option to enable hardware orientation in ofAppiOSWindow
        + ofxiOSSoundPlayer as default sound player (based on AVAudioPlayer)
        / ofxiPhone renamed to ofxiOS
        / ofxiPhoneSoundStream refactored and renamed to ofxiOSSoundStream
        / ofCamera now behaves correctly according to device orientation
        / app now unregisters for ofEvents on exit
        / external display mirror mode is now default
        / fixed some functions which were not properly returning BOOL
        / fixed bug where sound stream would not resume after interruptions
        / fixed bug where converting a UIImage to an ofImage would pull in a bit of garbage memory
        / ofxiOS-Info.plist no longer being copied into project bundle
        / deprecated ofxiPhoneSetOrientation and ofxiPhoneGetOrientation
        / deprecated ofxOpenALSoundPlayer

### Linux
        / Add glfw3.0 for Linux 32bit
        + core support for armv6 and armv7
        + drag & drop is now supported on linux through glfw

### Windows
        + Visual Studio 2012 support (drop support for earlier VS versions)
        + added ability to get current .exe path
        / ofSerial can now connect to COM ports higher than COM9
        / capitalized target names ("Debug" and "Release")
        / added "compile C as C++" flag for VS project template

CORE ADDONS
-----------
### All addons
        - install.xml removed in favour of addons_config.mk

### ofxKinect
        + isFrameNewVideo(), isFrameNewDepth()

### ofxOpenCV

### ofxAssimpModelLoader
        / loader now caches repeated textures
        / fixed crash where a scene is released while the application is exiting

### ofxTCP
        / ofxTCPServer now thread-safe

### ofxSvg
        / fixed bug where transform attribute was ignored
        / fixed bug where non-allocated memory was being free()d

### ofxOsc
        / ofxOscMessage accessors marked const

### ofxGui
        + added a new GUI addon using ofXml

PROJECT GENERATOR
-----------------
### project generator (simple)
        / fixed issue where xibs weren't being properly handled in iOS projects

EXAMPLES
--------

### All Platforms
        + gui examples, demonstrating ofParameter and ofxGui
        + vboMeshDrawInstanced demonstrates instanced drawing
        + 3DPrimitives example
        / many general bugfixes
        / many examples reworked to demonstrate better threading practices

### Android
        + ofxGui example
        / all examples now have WRITE_EXTERNAL_STORAGE permission

### iOS
        + ofxGui example
        - OpenAlExample

---------------------------------------------------------------

	  .oooo.        ooooooooo           .o
	 d8P'`Y8b      d"""""""8'         .d88
	888    888           .8'        .d'888
	888    888          .8'       .d'  888
	888    888         .8'        88ooo888oo
	`88b  d88' .o.    .8'     .o.      888
	 `Y8bd8P'  Y8P   .8'      Y8P     o888o

OF 0.7.4
========

#### change key
	  + added
	  - removed
	  / modified

------------------------------------------------------------------------------


MAJOR CHANGES
-------------

New Codeblocks (windows):
For 0.7.4 we updated all core libraries to work against the new version of codeblocks, which itself uses a new version of GCC.  Anyone using codeblocks on windows will need to update to 12.11, because these libraries will not work with older versions of codeblocks.

System Libraries (linux):
Linux now uses some system libraries instead of including them in the core.  Please ensure you have installed the system dependencies for your OS.  You can install the system dependencies by executing the install_dependencies.sh script in  $OF_ROOT/scripts/linux/DISTRO/install_dependencies.sh, where DISTRO is your running linux distribution.  Currently OF has install scripts for Debian, Ubuntu, ArchLinux, and Fedora.

New versioning system:
From now on, OF version are numbered Major.Minor.Patch (e.g. 0.7.4). See Utils section and readme.md for details.

DEPRECATIONS & REMOVALS
-----------------------
- removed OF_USING_POCO conditionals
- support for Codeblocks version 10.04 on windows
- removed portaudio and freetype libraries.  Linux now uses some system libraries.  Please ensure you have installed the system dependencies.

CORE
----
### 3d

	ofCamera
		+ added getForceAspectRatio() and getAspectRatio()
		- dependencies with glu, openGL
		/ fix const correctness
		/ fixed worldToScreen() bug
		/ lens offset

	ofEasyCam
		/ setPosition fix

	ofMesh
		/ fixed load and save type mismatch
		/ fixed ofMesh.load() to load Normals

	3DModelBuilder
		/ to work with OpenGL ES

### App
	No Updates
### Communication
	No Updates
### Events
	+ added void events (events that can be listened to from a method without arguments)
	- removed OF_USING_POCO conditionals

### Graphics

	ofTrueTypeFont
	   / fixed out-of-bounds access in string display functions
	   / fixed getStringAsPoints() when using full character set

	ofColor
	   + css3-style colors (ofColor::mediumAquaMarine, ofColor::lightSkyBlue, etc.)

	ofPath
		+ added ofCurveVertex(x, y, z) overload

### Math
	No Updates
### GL
	No Updates
### Sound
	No Updates
### Types
	No Updates
### Utils
	+ added ofToDouble()
	+ added Android and iOS functionality to ofLaunchBrowser
	/ ofGetVersionInfo() now returns only the version string.

	ofBuffer
		+ added append()

	ofConstants
		+ added #include <cfloat>
		/ The OF_VERSION* defines have been changed for the new version system.

### Video

	ofGstVideoGrabber
		+ included support for PREFER_RGB_OVER_YUV and PREFER_NON_COMPRESSED
		/ updated gstreamer player/grabber to be compatible with gstreamer 1.0

PLATFORM SPECIFIC
-----------------

### Android
	+ added ofxAndroidLaunchBrowser(String url)to ofxAndroidUtils
	+ added parent OFActivity class
	+ added wifi multicast lock
	+ added unloadTextures() to ofxAndroidApp

	/ fixed getPan() and multiplay resume in ofxAndroidSoundPlayer
	/ exposed getRealExternalStorageDirectory() as a public method
	/ changed dataPath location to use Android standards
	/ updated to Android SDK 21 and NDK r8d
	/ updated eclipse project to use default run method instead of AndroidInstall target
	/ fixed windows platform detection
	/ added ability to choose arm targets through config.make
	/ updates to the accelerometer units
	/ updated readme file to reflect latest changes in Eclipse setup

	ofImage
		/ register images on load

### Mac OS X
	/ ofQTKitVideoGrabber now uses BGRA with BGRA to RGB blitting for fast capture.

### iOS
	ofImage
		/ register images on load

### Linux
	- removed portaudio and freetype libraries

### Windows
	+ codeblocks 12.11 support (drop support for earlier CB versions)
	/ fixed grayscale image on windows
	/ fixed microsecond timer overflow

CORE ADDONS
-----------

### ofxOpenCV
	/ fixed broken hole detection in CV contour finder
	/ applied patch for opencv bug (http://code.opencv.org/issues/1643)

### ofxOpenALSoundPlayer
	/ fixed static ofMutex issue in ofxOpenALSoundPlayer

### ofxAssimpModelLoader
	/ Assimp makeover
	/ aiNode bugfix

### ofxTCP
	+ adds rawMsg send and receive
	/ fixes tcp close

### ofxSvg
	/ several bugfixes for ofxSvg

PROJECT GENERATOR
-----------------

### project generator (simple)
	/ updated the git submodule and gitignore for develop branch
	/ fix project generator on Linux

EXAMPLES
--------

### All Platforms
	+ added simpleEventsExample
	+ added graphics/colorExtended example
	/ fixed GPUparticleSystemExample unexpected behavior
	/ added missing baud variable to communication/serialExample
	/ updated firmataExample and fontShapesExample to use ofBaseApp instead of ofSimpleApp

### Android
	+ androidAccelerometerExample
	+ android3DModelLoaderExample
	/ updated android examples to use ofxAndroidApp instead of ofBaseApp


---------------------------------------------------------------
		  .oooo.     .oooo.    ooooooooo   .oooo.
		 d8P'`Y8b   d8P'`Y8b  d"""""""8' .dP""Y88b
		888    888 888    888       .8'        ]8P'
		888    888 888    888      .8'       <88b.
		888    888 888    888     .8'         `88b.
		`88b  d88' `88b  d88'    .8'     o.   .88P
		 `Y8bd8P'   `Y8bd8P'    .8'      `8bd88P'

OF 0073
=======

#### change key
	+ added
	- removed
	/ modified

---------------------------------------------------------------

/ ofGetElapsedTimef() no longer wraps after ~71 minutes, which was a bug introduced with 0072. ofGetElapsedTimeMicros() previously also wrapped after ~71 minutes, and this has been fixed as well.
/ fixed broken pause and other fixes in ofVideoPlayer on OSX
/ fixes to AndroidVideoGrabber
/ Android empty example inherits from ofxAndroidApp + adds android methods
+ auto register android touch events
+ ofMultMatrix(), ofLoadMatrix(), ofLoadIdentityMatrix()

---------------------------------------------------------------

		  .oooo.     .oooo.    ooooooooo   .oooo.
		 d8P'`Y8b   d8P'`Y8b  d"""""""8' .dP""Y88b
		888    888 888    888       .8'        ]8P'
		888    888 888    888      .8'       .d8P'
		888    888 888    888     .8'      .dP'
		`88b  d88' `88b  d88'    .8'     .oP     .o
		 `Y8bd8P'   `Y8bd8P'    .8'      8888888888

OF 0072
=======

#### change key
	+ added
	- removed
	/ modified

-------------------------------------------------------------

DEPRECATIONS & REMOVALS
-----------------------

### Deprecations (will be removed in a future release, replacements noted after --->)

	/ ofVec*f: squareLength() ---> lengthSquared()
	/ ofVideoGrabber: grabFrame() ---> update()
	/ ofVideoPlayer: idleMovie() ---> update()
	/ ofPolyline: addVertexes() ---> addVertices()

	ofGraphics
		/ ofVertexes() ---> ofVertices()
		/ ofCurveVertexes() ---> ofCurveVertices()

	ofQTKitPlayer
		/ bind() ---> getTexture()->bind()
		/ unbind() ---> getTexture()->unbind()

	ofAppiPhoneWindow
		/ enableRetinaSupport() ---> enableRetina()
		/ isRetinaSupported() ---> isRetinaSupportedOnDevice()
		/ isDepthEnabled() ---> isDepthBufferEnabled()

### Removals (have been deprecated previously, their functionality has been migrated to the OF core)

	- ofxDirList ---> ofFileUtils: ofFile, ofFilePath, ofDirectory, etc
	- ofxThread ---> ofThread
	- ofxVectorMath ---> ofVectorMath: ofVec2f, ofVec3f, etc

CORE
----

	/ fixed lots of compilation warnings in the core library

### 3d
	/ ofMaterial fixed incorrect storing / restoring of shininess

	ofEasyCam
		+ enable/disable/get now uses middle mouse button for translation, activated by default
		+ added set/get translation key, pressing this key and dragging with the left mouse button
		   will translate the camera instead of rotating
		- removed mouseEvent listener methods
		/ underlying math rewritten for readability, efficiency, & reliability
		/ fixed strange or erratic behaviour and blocked rotation
		/ setTarget now behaves as expected
		/ fixed camera movement before dragging
		/ updated rotation interactions to rotate over all three axes, rotation over the X or Y axes is
		  achieved by starting to drag inside a circle with it's diameter set to the minimum between
		  the viewport width and height, when the drag starts outside this circle Z axis rotation is achieved

	ofMesh
		/ convert all index types from int to ofIndexType
		/ fixes to multiLightExample

	ofNode:
		+ missing implementation for rotateAround()
		+ added getGlobalScale()

### App
	/ ofGetElapsedTimeMillis() now returns an unsigned long

### Communication
	+ added ofSerial speed: 230400

### Events
	/ ofNotifyMousePressed() wasn't updating currentMouseX and currentMouseY
	/ ofEvents: const parameters in events can be notifed by passing a value directly

### Graphics
	+ added ofClear(ofColor c)
	+ added ofRectRounded functions, bring on the lawsuit APPL ...
	/ fixed anti-aliasing issue when auto background set to false
	/ ofFloatImage and ofShortImage display correctly again
	/ ofDrawBitmapString(): default style changed to be backwards compatible with pre-007
	/ ofDrawBitmapString(): fixed drawing in simple mode, and drawing with alpha enabled
	/ ofDrawBitmapString(): fixed rendering to FBOs (i.e. upside-down inside FBO)
	/ ofGstUtils: several fixes for network streams and performance

	ofCairoRenderer
		+ added support for rendering to a png
		/ now sets current style every frame since Cairo wasn't preserving the style
		/ now gueses type from file extension and renders to memory
		/ fixed broken Z-rotation when called with ofRotate(X,0,0,1)
		/ use ofSaveImage() to save image buffers instead of Cairo png

	ofImage
		/ saveImage() now flattens color channels (4 to 3) when saving a jpeg
		/ fixed memcpy bug in getBmpFromPixels()
		/ update() rewritten to use ofGetGlInternalFormat so ofImage can handle short and float images

	ofPolyline
		+ added fromRectangle()
		/ getBoundingBox() is now const

	ofTrueTypeFont
		/ make font rendering whitespace and printable character width the same

### Math
	/ ofPoint & ofVec*f: fixed myVec = float only assigning to the x value,
	  now prints an error message to help track down bugs, you should now use myVec.set(float)
	  instead to set all three values to the same float value
	/ all instances of functions named *Vertexes have been changed to *Vertices
	/ ofVec3f: removed default argument for z component in 3 value constructor

### OpenGL
	- removed unused ofCheckGLTypesEqual(int, int)
	/ ofLight::customDraw(): fixed bug

	ofFbo:
		/ fixed FBO error when trying to retain depth texture
		/ cleanup and bugfixes for both iOS and desktop
		/ fixed bug where ofFbo was broken on systems that did not support MSAA

	ofTexture
		+ added setUseExternalTextureID for setting external Texture ID which will not be released by ofTexture,
		   was causing crashes for people trying to use external texture ids
		/ can now read to pixels from depth buffers
		/ added depth texture to image type support for OpenGL ES

	ofVboMesh
		+ added enabled/disable of flags for colors, texCoords, indices, & normals
		/ fixed problem when changing size to 0
		/ fixed clearing of vbo buffers when mesh is reallocated to 0

### Sound
	/ setPan() now works consistently (range is -1..1) over all objects that use sound
	/ ofOpenALSoundPlayer: fix for stopThread with no arguments
	/ ofSoundPlayer::loadSound() now properly returns a bool when it succeeds or fails

### Types
	 / ofPixels: fixes to channel count in OF_PIXELS_RGBA

	ofRectangle
		+ added getters and setters for x , y, width, & height
		+ added methods for working with lines, translation, & scaling
		+ edge x/y locations can now be called by their "anchor" name (ofAlignHorz and ofAlignVert)
		   using getHorzAnchor() and getVertAnchor()
		+ calling standardize() on a given ofRectangle will adjust the x / y position to make both
		   the width / height to greater than zero
		+ added a + operator, which is equivalent to calling translate(ofPoint ...)
		- removed getAsPolyline() and growToInclude()
		/ x & y vars are now references to an underlying ofPoint, you can get a reference to
		  the underlying ofPoint via getPositionRef()

	ofColor
		/ fixes to reading from byte streams
		/ fixes to setHsb for ofFloatColor and ofShortColor

### Utils
	+ added deprecation mechanism: compile-time warnings tell you if you are using legacy/old functions,
	   alternatives are mentioned where appropriate
	+ ofSystemTextDialog
	+ ofToString: added float formatting args
	+ ofThread: added getter for poco thread
	/ ofLog messages are significantly shorter
	/ ofToDataPath() bugfixes
	/ ofToString(value,int,int,char) now unambiguous
	/ ofToHex(): fixed incorrect return value for non-ascii input
	/ ofGetModifierPressed(): fixes to getting whether modifier keys are pressed
	/ ofGetElapsedTimeMillis(): fixes to time counter functions, and increased accuracy
	/ ofFileUtils: fixed comparison operators
	/ ofThread: stopThread() fix
	/ ofBuffer: set and operator= from string + append method
	/ ofLaunchBrowser: now accepts https URLs
	/ ofPtr: extended facade to allow for dynamic_pointer_cast

	ofConstants
		+ added ofAlignVert & ofAlignHorz enums based on Qt,
		   could be combined into a single bit-wise combined alignment flag
		+ added ofAspectRatioMode enum, describes how an aspect ratio is preserved (or not) during scaling operations,
		   can be combined with ofAlignVert and ofAlignHorz to describe a rectangular scaling operation
		+ added ofScaleMode enum, represents a set of common scaling modes where each scaling mode can also be
		   described by a specific combination of ofAspectRatioMode, an ofAlignVert, and an ofAlignHorz
		   making ofScaleMode makes common scaling operations less verbose

	ofURLFileLoader
		/ now catches all exceptions
		/ fixed issue with ofRemoveRequest if ofURLFileLoader currently loading the request

### Video
	/ solved dropped frame bug on setFrame
	/ fixed createMovieFromURL
	/ fixed problem when reloading movies
	/ fixed setTexture(true) (allocates texture if it wasn't already allocated)
	/ fixes to GStreamer streaming and initialization, squash unnecessary re-init on stop

	ofVideoPlayer
		+ added functionality to load video from a URL
		+ added movie path variable and getter
		/ setVolume changed to take a float 0..1 with a warning message on out-of-range
		/ updated to handle pixel formats more optimally

	ofVideoGrabber
		/ switched to 24RGB pixel format to avoid manual conversion
		/ now allows up to 24 cameras on Windows

PLATFORM SPECIFIC
-----------------

### Android
	+ ofSystemTextAlert: added android version
	+ ofSoundPlayer: implemented multiplay and streaming
	/ Makefile now includes ofVideoPlayer
	/ Eclipse project: update android target to use Android toolchain
	/ updated setPixelFormat and getPixelFormat
	/ updated OpenCV library
	/ no more modal dialogs, were causing problems with gui thread
	/ ofSoundPlayer: resource usage and bug fixes
	/ updates Hardware Key handling to use characters instead of keycodes

	ofAndroidLib
		+ added support for touchCancelled and swipe
		/ updated to the latest Android SDK R20 and Android-NDK-R8B
		/ updated minimum SDK level to 8 or Android 2.2
		/ multitouch now supports 10+ pointers

	ofxAndroid
		+ added listAlert
		+ added network alerts
		+ added ofxAndroidEvents
		/ no longer crashes on 4.0.4 and 4.1.1 devices
		/ updates to fix paths to external storage
		/ ofThread now registers pthreads through JNI

	ofAppAndroidWindow
		/ no longer crashes when returning from onPause()
		/ fix for non-threaded mouse events
		/ java side is already checking resume is not called on start

### Mac OS X
	/ OpenFrameworks now works "out of the box" on 10.7+ by using QTKit for video grabbing & playback,
	/ If compiling with the 10.6 SDK, the old Quicktime framework based players and grabbers are still included
	ofVideoPlayer
		/ uses Core Video and QTKit internally
		+ added support for getCurrentFrame() and setFrame() on videos with non-static framerates
		+ added support for more codecs through QTKit framework
		+ added support for direct-to-texture video rendering
		+ added option to enabled asynchronous seeking for non-blocking calls to setFrame()
	ofVideoGrabber
		/ uses Core Video and QTKit internally
		+ added ability to record videos to disk
		+ added support supports a wider range of video inputs, for example HDV over FireWire
		- removed video setting dialog for 10.7+

### iOS
	+ fixed/added antialiasing for iOS
	+ added ofxiPhoneGetGLParentView() for accessing the GLView parent UIView,
	   this is now where other UIViews should be added to (was ofxiPhoneGetUIWindow()),
	   also fixes a bug where the native video player was not displaying correctly on first load
	+ added EAGLViewDelegate for getting state updates from EAGLView, also added a destroy method
	+ added ofxiOSExtensions giving access to functions for reloading OpenGL textures,
	   called when a ofxiOSEAGLView is destroyed and another fresh ofxiOSEAGLView is created
	+ added ofxiPhoneLaunchBrowser(string url) to ofxiPhoneExtras
	- removed ofFmodSoundPlayer files from iOS+OF project which were causing compile issues with nightly builds
	- removed ofxiPhoneSoundPlayer.mm, was never used
	/ iOS Xcode project valid architecture is now set to armv6 armv7 by default
	/ iOS+OFLib deployment target has been set to 4.0, the base supported version of iOS by OF
	/ fixed buggy compiler error with iOS events
	/ fixed create_package to not delete iOS libraries
	/ ofiPhoneVideoPlayer and AVFoundationVideoPlayer rewritten / cleanedup
	/ fixed AVPlayerItem crash on unloading video
	/ glViewController is now set as the window's rootViewController,
	  this is a much nicer implementation than adding the glViewController's view to the window
	/ fixed ofxiPhoneSendGLViewToFront() and ofxiPhoneSendGLViewToBack() to always work with the GLView superview
	/ GLView is now brought to front using ofxiPhoneSendGLViewToFront()
	/ ofImage::grabScreen fixed for iOS
	/ ofxiPhoneScreenGrab: updated for retina display
	/ isRetinaSupported() made more reliable
	/ fixed accelerometer readings for devices with default landscape orientation (i.e. tablets)
	/ EAGLView, ES1Renderer, and ES2Renderer now do not have any OF dependencies,
	  can now be used to setup openGL ES without OF (if not required)
	/ ofOpenALSoundPlayer: safety fixes for iOS

	ofAppiPhoneWindow
		+ enabling retina was ambiguous, method names are now more concise
		+ added methods for disabling retina, depth, & anti-aliasing
		/ isRetinaSupported() made more reliable
		/ now accesses window position, size, and screen size from the ofxiOSEAGLView

	ofxiOSEAGLView
		+ ofxiOSEAGLView is the OF specific extension of EAGLView,
		   most of the logic from the ofxiPhoneViewController has been move into here,
		   it is now possible to create a ofxIOSEAGLView without the need for a ofxiPhoneViewController container
		+ only accepts ofxiPhoneApp instead of ofBaseApp, this is more strict and safe
		+ moved window size and position code from ofAppiPhoneWindow into ofxiOSEAGLView,
		   makes more sense for ofxiOSEAGLView to return these values and only recalculate them when
		   layoutSubviews() is called on a ofxiOSEAGLView resize

	 ofxiPhoneViewController
		+ now adds itself as a delegate when creating a glView,
		   this is handy to get glView callbacks when extending in another class
		/ completely refactored to only create and set up the glView, now rotates the glView to the device orientation

### Linux
	+ added new method for setting vsync
	- removed unused FMOD scripts
	/ ofOpenALSoundPlayer: bugfixes for Linux

### Windows
	+ added readme for Visual Studio
	/ added missing elif statement for MiniGW, changed the compiler from 4.4.3 to 4.6
	/ ofSystemAlertBoxDialog: fixed for VS2010 and CB Windows
	/ updated to Fmod 4.4
	/ ofSystemUtils: fix windows dialog
	/ ofUtils: fixed VS2010 compilation and dynamic shared pointer casting

CORE ADDONS
------------

	- removed ofAddons.h
	/ ofxOsc: install.xml brought up to date
	/ ofxOpenCV: partially fixed OpenCV runtime errors with MiniGW -  needs newer GCC for proper fix
	/ ofxXmlSettings::setValue() fixed truncated floating point arguments bug introduced in 007
	/ ofxXmlSettings: increased precision when saving double values

PROJECT GENERATOR
------------------

	/ updated project generator template to include QTKit and CoreVideo frameworks on Mac OS X

EXAMPLES
---------

	+ added rectangleAlignmentAndScaling example: demonstrates all of the new ofRectangle alignment and scaling features
	/ videoGrabber example: updated with comments about legacy video settings and info on how to enable alpha movies
	/ asciiVideo example: cleaned-up and clarified, now uses OF convenience functions for lightness and charToString conversion
	/ shader example: fixed for ATI cards
	/ fixed billboard and pointsAsTextures examples
	/ updated easyCam example, now explains the reimplementation of ofEasyCam in greater detail

### Mac
	+ added osxVideoRecorder example demonstrating recording videos to disk on Mac OS X using the ofQTKitGrabber
	+ added osxHighPerformanceVideoPlayer demonstrating accelerated playback of HD videos on OS X using the ofQTKitPlayer

### iOS
	+ moviePlayer example: added video player volume
	/ Mapkit example fixed

### Android
	+ added OpenCVFace face tracking example
	/ soundPlayer example: updated to show multiplay on Android
	/ touch example: bug fixes
	/ image example: updated to fix path to image in loadImage(FILE)


---------------------------------------------------------------
	  .oooo.     .oooo.    ooooooooo   .o
	 d8P'`Y8b   d8P'`Y8b  d"""""""8' o888
	888    888 888    888       .8'   888
	888    888 888    888      .8'    888
	888    888 888    888     .8'     888
	`88b  d88' `88b  d88'    .8'      888
	 `Y8bd8P'   `Y8bd8P'    .8'      o888o

OF 0071
=======

#### key
	+ = added
	- = subtracted
	/ = modified

---------------------------------------------------------------

The main focus for 0071 was to add new examples to better show the features added in 007.
0071 is mainly an 'examples + bug fixes' release, with some additional, much needed, updating to the mobile releases ( iOS and Android ).

Critical changes ( might need to update your projects to fix )
* ofEvents -> ofEvents() singleton - this means that projects which register calls using "ofEvents." will need to replace these with "ofEvents()." (note the parentheses).
* OF Examples now are found in the "examples/" folder in the root of the OF folder. The "apps/" is still included but is kept empty as a place to put your OF apps and experiments.
* Because of the project generator (see below) some addons have been standardized, and we've added "addons.make" to each project file that uses addons.  Prior to this release, addons.make was only used on linux / android platforms.  As we develop the project generator, we'll work to document these changes.
* audioRequested and audioReceived on the ofBaseApp/testApp are now called audioOut and audioIn


Project generator
A significant amount of the work which went into 0071 was in the development of a Project Generator. The Project Generator can make project files for all the OF platforms and it was critical to have this in order to increase the number of examples we could include with the release. The Project Generator is also able to add addons to existing projects, making the task of adding addons much faster than the current manual approach. Going forward, the Project Generator will be included with the new releases and be a key tool for generating, modifying and updating your OF projects. Currently, the Project Generator is in our github repository (https://github.com/openframeworks/openFrameworks) in apps/devApps/.

### core
	+ loads of new examples! 96 examples now come with the desktop releases of OF. For a full list see: https://gist.github.com/2836913
	platform specific:
	+ android support for front camera
	+ android support for ofSoundPlayer
	+ new iOS examples showing how to mix OF with Native Apps
	/ android compatibility  with ndk r7+ and lots of bugfixes
	/ loads of iOS bugfixes - including retina issue, FBO issues, native GUI fixes, video fixes.
	/ XCode fix to avoid constant rebuild of projects
	/ Some MacOS 10.7 fixes ( ofShowCursor / HideCursor )
	/ Better compatiblity with XCode 4
	/ linux: createProjects.py script dropped in favor of command line version of the projectGenerator

### utils
	+ implemented getUserHomeDir() for Windows
	/ ofSystemSaveFileDialog fix for linux
	/ ofFile and ofDirectory fixes for overwrite failing.
	/ ofThread now uses Poco::Thread
	/ bugfix for ofFile::copyFromTo
	/ makefiles now are modular + ensure compatibility with future versions

### addons
	+ OSC allows multiple binds to the same port and support for multicast / unicast
	/ Loads of ofxOpenCV fixes to prevent crashing when images are not allocated or the wrong size
	library changes:
	/ OpenCv library in the ofxOpenCv addon updated to version 2.3.1
	/ Poco library updated to version 1.4.3

### 3d
	+ ofDrawGrid() for showing an multi-axis xyz grid, ofDrawGridPlane() for a single plane
	+ ofDrawArrow() for lines with arrow heads (using ofCone())
	+ ofCone() for draw cone primitives
	+ 3D versions of ofCurve(), ofBezier(), ofVertex() and ofBezierVertex().
	+ ofMesh now allows you to explicitly enable/disable colors/textures/normals before drawing using ofMesh::[enable/disable][Colors/Textures/Normals]()
	+ ofxAssimpModelLoader utility methods
	/ faster ofSphere() without GLUT dependancies.

### math
	+ ofQuaternion::normalize()
	+ ofTexture/ofImage::drawSubsection() for drawing portions of images
	+ ofBackgroundGradient() for gradient app backgrounds, with OF_GRADIENT_CIRCULAR, OF_GRADIENT_LINEAR, OF_GRADIENT_BAR
	+ ofPolyline::getCentroid2D() and ofPolyline::getArea()
	/  fix mismatch between * and *= operators for ofMatrix3x3. Implemented separate hadamard/schur product ofMatrix3x3::entrywiseTimes()
	/ modify ofMatrix4x4::getRotate() method to operate on a scale-invariant version of the matrix
	Graphics:
	/ ofColor fixes for HSB color wrapping
	/ ofPixels optimizations for swapRGB() and mirror() and rotate()
	/ ofPolyline::getSmoothed() works with open and closed lines

### app
	+ ofAppGlutWindow::setDoubleBuffering() gets rid of flickering when drawing without clearing. use this in your main.cpp instead of just saying ofSetBackgroundAuto(false)

### openGL
	+ ofFbo added option to have the depth/stencil buffer as a texture
	/ ofFbo lots of fixes for iOS and POT textures

### communication
	/ ofSerial bugfixes

### sound
	+ ofSoundStream allow for different input and output devices.

For a full list of commits for 0071 see: https://gist.github.com/2836647

---------------------------------------------------------------

		  0000             0000        7777777777777777/========___________
	   00000000         00000000      7777^^^^^^^7777/ || ||   ___________
	  000    000       000    000     777       7777/=========//
	 000      000     000      000             7777// ((     //
	0000      0000   0000      0000           7777//   \\   //
	0000      0000   0000      0000          7777//========//
	0000      0000   0000      0000         7777
	0000      0000   0000      0000        7777
	 000      000     000      000        7777
	  000    000       000    000       77777
	   00000000         00000000       7777777
		 0000             0000        777777777

OF 007
======

#### key
	+ = added
	- = subtracted
	/ = modified

---------------------------------------------------------------

This is the biggest release yet. There are a ton of changes. The 0062 core was 624KB over 49 files. The 007 core is 1.12MB, 148 files. In other words, the core of OF has basically doubled.

Some of the favorite changes to OF include:
* PDF and SVG export with cairo
* ofMesh, ofPolyline utils and ofPath tessellation
* advanced ofColor support, like fromHex() and fromHsb()
* web-based resource and image loading
* advanced ofPixels power like getColor(x, y)
* the new Assimp addon
* advanced logging capabilities
* the ofPtr shared pointer, and shallow copy semantics: it's now safe to copy ofTexture
* moving ofxVectorMath, ofxDirList, ofxThread, and the previously unofficial ofxShader, ofxFbo, ofxVbo all into the core

Here's a fairly complete list of the changes.

### core changes
	+ drag and drop support in applications (except in linux)
	+ ofPtr for shared pointers, is actually std::tr1::shared_ptr and will be std::shared_ptr when c++11 is supported
	/ ofVideoGrabber, ofVideoPlayer, ofSoundPlayer and ofSoundStream use "swappable apis". so does rendering, which means you can use cairo and output to PDFs or SVG for 2d and 3d.
	/ new fps counting technique that works a little better across platforms
	/ ofApp*Window have been refactored so now all the common functionality is in ofEvents which makes easier to create new windows and maintain the current ones
	/ easier registering of core events with the new ofRegister*Events functions
	/ all the classes with opengl resources have shallow copy + shared pointer semantics so textureA = textureB makes textureB point to the same texture in the graphics card as textureA, and the texture is released when the last copy is destroyed
	/ all swappable api objects (ofVideoGrabber, ofVideoPlayer, ofSoundPlayer and ofSoundStream) have shallow copy + shared pointer semantics too
	/ all memory structures (ofPixels, ofImage, ofBuffer, ofMesh...) have deep copy semantics, so pixelsA = pixelsB makes b a full new copy of pixelsA
	/ Poco library is changed from 1.3.3 to 1.4

### video and cv
	+ ofxCvHaarFinder is brought into the core ofxOpenCv addon
	/ openCV is updated to 2.2
	/ better gstreamer support on linux

### 2d
	+ ofImage uses new web-based loading
	+ ofPath and ofPolyline wraps all the 2d drawing functionality previously supported by OF, and adds more. ofPolyline has some new advanced methods like getResampledBySpacing(), getClosestPoint(), simplify()
	+ ofSetOrientation() allows you to rotate the entire OF drawing space
	+ ofPixels and ofImage have getColor() which returns an ofColor, and setColor(x,y,ofColor)
	+ ofEnableBlendMode() supports additive, subtractive, alpha, etc. blending
	+ ofColor has constants for 8 basic colors (cmy, rgb, bw), for example ofColor::magenta
	+ ofColor finally adds HSB support. ofColor is always an RGB color, but you can get and set its hue, saturation and lightness via getHue()/setHue() etc. it also distinguishes lightness (value) from brightness. to create a color using HSB values, use ofColor::fromHsb(h,s,b,a). to set from hex, use ofColor::fromHex(hex)
	/ ofColor, ofPixels and ofImage are now templated classes with support for 8bit, 16bit and 32bit and easy conversion between them using the = operator
	/ ofDrawBitmapString() has been massively refactored to support all the different 3d drawing cases you can imagine via ofSetDrawBitmapMode()
	/ ofPixels is now a fully-formed class that is used by a variety of classes
	/ ofSetColor(int) is now interpreted as a grayscale value, ofSetHexColor(int) if you want to use hex values
	/ ofTrueTypeFont, now generates a texture atlas when loading a font instead of a texture per char. It also has bind()/unbind() functions that allow to bind the texture only once to render several texts, making font rendering much faster.
	/ ofTrueTypeFont getShapes uses ofPath
	/ new ofTessellator class used internally in OF by ofPath and ofBegin/EndShape bassed on the tess2 library (with some modifications so it also works on openGL ES) which is faster and has a cleaner api than the previous one from GLU

### 3d
	+ ofMesh, ofVbo and ofVboMesh provide a clean interface for drawing, and for gpu-accelerated drawing
	+ ofNode is a new 3d node class with position and orientation that can be manipulated with methods like truck() and tilt()
	+ ofCamera extends ofNode, is a new camera class for representing a camera position, orientation, and other parameters
	+ ofEasyCam wraps ofCamera with automatic mouse binding that resembles processing's PeasyCam
	+ ofLight also extends ofNode, provides a simple interface for placing and describing opengl lighting
	+ ofBox and ofSphere draw the respective 3d shapes
	+ multiple versions of ofxShader, ofxVbo and ofxFbo have been brought into the core as ofShader, ofVbo, and ofFbo
	+ ofxAssimpModelLoader for loading a huge variety of 3d formats

### utils
	+ functions and classes to load resources from the web to memory, or save to disk. using HTTP, either synchronously and asynchronously: ofSaveURLTo(), ofSaveURLAsync()
	+ added the ofxThreadedImageLoader addon
	+ cross-platform file dialog and alert dialog, ofSystemAlertDialog(), ofSystemLoadDialog(), ofSystemSaveDialog()
	+ a ton of conversion methods have been added: ofToHex, ofToBinary, ofToString, ofHexToInt, ofHexToFloat, etc. they're all inside ofUtils.h
	+ ofFile and ofBuffer have also been added for working with the filesystem
	/ the core addons ofxVectorMath, ofxThread, and ofxDirList have been turned into ofVectorMath, ofThread and ofDirectory
	/ new logging system with support for << operator, for example: ofLogVerbose() << "some message", logging to file and logging channels which makes it easier to redirect the logging output

### sound
	+ working towards a sound synthesis addon, ofxSynth
	/ slowly transitioning away from fmod toward openal for sound support
	/ ofSoundStream has been converted to an object oriented api, and now is possible to open several devices and choose which device you want to use.
	- FMOD is removed on linux, so there are no dependencies with any non-free library

### mobile
	+ android support
	/ better iPhone video and camera support

---------------------------------------------------------------
	  .oooo.     .oooo.       .ooo     .oooo.
	 d8P'`Y8b   d8P'`Y8b    .88'     .dP""Y88b
	888    888 888    888  d88'            ]8P'
	888    888 888    888 d888P"Ybo.     .d8P'
	888    888 888    888 Y88[   ]88   .dP'
	`88b  d88' `88b  d88' `Y88   88P .oP     .o
	 `Y8bd8P'   `Y8bd8P'   `88bod8'  8888888888

OF 0062 Pre-release
===================

#### key
	+ = added
	- = subtracted
	/ = modified

---------------------------------------------------------------

This is mostly minor changes, as it's a maintenance release with small bug fixes a few extra features and critical updates for Visual Studio 2010, Linux and iPhone developers.

	/ altered framerate calculation
	/ altered data path, adding ofSetDataPathRoot for OSX
	/ ofSerial - bug fixes, including EAGAIN
	+ added touchEvents
	/ ofDrawBitMapString - added openglES compatible mode
	+ ofGraphics - added universal functions to help control textures, such as "ofEnableNormalizedTexCoords()"
	+ ofImage - setCompression code via SoSoLimited
	+ ofTexture - added compression options, via SoSoLimited
	/ ofSoundStream - bug fixes for audio clearning
	/ ofConstants - switch some includes, for example, from <stdio.h> to <cstdio>
	+ ofMath - added some functions to help with angle math, such as ofLerpDegrees, and ofAngleDifferenceDegrees
	+ ofMath - added some polygon / polyline / line methods, such as ofInsidePoly and ofLineSegmentIntersection
	+ ofMath - added curve functions, ofBezierPoint, ofBezierTangent, ofCurvePoint, ofCurveTangent
	+ ofTypes - added functionality to ofRectangle, added [] options for ofColor
	- ofTypes - removed getPixels() from ofBaseVideo, as is was unnecessary
	+ ofUtils - new ofSetDataPathRoot code for OSX, changes to ofToDataPath


---------------------------------------------------------------
		  :::::::   :::::::   ::::::::    :::
		:+:   :+: :+:   :+: :+:    :+: :+:+:
	   +:+   +:+ +:+   +:+ +:+          +:+
	  +#+   +:+ +#+   +:+ +#++:++#+    +#+
	 +#+   +#+ +#+   +#+ +#+    +#+   +#+
	#+#   #+# #+#   #+# #+#    #+#   #+#
	#######   #######   ########  #######

OF 0061 Pre-release
===================

#### key
	+ = added
	- = subtracted
	/ = modified

---------------------------------------------------------------
### major changes

- In this release we've moved to a *compiled* openFrameworks library as opposed to un-compiled.  a few motivations:  cutting down on directory size of OF, reducing redundant compiles, making compile times faster and making development of OF easier.   This changes quite a bit the organization of projects but it should lead to a much easier to use and maintain OF.

- we've moved from svn to git / github, and now have an automated system of building the OF package.  This should also greatly ease our development of openFrameworks.  Before we had separate SVNs for the OF lib, the OF examples, and the core addons.  Now, we've got one major place where changes and development of OF can be tracked.   see http://github.com/openFrameworks/openFrameworks.

- To encourage faster releases, we've moved to a major / minor release schedule, with minor releases (0061, 0062) relating to bug fixes and major releases (007, 008) relating to major features.

### core

	/ folder structure is now changed so that all ide/packages can share one directory structure. Download the 0061 all release to check it out.
	- core addons now don't have libs from other platforms
	- core addons now don't have example project in the addons folder
	/ core FreeImage.h -> move BOOL typedefs etc to #defines and do #undef at the end
	+ Put MIT license in OF headers

### OSX

	+ xcode has snow leopard fixes, and an SL release
	/ xcode projects are now renamed to have the folder name (ie, every project has a unique target name)
	/ xcode apps now have max optimization settings for release.
	+ xcode now has 3 build modes, Debug, Release, Release Universal.  Debug and Release are for your current platform (either intel or PPC), Release Universal is for both.
	/ renamed libs - change all libSomething.a to be something.a - this prevents xcode linking to root level dynamic libraries.

### Win32 CodeBlocks

	+ added Poco linker flag for CB - needed for some network calls
	/ fixed Poco linking order on win32.

### linux

	+ changed the linux default videograbber to gstreamer
	- remove ofV4lUtils
	/ smarter makefiles
	/ modified install scripts to fix karmic problems &amp; build OF compile on install

### examples

	/ fixed missing files, missing calls to windowResized, removed unnecessary calls to ofSetupSCreen() and include stdio.

### app

	/ Fullscreen on OS X is not top level anymore (no more crazy fullscreen app crashes and can't escape).
	/ OSX now using custom GLUT framework
	/ ofSetBackgroundAuto(false) on windows OF now works for accumulation in both windowed and fullscreen mode.
	/ Windows FPS issues have been fixed.
	/ closing the window on PC now exits the application properly.
	/ ofAppGlutWindow - frameRate is now set to 60.0 at the start - before it was un-initialized.
	+ ofGetLastFrameTime - to get the elapsed time of the last frame.
	/ fixes for ofGetWidth() &amp; ofGetHeight() being super slow, we now store them in vars and change on windowResize
	+ ofGetAppPtr(), returns a pointer to the baseApp.
	+ glutInitDisplayString option for initializing glut with a unique string, useful for FSAA or altering the setup.
	/ fixed vertical sync for linux: http://www.openframeworks/forum/viewtopic.php?f=7&amp;t=561&amp;p=10683#p10683
	+ ofBaseVideo added isFrameNew()

### utils

	+ ofxNoise / ofxSignedNoise - Added simplex ( similar to perlin ) noise to core
	+ ofBaseHasTexture now has ofSetUseTexture ( to enable / disable use of the texture )
	+ ofBaseImage and ofBaseVideo have the same inheritance base
	/ ofMap - now has a clamp argument, defaults to false
	/ ofMap - added kyle's solution for division by 0 in ofMap: http://www.openframeworks/forum/viewtopic.php?f=6&amp;t=1413&amp;view=unread#unread
	/ detect C:/ as a root path in ofToDataPath
	/ fixed ofPoint warnings

### events

	/ fixed vs2008 intellisense doesn't like using Poco::delegate; in ofEventUtils: http://www.openframeworks/forum/viewtopic.php?f=9&amp;t=1877

### graphics

	/ ofImage - Loading a corrupted image in linux made the app crash, fixed now.
	/ ofImage setImageType now should properly alter the texture if we need to.
	+ ofTexture - get texPoint / texPercent, should help for doing texCoordinate stuff without knowing the target of the texture (ie, RECTANGLE_2D or ARB)
	+ ofTexture - setTextureWrap &amp; setMinMagFilters
	+ ofTexture - ofTexture float *, char *, short * -- added ability to make floating point textures, or upload float data.
	/ ofTrueTypeFont - fixed TTF mem leak - http://www.openframeworks/forum/viewtopic.php?p=10178&#p10178
	/ ofImage fixed resize color bug
	/ ofImage inline functions cause problems with inhereited classes that want to use them in CB. Switch inline -> static fixes this issue.
	/ ofGraphics - numCirclePts was being stored on ofPushStyle but not used, fixed now.
	/ ofTrueTypeFont - Xcode projects now use the same version of the freetype as win/linux
	+ ofSetCurveResolution - makes curved shapes a lot faster!
	/ setAnchorPoint - move arguments from int to float

### communication

	/ merged ofStandardFirmata into ofArduino, and removed ofStandardFirmata files from all projects.
	/ fixed Linux serial issues with device number.
	/ fixed firmata connect inconsistency: http://www.openframeworks/forum/viewtopic.php?t=1765
	/ serial enumerateDevices is doing ofLog so not always printing out: http://www.openframeworks/forum/posting.php?mode=reply&amp;f=6&amp;t=1860
	/ fix for ofArduino: http://www.openframeworks/forum/viewtopic.php?f=8&amp;t=3042&amp;p=16425#p16425

### video

	+ add setFrameRate for ofVideoGrabber
	/ update the quicktime settings dialog so that PS3 eye panel shows up.
	/ ofVideoPlayer and ofVideoGrabber on OS X Intel now using k24RGBPixelFormat - this has a huge speed improvement.
	/ ofVideoGrabber on mac seems to not list devices unless log level is verbose http://www.openframeworks/forum/viewtopic.php?f=9&amp;t=1799&amp;view=unread#unread
	/ ofVideoPlayer uninitialized bool
	/ ofVideoPlayer - pause delay
	/ ofUCUtils - PIX_FMT_YUV422 won't exist anymore on future ffmpeg versions
	/ ofUCUtils - sws_getContext needs PixFmt as parameter instead of int since karmic

### sound

	/ updated ofSoundStream to use rtAudio 4
	+ Add a ofSoundPlayerUpdate call - for keeping fmod on track - should be called once per frame to make sure sounds don't drop out.

### addons

	/ ofxXmlSettings fixed a printf(tag.c_str()) that xcode complained about
	/ ofxXmlSettings memory leak
	/ ofxXmlSettings read attributes
	/ ofxXmlSettings read from string / save to string

	ofxVectorGraphics

		+ Update to CreEPS 2.0 - has MIT license and new features.

	ofxOpenCv

		+ ofxCvColorImage - convertToPlanarImage(...) for one channel (ie, grabbing just sautration)
		/ ofxCvShortImage - fixed assignment operators
		+ ofxCvGrayscaleImage - add brightness / contrast to grayscale image
		/ ofxCvContourFinder converts centroid to ints
		/ ofxOpenCv - fixed grayscale to planar / planar to grayscale conversions
		- ofxOpenCv - remove implicit ROI intersection
		+ ofxCvImage - re-added drawBlobIntoMe() for painting a blob into an ofxCvImage
		+ ofxOpenCv - added appropriate iphone #ifdefs
		/ ofxOpenCv - do quicker copy for getPixels and setPixels when image is memory alligned already.
		/ ofxOpenCv scale conversions for floatImage
		/ ofxOpenCv swapTemp bug

	ofxOsc

		/ ofxOsc - shutdown leak
		/ ofxOsc - memory leak
		/ ofxOsc - Shutdown issue - fix here: http://damian.dreamhosters.com/svn/public/ofxOsc/latest/

	ofxThread

		/ stopThread now called when threaded function returns

	ofxVectorMath

		/ went back to inlining, for performance
		+ added ofxMatrx4x4 and ofxQuaternion
		/ ofxPoin2/3/4f deprecated merged with ofxVec2/3/4f

	ofxNetwork

		/ fixed for xcode needs tcp.h included in ofxUdpManager

---------------------------------------------------------------

	  .oooo.     .oooo.       .ooo
	 d8P'`Y8b   d8P'`Y8b    .88'
	888    888 888    888  d88'
	888    888 888    888 d888P"Ybo.
	888    888 888    888 Y88[   ]88
	`88b  d88' `88b  d88' `Y88   88P
	 `Y8bd8P'   `Y8bd8P'   `88bod8'

OF 006 Pre-release
==================

#### key
	+ = added
	- = subtracted
	/ = modified

---------------------------------------------------------------
### major changes

- in this release we've added another library Poco, v 1.3.3 ( http://pocoproject.org ),
just now only used for events but is included in the core available for addons like ofxHttpUtils.
For java folks, Poco is a bit like the java sdk (java.io, java.util, java.net, etc)
- added the event system for base events (setup, update, draw, mouse, keyboard) and constructing new events.
- dropped the addons.h system in place of direct inclusion of addons .h files.  Old code will get warnings but not break for this release
- abstracted the window toolkit, so that other windowing toolkits besides glut can be used with OF, such as iphone, glfw, non-windowed (command line).
- all graphics have been opengl-es-ified, allowing for iphone support
- all objects have virtual destructors (thanks memo for the tip)
- all objects that draw can return their internal texture
- adding base types for OF object (ofBaseDraws, ofBaseHasPixels) which should allow better OOP programming with OF
- added advanced user functionality, like choosing the ARB extension usage, disabling setupScreen, binding of textures, etc.
- tons and tons of bug fixes and small functions (ofClamp, push and pop styles) that should make programming in OF easier

---------------------------------------------------------------

### app

	/ ofAppRunner - setup update draw should be called in the right order (prior was setup, draw, update)
	+ ofAppBaseWindow / ofAppGlutWindow - glut abstraction and base window toolkit, tested with glfw, iphone and no window
	+ disable / enable setup screen
	/ ofSimpleApp - is now ofBaseApp
	/ ofSimpleApp - mouseRelease receives now x, y and button
	+ ofSimpleApp - added a windowResized function called when the window size changes

### utils

	+ ofTypes - added an inheritance hierarchy, ofBaseDraws, ofBaseUpdates, ofBaseHasTexture, ofBaseHasPixels and ofBaseVideo
	/ ofTypes - added operator overloading to ofPoint
	/ ofUtils - ofToDataPath can now be deactivated or queried for absolute path
	+ ofUtils - ofSplitString for tokenization
	+ ofUtils - added ofLog and logging system with warning levels
	/ ofUtils - ofLaunchBrowser now works on linux
	+ ofMath - added ofNormalize, ofMap, ofClamp, ofLerp, ofDist, ofDistSquared, ofSign, ofInRange, ofRadToDeg, ofDegToRad, ofRandomWidth, ofRandomHeight
	( thanks todd, memo. kyle )
	/ ofMath - better seeding for ofRandom

### graphics

	+ ofTrueType - added that ability to get fonts as polygons, bezier interpolation, with polygon simplification for performance
	+ ofGraphics - added an ofColor type, for push and pop style
	+ ofGraphics - ofRestoreGraphicsDefaults, for reseting all graphical changes
	/ ofGraphics - ofEllipse fixes
	/ ofGraphics - changed ofCircle from display list to a pre cache vertex
	list - faster!
	+ ofGraphics - basic tranformations and opengl wrapping, ie, ofRotate(), ofRotateX(), ofRotateY(), ofRotateZ(),
	+ ofGraphics - added an ofStyle struct - it holds color, line width, circle resolution, blend mode, smoothing, fill mode
	+ ofGraphics - push style / pop style
	 getStyle and setStyle to get and set the style
	+ ofGraphics - ofSetLineWidth
	/ ofGraphics - fixed a bug with projection vs modelview matrix order -

http://www.openframeworks/forum/viewtopic.php?p=5213#5213

	+ ofGraphics - changed immediate mode to vertex arrays for compatibility with opengl es
	+ ofGraphics / ofBitmapFont - wrapped the glut bitmapped font as a separate file, so that ofDrawBitmapString can be used in non glut windowing kits.
	+ ofTexture - ofTextureData contains info about the texId, etc so that advanced folks can bind
	+ ofTexture - calls to bind / unbind
	+ ofTexture - setAnchorPercent / setAnchorPoint
	+ ofTexture - enable / disable our texture "hack", a small padding that helps ofTextues look good in
	+ ofTexture - bUseARBExtention is controllable on allocate (and also, controllable globally), which can help make shaders work better
	/ ofImage - restructured for cleaner code -- freeImage moved to only a few places
	/ ofImage - grayscale saveImage bug fixed
	/ ofImage - grayscale setFromPixels fix -- http://www.openframeworks/forum/viewtopic.php?t=929
	/ ofImage - allocate now calls update -- http://www.openframeworks/forum/viewtopic.php?t=835
	/ ofImage - 8bpp fix - http://www.openframeworks/forum/viewtopic.php?t=712
	/ ofImage - grayscale save fix - http://www.openframeworks/forum/viewtopic.php?t=943
	/ ofImage - fixed loadImage() not returning true if successful
	/ ofImage - added setAnchorPercent / setAnchorPoint
	/ ofImage - fixed resize color swap problem - http://www.openframeworks/forum/viewtopic.php?t=1242

### events

	+ added an event system that allows objects receive core events ( draw, mouse, key...), and to create other events.

### video

	/ ofVideoGrabber - (quicktime) much better device listing / selection.
	/ ofVideoGrabber - (quicktime / mac) fixed isFrameNew always returning true
	/ ofVideoGrabber - (linux) some changes to size and format detection
	/ ofVideoGrabber - (linux) better support for yuv and other colorspaces (won't work on ubuntu hardy because of a problem with the ffmpeg version)
	/ ofVideoGrabber - (linux) resize and colorspace conversion through ffmpeg
	/ ofVideoGrabber - (linux) corrected errors on close
	/ ofVideoGrabber - (windows) using new video input library (0.1995)
	+ ofVideoPlayer - (quicktime) added rtsp for quicktime -

http://www.openframeworks/forum/viewtopic.php?p=7665#7665

	+ ofVideoPlayer - added a frame selection system, ie, goToFrame(x), getNumFrames(), goToFirstFrame(), getCurrentFrame(), goToNextFrame(), goToPreviousFrame()
	+ ofVideoPlayer - added getIsMovieDone() - let's you know if the movie hit the end
	/ ofVideoPlayer - (linux) all functionality working now
	/ ofVideoPlayer - (linux) changed fobs out for gstreamer
	/ ofVideoPlayer - (linux) sound in videos

### communication

	/ ofSerial - fixed read byte errors
	/ ofSerial - verbose, with non reads
	/ ofSerial - 0 / -1 error reporting better
	+ ofSerial - flush, allows you to clear the serial buffer
	+ ofSerial - available() lets you query how many bytes are available.
	+ ofStandardFirmata - added this class (thanks erik!) for easier serial support
	+ ofArduino - added this class which extends ofStandardFirmata with specific arduino functions

### sound

	/ ofSoundPlayer - (linux) alsa as default backend

addons changes that are part of FAT package

in this release, we've moved the use of ofAddons.h and the define system.
now, we just include the main .h file per addon.

	/ ofxNetwork - disconnection detection
	/ ofxNetwork - no data loss on tcp
	+ ofxNetwork - udp examples

	/ ofxThread - Applied the fix for startThread checking it is already
	running and moving mutex creation -

http://www.openframeworks/forum/viewtopic.php?p=7221#7221

	+ ofxVectorMath - add copy constructor - http://www.openframeworks/forum/viewtopic.php?t=617
	/ ofxVectorMath - fixed ofxVec2f::perpendicular bug, http://www.openframeworks/forum/viewtopic.php?t=4835
	/ ofxVectorMath - fixed /= operator
	+ ofxVectorMath - added some better names for functions (for example, getRotated() instead of rotated()).  All "d" names, ie rotated, normalized are deprecated for the next release)

	+ ofxOpenCv - added ROI functionality for all ofxCvImage
	/ ofxOpenCv - ofxContourFinder fixed getBlob(), which existed only in .h before
	/ ofxOpenCv - consolidated ofxCvImage - most functionality now across all image types
	/ ofxOpenCv - operator overloading changed to const
	/ ofxOpenCv - scaleIntoMe bug is fixed, http://www.openframeworks/forum/viewtopic.php?t=828
	/ ofxOpenCv - reallocation bug fixed, http://www.openframeworks/forum/viewtopic.php?t=935
	/ ofxOpenCv - warns on non odd valued blur param

	+ ofxOsc - now supports bundles
	+ ofxOsc - better error handling

---------------------------------------------------------------

	  .oooo.     .oooo.     oooooooo
	 d8P'`Y8b   d8P'`Y8b   dP"""""""
	888    888 888    888 d88888b.
	888    888 888    888     `Y88b
	888    888 888    888       ]88
	`88b  d88' `88b  d88' o.   .88P
	 `Y8bd8P'   `Y8bd8P'  `8bd88P'

OF 005 Pre-release
==================

#### key
	+ = added
	- = subtracted
	/ = modified

---------------------------------------------------------------
### major changes

in this release we introduced a "FAT" package as well as an addons.h system that uses #defines.
removed the ofCore to fix some include issues.

	/ better ofMain / addons system
	- no ofCore

---------------------------------------------------------------
### other changes

### app

	+ ofAppRunner - recording of the screens original position
	/ ofAppRunner - jorge's fix for idle / setFrameRate()

http://www.openframeworks/forum/viewtopic.php?t=515&amp;highlight=frame+rate

	/ ofAppRunner - mac hide cursor

### communication

	/ ofSerial - joerg's fixes for pc serial
	/ ofSerial - better support for non "COM" ports
	/ ofSerial - improved write bytes
	/ ofSerial - no longer using "string" for serial, which was failing for binary data. that was a terrible idea.
	/ ofSerial - returning -1 for errors on read byte;

### graphics

	/ ofGraphics - fixed the tesselator to work on different versions of xcode
	/ ofImage - moved constants to ofConstants
	/ ofImage - made freeImage stuff more integrated
	+ ofImage - added copy and equals overloaders, so that imageA = imageB will work as intended
	+ ofImage - added a clear() function
	/ ofImage - better cloning
	+ ofTexture - added copy and equals overloaders to prevent pass by copy
	+ ofTrueTypeFont - getStringBoundingBox() to get the bounding box

### utils

	+ ofTypes - added ofTypes, for base types like ofPoint, ofRectangle
	+ ofConstants - added a #define WIN32_LEAN_AND_MEAN for window
	+ ofConstants - some GLUT defines that help for windows
	/ ofConstants - disabled another VS warning
	+ ofConstants - added some std stuff, like iostream and vector
	/ ofConstants - backspace vs del fixes for OSX

http://www.openframeworks/forum/viewtopic.php?t=494

	+ ofUtils - added ofGetYear, ofGetMonth,ofGetDay(),ofGetWeekDay()

### sound

	+ ofSoundPlayer - added isStreaming field
	/ ofSoundStream  - fixed ofSoundStreamEnd
	/ ofSoundStream  - renamed ofSoundStreamEnd ofSoundStreamClose

### video

	+ ofVideoGrabber - added linux support for unicap as well as V4L, settable in ofContstants
	+ ofVideoGrabber - added ofUCUtils
	+ ofVideoGrabber - tons of V4L fixes
	+ ofVideoPlayer - createMovieFromURL for quicktime
	/ ofVideoPlayer - fix for bHavePixelsChanged on QT (isFrameNew returning true)

---------------------------------------------------------------
	  .oooo.     .oooo.         .o
	 d8P'`Y8b   d8P'`Y8b      .d88
	888    888 888    888   .d'888
	888    888 888    888 .d'  888
	888    888 888    888 88ooo888oo
	`88b  d88' `88b  d88'      888
	 `Y8bd8P'   `Y8bd8P'      o888o

OF 004 Pre-release
==================

#### key
	+ = added
	- = subtracted
	/ = modified

---------------------------------------------------------------
### major changes

- fixed an issue with addons include, now its a system with ofCore.h / ofMain.h / ofAddons.h
- addons folder added to the setup

---------------------------------------------------------------
a brief explanation about addons:

now the structure look like:

- apps
- libs
- addons

where libs are the core libraries for OF (which shouldn't change much), and addons is everything else.

Adding a library will work in the following way:

a) add to the addons folder everything downloaded
b) add to addons.h the "addon.h" lines (found in addon instructions), such as:

	#ifdef OFADDON_USING_OF_CV
		#include "ofCvMain.h"
	#endif

c) follow any steps about adding includes, sources, and libs to the project (or makefile)
d) when you want to use the addon, use the #define before including ofMain in testApp.h, such as:

	#define	 	OFADDON_USING_OF_CV
	#define	 	OFADDON_USING_OF_VECTOR_MATH
	#include 	"ofMain.h"
e) place any dll / .so files in the right place (after compiling)

---------------------------------------------------------------
### other changes

	/ 	ofGraphics - fixed a bug with OS X, 10.5 compiling
	/ 	ofSerial - fixed a bug with win32 destructor
	/ 	ofVideoGrabber - mac grabber fixed bug with settings loading
	/ 	ofMain - is now just ofCore and ofAddons
	+ 	ofAddons - is the place where addons will go
	+ 	ofCore - is the what ofMain was, all the "core" OF code  (internal and addons should include "core" not main to avoid recursive linking)
	/ 	movie grabber example - win32 / fixed an issue where some of the code was commented out.

---------------------------------------------------------------

	  .oooo.     .oooo.     .oooo.
	 d8P'`Y8b   d8P'`Y8b  .dP""Y88b
	888    888 888    888       ]8P'
	888    888 888    888     <88b.
	888    888 888    888      `88b.
	`88b  d88' `88b  d88' o.   .88P
	 `Y8bd8P'   `Y8bd8P'  `8bd88P'

OF 003 Pre-release
==================

#### key
	+ = added
	- = subtracted
	/ = modified

---------------------------------------------------------------
### major changes

- start of a structure for of addons
- all classes feature protected variables instead of priavete for easier extending
- linux is *really* integrated into the codebase, this is BIG!
- fmod is updated to fmodex
- glu is included for tesselation routines
- ofSerial is a class now
- much more careful classes in terms of memory usage
- much better closing routine
- string is used everywhere we can instead of char * or char arrays (much usage stays the same though)
- ofVertex for curves, more graphics options added
- every app now uses a "data" folder for any OF loading and saving media

---------------------------------------------------------------
### other changes

(there are likely more, this is what we can remember but we will update as we update the API)

	+ 	ofTexture - non power of 2 when possible using GLEE / ARB extensions
	+ 	ofGraphics - curves: beziers and curve vertex, etc.
	+ 	ofGraphics - polygons (poly shapes) using ofBeginShape() and tesselation
	+ 	ofGraphics - polygon w/ multiple contours for holes (using ofNextContour())
	+ 	ofImage -  copy image operator (clone)
	+ 	ofImage - upload data (fix rgb/bgr issues)
	/ 	whole code - remove all the top left bools, top left as 0,0 is fixed in the code now.
	+ 	ofVideoGrabber - (OSX) Save user preferences with qtVideoCapture dialog
	+ 	ofVideoGrabber - new VI input lib in for video grabber
	/ 	ofVideoGrabber / ofVideoPlayer - isFrameNew() behaves better, newness is per idle call
	+ 	ofSimpleApp - key release in addition to key press
	/	ofSimpleApp - special keys coming through uniquely (see constants.h)
	+ 	ofAppRunner - ofFullScreen opition to alter the screen
	+ 	ofAppRunner - get monitor info (w/h) and position window options
	+ 	ofAppRunner - glut game mode
	/ 	whole code - no "../../../" BS, we now have data folders!
	+ 	ofAppRunner - set window title
	+ 	ofUtils - open a URL in default browser
	+ 	ofConstants - clamp in ofConstants.h
	+  	ofUtils - simple version printout
	/ 	whole code - destructors everywhere

---------------------------------------------------------------

	  .oooo.     .oooo.     .oooo.
	 d8P'`Y8b   d8P'`Y8b  .dP""Y88b
	888    888 888    888       ]8P'
	888    888 888    888     .d8P'
	888    888 888    888   .dP'
	`88b  d88' `88b  d88' .oP     .o
	 `Y8bd8P'   `Y8bd8P'  8888888888

OF 002 Pre-release
==================

#### key
	+ = added
	- = subtracted
	/ = modified

---------------------------------------------------------------
### major changes
- added ofSerial class for serial communication
- added ofSoundPlayer class for fmod / sound sample playing
- ofAudio now named ofSoundStream to make more sense
- added glee and fmod libraries to the path
- added videoInput library on windows for grabbing video (via direct show)
- renamed ofQtVideoGrabber to ofVideoGrabber (since it doesn't nec.
use quicktime)
- renamed ofQtVideoPlayer to ofVideoPlayer
- lots of fixes to get rid of warnings
- xcode libs now all universal.
- xcode can now build universal apps when in 'Release' mode.

---------------------------------------------------------------

### others
	+ ofSimpleApp - mouseX, mouseY now added for p5 compatability
	/ ofSimpleApp - fixed some bugs, like mouseMoved before the windows is open
	+ ofUtils - ofSetFrameRate() to set a target framerate
	+ ofUtils - ofSetVertical sync to set vertical sync on or off
	/ ofGraphics - 0,0 is now top left by default
	+ ofGraphics - ofEnableSmoothing(), works just for lines for now.
	+ ofGraphics - ofSetCornerMode for drawing rects on center or corner
	+ ofGraphics - ofSetBackgroundAuto() to enable manual background
	clearing (works for fullscreen)
	+ ofTexture - loadScreenData(), upload screen data to texture
	/ ofTrueTypeFont - many bug fixes for the black edges and cut off curves
	/ ofVideoPlayer - setPaused() bug fix
	+ ofVideoGrabber - device selection options
	+ ofVideoGrabber - multiple devices works both qt and dshow
	+ ofConstants - added some high res timer code in win32, accessible
	through commenting in a #define and recompiling
	+ ofConstants - ability for win32 to choose quicktime or directshow (videoInput)
	through #define

---------------------------------------------------------------

most ascii art generated with http://patorjk.com/software/taag/ using the font 'roman'

---------------------------------------------------------------
