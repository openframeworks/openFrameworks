/*
 * CBWinProject.cpp
 *
 *  Created on: 28/12/2011
 *      Author: arturo
 */

#include "CBWinProject.h"
#include "ofFileUtils.h"
#include "ofLog.h"
#include "Utils.h"

string CBWinProject::LOG_NAME = "CBWinProject";


void CBWinProject::setup() {
    ;
}

bool CBWinProject::createProjectFile(){
    ofFile project(projectDir + projectName + ".cbp");
    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample.cbp"),project.path());
    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample.workspace"),ofFilePath::join(projectDir, projectName + ".workspace"));
    return true;
}
bool CBWinProject::loadProjectFile(){
    
    //project.open(ofFilePath::join(projectDir , projectName + ".cbp"));
    
    ofFile project(projectDir + projectName + ".cbp");
	if(!project.exists()){
		ofLogError(LOG_NAME) << "error loading" << project.path() << "doesn't exist";
		return false;
	}
	pugi::xml_parse_result result = doc.load(project);
	bLoaded =result.status==pugi::status_ok;
	return bLoaded;
}


bool CBWinProject::saveProjectFile(){
    
    findandreplaceInTexfile(ofFilePath::join(projectDir , projectName + ".workspace"),"emptyExample",projectName);
    pugi::xpath_node_set title = doc.select_nodes("//Option[@title]");
    if(!title.empty()){
        if(!title[0].node().attribute("title").set_value(projectName.c_str())){
            ofLogError(LOG_NAME) << "can't set title";
        }
    }
    doc.save_file((projectDir + projectName + ".cbp").c_str());
    
    //let's do some renaming: 
    string relRoot = getOFRelPath(projectDir);
    
    if (relRoot != "../../../"){
        
        string relRootWindows = relRoot;
        // let's make it windows friendly:
        for(int i = 0; i < relRootWindows.length(); i++) { 
            if( relRootWindows[i] == '/' ) 
                relRootWindows[i] = '\\'; 
        } 
        
        findandreplaceInTexfile(ofFilePath::join(projectDir , projectName + ".workspace"), "../../../", relRoot);
        findandreplaceInTexfile(ofFilePath::join(projectDir , projectName + ".cbp"), "../../../", relRoot);
        
        findandreplaceInTexfile(ofFilePath::join(projectDir , projectName + ".workspace"), "..\\..\\..\\", relRootWindows);
        findandreplaceInTexfile(ofFilePath::join(projectDir , projectName + ".cbp"), "..\\..\\..\\", relRootWindows);
    }
}


void CBWinProject::addSrc(string srcName, string folder){
	pugi::xml_node node = appendValue(doc, "Unit", "filename", srcName);
	if(!node.empty()){
		node.child("Option").attribute("virtualFolder").set_value(folder.c_str());
	}
    doc.save_file((projectDir + projectName + ".cbp").c_str());
}

void CBWinProject::addInclude(string includeName){
    appendValue(doc, "Add", "directory", includeName);
}

void CBWinProject::addLibrary(string libraryName){
    appendValue(doc, "Add", "library", libraryName);
}


void CBWinProject::addAddon(ofAddon & addon){
	for(int i=0;i<(int)addons.size();i++){
		if(addons[i].name==addon.name) return;
	}

	addons.push_back(addon);

	for(int i=0;i<(int)addon.includePaths.size();i++){
		addInclude(addon.includePaths[i]);
	}

	for(int i=0;i<(int)addon.libs.size();i++){
		addLibrary(addon.libs[i]);
	}

	for(int i=0;i<(int)addon.srcFiles.size();i++){
		addSrc(addon.srcFiles[i],addon.filesToFolders[addon.srcFiles[i]]);
	}

	ofFile addonsmake(projectDir+"addons.make",ofFile::WriteOnly);
	for(int i=0;i<(int)addons.size();i++){
		addonsmake << addons[i].name << endl;
	}
}

string CBWinProject::getName(){
	return projectName;
}

string CBWinProject::getPath(){
	return projectDir;
}
