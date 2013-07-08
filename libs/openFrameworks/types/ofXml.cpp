
#include "ofXml.h"

// templated to be anything
//template <class T = string> void addValue(const string& path, T data, bool createEntirePath = false);

////----------------------------------------
//// a pretty useful tokenization system:
//static vector<string> tokenize(const string & str, const string & delim)
//{
//    vector<string> tokens;
//    
//    size_t p0 = 0, p1 = string::npos;
//    while(p0 != string::npos)
//    {
//        p1 = str.find_first_of(delim, p0);
//        if(p1 != p0)
//        {
//            string token = str.substr(p0, p1 - p0);
//            tokens.push_back(token);
//        }
//        p0 = str.find_first_not_of(delim, p1);
//    }
//    return tokens;
//}

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
    element = 0;
}

Poco::XML::Document* ofXml::getPocoDocument()  {
    
    return document;
}

Poco::XML::Document* ofXml::getPocoDocument() const  {
    
    return document;
}

int ofXml::getNumChildren() const
{
    int numberOfChildren = 0;
    Poco::XML::NodeList *list = element->childNodes();
    int i = 0;
    
    while(i < list->length()) {
        if(list->item(i) && list->item(i)->nodeType() == Poco::XML::Node::ELEMENT_NODE) {
            numberOfChildren++;
        }
        i++;
    }

    return numberOfChildren;
}

int ofXml::getNumChildren(const string& path) const
{
    int numberOfChildren = 0;
    Poco::XML::NodeList *list = element->childNodes();
    int i = 0;
    
    while(i < list->length()) {
        
        if(list->item(i) && list->item(i)->nodeType() == Poco::XML::Node::ELEMENT_NODE) {
            string nodeName = list->item(i)->localName();
            if(path.compare(nodeName) == 0) {
                numberOfChildren++;
            }
        }
        i++;
    }
    
    return numberOfChildren;
    
}

string ofXml::toString() const
{
    ostringstream stream;
    
    Poco::XML::DOMWriter writer;
    writer.setOptions(Poco::XML::XMLWriter::PRETTY_PRINT);
    if(document != 0) {
        try {
            writer.writeNode( stream, getPocoDocument() );
        } catch( exception e ) {
            cout << e.what() << endl;
        }
    } else {
        element->normalize();
        writer.writeNode( stream, element );
    }
    
    string tmp = stream.str();
    
    // don't know how else to get rid of the hidden <#text></#text> nodes :/
    ofStringReplace(tmp, "<#text>", "");
    ofStringReplace(tmp, "</#text>", "");
    
    return tmp;
}

void ofXml::addXml( const ofXml& xml, bool copyAll ) {
    
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
    element->appendChild(n);
    
}


bool ofXml::addChild( const string& path )
{
    vector<string> tokens;
    bool needsTokenizing = false;
    
    if(path.find('/') != string::npos) {
        tokens = tokenize(path, "/");
    }
    
    // is this a tokenized tag?
    if(tokens.size() > 1) 
    {
        // don't 'push' down into the new nodes
        Poco::XML::Element *el = element;
        
        // find the last existing tag
        int lastExistingTag = tokens.size();
        
        vector<Poco::XML::Element*> toBeReleased;
        
        for(int i = 0; i < tokens.size(); i++)
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
    if(element->firstChild()->nodeType() == Poco::XML::Node::TEXT_NODE) {
        return element->innerText();
    }
    return "";
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
    
    if(!element) { // not sure how this could happen, but just in case
        element = (Poco::XML::Element*) document->documentElement()->firstChild();
    }
    
    int numberOfChildren = 0;
    Poco::XML::NodeList *list = element->childNodes();
    int i = 0, j = 0;
    
    while(i < list->length() && numberOfChildren < index + 1) {
        if(list->item(i) && list->item(i)->nodeType() == Poco::XML::Node::ELEMENT_NODE) {
            if(numberOfChildren == index) {
                element = (Poco::XML::Element*) list->item(i);
                return true;
            }
            numberOfChildren++;
        }
        i++;
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
    Poco::XML::Element *node = (Poco::XML::Element*) element->nextSibling();
    
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
    Poco::XML::Element *node = (Poco::XML::Element*) element->previousSibling();
    
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
    Poco::XML::Element *e = (Poco::XML::Element*) element->getNodeByPath(path);
    
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

    Poco::XML::Node *e = element->getNodeByPath(path);
    if(e) {
        return e->getNodeValue(); // this will be the value of the attribute
    }
    return "";
}

bool ofXml::clearAttributes(const string& path) 
{
    
    Poco::XML::Element *e = (Poco::XML::Element*) element->getNodeByPath(path);
    if(e) {
        Poco::XML::NamedNodeMap *map = e->attributes();
        
        for(int i = 0; i < map->length(); i++) {
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
        
        for(int i = 0; i < map->length(); i++) {
            element->removeAttribute(map->item(i)->nodeName());
        }
        
        map->release();
        return true;
    }
    return false;

}

bool ofXml::clearContents() {
    if(element)
    {
        Poco::XML::NodeList *list = element->childNodes();
        for( int i = 0; i < list->length(); i++) {
            element->removeChild(list->item(i));
        }
        list->release();
        return true;
    }
    return false;
}

bool ofXml::clearContents(const string& path) {
    
    Poco::XML::Element *e = (Poco::XML::Element*) element->getNodeByPath(path);
    if(e) {
        Poco::XML::NodeList *list = e->childNodes();
        for( int i = 0; i < list->length(); i++) {
            element->removeChild(list->item(i));
        }
        list->release();
        return true;
    }
    return false;
}


bool ofXml::remove(const string& path) // works for both attributes and tags
{
    Poco::XML::Node *node = element->getNodeByPath(path);
    
    if(node) {
        Poco::XML::Node *n = node->parentNode()->removeChild(node);
        n->release();
        return true;
    }
    return false;
}

bool ofXml::exists(const string& path) const // works for both attributes and tags
{
    Poco::XML::Node *node = element->getNodeByPath(path);
    
    if(node) {
        return true;
    }
    return false;
}

map<string, string> ofXml::getAttributes() const // works for both attributes and tags
{
    
    map<string, string> attrMap;
    Poco::AutoPtr<Poco::XML::NamedNodeMap> attr = element->attributes();
    for( int i = 0; i < attr->length(); i++) {
        attrMap[attr->item(i)->nodeName()] = attr->item(i)->nodeValue();
    }
    //attr->release();
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
        bool needsTokenizing = false;
        
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
            for(int i = lastExistingTag; i < tokens.size(); i++)
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
    } catch( exception e ) {
        
        short msg = atoi(e.what());
        ofLogWarning("ofXml", DOMErrorMessage(msg));
        document = 0;
    }

    if(document) {
        element = (Poco::XML::Element*) document->firstChild();
        document->normalize();
        return true;
    }

    document = new Poco::XML::Document();
    element = (Poco::XML::Element*) document->documentElement();
    
    ofLogWarning("ofXml", " Can't create Document ");
    return false;
    
}


Poco::XML::Element* ofXml::getPocoElement(const string& path) const
{
    string copy = path;
    // does it have an attribute? just in case
    int ind = copy.find("[@");
    if(ind != string::npos) {
        copy = path.substr(0, ind);
    }

    return (Poco::XML::Element*) element->getNodeByPath(copy);
    
}

Poco::XML::Element* ofXml::getPocoElement(const string& path) {
	return getPocoElement(path);
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
        
        if( (count * 3) > path.size() - 1 ) {
            
            element = parent;
            return true;
            
        } else {
            
            string remainingPath = path.substr((count * 3), path.size() - (count * 3));
            
            element = (Poco::XML::Element*) parent->getNodeByPath(remainingPath);
             if(!element) {
                 element = prev;
                 ofLog(OF_LOG_WARNING, "setCurrentElement passed invalid path");
                 return false;
             }
        }
    }  else if(path.find("//") != string::npos) {
        
        // another: we're looking all over
        Poco::XML::Element* prev = element;
        element = (Poco::XML::Element*) document->getNodeByPath(path);
        if(!element) {
            element = prev;
            ofLog(OF_LOG_WARNING, "setCurrentElement passed invalid path");
            return false;
        }
        
    } else {
        // another: we're actually looking down into the thing :)
        Poco::XML::Element* prev = element;
        element = (Poco::XML::Element*) element->getNodeByPath(path);
        if(!element) {
            element = prev;
            ofLog(OF_LOG_WARNING, "setCurrentElement passed invalid path");
            return false;
        }
    }
    return true;
}

Poco::XML::Element* ofXml::getPocoElement() const
{
    return element;
}

Poco::XML::Element* ofXml::getPocoElement() 
{
	return element;
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