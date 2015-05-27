/*
 * ImageSaverThread.h
 *
 *  Created on: Oct 14, 2014
 *      Author: arturo
 */
#pragma once
#include "ofMain.h"
class ImageSaverThread: public ofThread{
public:
	ImageSaverThread();
	~ImageSaverThread();

	void save(unsigned char * pixels);
	void waitReady();
	void threadedFunction();

private:
	ofPixels pixels;
	ofThreadChannel<unsigned char *> channel;
	ofThreadChannel<bool> channelReady;
	bool firstFrame;
};
