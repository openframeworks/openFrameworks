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
            
            // TODO: this can fail, if your OF directory is in some bad place like: 
            // /home/src/openframeworks/....
            // where it would pick up in the word src first. 
            // so let's make this much smarter!
            
            string first, last;
            splitFromFirst(fileNames[i],projectName, first, last);  
            // last is now something like " /src/main.cpp"
            // drop the trailing slash;
            last.erase(last.begin());
            string fileName = last;
            splitFromLast(fileName, "/", first, last);  
            if (fileName != "src/testApp.cpp" &&
                fileName != "src/testApp.h" &&
                fileName != "src/main.cpp" && 
                fileName != "src/testApp.mm" &&
                fileName != "src/main.mm"){
                addSrc(fileName, first);
            }
        }
    }
}