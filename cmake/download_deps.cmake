include(FetchContent)
include(${CMAKE_CURRENT_LIST_DIR}/utils.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)

set(OF_TARGET_ARCHITECTURE "auto" CACHE STRING "The target platform for openFrameworks. 'auto' to detect automatically")
option(OF_VERBOSE "Enable verbose printing while downloading the compiled binaries for OF" OFF)

set(_AVAILABLE_ARCHITECTURES "msvc, androidarm7, androidarm64, androidx86, ios, tvos, macos, msys-mingw, msys-clang, msys-ucrt, mingw, linux64, linuxarmv6l, linuxarmv7l")

if(OF_TARGET_ARCHITECTURE STREQUAL "auto")
    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(FATAL_ERROR "OpenFrameworks does no longer support 32-bit build systems. Please upgrade your compiler to 64-bit.")
    endif()

    message(STATUS "Auto-detecting platform...")

    if (MSVC)
        set(OF_TARGET_ARCHITECTURE "msvc" CACHE STRING "" FORCE)
    elseif (ANDROID)
        set(OF_TARGET_ARCHITECTURE "androidarm7" CACHE STRING "" FORCE)
        # androidx86 and androidarm64 must be specified manually
    elseif (APPLE)
        if (IOS)
            set(OF_TARGET_ARCHITECTURE "ios" CACHE STRING "" FORCE)
        elseif (CMAKE_SYSTEM_NAME STREQUAL "tvOS")
            set(OF_TARGET_ARCHITECTURE "tvos" CACHE STRING "" FORCE)
        else()
            set(OF_TARGET_ARCHITECTURE "macos" CACHE STRING "" FORCE)
        endif()
    elseif (MSYS)
        set(OF_TARGET_ARCHITECTURE "msys-mingw" CACHE STRING "" FORCE)
        # msys-clang and msys-ucrt must be specified manually
    elseif (MINGW)
        set(OF_TARGET_ARCHITECTURE "mingw" CACHE STRING "" FORCE)
    elseif (LINUX)
        set(OF_TARGET_ARCHITECTURE "linux64" CACHE STRING "" FORCE)
        # 'linuxarmv6l' and 'linuxarmv7l' must be specified manually
    else ()
        message(FATAL_ERROR "The target platform could not be detected automatically. Please specify it manually using '<cmake_command> -DOF_TARGET_ARCHITECTURE=<target_arch>'. Possible values are: ${_AVAILABLE_ARCHITECTURES}.")
    endif()

endif()

# Macro to download and extract the dependency archive
function(make_dependencies_available)
    #Set(FETCHCONTENT_QUIET FALSE)
    message(STATUS "[openframeworks] Using ${OF_TARGET_ARCHITECTURE} toolchain, downloading dependencies")
    foreach(dependency IN LISTS ARGN)
        set(_URL "http://ci.openframeworks.cc/libs/openFrameworksLibs_master_${dependency}")
        #message(STATUS "[openframeworks] Fetching ${_URL}")
        CPMAddPackage(
            NAME of-deps-${dependency}
            URL ${_URL}
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
            DOWNLOAD_ONLY YES
        )
    endforeach()
    #Set(FETCHCONTENT_QUIET TRUE)
endfunction()

# And now download the archives, depending on the platform we're on
if (OF_TARGET_ARCHITECTURE STREQUAL "msvc")
    make_dependencies_available("vs2017_64_1.zip" "vs2017_64_2.zip" "vs2017_64_3.zip" "vs2017_64_4.zip")

elseif (OF_TARGET_ARCHITECTURE STREQUAL "androidarm7")
    make_dependencies_available("androidarmv7.tar.bz2")

elseif (OF_TARGET_ARCHITECTURE STREQUAL "androidarm64")
    make_dependencies_available("androidarm64.tar.bz2")

elseif (OF_TARGET_ARCHITECTURE STREQUAL "androidx86")
    make_dependencies_available("androidx86.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "ios")
    make_dependencies_available("ios1.tar.bz2" "ios2.tar.bz2" "ios3.tar.bz2" "ios4.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "tvos")
    make_dependencies_available("tvos1.tar.bz2" "tvos2.tar.bz2" "tvos3.tar.bz2" "tvos4.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "macos")
    make_dependencies_available("osx1.tar.bz2" "osx2.tar.bz2" "osx3.tar.bz2" "osx4.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "msys-mingw")
    make_dependencies_available("msys2_mingw64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "msys-ucrt")
    make_dependencies_available("msys2_ucrt64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "msys-clang")
    make_dependencies_available("msys2_clang64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "mingw")
    make_dependencies_available("msys2_mingw64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "linux64")
    make_dependencies_available("linux64gcc6.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "linuxarmv6l")
    make_dependencies_available("linuxarmv6l.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "linuxarmv7l")
    make_dependencies_available("linuxarmv7l.tar.bz2")
    
else ()
    message(FATAL_ERROR "No target platform was chosen. Please specify it manually using '<cmake_command> -DOF_TARGET_ARCHITECTURE=<target_arch>'. Possible values are: ${_AVAILABLE_ARCHITECTURES}.")
endif()