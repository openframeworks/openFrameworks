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


class ofThreadedImageLoader : public ofThread {
public:
	ofThreadedImageLoader();
	void loadFromDisk(ofImage* image, string file);
	void loadFromURL(ofImage* image, string url);
	void start();
	void update(ofEventArgs & a);
	virtual void threadedFunction();
	void urlResponse(ofHttpResponse & response);
	entry_iterator getEntryFromAsyncQueue(string name);

private:
	bool shouldLoadImages();
	ofImageLoaderEntry getNextImageToLoad();
	ofImageLoaderEntry getNextImageToUpdate();
	
	int num_loading;

	deque<ofImageLoaderEntry> images_async_loading; // keeps track of images which are loading async
	deque<ofImageLoaderEntry> images_to_load;
	deque<ofImageLoaderEntry> images_to_update;;
};