#include "xcodeProject.h"
#include <iostream>
#include "Utils.h"

#include "Poco/HMACEngine.h"
#include "Poco/MD5Engine.h"

using Poco::DigestEngine;
using Poco::HMACEngine;
using Poco::MD5Engine;

#define STRINGIFY(A)  #A



//-----------------------------------------------------------------
const char PBXGroup[] = 
STRINGIFY(

    <key>GROUPUUID</key>
    <dict>
        <key>children</key>
        <array>
        </array>
        <key>isa</key>
        <string>PBXGroup</string>
        <key>name</key>
        <string>GROUPNAME</string>
        <key>sourceTree</key>
        <string>&lt;group&gt;</string>      // <group> or SOURCE_ROOT, etc
    </dict>
          
);

//-----------------------------------------------------------------
const char PBXFileReference[] = 
STRINGIFY(

        <key>FILEUUID</key>
        <dict>
            <key>explicitFileType</key>
            <string>FILETYPE</string>
            <key>fileEncoding</key>
            <string>30</string>
            <key>isa</key>
            <string>PBXFileReference</string>
            <key>name</key>
            <string>FILENAME</string>
            <key>path</key>
            <string>FILEPATH</string>
            <key>sourceTree</key>
            <string>SOURCE_ROOT</string>
        </dict>

);

//-----------------------------------------------------------------
const char PBXBuildFile[] = 
STRINGIFY(
          
          <key>BUILDUUID</key>
          <dict>
          <key>fileRef</key>
          <string>FILEUUID</string>
          <key>isa</key>
          <string>PBXBuildFile</string>
          </dict>
          
);

//-----------------------------------------------------------------
const char HeaderSearchPath[] = 
STRINGIFY(
          
    <key>HEADER_SEARCH_PATHS</key>
    <array>
    <string>$(OF_CORE_HEADERS)</string>
    </array>

);


//-----------------------------------------------------------------
const char LDFlags[] = 
STRINGIFY(
          
          <key>OTHER_LDFLAGS</key>
          <array>
          <string>$(OF_CORE_LIBS)</string>
          </array>
          
);


string xcodeProject::LOG_NAME = "xcodeProject";


void xcodeProject::setup(){
    templatePath = "xcode/template/";
}


//void xcodeProject::parseAddons(){
//	addons.clear();
//	ofFile addonsmake(projectDir+"addons.make");
//	if(!addonsmake.exists()){
//		addonsmake.create();
//		addonsmake.open(projectDir+"addons.make");
//	}
//	ofBuffer addonsmakebuff;
//	addonsmake >> addonsmakebuff;
//	while(!addonsmakebuff.isLastLine() && addonsmakebuff.size() > 0){
//		string line = addonsmakebuff.getNextLine();
//		if(line!=""){
//			addons.push_back(ofAddon(getOFRoot()+"/addons/"+line,"xcode"));
//		}
//	}
//}

bool xcodeProject::create(string path){
    
    projectDir = ofFilePath::addTrailingSlash(path);
	projectName = ofFilePath::getFileName(path);
	
    //cout << "project dir:" << projectDir;
    //cout << "project name:" << projectName;
    
    bool bDoesDirExist = false;
    
    ofDirectory project(projectDir);    // this is a directory, really?
	if(project.exists()){
        bDoesDirExist = true;
    }
    
    if (!bDoesDirExist){
    
        ofFile project(projectDir + projectName + ".xcodeproj");    // this is a directory, really?
        
        if(!project.exists()){
            ofLogVerbose(LOG_NAME) << "creating non existent project";
            ofDirectory dir(projectDir);
            dir.create(true);
            ofFile::copyFromTo(templatePath + "/src",projectDir);
            ofFile::copyFromTo(templatePath + "/bin",projectDir);
            ofFile::copyFromTo(templatePath + "/emptyExample.xcodeproj", projectDir);
            ofFile::copyFromTo(templatePath + "/openFrameworks-Info.plist", projectDir);
            ofFile::copyFromTo(templatePath + "/Project.xcconfig", projectDir);
            load(projectDir + "emptyExample.xcodeproj/project.pbxproj");
            renameProject();
            string xcodeProject = projectDir + "/" + projectName + ".xcodeproj";
            ofDirectory xcodeDir(xcodeProject);
            xcodeDir.create(true);
            save(projectDir + "/" + projectName + ".xcodeproj" + "/project.pbxproj");
            ofDirectory::removeDirectory(projectDir + "/emptyExample.xcodeproj", true);
        } else {
            // this exists, what to do now?  (load and parse?)
        }
        
    } else {
        printf("this exists ! \n");
        vector < string > fileNames;
       
        getFilesRecursively(projectDir + "src", fileNames);
        
        ofFile::copyFromTo(templatePath + "/emptyExample.xcodeproj", projectDir);
        ofFile::copyFromTo(templatePath + "/openFrameworks-Info.plist", projectDir);
        ofFile::copyFromTo(templatePath + "/Project.xcconfig", projectDir);
        
        load(projectDir + "emptyExample.xcodeproj/project.pbxproj");
        renameProject();
        
        string xcodeProject = projectDir + "/" + projectName + ".xcodeproj";
        ofDirectory xcodeDir(xcodeProject);
        xcodeDir.create(true);
        save(projectDir + "/" + projectName + ".xcodeproj" + "/project.pbxproj");
        
        
        if (projectName != "emptyExample"){
            ofDirectory::removeDirectory(projectDir + "/emptyExample.xcodeproj", true);
        }
        
        for (int i = 0; i < fileNames.size(); i++){
            
            string first, last;
            splitFromFirst(fileNames[i],projectName, first, last);  
            
            // last is now something like " /src/main.cpp"
            // drop the trailing slash;
            last.erase(last.begin());
            
            string fileName = last;
            splitFromLast(fileName,"/", first, last);  
            
            if (fileName != "src/testApp.cpp" &&
                fileName != "src/testApp.h" &&
                fileName != "src/main.cpp"){
                    addSrc(fileName, first);
            }
        }
        
        
        
        
    }
    return bLoaded;
    
    
}


void xcodeProject::renameProject(){
    pugi::xpath_node_set uuidSet = doc.select_nodes("//string[contains(.,'emptyExample')]");
    for (pugi::xpath_node_set::const_iterator it = uuidSet.begin(); it != uuidSet.end(); ++it){
        pugi::xpath_node node = *it;
        string val = it->node().first_child().value();
        
        findandreplace(val, "emptyExample",  projectName);
        
        it->node().first_child().set_value(val.c_str());
        //( std::string& tInput, std::string tFind, std::string tReplace ) 
    }
}


bool xcodeProject::load(string path){
    pugi::xml_parse_result result = doc.load_file(ofToDataPath(path).c_str());
    
    //bLoaded = true;
    
	return result.status==pugi::status_ok;
}  


bool xcodeProject::save(string fileName){
    return doc.save_file(ofToDataPath(fileName).c_str());
}  


bool xcodeProject::findArrayForUUID(string UUID, pugi::xml_node & nodeMe){
    char query[255];
    sprintf(query, "//string[contains(.,'%s')]", UUID.c_str());
    pugi::xpath_node_set uuidSet = doc.select_nodes(query);
    for (pugi::xpath_node_set::const_iterator it = uuidSet.begin(); it != uuidSet.end(); ++it){
        pugi::xpath_node node = *it;
        if (strcmp(node.node().parent().name(), "array") == 0){
            nodeMe = node.node().parent();
            return true;
        } else {
        }
    }
    return false;
}



string generateUUID(string input){
    
    std::string passphrase("openFrameworks"); // HMAC needs a passphrase
    
    HMACEngine<MD5Engine> hmac(passphrase); // we'll compute a MD5 Hash
    hmac.update(input);
    
    const DigestEngine::Digest& digest = hmac.digest(); // finish HMAC computation and obtain digest
    std::string digestString(DigestEngine::digestToHex(digest)); // convert to a string of hexadecimal numbers
    
    return digestString;	
    
    
}





pugi::xml_node xcodeProject::findOrMakeFolderSet(pugi::xml_node nodeToAddTo, vector < string > & folders, string pathForHash){
    
    
    
    
    char query[255];
    sprintf(query, "//key[contains(.,'%s')]/following-sibling::node()[1]//array/string", nodeToAddTo.previous_sibling().first_child().value());
    pugi::xpath_node_set array = doc.select_nodes(query);
    
    bool bAnyNodeWithThisName = false;
    pugi::xml_node nodeWithThisName;
    string name = folders[0];
    
    
    for (pugi::xpath_node_set::const_iterator it = array.begin(); it != array.end(); ++it){
        
        pugi::xpath_node node = *it;
        //node.node().first_child().print(std::cout);
        
        // this long thing checks, is this a pbxgroup, and if so, what's it's name. 
        // do it once for path and once for name, since ROOT level pbxgroups have a path name. ugh. 
        
        char querypbx[255];
        sprintf(querypbx, "//key[contains(.,'%s')]/following-sibling::node()[1]//string[contains(.,'PBXGroup')]/parent::node()[1]//key[contains(.,'path')]/following-sibling::node()[1]", node.node().first_child().value());
        if (doc.select_single_node(querypbx).node() != NULL){
            
            if (strcmp(doc.select_single_node(querypbx).node().first_child().value(), folders[0].c_str()) == 0){
                printf("found matching node \n");
                bAnyNodeWithThisName = true;
                nodeWithThisName = doc.select_single_node(querypbx).node().parent();
            }
        }
        
        sprintf(querypbx, "//key[contains(.,'%s')]/following-sibling::node()[1]//string[contains(.,'PBXGroup')]/parent::node()[1]//key[contains(.,'name')]/following-sibling::node()[1]", node.node().first_child().value());
        
        if (doc.select_single_node(querypbx).node() != NULL){
            if (strcmp(doc.select_single_node(querypbx).node().first_child().value(), folders[0].c_str()) == 0){
                bAnyNodeWithThisName = true;
                nodeWithThisName = doc.select_single_node(querypbx).node().parent();
            }
        }
        
    }
    
   
    
    // now, if we have a pbxgroup with the right name, pop this name off the folder set, and keep going. 
    // else, let's add a folder set, boom. 
    
    if (bAnyNodeWithThisName == false){
        
        // make a new UUID 
        // todo get the full path here somehow... 
        
        pathForHash += "/" + folders[0];
        
        string UUID = generateUUID(pathForHash);
        
        // add a new node
        string PBXGroupStr = string(PBXGroup);
        findandreplace( PBXGroupStr, "GROUPUUID", UUID);
        findandreplace( PBXGroupStr, "GROUPNAME", folders[0]);
        
        pugi::xml_document pbxDoc;
        pugi::xml_parse_result result = pbxDoc.load_buffer(PBXGroupStr.c_str(), strlen(PBXGroupStr.c_str()));
        
        
        nodeWithThisName = doc.select_single_node("/plist[1]/dict[1]/dict[2]").node().prepend_copy(pbxDoc.first_child().next_sibling()); 
        doc.select_single_node("/plist[1]/dict[1]/dict[2]").node().prepend_copy(pbxDoc.first_child()); 
        
        
        
        // add to array
        char queryArray[255];
        sprintf(queryArray, "//key[contains(.,'%s')]/following-sibling::node()[1]//array", nodeToAddTo.previous_sibling().first_child().value());
        doc.select_single_node(queryArray).node().append_child("string").append_child(pugi::node_pcdata).set_value(UUID.c_str());
        //array.begin()->node().parent().append_child("string").append_child(pugi::node_pcdata).set_value(UUID.c_str());
        
        
    } else {
        
        pathForHash += "/" + folders[0];
    }
    
    
    folders.erase(folders.begin());
    
    if (folders.size() > 0){ 
        return findOrMakeFolderSet(nodeWithThisName, folders, pathForHash);
    } else {
        return nodeWithThisName;
    }

}






void xcodeProject::addSrc(string srcFile, string folder){
    
    
    
    string buildUUID;
    
    //-----------------------------------------------------------------
    // find the extension for the file that's passed in. 
    //-----------------------------------------------------------------
    
    size_t found = srcFile.find_last_of(".");
    string ext = srcFile.substr(found+1);
    
    //-----------------------------------------------------------------
    // based on the extension make some choices about what to do:
    //-----------------------------------------------------------------
    
    bool addToResources = true;
    bool addToBuild = true;
    string fileKind = "";
    bool bAddFolder = true;
    
    if( ext == "cpp" ){
        fileKind = "sourcecode.cpp.cpp";
        addToResources = false;													
    }
    if( ext == "c" ){
        fileKind = "sourcecode.c.c";				
        addToResources = false;															
    }
    if(ext == "h" || ext == "hpp"){
        fileKind = "sourcecode.c.h";
        addToBuild = false;
        addToResources = false;													
    }
    if(ext == "mm" || ext == "m"){
        addToResources = false;						
        fileKind = "sourcecode.cpp.objcpp";
    }
    if(ext == "xib"){
        addToBuild	= false;
    }
    if (folder == "src"){
        bAddFolder = false;
    }
    
    //-----------------------------------------------------------------
    // (A) make a FILE REF
    //-----------------------------------------------------------------
    
    string pbxfileref = string(PBXFileReference);
    string UUID = generateUUID(srcFile);   // replace with theo's smarter system. 
    
    string name, path;
    splitFromLast(srcFile, "/", path, name);
    
    findandreplace( pbxfileref, "FILENAME", name);
    findandreplace( pbxfileref, "FILEPATH", srcFile);
    findandreplace( pbxfileref, "FILETYPE", fileKind);
    findandreplace( pbxfileref, "FILEUUID", UUID);
    
    pugi::xml_document fileRefDoc;
    pugi::xml_parse_result result = fileRefDoc.load_buffer(pbxfileref.c_str(), strlen(pbxfileref.c_str()));
    
    // insert it at <plist><dict><dict>
    doc.select_single_node("/plist[1]/dict[1]/dict[2]").node().prepend_copy(fileRefDoc.first_child().next_sibling());   // UUID FIRST
    doc.select_single_node("/plist[1]/dict[1]/dict[2]").node().prepend_copy(fileRefDoc.first_child());                  // DICT SECOND
    
    //-----------------------------------------------------------------
    // (B) BUILD REF
    //-----------------------------------------------------------------
    
    if (addToBuild == true){
        
        buildUUID = generateUUID(srcFile + "-build");
        string pbxbuildfile = string(PBXBuildFile);
        findandreplace( pbxbuildfile, "FILEUUID", UUID);
        findandreplace( pbxbuildfile, "BUILDUUID", buildUUID);
        fileRefDoc.load_buffer(pbxbuildfile.c_str(), strlen(pbxbuildfile.c_str()));
        doc.select_single_node("/plist[1]/dict[1]/dict[2]").node().prepend_copy(fileRefDoc.first_child().next_sibling());   // UUID FIRST
        doc.select_single_node("/plist[1]/dict[1]/dict[2]").node().prepend_copy(fileRefDoc.first_child());                  // DICT SECOND
        
        // add it to the build array. 
        pugi::xml_node array;
        findArrayForUUID("E4B69E200A3A1BDC003C02F2", array);    // this is the build array (all build refs get added here)
        array.append_child("string").append_child(pugi::node_pcdata).set_value(buildUUID.c_str());
    }
    
    //-----------------------------------------------------------------
    // (C) resrouces 
    //-----------------------------------------------------------------
    
    
    if (addToResources == true){
        // no idea where resources go
    }
    
    
    //-----------------------------------------------------------------
    // (D) folder
    //-----------------------------------------------------------------
    
 
    if (bAddFolder == true){
        
        vector < string > folders = ofSplitString(folder, "/", true);
        
        if (folders.size() > 1){
            if (folders[0] == "src"){
                
                folders.erase(folders.begin());
                pugi::xml_node node = doc.select_single_node("//key[contains(.,'E4B69E1C0A3A1BDC003C02F2')]/following-sibling::node()[1]").node();
                pugi::xml_node nodeToAddTo = findOrMakeFolderSet( node, folders, "src");
                nodeToAddTo.child("array").append_child("string").append_child(pugi::node_pcdata).set_value(UUID.c_str());
                
            } else if (folders[0] == "addons"){
                
                folders.erase(folders.begin());
                pugi::xml_node node = doc.select_single_node("//key[contains(.,'BB4B014C10F69532006C3DED')]/following-sibling::node()[1]").node();
                pugi::xml_node nodeToAddTo = findOrMakeFolderSet( node, folders, "addons");
                
                nodeToAddTo.child("array").append_child("string").append_child(pugi::node_pcdata).set_value(UUID.c_str());
                
            } else {
                
                pugi::xml_node node = doc.select_single_node("//key[contains(.,'E4B69E1C0A3A1BDC003C02F2')]/following-sibling::node()[1]").node();
                
                // I'm not sure the best way to proceed;
                // we should maybe find the rootest level and add it there. 
                // TODO: fix this. 
            }
        }; 
        
    } else {
        
        
        pugi::xml_node array;
        pugi::xml_node node = doc.select_single_node("//key[contains(.,'E4B69E1C0A3A1BDC003C02F2')]/following-sibling::node()[1]").node();
        node.child("array").append_child("string").append_child(pugi::node_pcdata).set_value(UUID.c_str());
        //nodeToAddTo.child("array").append_child("string").append_child(pugi::node_pcdata).set_value(UUID.c_str());
        
    }
    
    
    
    save(projectDir + "/" + projectName + ".xcodeproj" + "/project.pbxproj");
} 


// todo: these three have very duplicate code... please fix up a bit. 

void xcodeProject::addInclude(string includeName){
    

    char query[255];
    sprintf(query, "//key[contains(.,'baseConfigurationReference')]/parent::node()//key[contains(.,'HEADER_SEARCH_PATHS')]/following-sibling::node()[1]");
    pugi::xpath_node_set headerArray = doc.select_nodes(query);

    if (headerArray.size() > 0){
        
        for (pugi::xpath_node_set::const_iterator it = headerArray.begin(); it != headerArray.end(); ++it){
            pugi::xpath_node node = *it;
            //node.node().print(std::cout);
            node.node().append_child("string").append_child(pugi::node_pcdata).set_value(includeName.c_str());
        }
        
    } else {
        
        //printf("we don't have HEADER_SEARCH_PATHS, so we're adding them... and calling this function again \n");
        query[255];
        sprintf(query, "//key[contains(.,'baseConfigurationReference')]/parent::node()//key[contains(.,'buildSettings')]/following-sibling::node()[1]");
        pugi::xpath_node_set dictArray = doc.select_nodes(query);
        
        
        for (pugi::xpath_node_set::const_iterator it = dictArray.begin(); it != dictArray.end(); ++it){
            pugi::xpath_node node = *it;
            
            //node.node().print(std::cout);
            string headerXML = string(HeaderSearchPath);
            pugi::xml_document headerDoc;
            pugi::xml_parse_result result = headerDoc.load_buffer(headerXML.c_str(), strlen(headerXML.c_str()));
            
            // insert it at <plist><dict><dict>
            node.node().prepend_copy(headerDoc.first_child().next_sibling());   // KEY FIRST
            node.node().prepend_copy(headerDoc.first_child());                  // ARRAY SECOND
            
        }
        
        // now that we have it, try again...
        addInclude(includeName);
    }
    
    save(projectDir + "/" + projectName + ".xcodeproj" + "/project.pbxproj");

}  
        
        
void xcodeProject::addLibrary(string libraryName){
    
    // split apart path or not?
    
    
    char query[255];
    sprintf(query, "//key[contains(.,'baseConfigurationReference')]/parent::node()//key[contains(.,'OTHER_LDFLAGS')]/following-sibling::node()[1]");
    pugi::xpath_node_set headerArray = doc.select_nodes(query);
    
    
    if (headerArray.size() > 0){
        
        for (pugi::xpath_node_set::const_iterator it = headerArray.begin(); it != headerArray.end(); ++it){
            
            pugi::xpath_node node = *it;
            node.node().append_child("string").append_child(pugi::node_pcdata).set_value(libraryName.c_str());
        }
        
    } else {
        
        //printf("we don't have OTHER_LDFLAGS, so we're adding them... and calling this function again \n");
        query[255];
        sprintf(query, "//key[contains(.,'baseConfigurationReference')]/parent::node()//key[contains(.,'buildSettings')]/following-sibling::node()[1]");
        
        pugi::xpath_node_set dictArray = doc.select_nodes(query);
        
        
        for (pugi::xpath_node_set::const_iterator it = dictArray.begin(); it != dictArray.end(); ++it){
            pugi::xpath_node node = *it;
            
            //node.node().print(std::cout);
            string ldXML = string(LDFlags);
            pugi::xml_document ldDoc;
            pugi::xml_parse_result result = ldDoc.load_buffer(ldXML.c_str(), strlen(ldXML.c_str()));
            
            // insert it at <plist><dict><dict>
            node.node().prepend_copy(ldDoc.first_child().next_sibling());   // KEY FIRST
            node.node().prepend_copy(ldDoc.first_child());                  // ARRAY SECOND
            
            //node.node().print(std::cout);
        }
        
        // now that we have it, try again...
        addLibrary(libraryName);
    }
    
    save(projectDir + "/" + projectName + ".xcodeproj" + "/project.pbxproj");
}

void xcodeProject::addAddon(ofAddon & addon){
    
    for(int i=0;i<(int)addon.includePaths.size();i++){
		addInclude(addon.includePaths[i]);
	}
    
	for(int i=0;i<(int)addon.libs.size();i++){
		addLibrary(addon.libs[i]);
	}
    
	for(int i=0;i< addon.srcFiles.size(); i++){
        addSrc(addon.srcFiles[i],addon.filesToFolders[addon.srcFiles[i]]);
	}
}

string xcodeProject::getName(){
    return projectName;
}

string xcodeProject::getPath(){
    return projectDir;
}
