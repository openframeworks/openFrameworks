

#pragma once

#include "baseProject.h"

class xcodeProject : public baseProject {

public:

    xcodeProject(){};

    void setup();

private:

    bool createProjectFile();
    bool loadProjectFile();
    bool saveProjectFile();
    void saveMakefile();

public:

    void addSrc(std::string srcFile, std::string folder, SrcType type=DEFAULT);
    void addInclude(std::string includeName);
    void addLibrary(std::string libraryName, LibType libType = RELEASE_LIB);
    void addLDFLAG(std::string ldflag, LibType libType = RELEASE_LIB);
    void addCFLAG(std::string cflag, LibType libType = RELEASE_LIB); // Other C Flags
    void addCPPFLAG(std::string cppflag, LibType libType = RELEASE_LIB); // Other C++ Flags
    
    // specific to OSX
    void addFramework(std::string name, std::string path);
    
        
    
    void addAddon(ofAddon & addon);

    void saveWorkspaceXML();
    void saveScheme();
    void renameProject();

    std::string srcUUID;
    std::string addonUUID;
    std::string resourcesUUID;
    std::string buildPhaseUUID;
    std::string frameworksUUID;
    std::string buildPhaseResourcesUUID;
    std::string frameworksBuildPhaseUUID;
    

    pugi::xml_node findOrMakeFolderSet( pugi::xml_node nodeToAddTo, std::vector < std::string > & folders, std::string pathForHash);
	pugi::xml_node insertPoint;         // where are we inserting items (at the second dict tag,
                                        // /plist[1]/dict[1]/dict[2])
    bool findArrayForUUID(std::string UUID, pugi::xml_node & nodeMe);

};




