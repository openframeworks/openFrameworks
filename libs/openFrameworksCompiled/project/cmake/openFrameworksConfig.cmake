get_filename_component(openFrameworksRoot "${CMAKE_CURRENT_LIST_DIR}/../../../../" ABSOLUTE)

# Having to make the following distinction is a result of
# openFrameworks being not properly deployed into standard lib/include
# paths as of yet, but this is subject to change.
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        set(PLATFORM "linux64")
    else()
        set(PLATFORM "linux")
    endif()
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
    ${openFrameworksRoot}/libs/tess2/include
    ${openFrameworksRoot}/libs/utf8cpp/include
    ${openFrameworksRoot}/libs/poco/include
    ${openFrameworksRoot}/libs/cairo/include/cairo
    ${openFrameworksRoot}/libs/kiss/include
    # system-wide 3rd party include directories
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
find_library(GLFW_LIB glfw)
find_library(CAIRO_LIB cairo)
find_library(FONTCONFIG_LIB fontconfig)
find_library(FREETYPE_LIB freetype)
find_library(OPENAL_LIB openal)
find_library(MPG123_LIB mpg123)
find_library(SNDFILE_LIB sndfile)
find_library(OPENSSL_LIB ssl)
find_library(CRYPTO_LIB crypto)

set(openFrameworks_LIBRARIES
    # openFrameworks
    ${openFrameworksRoot}/libs/openFrameworksCompiled/lib/${PLATFORM}/libopenFrameworks.a

    # oF-supplied libraries
    ${openFrameworksRoot}/libs/kiss/lib/${PLATFORM}/libkiss.a
    ${openFrameworksRoot}/libs/tess2/lib/linux64/libtess2.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/libPocoData.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/libPocoDataSQLite.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/libPocoMongoDB.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/libPocoNet.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/libPocoNetSSL.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/libPocoCrypto.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/libPocoZip.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/libPocoUtil.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/libPocoXML.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/libPocoJSON.a
    ${openFrameworksRoot}/libs/poco/lib/${PLATFORM}/libPocoFoundation.a

    # system libraries
    ${SNDFILE_LIB}
    ${MPG123_LIB}
    ${OPENAL_LIB}
    ${FREETYPE_LIB}
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
    
    # on linux freeimage is searched systemwide
    find_library(FREEIMAGEPLUS_LIB freeimageplus)

    set(openFrameworks_LIBRARIES ${openFrameworks_LIBRARIES}
        ${X11_LIB}
        ${PTHREAD_LIB}
        ${FREEIMAGEPLUS_LIB}
    )
endif()

message("-- Found openFrameworks in ${openFrameworksRoot}")
