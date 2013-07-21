

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

    void addSrc(string srcFile, string folder);
    void addInclude(string includeName);
    void addLibrary(string libraryName, LibType libType = RELEASE_LIB);

    void addAddon(ofAddon & addon);

    void saveWorkspaceXML();
    void saveScheme();
    void renameProject();

	string srcUUID;
	string addonUUID;
	string resourcesUUID;
	string buildPhaseUUID;
    string buildPhaseResourcesUUID; 

    pugi::xml_node findOrMakeFolderSet( pugi::xml_node nodeToAddTo, vector < string > & folders, string pathForHash);
	pugi::xml_node insertPoint;         // where are we inserting items (at the second dict tag,
                                        // /plist[1]/dict[1]/dict[2])
    bool findArrayForUUID(string UUID, pugi::xml_node & nodeMe);

};




