/*
 * CBLinuxProject.h
 *
 *  Created on: 28/12/2011
 *      Author: arturo
 */

#ifndef CBLINUXPROJECT_H_
#define CBLINUXPROJECT_H_

#include "ofConstants.h"
#include "pugixml.hpp"
#include "ofAddon.h"
#include "baseProject.h"

class CBLinuxProject: public baseProject {
public:

	enum Arch{
		Linux,
		Linux64
	};

   
    void setup();

    bool createProjectFile();
    bool loadProjectFile();
    bool saveProjectFile();

    /*
	bool load(string path);
	bool create(string path);
	bool save(string path){return true;}
     */
    
	void addSrc(string srcName, string folder);
	void addInclude(string includeName);
	void addLibrary(string libraryName);

	string getName();
	string getPath();

	static string LOG_NAME;

private:
	
    //void parseAddons();

	pugi::xml_document doc;
	vector<ofAddon> addons;
	Arch arch;
};

#endif /* CBLINUXPROJECT_H_ */
