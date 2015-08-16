
#ifndef VSWINPROJECT_H_
#define VSWINPROJECT_H_

#include "ofConstants.h"
#include "ofAddon.h"
#include "baseProject.h"

class visualStudioProject : public baseProject {

public:

    void setup(std::string ofRoot= "../../../");

    void setup();

    bool createProjectFile();
    bool loadProjectFile();
    bool saveProjectFile();

    void addSrc(std::string srcFile, string folder, SrcType type=DEFAULT);
    void addInclude(std::string includeName);
    void addLibrary(std::string libraryName, LibType libType);
    void addCFLAG(std::string cflag, LibType libType = RELEASE_LIB); // C
    void addCPPFLAG(std::string cppflag, LibType libType = RELEASE_LIB); // C++

    void addAddon(ofAddon & addon);

	static std::string LOG_NAME;

	pugi::xml_document filterXmlDoc;


	void appendFilter(std::string folderName);

private:

};

#endif
