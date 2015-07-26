#pragma once

#include "ofMain.h"
#include <string.h>
#if (_MSC_VER)
#include "../libs/tinyxml.h"
#else
#include "tinyxml.h"
#endif

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

OF_DEPRECATED_MSG("ofXml is going to replace ofxXmlSetting", class ofxXmlSettings: public ofBaseFileSerializer{

	public:
        ofxXmlSettings();
        ofxXmlSettings(const string& xmlFile);

        ~ofxXmlSettings();

		OF_DEPRECATED_MSG("ofXml is going to replace ofxXmlSettings, but has no matching method.", void setVerbose(bool _verbose) );

		OF_DEPRECATED_MSG("Use ofXml::load().", bool loadFile(const string& xmlFile) );
		OF_DEPRECATED_MSG("Use ofXml::save().", bool saveFile(const string& xmlFile) );
		OF_DEPRECATED_MSG("Use ofXml::save().", bool saveFile() );

		OF_DEPRECATED_MSG("Use ofXml::load().", bool load(const string & path) );
		OF_DEPRECATED_MSG("Use ofXml::save().", bool save(const string & path) );

		OF_DEPRECATED_MSG("Use ofXml::removeContents().", void clearTagContents(const string& tag, int which = 0) );
		OF_DEPRECATED_MSG("Use ofXml::removeContents().", void removeTag(const string& tag, int which = 0) );

		OF_DEPRECATED_MSG("Use ofXml::exists().", bool tagExists(const string& tag, int which = 0) );

		// removes all tags from within either the whole document
		// or the tag you are currently at using pushTag
		OF_DEPRECATED_MSG("Use ofXml::clear().", void clear() );

		OF_DEPRECATED_MSG("Use ofXml::getIntValue().", int getValue(const string& tag, int defaultValue, int which = 0) );
		OF_DEPRECATED_MSG("Use ofXml::getFloatValue().", double getValue(const string& tag, double defaultValue, int which = 0) );
		OF_DEPRECATED_MSG("Use ofXml::getValue().", string getValue(const string& tag, const string& defaultValue, int which = 0) );

		OF_DEPRECATED_MSG("Use ofXml::setValue().", int setValue(const string& tag, int value, int which = 0) );
		OF_DEPRECATED_MSG("Use ofXml::setValue().", int setValue(const string& tag, double value, int which = 0) );
		OF_DEPRECATED_MSG("Use ofXml::setValue().", int setValue(const string& tag, const string& value, int which = 0) );

		//advanced

		//-- pushTag/popTag
		//pushing a tag moves you inside it which has the effect of
		//temporarily treating the tag you are in as the document root
		//all setValue, readValue and getValue commands are then be relative to the tag you pushed.
		//this can be used with addValue to create multiple tags of the same name within
		//the pushed tag - normally addValue only lets you create multiple tags of the same
		//at the top most level.

		OF_DEPRECATED_MSG("Use ofXml::setTo().", bool pushTag(const string& tag, int which = 0) );
		OF_DEPRECATED_MSG("Use ofXml::setTo().", int popTag() );
		OF_DEPRECATED_MSG("Use ofXml::setTo().", int getPushLevel() );

		//-- numTags
		//this only works for tags at the current root level
		//use pushTag and popTag to get number of tags whithin other tags
		// both getNumTags("PT"); and getNumTags("PT:X"); will just return the
		//number of <PT> tags at the current root level.
		OF_DEPRECATED_MSG("Use ofXml::getNumChildren().", int getNumTags(const string& tag) );

		//-- addValue/addTag
		//adds a tag to the document even if a tag with the same name
		//already exists - returns an index which can then be used to
		//modify the tag by passing it as the last argument to setValue

		//-- important - this only works for top level tags
		//   to put multiple tags inside other tags - use pushTag() and popTag()

		OF_DEPRECATED_MSG("Use ofXml::addValue()", int addValue(const string&  tag, int value) );
		OF_DEPRECATED_MSG("Use ofXml::addValue()", int addValue(const string&  tag, double value) );
		OF_DEPRECATED_MSG("Use ofXml::addValue()", int addValue(const string&  tag, const string&	value) );

		OF_DEPRECATED_MSG("Use ofXml::setToParent() and ofXml::addChild()",int addTag(const string& tag) ); //adds an empty tag at the current level

		OF_DEPRECATED_MSG("Use ofXml::serialize()", void serialize(const ofAbstractParameter & parameter) );
		OF_DEPRECATED_MSG("Use ofXml::deserialize()", void deserialize(ofAbstractParameter & parameter) );


        // Attribute-related methods
		OF_DEPRECATED_MSG("Use ofXml::addAttribute()", int addAttribute(const string& tag, const string& attribute, int value, int which = 0) );
		OF_DEPRECATED_MSG("Use ofXml::addAttribute()", int addAttribute(const string& tag, const string& attribute, double value, int which = 0) );
		OF_DEPRECATED_MSG("Use ofXml::addAttribute()", int addAttribute(const string& tag, const string& attribute, const string& value, int which = 0) );

		OF_DEPRECATED_MSG("Use ofXml::addAttribute()", int addAttribute(const string& tag, const string& attribute, int value) );
		OF_DEPRECATED_MSG("Use ofXml::addAttribute()", int addAttribute(const string& tag, const string& attribute, double value) );
		OF_DEPRECATED_MSG("Use ofXml::addAttribute()", int addAttribute(const string& tag, const string& attribute, const string& value) );

		OF_DEPRECATED_MSG("Use ofXml::removeAttribute()", void removeAttribute(const string& tag, const string& attribute, int which = 0) );
		OF_DEPRECATED_MSG("Use ofXml::removeAttribute()", void clearTagAttributes(const string& tag, int which = 0) );

		OF_DEPRECATED_MSG("Use ofXml::getAttributes()", int getNumAttributes(const string& tag, int which = 0) );

		OF_DEPRECATED_MSG("ofXml is going to replace ofxXmlSettings, maybe ofXml::getAttribute()?", bool attributeExists(const string& tag, const string& attribute, int which = 0) );

		OF_DEPRECATED_MSG("Use ofXml::getAttributes()", bool getAttributeNames(const string& tag, vector<string>& outNames, int which = 0) );

		OF_DEPRECATED_MSG("Use ofXml::getAttribute()", int getAttribute(const string& tag, const string& attribute, int defaultValue, int which = 0) );
		OF_DEPRECATED_MSG("Use ofXml::getAttribute()", double getAttribute(const string& tag, const string& attribute, double defaultValue, int which = 0) );
		OF_DEPRECATED_MSG("Use ofXml::getAttribute()", string getAttribute(const string& tag, const string& attribute, const string& defaultValue, int which = 0) );

		OF_DEPRECATED_MSG("Use ofXml::setAttribute()", int setAttribute(const string& tag, const string& attribute, int value, int which = 0) );
		OF_DEPRECATED_MSG("Use ofXml::setAttribute()", int setAttribute(const string& tag, const string& attribute, double value, int which = 0) );
		OF_DEPRECATED_MSG("Use ofXml::setAttribute()", int setAttribute(const string& tag, const string& attribute, const string& value, int which = 0) );

		OF_DEPRECATED_MSG("Use ofXml::setAttribute()", int setAttribute(const string& tag, const string& attribute, int value) );
		OF_DEPRECATED_MSG("Use ofXml::setAttribute()", int setAttribute(const string& tag, const string& attribute, double value) );
		OF_DEPRECATED_MSG("Use ofXml::setAttribute()", int setAttribute(const string& tag, const string& attribute, const string& value) );

		OF_DEPRECATED_MSG("Use ofXml::loadFromBuffer()", bool loadFromBuffer( string buffer ) );
		OF_DEPRECATED_MSG("Use ofXml::toString()", void	copyXmlToString(string & str) );

		OF_DEPRECATED_MSG("ofXml is going to replace ofxXmlSettings, but has no matching attribute.", TiXmlDocument doc );
		OF_DEPRECATED_MSG("ofXml is going to replace ofxXmlSettings, but has no matching attribute.", bool bDocLoaded );

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
);
