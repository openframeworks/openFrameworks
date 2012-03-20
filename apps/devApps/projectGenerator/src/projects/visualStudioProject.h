
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

		
	static string LOG_NAME;

};
