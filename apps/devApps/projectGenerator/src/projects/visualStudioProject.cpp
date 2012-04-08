


#include "visualStudioProject.h"
#include "Utils.h"

string visualStudioProject::LOG_NAME = "visualStudioProjectFile";

void visualStudioProject::setup() {
    ;
}

bool visualStudioProject::createProjectFile(){

    ofFile project(projectDir + projectName + ".vcxproj");
    ofFile user(projectDir + projectName + ".vcxproj.user");
    ofFile solution(projectDir + projectName + ".sln");

    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample_vs2010.vcxproj"),project.path(),false, true);
    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample_vs2010.vcxproj.user"),user.path(), false, true);
    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample_vs2010.sln"),solution.path(), false, true);

    findandreplaceInTexfile(solution.path(),"emptyExample_vs2010",projectName);
    findandreplaceInTexfile(user.path(),"emptyExample_vs2010",projectName);
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

void visualStudioProject::addLibrary(string libraryName, LibType libType){


    fixSlashOrder(libraryName);

    // ok first, split path and library name.
    size_t found = libraryName.find_last_of("\\");
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
    int platformCounter = 0;
    for (pugi::xpath_node_set::const_iterator it = source.begin(); it != source.end(); ++it){

        // still ghetto, but getting better
        // TODO: iterate by <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='BUILD TYPE|SOME PLATFORM'">
        // instead of making the weak assumption that VS projects do Debug|Win32 then Release|Win32
        if(libType != platformCounter) continue;

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
        platformCounter++;
    }

}

void visualStudioProject::addAddon(ofAddon & addon){
    for(int i=0;i<(int)addons.size();i++){
		if(addons[i].name==addon.name) return;
	}

	addons.push_back(addon);

    for(int i=0;i<(int)addon.includePaths.size();i++){
        ofLogVerbose() << "adding addon include path: " << addon.includePaths[i];
        addInclude(addon.includePaths[i]);
    }

    // divide libs into debug and release libs
    // the most reliable would be to have seperate
    // folders for debug and release libs
    // i'm gonna start with a ghetto approach of just
    // looking for duplicate names except for a d.lib
    // at the end -> this is not great as many
    // libs compile with the d somewhere in the middle of the name...

    vector <string> debugLibs;
    vector <string> releaseLibs;

    for(int i=0;i<(int)addon.libs.size();i++){

        // check that this isn't already a debug lib
        bool alreadyDone = false;
        for(int k=0;k<(int)debugLibs.size();k++){
            if(addon.libs[i] == debugLibs[k]){
                alreadyDone = true;
            }
        }
        if(alreadyDone) continue;

        size_t found = 0;
        // get the full lib name
        found = addon.libs[i].find_last_of("\\");
        string libName = addon.libs[i].substr(found+1);
        cout << libName << endl;
        // get the first part of a lib name ie., libodd.lib -> libodd OR liboddd.lib -> liboddd
        found = libName.find_last_of(".");
        string firstPart = libName.substr(0,found);
        cout << firstPart << endl;
        int debugLibIndex = -1; // assume there is no debug lib
        for(int j=0;j<(int)addon.libs.size();j++){
            if(addon.libs[i] == addon.libs[j]) continue; // don't do it to yourself
            if(ofIsStringInString(addon.libs[j], firstPart)){
                // assume that if the first part of a name is
                // in another name then we have a debug/release pair
                cout << "Found match" << addon.libs[i] << " " << addon.libs[j] << endl;
                debugLibIndex = j;
                break;
            }
        }
        if(debugLibIndex != -1){
            // add debug and release libs to appropriate vectors
            debugLibs.push_back(addon.libs[debugLibIndex]);
        }else{
            // there's only one lib (either debug or release) so add to both vectors and hope for the best ;-)
            debugLibs.push_back(addon.libs[i]);
        }
        releaseLibs.push_back(addon.libs[i]);
    }

    for(int i=0;i<(int)debugLibs.size();i++){
        ofLogVerbose() << "adding addon debug libs: " << debugLibs[i];
        addLibrary(debugLibs[i], DEBUG_LIB);
    }

    for(int i=0;i<(int)releaseLibs.size();i++){
        ofLogVerbose() << "adding addon release libs: " << releaseLibs[i];
        addLibrary(releaseLibs[i], RELEASE_LIB);
    }

    for(int i=0;i<(int)addon.srcFiles.size(); i++){
        ofLogVerbose() << "adding addon srcFiles: " << addon.srcFiles[i];
        addSrc(addon.srcFiles[i],addon.filesToFolders[addon.srcFiles[i]]);
    }
}
