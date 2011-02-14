#include "ofDirList.h"

#include "ofUtils.h"
#include "Poco/Path.h"
#include "Poco/String.h"

// for Poco::File, Path, and toLower()
using namespace Poco;

//----------------------------------------
// Simple example of a boolean function that can be used
// with ofRemove to remove items from vectors.
bool hiddenFile(File file) {
	return file.isHidden();
}

//----------------------------------------
// Advanced example of a class that has a boolean function
// that can be used with ofRemove to remove items from vectors.
class ExtensionComparator : public unary_function<File, bool> {
public:
	vector <string>* extensions;
	inline bool operator()(const File& file) {
		Path curPath(file.path());
		string curExtension = toLower(curPath.getExtension());
		return !ofContains(*extensions, curExtension);
	}
};

//----------------------------------------
ofDirList::ofDirList() {
	showHidden = false;
}

//----------------------------------------
void ofDirList::allowExt(string extension) {
	if(extension == "*") {
		ofLog(OF_LOG_WARNING, "ofDirList::allowExt() the extension * is deprecated");
	}
	extensions.push_back(toLower(extension));
}

//----------------------------------------
int ofDirList::listDir(string directory, bool absolute) {
	files.clear();

	originalDirectory = directory;
	string absolutePath = directory;
	if(!absolute) {
		absolutePath = ofToDataPath(directory);
	}
	
	File cur(absolutePath);
	if(cur.exists()) {
		cur.list(files);
		
		if(!showHidden) {
			ofRemove(files, hiddenFile);
		}
		
		if(!extensions.empty() && !ofContains(extensions, (string) "*")) {
			ExtensionComparator extensionFilter;
			extensionFilter.extensions = &extensions;
			ofRemove(files, extensionFilter);
		}
		
		// TODO: if(ofCheckLogLevel(OF_LOG_VERBOSE)) {
		for(int i = 0; i < size(); i++) {
			ofLog(OF_LOG_VERBOSE, "\t" + getName(i));
		}
		ofLog(OF_LOG_VERBOSE, "ofDirList::listDirectory() listed " + ofToString(size()) + " files in " + directory);
		// }
	} else {
		ofLog(OF_LOG_ERROR, "ofDirList::listDirectory() error opening directory " + directory);
	}
	
	return size();
}

//----------------------------------------
string ofDirList::getName(unsigned int position) {
	Path cur(files[position].path());
	return cur.getFileName();
}

//----------------------------------------
string ofDirList::getPath(unsigned int position) {
	Path cur(files[position].path());
	return cur.toString();
}

//----------------------------------------
void ofDirList::reset() {
	files.clear();
	extensions.clear();
}

//----------------------------------------
void ofDirList::sort() {
	ofSort(files);
}

//----------------------------------------
unsigned int ofDirList::size() {
	return files.size();
}

//----------------------------------------
int ofDirList::numFiles() {
	return size();
}
