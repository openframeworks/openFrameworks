define_addon(ofxSvg)

find_package(svgtiny REQUIRED)
find_package(libxml2 REQUIRED)
target_link_libraries(ofxSvg svgtiny::svgtiny libxml2::libxml2)