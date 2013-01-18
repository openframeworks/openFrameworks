#include "ofxLabThreadedImageLoader.h"
#include <sstream>
ofxLabThreadedImageLoader::ofxLabThreadedImageLoader() 
:ofThread()
{
	_num_loading = 0;
    ofAddListener(ofEvents().update, this, &ofxLabThreadedImageLoader::update);
	ofRegisterURLNotification(this);    
    
    startThread();
    _lastUpdate = 0;
    _errorCounter = 0;
}

// Load an image from disk.
//--------------------------------------------------------------
void ofxLabThreadedImageLoader::loadFromDisk(ofImage* image, string filename) {
    
	_bufferMutex.lock();
	
	_num_loading++;
	ofImageLoaderEntry entry(image, OF_LOAD_FROM_DISK);
	entry.filename = filename;
	entry.id = _num_loading;
	entry.image->setUseTexture(false);
	images_to_load_buffer.push_back(entry);

    _bufferMutex.unlock();
    
    _condition.signal();
    
    
}


// Load an url asynchronously from an url.
// * 5 bucks this doesn't work
//--------------------------------------------------------------
void ofxLabThreadedImageLoader::loadFromURL(ofImage* image, string url) {

    _bufferMutex.lock();
	
	_num_loading++;
	ofImageLoaderEntry entry(image, OF_LOAD_FROM_URL);
	entry.url = url;
	entry.id = _num_loading;
	entry.image->setUseTexture(false);	
	
	stringstream ss;
	ss << "image" << entry.id;
	entry.name = ss.str();

	images_to_load_buffer.push_back(entry);
	
    _bufferMutex.unlock();
    
    _condition.signal();
}


// Reads from the queue and loads new images.
//--------------------------------------------------------------
void ofxLabThreadedImageLoader::threadedFunction() {
    
    _errorCounter = 0;
    
	while(true) {
        
        try {
            
            lock();
            // wake every 1/2 second just in case we missed something
            //_condition.wait(this->mutex, 500);

            // above condition call crashes thread for unknown reasons,
            // resorting to a standard wait
            _condition.wait(this->mutex);

            // check on wait
            _bufferMutex.lock();
            images_to_load.insert( images_to_load.end(),
                                   images_to_load_buffer.begin(),
                                   images_to_load_buffer.end() );
            
            images_to_load_buffer.clear();
            _bufferMutex.unlock();
            
            while( shouldLoadImages() ) {
                ofImageLoaderEntry entry = getNextImageToLoad();
                if(entry.image == NULL) {
                    continue;
                }
            
                if(entry.type == OF_LOAD_FROM_DISK) {
                    if(! entry.image->loadImage(entry.filename) )  { 
                        stringstream ss;
                        ss << "ofxThreadedImageLoader error loading image " << entry.filename;
                        ofLog(OF_LOG_ERROR, ss.str() );
                    }
                    
                    images_to_update.push_back(entry);
                }
                else if(entry.type == OF_LOAD_FROM_URL) {
                    images_async_loading.push_back(entry);
                    ofLoadURLAsync(entry.url, entry.name);
                }
                
                // also check while looping
                _bufferMutex.lock();
                images_to_load.insert( images_to_load.end(),
                                       images_to_load_buffer.begin(),
                                       images_to_load_buffer.end() );
                
                images_to_load_buffer.clear();
                _bufferMutex.unlock();

            }
            
            unlock();
            _errorCounter = 0;
            
        } catch (exception& e) {
            stringstream ss;
            ss << "Exception caught in ofxLabThreadedImageLoader: " << e.what() << endl;
            ofLog( OF_LOG_ERROR, ss.str() );

            ++_errorCounter;
            // don't overload log
            sleep(_errorCounter * 1000);
            
            unlock();
        }
	}
}


// When we receive an url response this method is called; 
// The loaded image is removed from the async_queue and added to the
// update queue. The update queue is used to update the texture.
//--------------------------------------------------------------
void ofxLabThreadedImageLoader::urlResponse(ofHttpResponse & response) {
	if(response.status == 200) {
		lock();
		
		// Get the loaded url from the async queue and move it into the update queue.
		entry_iterator it = getEntryFromAsyncQueue(response.request.name);
		if(it != images_async_loading.end()) {
			(*it).image->loadImage(response.data);
            
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
ofxLabThreadedImageLoader::entry_iterator ofxLabThreadedImageLoader::getEntryFromAsyncQueue(string name) {
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
void ofxLabThreadedImageLoader::update(ofEventArgs & a){
    
    // TODO put a max on the number of images we copy over
    // so that we don't slow thngs down
    
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

    // get around the fact that our mutex won't time out
    if( ((int) ofGetElapsedTimef()) - _lastUpdate > 1 ) {
        _lastUpdate = ofGetElapsedTimef();
        _condition.signal();
    }
}


// Pick an entry from the queue with images for which the texture
// needs to be update.
//--------------------------------------------------------------
ofxLabThreadedImageLoader::ofImageLoaderEntry ofxLabThreadedImageLoader::getNextImageToUpdate() {
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
//   * private, no lock protection, called from area already locked
//--------------------------------------------------------------
ofxLabThreadedImageLoader::ofImageLoaderEntry ofxLabThreadedImageLoader::getNextImageToLoad() {
	
    ofImageLoaderEntry entry;
	if(images_to_load.size() > 0) {
		entry = images_to_load.front();
		images_to_load.pop_front();
	}
	return entry;
}

// Check if there are still images in the queue.
//--------------------------------------------------------------
bool ofxLabThreadedImageLoader::shouldLoadImages() {
	return (images_to_load.size() > 0);
}

