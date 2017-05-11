get_filename_component(openFrameworksRoot "${CMAKE_CURRENT_LIST_DIR}/../../../../" ABSOLUTE)

set(LIB_PREFIX)
# Having to make the following distinction is a result of
# openFrameworks being not properly deployed into standard lib/include
# paths as of yet, but this is subject to change.
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        set(PLATFORM "linux64")
    else()
        set(PLATFORM "linux")
    endif()
    set(LIB_PREFIX "lib")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(PLATFORM "osx")
elseif(${WIN32})
    set(PLATFORM "vs")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if ((GCC_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3))
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
        message(FATAL_ERROR
            "openFrameworks requires >=gcc-4.3 for C++11 support.")
    endif ()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif ()

# search for gstreamer includes
find_path(GST_INCLUDE gst/gst.h
    PATH_SUFFIXES gstreamer-1.0)
find_path(GST_CONFIG gst/gstconfig.h
    PATH_SUFFIXES ../lib/gstreamer-1.0/include)

# search for glib includes
find_path(GLIB_INCLUDE glib.h
    PATH_SUFFIXES glib-2.0)
find_path(GLIB_CONFIG glibconfig.h
    PATH_SUFFIXES ../lib/glib-2.0/include)
find_path(GLEW_INCLUDE glew.h
    PATH_SUFFIXES GL)

set(openFrameworks_INCLUDES
    # oF include directories
    ${openFrameworksRoot}/libs/openFrameworks
    ${openFrameworksRoot}/libs/openFrameworks/graphics
    ${openFrameworksRoot}/libs/openFrameworks/3d
    ${openFrameworksRoot}/libs/openFrameworks/math
    ${openFrameworksRoot}/libs/openFrameworks/gl
    ${openFrameworksRoot}/libs/openFrameworks/video
    ${openFrameworksRoot}/libs/openFrameworks/types
    ${openFrameworksRoot}/libs/openFrameworks/sound
    ${openFrameworksRoot}/libs/openFrameworks/app
    ${openFrameworksRoot}/libs/openFrameworks/events
    ${openFrameworksRoot}/libs/openFrameworks/communication
    ${openFrameworksRoot}/libs/openFrameworks/utils
    # oF-supplied 3rd party include directories
    ${openFrameworksRoot}/libs/utf8cpp/include
    ${openFrameworksRoot}/libs/poco/include
    ${openFrameworksRoot}/libs/cairo/include/cairo
    ${openFrameworksRoot}/libs/kiss/include
    # system-wide 3rd party include directories
    ${GLEW_INCLUDE}/..
    ${GST_INCLUDE}
    ${GST_CONFIG}
    ${GLIB_INCLUDE}
    ${GLIB_CONFIG}
    )

# find platform-independant libraries
find_library(BOOST_SYSTEM_LIB boost_system)
find_library(BOOST_FILESYSTEM_LIB boost_filesystem)
find_library(GL_LIB GL)
find_library(GLEW_LIB GLEW)
find_library(GLFW_LIB glfw3 NAMES glfw)
find_library(CAIRO_LIB cairo)
find_library(FONTCONFIG_LIB fontconfig)
find_library(FREETYPE_LIB freetype)
find_library(FREEIMAGE_LIB freeimage)
find_library(OPENAL_LIB openal)
find_library(MPG123_LIB mpg123)
find_library(SNDFILE_LIB sndfile)
find_library(OPENSSL_LIB ssl)
find_library(CRYPTO_LIB crypto)

set(openFrameworks_LIBRARIES
    # openFrameworks
    ${openFrameworksRoot}/libs/openFrameworksCompiled/lib/${PLATFORM}/${LIB_PREFIX}openFrameworks.a
    # oF-supplied libraries
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/${LIB_PREFIX}PocoData.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/${LIB_PREFIX}PocoDataSQLite.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/${LIB_PREFIX}PocoMongoDB.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/${LIB_PREFIX}PocoNet.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/${LIB_PREFIX}PocoNetSSL.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/${LIB_PREFIX}PocoCrypto.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/${LIB_PREFIX}PocoZip.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/${LIB_PREFIX}PocoUtil.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/${LIB_PREFIX}PocoXML.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/${LIB_PREFIX}PocoJSON.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/${LIB_PREFIX}PocoFoundation.a
    # system libraries
    ${SNDFILE_LIB}
    ${MPG123_LIB}
    ${OPENAL_LIB}
    ${FREETYPE_LIB}
    ${FREEIMAGE_LIB}
    ${FONTCONFIG_LIB}
    ${CAIRO_LIB}
    ${GLFW_LIB}
    ${GLEW_LIB}
    ${GL_LIB}
    ${CRYPTO_LIB}
    ${OPENSSL_LIB}
    ${BOOST_FILESYSTEM_LIB}
    ${BOOST_SYSTEM_LIB}
    )

# linux-specific
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_library(X11_LIB X11)

    # not sure if pthreads is Linux only, will iterate this if people
    # test on different platforms!
    find_library(PTHREAD_LIB pthread)
    
    set(openFrameworks_INCLUDES
        ${openFrameworks_INCLUDES}
        ${openFrameworksRoot}/libs/tess2/include)
    
    # libraries
    set(openFrameworks_LIBRARIES
        ${openFrameworks_LIBRARIES}
        ${openFrameworksRoot}/libs/tess2/lib/linux64/libtess2.a
        ${openFrameworksRoot}/libs/kiss/lib/${PLATFORM}/libkiss.a
        ${X11_LIB}
        ${PTHREAD_LIB}
        ${FREEIMAGEPLUS_LIB}
        )
endif()

# osx-specific
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(openFrameworks_INCLUDES ${openFrameworks_INCLUDES}
        ${openFrameworksRoot}/libs/fmodex/include)

    # libraries
    find_library(CORETEXT_LIB CoreText)
    find_library(COREFOUNDATION_LIB CoreFoundation)
    find_library(COCOA_LIB Cocoa)
    find_library(TESS_LIB tess2)
    message("***** ${COCOA_LIB}")
    set(openFrameworks_LIBRARIES
        ${openFrameworksRoot}/libs/fmodex/lib/${PLATFORM}/libfmodex.dylib
        ${openFrameworks_LIBRARIES}
        ${CORETEXT_LIB}
        ${COREFOUNDATION_LIB}
        ${COCOA_LIB}
        ${TESS_LIB}
        )

endif()

message("** Found openFrameworks in ${openFrameworksRoot}")
