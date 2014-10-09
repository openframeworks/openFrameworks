/*
 * ImgAnalysisThread.cpp
 *
 *  Created on: Oct 8, 2014
 *      Author: arturo
 */

#include "ImgAnalysisThread.h"
#include "ofConstants.h"

ImgAnalysisThread::ImgAnalysisThread()
:newFrame(true){
	// start the thread as soon as the
	// class is created, it won't use any CPU
	// until we send a new frame to be analyzed
	startThread();
}

ImgAnalysisThread::~ImgAnalysisThread(){
	// when the class is destroyed
	// close both channels and wait for
	// the thread to finish
	toAnalize.close();
	analized.close();
	waitForThread(true);
}

void ImgAnalysisThread::analize(ofPixels & pixels){
	// send the frame to the thread for analyzing
	// this makes a copy but we can't avoid it anyway if
	// we want to update the grabber while analyzing
	// previous frames
	toAnalize.send(pixels);
}

void ImgAnalysisThread::update(){
	// check if there's a new analyzed frame and upload
	// it to the texture. we use a while loop to drop any
	// extra frame in case the main thread is slower than
	// the analysis
	// tryReceive doesn't reallocate or make any copies
	newFrame = false;
	while(analized.tryReceive(pixels)){
		newFrame = true;
	}
	if(newFrame){
		texture.loadData(pixels);
	}
}

bool ImgAnalysisThread::isFrameNew(){
	return newFrame;
}

ofPixels & ImgAnalysisThread::getPixels(){
	return pixels;
}

ofTexture & ImgAnalysisThread::getTexture(){
	return texture;
}

void ImgAnalysisThread::draw(float x, float y){
	texture.draw(x,y);
}

void ImgAnalysisThread::draw(float x, float y, float w, float h){
	texture.draw(x,y,w,h);
}

void ImgAnalysisThread::threadedFunction(){
	while(isThreadRunning()){
		// wait until there's a new frame
		// this blocks the thread, so it doesn't use
		// the CPU at all, until a frame arrives.
		// also receive doesn't allocate or make any copies
		ofPixels pixels;
		if(toAnalize.receive(pixels)){
			// we have a new frame, process it, the analysis
			// here is just a thresholding for the sake of
			// simplicity
			pixels.setImageType(OF_IMAGE_GRAYSCALE);
			for(int i=0;i<pixels.size();i++){
				if(pixels[i]>80) pixels[i] = 255;
				else pixels[i] = 0;
			}

			// once processed send the result back to the
			// main thread. in c++11 we can move it to
			// avoid a copy
#if __cplusplus>=201103
			analized.send(std::move(pixels));
#else
			analized.send(pixels);
#endif
		}else{
			// if receive returns false the channel
			// has been closed, go out of the while loop
			// to end the thread
			break;
		}
	}
}

