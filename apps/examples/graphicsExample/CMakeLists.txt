cmake_minimum_required(VERSION 2.8)
set(PROJECT "graphicsExample")
project(${PROJECT})

include(FindPkgConfig)
pkg_check_modules(OF REQUIRED openFrameworks)

include_directories(${OF_INCLUDE_DIRS})

include_directories( src ${OF_INCLUDE_DIRS})

add_executable(${PROJECT} src/testApp.cpp src/main.cpp )

target_link_libraries(${PROJECT} ${OF_LIBRARIES})
