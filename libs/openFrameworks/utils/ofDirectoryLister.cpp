#include "ofDirectoryLister.h"

#include "ofUtils.h"
#include "ofFileUtils.h"
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
ofDirectoryLister::ofDirectoryLister() {
	showHidden = false;
}

//----------------------------------------
void ofDirectoryLister::allowExt(string extension) {
	if(extension == "*") {
		ofLog(OF_LOG_WARNING, "ofDirectoryLister::allowExt() the extension * is deprecated");
	}
	extensions.push_back(toLower(extension));
}

//----------------------------------------
int ofDirectoryLister::listDir(string directory, bool absolute) {
	directory = ofFileUtils::getPathForDirectory(directory);
	
	files.clear();

	originalDirectory = directory;
	string absolutePath = directory;
	if(!absolute) {
		absolutePath = ofToDataPath(directory);
	}
	
	Path base(absolutePath);
	File cur(base);
	if(cur.exists()) {
		// File::list(vector<File>) is broken on windows as of march 23, 2011...
		// so we need to use File::list(vector<string>) and build a vector<File>
		// in the future the following can be replaced width: cur.list(files);
		vector<string> fileStrings;
		cur.list(fileStrings);
		for(int i = 0; i < fileStrings.size(); i++) {
			Path curPath = base;
			curPath.setFileName(fileStrings[i]);
			files.push_back(File(curPath));
		}

		if(!showHidden) {
			ofRemove(files, hiddenFile);
		}

		if(!extensions.empty() && !ofContains(extensions, (string) "*")) {
			ExtensionComparator extensionFilter;
			extensionFilter.extensions = &extensions;
			ofRemove(files, extensionFilter);
		}

		// TODO: if(ofCheckLogLevel(OF_LOG_VERBOSE)) {
		for(int i = 0; i < (int)size(); i++) {
			ofLog(OF_LOG_VERBOSE, "\t" + getName(i));
		}
		ofLog(OF_LOG_VERBOSE, "ofDirectoryLister::listDirectory() listed " + ofToString(size()) + " files in " + directory);
		// }
	} else {
		ofLog(OF_LOG_ERROR, "ofDirectoryLister::listDirectory() error opening directory " + directory);
	}

	return size();
}

//----------------------------------------
string ofDirectoryLister::getName(unsigned int position) {
	Path cur(files.at(position).path());
	return cur.getFileName();
}

//----------------------------------------
string ofDirectoryLister::getPath(unsigned int position) {
	Path cur(originalDirectory);
	cur.setFileName(getName(position));
	return cur.toString();
}

//----------------------------------------
void ofDirectoryLister::reset() {
	files.clear();
	extensions.clear();
}

//----------------------------------------
void ofDirectoryLister::sort() {
	ofSort(files);
}

//----------------------------------------
unsigned int ofDirectoryLister::size() {
	return files.size();
}

//----------------------------------------
int ofDirectoryLister::numFiles() {
	return size();
}
