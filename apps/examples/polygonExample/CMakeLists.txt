cmake_minimum_required(VERSION 2.8)
# gess project name from current directory name
get_filename_component(PROJECT_ROOT ${CMAKE_CURRENT_LIST_FILE} PATH)                                                                                            
get_filename_component(PROJECT_NAME ${PROJECT_ROOT} NAME)
project(${PROJECT_NAME})
if(APPLE)
  set(CMAKE_OSX_ARCHITECTURES "i386")
endif(APPLE)

include(FindPkgConfig)
pkg_check_modules(OF REQUIRED openFrameworks)

include_directories(${OF_INCLUDE_DIRS})

include_directories( src ${OF_INCLUDE_DIRS})
file( GLOB_RECURSE PROJECT_SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} ${OF_LIBRARIES})
