/*
 * ofAddon.cpp
 *
 *  Created on: 28/12/2011
 *      Author: arturo
 */

#include "ofAddon.h"
#include "pugixml.hpp"
#include "ofUtils.h"
#include "ofFileUtils.h"
#include "Utils.h"
#include <list>

ofAddon::ofAddon(){
    pathToOF = "../../..";
}

void ofAddon::fromFS(string path, string platform){
	
    clear();
	name = ofFilePath::getFileName(path);

    string filePath = path + "/src";
    string ofRootPath = ofFilePath::addTrailingSlash(getOFRoot()); //we need to add a trailing slash for the erase to work properly
    
    ofLogVerbose() << "in fromFS, trying src " << filePath << endl;
    getFilesRecursively(filePath, srcFiles);

    for(int i=0;i<(int)srcFiles.size();i++){
    	srcFiles[i].erase (srcFiles[i].begin(), srcFiles[i].begin()+ofRootPath.length());
		ofLogVerbose() << " srcFiles " << srcFiles[i] << endl; 
    	int init = 0;
    	int end = srcFiles[i].rfind("/");
    	string folder = srcFiles[i].substr(init,end);
    	srcFiles[i] = pathToOF + srcFiles[i];
    	filesToFolders[srcFiles[i]] = folder;
    }

    string libsPath = path + "/libs";
    vector < string > libFiles;
    vector < string > libLibs;
    if (ofDirectory::doesDirectoryExist(libsPath)){
        getLibsRecursively(libsPath, libFiles, libs, platform);
    }

    // I need to add libFiles to srcFiles
    for (int i = 0; i < (int)libFiles.size(); i++){
    	libFiles[i].erase (libFiles[i].begin(), libFiles[i].begin()+ofRootPath.length());
		ofLogVerbose() << " libFiles " << libFiles[i] << endl; 
    	int init = 0;
    	int end = libFiles[i].rfind("/");
    	string folder = libFiles[i].substr(init,end);
    	libFiles[i] = pathToOF + libFiles[i];
        srcFiles.push_back(libFiles[i]);
    	filesToFolders[libFiles[i]] = folder;
    }

    for (int i = 0; i < (int)libs.size(); i++){
    	libs[i].erase (libs[i].begin(), libs[i].begin()+ofRootPath.length());
    	libs[i] = pathToOF + libs[i];
    }

    
    // get a unique list of the paths that are needed for the includes.
    list < string > paths;
    for (int i = 0; i < (int)srcFiles.size(); i++){
        size_t found;
        found = srcFiles[i].find_last_of("/");
        paths.push_back(srcFiles[i].substr(0,found));
    }
    
    // get every folder in addon/src and addon/libs
    
    vector < string > libFolders;
    ofLogVerbose() << "trying get folders recursively " << (path + "/libs") << endl;
    getFoldersRecursively(path + "/libs", libFolders, platform);
    
    vector < string > srcFolders;
    getFoldersRecursively(path + "/src", srcFolders, platform);
    
    for (int i = 0; i < libFolders.size(); i++){
        libFolders[i].erase (libFolders[i].begin(), libFolders[i].begin()+ofRootPath.length());
        libFolders[i] = pathToOF + libFolders[i];
        paths.push_back(libFolders[i]);
    }
    
    for (int i = 0; i < srcFolders.size(); i++){
        srcFolders[i].erase (srcFolders[i].begin(), srcFolders[i].begin()+ofRootPath.length());
        srcFolders[i] = pathToOF + srcFolders[i];
        paths.push_back(srcFolders[i]);
    }
    
    
    paths.sort();
    paths.unique();
    for (list<string>::iterator it=paths.begin(); it!=paths.end(); ++it){
        includePaths.push_back(*it);
    }

}

void ofAddon::fromXML(string installXmlName){
	clear();
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(ofToDataPath(installXmlName).c_str());

    // this is src to add:
    pugi::xpath_node_set add = doc.select_nodes("//add/src/folder/file");
    for (pugi::xpath_node_set::const_iterator it = add.begin(); it != add.end(); ++it){
        pugi::xpath_node node = *it;
        //std::cout << "folder name "  << node.node().parent().attribute("name").value() << " : ";
        //std::cout << "src: " << node.node().child_value() << endl;
    }


    add = doc.select_nodes("//include/path");
    for (pugi::xpath_node_set::const_iterator it = add.begin(); it != add.end(); ++it){
        pugi::xpath_node node = *it;
        //std::cout << "include: " << node.node().child_value() << endl;
    }


    add = doc.select_nodes("//link/lib[@compiler='codeblocks']");
    // this has to be smarter I guess...
    for (pugi::xpath_node_set::const_iterator it = add.begin(); it != add.end(); ++it){
        pugi::xpath_node node = *it;
        //std::cout << "link: " << node.node().child_value() << endl;
    }


}


void ofAddon::clear(){
    filesToFolders.clear();
    srcFiles.clear();
    libs.clear();
    includePaths.clear();
    name.clear();
}
