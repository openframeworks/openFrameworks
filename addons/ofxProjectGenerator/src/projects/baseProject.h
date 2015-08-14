

#pragma once

#include <set>

#include "ofAddon.h"
#include "Utils.h"
#include "ofConstants.h"

class baseProject {

public:

    enum LibType{
        DEBUG_LIB = 0,
        RELEASE_LIB
    };

    enum SrcType{
    	DEFAULT,
    	HEADER,
    	CPP,
    	C,
    	OBJC
    };

    baseProject(){
        bLoaded = false;
    };

    virtual ~baseProject(){};

    void setup(string _target);

    bool create(string path);
    bool save(bool createMakeFile);

    // this shouldn't be called by anyone.  call "create(...), save" etc
private:

    virtual void setup()=0;
    virtual bool createProjectFile()=0;
    virtual bool loadProjectFile()=0;
    virtual bool saveProjectFile()=0;

    // virtual void renameProject();
    // this should get called at the end.

public:

    virtual void addSrc(std::string srcFile, string folder, SrcType type=DEFAULT) = 0;
    virtual void addInclude(std::string includeName) = 0;
    virtual void addLibrary(std::string libraryName, LibType libType = RELEASE_LIB) = 0;
    virtual void addLDFLAG(std::string ldflag, LibType libType = RELEASE_LIB){}
    virtual void addCFLAG(std::string cflag, LibType libType = RELEASE_LIB){}; // C_FLAGS
    virtual void addCPPFLAG(std::string cppflag, LibType libType = RELEASE_LIB){}; // CXX_FLAGS

	virtual void addAddon(ofAddon & addon);

	std::string getName() { return projectName;};
	std::string getPath() { return projectDir; };

    pugi::xml_document doc;
    bool bLoaded;

    std::string projectDir;
    std::string projectName;
    std::string templatePath;
    std::string target;

private:

    void parseAddons();

protected:

    vector<ofAddon> addons;
};


