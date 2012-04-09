

#pragma once


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
    
    void setup(string _target){
        target = _target;
        templatePath = ofFilePath::join(getOFRoot(),"scripts/" + target + "/template/");
        setup(); // call the inherited class setup(), now that target is set. 
    }
    
    bool create(string path);
    bool save(){
        return saveProjectFile();
    }
    
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

	void addAddon(ofAddon & addon){
        for(int i=0;i<(int)addon.includePaths.size();i++){
            addInclude(addon.includePaths[i]);
        }
        for(int i=0;i<(int)addon.libs.size();i++){
            addLibrary(addon.libs[i]);
        }
        for(int i=0;i< addon.srcFiles.size(); i++){
            addSrc(addon.srcFiles[i],addon.filesToFolders[addon.srcFiles[i]]);
        }
    }

    string getName() { return projectName;};
	string getPath() { return projectDir; };

    pugi::xml_document doc;
    bool bLoaded;
    
    string projectDir;
    string projectName;
    string templatePath;
    string target;
    
};


