

#pragma once

#include "baseProject.h"



class xcodeProject : public baseProject {
    
public: 
    
    xcodeProject(){};
    
    
    void setup();
    
    bool load(string path);
    bool create(string path); // not done yet. 
	bool save(string path);

    void addSrc(string srcFile, string folder);
    void addInclude(string includeName);
    void addLibrary(string libraryName);

	void addAddon(ofAddon & addon);
    
    pugi::xml_node findOrMakeFolderSet( pugi::xml_node nodeToAddTo, vector < string > & folders, string pathForHash);
    
    
	string getName();
	string getPath();

    
    pugi::xml_node insertPoint;         // where are we inserting items (at the second dict tag,
                                        // /plist[1]/dict[1]/dict[2])
    
    
    void renameProject();
    
    
    
    string projectDir, projectName;
    vector<ofAddon> addons;
    void parseAddons();
    static string LOG_NAME;
    
    
   bool findArrayForUUID(string UUID, pugi::xml_node & nodeMe);
    
};




/*
 //http://code.google.com/p/pugixml/source/browse/trunk/docs/samples/modify_add.cpp
 // this is how to add a tag with a value. 
 pugi::xml_node descr = node.append_child("description");
 descr.append_child(pugi::node_pcdata).set_value("Simple node");
 */


//  this gets the names of all PBXGroup "group" tags....
//  not the ideal way to deal with folder tho. 
//  string[contains(.,'&lt;group')]/parent::node()//string[contains(.,'PBXGroup')]/parent::node()//key[contains(.,'name')]/following-sibling::node()[1]




//http://pocoproject.org/docs/Poco.UUIDGenerator.html


// search paths: 




// notes for include / linking
// it's in the dict here. (there are 2, release and debug) 

//  <dict>
//  <key>baseConfigurationReference</key>


// XPATH:   //key[contains(.,'baseConfigurationReference')]/parent::node()


// it would look like: 

//    <key>HEADER_SEARCH_PATHS</key>
//    <array>
//    <string>../../../includeTest.h</string>
//    <string>$(OF_CORE_HEADERS)</string>
//    </array>

//    <key>LIBRARY_SEARCH_PATHS</key>
//    <array>
//    <string>../../../librarySearchPath</string>
//    </array>

//    <key>OTHER_CPLUSPLUSFLAGS</key>
//    <array>
//    <string>-D__MACOSX_CORE__</string>
//    <string>-lpthread</string>
//    </array>

//    <key>OTHER_LDFLAGS</key>
//    <array>
//    <string>testLib.a</string>
//    <string>$(OF_CORE_LIBS)</string>
//    </array>

// we need to add

// HEADER_SEARCH_PATHS
// LIBRARY_SEARCH_PATHS
// OTHER_LDFLAGS

// if they don't exist, 
// because with the config files, this stuff is kind of missing. 
// I would guess anywhere in that dict is ok, order seems not to matter. 

// check if they exist: 
//key[contains(.,'baseConfigurationReference')]/parent::node()//key[contains(.,'HEADER_SEARCH_PATHS')]
//if they exist, get the array
//key[contains(.,'baseConfigurationReference')]/parent::node()//key[contains(.,'HEADER_SEARCH_PATHS')]//array


//if it exists, it looks like this to get to the array: 

//key[contains(.,'baseConfigurationReference')]/parent::node()//key[contains(.,'OTHER_CPLUSPLUSFLAGS')]/following-sibling::node()

//          returns

//        <array>
//        <string>-D__MACOSX_CORE__</string>
//        <string>-lpthread</string>
//        </array>
//        <array>
//        <string>-D__MACOSX_CORE__</string>
//        <string>-lpthread</string>
//        </array>



//  this is for SRC
//  key[contains(.,"E4B69E1C0A3A1BDC003C02F2")]/following-sibling::node()[1]
//  this is for addons
//  key[contains(.,"BB4B014C10F69532006C3DED")]/following-sibling::node()[1]

//  key[contains(.,"E4B69E1C0A3A1BDC003C02F2")]/following-sibling::node()[1]//array/string/text()
//  E4B69E1F0A3A1BDC003C02F2
// 
//  gets you all keys for pbxgroups
//  string[contains(.,'PBXGroup')]/parent::node()/preceding-sibling::node()[1]
//  checks, does a group exist, for this key?
//  string[contains(.,'PBXGroup')]/parent::node()/preceding-sibling::node()[1][contains(.,'BB4B014C10F69532006C3DED')]


// does group exist
// add group to group
// add fileToGroup


//bool xcodeProject::bDoesFileExist(string fileName, string fileType){
//    
//    // grab all the strings
//    pugi::xpath_node_set source = doc.select_nodes("//string/text()");
//    
//    // if it equals something like "sourcecode.cpp.cpp" check the file name
//    for (pugi::xpath_node_set::const_iterator it = source.begin(); it != source.end(); ++it){
//        pugi::xpath_node node = *it;
//        if (strcmp(node.node().value(), fileType.c_str()) == 0){
//            pugi::xpath_node_set set = node.node().parent().parent().select_nodes("string[4]");
//            set.begin()->node().first_child().value();
//        }
//    }
//    return false;
//}


