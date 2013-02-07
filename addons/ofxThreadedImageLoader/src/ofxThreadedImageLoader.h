#pragma once

#include <deque>
#include "ofThread.h"
#include "ofImage.h"
#include "ofURLFileLoader.h"
#include "ofTypes.h" 

// must use poco condition not lock for this
#include "Poco/Condition.h"

using namespace std;

class ofxThreadedImageLoader : public ofThread {
public:
    ofxThreadedImageLoader();
    ~ofxThreadedImageLoader();

	void loadFromDisk(ofImage& image, string file);
	void loadFromURL(ofImage& image, string url);



private:
	void update(ofEventArgs & a);
    virtual void threadedFunction();
	void urlResponse(ofHttpResponse & response);

    // Where to load form?
    enum ofLoaderType {
        OF_LOAD_FROM_DISK
        ,OF_LOAD_FROM_URL
    };
    
    
    // Entry to load.
    struct ofImageLoaderEntry {
        ofImageLoaderEntry() {
            image = NULL;
            type = OF_LOAD_FROM_DISK;
            id=0;
        }
        
        ofImageLoaderEntry(ofImage & pImage, ofLoaderType nType) {
            image = &pImage;
            type = nType;
            id=0;
        }
        ofImage* image;
        ofLoaderType type;
        string filename;
        string url;
        string name;
        int id;
    };


    typedef deque<ofImageLoaderEntry>::iterator entry_iterator;
	entry_iterator      getEntryFromAsyncQueue(string name);

	int                 nextID;

    Poco::Condition     condition;

    int                 lastUpdate;

	deque<ofImageLoaderEntry> images_async_loading; // keeps track of images which are loading async
	deque<ofImageLoaderEntry> images_to_load_buffer;
    deque<ofImageLoaderEntry> images_to_update;
};


