#include "testApp.h"
#include "appleScript.h"

#include "Poco/HMACEngine.h"
#include "Poco/MD5Engine.h"

using Poco::DigestEngine;
using Poco::HMACEngine;
using Poco::MD5Engine;

ostringstream msgStr;
string appsPath = "../../../../";

void findandreplace( std::string& tInput, std::string tFind, std::string tReplace ) { 
	size_t uPos = 0; 
	size_t uFindLen = tFind.length(); 
	size_t uReplaceLen = tReplace.length();
		
	if( uFindLen == 0 ){
		return;
	}

	for( ;(uPos = tInput.find( tFind, uPos )) != std::string::npos; ){
		tInput.replace( uPos, uFindLen, tReplace );
		uPos += uReplaceLen;
	}	
	
}

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

//
//-----------------------------------------------------------------------------------------------------
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

//---------------------------------------------------------------
void appendToProjectXcconfig(string projectFile, string buildValue, vector <string> libraryFiles){
	string str;
	
	ifstream in;
	in.open(ofToDataPath(projectFile, true).c_str(), ( ios::in ) );
	
	ofstream out;
	out.open(ofToDataPath(projectFile + ".tmp", true).c_str(), ( ios::out ) );
	
	int k = 0; 
	getline(in, str);
	while( in ){
		
		if( str.find(buildValue) != string::npos ){			
			for(int k = 0; k < libraryFiles.size(); k++){
				string pathToAdd = libraryFiles[k];
				
				if( pathToAdd.substr(0, 15) == "../../../addons" ){
					pathToAdd = "$(OF_PATH)" + pathToAdd.substr( 8, pathToAdd.length()-8 );
				}
				str += " ";
				str += pathToAdd;
			}
		}
		
		out << str + "\n";
		getline(in, str);		
		k++;
	}
	
	in.close();
	out.close();
	
	ofFile::moveFromTo(projectFile + ".tmp", projectFile, true, true);
}

//
//--------------------------------------------------------------
void addAddonsFromInstallXML(string addonPath, string projectFolder, string xcodePath){
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
			
			if( xml.pushTag("link") ){
				vector <string> libs;
				
				int numLibTags = xml.getNumTags("lib");
				for(int k = 0; k < numLibTags; k++){
					if( xml.getAttribute("lib", "os", "null", k) == "mac" ){
						libs.push_back(xml.getValue("lib", "null", k));
					}
				}
				
				if( numLibTags ){
					appendToProjectXcconfig( projectFolder + "Project.xcconfig", "OTHER_LDFLAGS", libs );
				}

				xml.popTag();
			}

			if( xml.pushTag("include") ){
				vector <string> includes;
				
				int numIncludes = xml.getNumTags("path");
				for(int k = 0; k < numIncludes; k++){
					includes.push_back(xml.getValue("path", "null", k));
				}
				
				if( numIncludes ){
					appendToProjectXcconfig( projectFolder + "Project.xcconfig", "HEADER_SEARCH_PATHS", includes );
				}

				xml.popTag();
			}
			
			xml.popTag();
		}
	
		xml.popTag();
	}
}

//---------------------------------------------------------------
void addSearchPathsToProjectXcconfig(string projectFile, vector <string> searchPaths){

}

//--------------------------------------------------------------
void convertProjectToXML(string projectPath){
	msgStr << "  --converting project to xml " << endl;
	
	findandreplace(projectPath, " ", "\\ ");
	
	string absPath = ofFilePath::getAbsolutePath(projectPath);	
	system(string("cd "+absPath+"; plutil -convert xml1 -o - project.pbxproj > project.xml; mv project.xml project.pbxproj").c_str());
}

//--------------------------------------------------------------
void openProject(string projectPath){
	cout << "opening: " << projectPath << endl;
	projectPath = ofFilePath::getAbsolutePath(projectPath, true);
	
	findandreplace(projectPath, " ", "\\ ");
	
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
void copyProjectFiles(string folderName, string folderPath, string xcodePath ){

		msgStr << " " << folderName << endl;
		ofDirectory::removeDirectory(xcodePath, true);
		ofFile::copyFromTo("OF_template.xcodeproj", xcodePath);
		ofFile::copyFromTo("Project.xcconfig", folderPath + "Project.xcconfig" );
		ofFile::copyFromTo("openFrameworks-Info.plist", folderPath + "openFrameworks-Info.plist" );
		
		folderPath = ofFilePath::getAbsolutePath(folderPath, false);
		findandreplace(folderPath, " ", "\\ ");
		findandreplace(folderName, " ", "");
		
//		cout << "folderPath = " << folderPath <<endl;
//		cout << "folderName = " << folderName <<endl;
//		cout << "to perl = " << "'s/emptyExample/"+folderName+"/g'" <<endl;
		
//		cout << " before find and replace " << endl;
		system( string("cd "+folderPath+" ; find . -name \"*.pbxproj*\" -print0 | xargs -0 perl -pi -e 's/emptyExample/"+folderName+"/g'").c_str() );		
//		cout << " after find and replace " << endl;
}

//--------------------------------------------------------------
void testApp::setup(){

	msgStr << "deploying examples"  << endl;
	
	ofDirectory examples;
	examples.listDir(appsPath + "examples/");
	
	for(int k = 0; k < examples.size(); k++){
		string folderName = examples.getName(k);
		string folderPath = ofFilePath::getAbsolutePath(examples.getPath(k) + "/");
		string xcodePath  = folderPath + folderName + ".xcodeproj";
		
		copyProjectFiles(folderName, folderPath, xcodePath);
		checkAddSrcFiles(folderName, xcodePath, folderPath + "/src/");		
	}
	
	msgStr << endl << "deploying advancedExamples" << endl;
	
	ofDirectory advanced;
	advanced.listDir(appsPath + "advancedExamples/");
	
	for(int k = 0; k < advanced.size(); k++){
		string folderName = advanced.getName(k);
		string folderPath = ofFilePath::getAbsolutePath(advanced.getPath(k) + "/");
		string xcodePath = advanced.getPath(k) + "/" + folderName + ".xcodeproj";
												
		copyProjectFiles(folderName, folderPath, xcodePath);
		checkAddSrcFiles(folderName, xcodePath, folderPath + "/src/");		
	}	


	msgStr << endl << "deploying addonsExamples" << endl;
	
	ofDirectory addons;
	addons.listDir(appsPath + "addonsExamples/");
	
	for(int k = 0; k < addons.size(); k++){
		string folderName = addons.getName(k);
		string folderPath = ofFilePath::getAbsolutePath(addons.getPath(k) + "/");		
		string xcodePath = addons.getPath(k) + "/" + folderName + ".xcodeproj";
				
		copyProjectFiles(folderName, folderPath, xcodePath);
		checkAddSrcFiles(folderName, xcodePath, folderPath + "/src/");		
		
		if( folderName == "allAddonsExample" ){
			convertProjectToXML(xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofx3DModelLoader/", folderPath, xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxAssimpModelLoader/", folderPath, xcodePath);					
			addAddonsFromInstallXML( appsPath + "../addons/ofxSynth/", folderPath, xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxOsc/", folderPath, xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxThreadedImageLoader/", folderPath, xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxOpenCv/", folderPath, xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxNetwork/", folderPath, xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxVectorGraphics/", folderPath, xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxXmlSettings/", folderPath, xcodePath);
		}
		
		if( folderName == "3DModelLoaderExample" ){
			convertProjectToXML(xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofx3DModelLoader/", folderPath, xcodePath);
		}

		if( folderName ==  "ofxAssimpExample"){
			convertProjectToXML(xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxAssimpModelLoader/", folderPath, xcodePath);
		}

		if( folderName ==  "ofxSynthExample" || folderName ==  "ofxSynthSequencingExample" ){
			convertProjectToXML(xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxSynth/", folderPath, xcodePath);
		}

		if( folderName ==  "oscReceiveExample" || folderName ==  "oscSenderExample"){
			convertProjectToXML(xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxOsc/", folderPath, xcodePath);
		}

		if( folderName ==  "threadedImageLoaderExample" ){
			convertProjectToXML(xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxThreadedImageLoader/", folderPath, xcodePath);
		}

		if( folderName == "ofxCvHaarFinderExample" || folderName == "opencvExample" ){
			convertProjectToXML(xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxOpenCv/", folderPath, xcodePath);
		}

		if( folderName == "network" ){
			convertProjectToXML(xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxNetwork/", folderPath, xcodePath);
		}
		
		if( folderName == "vectorGraphicsExample" ){
			convertProjectToXML(xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxVectorGraphics/", folderPath, xcodePath);
		}

		if( folderName == "xmlSettingsExample" ){
			convertProjectToXML(xcodePath);
			addAddonsFromInstallXML( appsPath + "../addons/ofxXmlSettings/", folderPath, xcodePath);
		}								
								
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