
#include "ofXml.h"

//----------------------------------------
// a pretty useful tokenization system:
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
    Node *n = document->importNode(rhs.getDocument()->documentElement(), true);
    document->appendChild(n);
    
    element = document->documentElement();
}

const ofXml& ofXml::operator =( const ofXml& rhs ) {

    document = (Document*) rhs.document->cloneNode(true);
    element = document->documentElement();
    return *this;
}

ofXml::ofXml() {
    document = new Poco::XML::Document(); // we create this so that they can be merged later
    element = 0;
}

Document* ofXml::getDocument()  {
    
    return document;
}

Document* ofXml::getDocument() const  {
    
    return document;
}

int ofXml::getNumChildren()
{
    int numberOfChildren = 0;
    NodeList *list = element->childNodes();
    int i = 0;
    
    while(i < list->length()) {
        if(list->item(i) && list->item(i)->nodeType() == Node::ELEMENT_NODE) {
            numberOfChildren++;
        }
        i++;
    }

    return numberOfChildren;
}

string ofXml::toString()
{
    ostringstream stream;
    
    Poco::XML::DOMWriter writer;
    writer.setOptions(XMLWriter::PRETTY_PRINT);
    if(document != 0) {
        try {
            writer.writeNode( stream, getDocument() );
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

void ofXml::addXml( ofXml& xml ) {
    
    Node *n = document->importNode(xml.getDocument()->documentElement(), true);
    element->appendChild(n);
    
}

bool ofXml::addValue(const string& path, const string& value, bool createEntirePath)
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
        Element* firstElement, *lastElement;
        if(element) {
            lastElement = element;
        }
        
        if(!firstElement) {
            firstElement = lastElement;
        }
        
        // find the last existing tag
        int lastExistingTag = tokens.size();
        
        for(int i = 0; i < tokens.size(); i++)
        {
            Element* newElement = getDocument()->createElement(tokens.at(i));
            
            cout << " creating " << newElement->nodeName() << endl;
            
            if(lastElement) {
                lastElement->appendChild(newElement);
            }
            
            lastElement = newElement;
        }
        
        if(value != "")
        {

            Text *text = getDocument()->createTextNode(value);
            try {

                lastElement->appendChild( text );

            } catch ( DOMException &e ) {
                stringstream sstream;
                sstream << " cannot set node value " << DOMErrorMessage(e.code());
                ofLog(OF_LOG_ERROR, sstream.str());
                return false;
            }
        }
        
        if(!element) {
            element = firstElement;
            document->appendChild(element);
        }
        
        return true;
        
    } else {
        
        Element *newElement = getDocument()->createElement(path);
        
        if(value != "") {
            
            Text *text = getDocument()->createTextNode(value);
            try {
                newElement->appendChild(text);
                text->release();
                
            } catch ( DOMException &e ) {
                stringstream sstream;
                sstream << " cannot set node value " << DOMErrorMessage(e.code());
                ofLog(OF_LOG_ERROR, sstream.str());
                return false;
            }
        }
        
        if(element) {
            element->appendChild(newElement);
        } else {
            element = newElement;
        }
        
    }
    return true;
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
        Element *el = element;
        
        // find the last existing tag
        int lastExistingTag = tokens.size();
        
        vector<Element*> toBeReleased;
        
        for(int i = 0; i < tokens.size(); i++)
        {
            Element *pe = getDocument()->createElement(tokens.at(i));
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
        Element* pe = getDocument()->createElement(path);
        
        if(element) {
            element->appendChild(pe);
        } else {
            document->appendChild(pe);
            element = document->documentElement();
        }
    }
    return true;
}

string ofXml::getValue(const string& path) {
    
    if(path == "")
    {
        if(element->firstChild()->nodeType() == Node::TEXT_NODE) {
            return element->innerText();
        }
        return "";
    }
    else
    {
        Element *e = (Element*) element->getNodeByPath(path);
        if(e) {
            return e->innerText();
        }
    }
    
    return "";
}

string ofXml::getValue()
{
    if(element->firstChild()->nodeType() == Node::TEXT_NODE) {
        return element->innerText();
    }
    return "";
}

bool ofXml::resetCurrentElement() {
    if(element) {
        element = document->documentElement();
        return true;
    }
    ofLog(OF_LOG_WARNING, "No element set yet");
    return false;
}

bool ofXml::setCurrentElementToParent(int numLevelsUp) {
    if(element) {
        
        int i = 0;
        while( i < numLevelsUp ) {
            if(element->parentNode()) {
                element = (Element*) element->parentNode();
            } else {
                ofLog(OF_LOG_WARNING, "Too many parents in setCurrentElementToParent");
                return false;
            }
            i++;
        }
        
        return true;
    }
    
    ofLog(OF_LOG_WARNING, "No element set yet");
    return false;
    
}

bool ofXml::setCurrentElementToSibling()
{
    Element *node = (Element*) element->nextSibling();
    
    // empty space in the XML doc is treated as text nodes. blerg.
    while(node && node->nodeType() == Node::TEXT_NODE) {
        node = (Element*) node->nextSibling();
    }
    
    if(!node || node->nodeType() == Node::TEXT_NODE) {
        return false;
    }
    
    // we're cool
    element = node;
    return true;
}

bool ofXml::setCurrentElementToPrevSibling()
{
    Element *node = (Element*) element->previousSibling();
    
    // empty space in the XML doc is treated as text nodes. blerg.
    while(node && node->nodeType() == Node::TEXT_NODE) {
        node = (Element*) node->previousSibling();
    }
    
    if(!node || node->nodeType() == Node::TEXT_NODE) {
        return false;
    }

    element = node;
    return true;
}

bool ofXml::setValue(const string& path, const string& value)
{
    Element *e = (Element*) element->getNodeByPath(path);
    
    if(!e) {
        ofLog(OF_LOG_WARNING, " setValue of " + path + " failed because path doesn't exist");
        return false;
    }
    
    if(e->firstChild()->nodeType() == Node::TEXT_NODE) {
        Text *node = getDocument()->createTextNode(ofToString(value));
        e->replaceChild( (Node*) node, e->firstChild()); // swap out
        node->release();
    }
    return true;
}

string ofXml::getAttribute(const string& path) {

    Node *e = element->getNodeByPath(path);
    if(e) {
        return e->getNodeValue(); // this will be the value of the attribute
    }
    return "";
}

bool ofXml::clearAttributes(const string& path) 
{
    
    Element *e = (Element*) element->getNodeByPath(path);
    if(e) {
        NamedNodeMap *map = e->attributes();
        
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
        NamedNodeMap *map = element->attributes();
        
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
        NodeList *list = element->childNodes();
        for( int i = 0; i < list->length(); i++) {
            element->removeChild(list->item(i));
        }
        list->release();
        return true;
    }
    return false;
}

bool ofXml::clearContents(const string& path) {
    
    Element *e = (Element*) element->getNodeByPath(path);
    if(e) {
        NodeList *list = e->childNodes();
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
    Node *node = element->getNodeByPath(path);
    
    if(node) {
        Node *n = node->parentNode()->removeChild(node);
        n->release();
        return true;
    }
    return false;
}

bool ofXml::exists(const string& path) // works for both attributes and tags
{
    Node *node = element->getNodeByPath(path);
    
    if(node) {
        return true;
    }
    return false;
}

map<string, string> ofXml::getAttributes() // works for both attributes and tags
{
    
    map<string, string> attrMap;
    //NamedNodeMap *attr = element->attributes();
    Poco::AutoPtr<NamedNodeMap> attr = element->attributes();
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
    //Attr* attr = getDocument()->createAttribute(attributeName);
    Poco::AutoPtr<Attr> attr = getDocument()->createAttribute(attributeName);
    attr->setValue(value);
    
    if(!hasPath) {
        //NamedNodeMap *map = element->attributes();
        Poco::AutoPtr<NamedNodeMap> map = element->attributes();
        map->setNamedItem(attr);
        //map->release();
        //attr->release();
        return true; // and we're done
    }
    
    // we have a path to resolve
    Element* curElement = getElement(pathToAttribute);
    
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
                Element* testElement = getElement(concat);
                if(testElement) {
                    lastExistingTag++;
                    curElement = testElement;
                    break;
                }
            }
            
            // create all the tags that don't exist
            for(int i = lastExistingTag; i < tokens.size(); i++)
            {
                Element *newElement = getDocument()->createElement(tokens.at(i));
                curElement->appendChild(newElement);
                curElement = newElement;
                
            }
            
            cout << curElement->nodeName() << endl;
            
            //Poco::AutoPtr<NamedNodeMap> map = curElement->attributes();
            //map->setNamedItem(attr);
            
            curElement->setAttribute(attributeName, value);
            
            
            cout << " has attribute " << curElement->hasAttribute(attributeName) << endl;
            
            return true;
        }
        else
        {
            Element* testElement = getElement(pathToAttribute);
            if(testElement)
            {
                curElement = testElement;
            }
            else
            {
                Element *newElement = getDocument()->createElement(pathToAttribute);
                curElement->appendChild(newElement);
                curElement = newElement;
            }
            
            //Poco::AutoPtr<NamedNodeMap> map = curElement->attributes();
            //map->setNamedItem(attr);
            curElement->setAttribute(attributeName, value);
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------
bool ofXml::loadFromBuffer( string buffer )
{

    Poco::XML::DOMParser parser;
    
    // release and null out if we already have a document
    if(document) {
        document->release();
    }
    
    document = parser.parseString(buffer);

    if(document) {
        element = (Element*) document->firstChild();
        document->normalize();
        return true;
    }
    return false;
    
}

//---------------------------------------------------------
void ofXml::copyXmlToString(string & str)
{
    
    ostringstream stream;
    Poco::XML::DOMWriter writer;
    writer.writeNode( stream, document );

    str = stream.str();
    
}

Element* ofXml::getElement(const string& path)
{
    string copy = path;
    // does it have an attribute? just in case
    int ind = copy.find("[@");
    if(ind != string::npos) {
        copy = path.substr(0, ind);
    }
    //ofPtr<Element> nestedElement = ofPtr<Element>( (Element*) element->getNodeByPath(copy), deleter<Element>()); // we know this'll always be a node
    return (Element*) element->getNodeByPath(copy);
    //return nestedElement;
    
}

string ofXml::getCurrentElementName()
{
    if(element)
    {
        return element->nodeName();
    }
    return "";
}

bool ofXml::setCurrentElement(const string& path)
{
    
    // one case: we're at the root, but we don't know it yet:
    if(element == document->documentElement() && element->nodeName() == path ) {
        return true;
    }
    
    //cout << path << " " << path.find("../") << endl;
    
    // another: let's go up a little
    if( path.find("../") != string::npos)
    {
        
        Element* prev = element;
        Element* parent;
        int count = 0;
        size_t offset;
        for (offset = path.find("../");
             offset != std::string::npos;
             offset = path.find("../", offset + 3))
        {
            
            if(count == 0) {
                parent = (Element*) element->parentNode();
            } else {
                parent = (Element*) parent->parentNode();
            }
            ++count;
        }
        
        //cout << (count * 3) << " " << path.size() << endl;
        
        if( (count * 3) > path.size() - 1 ) {
            
            element = parent;
            return true;
            
        } else {
            
            string remainingPath = path.substr((count * 3), path.size() - (count * 3));
            
            element = (Element*) parent->getNodeByPath(remainingPath);
             if(!element) {
                 element = prev;
                 ofLog(OF_LOG_WARNING, "setCurrentElement passed invalid path");
                 return false;
             }
        }
    } else {
        // another: we're actually looking down into the thing :)
        Element* prev = element;
        element = (Element*) element->getNodeByPath(path);
        if(!element) {
            element = prev;
            ofLog(OF_LOG_WARNING, "setCurrentElement passed invalid path");
            return false;
        }
    }
    return true;
}

Element* ofXml::getCurrentElement()
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
    
}