project('openFrameworks', 'cpp',
version : '0.12.0',
default_options : [
  'cpp_std=c++20',
  'buildtype=release',
  'optimization=3',
  'default_library=shared',
  # 'prefer_static=true',
  'b_ndebug=if-release',
  'c_args=-pipe',
  'cpp_args=-pipe',
  # 'warning_level=3'
]
)

inc = include_directories(
  'libs/openFrameworks/3d',
  'libs/openFrameworks/app',
  'libs/openFrameworks/communication',
  'libs/openFrameworks/events',
  'libs/openFrameworks/gl',
  'libs/openFrameworks/graphics',
  'libs/openFrameworks/math',
  'libs/openFrameworks/sound',
  'libs/openFrameworks/types',
  'libs/openFrameworks/utils',
  'libs/openFrameworks/video',
  'libs/openFrameworks',
)

sources = files(
  'libs/openFrameworks/3d/of3dPrimitives.cpp',
  'libs/openFrameworks/3d/of3dUtils.cpp',
  'libs/openFrameworks/3d/ofCamera.cpp',
  'libs/openFrameworks/3d/ofEasyCam.cpp',
  'libs/openFrameworks/3d/ofNode.cpp',
  'libs/openFrameworks/app/ofAppGLFWWindow.cpp',
  'libs/openFrameworks/app/ofAppNoWindow.cpp',
  'libs/openFrameworks/app/ofAppRunner.cpp',
  'libs/openFrameworks/app/ofBaseApp.cpp',
  'libs/openFrameworks/app/ofMainLoop.cpp',
  'libs/openFrameworks/communication/ofArduino.cpp',
  'libs/openFrameworks/communication/ofSerial.cpp',
  'libs/openFrameworks/events/ofEvents.cpp',
  'libs/openFrameworks/gl/ofBufferObject.cpp',
  'libs/openFrameworks/gl/ofCubeMap.cpp',
  'libs/openFrameworks/gl/ofFbo.cpp',
  'libs/openFrameworks/gl/ofGLProgrammableRenderer.cpp',
  'libs/openFrameworks/gl/ofGLRenderer.cpp',
  'libs/openFrameworks/gl/ofGLUtils.cpp',
  'libs/openFrameworks/gl/ofLight.cpp',
  'libs/openFrameworks/gl/ofMaterial.cpp',
  'libs/openFrameworks/gl/ofShader.cpp',
  'libs/openFrameworks/gl/ofShadow.cpp',
  'libs/openFrameworks/gl/ofTexture.cpp',
  'libs/openFrameworks/gl/ofVbo.cpp',
  'libs/openFrameworks/gl/ofVboMesh.cpp',
  'libs/openFrameworks/graphics/of3dGraphics.cpp',
  'libs/openFrameworks/graphics/ofBitmapFont.cpp',
  'libs/openFrameworks/graphics/ofCairoRenderer.cpp',
  'libs/openFrameworks/graphics/ofGraphicsBaseTypes.cpp',
  'libs/openFrameworks/graphics/ofGraphicsCairo.cpp',
  'libs/openFrameworks/graphics/ofGraphics.cpp',
  'libs/openFrameworks/graphics/ofImage.cpp',
  'libs/openFrameworks/graphics/ofPath.cpp',
  'libs/openFrameworks/graphics/ofPixels.cpp',
  'libs/openFrameworks/graphics/ofRendererCollection.cpp',
  'libs/openFrameworks/graphics/ofTessellator.cpp',
  'libs/openFrameworks/graphics/ofTrueTypeFont.cpp',
  'libs/openFrameworks/math/ofMath.cpp',
  'libs/openFrameworks/math/ofMatrix3x3.cpp',
  'libs/openFrameworks/math/ofMatrix4x4.cpp',
  'libs/openFrameworks/math/ofQuaternion.cpp',
  'libs/openFrameworks/math/ofVec2f.cpp',
  'libs/openFrameworks/math/ofVec4f.cpp',
  'libs/openFrameworks/sound/ofFmodSoundPlayer.cpp',
  'libs/openFrameworks/sound/ofMediaFoundationSoundPlayer.cpp',
  'libs/openFrameworks/sound/ofOpenALSoundPlayer.cpp',
  'libs/openFrameworks/sound/ofRtAudioSoundStream.cpp',
  'libs/openFrameworks/sound/ofSoundBaseTypes.cpp',
  'libs/openFrameworks/sound/ofSoundBuffer.cpp',
  'libs/openFrameworks/sound/ofSoundPlayer.cpp',
  'libs/openFrameworks/sound/ofSoundStream.cpp',
  'libs/openFrameworks/types/ofBaseTypes.cpp',
  'libs/openFrameworks/types/ofColor.cpp',
  'libs/openFrameworks/types/ofParameter.cpp',
  'libs/openFrameworks/types/ofParameterGroup.cpp',
  'libs/openFrameworks/types/ofRectangle.cpp',
  'libs/openFrameworks/utils/ofFileUtils.cpp',
  'libs/openFrameworks/utils/ofFpsCounter.cpp',
  'libs/openFrameworks/utils/ofLog.cpp',
  'libs/openFrameworks/utils/ofMatrixStack.cpp',
  'libs/openFrameworks/utils/ofSystemUtils.cpp',
  'libs/openFrameworks/utils/ofThread.cpp',
  'libs/openFrameworks/utils/ofTimer.cpp',
  'libs/openFrameworks/utils/ofTimerFps.cpp',
  'libs/openFrameworks/utils/ofURLFileLoader.cpp',
  'libs/openFrameworks/utils/ofUtils.cpp',
  'libs/openFrameworks/utils/ofXml.cpp',
  'libs/openFrameworks/video/ofDirectShowGrabber.cpp',
  'libs/openFrameworks/video/ofDirectShowPlayer.cpp',
  'libs/openFrameworks/video/ofMediaFoundationPlayer.cpp',
  'libs/openFrameworks/video/ofVideoGrabber.cpp',
  'libs/openFrameworks/video/ofVideoPlayer.cpp',
)

install_headers(
  'libs/openFrameworks/3d/of3dPrimitives.h',
  'libs/openFrameworks/3d/of3dUtils.h',
  'libs/openFrameworks/3d/ofCamera.h',
  'libs/openFrameworks/3d/ofEasyCam.h',
  'libs/openFrameworks/3d/ofMesh.h',
  'libs/openFrameworks/3d/ofNode.h',
  'libs/openFrameworks/app/ofAppBaseWindow.h',
  'libs/openFrameworks/app/ofAppGLFWWindow.h',
  'libs/openFrameworks/app/ofAppNoWindow.h',
  'libs/openFrameworks/app/ofAppRunner.h',
  'libs/openFrameworks/app/ofBaseApp.h',
  'libs/openFrameworks/app/ofIcon.h',
  'libs/openFrameworks/app/ofMainLoop.h',
  'libs/openFrameworks/app/ofWindowSettings.h',
  'libs/openFrameworks/communication/ofArduino.h',
  'libs/openFrameworks/communication/ofSerial.h',
  'libs/openFrameworks/events/ofEvent.h',
  'libs/openFrameworks/events/ofEvents.h',
  'libs/openFrameworks/events/ofEventUtils.h',
  'libs/openFrameworks/gl/ofBufferObject.h',
  'libs/openFrameworks/gl/ofCubeMap.h',
  'libs/openFrameworks/gl/ofCubeMapShaders.h',
  'libs/openFrameworks/gl/ofFbo.h',
  'libs/openFrameworks/gl/ofGLBaseTypes.h',
  'libs/openFrameworks/gl/ofGLProgrammableRenderer.h',
  'libs/openFrameworks/gl/ofGLRenderer.h',
  'libs/openFrameworks/gl/ofGLUtils.h',
  'libs/openFrameworks/gl/ofLight.h',
  'libs/openFrameworks/gl/ofMaterialBaseTypes.h',
  'libs/openFrameworks/gl/ofMaterial.h',
  'libs/openFrameworks/gl/ofShader.h',
  'libs/openFrameworks/gl/ofShadow.h',
  'libs/openFrameworks/gl/ofTexture.h',
  'libs/openFrameworks/gl/ofVbo.h',
  'libs/openFrameworks/gl/ofVboMesh.h',
  'libs/openFrameworks/graphics/of3dGraphics.h',
  'libs/openFrameworks/graphics/ofBitmapFont.h',
  'libs/openFrameworks/graphics/ofCairoRenderer.h',
  'libs/openFrameworks/graphics/ofGraphicsBaseTypes.h',
  'libs/openFrameworks/graphics/ofGraphicsCairo.h',
  'libs/openFrameworks/graphics/ofGraphicsConstants.h',
  'libs/openFrameworks/graphics/ofGraphics.h',
  'libs/openFrameworks/graphics/ofImage.h',
  'libs/openFrameworks/graphics/ofPath.h',
  'libs/openFrameworks/graphics/ofPixels.h',
  'libs/openFrameworks/graphics/ofPolyline.h',
  'libs/openFrameworks/graphics/ofRendererCollection.h',
  'libs/openFrameworks/graphics/ofTessellator.h',
  'libs/openFrameworks/graphics/ofTrueTypeFont.h',
  'libs/openFrameworks/math/ofMathConstants.h',
  'libs/openFrameworks/math/ofMath.h',
  'libs/openFrameworks/math/ofMatrix3x3.h',
  'libs/openFrameworks/math/ofMatrix4x4.h',
  'libs/openFrameworks/math/ofQuaternion.h',
  'libs/openFrameworks/math/ofVec2f.h',
  'libs/openFrameworks/math/ofVec3f.h',
  'libs/openFrameworks/math/ofVec4f.h',
  'libs/openFrameworks/math/ofVectorMath.h',
  'libs/openFrameworks/ofMain.h',
  'libs/openFrameworks/sound/ofAVEngineSoundPlayer.h',
  'libs/openFrameworks/sound/ofFmodSoundPlayer.h',
  'libs/openFrameworks/sound/ofMediaFoundationSoundPlayer.h',
  'libs/openFrameworks/sound/ofOpenALSoundPlayer.h',
  'libs/openFrameworks/sound/ofRtAudioSoundStream.h',
  'libs/openFrameworks/sound/ofSoundBaseTypes.h',
  'libs/openFrameworks/sound/ofSoundBuffer.h',
  'libs/openFrameworks/sound/ofSoundPlayer.h',
  'libs/openFrameworks/sound/ofSoundStream.h',
  'libs/openFrameworks/sound/ofSoundUtils.h',
  'libs/openFrameworks/types/ofBaseTypes.h',
  'libs/openFrameworks/types/ofColor.h',
  'libs/openFrameworks/types/ofParameter.h',
  'libs/openFrameworks/types/ofPoint.h',
  'libs/openFrameworks/types/ofRectangle.h',
  'libs/openFrameworks/types/ofTypes.h',
  'libs/openFrameworks/utils/ofConstants.h',
  'libs/openFrameworks/utils/ofFileUtils.h',
  'libs/openFrameworks/utils/ofFpsCounter.h',
  'libs/openFrameworks/utils/ofJson.h',
  'libs/openFrameworks/utils/ofLog.h',
  'libs/openFrameworks/utils/ofMatrixStack.h',
  'libs/openFrameworks/utils/ofNoise.h',
  'libs/openFrameworks/utils/ofRandomDistributions.h',
  'libs/openFrameworks/utils/ofRandomEngine.h',
  'libs/openFrameworks/utils/ofSingleton.h',
  'libs/openFrameworks/utils/ofSystemUtils.h',
  'libs/openFrameworks/utils/ofThreadChannel.h',
  'libs/openFrameworks/utils/ofThread.h',
  'libs/openFrameworks/utils/ofTimerFps.h',
  'libs/openFrameworks/utils/ofTimer.h',
  'libs/openFrameworks/utils/ofURLFileLoader.h',
  'libs/openFrameworks/utils/ofUtils.h',
  'libs/openFrameworks/utils/ofXml.h',
  'libs/openFrameworks/video/ofDirectShowGrabber.h',
  'libs/openFrameworks/video/ofDirectShowPlayer.h',
  'libs/openFrameworks/video/ofMediaFoundationPlayer.h',
  'libs/openFrameworks/video/ofVideoBaseTypes.h',
  'libs/openFrameworks/video/ofVideoGrabber.h',
  'libs/openFrameworks/video/ofVideoPlayer.h',
  subdir: 'openframeworks'
)

cmake   = import('cmake')

deps = [
  dependency('boost'),
  dependency('cairo'),
  dependency('fontconfig'),
  dependency('freeglut', 'glut'),
  dependency('freetype2'),
  dependency('glew'),
  dependency('glfw3'),
  dependency('glm'),
  dependency('libcrypto'),
  dependency('libcurl'),
  dependency('libmpg123'),
  dependency('libtess2'),
  dependency('nlohmann_json'),
  dependency('openal'),
  dependency('pugixml'),
  dependency('rtaudio'),
  dependency('sndfile'),
  dependency('utf8cpp'),
]

if host_machine.system() == 'linux'

  deps += [
    dependency('egl'),
    dependency('gstreamer-1.0'),
    dependency('gstreamer-app-1.0'),
    dependency('gstreamer-plugins-bad-1.0'),
    dependency('gstreamer-plugins-base-1.0'),
    dependency('gstreamer-video-1.0'),
    dependency('libudev'),
    dependency('x11-xcb'),
    dependency('xcursor'),
  ]

  sources += files(
    'libs/openFrameworks/app/ofAppEGLWindow.cpp',
    'libs/openFrameworks/video/ofGstUtils.cpp',
    'libs/openFrameworks/video/ofGstVideoGrabber.cpp',
    'libs/openFrameworks/video/ofGstVideoPlayer.cpp',
  )

  install_headers(
    'libs/openFrameworks/app/ofAppEGLWindow.h',
    'libs/openFrameworks/video/ofGstUtils.h',
    'libs/openFrameworks/video/ofGstVideoGrabber.h',
    'libs/openFrameworks/video/ofGstVideoPlayer.h',
    subdir: 'openframeworks'
  )

elif host_machine.system() == 'darwin'

  deps += [
    dependency('Cocoa'),
    dependency('AppKit'),
    dependency('Foundation'),
    dependency('AVKit'),
    dependency('GLKit'),
    dependency('AVFoundation'),
    dependency('opengl'),
    dependency('coremedia'),
    dependency('corevideo'),
    dependency('accelerate')
  ]
  add_languages('objcpp', native: false)
  add_languages('objc', native: false)
  add_project_arguments(['-ObjC++', '-fobjc-arc'], language: ['cpp', 'objcpp', 'objc'])

  sources += files(
    'libs/openFrameworks/sound/ofAVEngineSoundPlayer.mm',
    'libs/openFrameworks/video/ofAVFoundationGrabber.mm',
    'libs/openFrameworks/video/ofAVFoundationPlayer.mm',
    'libs/openFrameworks/video/ofAVFoundationVideoPlayer.m',
  )

  install_headers(
    'libs/openFrameworks/sound/ofAVEngineSoundPlayer.h',
    'libs/openFrameworks/video/ofAVFoundationGrabber.h',
    'libs/openFrameworks/video/ofAVFoundationPlayer.h',
    'libs/openFrameworks/video/ofAVFoundationGrabber.h',
    'libs/openFrameworks/video/ofAVFoundationPlayer.h',
    'libs/openFrameworks/video/ofAVFoundationVideoPlayer.h',
    subdir: 'openframeworks'
  )

endif

# We need to treat this dependency independently
# because if it's not installed in the host system,
# we will use the .wrap file in the subprojects file AND
# need to change a define in the cmake subproject
uriparser_dep = dependency('uriparser', required: false)
if not uriparser_dep.found()
  # disable the "URIPARSER_BUILD_DOCS" define that is default enabled in the subproject
  # which pulls Doxygen dependency uselessly
  uripaser_opts = cmake.subproject_options()
  uripaser_opts.add_cmake_defines({'URIPARSER_BUILD_DOCS': false, 'URIPARSER_BUILD_TESTS': false})
  uripaser_proj = cmake.subproject('uriparser', options: uripaser_opts)
  uriparser_dep = uripaser_proj.dependency('uriparser')
endif

deps += [uriparser_dep]

# Same: need to disable tests to not pull FFTW
kissfft_dep = dependency('kissfft-float', required: false)
if not kissfft_dep.found()
  kissfft_opts = cmake.subproject_options()
  kissfft_opts.add_cmake_defines({'KISSFFT_TEST': false})
  kissfft_proj = cmake.subproject('kissfft', options: kissfft_opts)
  kissfft_dep = kissfft_proj.dependency('kissfft')
endif

deps += [kissfft_dep]

# FreeImage is a special snowflake: no pkg-config / cmake / etc.. file is installed by it
# so we need to use the compiler to find it
compiler = meson.get_compiler('cpp')
freeimage_dep = compiler.find_library('freeimage', has_headers: ['FreeImage.h'], required: false)
if not freeimage_dep.found()
  freeimage_opts = cmake.subproject_options()
  # doesn't compile with the define bellow
  # freeimage_opts.add_cmake_defines({'BUILD_SHARED_LIBS': true})
  # We need -fPIC to link against the produced static library
  freeimage_opts.add_cmake_defines({'CMAKE_POSITION_INDEPENDENT_CODE': true})
  freeimage_proj = cmake.subproject('freeimage', options: freeimage_opts)
  freeimage_dep = freeimage_proj.dependency('FreeImage')
endif

deps += [freeimage_dep]

lib = library('openFrameworks',
  sources,
  include_directories: inc,
  dependencies: deps,
  install: true,
)

openframeworks_dep = declare_dependency(link_with: lib, include_directories: inc, dependencies: deps)

meson.override_dependency('openFrameworks', openframeworks_dep)

pkg = import('pkgconfig')
pkg.generate(lib)
