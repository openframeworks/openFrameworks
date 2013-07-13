
#include "ofXml.h"

ofXml::~ofXml() {
    
    if(document) {
        document->release();
        document = 0;
    }
    
    if(element) {
        element->release();
        element = 0;
    }
}

ofXml::ofXml( const ofXml& rhs ) {

    document = new Poco::XML::Document();
    Poco::XML::Node *n = document->importNode(rhs.getPocoDocument()->documentElement(), true);
    document->appendChild(n);
    
    element = document->documentElement();
}

const ofXml& ofXml::operator =( const ofXml& rhs ) {

    document = (Poco::XML::Document*) rhs.document->cloneNode(true);
    element = document->documentElement();
    return *this;
}

ofXml::ofXml() {
    document = new Poco::XML::Document(); // we create this so that they can be merged later
    element = document->documentElement();
}

bool ofXml::load(const string & path){
	ofFile xmlFile(path,ofFile::ReadOnly);
	ofBuffer xmlBuffer(xmlFile);
	return loadFromBuffer(xmlBuffer);
}

bool ofXml::save(const string & path){
    ofBuffer buffer(toString());
    ofFile file(path, ofFile::ReadWrite);
    return file.writeFromBuffer(buffer);
}

void ofXml::serialize(const ofAbstractParameter & parameter){
	if(!parameter.isSerializable()) return;
	string name = parameter.getEscapedName();
	if(name=="") name="UnknownName";
	if(parameter.type()==typeid(ofParameterGroup).name()){
		const ofParameterGroup & group = static_cast<const ofParameterGroup&>(parameter);
		if(!exists(name)){
			addChild(name);
			cout << "creating group " << name << endl;
		}
		setTo(name);
		cout << "group " << name << endl;
		for(int i=0;i<group.size();i++){
			serialize(group.get(i));
		}
		cout << "end group " << name << endl;
		setToParent();
	}else{
		string value = parameter.toString();
		if(!exists(name)){
			addChild(name);
			cout << "creating tag " << name << endl;
		}
		cout << "setting tag " << name << ": " << value << endl;
		setValue(name,value);
	}
}

void ofXml::deserialize(ofAbstractParameter & parameter){
	if(!parameter.isSerializable()) return;
	string name = parameter.getEscapedName();
	if(parameter.type()==typeid(ofParameterGroup).name()){
		ofParameterGroup & group = static_cast<ofParameterGroup&>(parameter);
		if(setTo(name)){
			for(int i=0;i<group.size();i++){
				deserialize(group.get(i));
			}
			setToParent();
		}
	}else{
		if(exists(name)){
			if(parameter.type()==typeid(ofParameter<int>).name()){
				parameter.cast<int>() = getIntValue(name);
			}else if(parameter.type()==typeid(ofParameter<float>).name()){
				parameter.cast<float>() = getFloatValue(name);
			}else if(parameter.type()==typeid(ofParameter<bool>).name()){
				parameter.cast<bool>() = getBoolValue(name);
			}else if(parameter.type()==typeid(ofParameter<string>).name()){
				parameter.cast<string>() = getValue(name);
			}else{
				parameter.fromString(getValue(name));
			}
		}
	}

}

int ofXml::getNumChildren() const
{
	if(!element) return 0;

    int numberOfChildren = 0;
    Poco::XML::NodeList *list = element->childNodes();
    
    for(int i=0; i < (int)list->length(); i++) {
        if(list->item(i) && list->item(i)->nodeType() == Poco::XML::Node::ELEMENT_NODE) {
            numberOfChildren++;
        }
    }

    return numberOfChildren;
}

int ofXml::getNumChildren(const string& path) const
{
	if(!element) return 0;

    int numberOfChildren = 0;
    Poco::XML::NodeList *list = element->childNodes();

    for(int i=0; i < (int)list->length(); i++) {
        if(list->item(i) && list->item(i)->nodeType() == Poco::XML::Node::ELEMENT_NODE) {
            string nodeName = list->item(i)->localName();
            if(path.compare(nodeName) == 0) {
                numberOfChildren++;
            }
        }
    }
    
    return numberOfChildren;
    
}

string ofXml::toString() const
{
    ostringstream stream;
    
    Poco::XML::DOMWriter writer;
    writer.setOptions(Poco::XML::XMLWriter::PRETTY_PRINT);
    if(document) {
        try {
            writer.writeNode( stream, getPocoDocument() );
        } catch( exception e ) {
            ofLogError() << e.what();
        }
    } else if(element){
        element->normalize();
        writer.writeNode( stream, element );
    }
    
    string tmp = stream.str();
    
    // don't know how else to get rid of the hidden <#text></#text> nodes :/
    ofStringReplace(tmp, "<#text>", "");
    ofStringReplace(tmp, "</#text>", "");
    
    return tmp;
}

void ofXml::addXml( ofXml& xml, bool copyAll ) {
    
    Poco::XML::Node *n = 0;
    if(copyAll) {
        n = document->importNode(xml.getPocoDocument()->documentElement(), true);
    } else {
        if(xml.getPocoElement() == 0 || xml.getPocoElement() == xml.getPocoDocument()->documentElement()) {
            n = document->importNode(xml.getPocoDocument()->documentElement(), true);
        } else {
            n = document->importNode( xml.getPocoElement(), true);
        }
    }

    // we have an element, i.e. the document has child nodes
    // or we don't, so append it directly to the document
    if( element ) {
        element->appendChild(n);
    } else {
        document->appendChild(n);
    }
    
}

bool ofXml::addChild( const string& path )
{
    vector<string> tokens;
    
    if(path.find('/') != string::npos) {
        tokens = tokenize(path, "/");
    }
    
    // is this a tokenized tag?
    if(tokens.size() > 1) 
    {
        // don't 'push' down into the new nodes
        Poco::XML::Element *el = element;
        
        vector<Poco::XML::Element*> toBeReleased;
        
        for(int i = 0; i < (int)tokens.size(); i++)
        {
            Poco::XML::Element *pe = getPocoDocument()->createElement(tokens.at(i));
            el->appendChild(pe);
            toBeReleased.push_back(pe);
            el = pe;
        }
        
        if(element) {
            element->appendChild(el);
        } else {
            element = el;
        }

        return true;
        
    } else {
        Poco::XML::Element* pe = getPocoDocument()->createElement(path);
        
        if(element) {
            element->appendChild(pe);
        } else {
            document->appendChild(pe);
            element = document->documentElement();
        }
    }
    return true;
}

string ofXml::getValue() const
{
    if(element && element->firstChild()->nodeType() == Poco::XML::Node::TEXT_NODE) {
        return element->innerText();
    }
    return "";
}

string ofXml::getValue(const string & path) const{
	return getValue<string>(path, "");
}

int ofXml::getIntValue() const{
	return ofToInt(getValue());
}

int	ofXml::getIntValue(const string & path) const{
	return getValue<int>(path,0);
}

float ofXml::getFloatValue() const{
	return ofToFloat(getValue());
}

float ofXml::getFloatValue(const string & path) const{
	return getValue<float>(path,0.0);
}

bool ofXml::getBoolValue() const{
	return ofToBool(getValue());
}

bool ofXml::getBoolValue(const string & path) const{
	return getValue<bool>(path,false);
}


bool ofXml::reset() {
    if(element) {
        element = document->documentElement();
        return true;
    }
    ofLog(OF_LOG_WARNING, "No element set yet");
    return false;
}

bool ofXml::setToChild(int index)
{
    
    if(!element) {
        if((Poco::XML::Element*) document->documentElement()->firstChild()) {
            element = (Poco::XML::Element*) document->documentElement()->firstChild();
        } else {
            ofLogWarning() << " setToChild() no element created yet " << endl;
        }
    }
    
    int numberOfChildren = 0;
    Poco::XML::NodeList *list = element->childNodes();

    for(int i=0; i < (int)list->length() && numberOfChildren < index + 1; i++) {
        if(list->item(i) && list->item(i)->nodeType() == Poco::XML::Node::ELEMENT_NODE) {
            if(numberOfChildren == index) {
                element = (Poco::XML::Element*) list->item(i);
                return true;
            }
            numberOfChildren++;
        }
    }
    
    return false;
    
}

bool ofXml::setToParent()
{
    if(element->parentNode()) {
        element = (Poco::XML::Element*) element->parentNode();
    } else {
        ofLog(OF_LOG_WARNING, "current element has no parent");
        return false;
    }
    return true;

}

bool ofXml::setToParent(int numLevelsUp) {
    if(element) {
        
        int i = 0;
        while( i < numLevelsUp ) {
            if(element->parentNode()) {
                element = (Poco::XML::Element*) element->parentNode();
            } else {
                ofLog(OF_LOG_WARNING, "Too many parents in setToParent");
                return false;
            }
            i++;
        }
        
        return true;
    }
    
    ofLog(OF_LOG_WARNING, "No element set yet");
    return false;
    
}

bool ofXml::setToSibling()
{
    Poco::XML::Element *node;
    if(element) {
        node = (Poco::XML::Element*) element->nextSibling();
    } else {
        ofLog(OF_LOG_WARNING, "No element set yet");
        return false;
    }
    
    // empty space in the XML doc is treated as text nodes. blerg.
    while(node && node->nodeType() == Poco::XML::Node::TEXT_NODE) {
        node = (Poco::XML::Element*) node->nextSibling();
    }
    
    if(!node || node->nodeType() == Poco::XML::Node::TEXT_NODE) {
        return false;
    }
    
    // we're cool
    element = node;
    return true;
}

bool ofXml::setToPrevSibling()
{
    Poco::XML::Element *node;
    if(element) {
        node = (Poco::XML::Element*) element->previousSibling();
    } else {
        ofLog(OF_LOG_WARNING, "No element set yet");
        return false;
    }
    
    // empty space in the XML doc is treated as text nodes. blerg.
    while(node && node->nodeType() == Poco::XML::Node::TEXT_NODE) {
        node = (Poco::XML::Element*) node->previousSibling();
    }
    
    if(!node || node->nodeType() == Poco::XML::Node::TEXT_NODE) {
        return false;
    }

    element = node;
    return true;
}

bool ofXml::setValue(const string& path, const string& value)
{
    Poco::XML::Element *e;
    if(element) {
        e = (Poco::XML::Element*) element->getNodeByPath(path);
    } else {
        ofLog(OF_LOG_WARNING, "No element set yet");
        return false;
    }
    
    if(!e) {
        ofLogWarning("ofXml", " setValue of " + path + " failed because path doesn't exist");
        return false;
    }
    
    if(e->firstChild()->nodeType() == Poco::XML::Node::TEXT_NODE) {
        Poco::XML::Text *node = getPocoDocument()->createTextNode(ofToString(value));
        e->replaceChild( (Poco::XML::Node*) node, e->firstChild()); // swap out
        node->release();
    }
    return true;
}

string ofXml::getAttribute(const string& path) const {

    Poco::XML::Node *e;
    if(element) {
        e = element->getNodeByPath(path);
    } else {
        ofLog(OF_LOG_WARNING, "No element set yet");
        return false;
    }
    
    if(e) {
        return e->getNodeValue(); // this will be the value of the attribute
    }
    return "";
}

bool ofXml::clearAttributes(const string& path) 
{
    Poco::XML::Element *e;
    if(element) {
        e = (Poco::XML::Element*) element->getNodeByPath(path);
    } else {
        ofLog(OF_LOG_WARNING, "No element set yet");
        return false;
    }
    
    if(e) {
        Poco::XML::NamedNodeMap *map = e->attributes();
        
        for(int i = 0; i < (int)map->length(); i++) {
            e->removeAttribute(map->item(i)->nodeName());
        }
        
        map->release();
        return true;
    }
    return false;
}

bool ofXml::clearAttributes()
{

    if(element) {
        Poco::XML::NamedNodeMap *map = element->attributes();
        
        for(int i = 0; i < (int)map->length(); i++) {
            element->removeAttribute(map->item(i)->nodeName());
        }
        
        map->release();
        return true;
    }
    ofLog(OF_LOG_WARNING, "No element set yet");
    return false;

}

bool ofXml::clearContents() {
    if(element)
    {
        Poco::XML::NodeList *list = element->childNodes();
        for( int i = 0; i < (int)list->length(); i++) {
            element->removeChild(list->item(i));
        }
        list->release();
        return true;
    }
    return false;
}

bool ofXml::clearContents(const string& path) {
    
    Poco::XML::Element *e;
    if(element) {
        e = (Poco::XML::Element*) element->getNodeByPath(path);
    } else {
        ofLog(OF_LOG_WARNING, "No element set yet");
        return false;
    }
    
    if(e) {
        Poco::XML::NodeList *list = e->childNodes();
        for( int i = 0; i < (int)list->length(); i++) {
            element->removeChild(list->item(i));
        }
        list->release();
        return true;
    }
    return false;
}


bool ofXml::remove(const string& path) // works for both attributes and tags
{
    Poco::XML::Node *node;
    if(element) {
        node = element->getNodeByPath(path);
    } else {
        ofLog(OF_LOG_WARNING, "No element set yet");
        return false;
    }
    
    if(node) {
        Poco::XML::Node *n = node->parentNode()->removeChild(node);
        n->release();
        return true;
    }
    return false;
}

bool ofXml::exists(const string& path) const // works for both attributes and tags
{
    Poco::XML::Node *node;
    if(element) {
        node = element->getNodeByPath(path);
    } else {
        ofLog(OF_LOG_WARNING, "No element set yet");
        return false;
    }
    
    if(node) {
        return true;
    }
    return false;
}

map<string, string> ofXml::getAttributes() const // works for both attributes and tags
{
    
    map<string, string> attrMap;
    
    if(element){
    
        Poco::AutoPtr<Poco::XML::NamedNodeMap> attr = element->attributes();
        for( int i = 0; i < (int)attr->length(); i++) {
            attrMap[attr->item(i)->nodeName()] = attr->item(i)->nodeValue();
        }
    } else {
        ofLogWarning() << " getAttribute() No element set " << endl;
    }

    return attrMap; 
}


//---------------------------------------------------------
bool ofXml::setAttribute(const string& path, const string& value)
{
    
    string attributeName, pathToAttribute;
    bool hasPath = false;

    // you can pass either /node[@attr] or just attr
    if(path.find("[@") != string::npos)
    {
        int attrBegin = path.find("[@");
        int start = attrBegin + 2;
        int end = path.find("]", start);
        attributeName = path.substr( start, end - start );
        pathToAttribute = path.substr(0, attrBegin);
        hasPath = true;
    }
    else
    {
        attributeName = path;
    }
    
    // we don't have a path to resolve
    Poco::AutoPtr<Poco::XML::Attr> attr = getPocoDocument()->createAttribute(attributeName);
    attr->setValue(value);
    
    if(!hasPath) {
        Poco::AutoPtr<Poco::XML::NamedNodeMap> map = element->attributes();
        map->setNamedItem(attr);
        return true; // and we're done
    }
    
    // we have a path to resolve
    Poco::XML::Element* curElement = getPocoElement(pathToAttribute);
    
    if(!curElement) { // if it doesn't exist
        
        vector<string> tokens;
        
        if(path.find('/') != string::npos) {
            tokens = tokenize(pathToAttribute, "/");
        }
        
        // is this a tokenized tag?
        if(tokens.size() > 1) 
        {
            // don't 'push' down into the new nodes
            curElement = element;
            
            // find the last existing tag
            int lastExistingTag = 0;
            
            // can't use reverse_iterator b/c accumulate doesn't like it
            for(vector<string>::iterator it = tokens.end(); it != tokens.begin(); it--) 
            {
                string empty = "";
                string concat = accumulate(tokens.begin(), it, std::string());
                Poco::XML::Element* testElement = getPocoElement(concat);
                if(testElement) {
                    lastExistingTag++;
                    curElement = testElement;
                    break;
                }
            }
            
            // create all the tags that don't exist
            for(int i = lastExistingTag; i < (int)tokens.size(); i++)
            {
                Poco::XML::Element *newElement = getPocoDocument()->createElement(tokens.at(i));
                curElement->appendChild(newElement);
                curElement = newElement;
                
            }
            
            curElement->setAttribute(attributeName, value);
            return true;
        }
        else
        {
            Poco::XML::Element* testElement = getPocoElement(pathToAttribute);
            if(testElement)
            {
                curElement = testElement;
            }
            else
            {
                Poco::XML::Element *newElement = getPocoDocument()->createElement(pathToAttribute);
                curElement->appendChild(newElement);
                curElement = newElement;
            }
            
            curElement->setAttribute(attributeName, value);
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------
bool ofXml::loadFromBuffer( const string& buffer )
{
    Poco::XML::DOMParser parser;
    
    // release and null out if we already have a document
    if(document) {
        document->release();
    }
    
    try {
        document = parser.parseString(buffer);
    	element = (Poco::XML::Element*) document->firstChild();
    	document->normalize();
    	return true;
    } catch( exception e ) {
        short msg = atoi(e.what());
        ofLogWarning() << "Error loading XML: " << DOMErrorMessage(msg);
        document = new Poco::XML::Document;
        element = document->documentElement();
        cout << "element " << element << endl;
        return false;
    }
}


string ofXml::getName() const
{
    if(element)
    {
        return element->nodeName();
    }
    return "";
}

bool ofXml::setTo(const string& path)
{
    
    if(!element) {
        if(document->documentElement()) {
            element = document->documentElement();
        } else {
            ofLog(OF_LOG_WARNING, " empty document ");
            return false;
        }
    }
        
    
    // one case: we're at the root, but we don't know it yet:
    if(element == document->documentElement() && element->nodeName() == path ) {
        return true;
    }
    
    //cout << path << " " << path.find("../") << endl;
    
    // another: let's go up a little
    if( path.find("../") != string::npos)
    {
        
        Poco::XML::Element* prev = element;
        Poco::XML::Element* parent;
        int count = 0;
        size_t offset;
        for (offset = path.find("../");
             offset != std::string::npos;
             offset = path.find("../", offset + 3))
        {
            
            if(count == 0) {
                parent = (Poco::XML::Element*) element->parentNode();
            } else {
                parent = (Poco::XML::Element*) parent->parentNode();
            }
            ++count;
        }
        
        //cout << (count * 3) << " " << path.size() << endl;
        
        if( (count * 3) > (int)path.size() - 1 ) {
            
            element = parent;
            return true;
            
        } else {
            
            string remainingPath = path.substr((count * 3), path.size() - (count * 3));
            
            element = (Poco::XML::Element*) parent->getNodeByPath(remainingPath);
             if(!element) {
                 element = prev;
                 ofLog(OF_LOG_WARNING, "setCurrentElement() passed invalid path");
                 return false;
             }
        }
    }  else if(path.find("//") != string::npos) {
        
        // another: we're looking all over
        Poco::XML::Element* prev = element;
        element = (Poco::XML::Element*) document->getNodeByPath(path);
        if(!element) {
            element = prev;
            ofLog(OF_LOG_WARNING, "setCurrentElement() passed invalid path");
            return false;
        }
        
    } else {
        // another: we're actually looking down into the thing :)
        Poco::XML::Element* prev = element;
        element = (Poco::XML::Element*) element->getNodeByPath(path);
        if(!element) {
            element = prev;
            ofLog(OF_LOG_WARNING, "setCurrentElement() passed invalid path");
            return false;
        }
    }
    return true;
}

const Poco::XML::Element* ofXml::getPocoElement() const
{
    return element;
}

Poco::XML::Element* ofXml::getPocoElement() 
{
	return element;
}

Poco::XML::Element* ofXml::getPocoElement(const string& path)
{
    string copy = path;
    // does it have an attribute? just in case
    int ind = copy.find("[@");
    if(ind != (int)string::npos) {
        copy = path.substr(0, ind);
    }
    
    if(element) {
        return (Poco::XML::Element*) element->getNodeByPath(copy);
    } else {
        ofLogWarning() << " getPocoElement() No element to get yet " << endl;
        return NULL;
    }

}

const Poco::XML::Element* ofXml::getPocoElement(const string& path) const
{
    string copy = path;
    // does it have an attribute? just in case
    int ind = copy.find("[@");
    if(ind != (int)string::npos) {
        copy = path.substr(0, ind);
    }

    if(element) {
        return (Poco::XML::Element*) element->getNodeByPath(copy);
    } else {
        ofLogWarning() << " getPocoElement() No element to get yet " << endl;
        return NULL;
    }

}

Poco::XML::Document* ofXml::getPocoDocument()  {

    return document;
}

const Poco::XML::Document* ofXml::getPocoDocument() const  {

    return document;
}

string ofXml::DOMErrorMessage(short msg)
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
    
    return "DOM ERROR";
    
}
