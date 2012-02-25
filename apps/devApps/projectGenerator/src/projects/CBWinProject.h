/*
 * CBLinuxProject.h
 *
 *  Created on: 28/12/2011
 *      Author: arturo
 */

#ifndef CBWINPROJECT_H_
#define CBWINPROJECT_H_

#include "ofConstants.h"
#include "pugixml.hpp"
#include "ofAddon.h"
#include "baseProject.h"

class CBWinProject: public baseProject  {
public:
	
    
    //
    void setup();
    
    CBWinProject();

	bool load(string path);
	bool create(string path);
	bool save(string path){return true;}

	void addSrc(string srcName, string folder);
	void addInclude(string includeName);
	void addLibrary(string libraryName);

	void addAddon(ofAddon & addon);

	string getName();
	string getPath();

	static string LOG_NAME;

private:
	
    
	pugi::xml_document doc;
	string projectDir, projectName;
	vector<ofAddon> addons;
};

#endif /* CBLINUXPROJECT_H_ */
