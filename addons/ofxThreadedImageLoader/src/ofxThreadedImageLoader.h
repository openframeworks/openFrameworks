#pragma once

#include <deque>
#include "ofThread.h"
#include "ofImage.h"
#include "ofURLFileLoader.h"
#include "ofTypes.h" 

// must use poco condition not lock for this
#include "Poco/Condition.h"

// TODO make this a singleton to prevent unecessary number of threads
// TODO currently if a large number of images are queued, the update()
//      callback can take a long period while images are updated.
//      in the future a cap on image updates per update() could be implemented

using namespace std;

class ofxThreadedImageLoader : public ofThread {
public:
    
    
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
    
    
	ofxThreadedImageLoader();
	void loadFromDisk(ofImage* image, string file);
	void loadFromURL(ofImage* image, string url);
    
	void update(ofEventArgs & a);
    
    virtual void threadedFunction();
    
	void urlResponse(ofHttpResponse & response);
	

	deque<ofImageLoaderEntry> images_async_loading; // keeps track of images which are loading async
	
    deque<ofImageLoaderEntry> images_to_load;
	deque<ofImageLoaderEntry> images_to_load_buffer;
    
    
    deque<ofImageLoaderEntry> images_to_update;
	
private:
    
	bool                shouldLoadImages();
	ofImageLoaderEntry  getNextImageToLoad();
	ofImageLoaderEntry  getNextImageToUpdate();
	entry_iterator      getEntryFromAsyncQueue(string name);
    
	int                 num_loading;
    
    Poco::Condition     condition;
    
    int                 lastUpdate;
    
    int                 errorCounter;
};

