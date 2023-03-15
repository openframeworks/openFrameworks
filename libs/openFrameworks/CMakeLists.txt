# We want all files in the folder to be our sources (everything should be shown in the IDE)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/**)
target_sources(openframeworks PRIVATE ${SOURCES})
target_include_directories(openframeworks PUBLIC ${CMAKE_CURRENT_LIST_DIR})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/" PREFIX "src" FILES ${SOURCES})

# And any subfolder (except starting with a dot) becomes an include directory
file(GLOB subfolders ${CMAKE_CURRENT_LIST_DIR}/**)
foreach(subfolder ${subfolders})
    get_filename_component(sub_name ${subfolder} NAME)
    if (IS_DIRECTORY ${subfolder} AND NOT ${sub_name} MATCHES "^\\.")
        target_include_directories(openframeworks PUBLIC ${subfolder})
    endif()
endforeach()