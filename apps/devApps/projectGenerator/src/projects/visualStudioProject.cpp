


#include "visualStudioProject.h"
#include "Utils.h"

string visualStudioProject::LOG_NAME = "visualStudioProjectFile";



void visualStudioProject::setup() {

}

bool visualStudioProject::createProjectFile(){
    
    ofFile project(projectDir + projectName + ".vcxproj");
    ofFile user(projectDir + projectName + ".vcxproj.user");
    ofFile solution(projectDir + projectName + ".sln");
    
    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample_vs2010.vcxproj"),project.path(),false, true);
    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample_vs2010.vcxproj.user"),user.path(), false, true);
    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample_vs2010.sln"),solution.path(), false, true);

    findandreplaceInTexfile(solution.path(),"emptyExample_vs2010",projectName);
    findandreplaceInTexfile(project.path(),"emptyExample",projectName);
    
    string relRoot = getOFRelPath(ofFilePath::removeTrailingSlash(projectDir));
    if (relRoot != "../../../"){
        
        string relRootWindows = relRoot;
        // let's make it windows friendly:
        for(int i = 0; i < relRootWindows.length(); i++) { 
            if( relRootWindows[i] == '/' ) 
                relRootWindows[i] = '\\'; 
        } 
        
        // sln has windows paths: 
        findandreplaceInTexfile(solution.path(), "..\\..\\..\\", relRootWindows);
        
        // vcx has unixy paths: 
        //..\..\..\libs
        findandreplaceInTexfile(project.path(), "../../../", relRoot);
    }

    return true;
}

    
bool visualStudioProject::loadProjectFile(){
    
    ofFile project(projectDir + projectName + ".vcxproj");
	if(!project.exists()){
		ofLogError(LOG_NAME) << "error loading" << project.path() << "doesn't exist";
		return false;
	}
	pugi::xml_parse_result result = doc.load(project);
    bLoaded = result.status==pugi::status_ok;
    return bLoaded;
}


bool visualStudioProject::saveProjectFile(){    
    return doc.save_file((projectDir + projectName + ".vcxproj").c_str());
}

void visualStudioProject::addSrc(string srcFile, string folder){
    
    // TODO: no folder love here...
    
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
