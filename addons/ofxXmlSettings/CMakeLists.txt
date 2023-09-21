define_addon(ofxXmlSettings)
target_sources(ofxXmlSettings PRIVATE libs/tinyxml.cpp libs/tinyxmlerror.cpp libs/tinyxmlparser.cpp)
target_include_directories(ofxXmlSettings PUBLIC libs)
target_link_libraries(ofxXmlSettings of::svgtiny)