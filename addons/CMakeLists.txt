message(STATUS "[openframeworks] Configuring addons")

function(define_interface_addon TARGET_NAME)
    add_library(${TARGET_NAME} INTERFACE)
    add_library(of::${TARGET_NAME} ALIAS ${TARGET_NAME})
    target_link_libraries(${TARGET_NAME} INTERFACE of::openframeworks)

    target_include_directories(${TARGET_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
    target_include_directories(${TARGET_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src)
endfunction()

function(define_addon_without_paths TARGET_NAME)
    add_library(${TARGET_NAME} STATIC EXCLUDE_FROM_ALL)
    add_library(of::${TARGET_NAME} ALIAS ${TARGET_NAME})
    target_link_libraries(${TARGET_NAME} of::openframeworks)

    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
    set_target_properties(${TARGET_NAME} PROPERTIES CXX_EXTENSIONS OFF)
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "openframeworks/addons")

    set_target_properties(${TARGET_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/$<IF:$<CONFIG:Debug>,debug,release>"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/$<IF:$<CONFIG:Debug>,debug,release>"
    )
endfunction()

function(define_addon TARGET_NAME)
    define_addon_without_paths(${TARGET_NAME})

    file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/**)
    target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)
    source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/" PREFIX "" FILES ${SOURCES})
endfunction()

add_subdirectory(ofxAccelerometer)

if (OF_TARGET_ARCHITECTURE MATCHES "android")
    add_subdirectory(ofxAndroid)
endif()

add_subdirectory(ofxAssimpModelLoader)

if (OF_TARGET_ARCHITECTURE MATCHES "emscripten")
    add_subdirectory(ofxEmscripten)
endif()

add_subdirectory(ofxGPS)
add_subdirectory(ofxGui)
# add_subdirectory(ofxiOS)      # Not implemented yet, please contribute!
add_subdirectory(ofxKinect)
add_subdirectory(ofxNetwork)
add_subdirectory(ofxOpenCv)
add_subdirectory(ofxOsc)
add_subdirectory(ofxPoco) 
add_subdirectory(ofxSvg)
add_subdirectory(ofxThreadedImageLoader)
add_subdirectory(ofxUnitTests)
add_subdirectory(ofxVectorGraphics)
add_subdirectory(ofxXmlSettings)