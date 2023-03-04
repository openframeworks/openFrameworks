
# A macro that will add all the files in the given module directory
macro(add_module MODULE_NAME)
    target_include_directories(openframeworks-core PUBLIC ${CMAKE_CURRENT_LIST_DIR}/${MODULE_NAME})   # Include directory
    file(GLOB OF_SOURCES "${CMAKE_CURRENT_LIST_DIR}/${MODULE_NAME}/**") # Find and add all the source files
    target_sources(openframeworks-core PRIVATE ${OF_SOURCES})

    # Find all obj-c files and exclude them from parallel builds
    #file(GLOB OBJC_FILES "${CMAKE_CURRENT_LIST_DIR}/${MODULE_NAME}/**.m")
    #set_source_files_properties(${OBJC_FILES} PROPERTIES COMPILE_FLAGS "/MP")
    # TODO: Implement parallel builds in MSVC!!!
endmacro()

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
