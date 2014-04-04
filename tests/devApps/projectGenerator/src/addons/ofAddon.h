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
    
	void fromFS(string path, string platform);
	void fromXML(string installXmlName);
	void clear();

    // this is source files:
    map < string, string > filesToFolders;      //the addons has had, for each file,
                                                //sometimes a listing of what folder to put it in, such as "addons/ofxOsc/src"

    vector < string > srcFiles;
    vector < string > libs;
    vector < string > includePaths;
    
    // From addon_config.mk
    vector < string > dependencies;
    vector < string > cflags;
    vector < string > ldflags;
    vector < string > pkgConfigLibs; 	// linux only
    vector < string > frameworks;		// osx only
    vector < string > data;

    // metadata
    string name;
    string description;
    string author;
    vector<string> tags;
    string url;
    
    
    string pathToOF;

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
    void parseVariableValue(string variable, string value, bool addToValue, string line, int lineNum);
    void addReplaceString(string & variable, string value, bool addToVariable);
    void addReplaceStringVector(vector<string> & variable, string value, string prefix, bool addToVariable);
    void exclude(vector<string> & variable, vector<string> exclusions);
    ConfigParseState stateFromString(string name);
    string stateName(ConfigParseState state);
    bool checkCorrectVariable(string variable, ConfigParseState state);
    bool checkCorrectPlatform(ConfigParseState state);

    string platform;

    vector<string> excludeLibs;
    vector<string> excludeSources;
    vector<string> excludeIncludes;

    string addonPath;
};

#endif /* OFADDON_H_ */
