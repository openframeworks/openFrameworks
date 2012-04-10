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
	templatePath = ofFilePath::join(getOFRoot(),"scripts/linux/template/"+target);
}

bool CBLinuxProject::createProjectFile(){
	ofDirectory dir(projectDir);
	if(!dir.exists()) dir.create(true);

    ofFile project(ofFilePath::join(projectDir, projectName + ".cbp"));
    string src =  ofFilePath::join(templatePath,"emptyExample_" + target + ".cbp");
    string dst = project.path();
    bool ret;

    if(!project.exists()){
		ret = ofFile::copyFromTo(src,dst);
		if(!ret){
			ofLogError(LOG_NAME) << "error copying cbp template from " << src << " to " << dst;
			return false;
		}else{
			findandreplaceInTexfile(dst, "emptyExample", projectName);
		}
    }

    ofFile workspace(ofFilePath::join(projectDir, projectName + ".workspace"));
    if(!workspace.exists()){
		src = ofFilePath::join(templatePath,"emptyExample_" + target + ".workspace");
		dst = workspace.path();
		ret = ofFile::copyFromTo(src,dst);
		if(!ret){
			ofLogError(LOG_NAME) << "error copying workspace template from "<< src << " to " << dst;
			return false;
		}else{
			findandreplaceInTexfile(dst, "emptyExample", projectName);
		}
    }

    ofFile makefile(ofFilePath::join(projectDir,"Makefile"));
    if(!makefile.exists()){
		src = ofFilePath::join(templatePath,"Makefile");
		dst = makefile.path();
		ret = ofFile::copyFromTo(src,dst);
		if(!ret){
			ofLogError(LOG_NAME) << "error copying Makefile template from " << src << " to " << dst;
			return false;
		}
    }

    ofFile config(ofFilePath::join(projectDir,"config.make"));
    if(!config.exists()){
    	src = ofFilePath::join(templatePath,"config.make");
    	dst = config.path();
    	ret = ofFile::copyFromTo(src,dst);
    	if(!ret){
    		ofLogError(LOG_NAME) << "error copying config.make template from " << src << " to " << dst;
    		return false;
    	}
    }


    // handle the relative roots.
    string relRoot = getOFRelPath(ofFilePath::removeTrailingSlash(projectDir));
    if (relRoot != "../../../"){
        string relPath2 = relRoot;
        relPath2.erase(relPath2.end()-1);
        findandreplaceInTexfile(projectDir + "config.make", "../../..", relPath2);
        findandreplaceInTexfile(ofFilePath::join(projectDir , projectName + ".workspace"), "../../../", relRoot);
        findandreplaceInTexfile(ofFilePath::join(projectDir , projectName + ".cbp"), "../../../", relRoot);
    }

    return true;
}

bool CBLinuxProject::loadProjectFile(){

    //project.open(ofFilePath::join(projectDir , projectName + ".cbp"));

    ofFile project(ofFilePath::join(projectDir , projectName + ".cbp"));
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
    return doc.save_file((projectDir + projectName + ".cbp").c_str());

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

void CBLinuxProject::addLibrary(string libraryName, LibType libType){
    //appendValue(doc, "Add", "library", libraryName);
}

void CBLinuxProject::addAddon(ofAddon & addon){
    for(int i=0;i<(int)addons.size();i++){
		if(addons[i].name==addon.name) return;
	}

	addons.push_back(addon);

    for(int i=0;i<(int)addon.includePaths.size();i++){
        ofLogVerbose() << "adding addon include path: " << addon.includePaths[i];
        addInclude(addon.includePaths[i]);
    }
    for(int i=0;i<(int)addon.libs.size();i++){
        ofLogVerbose() << "adding addon libs: " << addon.libs[i];
        addLibrary(addon.libs[i]);
    }
    for(int i=0;i<(int)addon.srcFiles.size(); i++){
        ofLogVerbose() << "adding addon srcFiles: " << addon.srcFiles[i];
        addSrc(addon.srcFiles[i],addon.filesToFolders[addon.srcFiles[i]]);
    }
}

string CBLinuxProject::getName(){
	return projectName;
}

string CBLinuxProject::getPath(){
	return projectDir;
}
