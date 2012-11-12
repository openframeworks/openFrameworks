#include "ofxXmlSettings.h"

#include <vector>
#include <string>
#include <iostream>

// this increases the accuracy of ofToString() when saving floating point values
// but in the process of setting it also causes very small values to be ignored.
const float floatPrecision = 9;

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

//----------------------------------------
ofxXmlSettings::ofxXmlSettings()
{
	level			= 0;    
}

//----------------------------------------
ofxXmlSettings::ofxXmlSettings(const string& xmlFile)
{
	level			= 0;
	loadFile(xmlFile);
}

//---------------------------------------------------------
ofxXmlSettings::~ofxXmlSettings()
{
}

//---------------------------------------------------------
void ofxXmlSettings::setVerbose(bool _verbose){
}

//---------------------------------------------------------
void ofxXmlSettings::clear(){
    
    //currentElement->release();
}

//---------------------------------------------------------
bool ofxXmlSettings::loadFile(const string& xmlFile){

	cachedFilename = ofToDataPath(xmlFile);

	bool loadOkay = true;// = doc.LoadFile(fullXmlFile);
    
    file.open(cachedFilename); 
    ofBuffer buff = file.readToBuffer();
    
    string xml = buff.getText();
    
    documentElement.loadFromBuffer(xml);
    
	//theo removed bool check as it would
	//return false if the file exists but was
	//empty

    //our push pop level should be set to 0!
	level = 0;
    currentElement = documentElement; // now we're at the root

    file.close();
    
	return loadOkay;
}

//---------------------------------------------------------
bool ofxXmlSettings::saveFile(const string& xmlFile){

	string fullXmlFile = ofToDataPath(xmlFile);
    string xmlString = documentElement.toString();
    
    file.open(xmlFile, ofFile::ReadWrite);
    
    if(!file.exists()) {
        
        if(!file.create()) {
            ofLog(OF_LOG_ERROR, " can't create xml file " );
            return false;
        }
    }
    
    // don't like this so much
    ofBuffer buffer(xmlString.c_str(), xmlString.size());
    
    if(!file.writeFromBuffer(buffer)) {
        ofLog(OF_LOG_ERROR, " can't write to xml file " );
        return false;
    }
    
    file.close();
    
	return true;
}

//---------------------------------------------------------
bool ofxXmlSettings::saveFile(){
	//return doc.SaveFile();
    
    string xmlString = currentElement.toString();
    
    ofBuffer buffer(xmlString.c_str(), xmlString.size());
    
    file.open(cachedFilename, ofFile::ReadWrite);
    
    if(!file.writeFromBuffer(buffer)) {
        ofLog(OF_LOG_ERROR, " can't write to xml file " );
        return false;
    }
    
    file.close(); // do we just keep the file open?
    return true;

}

//---------------------------------------------------------
void ofxXmlSettings::clearTagContents(const string& tag, int which){

    //NodeList *list = currentElement->getElementsByTagName(tag);
    //list->item(which)->removeChild(list->item(which));
    
    ofXml child = currentElement.getChild(tag);
    currentElement.clearContents();
    
}

//---------------------------------------------------------
void ofxXmlSettings::removeTag(const string& tag, int which){
    
    ofXml element = getElement(tag, which);
    if(!element.isEmpty()) {
        element.remove(tag);
    }
}

//---------------------------------------------------------
int ofxXmlSettings::getValue(const string& tag, int defaultValue, int which){

    ofXml element = getElement(tag, which);
    if(!element.isEmpty()) {
        return ofToInt(element.getValue());
    }
    return defaultValue;
    
}

//---------------------------------------------------------
double ofxXmlSettings::getValue(const string& tag, double defaultValue, int which){
    
    // is it a path using either tinyXML : for legacy or Poco / for the present
    if (tag.find(':') != string::npos || tag.find('/') != string::npos){
		ofXml element = getElement(tag, which);
        if (!element.isEmpty()){
            return ofToFloat(element.getValue());
        }
	}
    return defaultValue;
    
}

//---------------------------------------------------------
string ofxXmlSettings::getValue(const string& tag, const string& defaultValue, int which){
 
    ofXml child = getElement(tag, which, true);
    return child.getValue();
    return defaultValue;
}

//---------------------------------------------------------
bool ofxXmlSettings::pushTag(const string&  tag, int which){
    
    int tagCount = 1;
    
    // if there's pre0072 style ':' (not proper xpath but is how tinyXml works) get rid of them
    if(tag.find(":") != string::npos) {
        tagCount = count( tag.begin(), tag.end(), '/') + 1;
    } else if(tag.find("/") != string::npos) {
        tagCount = count( tag.begin(), tag.end(), '/') + 1;
    }
    
    string copy = tag;
    
    if(copy.find(":") != string::npos) {
        replace( copy.begin(), copy.end(), ':', '/');
    }
    
    //if(which != 0) {
    copy += "[" + ofToString(which) + "]";
    //}

    // so we can bail from errors
    ofXml cache = currentElement;
    
    
    //cout << currentElement.getElement()->nodeName() << " " << currentElement.getElement()->getElementsByTagName("PT")->length() << endl;
    currentElement = currentElement.getChild(copy);
    //cout << currentElement.getElement()->nodeName() << " " << currentElement.getElement()->getElementsByTagName("PT")->length() << endl;
    
    if(!currentElement.isEmpty()) {
        level += tagCount;
        return true;
    } else {
        currentElement = cache; // back out safely
        ofLog( OF_LOG_ERROR, "pushTag - <" + tag + "> tag not found");
	}
    
	return false;
    
}

//---------------------------------------------------------
int ofxXmlSettings::popTag()
{
    
    if(currentElement.hasParent()) {
        currentElement = currentElement.getParent(); // we already know it's an element, so this is oK
        level--;
    } else {
        level = 0;
    }
    
    return level;
    
}

int     ofxXmlSettings::addTag( const string& tag) {
    currentElement.addValue(tag, "");
    return getSiblingCount(currentElement, tag);
}

//---------------------------------------------------------
int ofxXmlSettings::getPushLevel(){
	return level;
}

//---------------------------------------------------------
bool ofxXmlSettings::tagExists(const string& tag, int which){
    
    string copy = tag;
    
    if(copy.find(":") != string::npos) {
        replace( copy.begin(), copy.end(), ':', '/');
    }
    
    if(which != 0) {
        copy += "[" + ofToString(which) + "]";
    }

    return currentElement.exists(copy);
}


//---------------------------------------------------------
int ofxXmlSettings::getNumTags(const string&  tag){
    
    string copy = tag;
    
    if(copy.find(":") != string::npos) {
        replace( copy.begin(), copy.end(), ':', '/');
    }
    
    NodeList *list = currentElement.getElement()->getElementsByTagName(copy);
    if(list) {
        int len = list->length();
        cout << "getNumTags() " << len << endl;
        list->release();
        return len;
    }
    return 0;
    //int len = currentElement.getElement()->getElementsByTagName(copy)->length();
    //cout << "getNumTags() " << len << endl;
    //return len;
}



//---------------------------------------------------------
int ofxXmlSettings::writeTag(const string& tag, const string& valueStr, int which)
{
    
    string copy = tag;
    if(which != 0) {
        copy += "[" + ofToString(which) + "]";
    }
    currentElement.setValue(copy, valueStr);
    return getSiblingCount(currentElement, tag);
}

//---------------------------------------------------------
int ofxXmlSettings::setValue(const string& tag, int value, int which){
    
    // this is the current way that the implementation is done, which seems wrong
    return writeTag(tag, ofToString(value), which);
}

//---------------------------------------------------------
int ofxXmlSettings::setValue(const string& tag, double value, int which){
    
    // this is the current way that the implementation is done, which seems wrong
    return writeTag(tag, ofToString(value), which);
	//int tagID = writeTag(tag, ofToString(value, floatPrecision).c_str(), which) -1;
	//return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::setValue(const string& tag, const string& value, int which){
        
    // this is the current way that the implementation is done, which seems wrong
    return writeTag(tag, value, which);
}

int ofxXmlSettings::getSiblingCount(ofXml element, const string tag) 
{
    vector<string> tokens;
    bool needTokenize = false;
    if(tag.find(':') != string::npos) {
        tokens = tokenize(tag, ":");
        needTokenize = true;
    } else if(tag.find('/') != string::npos) {
        tokens = tokenize(tag, "/");
        needTokenize = true;
    } 
    
    ofXml parent = currentElement.getParent();
    if(needTokenize) {
        NodeList *list = parent.getElement()->getElementsByTagName(tokens[tokens.size()-1]);
        int len = list->length();
        list->release();
        return len; // return just the same tags
    } else {
        NodeList *list = parent.getElement()->getElementsByTagName(tag);
        int len = list->length();
        list->release();
        return len; // return just the same tags
    }
}

//---------------------------------------------------------
int ofxXmlSettings::addValue(const string& tag, int value){
    currentElement.addValue(tag, ofToString(value));
    return getSiblingCount(currentElement, tag);
}

//---------------------------------------------------------
int ofxXmlSettings::addValue(const string&  tag, double value){
    //currentElement.addValue(tag, ofToString(value));
    //return getSiblingCount(currentElement, tag);
	int tagID = writeTag(tag, ofToString(value, floatPrecision).c_str(), -1) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::addValue(const string& tag, const string& value){
    currentElement.addValue(tag, value);
    return getSiblingCount(currentElement, tag);
}


//---------------------------------------------------------
// now attributes
//---------------------------------------------------------

//---------------------------------------------------------
int ofxXmlSettings::addAttribute(const string& tag, const string& attribute, int value, int which){
	return writeAttribute(tag, attribute, ofToString(value), which);
}

//---------------------------------------------------------
int ofxXmlSettings::addAttribute(const string& tag, const string& attribute, int value){
	return writeAttribute(tag, attribute, ofToString(value), 0);
}

//---------------------------------------------------------
int ofxXmlSettings::addAttribute(const string& tag, const string& attribute, double value, int which){
	//return writeAttribute(tag, attribute, ofToString(value), which);
	int tagID = writeAttribute(tag, attribute, ofToString(value, floatPrecision).c_str(), which) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::addAttribute(const string& tag, const string& attribute, double value){
	return writeAttribute(tag, attribute, ofToString(value), 0);
}

//---------------------------------------------------------
int ofxXmlSettings::addAttribute(const string& tag, const string& attribute, const string& value, int which){
	return writeAttribute(tag, attribute, value, which);
}

//---------------------------------------------------------
int ofxXmlSettings::addAttribute(const string& tag, const string& attribute, const string& value){
	return writeAttribute(tag, attribute, value, 0);
}

//---------------------------------------------------------
void ofxXmlSettings::removeAttribute(const string& tag, const string& attribute, int which){
    
    string attr = "[@" + attribute + "]";
    getElement(tag, which).remove(attr);
    
}

//---------------------------------------------------------
void ofxXmlSettings::clearTagAttributes(const string& tag, int which){

    getElement(tag, which).clearAttributes();
}

//---------------------------------------------------------
int ofxXmlSettings::getNumAttributes(const string& tag, int which){
    
    ofXml child = getElement(tag, which);
    map<string, string> attrs = child.getAttributes();
    return attrs.size();
    
}

//---------------------------------------------------------
bool ofxXmlSettings::attributeExists(const string& tag, const string& attribute, int which){
    
    ofXml child = getElement(tag, which);
    
    map<string, string> attrs = child.getAttributes();
    
    if(attrs.find(attribute) != attrs.end()) {
        return true;
    }
    
    return false;
}

//---------------------------------------------------------
bool ofxXmlSettings::getAttributeNames(const string& tag, vector<string>& outNames, int which){
    
    ofXml child = getElement(tag, which);
    
    map<string, string> attrs = child.getAttributes();
    map<string, string>::iterator it = attrs.begin();
    while( it != attrs.end()) {
        string attrName = it->first;
        outNames.push_back(attrName);
        ++it;
    }
    return true;
}

//---------------------------------------------------------
int ofxXmlSettings::getAttribute(const string& tag, const string& attribute, int defaultValue, int which){
    int value = defaultValue;
    string path = tag + "[@" + attribute + "]";
    ofXml child = getElement(path, which, true);
    string attr = child.getAttribute(attribute);
    if(attr != "") {
        return ofToInt(attr);
    }
	return value;
}

//---------------------------------------------------------
double ofxXmlSettings::getAttribute(const string& tag, const string& attribute, double defaultValue, int which){
    
    double value = defaultValue;
    string path = tag + "[@" + attribute + "]";
    ofXml child = getElement(path, which, true);
    string attr = child.getAttribute(attribute);
    if(attr != "") {
        return ofToFloat(attr);
    }
    return value;
}

//---------------------------------------------------------
string ofxXmlSettings::getAttribute(const string& tag, const string& attribute, const string& defaultValue, int which){

    string value = defaultValue;
    string path = tag + "[@" + attribute + "]";
    ofXml child = getElement(path, which, true);
    string attr = child.getAttribute(attribute);
    if(attr != "") {
        return attr;
    }
    return value;
}

//---------------------------------------------------------
int ofxXmlSettings::setAttribute(const string& tag, const string& attribute, int value, int which){
	return writeAttribute(tag, attribute, ofToString(value), which);
}

//---------------------------------------------------------
int ofxXmlSettings::setAttribute(const string& tag, const string& attribute, double value, int which){
	return writeAttribute(tag, attribute, ofToString(value), which);
}

//---------------------------------------------------------
int ofxXmlSettings::setAttribute(const string& tag, const string& attribute, const string& value, int which){
	return writeAttribute(tag, attribute, value, which);
}

//---------------------------------------------------------
int ofxXmlSettings::writeAttribute(const string& tag, const string& attribute, const string& valueString, int which){
    
    currentElement = getElement(tag, which);
    currentElement.setAttribute(attribute, valueString);
    return getSiblingCount(currentElement, tag);
}

//---------------------------------------------------------
bool ofxXmlSettings::loadFromBuffer( string buffer )
{
    if(documentElement.loadFromBuffer(buffer)) {
        currentElement = documentElement;
        return true;
    }
    return false;

}

//---------------------------------------------------------
void ofxXmlSettings::copyXmlToString(string & str)
{
	/*TiXmlPrinter printer;
	doc.Accept(&printer);

	str = printer.CStr();*/

    str = currentElement.toString();
    
}

    
ofXml ofxXmlSettings::getElement(const string& tag, const int which, bool useFirstTagForIndex)
{
    string copy = tag;
    
    if(copy.find(":") != string::npos) {
        replace( copy.begin(), copy.end(), ':', '/');
    }
    
    if(which != 0) {
        
        if(!useFirstTagForIndex){
        
            // now the selector
            stringstream stream;
            stream << "[" << which << "]";
            copy.append(stream.str());
        } else {
            
            vector<string> tokens = tokenize(copy, "/");
            stringstream sstream;
            sstream << tokens[0] << "[" << which << "]";
            for( int i = 1; i < tokens.size(); i++) {
                sstream << tokens[i];
            }
            copy = sstream.str();
            
        }
    }
    
    return currentElement.getChild(copy); // we know this'll always be a node
    
}

ofXml ofxXmlSettings::getElement(const string& path)
{
    string copy = path;
    // does it have an attribute?
    int ind = copy.find("[@");
    if(ind != string::npos) {
        copy = path.substr(0, ind);
    }
    return currentElement.getChild(copy); // we know this'll always be a node
    
}


