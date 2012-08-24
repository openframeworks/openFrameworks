#include "ofxXmlSettings.h"

#include <vector>
#include <string>
#include <iostream>

Poco::XML::Document& ofxXmlSettings::getDocument() 
{
    return *document;
}


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
    if(currentElement)
        currentElement->release();
    
    if(document)
        document->release();
}

//---------------------------------------------------------
void ofxXmlSettings::setVerbose(bool _verbose){
}

//---------------------------------------------------------
void ofxXmlSettings::clear(){
    
    currentElement->release();
}

//---------------------------------------------------------
bool ofxXmlSettings::loadFile(const string& xmlFile){

	cachedFilename = ofToDataPath(xmlFile);

	bool loadOkay = true;// = doc.LoadFile(fullXmlFile);
    
    file.open(cachedFilename); 
    ofBuffer buff = file.readToBuffer();
    
    string xml = buff.getText();
    
    try {
        // read the buffer into document
        document = parser.parseString(xml);
    } catch ( Poco::XML::SAXParseException &e) {
        
        stringstream sstream;
        sstream << e.name() << " " << e.getLineNumber();
        
        ofLog(OF_LOG_ERROR, sstream.str());
        return false;
    }
    
	//theo removed bool check as it would
	//return false if the file exists but was
	//empty

    //our push pop level should be set to 0!
	level = 0;
    currentElement = document->documentElement(); // now we're at the root

    file.close();
    
	return loadOkay;
}

//---------------------------------------------------------
bool ofxXmlSettings::saveFile(const string& xmlFile){

	string fullXmlFile = ofToDataPath(xmlFile);
    
    ostringstream stream;
    
    Poco::XML::DOMWriter writer;
    writer.writeNode( stream, document );
    
    file.open(xmlFile, ofFile::ReadWrite);
    
    if(!file.exists()) {
        
        if(!file.create()) {
            ofLog(OF_LOG_ERROR, " can't create xml file " );
            return false;
        }
    }
    
    // don't like this so much
    ofBuffer buffer(stream.str().c_str(), stream.str().size());
    
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
    
    ostringstream stream;
    
    Poco::XML::DOMWriter writer;
    writer.writeNode( stream, document );
    
    ofBuffer buffer(stream.str().c_str(), stream.str().size());
    
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

    // this expects that we have
    // <X><Y/><Y/><Y/></X>
    // but Poco doesn't care about this, so we could have <X><Y/><Z/><Y/></X>
    // but when 'which' == 2 we need the 2nd Y, not 2nd thing in the list
    NodeList *list = currentElement->getElementsByTagName(tag);
    list->item(which)->removeChild(list->item(which));
    
}

//---------------------------------------------------------
void ofxXmlSettings::removeTag(const string& tag, int which){
    
    Element *element = getElement(tag, which);
    
    if(element) {
        element->parentNode()->removeChild(element);
    }
}

//---------------------------------------------------------
int ofxXmlSettings::getValue(const string& tag, int defaultValue, int which){

    if (tag.find(':') != string::npos || tag.find('/') != string::npos){
		Element* element = getElement(tag, which, true);
        if (element){
            return ofToInt(element->innerText());
        }
	}
    return defaultValue;
    
}

//---------------------------------------------------------
double ofxXmlSettings::getValue(const string& tag, double defaultValue, int which){
    
    // is it a path using either tinyXML : for legacy or Poco / for the present
    if (tag.find(':') != string::npos || tag.find('/') != string::npos){
		Element* element = getElement(tag, which, true);
        if (element){
            return ofToFloat(element->innerText());
        }
	}
    return defaultValue;
    
}

//---------------------------------------------------------
string ofxXmlSettings::getValue(const string& tag, const string& defaultValue, int which){
 
    if (tag.find(':') != string::npos || tag.find('/') != string::npos){
		Element* element = getElement(tag, which, true);
        if (element){
            return element->innerText();
        }
        return defaultValue;
	}
}

//---------------------------------------------------------
int ofxXmlSettings::getPathValue(const string& path, int defaultValue){
    
    
    Element* element = (Element*) currentElement->getNodeByPath(path);
    if (element){
        return ofToInt(element->innerText());
    }
    return defaultValue;
    
}

//---------------------------------------------------------
double ofxXmlSettings::getPathValue(const string& path, double defaultValue){
    
    Element* element = (Element*) currentElement->getNodeByPath(path);
    if (element){
        return ofToFloat(element->innerText());
    }
    return defaultValue;
}

//---------------------------------------------------------
string ofxXmlSettings::getPathValue(const string& path, const string& defaultValue){
    
    Element* element = (Element*) currentElement->getNodeByPath(path);
    if (element){
        return element->innerText();
    }
    return defaultValue;

}


//---------------------------------------------------------
bool ofxXmlSettings::pushTag(const string&  tag, int which){
    
    int tagCount;
    
    // if there's pre0072 style ':' (not proper xpath but is how tinyXml works) get rid of them
    if(tag.find(":") != string::npos) {
        tagCount = count( tag.begin(), tag.end(), '/') + 1;
    } else if(tag.find("/") != string::npos) {
        tagCount = count( tag.begin(), tag.end(), '/') + 1;
    }

    // so we can bail from errors
    Element *cache = currentElement;
    currentElement = getElement(tag, which);
    
    if(currentElement) {
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
    
    if(currentElement->parentNode()->nodeType() == Node::ELEMENT_NODE) {
        currentElement = (Element*) currentElement->parentNode(); // we already know it's an element, so this is oK
        level--;
    }
    
    if(currentElement->parentNode()->nodeType() == Node::DOCUMENT_NODE) {
        currentElement = document->documentElement(); // we're back at the top
        level = 0;
    }
    
    return level;
    
}

//---------------------------------------------------------
int ofxXmlSettings::getPushLevel(){
	return level;
}

//---------------------------------------------------------
bool ofxXmlSettings::tagExists(const string& tag, int which){

    Element* element = getElement(tag, which, true);
    if (element){
        return true;
    }
    return false;
}


//---------------------------------------------------------
int ofxXmlSettings::getNumTags(const string&  tag){
    
    string copy = tag;
    
    if(copy.find(":") != string::npos) {
        replace( copy.begin(), copy.end(), ':', '/');
    }
    
    return currentElement->getElementsByTagName(copy)->length();
}



//---------------------------------------------------------
int ofxXmlSettings::writeTag(const string& tag, const string& valueStr, int which)
{
    
    // we need to handle 3 distinct situations:
    // A: writeTag("1/2/3", "blah") where none exist, so write <1><2><3>blah</1></2></3>
    // B: writeTag("1/2/3/4", "blah") where <1><2> exist, so write <3><4>blah</4></3> - this is the ugliest and should be done with a path going forward, please.
    // C: writeTag("3", "blah") so write <3>blah</3>
    
    vector<string> tokens;
    bool needsTokenizing = false;
    
    if(tag.find(':') != string::npos) {
        tokens = tokenize(tag, ":");
    } else if(tag.find('/') != string::npos) {
        tokens = tokenize(tag, "/");
    }
    
    // is this a tokenized tag?
    if(tokens.size() > 1) 
    {
        // don't 'push' down into the new nodes
        Element *element = currentElement;
        
        // find the last existing tag
        int lastExistingTag = tokens.size();
        
        // can't use reverse_iterator b/c accumulate doesn't like it
        for(vector<string>::iterator it = tokens.end(); it != tokens.begin(); it--) 
        {
            Element *testElement = getElement(accumulate(tokens.begin(), it, string("")), which);
            if(testElement) {
                lastExistingTag--;
                element = testElement;
                break;
            }
        }
        
        // create all the tags that don't exist
        for(int i = lastExistingTag; i < tokens.size(); i++)
        {
            Element *newElement = document->createElement(tokens.at(i));
            element->appendChild(newElement);
            element = newElement;
        }
        
        
        if(valueStr != "") 
        {
            Text *text = document->createTextNode(valueStr);
            try {
                element->appendChild( (Node*) text);
            } catch ( DOMException &e ) {
                stringstream sstream;
                sstream << " cannot set node value " << DOMErrorMessage(e.code());
                ofLog(OF_LOG_ERROR, sstream.str());
            }
        }
        
        return getSiblingCount(element, tag);
        
    } else {
    
        // get last tag in the tag list
        Element *newElement = document->createElement(tag);
        
        if(valueStr != "") {
            Text *text = document->createTextNode(valueStr);
            try {
                newElement->appendChild( (Node*) text);
            } catch ( DOMException &e ) {
                stringstream sstream;
                sstream << " cannot set node value " << DOMErrorMessage(e.code());
                ofLog(OF_LOG_ERROR, sstream.str());
            }
        }
        
        currentElement->appendChild(newElement);
        
        return getSiblingCount(newElement, tag);
    }
}

//---------------------------------------------------------
int ofxXmlSettings::setValue(const string& tag, int value, int which){
    Element *element = getElement(tag, which);
    
    if(!element) {
        ofLog(OF_LOG_WARNING, " setValue of " + tag + " failed because tag doesn't exist");
        return -1;
    }
    
    if(element->firstChild()->nodeType() == Node::TEXT_NODE) {
        Text *node = document->createTextNode(ofToString(value));
        element->replaceChild( (Node*) node, element->firstChild()); // swap out
    }
    
    return getSiblingCount(element, tag);
    
    // this is the current way that the implementation is done, which seems wrong
    /*
     return writeTag(tag, value, which);
    */
}

//---------------------------------------------------------
int ofxXmlSettings::setValue(const string& tag, double value, int which){
    Element *element = getElement(tag, which);
    
    if(!element) {
        ofLog(OF_LOG_WARNING, " setValue of " + tag + " failed because tag doesn't exist");
        return -1;
    }
    
    if(element->firstChild()->nodeType() == Node::TEXT_NODE) {
        Text *node = document->createTextNode(ofToString(value));
        element->replaceChild( (Node*) node, element->firstChild()); // swap out
    }
    
    return getSiblingCount(element, tag);
    
    // this is the current way that the implementation is done, which seems wrong
    /*
     return writeTag(tag, value, which);
     */
}

//---------------------------------------------------------
int ofxXmlSettings::setValue(const string& tag, const string& value, int which){
    Element *element = getElement(tag, which);
    
    if(!element) {
        ofLog(OF_LOG_WARNING, " setValue of " + tag + " failed because tag doesn't exist");
        return -1;
    }
    
    if(element->firstChild()->nodeType() == Node::TEXT_NODE) {
        Text *node = document->createTextNode(value);
        element->replaceChild( (Node*) node, element->firstChild()); // swap out
    }
    
    return getSiblingCount(element, tag);
    
    // this is the current way that the implementation is done, which seems wrong
    /*
     return writeTag(tag, value, which);
     */
}

int ofxXmlSettings::getSiblingCount(Element *element, const string tag) {
    vector<string> tokens;
    bool needTokenize = false;
    if(tag.find(':') != string::npos) {
        tokens = tokenize(tag, ":");
        needTokenize = true;
    } else if(tag.find('/') != string::npos) {
        tokens = tokenize(tag, "/");
        needTokenize = true;
    } 
    
    Element *parent = (Element*) element->parentNode();
    if(needTokenize) {
        return parent->getElementsByTagName(tokens[tokens.size()-1])->length(); // return just the same tags
    } else {
        return parent->getElementsByTagName(tag)->length(); // return just the same tags
    }
}

// to be deprecated
//---------------------------------------------------------
int ofxXmlSettings::addValue(const string& tag, int value){
    return addTagByPath(tag, value) - 1;
}

// to be deprecated
//---------------------------------------------------------
int ofxXmlSettings::addValue(const string&  tag, double value){
	return addTagByPath(tag, value) - 1;
}

// to be deprecated
//---------------------------------------------------------
int ofxXmlSettings::addValue(const string& tag, const string& value){
	return addTagByPath(tag, value) - 1;
}

//---------------------------------------------------------
int ofxXmlSettings::addTagByPath(const string& tag){
    
	return writeTag(tag, "", -1) - 1;
}

//---------------------------------------------------------
int ofxXmlSettings::addTagByPath(const string& tag, int value){
	return writeTag(tag, ofToString(value), -1) - 1;
}

//---------------------------------------------------------
int ofxXmlSettings::addTagByPath(const string&  tag, double value){
	return writeTag(tag, ofToString(value), -1) - 1;
}

//---------------------------------------------------------
int ofxXmlSettings::addTagByPath(const string& tag, const string& value){
	return writeTag(tag, value) - 1;
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
	return writeAttribute(tag, attribute, ofToString(value), which);
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
int ofxXmlSettings::addAttributeByPath(const string& path, const string& value){
    Element *element = getElement(path);
	//return writeAttribute(tag, attribute, value, which);
}

//---------------------------------------------------------
void ofxXmlSettings::removeAttribute(const string& tag, const string& attribute, int which){
    
    Element *element = getElement(tag, which);
    
    if(element) {
        element->removeAttribute(attribute);
    }
    
}

//---------------------------------------------------------
void ofxXmlSettings::clearTagAttributes(const string& tag, int which){

    Element *element = getElement(tag, which);
    NamedNodeMap *map = element->attributes();
    
    for(int i = 0; i < map->length(); i++) {
        element->removeAttribute(map->item(i)->nodeName());
    }

    map->release();
}

//---------------------------------------------------------
int ofxXmlSettings::getNumAttributes(const string& tag, int which){
    
    Element *element = getElement(tag, which);
    
    if(element) {
        return element->attributes()->length();
    }
    
    return 0;
    
}

//---------------------------------------------------------
bool ofxXmlSettings::attributeExists(const string& tag, const string& attribute, int which){
    
    Element *element = getElement(tag, which);
    
    if(element) {
        return (element->getAttributeNode(attribute) != NULL);
    }
    
    return 0;
}

//---------------------------------------------------------
bool ofxXmlSettings::getAttributeNames(const string& tag, vector<string>& outNames, int which){
    
    Element *element = getElement(tag, which);
    
    NamedNodeMap *map = element->attributes();
    
    for(int i = 0; i < map->length(); i++) {
        outNames.push_back(map->item(i)->nodeName());
    }
    
    map->release();
    
}

//---------------------------------------------------------
int ofxXmlSettings::getAttribute(const string& tag, const string& attribute, int defaultValue, int which){
    int value = defaultValue;
   Element *element = getElement(tag, which);
    
    if(element) {
        string sa = element->getAttribute(attribute);
        value = atoi(sa.c_str());
    }
	return value;
}

//---------------------------------------------------------
double ofxXmlSettings::getAttribute(const string& tag, const string& attribute, double defaultValue, int which){
    
    double value = defaultValue;
    Element *element = getElement(tag, which);
    
    if(element) {
        string sa = element->getAttribute(attribute);
        value = atof(sa.c_str());
    }
    return value;
}

//---------------------------------------------------------
string ofxXmlSettings::getAttribute(const string& tag, const string& attribute, const string& defaultValue, int which){

    string value = defaultValue;
    Element *element = getElement(tag, which);
    
    if(element) {
        value = element->getAttribute(attribute);
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
    
    Element* element = getElement(tag, which);
    
    Attr *attr = document->createAttribute(attribute);
    attr->setValue(valueString);
    
    if(!element) { // if it doesn't exist
        
        /*vector<string> tokens;
        
        if(tag.find(':') != string::npos) {
            tokens = tokenize(tag, ":");
        } else if(tag.find('/') != string::npos) {
            tokens = tokenize(tag, "/");
        }
        
        for(int i = 0; i < tokens.size(); i++) {
            Element *newElement = document->createElement(tokens.at(i));
            currentElement->appendChild(newElement);
            currentElement = newElement;
        }*/
        
        vector<string> tokens;
        bool needsTokenizing = false;
        
        if(tag.find(':') != string::npos) {
            tokens = tokenize(tag, ":");
        } else if(tag.find('/') != string::npos) {
            tokens = tokenize(tag, "/");
        }
        
        // is this a tokenized tag?
        if(tokens.size() > 1) 
        {
            // don't 'push' down into the new nodes
            element = currentElement;
            
            // find the last existing tag
            int lastExistingTag = tokens.size();
            
            // can't use reverse_iterator b/c accumulate doesn't like it
            for(vector<string>::iterator it = tokens.end(); it != tokens.begin(); it--) 
            {
                string empty = "";
                string concat = accumulate(tokens.begin(), it, std::string());
                Element *testElement = getElement(concat, which);
                if(testElement) {
                    lastExistingTag--;
                    element = testElement;
                    break;
                }
            }
            
            // create all the tags that don't exist
            for(int i = lastExistingTag; i < lastExistingTag; i++)
            {
                Element *newElement = document->createElement(tokens.at(i));
                element->appendChild(newElement);
                element = newElement;
            }
            
            
            if(valueString != "") 
            {
                Text *text = document->createTextNode(valueString);
                try {
                    element->appendChild( (Node*) text);
                } catch ( DOMException &e ) {
                    stringstream sstream;
                    sstream << " cannot set node value " << DOMErrorMessage(e.code());
                    ofLog(OF_LOG_ERROR, sstream.str());
                }
            }
        }
    }
    
    element->attributes()->setNamedItem(attr);
    currentElement = element;
    
    return getSiblingCount(element, tag);
}

//---------------------------------------------------------
bool ofxXmlSettings::loadFromBuffer( string buffer )
{

    int size = buffer.size();

    //bool loadOkay = doc.ReadFromMemory( buffer.c_str(), size);//, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING);
    Poco::XML::DOMParser parser;
    
    // release and null out if we already have a document
    if(document) {
        document->release();
        document = 0;
    }
    
    document = parser.parseString(buffer);
    
    if(document) {
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
    
    ostringstream stream;
    Poco::XML::DOMWriter writer;
    writer.writeNode( stream, document );

    str = stream.str();
    
}
    
Element* ofxXmlSettings::getElement(const string& tag, const int which, bool useFirstTagForIndex)
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
    
    return (Element*) currentElement->getNodeByPath(copy); // we know this'll always be a node
    
}

Element* ofxXmlSettings::getElement(const string& path)
{
    string copy = path;
    // does it have an attribute?
    int ind = copy.find("[@");
    if(ind != string::npos) {
        copy = path.substr(0, ind);
    }
    return (Element*) currentElement->getNodeByPath(copy); // we know this'll always be a node
    
}

string ofxXmlSettings::DOMErrorMessage(short msg)
{
    switch(msg) {
        case 1:            
            return "INDEX_SIZE_ERR";
            break;/// index or size is negative or greater than allowed value
        case 2:
            return "DOMSTRING_SIZE_ERR";          /// the specified range of text does not fit into a DOMString (not used)
            break;
        case 3:
            return "HIERARCHY_REQUEST_ERR";       /// a node is inserted somewhere it doesn't belong
            break;
        case 4:
            return "WRONG_DOCUMENT_ERR";          /// a node is used in a different document than the one that created it
            break;
        case 5:
            return "INVALID_CHARACTER_ERR";       /// an invalid character is specified (not used)
            break;
        case 6:
            return "NO_DATA_ALLOWED_ERR";         /// data is specified for a node which does not support data
            break;
        case 7:
            return "NO_MODIFICATION_ALLOWED_ERR"; /// an attempt is made to modify an object where modifications are not allowed
            break;
        case 8:
            return "NOT_FOUND_ERR";               /// an attempt was made to reference a node in a context where it does not exist
            break;
        case 9:
            return "NOT_SUPPORTED_ERR";           /// the implementation does not support the type of object requested
            break;
        case 10:
            return "INUSE_ATTRIBUTE_ERR";         /// an attempt is made to add an attribute that is already in use elsewhere
            break;
        case 11:
            return "INVALID_STATE_ERR";           /// a parameter or an operation is not supported by the underlying object
            break;
        case 12:
            return "SYNTAX_ERR";                  /// an invalid or illegal string is specified
            break;
        case 13:
            return "INVALID_MODIFICATION_ERR";    /// an attempt is made to modify the type of the underlying object
            break;
        case 14:
            return "NAMESPACE_ERR";               /// an attempt is made to create or change an object in a way which is incorrect with regard to namespaces
            break;
        case 15:
            return "INVALID_ACCESS_ERR";          /// an attempt is made to use an object that is not or is no longer usable
            break;
    }
    
}

