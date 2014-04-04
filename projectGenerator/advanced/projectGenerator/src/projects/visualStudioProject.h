
#pragma once

#include "ofConstants.h"
#include "pugixml.hpp"
#include "ofAddon.h"
#include "baseProject.h"

class visualStudioProject : public baseProject {

public:

    void setup(string ofRoot= "../../../");

    void setup();

    bool createProjectFile();
    bool loadProjectFile();
    bool saveProjectFile();

    void addSrc(string srcFile, string folder);
    void addInclude(string includeName);
    void addLibrary(string libraryName, LibType libType);

    void addAddon(ofAddon & addon);

	static string LOG_NAME;

	pugi::xml_document filterXmlDoc;


	void appendFilter(string folderName);

private:

};
