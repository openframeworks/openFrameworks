
if (OF_TARGET_ARCHITECTURE MATCHES "android" OR OF_TARGET_ARCHITECTURE MATCHES "ios")

    define_raw_addon_without_paths(ofxGPS)

    file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/common/**)
    target_sources(ofxGPS PRIVATE ${SOURCES})
    target_include_directories(ofxGPS PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/common)
    source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/" PREFIX "" FILES ${SOURCES})

    if (OF_TARGET_ARCHITECTURE MATCHES "android")
        file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/android/**)
        target_sources(ofxGPS PRIVATE ${SOURCES})
        target_include_directories(ofxGPS PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/android)
        source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/" PREFIX "" FILES ${SOURCES})
    endif()

    if (OF_TARGET_ARCHITECTURE MATCHES "ios")
        file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/ios/**)
        target_sources(ofxGPS PRIVATE ${SOURCES})
        target_include_directories(ofxGPS PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/ios)
        source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/" PREFIX "" FILES ${SOURCES})
    endif()

endif()