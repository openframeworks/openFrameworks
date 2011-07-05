#!/usr/bin/python
import os
from lxml import etree
from lxml import objectify
import argparse
import shutil
import glob


of_root = os.path.realpath(__file__)[0:-(len(os.path.join('scripts','linux','createProject.py'))+2)]
platform = 'win_cb'
arch = 'win_cb'
templates_path = os.path.join(of_root,'apps','devApps','win_cb')
template = {'cbp': os.path.join(templates_path , 'emptyExample_' + arch + '.cbp'), 'full_cbp': os.path.join(templates_path , 'emptyExample_' + arch + '.cbp'), 'workspace': os.path.join(templates_path , 'emptyExample_' + arch + '.workspace'), 'makefile': templates_path + 'Makefile', 'config.make': templates_path + 'config.make'}
fullCBP = True

def addCBPIncludePath(project,dirpath):
    found=False
    if project.find('Compiler') == None:
        etree.SubElement(project,"Compiler")
    if project.Compiler.find('Add') != None:
        for include in project.Compiler.Add:
            if str(include.get("directory"))==str(dirpath):
                found=True
                break
    if not found:
        include = etree.SubElement(project.Compiler,"Add")
        include.set("directory",dirpath)
        
def addCBPLibrary(project,libpath):
    found=False
    if project.find('Linker') == None:
        etree.SubElement(project,"Linker")
    if project.Linker.find('Add') != None:
        for lib in project.Linker.Add:
            if str(lib.get("option"))==str(libpath):
                found=True
                break
    if not found:
        include = etree.SubElement(project.Linker,"Add")
        include.set("option",libpath)
        
def addCBPUnit(project,filepath,basefolder):
    found=False
    for unit in project.Unit:
        if str(unit.get("filename"))==str(filepath):
            found=True
            break
    if not found:
        unit = etree.SubElement(project,"Unit")
        unit.set("filename",filepath)
        virtual_folder = etree.SubElement(unit,"Option")
        virtual_folder.set("virtualFolder",basefolder)

def addAddon(project,addon):
    if addon == '':
        return
    if not os.path.exists(os.path.join(of_root,'addons',addon)):
        print 'error', addon, 'in addons.make not found'
        return
    if not os.path.exists(os.path.join(of_root,'addons',addon,'src')):
        print 'error', addon, 'has no src folder'
        return
    if fullCBP:
        addon_src = os.path.join('..','..','..','addons',addon,'src')
        addCBPIncludePath(project,addon_src)
    for root, dirs, files in os.walk(os.path.join(of_root,'addons',addon,'src')):
        for name in files:
            basefolder = root[len(of_root):]
            filepath = str(os.path.join('..','..','..',basefolder,name))
            addCBPUnit(project,filepath,basefolder)
        if fullCBP:
            for dir in dirs:
                basefolder = root[len(of_root):]
                dirpath = os.path.join('..','..','..',basefolder,dir)
                addCBPIncludePath(project,dirpath)
    
    if fullCBP:
        if not os.path.exists(os.path.join(of_root,'addons',addon,'libs')):
            return
            
        dirpath = os.path.join(of_root,'addons',addon,'libs')
        addCBPIncludePath(project,os.path.join('..','..','..',basefolder))
        for root, dirs, files in os.walk(dirpath):
            for dir in dirs:
                basefolder_addon = root[len(of_root):]
                dirpath_addon = os.path.join('..','..','..',basefolder_addon,dir)
                addCBPIncludePath(project,dirpath_addon)
                
        for libdir in os.listdir(os.path.join(of_root,'addons',addon,'libs')):
            if not os.path.isdir(os.path.join(of_root,'addons',addon,'libs',libdir)):
                continue
            basefolder = os.path.join('addons',addon,'libs',libdir);
            if os.path.exists(os.path.join(of_root,basefolder,'include')):
                dirpath = os.path.join(of_root,basefolder,'include')
                addCBPIncludePath(project,os.path.join('..','..','..',basefolder,'include'))
                for root, dirs, files in os.walk(dirpath):
                    for dir in dirs:
                        basefolder_addon = root[len(of_root):]
                        dirpath_addon = os.path.join('..','..','..',basefolder_addon,dir)
                        addCBPIncludePath(project,dirpath_addon)
            
            basefolder = os.path.join('addons',addon,'libs',libdir);
            if os.path.exists(os.path.join(of_root,basefolder,'lib',arch)):
                dirpath = os.path.join('..','..','..',basefolder,'lib',arch)
                if os.path.exists(os.path.join(of_root,basefolder,'lib',arch,'libsorder.make')):
                    libsorder = open(os.path.join(of_root,basefolder,'lib',arch,'libsorder.make'))
                    for lib in libsorder:
                        if lib[-1]=='\n':
                            lib = lib[:-1]
                        addCBPLibrary(project,os.path.join(dirpath,'lib'+lib.strip()+'.a'))
                    libsorder.close()
                else:
                    for lib in glob.glob(os.path.join(of_root,basefolder,'lib',arch,'*.a')):
                        baselib = lib[len(of_root):]
                        addCBPLibrary(project,os.path.join('..','..','..',baselib))
                    for lib in glob.glob(os.path.join(of_root,basefolder,'lib',arch,'*.dll')):
                        baselib = lib[len(of_root):]
                        addCBPLibrary(project,os.path.join('..','..','..',baselib))
                        

def addAddons(project,project_path):
    if not os.path.exists(os.path.join(project_path,'addons.make')):
        return
    addons_make = open(os.path.join(project_path,'addons.make'),'r')
    for addon in addons_make:
        if addon[-1]=='\n':
            addon = addon[:-1]
        addAddon(project, addon)

def createCBP(project_path):
    if os.path.abspath(project_path) == os.path.abspath(templates_path):
        return
    project_name = os.path.basename(project_path)
    cbp = objectify.parse(os.path.join(project_path,project_name+'.cbp'))
    root = cbp.getroot()
    project = root.Project
    
    for option in project.Option:
        if option.get("title")!=None:
            option.set("title",project_name)
    
    # add existing files in src/ to the codeblocks project
    for root, dirs, files in os.walk(os.path.join(project_path,'src')):
        for name in files:
            basefolder = root[len(project_path)+1:]
            filepath = str(os.path.join(basefolder,name))
            addCBPUnit(project,filepath,basefolder)
    
    # add addons from addons.make to the cbp
    addAddons(project,project_path)

    cbp_file = open(os.path.join(project_path,project_name+'.cbp'),mode='w')
    cbp_file.write(etree.tostring(cbp, xml_declaration=True, encoding='UTF-8', pretty_print=True))
    cbp_file.close()
    
def createWorkspace(project_path):
    if os.path.abspath(project_path) == os.path.abspath(templates_path):
        return
    project_name = os.path.basename(project_path)
    ws = objectify.parse(os.path.join(project_path,project_name+'.workspace'))
    root = ws.getroot()
    workspace = root.Workspace
    
    for project in workspace.Project:
        if project.get("filename")=="emptyExample.cbp":
            project.set("filename",project_name+".cbp")

    ws_file = open(os.path.join(project_path,project_name+'.workspace'),mode='w')
    ws_file.write(etree.tostring(ws, xml_declaration=True, encoding='UTF-8', pretty_print=True))
    ws_file.close()

def createProject(project_path):
    print 'generating',project_path
    if os.path.abspath(project_path) == os.path.abspath(templates_path):
        return
    if project_path[-1]==os.sep:
        project_path=project_path[:-1]
    if not os.path.exists(project_path):
        os.mkdir(project_path)
    
    project_name = os.path.basename(project_path)
    if fullCBP:
        shutil.copyfile(template['full_cbp'],os.path.join(project_path,project_name+'.cbp'))
    else:
        shutil.copyfile(template['cbp'],os.path.join(project_path,project_name+'.cbp'))
        
    shutil.copyfile(template['workspace'],os.path.join(project_path,project_name+'.workspace'))
    
    if platform == "linux":
        shutil.copyfile(template['makefile'],os.path.join(project_path,'Makefile'))

    if platform == "linux" and not os.path.exists(os.path.join(project_path, 'config.make')):
        shutil.copyfile(template['config.make'],os.path.join(project_path,'config.make'))

    if not os.path.exists(os.path.join(project_path,'src')):
        os.mkdir(os.path.join(project_path , 'src'))
        for file in os.listdir(os.path.join(templates_path , 'src')):
            shutil.copyfile(os.path.join(templates_path , 'src' , file), os.path.join(project_path , 'src' , file))

    if not os.path.exists(os.path.join(project_path , 'bin')):
        os.mkdir(os.path.join(project_path , 'bin'))

    if not os.path.exists(os.path.join(project_path , 'bin', 'data')):
        os.mkdir(os.path.join(project_path , 'bin','data'))

    createCBP(project_path)
    createWorkspace(project_path)



parser = argparse.ArgumentParser(description='OF linux project generator')
parser.add_argument('project_path', metavar='project_path', nargs='?')
#parser.add_argument('-n', '--not_mk', dest='not_mk', action='store_const',
#        default=False, const=True, help='create cbp not dependent on Makefile')

project_path = parser.parse_args().project_path
fullCBP = True #parser.parse_args().not_mk

if project_path==None: #parse all examples
    for example in os.listdir(os.path.join(of_root,'apps','examples')):
        createProject(os.path.join(of_root,'apps','examples',example))
    for example in os.listdir(os.path.join(of_root,'apps','addonsExamples')):
        createProject(os.path.join(of_root,'apps','addonsExamples',example))
else:
    createProject(project_path)
    
