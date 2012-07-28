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
#include "CBWinProject.h"

class CBLinuxProject: public CBWinProject {
public:

	enum Arch{
		Linux,
		Linux64
	};

    void setup();

    bool createProjectFile();
    void addInclude(string includeName){};
    void addLibrary(string libraryName, LibType libType = RELEASE_LIB){};

	static string LOG_NAME;

private:

	Arch arch;
};

#endif /* CBLINUXPROJECT_H_ */
