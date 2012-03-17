
#pragma once

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
    void addLibrary(string libraryName);

	void addAddon(ofAddon & addon);

	string getName();
	string getPath();

	static string LOG_NAME;

private:
	string projectDir, projectName, ofRoot;
};
