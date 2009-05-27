#!/usr/bin/python
from ofProject import ofProject
from ofxDescriptor import ofxDescriptor
import os
import shutil
import sys
from ofEnvironment import of_root

class ofProjectManager:
    """class to add addons to projects"""
    
    def __init__(self,project):
        try:
            f = open(of_root + "apps/myApps/" + project + "/" + project + ".cbp","r")
            f.close()
        except IOError:
            self.createFromEmpty(project)
            
        self.projectName = project
        self.project = ofProject(of_root + "apps/myApps/" + project + "/" + project + ".cbp")
        
    def createFromEmpty(self,project):
        shutil.copytree (of_root + "apps/examples/emptyExample",of_root + "apps/myApps/" + project)
        shutil.move(of_root + "apps/myApps/"+project+"/emptyExample.cbp", of_root +"apps/myApps/" + project+ "/" + project + ".cbp")
        f = open(of_root + "apps/myApps/" + project + "/" + project + ".cbp","r")
        projectStr = f.read()
        f.close()
        projectStr = projectStr.replace("emptyExample",project)
        f = open(of_root + "apps/myApps/" + project + "/" + project + ".cbp","w")
        f.write(projectStr)
        
    def addAddon(self,addon):
        addon = ofxDescriptor(of_root + "addons/" + addon + "/install.xml")
        self.project.addAddon(addon)
        
    def saveProject(self):
        f = open(of_root + "apps/myApps/" + self.projectName + "/"+self.projectName+".cbp","w")
        self.project.xml(f);
        f.close()


#if executed from command line
if __name__ == "__main__":
    if(len(sys.argv) > 2):
        if sys.argv[1] == "add":       
            if(len(sys.argv) > 3):
                projectManager = ofProjectManager(sys.argv[2])
                projectManager.addAddon(sys.argv[3])
                projectManager.saveProject()
        
        if sys.argv[1] == "create":
                projectManager = ofProjectManager(sys.argv[2])
                projectManager.saveProject()
            
    else:
        print """Usage: ofProjectManager [command] [project] [addon]
        commands: create
                  add
        """

