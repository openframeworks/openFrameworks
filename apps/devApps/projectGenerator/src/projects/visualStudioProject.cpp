


#include "visualStudioProject.h"
#include "Utils.h"

string visualStudioProject::LOG_NAME = "visualStudioProjectFile";


void visualStudioProject::setup(){
    
}

bool visualStudioProject::load(string path){
	projectDir = ofFilePath::addLeadingSlash(path);
	projectName = ofFilePath::getFileName(path);
	ofFile project(projectDir + projectName + ".vcxproj");
	if(!project.exists()){
		ofLogError(LOG_NAME) << "error loading" << path << "doesn't exist";
		return false;
	}

	pugi::xml_parse_result result = doc.load(project);

    bLoaded = result.status==pugi::status_ok;
    return bLoaded;
}  


bool visualStudioProject::save(string fileName){
    return doc.save_file(ofToDataPath(fileName).c_str());
}  

bool visualStudioProject::create(string path){
	projectDir = ofFilePath::addTrailingSlash(path);
	ofLogVerbose(LOG_NAME) << "project dir:" << projectDir;
	projectName = ofFilePath::getFileName(path);
	ofLogVerbose(LOG_NAME) << "project name:" << projectName;
	ofFile project(projectDir + projectName + ".vcxproj");
	if(!project.exists()){
		ofLogVerbose(LOG_NAME) << "creating non existent project";
		ofDirectory dir(projectDir);
		dir.create(true);
		ofFile::copyFromTo(getOFRoot()+"/scripts/vs2010/template/emptyExample_vs2010.vcxproj",project.path());
		ofFile::copyFromTo(getOFRoot()+"/scripts/vs2010/template/emptyExample_vs2010.vcxproj.user",projectDir + projectName + ".vcxproj.user");
		ofFile::copyFromTo(getOFRoot()+"/scripts/vs2010/template/emptyExample_vs2010.sln",projectDir + projectName + ".sln");
		ofFile::copyFromTo(getOFRoot()+"/scripts/vs2010/template/src",projectDir);
		ofFile::copyFromTo(getOFRoot()+"/scripts/vs2010/template/bin",projectDir);
		project.open(projectDir + projectName + ".vcxproj");
	}

	pugi::xml_parse_result result = doc.load(project);
	if(result.status==pugi::status_ok){
		//TODO: change project name in xml and save
		//doc.save_file((projectDir + projectName + ".cbp").c_str());
		bLoaded = true;
	}else{
		bLoaded = false;
	}
	return bLoaded;
}

void visualStudioProject::addSrc(string srcFile, string folder){
    
    fixSlashOrder(srcFile);
    
    if (ofIsStringInString(srcFile, ".h") || ofIsStringInString(srcFile, ".hpp")){
        appendValue(doc, "ClInclude", "Include", srcFile);
    } else {
        appendValue(doc, "ClCompile", "Include", srcFile);
    }
    
} 

void visualStudioProject::addInclude(string includeName){
   
    
    fixSlashOrder(includeName);
    
    pugi::xpath_node_set source = doc.select_nodes("//ClCompile/AdditionalIncludeDirectories");
    for (pugi::xpath_node_set::const_iterator it = source.begin(); it != source.end(); ++it){
        //printf("here ?? \n");
        pugi::xpath_node node = *it;
        string includes = node.node().first_child().value();
        vector < string > strings = ofSplitString(includes, ";");
        bool bAdd = true;
        for (int i = 0; i < (int)strings.size(); i++){
            if (strings[i].compare(includeName) == 0){
                bAdd = false;
            }
        }
        if (bAdd == true){
            strings.push_back(includeName);
            string includesNew = unsplitString(strings, ";");
            node.node().first_child().set_value(includesNew.c_str());
        }
       
    }
    //appendValue(doc, "Add", "directory", includeName);
}  
void visualStudioProject::addLibrary(string libraryName){

    
    fixSlashOrder(libraryName);
    
    // ok first, split path and library name.  
    size_t found = libraryName.find_last_of("/");
    string libFolder = libraryName.substr(0,found);
    string libName = libraryName.substr(found+1);
    
    // do the path, then the library
    
    // paths for libraries
    pugi::xpath_node_set source = doc.select_nodes("//Link/AdditionalLibraryDirectories");
    for (pugi::xpath_node_set::const_iterator it = source.begin(); it != source.end(); ++it){
        pugi::xpath_node node = *it;
        string includes = node.node().first_child().value();
        vector < string > strings = ofSplitString(includes, ";");
        bool bAdd = true;
        for (int i = 0; i < (int)strings.size(); i++){
            if (strings[i].compare(libFolder) == 0){
                bAdd = false;
            }
        }
        if (bAdd == true){
            strings.push_back(libFolder);
            string libPathsNew = unsplitString(strings, ";");
            cout << libPathsNew << endl;
            node.node().first_child().set_value(libPathsNew.c_str());
        }
    }
    
    // libs
    source = doc.select_nodes("//Link/AdditionalDependencies");
    for (pugi::xpath_node_set::const_iterator it = source.begin(); it != source.end(); ++it){
        pugi::xpath_node node = *it;
        string includes = node.node().first_child().value();
        vector < string > strings = ofSplitString(includes, ";");
        bool bAdd = true;
        for (int i = 0; i < (int)strings.size(); i++){
            if (strings[i].compare(libName) == 0){
                bAdd = false;
            }
        }
        if (bAdd == true){
            strings.push_back(libName);
            string libsNew = unsplitString(strings, ";");
            cout << libsNew << endl;
            node.node().first_child().set_value(libsNew.c_str());
        }
       
    }


}


void visualStudioProject::addAddon(ofAddon & addon){
	for(int i=0;i<(int)addon.includePaths.size();i++){
		addInclude(addon.includePaths[i]);
	}
	for(int i=0;i<(int)addon.libs.size();i++){
		addLibrary(addon.libs[i]);
	}
	for(int i=0;i<(int)addon.srcFiles.size();i++){
		addSrc(addon.srcFiles[i],addon.filesToFolders[addon.srcFiles[i]]);
	}
}

string visualStudioProject::getName(){
	return projectName;
}

string visualStudioProject::getPath(){
	return projectDir;
}

