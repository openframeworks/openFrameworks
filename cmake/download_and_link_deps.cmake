include(FetchContent)
include(${CMAKE_CURRENT_LIST_DIR}/utils.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/import_deps.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)

set(OF_TARGET_ARCHITECTURE "auto" CACHE STRING "The target platform for openFrameworks. 'auto' to detect automatically")
option(OF_VERBOSE "Enable verbose printing while downloading the compiled binaries for OF" OFF)

set(_AVAILABLE_ARCHITECTURES "msvc, androidarm7, androidarm64, androidx86, ios, tvos, macos, msys-mingw, msys-clang, msys-ucrt, mingw, linux64, linuxarmv6l, linuxarmv7l, emscripten")

if(OF_TARGET_ARCHITECTURE STREQUAL "auto")
    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(FATAL_ERROR "OpenFrameworks does no longer support 32-bit build systems. Please upgrade your compiler to 64-bit.")
    endif()

    message(STATUS "Auto-detecting platform...")

    if (MSVC)
        set(OF_TARGET_ARCHITECTURE "msvc" CACHE STRING "" FORCE)
    elseif (EMSCRIPTEN)
        set(OF_TARGET_ARCHITECTURE "emscripten" CACHE STRING "" FORCE)
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

function(get_packages_and_link)
    #Set(FETCHCONTENT_QUIET FALSE)
    message(STATUS "[openframeworks] Using ${OF_TARGET_ARCHITECTURE} toolchain, downloading dependencies")
    foreach(dependency IN LISTS ARGN)
        set(_URL "http://ci.openframeworks.cc/libs/openFrameworksLibs_master_${dependency}")
        #message(STATUS "[openframeworks] Fetching ${_URL}")

        # Download and extract the compressed archive
        CPMAddPackage(
            NAME of-deps-${dependency}
            URL ${_URL}
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
            DOWNLOAD_ONLY YES
        )

        # Now, create the targets and link all files to them
        set(PACKAGE_SOURCE_DIR ${of-deps-${dependency}_SOURCE_DIR})
        file(GLOB deps "${PACKAGE_SOURCE_DIR}/**")
        foreach(DEP_ROOT IN LISTS deps)
            get_filename_component(DEP_NAME ${DEP_ROOT} NAME)   #  vvv  This is called for each dependency in each downloaded package
            import_dependency(${DEP_NAME} ${DEP_ROOT} ${PACKAGE_SOURCE_DIR})
        endforeach()

    endforeach()
    #Set(FETCHCONTENT_QUIET TRUE)
endfunction()

# And now download the archives, depending on the platform we're on
if (OF_TARGET_ARCHITECTURE STREQUAL "msvc")
    get_packages_and_link("vs2017_64_1.zip" "vs2017_64_2.zip" "vs2017_64_3.zip" "vs2017_64_4.zip")

elseif (OF_TARGET_ARCHITECTURE STREQUAL "emscripten")       # TODO
    message(SEND_ERROR "Emscripten dependencies are not yet implemented in CMake! Please edit cmake/download_deps.cmake to add support yourself!")
    get_packages_and_link()

elseif (OF_TARGET_ARCHITECTURE STREQUAL "androidarm7")
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
    get_packages_and_link("msys2_mingw64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "msys-ucrt")
    get_packages_and_link("msys2_ucrt64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "msys-clang")
    get_packages_and_link("msys2_clang64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "mingw")
    get_packages_and_link("msys2_mingw64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "linux64")
    get_packages_and_link("linux64gcc6.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "linuxarmv6l")
    get_packages_and_link("linuxarmv6l.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "linuxarmv7l")
    get_packages_and_link("linuxarmv7l.tar.bz2")
    
else ()
    message(FATAL_ERROR "No target platform was chosen. Please specify it manually using '<cmake_command> -DOF_TARGET_ARCHITECTURE=<target_arch>'. Possible values are: ${_AVAILABLE_ARCHITECTURES}.")
endif()



# TODO: Now, download glut since it's somehow not part of the apothecary. Glut support seems to be deprecated
CPMAddPackage(
    NAME of-deps-glut
    GIT_REPOSITORY "https://github.com/markkilgard/glut"
    GIT_TAG "8cd96cb440f1f2fac3a154227937be39d06efa53"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_ONLY YES
)
add_library(of-deps-glut INTERFACE)
add_library(of::glut ALIAS of-deps-glut)
target_include_directories(of-deps-glut INTERFACE ${of-deps-glut_SOURCE_DIR}/include/GL)

# After compilation copy the dll files to the binary dir
file(GLOB_RECURSE __SHARED_LIBS "${CMAKE_CURRENT_BINARY_DIR}/_deps/**/${CMAKE_SHARED_LIBRARY_PREFIX}**${CMAKE_SHARED_LIBRARY_SUFFIX}")
foreach(LIB IN LISTS __SHARED_LIBS)
    add_custom_command(TARGET openframeworks POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${LIB}" "${CMAKE_BINARY_DIR}/bin/$<IF:$<CONFIG:Debug>,Debug,Release>")
endforeach()



# And link to all the dependencies we depend upon
target_link_libraries(openframeworks 
    of::tess2 
    of::cairo 
    of::glm 
    of::utf8
    of::glfw
    of::glut
    of::fmod
    of::FreeImage
    of::rtAudio
    of::videoInput
    of::uriparser
    of::openssl
    of::curl
    of::freetype
    of::pugixml
    of::json
)

# Conditional dependencies
if(WIN32)       # Linking to WinAPI system libraries
    target_link_libraries(openframeworks winmm ws2_32 wldap32 crypt32 dsound.lib)
endif()

if (OF_TARGET_ARCHITECTURE STREQUAL "msvc" OR OF_TARGET_ARCHITECTURE STREQUAL "macos")
    target_link_libraries(openframeworks of::glew)
endif()

find_package(OpenGL)
if (NOT OpenGL_FOUND)       # This should never be not found on windows
    message(FATAL_ERROR "Dependency OpenGL not found. On Linux, please install it using your system's equivalent of 'sudo apt install libgl1-mesa-dev'")
endif()
target_include_directories(openframeworks PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(openframeworks ${OPENGL_LIBRARIES})

if (LINUX)
    find_package(assimp)
    if (NOT assimp_FOUND)
        message(FATAL_ERROR "Dependency Assimp not found. Please install it using your system's equivalent of 'sudo apt install libassimp-dev'")
    endif()
    target_include_directories(openframeworks PUBLIC ${assimp_INCLUDE_DIRS})
    target_link_libraries(openframeworks ${assimp_LIBRARIES})

    find_package(OpenCV)
    if (NOT OpenCV_FOUND)
        message(FATAL_ERROR "Dependency OpenCV not found. Please install it using your system's equivalent of 'sudo apt install libopencv-dev'")
    endif()
    target_include_directories(openframeworks PUBLIC ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(openframeworks ${OpenCV_LIBS})
endif()
