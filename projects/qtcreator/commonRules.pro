# Detect platform and flags for the OF library
unix:!mac{
    *-g++-64 {
        PLATFORM=linux64
    }else{
        PLATFORM=linux
    }
    CONFIG( debug, debug|release ){
        LIBS += -L$$OF_ROOT/libs/openFrameworksCompiled/lib/$$PLATFORM/ -lopenFrameworksDebug
        PRE_TARGETDEPS += $$OF_ROOT/libs/openFrameworksCompiled/lib/$$PLATFORM/libopenFrameworksDebug.a
    }else{
        LIBS += -L$$OF_ROOT/libs/openFrameworksCompiled/lib/$$PLATFORM/ -lopenFrameworks
        PRE_TARGETDEPS += $$OF_ROOT/libs/openFrameworksCompiled/lib/$$PLATFORM/libopenFrameworks.a
    }
}
macx{
    PLATFORM=osx
    CONFIG( debug, debug|release ){
        LIBS += $$OF_ROOT/libs/openFrameworksCompiled/lib/$$PLATFORM/openFrameworksDebug.a
        PRE_TARGETDEPS += $$OF_ROOT/libs/openFrameworksCompiled/lib/$$PLATFORM/openFrameworksDebug.a
    }else{
        LIBS += $$OF_ROOT/libs/openFrameworksCompiled/lib/$$PLATFORM/openFrameworks.a
        PRE_TARGETDEPS += $$OF_ROOT/libs/openFrameworksCompiled/lib/$$PLATFORM/openFrameworks.a
    }
}

# Add includes for OF core
INCLUDEPATH += $$OF_ROOT/libs/openFrameworks
INCLUDEPATH += $$OF_ROOT/libs/openFrameworks/gl
INCLUDEPATH += $$OF_ROOT/libs/openFrameworks/app
INCLUDEPATH += $$OF_ROOT/libs/openFrameworks/communication
INCLUDEPATH += $$OF_ROOT/libs/openFrameworks/graphics
INCLUDEPATH += $$OF_ROOT/libs/openFrameworks/3d
INCLUDEPATH += $$OF_ROOT/libs/openFrameworks/events
INCLUDEPATH += $$OF_ROOT/libs/openFrameworks/math
INCLUDEPATH += $$OF_ROOT/libs/openFrameworks/sound
INCLUDEPATH += $$OF_ROOT/libs/openFrameworks/types
INCLUDEPATH += $$OF_ROOT/libs/openFrameworks/utils
INCLUDEPATH += $$OF_ROOT/libs/openFrameworks/video

# Auto detect all libraries in core
CORE_LIBS = $$files("$$OF_ROOT/libs/*")

# Linux exceptions
unix:!mac{
    CORE_LIBS -= $$OF_ROOT/libs/glew
    CORE_LIBS -= $$OF_ROOT/libs/cairo
    CORE_LIBS -= $$OF_ROOT/libs/glu
    CORE_LIBS -= $$OF_ROOT/libs/poco
    CORE_LIBS -= $$OF_ROOT/libs/quicktime
    CORE_LIBS -= $$OF_ROOT/libs/videoInput
    CORE_LIBS -= $$OF_ROOT/libs/freetype
    CORE_LIBS -= $$OF_ROOT/libs/FreeImage
    CORE_LIBS -= $$OF_ROOT/libs/assimp
    CORE_LIBS -= $$OF_ROOT/libs/glut
    CORE_LIBS -= $$OF_ROOT/libs/rtAudio
    CORE_LIBS -= $$OF_ROOT/libs/openssl
    CORE_LIBS -= $$OF_ROOT/libs/boost
}

# Don't search in openFrameworksCompiled
CORE_LIBS -= $$OF_ROOT/libs/openFrameworksCompiled
CORE_LIBS -= $$OF_ROOT/libs/openFrameworks

for(libdir, CORE_LIBS) {
    exists($$libdir/lib/linux64) {
        LIBFILES=$$files("$$libdir/lib/$$PLATFORM/*.a")
        for(libfile, LIBFILES) {
            exists($$libfile){
                LIBS += $$libfile
            }
        }
    }
    exists($$libdir/include) {
        INCLUDEPATH += $$libdir/include
    }
}

# Poco needs specific order
INCLUDEPATH += $$OF_ROOT/libs/poco/include
unix:!mac{
    LIBS += $$OF_ROOT/libs/poco/lib/$$PLATFORM/libPocoNetSSL.a
    LIBS += $$OF_ROOT/libs/poco/lib/$$PLATFORM/libPocoNet.a
    LIBS += $$OF_ROOT/libs/poco/lib/$$PLATFORM/libPocoCrypto.a
    LIBS += $$OF_ROOT/libs/poco/lib/$$PLATFORM/libPocoUtil.a
    LIBS += $$OF_ROOT/libs/poco/lib/$$PLATFORM/libPocoJSON.a
    LIBS += $$OF_ROOT/libs/poco/lib/$$PLATFORM/libPocoXML.a
    LIBS += $$OF_ROOT/libs/poco/lib/$$PLATFORM/libPocoFoundation.a
}

# Other release and debug cflags
QMAKE_CXXFLAGS_DEBUG += -DDEBUG

QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

QMAKE_CXXFLAGS += -DGCC_HAS_REGEX -DOF_USING_GTK -DOF_USING_MPG123

# Linux pkgconfig and libs
unix:!mac{
    QMAKE_CXXFLAGS += -Wno-unused-parameter -std=c++14
    CONFIG += thread

    CONFIG += link_pkgconfig
    PKGCONFIG += cairo
    PKGCONFIG += gstreamer-1.0
    PKGCONFIG += zlib
    PKGCONFIG += gstreamer-app-1.0
    PKGCONFIG += gstreamer-video-1.0
    PKGCONFIG += gstreamer-base-1.0
    PKGCONFIG += libudev
    PKGCONFIG += freetype2
    PKGCONFIG += fontconfig
    PKGCONFIG += sndfile
    PKGCONFIG += openal
    PKGCONFIG += openssl
    PKGCONFIG += libpulse-simple
    PKGCONFIG += alsa
    PKGCONFIG += gl
    PKGCONFIG += glu
    PKGCONFIG += glew
    PKGCONFIG += gtk+-3.0
    PKGCONFIG += libmpg123

    LIBS += -lglut
    LIBS += -lX11
    LIBS += -lXrandr
    LIBS += -lXxf86vm
    LIBS += -lXi
    LIBS += -lXcursor
    LIBS += -ldl
    LIBS += -lpthread
    LIBS += -lfreeimage
    LIBS += -lrtaudio
    LIBS += -lboost_filesystem
    LIBS += -lboost_system
}
