#pragma once

#include "ofMain.h"
#include "Poco/File.h"

class ofDirectoryLister{
	public:	
		ofDirectoryLister();

		void allowExt(string extension);
		int listDir(string directory, bool absolute = false);
		
		string getName(unsigned int position); // e.g., "image.png"
		string getPath(unsigned int position);
		
		void reset();
		void sort();
		
		unsigned int size();
		
		int numFiles(); // numFiles is deprecated, use size()
		
	private:
		string originalDirectory;
		vector <string> extensions;
		vector <Poco::File> files;
		bool showHidden;
};