


#include "visualStudioProject.h"
#include "Utils.h"

string visualStudioProject::LOG_NAME = "visualStudioProjectFile";

void visualStudioProject::setup() {
    ;
}

bool visualStudioProject::createProjectFile(){

    string project = ofFilePath::join(projectDir,projectName + ".vcxproj");
    string user = ofFilePath::join(projectDir,projectName + ".vcxproj.user");
    string solution = ofFilePath::join(projectDir,projectName + ".sln");
	string filters = ofFilePath::join(projectDir, projectName + ".vcxproj.filters");

    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample_vs2010.vcxproj"),project,false, true);
    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample_vs2010.vcxproj.user"),user, false, true);
    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample_vs2010.sln"),solution, false, true);
	ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample_vs2010.vcxproj.filters"),filters, false, true);

	ofFile filterFile(filters);
	string temp = filterFile.readToBuffer();
	pugi::xml_parse_result result = filterXmlDoc.load(temp.c_str());
	if (result.status==pugi::status_ok) ofLogVerbose() << "loaded filter ";
	else ofLogVerbose() << "problem loading filter ";
	
    findandreplaceInTexfile(solution,"emptyExample_vs2010",projectName);
    findandreplaceInTexfile(user,"emptyExample_vs2010",projectName);
    findandreplaceInTexfile(project,"emptyExample",projectName);

    string relRoot = getOFRelPath(ofFilePath::removeTrailingSlash(projectDir));
    if (relRoot != "../../../"){

        string relRootWindows = relRoot;
        // let's make it windows friendly:
        for(int i = 0; i < relRootWindows.length(); i++) {
            if( relRootWindows[i] == '/' )
                relRootWindows[i] = '\\';
        }

        // sln has windows paths:
        findandreplaceInTexfile(solution, "..\\..\\..\\", relRootWindows);

        // vcx has unixy paths:
        //..\..\..\libs
        findandreplaceInTexfile(project, "../../../", relRoot);
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

	string filters = projectDir + projectName + ".vcxproj.filters";
	filterXmlDoc.save_file(filters.c_str());


    return doc.save_file((projectDir + projectName + ".vcxproj").c_str());
}


void visualStudioProject::appendFilter(string folderName){

	string uuid = generateUUID(folderName);
	
	string tag = "//ItemGroup[Filter]/Filter[@Include=\"" + folderName + "\"]";
	 pugi::xpath_node_set set = filterXmlDoc.select_nodes(tag.c_str());
	 if (set.size() > 0){
		
		//pugi::xml_node node = set[0].node();
	 } else {

		
		 pugi::xml_node node = filterXmlDoc.select_single_node("//ItemGroup[Filter]/Filter").node().parent();
		 pugi::xml_node nodeAdded = node.append_child("Filter");
		 nodeAdded.append_attribute("Include").set_value(folderName.c_str());
		 pugi::xml_node nodeAdded2 = nodeAdded.append_child("UniqueIdentifier");

		 uuid.insert(8,"-");
		 uuid.insert(8+4+1,"-");
		 uuid.insert(8+4+4+2,"-");
		 uuid.insert(8+4+4+4+3,"-");
		 
		 //d8376475-7454-4a24-b08a-aac121d3ad6f

		 string uuidAltered = "{" + uuid + "}";
		 nodeAdded2.append_child(pugi::node_pcdata).set_value(uuidAltered.c_str());
	 }
}

void visualStudioProject::addSrc(string srcFile, string folder){


	vector < string > folderSubNames = ofSplitString(folder, "\\");
	string folderName = "";
	for (int i = 0; i < folderSubNames.size(); i++){
		if (i != 0) folderName += "\\";
		folderName += folderSubNames[i];
		appendFilter(folderName);
	}

    // TODO: no folder love here...

    fixSlashOrder(srcFile);

    if (ofIsStringInString(srcFile, ".h") || ofIsStringInString(srcFile, ".hpp")){
        appendValue(doc, "ClInclude", "Include", srcFile);

		pugi::xml_node node = filterXmlDoc.select_single_node("//ItemGroup[ClInclude]").node();
		pugi::xml_node nodeAdded = node.append_child("ClInclude");
		nodeAdded.append_attribute("Include").set_value(srcFile.c_str());
		nodeAdded.append_child("Filter").append_child(pugi::node_pcdata).set_value(folder.c_str());

    } else {
        appendValue(doc, "ClCompile", "Include", srcFile);

		pugi::xml_node node = filterXmlDoc.select_single_node("//ItemGroup[ClCompile]").node();
		pugi::xml_node nodeAdded = node.append_child("ClCompile");
		nodeAdded.append_attribute("Include").set_value(srcFile.c_str());
		nodeAdded.append_child("Filter").append_child(pugi::node_pcdata).set_value(folder.c_str());

    }

	
	
}

void visualStudioProject::addInclude(string includeName){


    fixSlashOrder(includeName);

    pugi::xpath_node_set source = doc.select_nodes("//ClCompile/AdditionalIncludeDirectories");
    for (pugi::xpath_node_set::const_iterator it = source.begin(); it != source.end(); ++it){
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
        // get the first part of a lib name ie., libodd.lib -> libodd OR liboddd.lib -> liboddd
        found = libName.find_last_of(".");
        string firstPart = libName.substr(0,found);
        int debugLibIndex = -1; // assume there is no debug lib
        for(int j=0;j<(int)addon.libs.size();j++){
            if(addon.libs[i] == addon.libs[j]) continue; // don't do it to yourself
            if(ofIsStringInString(addon.libs[j], firstPart)){
                // assume that if the first part of a name is
                // in another name then we have a debug/release pair
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
