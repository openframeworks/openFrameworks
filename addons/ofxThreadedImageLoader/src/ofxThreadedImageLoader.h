#pragma once

#include "ofThread.h"
#include "ofImage.h"
#include "ofPixels.h"
#include "ofURLFileLoader.h"
#include "ofTypes.h"
#include "ofThreadChannel.h"

class ofxThreadedImageLoader : public ofThread {
public:
    ofxThreadedImageLoader();
    ~ofxThreadedImageLoader();

	void loadFromDisk(ofImage& image, std::string file);
	void loadFromURL(ofImage& image, std::string url);



private:
	void update(ofEventArgs & a);
    virtual void threadedFunction();
	void urlResponse(ofHttpResponse & response);
    
    // Entry to load.
    struct ofImageLoaderEntry {
    public:
        ofImageLoaderEntry() {
            image = NULL;
        }
        
        ofImageLoaderEntry(ofImage & pImage) {
            image = &pImage;
        }
        ofImage* image;
        std::string filename;
        std::string url;
        std::string name;
    };


    typedef std::map<std::string, ofImageLoaderEntry>::iterator entry_iterator;

	int                 nextID;
    int                 lastUpdate;

	std::map<std::string,ofImageLoaderEntry> images_async_loading; // keeps track of images which are loading async
	ofThreadChannel<ofImageLoaderEntry> images_to_load_from_disk;
	ofThreadChannel<ofImageLoaderEntry> images_to_update;
};


