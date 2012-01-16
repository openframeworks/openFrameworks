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

# xcode osx
sourceXcconfig = "example-empty/Project.xcconfig"
sourcePlist = "example-empty/openFrameworks-Info.plist"
sourcePbxproj = "example-empty/EmptyExample.xcodeproj/project.pbxproj"

examples = glob.glob("example*")
for example in examples:
	if example != "example-empty":
		sansExample = re.sub("^example", "", example)
		upper = re.sub("-([a-z])", toUpper, sansExample)
		targetProjectName = upper + "Example"
		
		# windows code::blocks
		targetCbp = "{0}/{1}.cbp".format(example, targetProjectName)
		targetWorkspace = "{0}/{1}.workspace".format(example, targetProjectName)
		shutil.copy(sourceCbp, targetCbp)
		shutil.copy(sourceWorkspace, targetWorkspace)
		replaceInFile(targetCbp, sourceProjectName, targetProjectName)
		replaceInFile(targetWorkspace, sourceProjectName, targetProjectName)

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