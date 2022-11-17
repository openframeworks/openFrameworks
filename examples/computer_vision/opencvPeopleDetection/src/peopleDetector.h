//
//  peopleDetector.h
//  opencvExample
//
//  Created by Theo on 9/22/22.
//
//based on https://docs.opencv.org/4.x/df/d54/samples_2cpp_2peopledetect_8cpp-example.html

#pragma once

#include "ofxOpenCv.h"

#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

class peopleDetector{
		
	public:

		//-----------------------------------------------------------------------------------------
		void setup(){
			if(!bSetup){
				hog.setSVMDetector( cv::HOGDescriptor::getDefaultPeopleDetector());
				bSetup = true;
			}
		}
		
		//for better speed / performance you can set a smaller size to resize the image to before running the detection
		//ie: If 320 - and you have a 1920 x 1080 video it will automatically resize the input image down to 320 x 284
		//-----------------------------------------------------------------------------------------
		void setResizeWidth(int aWidth){
			resizeW = aWidth;
		}
		
		//this should be the minimum dimensions a person could be in the input image
		//needs to be a multiple of 8
		//-----------------------------------------------------------------------------------------
		void setMinPersonSize(int width, int height){
			if( !bSetup ){
				setup();
			}
			
			if( width % 8 != 0 ){
				width = width / 8;
				width = width * 8;
			}
			if( height % 8 != 0 ){
				height = height / 8;
				height = height * 8;
			}
			
			hog.winSize = cv::Size(width, height);
		}
		
		//-----------------------------------------------------------------------------------------
		vector<ofRectangle> detect(cv::Mat img){
			if( !bSetup ){
				setup();
			}
			
			float imageScale = 1.0;
			if( resizeW > 0 ){
				if( resizeH == 0 ){
					//find the corresponding height for the input image
					resizeH = ((float)resizeW) * ((float)img.rows) / (float)img.cols;
					cout << "making smaller image of size: " << resizeW << "x" << resizeH << endl;
				}

				imageScale = (float)img.cols/(float)resizeW;
				cv::resize(img, img, cv::Size(resizeW, resizeH));
			}
			
			// Run the detector with default parameters. to get a higher hit-rate
			// (and more false alarms, respectively), decrease the hitThreshold and
			// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
			vector<cv::Rect> found;
			hog.detectMultiScale(img, found, 0, cv::Size(4,4), cv::Size(), 1.05, 2, true);
			
			//now resize up the rects so we are back to the input scale
			vector <ofRectangle> scaledRects;
			for( auto & rect : found ){
				rect = cv::Rect(rect.x*imageScale, rect.y*imageScale, rect.width*imageScale, rect.height*imageScale);
				
				//make them OF friendly
				scaledRects.push_back(ofRectangle(rect.x, rect.y, rect.width, rect.height));
			}
			
			return scaledRects;
		}
		
		
	protected:
		enum Mode { Default, Daimler } m;
		cv::HOGDescriptor hog;
		bool bSetup = false;
		int resizeW, resizeH = 0;
};
