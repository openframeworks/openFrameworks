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
        ofxXmlSettings(const string& xmlFile);

        ~ofxXmlSettings();

		void setVerbose(bool _verbose);

		bool loadFile(const string& xmlFile);
		bool saveFile(const string& xmlFile);
		bool saveFile();

		void clearTagContents(const string& tag, int which = 0);
		void removeTag(const string& tag, int which = 0);

		bool tagExists(const string& tag, int which = 0);

		// removes all tags from within either the whole document
		// or the tag you are currently at using pushTag
		void	clear();

		int 	getValue(const string&  tag, int            defaultValue, int which = 0);
		double 	getValue(const string&  tag, double         defaultValue, int which = 0);
		string 	getValue(const string&  tag, const string& 	defaultValue, int which = 0);

		int 	setValue(const string&  tag, int            value, int which = 0);
		int 	setValue(const string&  tag, double         value, int which = 0);
		int 	setValue(const string&  tag, const string& 	value, int which = 0);

		//advanced

		//-- pushTag/popTag
		//pushing a tag moves you inside it which has the effect of
		//temporarily treating the tag you are in as the document root
		//all setValue, readValue and getValue commands are then be relative to the tag you pushed.
		//this can be used with addValue to create multiple tags of the same name within
		//the pushed tag - normally addValue only lets you create multiple tags of the same
		//at the top most level.

		bool	pushTag(const string&  tag, int which = 0);
		int		popTag();
		int		getPushLevel();

		//-- numTags
		//this only works for tags at the current root level
		//use pushTag and popTag to get number of tags whithin other tags
		// both getNumTags("PT"); and getNumTags("PT:X"); will just return the
		//number of <PT> tags at the current root level.
		int		getNumTags(const string& tag);

		//-- addValue/addTag
		//adds a tag to the document even if a tag with the same name
		//already exists - returns an index which can then be used to
		//modify the tag by passing it as the last argument to setValue

		//-- important - this only works for top level tags
		//   to put multiple tags inside other tags - use pushTag() and popTag()

		int 	addValue(const string&  tag, int            value);
		int 	addValue(const string&  tag, double         value);
		int 	addValue(const string&  tag, const string& 	value);

		int		addTag(const string& tag); //adds an empty tag at the current level


        // Attribute-related methods
		int		addAttribute(const string& tag, const string& attribute, int value, int which = 0);
		int		addAttribute(const string& tag, const string& attribute, double value, int which = 0);
		int		addAttribute(const string& tag, const string& attribute, const string& value, int which = 0);

		int		addAttribute(const string& tag, const string& attribute, int value);
		int		addAttribute(const string& tag, const string& attribute, double value);
		int		addAttribute(const string& tag, const string& attribute, const string& value);

		void	removeAttribute(const string& tag, const string& attribute, int which = 0);
		void	clearTagAttributes(const string& tag, int which = 0);

		int		getNumAttributes(const string& tag, int which = 0);

		bool	attributeExists(const string& tag, const string& attribute, int which = 0);

		bool    getAttributeNames(const string& tag, vector<string>& outNames, int which = 0);

		int		getAttribute(const string& tag, const string& attribute, int defaultValue, int which = 0);
		double	getAttribute(const string& tag, const string& attribute, double defaultValue, int which = 0);
		string	getAttribute(const string& tag, const string& attribute, const string& defaultValue, int which = 0);

		int		setAttribute(const string& tag, const string& attribute, int value, int which = 0);
		int		setAttribute(const string& tag, const string& attribute, double value, int which = 0);
		int		setAttribute(const string& tag, const string& attribute, const string& value, int which = 0);

		int		setAttribute(const string& tag, const string& attribute, int value);
		int		setAttribute(const string& tag, const string& attribute, double value);
		int		setAttribute(const string& tag, const string& attribute, const string& value);

		bool	loadFromBuffer( string buffer );
		void	copyXmlToString(string & str);

		TiXmlDocument 	doc;
		bool 			bDocLoaded;

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

