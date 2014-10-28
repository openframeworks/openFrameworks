#include "ofxThreadedImageLoader.h"
#include <sstream>
ofxThreadedImageLoader::ofxThreadedImageLoader(){
	nextID = 0;
    ofAddListener(ofEvents().update, this, &ofxThreadedImageLoader::update);
	ofAddListener(ofURLResponseEvent(),this,&ofxThreadedImageLoader::urlResponse);
    
    startThread();
    lastUpdate = 0;
}

ofxThreadedImageLoader::~ofxThreadedImageLoader(){
	images_to_load_from_disk.close();
	images_to_update.close();
	waitForThread(true);
    ofRemoveListener(ofEvents().update, this, &ofxThreadedImageLoader::update);
	ofRemoveListener(ofURLResponseEvent(),this,&ofxThreadedImageLoader::urlResponse);
}

// Load an image from disk.
//--------------------------------------------------------------
void ofxThreadedImageLoader::loadFromDisk(ofImage& image, string filename) {
	nextID++;
	ofImageLoaderEntry entry(image);
	entry.filename = filename;
	entry.image->setUseTexture(false);
	entry.name = filename;
    
	images_to_load_from_disk.send(entry);
}


// Load an url asynchronously from an url.
//--------------------------------------------------------------
void ofxThreadedImageLoader::loadFromURL(ofImage& image, string url) {
	nextID++;
	ofImageLoaderEntry entry(image);
	entry.url = url;
	entry.image->setUseTexture(false);	
	entry.name = "image" + ofToString(nextID);
	images_async_loading[entry.name] = entry;
	ofLoadURLAsync(entry.url, entry.name);
}


// Reads from the queue and loads new images.
//--------------------------------------------------------------
void ofxThreadedImageLoader::threadedFunction() {
	thread.setName("ofxThreadedImageLoader " + thread.name());
	ofImageLoaderEntry entry;
	while( images_to_load_from_disk.receive(entry) ) {
		if(entry.image->load(entry.filename) )  {
			images_to_update.send(entry);
		}else{
			ofLogError("ofxThreadedImageLoader") << "couldn't load file: \"" << entry.filename << "\"";
		}
	}
	ofLogVerbose("ofxThreadedImageLoader") << "finishing thread on closed queue";
}


// When we receive an url response this method is called; 
// The loaded image is removed from the async_queue and added to the
// update queue. The update queue is used to update the texture.
//--------------------------------------------------------------
void ofxThreadedImageLoader::urlResponse(ofHttpResponse & response) {
	// this happens in the update thread so no need to lock to access
	// images_async_loading
	entry_iterator it = images_async_loading.find(response.request.name);
	if(response.status == 200) {
		if(it != images_async_loading.end()) {
			it->second.image->load(response.data);
			images_to_update.send(it->second);
		}
	}else{
		// log error.
		ofLogError("ofxThreadedImageLoader") << "couldn't load url, response status: " << response.status;
		ofRemoveURLRequest(response.request.getID());
	}

	// remove the entry from the queue
	if(it != images_async_loading.end()) {
		images_async_loading.erase(it);
	}
}


// Check the update queue and update the texture
//--------------------------------------------------------------
void ofxThreadedImageLoader::update(ofEventArgs & a){
    // Load 1 image per update so we don't block the gl thread for too long
	ofImageLoaderEntry entry;
	if (images_to_update.tryReceive(entry)) {
		entry.image->setUseTexture(true);
		entry.image->update();
	}
}

