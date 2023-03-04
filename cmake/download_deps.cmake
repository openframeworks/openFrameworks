include(FetchContent)
include(${CMAKE_CURRENT_LIST_DIR}/utils.cmake)
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


# Macro to download and extract the dependency archive
function(get_dependency_and_link)
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
        file(GLOB deps "${of-deps-${dependency}_SOURCE_DIR}/**")
        foreach(dep_root IN LISTS deps)                             # <- looping over all dependencies in each binary package
            get_filename_component(dep_name ${dep_root} NAME)
            set(target_name of-deps-${dep_name})

            # Declaring the target for each dependency so we can later link to it one by one
            add_library(${target_name} STATIC IMPORTED)
            add_library(openframeworks::${dep_name} ALIAS ${target_name})

            # Header file directories
            if (${dep_name} STREQUAL "cairo")                       # Unfortunately cairo and freetype are inconsistent in the apothecary
                target_include_directories(${target_name} INTERFACE ${dep_root}/include/cairo)
            elseif(${dep_name} STREQUAL "freetype")
                target_include_directories(${target_name} INTERFACE ${dep_root}/include/freetype2)
            else()
                target_include_directories(${target_name} INTERFACE ${dep_root}/include)
            endif()

            # Now we link to every static library file found in the given dependency's folder
            file(GLOB_RECURSE libraries "${of-deps-${dependency}_SOURCE_DIR}/**/${CMAKE_STATIC_LIBRARY_PREFIX}**${CMAKE_STATIC_LIBRARY_SUFFIX}")
            foreach(lib IN LISTS libraries)          
                set_target_properties(${target_name} PROPERTIES IMPORTED_LOCATION ${lib})          
            endforeach()
        endforeach()

    endforeach()
    #Set(FETCHCONTENT_QUIET TRUE)
endfunction()


# And now download the archives, depending on the platform we're on
if (OF_TARGET_ARCHITECTURE STREQUAL "msvc")
    get_dependency_and_link("vs2017_64_1.zip" "vs2017_64_2.zip" "vs2017_64_3.zip" "vs2017_64_4.zip")

elseif (OF_TARGET_ARCHITECTURE STREQUAL "emscripten")       # TODO
    message(SEND_ERROR "Emscripten dependencies are not yet implemented in CMake! Please edit cmake/download_deps.cmake to add support yourself!")
    get_dependency_and_link()

elseif (OF_TARGET_ARCHITECTURE STREQUAL "androidarm7")
    get_dependency_and_link("androidarmv7.tar.bz2")

elseif (OF_TARGET_ARCHITECTURE STREQUAL "androidarm64")
    get_dependency_and_link("androidarm64.tar.bz2")

elseif (OF_TARGET_ARCHITECTURE STREQUAL "androidx86")
    get_dependency_and_link("androidx86.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "ios")
    get_dependency_and_link("ios1.tar.bz2" "ios2.tar.bz2" "ios3.tar.bz2" "ios4.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "tvos")
    get_dependency_and_link("tvos1.tar.bz2" "tvos2.tar.bz2" "tvos3.tar.bz2" "tvos4.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "macos")
    get_dependency_and_link("osx1.tar.bz2" "osx2.tar.bz2" "osx3.tar.bz2" "osx4.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "msys-mingw")
    get_dependency_and_link("msys2_mingw64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "msys-ucrt")
    get_dependency_and_link("msys2_ucrt64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "msys-clang")
    get_dependency_and_link("msys2_clang64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "mingw")
    get_dependency_and_link("msys2_mingw64.zip")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "linux64")
    get_dependency_and_link("linux64gcc6.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "linuxarmv6l")
    get_dependency_and_link("linuxarmv6l.tar.bz2")
    
elseif (OF_TARGET_ARCHITECTURE STREQUAL "linuxarmv7l")
    get_dependency_and_link("linuxarmv7l.tar.bz2")
    
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
add_library(openframeworks::glut ALIAS of-deps-glut)
target_include_directories(of-deps-glut INTERFACE ${of-deps-glut_SOURCE_DIR}/include/GL)
