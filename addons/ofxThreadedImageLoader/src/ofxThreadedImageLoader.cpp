#include "ofxThreadedImageLoader.h"
#include <sstream>
ofxThreadedImageLoader::ofxThreadedImageLoader() 
:ofThread()
{
	num_loading = 0;
	ofAddListener(ofEvents.update, this, &ofxThreadedImageLoader::update);
	ofRegisterURLNotification(this);
}


// Load an image from disk.
//--------------------------------------------------------------
void ofxThreadedImageLoader::loadFromDisk(ofImage* image, string filename) {
	lock();	
	
	num_loading++;
	ofImageLoaderEntry entry(image, OF_LOAD_FROM_DISK);
	entry.filename = filename;
	entry.id = num_loading;
	entry.image->setUseTexture(false);
	images_to_load.push_back(entry);
	
	unlock();
}


// Load an url asynchronously from an url.
//--------------------------------------------------------------
void ofxThreadedImageLoader::loadFromURL(ofImage* image, string url) {
	lock();
	
	num_loading++;
	ofImageLoaderEntry entry(image, OF_LOAD_FROM_URL);
	entry.url = url;
	entry.id = num_loading;
	entry.image->setUseTexture(false);	
	
	stringstream ss;
	ss << "image" << entry.id;
	entry.name = ss.str();
	images_to_load.push_back(entry);
	
	unlock();
}


// Reads from the queue and loads new images.
//--------------------------------------------------------------
void ofxThreadedImageLoader::threadedFunction() {
	while(true) {
		if(shouldLoadImages()) {
			ofImageLoaderEntry entry = getNextImageToLoad();
			if(entry.image == NULL) {
				continue;
			}
		
			if(entry.type == OF_LOAD_FROM_DISK) {
				entry.image->loadImage(entry.filename);
				lock();
				images_to_update.push_back(entry);
				unlock();
			}
			else if(entry.type == OF_LOAD_FROM_URL) {
				lock();
				images_async_loading.push_back(entry);
				unlock();	
				ofLoadURLAsync(entry.url, entry.name);
			}
		}
		else {
			// TODO: what do we do when there ar eno entries left?
		//	ofSleepMillis(1000);
		}
	}
}


// When we receive an url response this method is called; 
// The loaded image is removed from the async_queue and added to the
// update queue. The update queue is used to update the texture.
//--------------------------------------------------------------
void ofxThreadedImageLoader::urlResponse(ofHttpResponse & response) {
	if(response.status == 200) {
		lock();
		
		// Get the loaded url from the async queue and move it into the update queue.
		entry_iterator it = getEntryFromAsyncQueue(response.request.name);
		if(it != images_async_loading.end()) {
			(*it).image->loadImage(response.data);
			images_to_update.push_back((*it));
			images_async_loading.erase(it);
		}
		
		unlock();
	}
	else {
		// log error.
		stringstream ss;
		ss << "Could not image from url, response status: " << response.status;
		ofLog(OF_LOG_ERROR, ss.str());
		
		// remove the entry from the queue
		lock();
		entry_iterator it = getEntryFromAsyncQueue(response.request.name);
		if(it != images_async_loading.end()) {
			images_async_loading.erase(it);
		}
		else {
			ofLog(OF_LOG_WARNING, "Cannot find image in load-from-url queue");
		}
		unlock();
	}
}

// Find an entry in the aysnc queue.
//--------------------------------------------------------------
entry_iterator ofxThreadedImageLoader::getEntryFromAsyncQueue(string name) {
	entry_iterator it = images_async_loading.begin();		
	while(it != images_async_loading.end()) {
		if((*it).name == name) {
			return it;			
		}
	}
	return images_async_loading.end();
}


// Check the update queue and update the texture
//--------------------------------------------------------------
void ofxThreadedImageLoader::update(ofEventArgs & a){
	ofImageLoaderEntry entry = getNextImageToUpdate();
	if (entry.image != NULL) {

		const ofPixels& pix = entry.image->getPixelsRef();
		entry.image->getTextureReference().allocate(
				 pix.getWidth()
				,pix.getHeight()
				,ofGetGlInternalFormat(pix)
		);
		
		entry.image->setUseTexture(true);
		entry.image->update();
	}
}


// Pick an entry from the queue with images for which the texture
// needs to be update.
//--------------------------------------------------------------
ofImageLoaderEntry ofxThreadedImageLoader::getNextImageToUpdate() {
	lock();
	ofImageLoaderEntry entry;
	if(images_to_update.size() > 0) {
		entry = images_to_update.front();
		images_to_update.pop_front();
	}	
	unlock();
	return entry;
}

// Pick the next image to load from disk.
//--------------------------------------------------------------
ofImageLoaderEntry ofxThreadedImageLoader::getNextImageToLoad() {
	lock();
	ofImageLoaderEntry entry;
	if(images_to_load.size() > 0) {
		entry = images_to_load.front();
		images_to_load.pop_front();
	}
	unlock();
	return entry;
}

// Check if there are still images in the queue.
//--------------------------------------------------------------
bool ofxThreadedImageLoader::shouldLoadImages() {
	return (images_to_load.size() > 0);
}

