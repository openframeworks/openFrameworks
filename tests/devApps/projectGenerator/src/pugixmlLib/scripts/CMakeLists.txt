project(pugixml)

set(SOURCES ../src/pugixml.hpp ../src/pugiconfig.hpp ../src/pugixml.cpp)

add_library(pugixml STATIC ${SOURCES})
