/*
 * CBLinuxProject.cpp
 *
 *  Created on: 28/12/2011
 *      Author: arturo
 */

#include "CBLinuxProject.h"
#include "ofFileUtils.h"
#include "ofLog.h"
#include "Utils.h"

string CBLinuxProject::LOG_NAME = "CBLinuxProject";







void CBLinuxProject::setup() {
	if(target=="linux")
		templatePath = ofFilePath::join(getOFRoot(),"scripts/linux/template/linux");
	else
		templatePath = ofFilePath::join(getOFRoot(),"scripts/linux/template/linux64");
}

bool CBLinuxProject::createProjectFile(){
    ofFile project(projectDir + projectName + ".cbp");
    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample_linux.cbp"),project.path());
    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample_linux.workspace"),ofFilePath::join(projectDir, projectName + ".workspace"));
    ofFile::copyFromTo(ofFilePath::join(templatePath,"Makefile"),ofFilePath::join(projectDir,"Makefile"));
    ofFile config(ofFilePath::join(projectDir,"config.make"));
    if(!config.exists()) ofFile::copyFromTo(ofFilePath::join(templatePath,"config.make"),config.path());
    return true;
}
bool CBLinuxProject::loadProjectFile(){
    
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


bool CBLinuxProject::saveProjectFile(){
    
   findandreplaceInTexfile(ofFilePath::join(projectDir , projectName + ".workspace"),"emptyExample",projectName);
   pugi::xpath_node_set title = doc.select_nodes("//Option[@title]");
   if(!title.empty()){
        if(!title[0].node().attribute("title").set_value(projectName.c_str())){
            ofLogError(LOG_NAME) << "can't set title";
        }
    }
    doc.save_file((projectDir + projectName + ".cbp").c_str());
    
    //let's do some renaming: 
    string relRoot = getOFRelPath(ofFilePath::removeTrailingSlash(projectDir));
    
    if (relRoot != "../../../"){
        string relPath2 = relRoot;
        relPath2.erase(relPath2.end()-1);
        findandreplaceInTexfile(projectDir + "config.make", "../../..", relPath2);
        findandreplaceInTexfile(ofFilePath::join(projectDir , projectName + ".workspace"), "../../../", relRoot);
        findandreplaceInTexfile(ofFilePath::join(projectDir , projectName + ".cbp"), "../../../", relRoot);
    }
    
    
}





void CBLinuxProject::addSrc(string srcName, string folder){
	pugi::xml_node node = appendValue(doc, "Unit", "filename", srcName);
	if(!node.empty()){
		node.child("Option").attribute("virtualFolder").set_value(folder.c_str());
	}
    
}

void CBLinuxProject::addInclude(string includeName){
    //appendValue(doc, "Add", "directory", includeName);
}

void CBLinuxProject::addLibrary(string libraryName){
    //appendValue(doc, "Add", "library", libraryName);
}



string CBLinuxProject::getName(){
	return projectName;
}

string CBLinuxProject::getPath(){
	return projectDir;
}
