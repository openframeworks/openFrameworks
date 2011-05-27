#include "testApp.h"
#include "appleScript.h"

#include "Poco/HMACEngine.h"
#include "Poco/MD5Engine.h"

using Poco::DigestEngine;
using Poco::HMACEngine;
using Poco::MD5Engine;

ostringstream msgStr;
string appsPath = "../../../../";

//--------------------------------------------------------------
string getHash(string input){

		std::string passphrase("openFrameworks"); // HMAC needs a passphrase
		
		HMACEngine<MD5Engine> hmac(passphrase); // we'll compute a MD5 Hash
		hmac.update(input);
		
		const DigestEngine::Digest& digest = hmac.digest(); // finish HMAC computation and obtain digest
		std::string digestString(DigestEngine::digestToHex(digest)); // convert to a string of hexadecimal numbers
	
		return digestString;	
}

//--------------------------------------------------------------
void addFilesToProjectAS(string projectName, vector <string> filePath, vector <string> fileName, string group){

	string str;
	str = "tell application \"Xcode\" \n";
	str += "	tell group \""+group+"\" of project \""+projectName+"\"\n";
	//str += "	tell root group of project \""+projectName+"\"\n";
	for(int k = 0; k < filePath.size(); k++){
		
		string fileKind = "sourcecode.cpp.cpp";		
		if( ofFilePath::getFileExt(filePath[k]) == "h" ){
			fileKind = "sourcecode.cpp.h";
		}
		
		str += "		make new file reference with properties {file kind:\""+fileKind+"\", name:\""+fileName[k]+"\", path type:project relative, path:\""+filePath[k]+"\"}\n";
	}
	str += "	end tell\n";
	str += "end tell\n";
	
	cout << endl << str << endl;

	OSStatus err = SimpleRunAppleScript(str.c_str());	
	if( err == noErr ){
		cout << " added files! " << endl;
	}else{
		cout << " error " << endl;
	}
}


//--------------------------------------------------------------
void addFilesToProjectXML(string xcodePath, vector <string> filePath, vector <string> fileName, vector <string> hash, string groupId){

//		<key>E4EECEAB138EB72700A80321</key>
//		<dict>
//			<key>fileEncoding</key>
//			<string>4</string>
//			<key>isa</key>
//			<string>PBXFileReference</string>
//			<key>lastKnownFileType</key>
//			<string>sourcecode.c.h</string>
//			<key>name</key>
//			<string>threadedObject.h</string>
//			<key>path</key>
//			<string>src/threadedObject.h</string>
//			<key>sourceTree</key>
//			<string>SOURCE_ROOT</string>
//		</dict>

	ofxXmlSettings xml;
	if( xml.loadFile(xcodePath + "/project.pbxproj") ){
		if( xml.pushTag("plist") ){
			if( xml.pushTag("dict") ){
				if( xml.pushTag("dict", 1)  ){
				
					for(int k = 0; k < filePath.size(); k++){
					
						bool addToBuild = true;
						string fileKind = "sourcecode.cpp.cpp";		
						if( ofFilePath::getFileExt(filePath[k]) == "h" ){
							fileKind = "sourcecode.c.h";
							addToBuild = false;
						}
						
						//lets add our file hash to the group 									
						int numKeyTags = xml.getNumTags("key");
						for(int j = 0; j < numKeyTags; j++){
							if( xml.getValue("key", "null", j) == groupId ){
								msgStr << " --found group key! " << endl;
								
								if( xml.pushTag("dict", j) ){
									if( xml.getValue("key", "null") == "children" ){
										if( !xml.tagExists("array") ){
											xml.addTag("array");
										}
										if( xml.pushTag("array") ){
											msgStr << " --added hash to group! " << endl;
					
											xml.addValue("string", hash[k]);
											xml.popTag();
										}
									}
									xml.popTag();
								}
								
								break;
							}
						}
						
						//okay now we add our actual file 
						xml.addValue("key", hash[k]);
						
						int dictID = xml.addTag("dict");
						if( xml.pushTag("dict", dictID) ){

							msgStr << "   --adding file " << fileName[k] <<  " with hash " << hash[k] << endl; 
						
							xml.addValue("key", "fileEncoding");
							xml.addValue("string", "4");

							xml.addValue("key", "isa");
							xml.addValue("string", "PBXFileReference");

							xml.addValue("key", "lastKnownFileType");
							xml.addValue("string", fileKind);

							xml.addValue("key", "name");
							xml.addValue("string", fileName[k]);

							xml.addValue("key", "path");
							xml.addValue("string", filePath[k]);

							xml.addValue("key", "sourceTree");
							xml.addValue("string", "SOURCE_ROOT");						
							
							xml.popTag();
						}
						
						if( addToBuild ){
						
							//now we need to create a fileRef for the build phase. 
							string buildHash = getHash(filePath[k]+".build");
							xml.addValue("key", buildHash);
							
							dictID = xml.addTag("dict");
							if( xml.pushTag("dict", dictID) ){

								msgStr << "  --adding fileRef for " << fileName[k] <<  endl; 
							
								xml.addValue("key", "fileRef");
								xml.addValue("string", hash[k]);

								xml.addValue("key", "isa");
								xml.addValue("string", "PBXBuildFile");
													
								xml.popTag();
							}	
							
							//finally we need to search for the buildActionMask dict and add the fileRef
							
							//lets add our file hash to the group 									
							int numDicts = xml.getNumTags("dict");
							for(int j = 0; j < numDicts; j++){
								bool bFound = false;
								if( xml.getValue("dict:key", "null", j) == "buildActionMask" ){
									msgStr << " --found build dict! " << endl;
									
									if( xml.pushTag("dict", j) ){
										if( xml.getValue("string", "null", 1) == "PBXSourcesBuildPhase" ){
											
											if( xml.getValue("key", "null", 1) == "files" ){
												if( !xml.tagExists("array") ){
													xml.addTag("array");
												}
												if( xml.pushTag("array") ){
													msgStr << " --added hash to build phase! " << endl;
							
													xml.addValue("string", buildHash);
													xml.popTag();
												}
											}
											
											bFound = true;
										}
										xml.popTag();
										if( bFound ){
											break;
										}
									}									
								}
							}
						}
						
					}
					xml.popTag();
				}
				xml.popTag();
			}
			xml.popTag();
		}
	}
	
	xml.saveFile();
}

//<install>
//    <name>ofxOpenCv</name>
//	<version>0.02</version>
//	<author>zach lieberman, stefan hechenberger</author>
//	<url></url>
//
//	<add>
//
//		<!-- 	=====================================================================	-->
//		<!--	========================== add per project using this addon =========	-->
//		<!-- 	=====================================================================	-->
//
//		<src>
//			<folder name="addons/ofxOpenCv/src">						
//				<file>../../../addons/ofxOpenCv/src/ofxCvBlob.h</file>
//				<file>../../../addons/ofxOpenCv/src/ofxCvColorImage.cpp</file>
//				<file>../../../addons/ofxOpenCv/src/ofxCvColorImage.h</file>
//				<file>../../../addons/ofxOpenCv/src/ofxCvConstants.h</file>
//				<file>../../../addons/ofxOpenCv/src/ofxCvContourFinder.cpp</file>
//				<file>../../../addons/ofxOpenCv/src/ofxCvContourFinder.h</file>
//				<file>../../../addons/ofxOpenCv/src/ofxCvFloatImage.cpp</file>
//				<file>../../../addons/ofxOpenCv/src/ofxCvFloatImage.h</file>
//				<file>../../../addons/ofxOpenCv/src/ofxCvGrayscaleImage.cpp</file>
//				<file>../../../addons/ofxOpenCv/src/ofxCvGrayscaleImage.h</file>
//				<file>../../../addons/ofxOpenCv/src/ofxCvImage.cpp</file>
//				<file>../../../addons/ofxOpenCv/src/ofxCvImage.h</file>
//				<file>../../../addons/ofxOpenCv/src/ofxCvMain.h</file>
//				<file>../../../addons/ofxOpenCv/src/ofxCvShortImage.cpp</file>
//				<file>../../../addons/ofxOpenCv/src/ofxCvShortImage.h</file>				
//				<file>../../../addons/ofxOpenCv/src/ofxOpenCv.h</file>
//			</folder>
//		</src>
//
//		<include>




void addGroupToProject(string xcodePath,  string hashParent, string groupName, string hashChild ){
	
	ofxXmlSettings xml;
	if( xml.loadFile(xcodePath + "/project.pbxproj") ){
		if( xml.pushTag("plist") ){
			if( xml.pushTag("dict") ){
				if( xml.pushTag("dict", 1)  ){
									
					int numKeyTags = xml.getNumTags("key");
					for(int j = 0; j < numKeyTags; j++){
						if( xml.getValue("key", "null", j) == hashParent ){
							msgStr << " --found group key! " << endl;
							
							if( xml.pushTag("dict", j) ){
								if( xml.getValue("key", "null") == "children" ){
									if( !xml.tagExists("array") ){
										xml.addTag("array");
									}
									if( xml.pushTag("array") ){
										msgStr << " --added hash to group! " << endl;
				
										xml.addValue("string", hashChild);
										xml.popTag();
									}
								}
								xml.popTag();
							}
							
							break;
						}
						
					}
					
					xml.addValue("key", hashChild);
						
					int dictID = xml.addTag("dict");
					if( xml.pushTag("dict", dictID) ){

						msgStr << "   --adding group to project " << groupName <<  " with hash " << hashChild << endl; 
					
						xml.addValue("key", "children");
						xml.addValue("array", "");

						xml.addValue("key", "isa");
						xml.addValue("string", "PBXGroup");

						xml.addValue("key", "name");
						xml.addValue("string", groupName);

						xml.addValue("key", "sourceTree");
						xml.addValue("string", "<group>");						
						
						xml.popTag();
					}
					
					xml.popTag();
				}
				xml.popTag();
			}
			xml.popTag();
		}
	}
	
	xml.saveFile();

}

//
//--------------------------------------------------------------
void addAddonsFromInstallXML(string addonPath, string xcodePath){
	ofxXmlSettings xml;
	
	string installXmlPath = addonPath + "install.xml";
	xml.loadFile(installXmlPath);
	
	if( xml.pushTag("install") ){
		string addonName = xml.getValue("name", "null");

		string rootHash =  getHash(addonPath+addonName);
		addGroupToProject(xcodePath, "BB4B014C10F69532006C3DED", addonName, rootHash);
	
		if( xml.pushTag("add") ){
			if( xml.pushTag("src") ){
				int numFolders = xml.getNumTags("folder");
				for(int k = 0; k < numFolders; k++){
					string folderPath = xml.getAttribute("folder", "name", "null", k);
					string folderName = ofFilePath::getFilename(folderPath);
					
					cout << "adding folder folderName: " << folderName << endl;
					
					string curHash = getHash(folderPath);
					addGroupToProject(xcodePath, rootHash, folderName, curHash);
					
					if( xml.pushTag("folder", k) ){
						int numFiles = xml.getNumTags("file");

						vector <string> filePaths;
						vector <string> fileNames; 
						vector <string> hashes;
						
						for(int d = 0; d < numFiles; d++){
							filePaths.push_back( xml.getValue("file", "null", d) );
							fileNames.push_back( ofFilePath::getFilename( filePaths.back() ) );
							hashes.push_back( getHash(filePaths.back()) );
							
							cout << " adding " << filePaths.back() <<  " " << fileNames.back() << " " << hashes.back() << endl;
						}
					
						addFilesToProjectXML(xcodePath, filePaths, fileNames, hashes, curHash);
						xml.popTag();
					}
				}
				xml.popTag();
			}
			xml.popTag();
		}
	
		xml.popTag();
	}
}

//--------------------------------------------------------------
void convertProjectToXML(string projectPath){
	msgStr << "  --converting project to xml " << endl;
	
	string absPath = ofFilePath::getAbsolutePath(projectPath);
	system(string("cd "+absPath+"; plutil -convert xml1 -o - project.pbxproj > project.xml; mv project.xml project.pbxproj").c_str());
}

//--------------------------------------------------------------
void openProject(string projectPath){
	cout << "opening: " << projectPath << endl;
	projectPath = ofFilePath::getAbsolutePath(projectPath, true);
	system(string("open "+ projectPath).c_str());	
	ofSleepMillis(3000);
}

//--------------------------------------------------------------
void checkAddSrcFiles(string projectName, string xcodePath, string srcFolderPath){

	ofDirectory srcPath;
	srcPath.allowExt("h");
	srcPath.allowExt("cpp");		
	srcPath.listDir( srcFolderPath );
	if( srcPath.size() > 3 ){
		msgStr << "   adding src/ files " << endl;
		//openProject(xcodePath);
		convertProjectToXML(xcodePath);
	
		vector <string> filePathsToAdd;
		vector <string> fileNamesToAdd;	
		vector <string> hashes;
		for(int j = 0; j < srcPath.size(); j++){
			if( srcPath.getName(j) != "main.cpp" && srcPath.getName(j) != "testApp.cpp" &&  srcPath.getName(j) != "testApp.h" ){
				
				//this should be: filePathsToAdd.push_back( "src/" + srcPath.getName(j) ); but applescript makes it relative to group not project
				filePathsToAdd.push_back( "src/"+srcPath.getName(j) );
				fileNamesToAdd.push_back( srcPath.getName(j) );
				
				hashes.push_back( getHash(srcPath.getPath(j)) );
			}
		}
		
		//TODO: get group hash of src/ folder programmatically 
		addFilesToProjectXML(xcodePath, filePathsToAdd, fileNamesToAdd, hashes, "E4B69E1C0A3A1BDC003C02F2");			
	}	

}

//--------------------------------------------------------------
void testApp::setup(){

	msgStr << "deploying examples"  << endl;
	
	ofDirectory examples;
	examples.listDir(appsPath + "examples/");
	
	for(int k = 0; k < examples.size(); k++){
		string folderName = examples.getName(k);
	
		msgStr << " " << folderName << endl;
		ofFile::copyFromTo("OF_template.xcodeproj", examples.getPath(k) + "/" + folderName + ".xcodeproj");
		ofFile::copyFromTo("Project.xcconfig",  examples.getPath(k) + "/Project.xcconfig" );
	}
	
	msgStr << endl << "deploying advancedExamples" << endl;
	
	ofDirectory advanced;
	advanced.listDir(appsPath + "advancedExamples/");
	
	for(int k = 0; k < advanced.size(); k++){
		string folderName = advanced.getName(k);
		string xcodePath = advanced.getPath(k) + "/" + folderName + ".xcodeproj";
		
		msgStr << " " << folderName << endl;
				
		ofDirectory::removeDirectory(xcodePath, true);
		ofFile::copyFromTo("OF_template.xcodeproj", xcodePath);
		ofFile::copyFromTo("Project.xcconfig",  advanced.getPath(k) + "/Project.xcconfig" );
	
		checkAddSrcFiles(folderName, xcodePath, advanced.getPath(k) + "/src/");		
	}	


	msgStr << endl << "deploying addonsExamples" << endl;
	
	ofDirectory addons;
	addons.listDir(appsPath + "addonsExamples/");
	
	for(int k = 0; k < addons.size(); k++){
		string folderName = addons.getName(k);
		string xcodePath = addons.getPath(k) + "/" + folderName + ".xcodeproj";
		
		msgStr << " " << folderName << endl;
				
		ofDirectory::removeDirectory(xcodePath, true);
		ofFile::copyFromTo("OF_template.xcodeproj", xcodePath);
		ofFile::copyFromTo("Project.xcconfig",  addons.getPath(k) + "/Project.xcconfig" );
		
		if( folderName == "xmlSettingsExample" ){
			convertProjectToXML(xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxXmlSettings/", xcodePath);
		}
		
		if( folderName.substr(0,7) == "network" ){
			convertProjectToXML(xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxNetwork/", xcodePath);
		}

		if( folderName  == "ofxCvHaarFinderExample" || folderName  == "opencvExample"){
			convertProjectToXML(xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxOpenCv/", xcodePath);
		}
				
		checkAddSrcFiles(folderName, xcodePath, addons.getPath(k) + "/src/");	
	}		
	
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	ofTranslate(0, -10 * ofGetMouseY(), 0);

	ofSetColor(0, 0, 0);
	ofDrawBitmapString(msgStr.str(), 10, 20);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}