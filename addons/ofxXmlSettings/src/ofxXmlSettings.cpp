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
	//we do this so that we have a valid handle
	//without the need for loadFile
	//storedHandle   = TiXmlHandle(&doc);
    
    
}

//----------------------------------------
ofxXmlSettings::ofxXmlSettings(const string& xmlFile)
{
	level			= 0;
	//we do this so that we have a valid handle
	//without the need for loadFile
	//storedHandle   = TiXmlHandle(&doc);
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
	//we clear from our root level
	//this is usually the document
	//but if we are pushed  - it could
	//be all the tags inside of the pushed
	//node - including the node itself!

	//storedHandle.ToNode()->Clear();
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
    
    file.open(xmlFile);
    
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
    
    file.open(cachedFilename);
    
    if(!file.writeFromBuffer(buffer)) {
        ofLog(OF_LOG_ERROR, " can't write to xml file " );
        return false;
    }
    
    file.close(); // do we just keep the file open?
    return true;

}

//---------------------------------------------------------
void ofxXmlSettings::clearTagContents(const string& tag, int which){
	//we check it first to see if it exists
	//otherwise setValue will make a new empty tag
	//if( tagExists(tag, which) )setValue(tag, "", which);
    
    // this expects that we have
    // <X><Y/><Y/><Y/></X>
    // but Poco doesn't care about this, so we could have <X><Y/><Z/><Y/></X>
    // but when 'which' == 2 we need the 2nd Y, not 2nd thing in the list
    NodeList *list = currentElement->getElementsByTagName(tag);
    list->item(which)->setNodeValue("");
    
}

//---------------------------------------------------------
void ofxXmlSettings::removeTag(const string& tag, int which){
    
    NodeList *list = currentElement->getElementsByTagName(tag);
    int i = 0;
    while(i < list->length()) {
        currentElement->removeChild(list->item(i));
        i++;
    }

	/*vector<string> tokens = tokenize(tag,":");

	//no tags so we return
	if( tokens.size() == 0 ) return;

	//grab the handle from the level we are at
	//normally this is the doc but could be a pushed node
	TiXmlHandle tagHandle = storedHandle;

	if(which < 0) which = 0;

	for(int x=0;x<(int)tokens.size();x++){

		//we only support multi tags
		//with same name at root level
		if(x > 0) which = 0;

		TiXmlHandle isRealHandle = tagHandle.ChildElement( tokens.at(x), which);

		if ( !isRealHandle.ToNode() ) break;
		else{
			if (x == (int)tokens.size()-1){
				//if we are at the last tag and it exists
				//we use its parent to remove it - haha
				tagHandle.ToNode()->RemoveChild( isRealHandle.ToNode() );
			}
			tagHandle = isRealHandle;
		}
	}*/
}

//---------------------------------------------------------
int ofxXmlSettings::getValue(const string& tag, int defaultValue, int which){
    /*TiXmlHandle valHandle(NULL);
	if (readTag(tag, valHandle, which)){
		return ofToInt(valHandle.ToText()->Value());
	}
	return defaultValue;*/
    
    if (tag.find(':') != string::npos || tag.find('/') != string::npos){
		Element* element;
        if (readTag(tag, &element, which)){
            return ofToInt(element->innerText());
        }
	}
    return defaultValue;
    
}

//---------------------------------------------------------
double ofxXmlSettings::getValue(const string& tag, double defaultValue, int which){
    
    // is it a path using either tinyXML : for legacy or Poco / for the present
    if (tag.find(':') != string::npos || tag.find('/') != string::npos){
		Element* element;
        if (readTag(tag, &element, which)){
            return ofToFloat(element->innerText());
        }
	}
    return defaultValue;
    
    // maybe not use this, but it's much much faster for non-path
    /*NodeList *list = currentElement->getElementsByTagName(tag);
    if(list->length() < which) {
        return (double) list->item(which)->nodeValue(); // can't do ofToInt
    }
    return defaultValue;*/
}

//---------------------------------------------------------
string ofxXmlSettings::getValue(const string& tag, const string& defaultValue, int which){
    /*TiXmlHandle valHandle(NULL);
	if (readTag(tag, valHandle, which)){
		return valHandle.ToText()->ValueStr();
	}
	return defaultValue;*/
 
    if (tag.find(':') != string::npos || tag.find('/') != string::npos){
		Element* element;
        if (readTag(tag, &element, which)){
            return element->innerText();
        }
        return defaultValue;
	}
}

//---------------------------------------------------------
bool ofxXmlSettings::readTag(const string&  tag, Element** node, int which){

    ////////// from POCO documentation
    /*
     Only simple XPath expressions are supported. These are the slash notation for specifying paths to elements, and the square bracket expression for finding elements by their index, by attribute value, or finding attributes by names.
     The slash at the beginning is optional, the evaluation always starts at this element. A double-slash at the beginning recursively searches the entire subtree for the first element.
     Examples:
     elem1/elem2/elem3
     /elem1/elem2/elem3
     /elem1/elem2[1]
     /elem1/elem2[@attr1]
     /elem1/elem2[@attr1='value']
     //elem2[@attr1='value']
     //[@attr1='value']
     */
    
	

	/*TiXmlHandle tagHandle = storedHandle;
	for(int x=0;x<(int)tokens.size();x++){
		if(x == 0)tagHandle = tagHandle.ChildElement(tokens.at(x), which);
		else tagHandle = tagHandle.FirstChildElement( tokens.at(x) );
	}

	// once we've walked, let's get that value...
	valHandle = tagHandle.Child( 0 );
    return (valHandle.ToText() != NULL);*/
    
    /*string copy = "/" + tag;
    
    // if there's pre0072 style ':' (not proper xpath but is how tinyXml works) get rid of them
    if(copy.find(':') != string::npos) {
        replace( copy.begin(), copy.end(), ':', '/');
    }
    
    if(which != 0) {
        // now the selector
        stringstream stream;
        stream << "[" << which << "]";
        copy.append(stream.str());
    }
    
    *node = currentElement->getNodeByPath(copy);*/
    
    *node = getElement(tag, which);
    
    if(node) {
        return true;
    }
    
    return false;
}


//---------------------------------------------------------
bool ofxXmlSettings::pushTag(const string&  tag, int which){

	/*int pos = tag.find(":");

    // Either find the tag specified, or the first tag if colon-seperated.
    string tagToFind((pos > 0) ? tag.substr(0,pos) :tag);

	//we only allow to push one tag at a time.
	TiXmlHandle isRealHandle = storedHandle.ChildElement(tagToFind, which);

	if( isRealHandle.ToNode() ){
		storedHandle = isRealHandle;
		level++;
		return true;
	}else{
        ofLog( OF_LOG_ERROR, "pushTag - <" + tag + "> tag not found");
	}

	return false;*/
    
    string copy = tag;
    int tagCount;
    
    // if there's pre0072 style ':' (not proper xpath but is how tinyXml works) get rid of them
    if(copy.find(":") != string::npos) {
        replace( copy.begin(), copy.end(), ':', '/');
    }
    
    tagCount = count( copy.begin(), copy.end(), '/') + 1;
    
    // now the index selector
    stringstream stream;
    stream << "[" << which << "]";
    copy.append(stream.str());

    // so we can bail from errors
    Element *cache = currentElement;
    currentElement = (Element*) currentElement->getNodeByPath(copy);
    
    if(currentElement) {
        level += tagCount;
        return true;
    } else {
        currentElement = cache;
        ofLog( OF_LOG_ERROR, "pushTag - <" + tag + "> tag not found");
	}
    
	return false;
    
}

//---------------------------------------------------------
int ofxXmlSettings::popTag(){

	/*if(level >= 1){
		//TiXmlHandle parent( (storedHandle.ToNode() )->Parent() );
		//storedHandle = parent;
	}else{
		storedHandle = TiXmlHandle(&doc);
		level = 0;
	}

	return level;*/
    
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

	/*vector<string> tokens = tokenize(tag,":");

	bool found = false;

	//grab the handle from the level we are at
	//normally this is the doc but could be a pushed node
	TiXmlHandle tagHandle = storedHandle;

	if(which < 0) which = 0;

	for(int x=0;x<(int)tokens.size();x++){

		//we only support multi tags
		//with same name at root level
		if(x > 0) which = 0;

		TiXmlHandle isRealHandle = tagHandle.ChildElement( tokens.at(x), which);

		//as soon as we find a tag that doesn't exist
		//we return false;
		if ( !isRealHandle.ToNode() ){
			found = false;
			break;
		}
		else{
			found = true;
			tagHandle = isRealHandle;
		}
	}

	return found;*/
    
    Element* node;
    if (readTag(tag, &node, which)){
        return true;
    }
    
    return false;
}


//---------------------------------------------------------
int ofxXmlSettings::getNumTags(const string&  tag){
	//this only works for tags at the current root level

	/*int pos = tag.find(":");

    // Either find the tag specified, or the first tag if colon-seperated.
    string tagToFind((pos > 0) ? tag.substr(0,pos) :tag);

	//grab the handle from the level we are at
	//normally this is the doc but could be a pushed node
	TiXmlHandle tagHandle = storedHandle;

	int count = 0;

	//ripped from tinyXML as doing this ourselves once is a LOT! faster
	//than having this called n number of times in a while loop - we go from n*n iterations to n iterations

	TiXmlElement* child = ( storedHandle.FirstChildElement( tagToFind ) ).ToElement();
	for (count = 0; child; child = child->NextSiblingElement( tagToFind ), ++count){
		//nothing
	}

	return count;*/
    
    string copy = tag;
    
    if(copy.find(":") != string::npos) {
        replace( copy.begin(), copy.end(), ':', '/');
    }
    
    return currentElement->getElementsByTagName(copy)->length();
}



//---------------------------------------------------------
int ofxXmlSettings::writeTag(const string&  tag, const string& valueStr, int which){

	/*vector<string> tokens = tokenize(tag,":");

	// allocate on the stack
    vector<TiXmlElement> elements;
    elements.reserve(tokens.size());
	for(int x=0;x<(int)tokens.size();x++)
        elements.push_back(tokens.at(x));


	TiXmlText Value(valueStr);

	// search our way up - do these tags exist?
	// find the first that DOESNT exist, then move backwards...
	TiXmlHandle tagHandle = storedHandle;

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

		if ( !isRealHandle.ToNode() ||  addNewTag){

			for(int i=(int)tokens.size()-1;i>=x;i--){
				if (i == (int)tokens.size()-1){
					elements[i].InsertEndChild(Value);
				} else {
					elements[i].InsertEndChild(elements[i+1]);
				}
			}

			tagHandle.ToNode()->InsertEndChild(elements[x]);

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
	TiXmlElement* child = ( storedHandle.FirstChildElement( tokens.at(0) ) ).ToElement();
	for (numSameTags = 0; child; child = child->NextSiblingElement( tokens.at(0) ), ++numSameTags){
		//nothing
	}

	return numSameTags;*/
    
    
    Element *element = getElement(tag, which);
    
    if(!element) { // if it doesn't exist
        
        vector<string> tokens;
        
        if(tag.find(':') != string::npos) {
            tokens = tokenize(tag, ":");
        }
        
        if(tag.find('/') != string::npos) {
            tokens = tokenize(tag, "/");
        }
        
        for(int i = 0; i < tokens.size(); i++) {
            Element *newElement = document->createElement(tokens.at(i));
            currentElement->appendChild(newElement);
            currentElement = newElement;
            
        }
    } else {
    
        // need to walk these back up? ugh.
        Element *newElement = document->createElement(tag);
        newElement->setNodeValue(valueStr);
        element->appendChild(newElement);
    }
    
    return 0;
    
}

//---------------------------------------------------------
int ofxXmlSettings::setValue(const string& tag, int value, int which){
	int tagID = writeTag(tag, ofToString(value).c_str(), which) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::setValue(const string& tag, double value, int which){
	int tagID = writeTag(tag, ofToString(value).c_str(), which) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::setValue(const string& tag, const string& value, int which){
	int tagID = writeTag(tag, value, which) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::addValue(const string& tag, int value){
	int tagID = writeTag(tag, ofToString(value).c_str(), -1) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::addValue(const string&  tag, double value){
	int tagID = writeTag(tag, ofToString(value).c_str(), -1) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::addValue(const string& tag, const string& value){
	int tagID = writeTag(tag, value, -1) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::addTag(const string& tag){
	int tagID = writeTag(tag, "", -1) -1;
	return tagID;
}

/*******************
* Attribute addons *
*******************/

//---------------------------------------------------------
int ofxXmlSettings::addAttribute(const string& tag, const string& attribute, int value, int which){
	int tagID = writeAttribute(tag, attribute, ofToString(value).c_str(), which) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::addAttribute(const string& tag, const string& attribute, int value){
	return addAttribute(tag,attribute,value,-1);
}

//---------------------------------------------------------
int ofxXmlSettings::addAttribute(const string& tag, const string& attribute, double value, int which){
	int tagID = writeAttribute(tag, attribute, ofToString(value).c_str(), which) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::addAttribute(const string& tag, const string& attribute, double value){
	return addAttribute(tag,attribute,value,-1);
}

//---------------------------------------------------------
int ofxXmlSettings::addAttribute(const string& tag, const string& attribute, const string& value, int which){
	int tagID = writeAttribute(tag, attribute, value, which) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::addAttribute(const string& tag, const string& attribute, const string& value){
	return addAttribute(tag,attribute,value,-1);
}

//---------------------------------------------------------
void ofxXmlSettings::removeAttribute(const string& tag, const string& attribute, int which){
	/*vector<string> tokens = tokenize(tag,":");
	TiXmlHandle tagHandle = storedHandle;
	for (int x = 0; x < (int)tokens.size(); x++) {
		if (x == 0)
			tagHandle = tagHandle.ChildElement(tokens.at(x), which);
		else
			tagHandle = tagHandle.FirstChildElement(tokens.at(x));
	}

	if (tagHandle.ToElement()) {
		TiXmlElement* elem = tagHandle.ToElement();
		elem->RemoveAttribute(attribute);
	}*/
    
    Element *element = getElement(tag, which);
    
    if(element) {
        element->removeAttribute(attribute);
    }
    
}

//---------------------------------------------------------
void ofxXmlSettings::clearTagAttributes(const string& tag, int which){
	/*vector<string> names;
    getAttributeNames( tag, names, which );
	for (vector<string>::iterator i = names.begin(); i != names.end(); i++)
		removeAttribute(tag, *i, which);*/
    
    
    Element *element = getElement(tag, which);
    NamedNodeMap *map = element->attributes();
    
    for(int i = 0; i < map->length(); i++) {
        element->removeAttribute(map->item(i)->nodeName());
    }

    map->release();
}

//---------------------------------------------------------
int ofxXmlSettings::getNumAttributes(const string& tag, int which){
	/*vector<string> tokens = tokenize(tag,":");
	TiXmlHandle tagHandle = storedHandle;
	for (int x = 0; x < (int)tokens.size(); x++) {
		if (x == 0)
			tagHandle = tagHandle.ChildElement(tokens.at(x), which);
		else
			tagHandle = tagHandle.FirstChildElement(tokens.at(x));
	}

	if (tagHandle.ToElement()) {
		TiXmlElement* elem = tagHandle.ToElement();

		// Do stuff with the element here
		TiXmlAttribute* first = elem->FirstAttribute();
		if (first) {
			int count = 1;
			for (TiXmlAttribute* curr = first; curr != elem->LastAttribute(); curr = curr->Next())
				count++;
			return count;
		}
	}
	return 0;*/
    
    
    Element *element = getElement(tag, which);
    
    if(element) {
        return element->attributes()->length();
    }
    
    return 0;
    
}

//---------------------------------------------------------
bool ofxXmlSettings::attributeExists(const string& tag, const string& attribute, int which){
	/*vector<string> tokens = tokenize(tag,":");
	TiXmlHandle tagHandle = storedHandle;
	for (int x = 0; x < (int)tokens.size(); x++) {
		if (x == 0)
			tagHandle = tagHandle.ChildElement(tokens.at(x), which);
		else
			tagHandle = tagHandle.FirstChildElement(tokens.at(x));
	}

	if (tagHandle.ToElement()) {
		TiXmlElement* elem = tagHandle.ToElement();

		// Do stuff with the element here
		for (TiXmlAttribute* a = elem->FirstAttribute(); a; a = a->Next()) {
			if (a->Name() == attribute)
				return true;
		}
	}
	return false;*/
    
    Element *element = getElement(tag, which);
    
    if(element) {
        return (element->getAttributeNode(attribute) != NULL);
    }
    
    return 0;
}

//---------------------------------------------------------
bool ofxXmlSettings::getAttributeNames(const string& tag, vector<string>& outNames, int which){
	/*vector<string> tokens = tokenize(tag,":");
	TiXmlHandle tagHandle = storedHandle;
	for (int x = 0; x < (int)tokens.size(); x++) {
		if (x == 0)
			tagHandle = tagHandle.ChildElement(tokens.at(x), which);
		else
			tagHandle = tagHandle.FirstChildElement(tokens.at(x));
	}

	if (tagHandle.ToElement()) {
		TiXmlElement* elem = tagHandle.ToElement();

		// Do stuff with the element here
		for (TiXmlAttribute* a = elem->FirstAttribute(); a; a = a->Next())
			outNames.push_back( string(a->Name()) );
	}
	return !outNames.empty();*/
    
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
	//readIntAttribute(tag, attribute, value, which);
    
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
    /*
	readDoubleAttribute(tag, attribute, value, which);
	return value;*/
    Element *element = getElement(tag, which);
    
    if(element) {
        string sa = element->getAttribute(attribute);
        value = atof(sa.c_str());
    }
    return value;
}

//---------------------------------------------------------
string ofxXmlSettings::getAttribute(const string& tag, const string& attribute, const string& defaultValue, int which){
    /*
	readStringAttribute(tag, attribute, value, which);
	return value;*/
    
    string value = defaultValue;
    Element *element = getElement(tag, which);
    
    if(element) {
        value = element->getAttribute(attribute);
    }  
    return value;
}

//---------------------------------------------------------
int ofxXmlSettings::setAttribute(const string& tag, const string& attribute, int value, int which){
	char valueStr[255];
	sprintf(valueStr, "%i", value);
	int tagID = writeAttribute(tag, attribute, valueStr, which) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::setAttribute(const string& tag, const string& attribute, double value, int which){
	char valueStr[255];
	sprintf(valueStr, "%lf", value);
	int tagID = writeAttribute(tag, attribute, valueStr, which) -1;
	return tagID;
}

//---------------------------------------------------------
int ofxXmlSettings::setAttribute(const string& tag, const string& attribute, const string& value, int which){
	int tagID = writeAttribute(tag, attribute, value, which) -1;
	return tagID;
}

//---------------------------------------------------------
/*TiXmlElement* ofxXmlSettings::getElementForAttribute(const string& tag, int which){
	vector<string> tokens = tokenize(tag,":");
	TiXmlHandle tagHandle = storedHandle;
	for (int x = 0; x < (int)tokens.size(); x++) {
		if (x == 0)
			tagHandle = tagHandle.ChildElement(tokens.at(x), which);
		else
			tagHandle = tagHandle.FirstChildElement(tokens.at(x));
	}
    return tagHandle.ToElement();
}*/

//---------------------------------------------------------
/*bool ofxXmlSettings::readIntAttribute(const string& tag, const string& attribute, int& outValue, int which){

    TiXmlElement* elem = getElementForAttribute(tag, which);
    if (elem)
        return (elem->QueryIntAttribute(attribute, &outValue) == TIXML_SUCCESS);
    return false;
    
    Element *element = getElement(const string& tag, const int which);
    
    if(element) {
        outValue = element->getAttribute(attribute);
        return true;
    }
    return false;
}

//---------------------------------------------------------
bool ofxXmlSettings::readDoubleAttribute(const string& tag, const string& attribute, double& outValue, int which){

    TiXmlElement* elem = getElementForAttribute(tag, which);
    if (elem)
        return (elem->QueryDoubleAttribute(attribute, &outValue) == TIXML_SUCCESS);
    return false;
    
    Element *element = getElement(const string& tag, const int which);
    
    if(element) {
        outValue = element->getAttribute(attribute);
        return true;
    }
    return false;
}

//---------------------------------------------------------
bool ofxXmlSettings::readStringAttribute(const string& tag, const string& attribute, string& outValue, int which){

    TiXmlElement* elem = getElementForAttribute(tag, which);
    if (elem)
    {
        const string* value = elem->Attribute(attribute);
        if (value)
        {
            outValue = *value;
            return true;
        }
    }
    return false;
    
    Element *element = getElement(const string& tag, const int which);
    
    if(element) {
        outValue = element->getAttribute(attribute);
        return true;
    }
    return false;
}*/

//---------------------------------------------------------
int ofxXmlSettings::writeAttribute(const string& tag, const string& attribute, const string& valueString, int which){
	/*vector<string> tokens = tokenize(tag,":");
	TiXmlHandle tagHandle = storedHandle;
	for (int x = 0; x < (int)tokens.size(); x++) {
		if (x == 0)
			tagHandle = tagHandle.ChildElement(tokens.at(x), which);
		else
			tagHandle = tagHandle.FirstChildElement(tokens.at(x));
	}

	int ret = 0;
	if (tagHandle.ToElement()) {
		TiXmlElement* elem = tagHandle.ToElement();
		elem->SetAttribute(attribute, valueString);

        // Do we really need this?  We could just ignore this and remove the 'addAttribute' functions...
		// Now, just get the ID.
		int numSameTags;
		TiXmlElement* child = ( storedHandle.FirstChildElement( tokens.at(0) ) ).ToElement();
		for (numSameTags = 0; child; child = child->NextSiblingElement( tokens.at(0) ), ++numSameTags) {
			// nothing
		}
		ret = numSameTags;
	}
	return ret;*/
    
    
    Element* element = getElement(tag, which);
    
    Attr *attr = document->createAttribute(attribute);
    attr->setValue(valueString);
    
    if(!element) { // if it doesn't exist
        
        vector<string> tokens;
        
        if(tag.find(':') != string::npos) {
            tokens = tokenize(tag, ":");
        }
        
        if(tag.find('/') != string::npos) {
            tokens = tokenize(tag, "/");
        }
        
        for(int i = 0; i < tokens.size(); i++) {
            Element *newElement = document->createElement(tokens.at(i));
            currentElement->appendChild(newElement);
            currentElement = newElement;
            
        }
    }
    
    element->attributes()->setNamedItem(attr);
    currentElement = element;
    
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
    
Element* ofxXmlSettings::getElement(const string& tag, const int which)
{
    string copy = tag;
    
    if(copy.find(":") != string::npos) {
        replace( copy.begin(), copy.end(), ':', '/');
    }
    
    if(which != 0) {
        // now the selector
        stringstream stream;
        stream << "[" << which << "]";
        copy.append(stream.str());
    }
    
    return (Element*) currentElement->getNodeByPath(copy); // we know this'll always be a node
    
}

