import sys
import amara
from ofxDescriptor import ofxDescriptor

class ofProject:
    def __init__(self,name):
        self.doc=amara.parse(name)
        
                
    compilerOptions = property(
        fget = lambda self: self.doc.CodeBlocks_project_file.Project.Compiler.Add
    )
    linkerOptions = property(
        fget = lambda self: self.doc.CodeBlocks_project_file.Project.Linker.Add
    )
    
    def _get_folders(self):
        virtualFolders = 0
        for option in self.doc.CodeBlocks_project_file.Project.Option:
            if hasattr(option,"virtualFolders"):
                virtualFolders=option.virtualFolders
                break;
        virtualFolders = str(virtualFolders).split(";")
        return virtualFolders
    
    def _get_virtualFolders(self):
        virtualFolders = 0
        for option in self.doc.CodeBlocks_project_file.Project.Option:
            if hasattr(option,"virtualFolders"):
                virtualFolders=option.virtualFolders
        return virtualFolders
    
    def _set_virtualFolders(self,value):
        virtualFolders = 0
        for option in self.doc.CodeBlocks_project_file.Project.Option:
            if hasattr(option,"virtualFolders"):
                option.virtualFolders=value
    
    folders = property(
        fget = _get_folders
    )
    
    virtualFolders = property(
        fget = _get_virtualFolders,
        fset = _set_virtualFolders
    )
    
    def addIncludePath(self,path):
        for includePath in self.compilerOptions:
            if(hasattr(includePath,"directory")):
                if(str(includePath.directory)==str(path)):
                    return
        self.doc.CodeBlocks_project_file.Project.Compiler.xml_append(self.doc.xml_create_element(u'Add', attributes={u'directory': unicode(path)}))
        
    def addLibrary(self,library):
        if library=="":
            return
        for libraryFile in self.linkerOptions:
            if(hasattr(libraryFile,"library")):
                if(str(libraryFile.library)==str(library)):
                    return
        self.doc.CodeBlocks_project_file.Project.Linker.xml_append(self.doc.xml_create_element(u'Add', attributes={u'library': unicode(library)}))
        
    def addUnit(self,file,folder):
        for unit in self.doc.CodeBlocks_project_file.Project.Unit:
            if unit.filename==file:
                return
        unit = self.doc.CodeBlocks_project_file.Project.xml_append(self.doc.xml_create_element(u'Unit', attributes={u'filename': unicode(file)}))
        unit.xml_append(self.doc.xml_create_element(u'Option',attributes={u'virtualFolder': unicode(folder)}))
                        
    def addFolder(self,folder):
        folderExists=False
        for virtualFolder in self.folders:
            if(folder.name==virtualFolder):
                folderExists = True
                break
        if(folderExists!=True):
            self.virtualFolders=self.virtualFolders + ";" + unicode(folder.name)
            
        for file in folder.file:
            self.addUnit(file,folder.name)
        #self.doc.CodeBlocks_project_file.Project.Linker.xml_append(self.doc.xml_create_element(u'Add', attributes={u'library': unicode(library)}))
        
        
    def addAddon(self,addon):
        for include in addon.includePaths:
            self.addIncludePath(include)
        for library in addon.libraries:
            self.addLibrary(library)
        for folder in addon.folders:
            self.addFolder(folder)
        
    def xml(self,out=0):
        if(out==0):
            self.doc.xml()
        else:
            self.doc.xml(out)
        
#project = ofProject("oscSenderExample.cbp")
#addon = ofxDescriptor("install.xml")
#project.addAddon(addon)

#project.xml(sys.stdout)





#import sys
#import xml.dom.minidom
#import xml.dom.expatbuilder

"""class OFProject:
    #Abstraction for an openframeworks
    #codeblocks project file
    
    def __init__(self,name):
        self.name=name
       
        self.doc = xml.dom.minidom.parse(name)
        
        for cbpProject in self.doc.childNodes:
            for firstLevel in cbpProject.childNodes:
                if firstLevel.nodeType == firstLevel.ELEMENT_NODE and firstLevel.localName == "Project":
                    for projectSettings in firstLevel.childNodes:
                        if projectSettings.nodeType == projectSettings.ELEMENT_NODE:
                            print projectSettings.localName"""

#project = OFProject("oscSenderExample.cbp")
