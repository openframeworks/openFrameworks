/*
 * CBLinuxProject.h
 *
 *  Created on: 28/12/2011
 *      Author: arturo
 */

#pragma once

#include "ofConstants.h"
#include "pugixml.hpp"
#include "ofAddon.h"
#include "baseProject.h"

class CBWinProject: virtual public baseProject  {
public:

    void setup();

    bool createProjectFile();
    bool loadProjectFile();
    bool saveProjectFile();

	void addSrc(string srcName, string folder);
	void addInclude(string includeName);
	void addLibrary(string libraryName, LibType libType = RELEASE_LIB);

	string getName();
	string getPath();

	static string LOG_NAME;

private:

};
