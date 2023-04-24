
function(add_libraries_to_dependency LIB_DIR_NAME LIB_NAME DEP_NAME LIB_PATH)

    set(CONFIG "ALL")
    if (LIB_DIR_NAME STREQUAL "Debug")
        set(CONFIG "Debug")
    elseif(LIB_DIR_NAME STREQUAL "Release")
        set(CONFIG "Release")
    endif()

    if (DEP_NAME STREQUAL "pugixml")
        if (LIB_NAME STREQUAL "${CMAKE_STATIC_LIBRARY_PREFIX}pugixmld${CMAKE_STATIC_LIBRARY_SUFFIX}")
            set(CONFIG "Debug")
        else()
            set(CONFIG "Release")
        endif()
    endif()

    if (DEP_NAME STREQUAL "videoInput")
        if (LIB_NAME STREQUAL "${CMAKE_STATIC_LIBRARY_PREFIX}videoInputD${CMAKE_STATIC_LIBRARY_SUFFIX}")
            set(CONFIG "Debug")
        else()
            set(CONFIG "Release")
        endif()
    endif()

    if (DEP_NAME STREQUAL "rtAudio")
        if (LIB_NAME STREQUAL "${CMAKE_STATIC_LIBRARY_PREFIX}rtAudioD${CMAKE_STATIC_LIBRARY_SUFFIX}")
            set(CONFIG "Debug")
        else()
            set(CONFIG "Release")
        endif()
    endif()

    if (CONFIG STREQUAL "Debug")
        target_link_libraries(${TARGET_NAME} INTERFACE $<IF:$<CONFIG:Debug>,${LIB_PATH},>)
    elseif(CONFIG STREQUAL "Release")
        target_link_libraries(${TARGET_NAME} INTERFACE $<IF:$<CONFIG:Debug>,,${LIB_PATH}>)
    else()
        target_link_libraries(${TARGET_NAME} INTERFACE ${LIB_PATH})
    endif()
endfunction()

# This function is called for every dependency in every package, e.g. 'boost', 'freetype', 'libcurl', etc.
# It creates the CMake target for each dependency and adds the library files to the targets
function(import_dependency DEP_NAME DEP_ROOT PACKAGE_SOURCE_DIR)

    # Find all the library files in one dependency
    file(GLOB_RECURSE libraries "${DEP_ROOT}/**/${CMAKE_STATIC_LIBRARY_PREFIX}**${CMAKE_STATIC_LIBRARY_SUFFIX}")

    set(TARGET_NAME of-deps-${DEP_NAME})
    add_library(${TARGET_NAME} INTERFACE)

    if (libraries)
        # Some Libraries were found -> Link all of them into a single target
        foreach(LIB_PATH IN LISTS libraries)     # Loop over all found library files

            get_filename_component(LIB_DIR ${LIB_PATH} DIRECTORY)
            get_filename_component(LIB_DIR_NAME ${LIB_DIR} NAME)
            get_filename_component(LIB_NAME ${LIB_PATH} NAME)
            get_filename_component(LIB_EXTENSION ${LIB_PATH} LAST_EXT)

            add_libraries_to_dependency(${LIB_DIR_NAME} ${LIB_NAME} ${DEP_NAME} ${LIB_PATH})

        endforeach()

    endif()
    add_library(of::${DEP_NAME} ALIAS of-deps-${DEP_NAME})

    # Header file directories
    if (${DEP_NAME} STREQUAL "cairo")                       # Unfortunately cairo and freetype are inconsistent in the apothecary
        target_include_directories(of-deps-${DEP_NAME} INTERFACE ${DEP_ROOT}/include/cairo)
    elseif(${DEP_NAME} STREQUAL "freetype")
        target_include_directories(of-deps-${DEP_NAME} INTERFACE ${DEP_ROOT}/include/freetype2)
    else()
        target_include_directories(of-deps-${DEP_NAME} INTERFACE ${DEP_ROOT}/include)
    endif()

endfunction()
