
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**)
define_example(guiFromParametersExample ${SOURCES})

target_link_libraries(guiFromParametersExample ofxGui)

copy_resource_after_build(guiFromParametersExample ${CMAKE_CURRENT_LIST_DIR}/bin/data/ring.wav data)
