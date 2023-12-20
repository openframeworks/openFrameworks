define_addon(ofxPoco)

find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Zip)
target_link_libraries(ofxPoco Poco::Foundation Poco::Util Poco::Net Poco::XML Poco::JSON Poco::Zip)