#!/bin/python

import glob, re, shutil, fileinput, os

def toUpper(pattern):
	return pattern.group(1).upper()
	
def replaceInFile(filename, pattern, replacement):
	for line in fileinput.FileInput(filename, inplace=1):
		print re.sub(pattern, replacement, line),

sourceProjectName = "EmptyExample"

# windows code::blocks
sourceCbp = "example-empty/EmptyExample.cbp"
sourceWorkspace = "example-empty/EmptyExample.workspace"

# windows vs2010
sourceSln = "example-empty/EmptyExample.sln"
sourceVcxproj = "example-empty/EmptyExample.vcxproj"
sourceVcxprojFilters = "example-empty/EmptyExample.vcxproj.filters"
sourceVcxprojUser = "example-empty/EmptyExample.vcxproj.user"

# xcode osx
sourceXcconfig = "example-empty/Project.xcconfig"
sourcePlist = "example-empty/openFrameworks-Info.plist"
sourcePbxproj = "example-empty/EmptyExample.xcodeproj/project.pbxproj"

# linux
sourceMakefile = "example-empty/Makefile"
sourceConfigs = "example-empty/config.make"
sourceAddons = "example-empty/addons.make"

examples = glob.glob("example*")
for example in examples:
	if example != "example-empty":
		sansExample = re.sub("^example", "", example)
		upper = re.sub("-([a-z])", toUpper, sansExample)
		targetProjectName = upper + "Example"
		
		#linux
		targetDir = "{0}/".format(example, targetProjectName)
		emptyDir = "example-empty"
		shutil.copy(sourceMakefile, targetDir)
		shutil.copy(sourceConfigs, targetDir)
		shutil.copy(sourceAddons, targetDir)

		# windows code::blocks
		targetCbp = "{0}/{1}.cbp".format(example, targetProjectName)
		targetWorkspace = "{0}/{1}.workspace".format(example, targetProjectName)
		shutil.copy(sourceCbp, targetCbp)
		shutil.copy(sourceWorkspace, targetWorkspace)
		replaceInFile(targetCbp, sourceProjectName, targetProjectName)
		replaceInFile(targetWorkspace, sourceProjectName, targetProjectName)

		# windows vs2010
		targetSln = "{0}/{1}.sln".format(example, targetProjectName)
		targetVcxproj = "{0}/{1}.vcxproj".format(example, targetProjectName)
		targetVcxprojFilters = "{0}/{1}.vcxproj.filters".format(example, targetProjectName)
		targetVcxprojUser = "{0}/{1}.vcxproj.user".format(example, targetProjectName)
		shutil.copy(sourceSln, targetSln)
		shutil.copy(sourceVcxproj, targetVcxproj)
		shutil.copy(sourceVcxprojFilters, targetVcxprojFilters)
		shutil.copy(sourceVcxprojUser, targetVcxprojUser)
		replaceInFile(targetSln, sourceProjectName, targetProjectName)
		replaceInFile(targetVcxproj, sourceProjectName, targetProjectName)

		# xcode osx
		targetXcodeproj = "{0}/{1}.xcodeproj".format(example, targetProjectName)
		targetPbxproj = "{0}/project.pbxproj".format(targetXcodeproj)
		shutil.copy(sourceXcconfig, example)
		shutil.copy(sourcePlist, example)
		if not os.path.exists(targetXcodeproj):
			os.mkdir(targetXcodeproj)
		shutil.copy(sourcePbxproj, targetPbxproj)
		replaceInFile(targetPbxproj, sourceProjectName, targetProjectName)
		
		print "Copied into " + targetProjectName + "."
