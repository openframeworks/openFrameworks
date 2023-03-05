
# A macro that will add all the files in the given module directory
macro(add_module MODULE_NAME)
    target_include_directories(openframeworks PUBLIC ${CMAKE_CURRENT_LIST_DIR}/${MODULE_NAME})   # Include directory
    file(GLOB OF_SOURCES "${CMAKE_CURRENT_LIST_DIR}/${MODULE_NAME}/**") # Find and add all the source files
    target_sources(openframeworks PRIVATE ${OF_SOURCES})
    source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/${MODULE_NAME}" PREFIX ${MODULE_NAME} FILES ${OF_SOURCES})
endmacro()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
target_include_directories(openframeworks PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_module(3d)
add_module(app)
add_module(communication)
add_module(events)
add_module(gl)
add_module(graphics)
add_module(math)
add_module(sound)
add_module(types)
add_module(utils)
add_module(video)

target_sources(openframeworks PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ofSources.cmake)