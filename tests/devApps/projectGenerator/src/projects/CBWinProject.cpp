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

    string project = projectDir + projectName + ".cbp";
    string workspace = projectDir + projectName + ".workspace";


	ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample.cbp"),project, false, true);

	ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample.workspace"),workspace, false, true);
	ofFile::copyFromTo(ofFilePath::join(templatePath,"icon.rc"), projectDir + "icon.rc", false, true);

    //let's do some renaming:
    string relRoot = getOFRelPath(ofFilePath::removeTrailingSlash(projectDir));

    if (relRoot != "../../../"){

        string relRootWindows = relRoot;
        // let's make it windows friendly:
        for(int i = 0; i < relRootWindows.length(); i++) {
            if( relRootWindows[i] == '/' )
                relRootWindows[i] = '\\';
        }

        findandreplaceInTexfile(workspace, "../../../", relRoot);
        findandreplaceInTexfile(project, "../../../", relRoot);

        findandreplaceInTexfile(workspace, "..\\..\\..\\", relRootWindows);
        findandreplaceInTexfile(project, "..\\..\\..\\", relRootWindows);
    }

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
    return doc.save_file((projectDir + projectName + ".cbp").c_str());
}

void CBWinProject::addSrc(string srcName, string folder){
	pugi::xml_node node = appendValue(doc, "Unit", "filename", srcName);
	if(!node.empty()){
		node.child("Option").attribute("virtualFolder").set_value(folder.c_str());
	}
    doc.save_file((projectDir + projectName + ".cbp").c_str());
}

void CBWinProject::addInclude(string includeName){
    ofLogNotice() << "adding include " << includeName;
    appendValue(doc, "Add", "directory", includeName);
}

void CBWinProject::addLibrary(string libraryName, LibType libType){
    ofLogNotice() << "adding library " << libraryName;
    appendValue(doc, "Add", "library", libraryName, true);
    // overwriteMultiple for a lib if it's there (so libsorder.make will work)
    // this is because we might need to say libosc, then ws2_32
}

string CBWinProject::getName(){
	return projectName;
}

string CBWinProject::getPath(){
	return projectDir;
}
