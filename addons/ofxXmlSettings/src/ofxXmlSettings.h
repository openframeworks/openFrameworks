#ifndef __ofxXmlSettings__
#define __ofxXmlSettings__

#include "ofMain.h"
#include <string.h>
#include "tinyxml.h"

using namespace std;

/*
	Q: what is the which = 0 argument?

	A: Glad you asked - most of the time you can ignore this and treat it as if it weren't there
	   But if specified it selects the nth tag with the same tag name at the current root of the document
	   Normally this just means the top level tags in the document - but if you use the pushTag and popTag
	   you can temporarily set the root of the document to be that specified tag.
	   The main idea is to allow you to have multiple tags with the same name.

	   So here is an example without pushTag

	   <time>102229</time>  <-- which = 0
	   <time>298292</time>  <-- which = 1
	   <time>393393</time>  <-- which = 2
	   <time>447373</time>  <-- which = 3

		But if we wanted to group these into multiple <recording> tags and have multiple time values inside
		we can use push and pop to move into the recording tags as if they were the document root

	   <recording> <-- we temporarily push into here with pushTag("recording", 0);
			<time>19222</time>  <-- to set this we call setValue("time", 19222, 0);   ( which = 0 )
			<time>23232</time>  <-- to set this we call setValue("time", 23232, 1);   ( which = 1 )
	   </recording> <-- we pop back out here with popTag();

		<recording>  <-- we temporarily push into here with pushTag("recording", 1); <-- now we use 1 to select the 2nd recording tag
			<time>33342</time>  <-- setValue("time", 33342, 0);   ( which = 0 )
			<time>22722</time>  <-- setValue("time", 22722, 0);   ( which = 1 )
	   </recording>

*/


#define MAX_TAG_VALUE_LENGTH_IN_CHARS		1024

class ofxXmlSettings{

	public:
        ofxXmlSettings();
        bool load(const string& filename);
	 

	protected:

		TiXmlHandle     storedHandle;
		int             level;


		int 	writeTag(const string&  tag, const string& valueString, int which = 0);
		bool 	readTag(const string&  tag, TiXmlHandle& valHandle, int which = 0);	// max 1024 chars...


		int		writeAttribute(const string& tag, const string& attribute, const string& valueString, int which = 0);

        TiXmlElement* getElementForAttribute(const string& tag, int which);
        bool readIntAttribute(const string& tag, const string& attribute, int& valueString, int which);
        bool readDoubleAttribute(const string& tag, const string& attribute, double& outValue, int which);
        bool readStringAttribute(const string& tag, const string& attribute, string& outValue, int which);
};

#endif

