include(FetchContent)
include(${CMAKE_CURRENT_LIST_DIR}/utils.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/import_deps.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/modules)

set(OF_TARGET_ARCHITECTURE "auto" CACHE STRING "The target platform for openFrameworks. 'auto' to detect automatically")
option(OF_VERBOSE "Enable verbose printing while downloading the compiled binaries for OF" OFF)

set(_AVAILABLE_ARCHITECTURES "msvc, androidarmeabi, androidarm64, androidx86, ios, tvos, macos, msys-mingw, msys-clang, msys-ucrt, mingw, linux64, linuxarmv6l, linuxarmv7l, emscripten")

if(OF_TARGET_ARCHITECTURE STREQUAL "auto")
    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(FATAL_ERROR "OpenFrameworks no longer supports 32-bit build systems. Please upgrade your compiler to 64-bit.")
    endif()

    message(STATUS "[openframeworks] Auto-detecting platform...")

    if (MSVC)
        set(OF_TARGET_ARCHITECTURE "msvc" CACHE STRING "" FORCE)
    elseif (EMSCRIPTEN)
        set(OF_TARGET_ARCHITECTURE "emscripten" CACHE STRING "" FORCE)
    elseif (ANDROID)
        if (CMAKE_ANDROID_ARCH_ABI MATCHES "arm64")
            set(OF_TARGET_ARCHITECTURE "androidarm64" CACHE STRING "" FORCE)
        else ()
            if (CMAKE_ANDROID_ARCH_ABI MATCHES "armeabi")
                set(OF_TARGET_ARCHITECTURE "androidarmeabi" CACHE STRING "" FORCE)
            else ()
                if (CMAKE_ANDROID_ARCH_ABI MATCHES "x86")
                    set(OF_TARGET_ARCHITECTURE "androidx86" CACHE STRING "" FORCE)
                else ()
                    message(FATAL_ERROR "The target platform could not be detected automatically. Please specify it manually using '-DOF_TARGET_ARCHITECTURE=<target_arch> to your cmake call'. Possible values are: ${_AVAILABLE_ARCHITECTURES}.")
                endif ()
            endif ()
        endif ()
    elseif (APPLE)
        if (CMAKE_SYSTEM_NAME STREQUAL "iOS")
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
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(OF_TARGET_ARCHITECTURE "linux64" CACHE STRING "" FORCE)
        # 'linuxarmv6l' and 'linuxarmv7l' must be specified manually
    else ()
        message(FATAL_ERROR "The target platform could not be detected automatically. Please specify it manually using '-DOF_TARGET_ARCHITECTURE=<target_arch> to your cmake call'. Possible values are: ${_AVAILABLE_ARCHITECTURES}.")
    endif()

endif()

function(get_packages_and_link)
    if (OF_VERBOSE)
        Set(FETCHCONTENT_QUIET FALSE)
    endif()
    message(STATUS "[openframeworks] Using ${OF_TARGET_ARCHITECTURE} toolchain, downloading dependencies (-DOF_VERBOSE=ON for progress)")
    foreach(dependency IN LISTS ARGN)

        # Download and extract the compressed archive
        if (NOT of-deps-${dependency}_CONFIGURED)
            set(_URL "http://ci.openframeworks.cc/libs/openFrameworksLibs_master_${dependency}")
            message(STATUS "[openframeworks] Fetching ${_URL}")
            CPMAddPackage(
                NAME of-deps-${dependency}
                DOWNLOAD_EXTRACT_TIMESTAMP TRUE
                DOWNLOAD_ONLY YES
                URL ${_URL}
            )
            set(of-deps-${dependency}_SOURCE_DIR ${of-deps-${dependency}_SOURCE_DIR} CACHE BOOL "" FORCE)
            set(of-deps-${dependency}_CONFIGURED ON CACHE BOOL "" FORCE)
        else()
            message(STATUS "[openframeworks] Skipping check of ${dependency} to save build time. To force a re-download, delete CMakeCache.txt and re-run cmake.")
        endif()

        # Now, create the targets and link all files to them
        set(PACKAGE_SOURCE_DIR ${of-deps-${dependency}_SOURCE_DIR})
        file(GLOB deps "${PACKAGE_SOURCE_DIR}/**")
        foreach(DEP_ROOT IN LISTS deps)
            get_filename_component(DEP_NAME ${DEP_ROOT} NAME)   #  vvv  This is called for each dependency in each downloaded package

            if (OF_TARGET_ARCHITECTURE MATCHES "linux" AND DEP_NAME STREQUAL "poco")   # We do not want to use the embedded poco on linux as it's deprecated. We use the system's package instead
                continue()
            endif ()

            import_dependency(${DEP_NAME} ${DEP_ROOT} ${PACKAGE_SOURCE_DIR})
        endforeach()

    endforeach()
    if (OF_VERBOSE)
        Set(FETCHCONTENT_QUIET TRUE)
    endif()
endfunction()

# And now download the archives, depending on the platform we're on
if (OF_TARGET_ARCHITECTURE STREQUAL "msvc")
    get_packages_and_link("vs2017_64_1.zip" "vs2017_64_2.zip" "vs2017_64_3.zip" "vs2017_64_4.zip")

elseif (OF_TARGET_ARCHITECTURE STREQUAL "emscripten")       # TODO: Implement Emscripten dependencies
    message(SEND_ERROR "Emscripten is not yet implemented in CMake! You are welcome to contribute!")
    get_packages_and_link()

elseif (OF_TARGET_ARCHITECTURE STREQUAL "androidarmeabi")
    get_packages_and_link("androidarmv7.tar.bz2")

elseif (OF_TARGET_ARCHITECTURE STREQUAL "androidarm64")
    get_packages_and_link("androidarm64.tar.bz2")

elseif (OF_TARGET_ARCHITECTURE STREQUAL "androidx86")
    get_packages_and_link("androidx86.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "ios")
    get_packages_and_link("ios1.tar.bz2" "ios2.tar.bz2" "ios3.tar.bz2" "ios4.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "tvos")
    get_packages_and_link("tvos1.tar.bz2" "tvos2.tar.bz2" "tvos3.tar.bz2" "tvos4.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "macos")
    get_packages_and_link("osx1.tar.bz2" "osx2.tar.bz2" "osx3.tar.bz2" "osx4.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "msys-mingw")
    message(SEND_ERROR "MSYS2/MinGW is not yet implemented in CMake! You are welcome to contribute!")
#    get_packages_and_link("msys2_mingw64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "msys-ucrt")
    message(SEND_ERROR "MSYS2/ucrt64 is not yet implemented in CMake! You are welcome to contribute!")
#    get_packages_and_link("msys2_ucrt64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "msys-clang")
    message(SEND_ERROR "MSYS2/clang64 is not yet implemented in CMake! You are welcome to contribute!")
#    get_packages_and_link("msys2_clang64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "mingw")
    message(SEND_ERROR "Raw MinGW is not yet implemented in CMake! You are welcome to contribute!")
#    get_packages_and_link("msys2_mingw64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "linux64")
    get_packages_and_link("linux64gcc6.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "linuxarmv6l")
    get_packages_and_link("linuxarmv6l.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "linuxarmv7l")
    get_packages_and_link("linuxarmv7l.tar.bz2")
    
else ()
    message(FATAL_ERROR "No target platform was chosen. Please specify it manually using '<cmake_command> -DOF_TARGET_ARCHITECTURE=<target_arch>'. Possible values are: ${_AVAILABLE_ARCHITECTURES}.")
endif()


# Find system packages
if (NOT OF_TARGET_ARCHITECTURE MATCHES "android")
    find_package(OpenGL)
    if (NOT OpenGL_FOUND)       # This should never be not found on windows
        message(SEND_ERROR "Dependency OpenGL not found. On Linux, please install it using your system's equivalent of 'sudo apt install libgl1-mesa-dev'")
    endif()
    target_include_directories(openframeworks PUBLIC ${OPENGL_INCLUDE_DIRS})
    target_link_libraries(openframeworks ${OPENGL_LIBRARIES})
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")  # All Linux-only packages (that are part of apothecary on all platforms except Linux)
    include(${CMAKE_CURRENT_LIST_DIR}/find_linux_deps.cmake)
    find_linux_deps()
endif()



# TODO: Now, download glut since it's somehow not part of the apothecary. Glut support seems to be deprecated
if (WIN32)
    if (NOT of-deps-glut_CONFIGURED)
        CPMAddPackage(
            NAME of-deps-glut
            GIT_REPOSITORY "https://github.com/markkilgard/glut"
            GIT_TAG "8cd96cb440f1f2fac3a154227937be39d06efa53"
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
            DOWNLOAD_ONLY YES
        )
        set(of-deps-glut_SOURCE_DIR ${of-deps-glut_SOURCE_DIR} CACHE PATH "" FORCE)
        set(of-deps-glut_CONFIGURED ON CACHE BOOL "" FORCE)
    else()
        message(STATUS "[openframeworks] Skipping check of of-deps-glut to save build time. To force a re-download, delete CMakeCache.txt and and re-run cmake.")
    endif()

    add_library(of-deps-glut INTERFACE)
    add_library(of::glut ALIAS of-deps-glut)
    target_include_directories(of-deps-glut INTERFACE ${of-deps-glut_SOURCE_DIR}/include)
    target_include_directories(of-deps-glut INTERFACE ${of-deps-glut_SOURCE_DIR}/include/GL)
endif()

# And link to all the dependencies we depend upon
target_link_libraries(openframeworks 
    of::tess2 
    of::glm 
    of::utf8
    of::FreeImage
    of::uriparser
    of::openssl
    of::curl
    of::freetype
    of::pugixml
    of::json
)

# Conditional dependencies
if(WIN32)       # Linking to WinAPI system libraries
    target_link_libraries(openframeworks of::videoInput of::glut)    # videoInput library is Windows-only
    target_link_libraries(openframeworks winmm ws2_32 wldap32 crypt32 dsound setupapi)
endif()

if (OF_TARGET_ARCHITECTURE MATCHES "linux")
    target_link_libraries(openframeworks of::OpenAL of::KissFFT of::sndfile of::gstreamer of::glib of::fontconfig of::udev)
    target_link_libraries(openframeworks -lX11)
endif()

if (OF_TARGET_ARCHITECTURE STREQUAL "msvc" OR       # GLEW is only a requirement on these systems
    OF_TARGET_ARCHITECTURE STREQUAL "macos" OR 
    OF_TARGET_ARCHITECTURE MATCHES "linux")
        target_link_libraries(openframeworks of::rtAudio of::fmod of::glfw of::cairo of::glew)
endif()

# And finally find all shared libraries that are part of the dependencies, to be used later
file(GLOB_RECURSE _OF_DEPS_SHARED_LIBS "${CMAKE_BINARY_DIR}/_deps/**/${CMAKE_SHARED_LIBRARY_PREFIX}**${CMAKE_SHARED_LIBRARY_SUFFIX}")
set(OF_DEPS_SHARED_LIBS ${_OF_DEPS_SHARED_LIBS} CACHE INTERNAL "")
