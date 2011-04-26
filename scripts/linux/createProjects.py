#!/usr/bin/python
import os
from lxml import etree
from lxml import objectify
import argparse
import shutil

of_root = '/home/arturo/Desktop/openFrameworks'
platform = 'linux'
arch = 'linux64'
templates_path = '../../apps/examples/emptyExample/'
template = {'cbp': templates_path + 'emptyExample_' + arch + '.cbp', 'makefile': templates_path + 'Makefile', 'config.make': templates_path + 'config.make'}

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
    for root, dirs, files in os.walk(os.path.join(of_root,'addons',addon,'src')):
        for name in files:
            basefolder = root[len(of_root)+1:]
            filepath = str(os.path.join('..','..','..',basefolder,name))
            print filepath 
            addCBPUnit(project,filepath,basefolder)

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
            print filepath 
            addCBPUnit(project,filepath,basefolder)
    
    # add addons from addons.make to the cbp
    addAddons(project,project_path)

    cbp_file = open(os.path.join(project_path,project_name+'.cbp'),mode='w')
    cbp_file.write(etree.tostring(cbp, xml_declaration=True, encoding='UTF-8', pretty_print=True))
    cbp_file.close()

def createProject(project_path):
    if os.path.abspath(project_path) == os.path.abspath(templates_path):
        return
    if project_path[-1]==os.sep:
        project_path=project_path[:-1]
    if not os.path.exists(project_path):
        os.mkdir(project_path)
    
    project_name = os.path.basename(project_path)
    shutil.copyfile(template['cbp'],os.path.join(project_path,project_name+'.cbp'))
    shutil.copyfile(template['makefile'],os.path.join(project_path,'Makefile'))

    if not os.path.exists(os.path.join(project_path, 'config.make')):
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



parser = argparse.ArgumentParser(description='OF linux project generator')
parser.add_argument('project_path', metavar='project_path', nargs='?')
project_path = parser.parse_args().project_path

if project_path==None: #parse all examples
    #for root, dirs, files in os.walk('../../apps/examples'):
    #    for name in dirs:
    #        print os.path.join(root,name)
    for example in os.listdir(os.path.join('..','..','apps','examples')):
        createProject(os.path.join('..','..','apps','examples',example))
    for example in os.listdir(os.path.join('..','..','apps','addonsExamples')):
        createProject(os.path.join('..','..','apps','addonsExamples',example))
else:
    createProject(project_path)
