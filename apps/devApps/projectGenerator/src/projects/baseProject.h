

#pragma once

#include <set>

#include "ofMain.h"
#include "pugixml.hpp"
#include "ofAddon.h"
#include "Utils.h"

class baseProject {
    
public: 
    
    baseProject(){
        bLoaded = false;
    };
    
    virtual ~baseProject(){};
    
    void setup(string _target);
    
    bool create(string path);
    bool save();
    
    // this shouldn't be called by anyone.  call "create(...), save" etc
private:
    
    virtual void setup()=0;
    virtual bool createProjectFile()=0;
    virtual bool loadProjectFile()=0;
    virtual bool saveProjectFile()=0;
    
    // virtual void renameProject();
    // this should get called at the end. 

public: 
    
    virtual void addSrc(string srcFile, string folder) = 0;
    virtual void addInclude(string includeName) = 0;
    virtual void addLibrary(string libraryName) = 0;

	void addAddon(ofAddon & addon);

    string getName() { return projectName;};
	string getPath() { return projectDir; };

    pugi::xml_document doc;
    bool bLoaded;
    
    string projectDir;
    string projectName;
    string templatePath;
    string target;

private:
    
    set<ofAddon> addons;
    void parseAddons();
};


