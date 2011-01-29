#pragma once

#include <deque>
#include "ofThread.h"
#include "ofImage.h"
#include "ofURLFileLoader.h"
#include "ofTypes.h" 

using namespace std;

// Where to load form?
enum ofLoaderType {
	 OF_LOAD_FROM_DISK
	,OF_LOAD_FROM_URL
};


// Entry to load.
struct ofImageLoaderEntry {
	ofImageLoaderEntry() {
		image = NULL;
	}
	
	ofImageLoaderEntry(ofImage* pImage, ofLoaderType nType) {
		image = pImage;
		type = nType;
	}
	ofImage* image;
	ofLoaderType type;
	string filename;
	string url;
	string name;
	int id;
};

typedef deque<ofImageLoaderEntry>::iterator entry_iterator;


class ofxThreadedImageLoader : public ofThread {
public:
	ofxThreadedImageLoader();
	void loadFromDisk(ofImage* image, string file);
	void loadFromURL(ofImage* image, string url);
	void start();
	void update(ofEventArgs & a);
	virtual void threadedFunction();
	void urlResponse(ofHttpResponse & response);
	entry_iterator getEntryFromAsyncQueue(string name);
	friend ostream& operator<<(ostream& os, const ofxThreadedImageLoader& loader);

	deque<ofImageLoaderEntry> images_async_loading; // keeps track of images which are loading async
	deque<ofImageLoaderEntry> images_to_load;
	deque<ofImageLoaderEntry> images_to_update;
	
private:
	bool shouldLoadImages();
	ofImageLoaderEntry getNextImageToLoad();
	ofImageLoaderEntry getNextImageToUpdate();
	
	int num_loading;

	
};

//TODO: Theo - THIS DOESN'T MAKE SENSE! You are overloading the << operator but its not clear what that is doing. 
//its not a data structure like ofPoint or ofRect which << overloading makes sense for. 
//itnerally it should call ofLog for the approproate log levels. 
inline ostream& operator<<(ostream& os,  const ofxThreadedImageLoader& loader) {
	
	deque<ofImageLoaderEntry>::const_iterator it = loader.images_async_loading.begin();
	if(it != loader.images_async_loading.end()) {
		os << "Loading from url\n-----------------------\n" << std::endl;
		while(it != loader.images_async_loading.end()) {
			os << "Loading: " << (*it).url << std::endl;
			++it;
		}
		os << std::endl;
	}

	os << "To be loaded from disk\n-----------------------\n";
	it = loader.images_to_load.begin();
	if(it != loader.images_to_load.end()) {
		
		while(it != loader.images_to_load.end()) {
			if ((*it).type == OF_LOAD_FROM_DISK) {
				os << (*it).filename << std::endl;
			}
			++it;
		}
		os << std::endl;
	}
	
	it = loader.images_to_load.begin();
	if(it != loader.images_to_load.end()) {
		os << "To be loaded from url\n-----------------------\n";
		while(it != loader.images_to_load.end()) {
			if((*it).type == OF_LOAD_FROM_URL) {
				os << (*it).url << std::endl;
			}
			++it;
		}
		os << std::endl;
	}
	return os;
}