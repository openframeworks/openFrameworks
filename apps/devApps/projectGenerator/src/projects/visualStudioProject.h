
#pragma once

#include "baseProject.h"

class visualStudioProject : public baseProject {
    
public: 
    
    void setup();
    
    bool load(string path);
    bool create(string path);
	bool save(string path);

    void addSrc(string srcFile, string folder);
    void addInclude(string includeName);
    void addLibrary(string libraryName);

	void addAddon(ofAddon & addon);

	string getName();
	string getPath();

	static string LOG_NAME;

private:
	string projectDir, projectName;
};
