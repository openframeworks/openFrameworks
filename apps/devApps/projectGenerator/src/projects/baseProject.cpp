//
//  baseProject.cpp
//  projectGenerator
//
//  Created by molmol on 3/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "baseProject.h"




bool baseProject::create(string path){
    
    projectDir = ofFilePath::addTrailingSlash(path);
    projectName = ofFilePath::getFileName(path);
    bool bDoesDirExist = false;
    
    ofDirectory project(projectDir);    // this is a directory, really?
    if(project.exists()){
        bDoesDirExist = true;
    }
    
    if (!bDoesDirExist){
        ofDirectory project(projectDir);
        ofFile::copyFromTo(ofFilePath::join(templatePath,"src"),ofFilePath::join(projectDir,"src"));
        ofFile::copyFromTo(ofFilePath::join(templatePath,"bin"),ofFilePath::join(projectDir,"bin"));
    } 
    
    // if overwrite then ask for permission...
    
    createProjectFile();
    loadProjectFile();
    
    if (bDoesDirExist){
        
        vector < string > fileNames;
        getFilesRecursively(projectDir + "src", fileNames);
        
        for (int i = 0; i < fileNames.size(); i++){
            
            fileNames[i].erase(fileNames[i].begin(), fileNames[i].begin() + projectDir.length());
            
            string first, last;
            splitFromLast(fileNames[i], "/", first, last);  
            if (fileNames[i] != "src/testApp.cpp" &&
                fileNames[i] != "src/testApp.h" &&
                fileNames[i] != "src/main.cpp" && 
                fileNames[i] != "src/testApp.mm" &&
                fileNames[i] != "src/main.mm"){
                addSrc(fileNames[i], first);
            }
        }
    }
}