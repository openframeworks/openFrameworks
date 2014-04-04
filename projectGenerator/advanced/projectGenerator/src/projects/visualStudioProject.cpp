


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

    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample.vcxproj"),project,false, true);
    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample.vcxproj.user"),user, false, true);
    ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample.sln"),solution, false, true);
	ofFile::copyFromTo(ofFilePath::join(templatePath,"emptyExample.vcxproj.filters"),filters, false, true);
	ofFile::copyFromTo(ofFilePath::join(templatePath,"icon.rc"), projectDir + "icon.rc", false, true);

	ofFile filterFile(filters);
	string temp = filterFile.readToBuffer();
	pugi::xml_parse_result result = filterXmlDoc.load(temp.c_str());
	if (result.status==pugi::status_ok) ofLogVerbose() << "loaded filter ";
	else ofLogVerbose() << "problem loading filter ";

    findandreplaceInTexfile(solution,"emptyExample",projectName);
    findandreplaceInTexfile(user,"emptyExample",projectName);
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
		ofLogError(LOG_NAME) << "error loading " << project.path() << " doesn't exist";
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


    fixSlashOrder(folderName);

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

void visualStudioProject::addSrc(string srcFile, string folder, SrcType type){

    fixSlashOrder(folder);
    fixSlashOrder(srcFile);

	vector < string > folderSubNames = ofSplitString(folder, "\\");
	string folderName = "";
	for (int i = 0; i < folderSubNames.size(); i++){
		if (i != 0) folderName += "\\";
		folderName += folderSubNames[i];
		appendFilter(folderName);
	}

	if(type==DEFAULT){
		if (ofIsStringInString(srcFile, ".h") || ofIsStringInString(srcFile, ".hpp")){
			appendValue(doc, "ClInclude", "Include", srcFile);

			pugi::xml_node node = filterXmlDoc.select_single_node("//ItemGroup[ClInclude]").node();
			pugi::xml_node nodeAdded = node.append_child("ClInclude");
			nodeAdded.append_attribute("Include").set_value(srcFile.c_str());
			nodeAdded.append_child("Filter").append_child(pugi::node_pcdata).set_value(folder.c_str());

		} else {
			appendValue(doc, "ClCompile", "Include", srcFile);

			pugi::xml_node nodeFilters = filterXmlDoc.select_single_node("//ItemGroup[ClCompile]").node();
			pugi::xml_node nodeAdded = nodeFilters.append_child("ClCompile");
			nodeAdded.append_attribute("Include").set_value(srcFile.c_str());
			nodeAdded.append_child("Filter").append_child(pugi::node_pcdata).set_value(folder.c_str());
		}
	}else{
    	switch(type){
    	case CPP:{
			appendValue(doc, "ClCompile", "Include", srcFile);

			pugi::xml_node nodeFilters = filterXmlDoc.select_single_node("//ItemGroup[ClCompile]").node();
			pugi::xml_node nodeAdded = nodeFilters.append_child("ClCompile");
			nodeAdded.append_attribute("Include").set_value(srcFile.c_str());
			nodeAdded.append_child("Filter").append_child(pugi::node_pcdata).set_value(folder.c_str());
			break;
    	}
    	case C:{
			pugi::xml_node node = appendValue(doc, "ClCompile", "Include", srcFile);

			if(!node.child("CompileAs")){
				pugi::xml_node compileAs = node.append_child("CompileAs");
				compileAs.append_attribute("Condition").set_value("'$(Configuration)|$(Platform)'=='Debug|Win32'");
				compileAs.set_value("Default");

				compileAs = node.append_child("CompileAs");
				compileAs.append_attribute("Condition").set_value("'$(Configuration)|$(Platform)'=='Release|Win32'");
				compileAs.set_value("Default");
			}

			pugi::xml_node nodeFilters = filterXmlDoc.select_single_node("//ItemGroup[ClCompile]").node();
			pugi::xml_node nodeAdded = nodeFilters.append_child("ClCompile");
			nodeAdded.append_attribute("Include").set_value(srcFile.c_str());
			nodeAdded.append_child("Filter").append_child(pugi::node_pcdata).set_value(folder.c_str());
			break;
    	}
    	case HEADER:{
			appendValue(doc, "ClInclude", "Include", srcFile);

			pugi::xml_node node = filterXmlDoc.select_single_node("//ItemGroup[ClInclude]").node();
			pugi::xml_node nodeAdded = node.append_child("ClInclude");
			nodeAdded.append_attribute("Include").set_value(srcFile.c_str());
			nodeAdded.append_child("Filter").append_child(pugi::node_pcdata).set_value(folder.c_str());
			break;
    	}
    	case OBJC:{
    		ofLogError() << "objective c type not supported on vs for " << srcFile;
			break;
    	}
    	default:{
    		ofLogError() << "explicit source type " << type << " not supported yet on osx for " << srcFile;
    		break;
    	}
    	}
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

	cout << "adding library " << libType << "  " << libraryName << endl;
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

		if(libType != platformCounter){
			platformCounter++;
			continue;
		}

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

void visualStudioProject::addCFLAG(string cflag, LibType libType){
	pugi::xpath_node_set items = doc.select_nodes("//ItemDefinitionGroup");
	for(int i=0;i<items.size();i++){
		pugi::xml_node additionalOptions;
		bool found=false;
		if(libType==RELEASE_LIB && string(items[i].node().attribute("Condition").value())=="'$(Configuration)|$(Platform)'=='Release|Win32'"){
			additionalOptions = items[i].node().child("ClCompile").child("AdditionalOptions");
			found = true;
		}else if(libType==DEBUG_LIB && string(items[i].node().attribute("Condition").value())=="'$(Configuration)|$(Platform)'=='Debug|Win32'"){
			additionalOptions = items[i].node().child("ClCompile").child("AdditionalOptions");
			found = true;
		}
		if(!found) continue;
		if(!additionalOptions){
			items[i].node().child("ClCompile").append_child("AdditionalOptions").append_child(pugi::node_pcdata).set_value(cflag.c_str());
		}else{
			additionalOptions.set_value((string(additionalOptions.value()) + " " + cflag).c_str());
		}
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

    vector <string> possibleReleaseOrDebugOnlyLibs;

    for(int i = 0; i < addon.libs.size(); i++){

        size_t found = 0;

        // get the full lib name
#ifdef TARGET_WIN32
    	found = addon.libs[i].find_last_of("\\");
#else
        found = addon.libs[i].find_last_of("/");
#endif

        string libName = addon.libs[i].substr(found+1);
        // get the first part of a lib name ie., libodd.lib -> libodd OR liboddd.lib -> liboddd
        found = libName.find_last_of(".");
        string firstPart = libName.substr(0,found);

        // check this lib name against every other lib name
        for(int j = 0; j < addon.libs.size(); j++){
            // check if this lib name is contained within another lib name and is not the same name
            if(ofIsStringInString(addon.libs[j], firstPart) && addon.libs[i] != addon.libs[j]){
                // if it is then add respecitive libs to debug and release
                if(!isInVector(addon.libs[j], debugLibs)){
                    //cout << "adding to DEBUG " << addon.libs[j] << endl;
                    debugLibs.push_back(addon.libs[j]);
                }
                if(!isInVector(addon.libs[i], releaseLibs)){
                    //cout << "adding to RELEASE " << addon.libs[i] << endl;
                    releaseLibs.push_back(addon.libs[i]);
                }
                // stop searching
                break;
            }else{
                // if we only have a release or only have a debug lib
                // we'll want to add it to both releaseLibs and debugLibs
                // NB: all debug libs will get added to this vector,
                // but we catch that once all pairs have been added
                // since we cannot guarantee the order of parsing libs
                // although this is innefficient it fixes issues on linux
                if(!isInVector(addon.libs[i], possibleReleaseOrDebugOnlyLibs)){
                    possibleReleaseOrDebugOnlyLibs.push_back(addon.libs[i]);
                }
                // keep searching...
            }
        }
    }

    for(int i=0;i<(int)possibleReleaseOrDebugOnlyLibs.size();i++){
         if(!isInVector(possibleReleaseOrDebugOnlyLibs[i], debugLibs) && !isInVector(possibleReleaseOrDebugOnlyLibs[i], releaseLibs)){
            ofLogVerbose() << "RELEASE ONLY LIBS FOUND " << possibleReleaseOrDebugOnlyLibs[i] << endl;
            debugLibs.push_back(possibleReleaseOrDebugOnlyLibs[i]);
            releaseLibs.push_back(possibleReleaseOrDebugOnlyLibs[i]);
         }
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
		if(addon.filesToFolders[addon.srcFiles[i]]=="") addon.filesToFolders[addon.srcFiles[i]]="other";
        addSrc(addon.srcFiles[i],addon.filesToFolders[addon.srcFiles[i]]);
    }

    for(int i=0;i<(int)addon.csrcFiles.size(); i++){
        ofLogVerbose() << "adding addon c srcFiles: " << addon.csrcFiles[i];
		if(addon.filesToFolders[addon.csrcFiles[i]]=="") addon.filesToFolders[addon.csrcFiles[i]]="other";
        addSrc(addon.csrcFiles[i],addon.filesToFolders[addon.csrcFiles[i]],C);
    }

    for(int i=0;i<(int)addon.cppsrcFiles.size(); i++){
        ofLogVerbose() << "adding addon c srcFiles: " << addon.cppsrcFiles[i];
		if(addon.filesToFolders[addon.cppsrcFiles[i]]=="") addon.filesToFolders[addon.cppsrcFiles[i]]="other";
        addSrc(addon.cppsrcFiles[i],addon.filesToFolders[addon.cppsrcFiles[i]],C);
    }

    for(int i=0;i<(int)addon.headersrcFiles.size(); i++){
        ofLogVerbose() << "adding addon c srcFiles: " << addon.headersrcFiles[i];
		if(addon.filesToFolders[addon.headersrcFiles[i]]=="") addon.filesToFolders[addon.headersrcFiles[i]]="other";
        addSrc(addon.headersrcFiles[i],addon.filesToFolders[addon.headersrcFiles[i]],C);
    }

    for(int i=0;i<(int)addon.objcsrcFiles.size(); i++){
        ofLogVerbose() << "adding addon c srcFiles: " << addon.objcsrcFiles[i];
		if(addon.filesToFolders[addon.objcsrcFiles[i]]=="") addon.filesToFolders[addon.objcsrcFiles[i]]="other";
        addSrc(addon.objcsrcFiles[i],addon.filesToFolders[addon.objcsrcFiles[i]],C);
    }

	for(int i=0;i<(int)addon.dllsToCopy.size();i++){
		ofLogVerbose() << "adding addon dlls to bin: " << addon.dllsToCopy[i];
		string dll = ofFilePath::join("addons/" + addon.name, addon.dllsToCopy[i]);
		ofFile(ofFilePath::join(getOFRoot(),dll)).copyTo(ofFilePath::join(projectDir,"bin/"),false,true);
	}

	for(int i=0;i<(int)addon.cflags.size();i++){
		addCFLAG(addon.cflags[i],RELEASE_LIB);
		addCFLAG(addon.cflags[i],DEBUG_LIB);
	}
}
