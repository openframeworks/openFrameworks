#include "ofxDirList.h"
#include <algorithm>
#include <string>


// Handy string functions

static std::string::size_type idx;
static string getExt(string filename){
    idx = filename.rfind('.');

    if(idx != std::string::npos){
        return filename.substr(idx+1);
    }
    else{
        return "";
    }
}

static string strToLower(string myStr){
    transform( myStr.begin(), myStr.end(), myStr.begin(), ::tolower );
    return myStr;
}

//----------------------------------------------------------
ofxDirList::ofxDirList(){
    reset();
}

//----------------------------------------------------------
void ofxDirList::reset(){
	allowedFileExt.clear();
	nameArray.clear();
	pathArray.clear();
}

//----------------------------------------------------------
void ofxDirList::setVerbose(bool _verbose){
    ofLog(OF_LOG_WARNING, "ofxDirList setVerbose is depreciated use ofSetLogLevel instead");
}

//----------------------------------------------------------
bool ofxDirList::allowExt(string ext){
	allowedFileExt.push_back( strToLower(ext) );
	return true;
}

//----------------------------------------------------------
string ofxDirList::getName(int pos){
	if(pos >= (int)nameArray.size()) return 0;
	return nameArray[pos];
}

//----------------------------------------------------------
string ofxDirList::getPath(int pos){
	if(pos >= (int)pathArray.size()) return 0;
	return pathArray[pos];
}

//----------------------------------------------------------
int ofxDirList::listDir(string directory){
    directory = ofToDataPath(directory);

	nameArray.clear();
	pathArray.clear();

    if(directory.length() <= 0)return 0;

    //if the trailing slash was not added - then add it
	if( directory[directory.length()-1] != '/'){
        directory = directory + "/";
	}

	DIR *dir = NULL;
	struct dirent *entry;

    //open the directory
    ofLog(OF_LOG_VERBOSE, "ofxDirList - attempting to open %s", directory.c_str());
    dir = opendir(directory.c_str());

	if(dir == NULL){
		ofLog(OF_LOG_ERROR, "ofxDirList - error opening directory");
		return 0;
	}else{
		ofLog(OF_LOG_VERBOSE, "ofxDirList - success opening directory");
	}

    string entry_name = "";
    string ext = "";
	bool skip = false;

	while ((entry = readdir(dir)) != NULL){

        //turn it into a C++ string
        entry_name = entry->d_name;

        //lets get the length of the string here as we query it again
        int fileLen = entry_name.length();

		if(fileLen <= 0)continue; //if the name is not existant
		if(entry_name[0] == '.')continue; //ignore invisible files, ./ and ../

		//by default we don't skip files unless we are checking extensions
		skip = false;

		if(allowedFileExt.size() > 0){
			//we will skip this files unless it has an allowed extension
			skip = true;
			for(int i = 0; i < (int)allowedFileExt.size(); i++){

				//if the wildecard * has been entered for an ext type then don't check any extensions
				if( allowedFileExt[i] == "*"){ skip = false; break; }


				int extLen = allowedFileExt[i].length();

				//the extension has to be shorter than the filename - simple check
				if(extLen >= fileLen) continue;

                //lets get the ext as lowercase
                ext = strToLower( getExt(entry_name) );

                //if no ext - then skip this ext check
                if( ext == "" )continue;

                //if we find a match then stop checking and approve this file
                if(ext == allowedFileExt[i]){
                    skip = false;
                    break;
                }
			}
		}
		if(skip) continue;

		//finally we store the result
        pathArray.push_back(directory + entry_name);
        nameArray.push_back(entry_name);

		ofLog(OF_LOG_VERBOSE, "ofxDirList - listing %s ", nameArray.back().c_str());
	}
	if(dir != NULL) closedir(dir);

	ofLog(OF_LOG_VERBOSE, "ofxDirList - listed %i files in %s", nameArray.size(), directory.c_str());
	return nameArray.size();
}
