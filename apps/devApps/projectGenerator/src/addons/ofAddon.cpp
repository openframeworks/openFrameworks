/*
 * ofAddon.cpp
 *
 *  Created on: 28/12/2011
 *      Author: arturo
 */

#include "ofAddon.h"
#include "pugixml.hpp"
#include "ofUtils.h"
#include "ofFileUtils.h"
#include "Utils.h"
#include "Poco/String.h"
#include "Poco/RegularExpression.h"
#include <list>

ofAddon::ofAddon(){
    pathToOF = "../../../";
    currentParseState = Unknown;
}

ofAddon::ConfigParseState ofAddon::stateFromString(string name){
	if(name=="meta") return Meta;
	if(name=="common") return Common;
	if(name=="linux64") return Linux64;
	if(name=="linux") return Linux;
	if(name=="win_cb") return WinCB;
	if(name=="vs") return VS;
	if(name=="linuxarmv6l") return LinuxARMv6;
	if(name=="linuxarmv7l") return LinuxARMv7;
	if(name=="android/armeabi") return AndroidARMv5;
	if(name=="android/armeabi-v7a") return AndroidARMv7;
	if(name=="ios") return iOS;
	if(name=="osx") return OSX;
	return Unknown;
}

string ofAddon::stateName(ofAddon::ConfigParseState state){
	switch(state){
	case Meta:
		return "meta";
	case Common:
		return "common";
	case Linux:
		return "linux";
	case Linux64:
		return "linux64";
	case WinCB:
		return "win_cb";
	case VS:
		return "vs";
	case LinuxARMv6:
		return "linuxarmv6";
	case LinuxARMv7:
		return "linuxarmv7";
	case AndroidARMv5:
		return "android/armeabi";
	case AndroidARMv7:
		return "android/armeabi-v7a";
	case iOS:
		return "ios";
	case OSX:
		return "osx";
	case Unknown:
	default:
		return "unknown";
	}

}

bool ofAddon::checkCorrectPlatform(ConfigParseState state){
	switch(state){
	case Meta:
		return true;
	case Common:
		return true;
	case Linux:
	case Linux64:
	case WinCB:
	case VS:
	case LinuxARMv6:
	case LinuxARMv7:
	case AndroidARMv5:
	case AndroidARMv7:
	case iOS:
	case OSX:
		return platform==stateName(state);
	case Unknown:
	default:
		return false;
	}
}


bool ofAddon::checkCorrectVariable(string variable, ConfigParseState state){
	switch(state){
	case Meta:
		return (variable=="ADDON_NAME" || variable=="ADDON_DESCRIPTION" || variable=="ADDON_AUTHOR" || variable=="ADDON_TAGS" || variable=="ADDON_URL");
	case Common:
	case Linux:
	case Linux64:
	case WinCB:
	case VS:
	case LinuxARMv6:
	case LinuxARMv7:
	case AndroidARMv5:
	case AndroidARMv7:
	case iOS:
	case OSX:
		return (variable == "ADDON_DEPENDENCIES" || variable == "ADDON_INCLUDES" || variable == "ADDON_CFLAGS" || variable == "ADDON_LDFLAGS"  || variable == "ADDON_LIBS" || variable == "ADDON_PKG_CONFIG_LIBRARIES" ||
				variable == "ADDON_FRAMEWORKS" || variable == "ADDON_SOURCES" || variable == "ADDON_DATA" || variable == "ADDON_LIBS_EXCLUDE" || variable == "ADDON_SOURCES_EXCLUDE" || variable == "ADDON_INCLUDES_EXCLUDE");
	case Unknown:
	default:
		return false;
	}
}

void ofAddon::addReplaceString(string & variable, string value, bool addToVariable){
	if(addToVariable) variable += value;
	else variable = value;
}

void ofAddon::addReplaceStringVector(vector<string> & variable, string value, string prefix, bool addToVariable){
	vector<string> values;
	if(value.find("\"")!=string::npos){
		values = ofSplitString(value,"\"",true,true);
	}else{
		values = ofSplitString(value," ",true,true);
	}

	if(!addToVariable) variable.clear();
	for(int i=0;i<(int)values.size();i++){
		if(values[i]!="") variable.push_back(ofFilePath::join(prefix,values[i]));
	}
}

void ofAddon::parseVariableValue(string variable, string value, bool addToValue, string line, int lineNum){
	if(variable == "ADDON_NAME"){
		if(value!=name){
			ofLogError() << "Error parsing " << name << " addon_config.mk" << "\n\t\t"
						<< "line " << lineNum << ": " << line << "\n\t\t"
						<< "addon name in filesystem " << name << " doesn't match with addon_config.mk " << value;
		}
		return;
	}

	string addonRelPath = ofFilePath::addTrailingSlash(pathToOF) + "addons/" + name;

	if(variable == "ADDON_DESCRIPTION"){
		addReplaceString(description,value,addToValue);
		return;
	}

	if(variable == "ADDON_AUTHOR"){
		addReplaceString(author,value,addToValue);
		return;
	}

	if(variable == "ADDON_TAGS"){
		addReplaceStringVector(tags,value,"",addToValue);
		return;
	}

	if(variable == "ADDON_URL"){
		addReplaceString(url,value,addToValue);
		return;
	}

	if(variable == "ADDON_DEPENDENCIES"){
		addReplaceStringVector(dependencies,value,"",addToValue);
	}

	if(variable == "ADDON_INCLUDES"){
		addReplaceStringVector(includePaths,value,addonRelPath,addToValue);
	}

	if(variable == "ADDON_CFLAGS"){
		addReplaceStringVector(cflags,value,"",addToValue);
	}

	if(variable == "ADDON_LDFLAGS"){
		addReplaceStringVector(ldflags,value,"",addToValue);
	}

	if(variable == "ADDON_LIBS"){
		addReplaceStringVector(libs,value,addonRelPath,addToValue);
	}

	if(variable == "ADDON_PKG_CONFIG_LIBRARIES"){
		addReplaceStringVector(pkgConfigLibs,value,"",addToValue);
	}

	if(variable == "ADDON_FRAMEWORKS"){
		addReplaceStringVector(frameworks,value,"",addToValue);
	}

	if(variable == "ADDON_SOURCES"){
		addReplaceStringVector(srcFiles,value,addonRelPath,addToValue);
	}

	if(variable == "ADDON_DATA"){
		addReplaceStringVector(data,value,addonRelPath,addToValue);
	}

	if(variable == "ADDON_LIBS_EXCLUDE"){
		addReplaceStringVector(excludeLibs,value,"",addToValue);
	}

	if(variable == "ADDON_SOURCES_EXCLUDE"){
		addReplaceStringVector(excludeSources,value,"",addToValue);
	}

	if(variable == "ADDON_INCLUDES_EXCLUDE"){
		addReplaceStringVector(excludeIncludes,value,"",addToValue);
	}
}

void ofAddon::exclude(vector<string> & variable, vector<string> exclusions){
	for(int i=0;i<(int)exclusions.size();i++){
		string exclusion = exclusions[i];
		//ofStringReplace(exclusion,"/","\\/");
		ofStringReplace(exclusion,".","\\.");
		ofStringReplace(exclusion,"%",".*");
		exclusion =".*"+ exclusion;
		Poco::RegularExpression regExp(exclusion);
		for(int j=0;j<(int)variable.size();j++){
			cout << "checking " << variable[j] << endl;
			if(regExp.match(variable[j])){
				variable.erase(variable.begin()+j);
				j--;
			}
		}
	}
}

void ofAddon::parseConfig(){
	ofFile addonConfig(ofFilePath::join(addonPath,"addon_config.mk"));

	cout << "parse config " << addonPath << endl;

	if(!addonConfig.exists()) return;

	string line, originalLine;
	int lineNum = 0;
	while(addonConfig.good()){
		lineNum++;
		getline(addonConfig,originalLine);
		line = originalLine;
		ofStringReplace(line,"\r","");
		ofStringReplace(line,"\n","");
		Poco::trimInPlace(line);

		// discard comments
		if(!line[0] || line[0]=='#'){
			continue;
		}

		// found section?
		if(line[line.size()-1]==':'){
			ofStringReplace(line,":","");
			currentParseState = stateFromString(line);
			if(currentParseState == Unknown){
				ofLogError() << "Error parsing " << name << " addon_config.mk" << "\n\t\t"
								<< "line " << lineNum << ": " << originalLine << "\n\t\t"
								<< "sectionName " << stateName(currentParseState) << " not recognized";
			}
			continue;
		}

		// found Variable
		if(line.find("=")!=string::npos){
			bool addToValue = false;
			string variable, value;
			vector<string> varValue;
			if(line.find("+=")!=string::npos){
				addToValue = true;
				varValue = ofSplitString(line,"+=");
			}else{
				addToValue = false;
				varValue = ofSplitString(line,"=");
			}
			variable = Poco::trim(varValue[0]);
			value = Poco::trim(varValue[1]);

			if(!checkCorrectPlatform(currentParseState)){
				continue;
			}

			if(!checkCorrectVariable(variable,currentParseState)){
				ofLogError() << "Error parsing " << name << " addon_config.mk" << "\n\t\t"
								<< "line " << lineNum << ": " << originalLine << "\n\t\t"
								<< "variable " << variable << " not recognized for section " << stateName(currentParseState);
				continue;
			}
			parseVariableValue(variable, value, addToValue, originalLine, lineNum);
		}
	}

	exclude(includePaths,excludeIncludes);
	exclude(srcFiles,excludeSources);
	exclude(libs,excludeLibs);
}

void ofAddon::fromFS(string path, string platform){

    
    
    clear();
    this->platform = platform;
	name = ofFilePath::getFileName(path);
	addonPath = ofFilePath::join(getAddonsRoot(),name);

    string filePath = path + "/src";
    string ofRootPath = ofFilePath::addTrailingSlash(getOFRoot()); //we need to add a trailing slash for the erase to work properly

    ofLogVerbose() << "in fromFS, trying src " << filePath;


	ofSetLogLevel(OF_LOG_NOTICE);
    getFilesRecursively(filePath, srcFiles);
	//ofSetLogLevel(OF_LOG_VERBOSE);

    for(int i=0;i<(int)srcFiles.size();i++){
    	srcFiles[i].erase (srcFiles[i].begin(), srcFiles[i].begin()+ofRootPath.length());
		//ofLogVerbose() << " srcFiles " << srcFiles[i];
    	int init = 0;
#ifdef TARGET_WIN32
    	int end = srcFiles[i].rfind("\\");
#else
        int end = srcFiles[i].rfind("/");
#endif
    	string folder = srcFiles[i].substr(init,end);
    	srcFiles[i] = pathToOF + srcFiles[i];
    	filesToFolders[srcFiles[i]] = folder;
    }

    string libsPath = path + "/libs";
    vector < string > libFiles;


	//ofSetLogLevel(OF_LOG_NOTICE);
    if (ofDirectory::doesDirectoryExist(libsPath)){
        getLibsRecursively(libsPath, libFiles, libs, platform);
    }
    //ofSetLogLevel(OF_LOG_VERBOSE);


    // I need to add libFiles to srcFiles
    for (int i = 0; i < (int)libFiles.size(); i++){
    	libFiles[i].erase (libFiles[i].begin(), libFiles[i].begin()+ofRootPath.length());
		//ofLogVerbose() << " libFiles " << libFiles[i];
    	int init = 0;
#ifdef TARGET_WIN32
    	int end = libFiles[i].rfind("\\");
#else
        int end = libFiles[i].rfind("/");
#endif
        if (end > 0){
            string folder = libFiles[i].substr(init,end);
            libFiles[i] = pathToOF + libFiles[i];
            srcFiles.push_back(libFiles[i]);
            filesToFolders[libFiles[i]] = folder;
        }

    }

    for (int i = 0; i < (int)libs.size(); i++){

        // does libs[] have any path ? let's fix if so.
#ifdef TARGET_WIN32
    	int end = libs[i].rfind("\\");
#else
        int end = libs[i].rfind("/");
#endif
        if (end > 0){

            libs[i].erase (libs[i].begin(), libs[i].begin()+ofRootPath.length());
            libs[i] = pathToOF + libs[i];
        }

    }

    // get a unique list of the paths that are needed for the includes.
    list < string > paths;
    for (int i = 0; i < (int)srcFiles.size(); i++){
        size_t found;
#ifdef TARGET_WIN32
    	found = srcFiles[i].find_last_of("\\");
#else
        found = srcFiles[i].find_last_of("/");
#endif
        paths.push_back(srcFiles[i].substr(0,found));
    }

    // get every folder in addon/src and addon/libs

    vector < string > libFolders;
    ofLogVerbose() << "trying get folders recursively " << (path + "/libs");

	// the dirList verbosity is crazy, so I'm setting this off for now.
	//ofSetLogLevel(OF_LOG_NOTICE);
    getFoldersRecursively(path + "/libs", libFolders, platform);
    vector < string > srcFolders;
    getFoldersRecursively(path + "/src", srcFolders, platform);
	//ofSetLogLevel(OF_LOG_VERBOSE);

    for (int i = 0; i < (int)libFolders.size(); i++){
        libFolders[i].erase (libFolders[i].begin(), libFolders[i].begin()+ofRootPath.length());
        libFolders[i] = pathToOF + libFolders[i];
        paths.push_back(libFolders[i]);
    }

    for (int i = 0; i < (int)srcFolders.size(); i++){
        srcFolders[i].erase (srcFolders[i].begin(), srcFolders[i].begin()+ofRootPath.length());
        srcFolders[i] = pathToOF + srcFolders[i];
        paths.push_back(srcFolders[i]);
    }

    paths.sort();
    paths.unique();
    for (list<string>::iterator it=paths.begin(); it!=paths.end(); ++it){
        includePaths.push_back(*it);
    }

    parseConfig();

}

void ofAddon::fromXML(string installXmlName){
	clear();
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(ofToDataPath(installXmlName).c_str());

    // this is src to add:
    pugi::xpath_node_set add = doc.select_nodes("//add/src/folder/file");
    for (pugi::xpath_node_set::const_iterator it = add.begin(); it != add.end(); ++it){
        pugi::xpath_node node = *it;
        //std::cout << "folder name "  << node.node().parent().attribute("name").value() << " : ";
        //std::cout << "src: " << node.node().child_value() << endl;
    }


    add = doc.select_nodes("//include/path");
    for (pugi::xpath_node_set::const_iterator it = add.begin(); it != add.end(); ++it){
        pugi::xpath_node node = *it;
        //std::cout << "include: " << node.node().child_value() << endl;
    }


    add = doc.select_nodes("//link/lib[@compiler='codeblocks']");
    // this has to be smarter I guess...
    for (pugi::xpath_node_set::const_iterator it = add.begin(); it != add.end(); ++it){
        pugi::xpath_node node = *it;
        //std::cout << "link: " << node.node().child_value() << endl;
    }


}


void ofAddon::clear(){
    filesToFolders.clear();
    srcFiles.clear();
    libs.clear();
    includePaths.clear();
    name.clear();
}
