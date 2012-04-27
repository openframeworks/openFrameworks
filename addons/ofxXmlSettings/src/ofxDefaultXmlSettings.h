/*
 *  ofxDefaultXmlSettings.h
 *  Created by Paul Vollmer, http://www.wng.cc
 *  
 *  Copyright (c) 2012 openFrameworks. All rights reserved.
 *  
 *  The MIT License
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *
 *
 *  @modified 2012.04.26
 *  @version  0.1.0
 */

#include "ofxXmlSettings.h"

	
/**
 * This class is an extension for ofxXmlSettings to create a
 * default settings xml file. At this file we create
 * openFrameworks placeholder to store core variables like:
 * - window position,
 * - window size,
 * - framerate,
 * - fullscreen,
 * - cursor
 * Also you can use this xml to store other variables.
 */
class ofxDefaultXmlSettings : public ofxXmlSettings {
	
		
	public:
		ofxDefaultXmlSettings();
		
		
		void init(string filename);
		void init();
		bool load();
		bool save();
		void setSettings();
		void getSettings();
		
		string filename;
		
		
	private:
		void createDefaultXml();
		
};
