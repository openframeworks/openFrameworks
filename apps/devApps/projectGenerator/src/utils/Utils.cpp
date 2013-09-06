/*
 * Utils.cpp
 *
 *  Created on: 28/12/2011
 *      Author: arturo
 */

#include "Utils.h"

#include <algorithm>
#include <Poco/DirectoryIterator.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/LocalDateTime.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

#include "Poco/HMACEngine.h"
#include "Poco/MD5Engine.h"
using Poco::DigestEngine;
using Poco::HMACEngine;
using Poco::MD5Engine;



#ifdef TARGET_WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#elif defined(TARGET_LINUX)
#include <unistd.h>
#define GetCurrentDir getcwd
#else
#include <mach-o/dyld.h>	/* _NSGetExecutablePath */
#include <limits.h>		/* PATH_MAX */
#endif


using namespace Poco;

#include "ofUtils.h"


string generateUUID(string input){

    std::string passphrase("openFrameworks"); // HMAC needs a passphrase

    HMACEngine<MD5Engine> hmac(passphrase); // we'll compute a MD5 Hash
    hmac.update(input);

    const DigestEngine::Digest& digest = hmac.digest(); // finish HMAC computation and obtain digest
    std::string digestString(DigestEngine::digestToHex(digest)); // convert to a string of hexadecimal numbers
    
    digestString = digestString.substr(0, 24); 

    return digestString;
}





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


std::string LoadFileAsString(const std::string & fn)
{
    std::ifstream fin(fn.c_str());

    if(!fin)
    {
        // throw exception
    }

    std::ostringstream oss;
    oss << fin.rdbuf();

    return oss.str();
}

void findandreplaceInTexfile (string fileName, std::string tFind, std::string tReplace ){
   if( ofFile::doesFileExist(fileName) ){
	
	    std::ifstream t(ofToDataPath(fileName).c_str());
	    std::stringstream buffer;
	    buffer << t.rdbuf();
		string bufferStr = buffer.str();
		t.close();
	    findandreplace(bufferStr, tFind, tReplace);
		ofstream myfile;
        myfile.open (ofToDataPath(fileName).c_str());
        myfile << bufferStr;
		myfile.close();

	/*
	std::ifstream ifile(ofToDataPath(fileName).c_str(),std::ios::binary);
	ifile.seekg(0,std::ios_base::end);
	long s=ifile.tellg();
	char *buffer=new char[s];
 	ifile.seekg(0);
	ifile.read(buffer,s);
	ifile.close();
	std::string txt(buffer,s);
	delete[] buffer;
	findandreplace(txt, tFind, tReplace);
	std::ofstream ofile(ofToDataPath(fileName).c_str());
	ofile.write(txt.c_str(),txt.size());
	*/  
		//return 0;
   } else {
       ; // some error checking here would be good.
   }
}




bool doesTagAndAttributeExist(pugi::xml_document & doc, string tag, string attribute, string newValue){
    char xpathExpressionExists[1024];
    sprintf(xpathExpressionExists, "//%s[@%s='%s']", tag.c_str(), attribute.c_str(), newValue.c_str());
    //cout <<xpathExpressionExists <<endl;
    pugi::xpath_node_set set = doc.select_nodes(xpathExpressionExists);
    if (set.size() != 0){
        return true;
    } else {
        return false;
    }
}

pugi::xml_node appendValue(pugi::xml_document & doc, string tag, string attribute, string newValue, bool overwriteMultiple){

    if (overwriteMultiple == true){
        // find the existing node...
        char xpathExpression[1024];
        sprintf(xpathExpression, "//%s[@%s='%s']", tag.c_str(), attribute.c_str(), newValue.c_str());
        pugi::xpath_node node = doc.select_single_node(xpathExpression);
        if(string(node.node().attribute(attribute.c_str()).value()).size() > 0){ // for some reason we get nulls here?
            // ...delete the existing node
            cout << "DELETING: " << node.node().name() << ": " << " " << node.node().attribute(attribute.c_str()).value() << endl;
            node.node().parent().remove_child(node.node());
        }
    }

    if (!doesTagAndAttributeExist(doc, tag, attribute, newValue)){
        // otherwise, add it please:
        char xpathExpression[1024];
        sprintf(xpathExpression, "//%s[@%s]", tag.c_str(), attribute.c_str());
        //cout << xpathExpression << endl;
        pugi::xpath_node_set add = doc.select_nodes(xpathExpression);
        pugi::xml_node node = add[add.size()-1].node();
        pugi::xml_node nodeAdded = node.parent().append_copy(node);
        nodeAdded.attribute(attribute.c_str()).set_value(newValue.c_str());
        return nodeAdded;
    }else{
    	return pugi::xml_node();
    }

}

// todo -- this doesn't use ofToDataPath -- so it's broken a bit.  can we fix?
void getFilesRecursively(const string & path, vector < string > & fileNames){

    ofDirectory dir;

    //ofLogVerbose() << "in getFilesRecursively "<< path << endl;

    dir.listDir(path);
    for (int i = 0; i < dir.size(); i++){
        ofFile temp(dir.getFile(i));
        if (dir.getName(i) == ".svn") continue; // ignore svn
        if (temp.isFile()){
            fileNames.push_back(dir.getPath(i));
        } else if (temp.isDirectory()){
            getFilesRecursively(dir.getPath(i), fileNames);
        }
    }
    //folderNames.push_back(path);

}

static vector <string> platforms;
bool isFolderNotCurrentPlatform(string folderName, string platform){
	if( platforms.size() == 0 ){
		platforms.push_back("osx");
		platforms.push_back("win_cb");
		platforms.push_back("vs");
		platforms.push_back("ios");
		platforms.push_back("linux");
		platforms.push_back("linux64");
		platforms.push_back("android");
		platforms.push_back("iphone");
	}

	for(int i = 0; i < platforms.size(); i++){
		if( folderName == platforms[i] && folderName != platform ){
			return true;
		}
	}

	return false;
}

void splitFromLast(string toSplit, string deliminator, string & first, string & second){
    size_t found = toSplit.find_last_of(deliminator.c_str());
    first = toSplit.substr(0,found);
    second = toSplit.substr(found+1);
}

void splitFromFirst(string toSplit, string deliminator, string & first, string & second){
    size_t found = toSplit.find(deliminator.c_str());
    first = toSplit.substr(0,found );
    second = toSplit.substr(found+deliminator.size());
}


void getFoldersRecursively(const string & path, vector < string > & folderNames, string platform){
    ofDirectory dir;
    dir.listDir(path);
    for (int i = 0; i < dir.size(); i++){
        ofFile temp(dir.getFile(i));
        if (temp.isDirectory() && isFolderNotCurrentPlatform(temp.getFileName(), platform) == false ){
            getFoldersRecursively(dir.getPath(i), folderNames, platform);
        }
    }
    folderNames.push_back(path);
}



void getLibsRecursively(const string & path, vector < string > & libFiles, vector < string > & libLibs, string platform ){
    
    
    
    
    if (ofFile::doesFileExist(ofFilePath::join(path, "libsorder.make"))){
        
        bool platformFound = false;
        
#ifdef TARGET_WIN32
        vector<string> splittedPath = ofSplitString(path,"\\");
#else
        vector<string> splittedPath = ofSplitString(path,"/");
#endif
        
        
        if(platform!=""){
            for(int j=0;j<(int)splittedPath.size();j++){
                if(splittedPath[j]==platform){
                    platformFound = true;
                    // break;
                }
            }
        }
        
        
        if (platformFound == true){
            vector < string > libsInOrder;
            ofFile libsorderMake(ofFilePath::join(path, "libsorder.make"));
            ofBuffer libsorderMakeBuff;
            libsorderMake >> libsorderMakeBuff;
            while(!libsorderMakeBuff.isLastLine() && libsorderMakeBuff.size() > 0){
                string line = libsorderMakeBuff.getNextLine();
                if (ofFile::doesFileExist(ofFilePath::join(path , line))){
                    
                    libLibs.push_back(ofFilePath::join(path , line) );
                } else {
                    libLibs.push_back(line);        // this might be something like ws2_32 or other libs no in this project
                }
            }
        }
        
    } else {
        
        
        ofDirectory dir;
        dir.listDir(path);
        
        
        for (int i = 0; i < dir.size(); i++){
            
#ifdef TARGET_WIN32
            vector<string> splittedPath = ofSplitString(dir.getPath(i),"\\");
#else
            vector<string> splittedPath = ofSplitString(dir.getPath(i),"/");
#endif
            
            ofFile temp(dir.getFile(i));
            
            if (temp.isDirectory()){
                //getLibsRecursively(dir.getPath(i), folderNames);
                getLibsRecursively(dir.getPath(i), libFiles, libLibs, platform);
                
            } else {
                
                
                bool platformFound = false;
                
                if(platform!=""){
                    for(int j=0;j<(int)splittedPath.size();j++){
                        if(splittedPath[j]==platform){
                            platformFound = true;
                        }
                    }
                }
                
                
                
                
                //string ext = ofFilePath::getFileExt(temp.getFile(i));
                string ext;
                string first;
                splitFromLast(dir.getPath(i), ".", first, ext);
                
                if (ext == "a" || ext == "lib" || ext == "dylib" || ext == "so" || ext == "dll"){
                    if (platformFound){
						libLibs.push_back(dir.getPath(i));
						
						//TODO: THEO hack
						if( platform == "ios" ){ //this is so we can add the osx libs for the simulator builds
							
							string currentPath = dir.getPath(i);
							
							//TODO: THEO double hack this is why we need install.xml - custom ignore ofxOpenCv 
							if( currentPath.find("ofxOpenCv") == string::npos ){
								ofStringReplace(currentPath, "ios", "osx");
								if( ofFile::doesFileExist(currentPath) ){
									libLibs.push_back(currentPath);
								}
							}
						}
					}
                } else if (ext == "h" || ext == "hpp" || ext == "c" || ext == "cpp" || ext == "cc"){
                    libFiles.push_back(dir.getPath(i));
                }
                
            }
        }
        
    }
    
    
    
    //folderNames.push_back(path);
    
    
    
    //    DirectoryIterator end;
    //        for (DirectoryIterator it(path); it != end; ++it){
    //            if (!it->isDirectory()){
    //            	string ext = ofFilePath::getFileExt(it->path());
    //            	vector<string> splittedPath = ofSplitString(ofFilePath::getEnclosingDirectory(it->path()),"/");
    //
    //                if (ext == "a" || ext == "lib" || ext == "dylib" || ext == "so"){
    //
    //                	if(platform!=""){
    //                		bool platformFound = false;
    //                		for(int i=0;i<(int)splittedPath.size();i++){
    //                			if(splittedPath[i]==platform){
    //                				platformFound = true;
    //                				break;
    //                			}
    //                		}
    //                		if(!platformFound){
    //                			continue;
    //                		}
    //                	}
    //                    libLibs.push_back(it->path());
    //                } else if (ext == "h" || ext == "hpp" || ext == "c" || ext == "cpp" || ext == "cc"){
    //                    libFiles.push_back(it->path());
    //                }
    //            }
    //
    //            if (it->isDirectory()){
    //                getLibsRecursively(it->path(), libFiles, libLibs, platform);
    //            }
    //        }
    
}



void fixSlashOrder(string & toFix){
    std::replace(toFix.begin(), toFix.end(),'/', '\\');
}


string unsplitString (vector < string > strings, string deliminator ){
    string result;
    for (int i = 0; i < (int)strings.size(); i++){
        if (i != 0) result += deliminator;
        result += strings[i];
    }
    return result;
}


static string OFRoot = "../../..";

string getOFRoot(){
	return ofFilePath::removeTrailingSlash(OFRoot);
}

string getAddonsRoot(){
	return ofFilePath::join(getOFRoot(), "addons");
}

void setOFRoot(string path){
	OFRoot = path;
}

string getOFRelPath(string from){
	from = ofFilePath::removeTrailingSlash(from);
    Poco::Path base(true);
    base.parse(from);

    Poco::Path path;
    path.parse( getOFRoot() );
    path.makeAbsolute();


	string relPath;
	if (path.toString() == base.toString()){
		// do something.
	}

	int maxx = MAX(base.depth(), path.depth());
	for (int i = 0; i <= maxx; i++){

		bool bRunOut = false;
		bool bChanged = false;
		if (i <= base.depth() && i <= path.depth()){
			if (base.directory(i) == path.directory(i)){

			} else {
				bChanged = true;
			}
		} else {
			bRunOut = true;
		}


		if (bRunOut == true || bChanged == true){
            for (int j = i; j <= base.depth(); j++){
				relPath += "../";
			}
			for (int j = i; j <= path.depth(); j++){
				relPath += path.directory(j) + "/";
			}
			break;
		}
	}

    ofLogVerbose() << " returning path " << relPath << endl;

    return relPath;
}

void parseAddonsDotMake(string path, vector < string > & addons){

    addons.clear();
	ofFile addonsmake(path);
	if(!addonsmake.exists()){
		return;
	}
	ofBuffer addonsmakebuff;
	addonsmake >> addonsmakebuff;
	while(!addonsmakebuff.isLastLine() && addonsmakebuff.size() > 0){
        string line = addonsmakebuff.getNextLine();
		if(line!=""){
			addons.push_back(line);
		}
	}
}

bool checkConfigExists(){
	
	return ofFile::doesFileExist(ofFilePath::join(ofFilePath::getUserHomeDir(),".ofprojectgenerator/config"));
}

bool askOFRoot(){
	ofFileDialogResult res = ofSystemLoadDialog("OF project generator", "choose the folder of your OF install");
	if (res.fileName == "" || res.filePath == "") return false;

	ofDirectory config(ofFilePath::join(ofFilePath::getUserHomeDir(),".ofprojectgenerator"));
	config.create(true);
	ofFile configFile(ofFilePath::join(ofFilePath::getUserHomeDir(),".ofprojectgenerator/config"),ofFile::WriteOnly);
	configFile << res.filePath;
	return true;
}

string getOFRootFromConfig(){
	if(!checkConfigExists()) return "";
	ofFile configFile(ofFilePath::join(ofFilePath::getUserHomeDir(),".ofprojectgenerator/config"),ofFile::ReadOnly);
	ofBuffer filePath = configFile.readToBuffer();
	return filePath.getFirstLine();
}
