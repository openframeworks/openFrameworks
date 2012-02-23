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


void CBWinProject::setup(){
    
}

CBWinProject::CBWinProject() {

}

void CBWinProject::parseAddons(){
	addons.clear();
	ofFile addonsmake(projectDir+"addons.make");
	if(!addonsmake.exists()){
		addonsmake.create();
		addonsmake.open(projectDir+"addons.make");
	}
	ofBuffer addonsmakebuff;
	addonsmake >> addonsmakebuff;
	while(!addonsmakebuff.isLastLine()){
		string line = addonsmakebuff.getNextLine();
		if(line!=""){
			addons.push_back(ofAddon(getOFRoot()+"/addons/"+line,"win_cb"));
		}
	}
}


bool CBWinProject::load(string path){
	projectDir = ofFilePath::addLeadingSlash(path);
	projectName = ofFilePath::getFileName(path);
	ofFile project(projectDir + projectName + ".cbp");
	if(!project.exists()){
		ofLogError(LOG_NAME) << "error loading" << path << "doesn't exist";
		return false;
	}
	parseAddons();
	pugi::xml_parse_result result = doc.load(project);
	bLoaded = result.status==pugi::status_ok;
	return bLoaded;
}

bool CBWinProject::create(string path){
	projectDir = ofFilePath::addTrailingSlash(path);
	ofLogVerbose(LOG_NAME) << "project dir:" << projectDir;
	projectName = ofFilePath::getFileName(path);
	ofLogVerbose(LOG_NAME) << "project name:" << projectName;
	ofFile project(projectDir + projectName + ".cbp");
	if(!project.exists()){
		ofLogVerbose(LOG_NAME) << "creating non existent project";
		ofDirectory dir(projectDir);
		dir.create(true);
		ofFile::copyFromTo(getOFRoot()+"/scripts/win_cb/template/emptyExample_win_cb.cbp",project.path());
		ofFile::copyFromTo(getOFRoot()+"/scripts/win_cb/template/emptyExample_win_cb.workspace",projectDir + projectName + ".workspace");
		ofFile::copyFromTo(getOFRoot()+"/scripts/linux/template/src",projectDir);
		ofFile::copyFromTo(getOFRoot()+"/scripts/linux/template/bin",projectDir);
		project.open(projectDir + projectName + ".cbp");
	}
	parseAddons();
	pugi::xml_parse_result result = doc.load(project);
	if(result.status==pugi::status_ok){
        pugi::xpath_node_set title = doc.select_nodes("//Option[@title]");
        if(!title.empty()){
        	if(!title[0].node().attribute("title").set_value(projectName.c_str())){
        		ofLogError(LOG_NAME) << "cannot set title";
        		bLoaded = false;
        		return bLoaded;
        	}
            doc.save_file((projectDir + projectName + ".cbp").c_str());
        }
        bLoaded = true;
	}else{
		bLoaded = false;
	}
	return bLoaded;
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
