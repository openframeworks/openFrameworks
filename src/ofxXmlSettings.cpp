#include "ofxXmlSettings.h"

#include <vector>
#include <string>
#include <iostream>

//----------------------------------------
// a pretty useful tokenization system:
static vector<string> tokenize(const string & str, const string & delim);
static vector<string> tokenize(const string & str, const string & delim)
{
  vector<string> tokens;

  size_t p0 = 0, p1 = string::npos;
  while(p0 != string::npos)
  {
    p1 = str.find_first_of(delim, p0);
    if(p1 != p0)
    {
      string token = str.substr(p0, p1 - p0);
      tokens.push_back(token);
    }
    p0 = str.find_first_not_of(delim, p1);
  }
  return tokens;
}
//----------------------------------------


ofxXmlSettings::ofxXmlSettings(){
	storedHandle	= new TiXmlHandle(NULL);
	level			= 0;
	//we do this so that we have a valid handle
	//without the need for loadFile
	*storedHandle   = TiXmlHandle(&doc);
}

//---------------------------------------------------------
void ofxXmlSettings::setVerbose(bool _verbose){
}

//---------------------------------------------------------
void ofxXmlSettings::clear(){
	//we clear from our root level
	//this is usually the document
	//but if we are pushed  - it could
	//be all the tags inside of the pushed
	//node - including the node itself!

	storedHandle->ToNode()->Clear();
}

//---------------------------------------------------------
bool ofxXmlSettings::loadFile(string xmlFile){

	xmlFile = ofToDataPath(xmlFile);

	bool loadOkay = doc.LoadFile(xmlFile.c_str());

	//theo removed bool check as it would
	//return false if the file exists but was
	//empty

    //our push pop level should be set to 0!
	level = 0;

	*storedHandle = TiXmlHandle(&doc);
	return loadOkay;
}

//---------------------------------------------------------
void ofxXmlSettings::saveFile(string xmlFile){

	xmlFile = ofToDataPath(xmlFile);
	doc.SaveFile(xmlFile.c_str());
}

//---------------------------------------------------------
void ofxXmlSettings::clearTagContents(string tag, int which){
	//we check it first to see if it exists
	//otherwise setValue will make a new empty tag
	if( tagExists(tag, which) )setValue(tag, "", which);
}

//---------------------------------------------------------
void ofxXmlSettings::removeTag(string  tag, int which){

	vector<string> tokens = tokenize(tag,":");

	//no tags so we return
	if( tokens.size() == 0 ) return;

	//grab the handle from the level we are at
	//normally this is the doc but could be a pushed node
	TiXmlHandle tagHandle = *storedHandle;

	if(which < 0) which = 0;

	for(int x=0;x<(int)tokens.size();x++){

		//we only support multi tags
		//with same name at root level
		if(x > 0) which = 0;

		TiXmlHandle isRealHandle = tagHandle.ChildElement( tokens.at(x), which);

		if ( !isRealHandle.Node() ) break;
		else{
			if (x == (int)tokens.size()-1){
				//if we are at the last tag and it exists
				//we use its parent to remove it - haha
				tagHandle.ToNode()->RemoveChild( isRealHandle.ToNode() );
			}
			tagHandle = isRealHandle;
		}
	}

	tokens.clear();
}

//---------------------------------------------------------
int ofxXmlSettings::getValue(string tag, int defaultValue, int which){
	char * tempStr = new char[MAX_TAG_VALUE_LENGTH_IN_CHARS];
	memset(tempStr, 0, MAX_TAG_VALUE_LENGTH_IN_CHARS);
	int returnValue = defaultValue;

	if (readTag(tag, tempStr, which)){
		returnValue = strtol(tempStr, NULL, 0);
	}
	delete tempStr;
	return returnValue;
}

//---------------------------------------------------------
float ofxXmlSettings::getValue(string tag, double defaultValue, int which){
	char * tempStr = new char[MAX_TAG_VALUE_LENGTH_IN_CHARS];
	memset(tempStr, 0, MAX_TAG_VALUE_LENGTH_IN_CHARS);
	float returnValue = defaultValue;

	if (readTag(tag, tempStr, which)){
		returnValue = strtod(tempStr,  NULL);
	}
	delete tempStr;
	return returnValue;
}

//---------------------------------------------------------
string ofxXmlSettings::getValue(string tag, string defaultValue, int which){

	// lots of char *, string kung-fu here...

	char * tempStr = new char[MAX_TAG_VALUE_LENGTH_IN_CHARS];
	memset(tempStr, 0, MAX_TAG_VALUE_LENGTH_IN_CHARS);
	char * returnPtr = (char *) defaultValue.c_str();
	if (readTag(tag, tempStr, which)){
		returnPtr = tempStr;
	}
	string returnString(returnPtr);
	delete tempStr;
	return returnString;
}

//---------------------------------------------------------
bool ofxXmlSettings::readTag(string  tag, char * valueString, int which){

	vector<string> tokens = tokenize(tag,":");

	TiXmlHandle tagHandle = *storedHandle;
	for(int x=0;x<(int)tokens.size();x++){
		if(x == 0)tagHandle = tagHandle.ChildElement(tokens.at(x), which);
		else tagHandle = tagHandle.FirstChildElement( tokens.at(x) );
	}

	// once we've walked, let's get that value...
	TiXmlHandle valHandle = tagHandle.Child( 0 );

    //now, clear that vector!
	tokens.clear();

    // if that value is really text, let's get the value out of it !
    if (valHandle.Text()){
    	int maxLen = MIN(MAX_TAG_VALUE_LENGTH_IN_CHARS, strlen(valHandle.Text()->Value()));
    	memcpy(valueString, valHandle.Text()->Value(), maxLen);
    	return true;
    }  else {
		return false;
	}
}


//---------------------------------------------------------
bool ofxXmlSettings::pushTag(string  tag, int which){

	int pos = tag.find(":");

	if(pos > 0){
		tag = tag.substr(0,pos);
	}

	//we only allow to push one tag at a time.
	TiXmlHandle isRealHandle = storedHandle->ChildElement(tag, which);

	if( isRealHandle.Node() ){
		*storedHandle = isRealHandle;
		level++;
		return true;
	}else{
		printf("pushTag - tag not found\n");
	}

	return false;
}

//---------------------------------------------------------
int ofxXmlSettings::popTag(){

	if(level >= 1){
		TiXmlHandle parent( (storedHandle->ToNode() )->Parent() );
		*storedHandle = parent;
		level--;
	}else{
		*storedHandle = TiXmlHandle(&doc);
		level = 0;
	}

	return level;
}

//---------------------------------------------------------
int ofxXmlSettings::getPushLevel(){
	return level;
}

//---------------------------------------------------------
bool ofxXmlSettings::tagExists(string  tag, int which){

	vector<string> tokens = tokenize(tag,":");

	bool found = false;

	//grab the handle from the level we are at
	//normally this is the doc but could be a pushed node
	TiXmlHandle tagHandle = *storedHandle;

	if(which < 0) which = 0;

	for(int x=0;x<(int)tokens.size();x++){

		//we only support multi tags
		//with same name at root level
		if(x > 0) which = 0;

		TiXmlHandle isRealHandle = tagHandle.ChildElement( tokens.at(x), which);

		//as soon as we find a tag that doesn't exist
		//we return false;
		if ( !isRealHandle.Node() ){
			found = false;
			break;
		}
		else{
			found = true;
			tagHandle = isRealHandle;
		}
	}

	tokens.clear();

	return found;
}


//---------------------------------------------------------
int ofxXmlSettings::getNumTags(string  tag){
	//this only works for tags at the current root level

	int pos = tag.find(":");

	if(pos > 0){
		tag = tag.substr(0,pos);
	}

	//grab the handle from the level we are at
	//normally this is the doc but could be a pushed node
	TiXmlHandle tagHandle = *storedHandle;

	int count = 0;

	//ripped from tinyXML as doing this ourselves once is a LOT! faster
	//than having this called n number of times in a while loop - we go from n*n iterations to n iterations

	TiXmlElement* child = ( storedHandle->FirstChildElement( tag ) ).Element();
	for (count = 0; child; child = child->NextSiblingElement( tag ), ++count){
		//nothing
	}

	return count;
}



//---------------------------------------------------------
int ofxXmlSettings::writeTag(string  tag, char * valueStr, int which){

	vector<string> tokens = tokenize(tag,":");

	// allocate then clean up :
	TiXmlElement ** elements = new TiXmlElement*[tokens.size()];
	for(int x=0;x<(int)tokens.size();x++){
		elements[x] = new TiXmlElement(tokens.at(x));
	}

	TiXmlText Value(valueStr);

	// search our way up - do these tags exist?
	// find the first that DOESNT exist, then move backwards...
	TiXmlHandle tagHandle = *storedHandle;

	bool addNewTag = false;
	if(which == -1)addNewTag = true;

	for(int x=0;x<(int)tokens.size();x++){

		if( x > 0 ){
			//multi tags of same name
			//only for the root level
			which = 0;
			addNewTag = false;
		}

		TiXmlHandle isRealHandle = tagHandle.ChildElement( tokens.at(x), which);

		if ( !isRealHandle.Node() ||  addNewTag){

			for(int i=tokens.size()-1;i>=x;i--){
				if (i == (int)tokens.size()-1){
					elements[i]->InsertEndChild(Value);
				} else {
					elements[i]->InsertEndChild(*(elements[i+1]));
				}
			}

			tagHandle.ToNode()->InsertEndChild(*(elements[x]));

			break;

		} else {
			 tagHandle = isRealHandle;
			 if (x == (int)tokens.size()-1){
				// what we want to change : TiXmlHandle valHandle = tagHandle.Child( 0 );
				tagHandle.ToNode()->Clear();
				tagHandle.ToNode()->InsertEndChild(Value);
			}
		}
	}


	//lets count how many tags with our name exist so we can return an index

	//ripped from tinyXML as doing this ourselves once is a LOT! faster
	//than having this called n number of times in a while loop - we go from n*n iterations to n iterations
	int numSameTags;
	TiXmlElement* child = ( storedHandle->FirstChildElement( tokens.at(0) ) ).Element();
	for (numSameTags = 0; child; child = child->NextSiblingElement( tokens.at(0) ), ++numSameTags){
		//nothing
	}

	//now, clear that vector!
	tokens.clear();

	return numSameTags;
}

//---------------------------------------------------------
int ofxXmlSettings::setValue(string tag, int value, int which){
	char valueStr[255];
	sprintf(valueStr, "%i", value);
	int tagID = writeTag(tag, valueStr, which) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::setValue(string  tag, double value, int which){
	char valueStr[255];
	sprintf(valueStr, "%f", value);
	int tagID = writeTag(tag, valueStr, which) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::setValue(string tag, string value, int which){
	int tagID = writeTag(tag, (char *)value.c_str(), which) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::addValue(string tag, int value){
	char valueStr[255];
	sprintf(valueStr, "%i", value);
	int tagID = writeTag(tag, valueStr, -1) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::addValue(string  tag, double value){
	char valueStr[255];
	sprintf(valueStr, "%f", value);
	int tagID = writeTag(tag, valueStr, -1) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::addValue(string tag, string value){
	int tagID = writeTag(tag, (char *)value.c_str(), -1) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::addTag(string tag){
	int tagID = writeTag(tag, "", -1) -1;
	return tagID;
}
