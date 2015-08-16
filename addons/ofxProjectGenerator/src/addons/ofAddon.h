/*
 * ofAddon.h
 *
 *  Created on: 28/12/2011
 *      Author: arturo
 */

#ifndef OFADDON_H_
#define OFADDON_H_

#include <map>
#include "ofConstants.h"

class ofAddon {

public:
	
    ofAddon();
    
	void fromFS(std::string path, std::string platform);
	void fromXML(std::string installXmlName);
	void clear();

    // this is source files:
	std::map < std::string, std::string > filesToFolders;      //the addons has had, for each file,
                                                //sometimes a listing of what folder to put it in, such as "addons/ofxOsc/src"

    std::vector < std::string > srcFiles;
    std::vector < std::string > csrcFiles;
    std::vector < std::string > cppsrcFiles;
    std::vector < std::string > headersrcFiles;
    std::vector < std::string > objcsrcFiles;
    std::vector < std::string > libs;
    std::vector < std::string > dllsToCopy;
    std::vector < std::string > includePaths;
    
    // From addon_config.mk
    std::vector < std::string > dependencies;
    std::vector < std::string > cflags;   // C_FLAGS
    std::vector < std::string > cppflags; // CXX_FLAGS
    std::vector < std::string > ldflags;
    std::vector < std::string > pkgConfigLibs; 	// linux only
    std::vector < std::string > frameworks;		// osx only
    std::vector < std::string > data;

    // metadata
    std::string name;
    std::string description;
    std::string author;
    std::vector<std::string> tags;
    std::string url;
    
    
    std::string pathToOF;

    bool operator <(const ofAddon & addon) const{
    	return addon.name < name;
    }

private:

    enum ConfigParseState{
    	Meta,
    	Common,
    	Linux,
    	Linux64,
    	WinCB,
    	VS,
    	LinuxARMv6,
    	LinuxARMv7,
    	AndroidARMv5,
    	AndroidARMv7,
    	iOS,
    	OSX,
    	Unknown
    } currentParseState;

    void parseConfig();
    void parseVariableValue(std::string variable, std::string value, bool addToValue, std::string line, int lineNum);
    void addReplaceString(std::string & variable, std::string value, bool addToVariable);
    void addReplaceStringVector(std::vector<std::string> & variable, std::string value, std::string prefix, bool addToVariable);
    void exclude(std::vector<std::string> & variable, std::vector<std::string> exclusions);
    ConfigParseState stateFromString(std::string name);
    std::string stateName(ConfigParseState state);
    bool checkCorrectVariable(std::string variable, ConfigParseState state);
    bool checkCorrectPlatform(ConfigParseState state);

    std::string platform;

    std::vector<std::string> excludeLibs;
    std::vector<std::string> excludeSources;
    std::vector<std::string> excludeIncludes;

    std::string addonPath;
};

#endif /* OFADDON_H_ */
