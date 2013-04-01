
#include "ofXml.h"

/*ofXmlElement::ofXmlElement(const ofXmlElement &rhs) {
    //element = rhs.getElement();
    element = rhs();
}

const ofXmlElement& ofXmlElement::operator =( const ofXmlElement& rhs ) {
    //element = rhs.getElement();
    element = rhs();
    return *this;
}*/


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
    
    //element->release();
    
    // this is tricky. delete the document?
    if(documentRawPtr) {
        documentRawPtr->release();
    }
}

ofXml::ofXml( const ofXml& rhs ) {
    if(rhs.documentRawPtr) {
        documentRawPtr = rhs.documentRawPtr;
    } else {
        documentRawPtr = 0;
    }
    document = rhs.document;
    element = rhs.element;
    _empty = rhs._empty;
}

const ofXml& ofXml::operator =( const ofXml& rhs ) {
    if(rhs.documentRawPtr) {
        documentRawPtr = rhs.documentRawPtr;
    } else {
        documentRawPtr = 0;
    }
    document = rhs.document;
    element = rhs.element;
    _empty = rhs._empty;
    return *this;
}

ofXml::ofXml():_empty(true) {
    documentRawPtr = new Poco::XML::Document(); // we create this so that they can be merged later
    document = ofPtr<ofXmlDocument>(new ofXmlDocument(documentRawPtr));
    element = ofPtr<Element>( (Element*) getDocument(), deleter<Element>());
}

//ofXml::ofXml(const ofPtr<ofXmlDocument>& pdocument, Element* pCurrentElement) {
ofXml::ofXml(const ofPtr<ofXmlDocument>& pdocument, ofPtr<Element> pCurrentElement) {
    documentRawPtr = 0;
    document = pdocument;
    element = pCurrentElement;
    _empty = false;
}

string ofXml::getName()
{
    //return element->nodeName();
    return element->nodeName();
}

void ofXml::setName( string name ) {
    
}

Document* ofXml::getDocument()  {
    if(documentRawPtr) {
        return documentRawPtr;
    } else if(document) {
        return document->getDocument();
    }
    return NULL;
}

int ofXml::getNumChildren()
{
    //vector<ofXml> vec;
    //NodeList *list = element->childNodes();
    
    int numberOfChildren = 0;
    NodeList *list = element->childNodes();
    int i = 0;
    
    while(i < list->length()) {
        if(list->item(i) && list->item(i)->nodeType() == Node::ELEMENT_NODE) {
            //cout << list->item(i)->nodeName() << endl;
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
    if(documentRawPtr) {
        documentRawPtr->normalize();
        writer.writeNode( stream, documentRawPtr->documentElement() );
    } else {
        //writer.writeNode( stream, element );
        element->normalize();
        writer.writeNode( stream, element.get() );
    }
    
    string tmp = stream.str();
    // don't know how else to get rid of the hidden <#text></#text> nodes
    ofStringReplace(tmp, "<#text>", "");
    ofStringReplace(tmp, "</#text>", "");
    
    return tmp;
}

bool ofXml::addChild( ofXml& child )
{
    
    addList( element.get(), child.document->getDocument()->documentElement(), true);
    
}

void ofXml::addList( Element *addTo, Element *addFrom, bool topLevel)
{
    
    // TODO -
    // the top level node isn't having it's attributes set
    // or all the children added to it correctly, so if you make parent, everything needs
    // to get added to it.

    Element *localToAddTo;
    
    //cout << addTo->nodeName() << " " << addFrom->nodeName() << endl;
    
    //if(topLevel)
    //{
        // get the top level node
        localToAddTo = getDocument()->createElement(addFrom->nodeName());
        
        if(addFrom->hasAttributes()) {
            
            NamedNodeMap *map = addFrom->attributes();
            for(int i = 0; i < map->length(); i++) {
                //Attr *attr = getDocument()->createAttribute(map->item(i)->getNodeValue());
                Attr *attr = getDocument()->createAttribute(map->item(i)->nodeName());
                attr->setValue(map->item(i)->getNodeValue());
                localToAddTo->attributes()->setNamedItem(attr);
            }
            
            map->release();
        }
    
        if(addFrom->nodeValue() != "") {
            Text *text = getDocument()->createTextNode(addFrom->innerText());
            localToAddTo->appendChild((Node*) text);
        }
    
        // need to make sure parent is what all subsequent adds get put onto it
        //addTo->appendChild(parent);
    //} else {
    //    localToAddTo = addTo;
    //}
    
    // now look for all children
    
    NodeList *list = addFrom->childNodes();
    if(list)
    {
        unsigned long i = 0;
        while(list->item(i))
        {
            Element* node = getDocument()->createElement(list->item(i)->nodeName());
            
            ///////////
            // TODO - this makes <#text>value</#text>
            ///////////
            
            if(list->item(i)->nodeValue() != "") {
                Text *text = getDocument()->createTextNode(list->item(i)->innerText());
                node->appendChild((Node*) text);
            }
            
            if(list->item(i)->hasAttributes()) {
                
                NamedNodeMap *map = list->item(i)->attributes();
                for(int i = 0; i < map->length(); i++) {
                    Attr *attr = getDocument()->createAttribute(map->item(i)->getNodeValue());
                    attr->setValue(map->item(i)->getNodeValue());
                    node->appendChild(attr);
                }
                
                map->release();
            }
            
            if(list->item(i)->hasChildNodes()) // recurse-action
            {
                NodeList *childList = list->item(i)->childNodes();
                
                int j = 0;
                while(childList->item(j))
                {
                    addList( node, (Element*) childList->item(j), false );
                    j++;
                }
            }
            
            node->normalize();
            localToAddTo->appendChild( (Element*) node);
            i++;
        }
    }
    
    addTo->appendChild(localToAddTo);
    
    if( list ) { // we might not have a list of child elements
        list->release();
    }
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
        ofPtr<Element> el = element;
        
        // find the last existing tag
        int lastExistingTag = tokens.size();
        
        if(!createEntirePath)
        {
            // can't use reverse_iterator b/c accumulate doesn't like it
            for(vector<string>::iterator it = tokens.end(); it != tokens.begin(); it--) 
            {
                //Element *testElement = getElement(accumulate(tokens.begin(), it, string("")));
                ofPtr<Element> testElement = getElement(accumulate(tokens.begin(), it, string("")));
                if(testElement) {
                    lastExistingTag--;
                    el = testElement;
                    break;
                }
            }
        }
        
        // create all the tags that don't exist
        for(int i = lastExistingTag; i < tokens.size(); i++)
        {
            Element* ei = getDocument()->createElement(tokens.at(i));
            ofPtr<Element> newElement = ofPtr<Element>( ei, deleter<Element>());
            el->appendChild(newElement.get());
            el = newElement;
        }
        
        
        if(value != "") 
        {
            Text *text = getDocument()->createTextNode(value);
            try {
                element->appendChild( (Node*) text);
                //el->setNodeValue(value);
            } catch ( DOMException &e ) {
                stringstream sstream;
                sstream << " cannot set node value " << DOMErrorMessage(e.code());
                ofLog(OF_LOG_ERROR, sstream.str());
                return false;
            }
        }
        
        return true;
        
    } else {
        
        // get last tag in the tag list
        //Element *newElement = document->getDocument()->createElement(path);
        Element *newElement = getDocument()->createElement(path);
        
        if(value != "") {
            //Text *text = document->getDocument()->createTextNode(value);
            Text *text = getDocument()->createTextNode(value);
            try {
                newElement->appendChild( (Node*) text);
                //newElement->setNodeValue(value);
            } catch ( DOMException &e ) {
                stringstream sstream;
                sstream << " cannot set node value " << DOMErrorMessage(e.code());
                ofLog(OF_LOG_ERROR, sstream.str());
                return false;
            }
        }
        
        element->appendChild(newElement);
    }
    return true;
}

string ofXml::getValue(const string& path) {
    Element *e = (Element*) element->getNodeByPath(path);
    if(e) {
        return e->innerText();
    }
    return "";
}

string ofXml::getValue() {
    if(element) {
        return element->innerText();
    }
    return "";
}

bool ofXml::setValue(const string& path, const string& value)
{
    Element *e = (Element*) element->getNodeByPath(path);
    
    if(!element) {
        ofLog(OF_LOG_WARNING, " setValue of " + path + " failed because path doesn't exist");
        return false;
    }
    
    if(e->firstChild()->nodeType() == Node::TEXT_NODE) {
        //Text *node = document->getDocument()->createTextNode(ofToString(value));
        Text *node = getDocument()->createTextNode(ofToString(value));
        e->replaceChild( (Node*) node, e->firstChild()); // swap out
    }
    return true;
}

string ofXml::getAttribute(const string& path) {

    Node *e = element->getNodeByPath(path);
    
    return e->getNodeValue(); // this will be the value of the attribute
}

bool ofXml::setAttribute(const string& path, const string& value)
{
    
    string attributeName;
    bool hasPath = false;
    
    // you can pass either /node[@attr] or just attr
    if(path.find("[@") != string::npos)
    {
        int start = path.find("[@") + 2;
        int end = path.find("]", start);
        attributeName = path.substr( start, end );
        hasPath = true;
    }
    else
    {
        attributeName = path;
    }
    
    // we don't have a path to resolve
    Attr *attr = getDocument()->createAttribute(attributeName);
    attr->setValue(value);
    
    if(!hasPath) {
        element->attributes()->setNamedItem(attr);
        return true; // and we're done
    }
    
    // we do have a path to resolve
    Element* currentElement = (Element*) element->getNodeByPath(path);
    int begin = path.find("[@attr1='");
    string attribute = path.substr( begin, path.find("'", begin) - begin);
    
    if(!currentElement) { // if it doesn't exist
        
        vector<string> tokens;
        bool needsTokenizing = false;
        
        if(path.find('/') != string::npos) {
            tokens = tokenize(path, "/");
        }
        
        // is this a tokenized tag?
        if(tokens.size() > 1) 
        {
            // don't 'push' down into the new nodes
            Element* currentElement = element.get();
            
            // find the last existing tag
            int lastExistingTag = tokens.size();
            
            // can't use reverse_iterator b/c accumulate doesn't like it
            for(vector<string>::iterator it = tokens.end(); it != tokens.begin(); it--) 
            {
                Element *testElement = getElement(accumulate(tokens.begin(), it, string(""))).get();
                if(testElement) {
                    lastExistingTag--;
                    currentElement = testElement;
                    break;
                }
            }
            
            // create all the tags that don't exist
            for(int i = lastExistingTag; i < lastExistingTag; i++)
            {
                //Element *newElement = document->getDocument()->createElement(tokens.at(i));
                Element *newElement = getDocument()->createElement(tokens.at(i));
                currentElement->appendChild(newElement);
                currentElement = newElement;
            }
        }
    }
    
    currentElement->attributes()->setNamedItem(attr);
}

bool ofXml::clearAttributes(const string& path) 
{
    
    Element *e = (Element*) element->getNodeByPath(path);
    NamedNodeMap *map = e->attributes();
    
    for(int i = 0; i < map->length(); i++) {
        e->removeAttribute(map->item(i)->nodeName());
    }
    
    map->release();

}

bool ofXml::clearAttributes()
{
    
}

bool ofXml::clearContents() {
    NodeList *list = element->childNodes();
    for( int i = 0; i < list->length(); i++) {
        element->removeChild(list->item(i));
    }
}

bool ofXml::clearContents(const string& path) {
    Element *e = (Element*) element->getNodeByPath(path);

    NodeList *list = e->childNodes();
    for( int i = 0; i < list->length(); i++) {
        element->removeChild(list->item(i));
    }
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

ofXml ofXml::getParent() // works for both attributes and tags
{
    //cout << " calling get parent " << endl;
    if(element && element->parentNode()->nodeType() == Node::ELEMENT_NODE) {
        Element *pE = (Element*) element->parentNode();
        ofPtr<Element> ap( pE, deleter<Element>());
        ofXml parent(document, ap);
        return parent;
    }
    return ofXml();
}

ofXml ofXml::getChild(const string &path) // works for both attributes and tags
{
    //cout << " calling get child " << endl;
    //cout << element->nodeName() << " " << element->getNodeByPath(path)->nodeName() << endl;
    //cout << path << " " << element->nodeName() << endl;
    Element *e = (Element *) element->getNodeByPath(path);
    if(e) {
        ofPtr<Element> ap( e, deleter<Element>());
        ofXml child(document, ap);
        return child;
    }
    return ofXml();
}

ofXml ofXml::getChildAt(int i) // works for both attributes and tags
{
    if(element->hasChildNodes())   
    {
        vector<Element*> childElements;
        NodeList *list = element->childNodes();
        
        for( int j = 0; j < list->length(); j++) {
            if(list->item(j)->nodeType() == Node::ELEMENT_NODE) {
                childElements.push_back( (Element*) list->item(j));
            }
        }

        list->release();
        
        ofPtr<Element> ap( childElements[i], deleter<Element>());
        ofXml child(document, ap);
        return child;
    }
    
    // TODO fix this - shouldn't return new
    return ofXml();
}

vector<ofXml> ofXml::getChildren() // works for both attributes and tags
{
    
    vector<ofXml> vec;
    NodeList *list = element->childNodes();
    
    for( int i = 0; i < list->length(); i++) {
        //ofPtr<Element> ap( ( Element* )list->item(i) );
        if(list->item(i)->nodeType() == Node::ELEMENT_NODE) {
            Element* ei = ( Element* ) list->item(i);
            ofPtr<Element> ap(  ei , deleter<Element>());
            ofXml child(document, ap);
            vec.push_back(child);
        }
    }
    
    list->release();
    return vec;
}

map<string, string> ofXml::getAttributes() // works for both attributes and tags
{
    
    map<string, string> attrMap;
    NamedNodeMap *attr = element->attributes();
    for( int i = 0; i < attr->length(); i++) {
        attrMap[attr->item(i)->nodeName()] = attr->item(i)->nodeValue();
    }
    attr->release();
    return attrMap; 
}


//---------------------------------------------------------
bool ofXml::addAttribute(const string& path, const string& value)
{
    
    string attributeName;
    bool hasPath = false;

    // you can pass either /node[@attr] or just attr
    if(path.find("[@") != string::npos)
    {
        int start = path.find("[@") + 2;
        int end = path.find("]", start);
        attributeName = path.substr( start, end );
        hasPath = true;
    }
    else
    {
        attributeName = path;
    }
    
    // we don't have a path to resolve
    Attr *attr = getDocument()->createAttribute(attributeName);
    attr->setValue(value);
    
    if(!hasPath) {
        element->attributes()->setNamedItem(attr);
        return true; // and we're done
    }
    
    // we have a path to resolve
    Element* curElement = getElement(path).get();
    
    if(!curElement) { // if it doesn't exist
        
        vector<string> tokens;
        bool needsTokenizing = false;
        
        if(path.find('/') != string::npos) {
            tokens = tokenize(path, "/");
        }
        
        // is this a tokenized tag?
        if(tokens.size() > 1) 
        {
            // don't 'push' down into the new nodes
            curElement = element.get();
            
            // find the last existing tag
            int lastExistingTag = tokens.size();
            
            // can't use reverse_iterator b/c accumulate doesn't like it
            for(vector<string>::iterator it = tokens.end(); it != tokens.begin(); it--) 
            {
                string empty = "";
                string concat = accumulate(tokens.begin(), it, std::string());
                Element* testElement = getElement(concat).get();
                if(testElement) {
                    lastExistingTag--;
                    curElement = testElement;
                    break;
                }
            }
            
            // create all the tags that don't exist
            for(int i = lastExistingTag; i < lastExistingTag; i++)
            {
                //Element *newElement = document->getDocument()->createElement(tokens.at(i));
                Element *newElement = getDocument()->createElement(tokens.at(i));
                curElement->appendChild(newElement);
                curElement = newElement;
            }
            
            
            if(value != "") 
            {
                //Text *text = document->getDocument()->createTextNode(value);
                Text *text = getDocument()->createTextNode(value);
                try {
                    curElement->appendChild( (Node*) text);
                } catch ( DOMException &e ) {
                    stringstream sstream;
                    sstream << " cannot set node value " << DOMErrorMessage(e.code());
                    ofLog(OF_LOG_ERROR, sstream.str());
                    return false;
                }
            }
        }
    }
    
    curElement->attributes()->setNamedItem(attr);
    return true;
}

//---------------------------------------------------------
bool ofXml::loadFromBuffer( string buffer )
{

    _empty = false;
    int size = buffer.size();

    //bool loadOkay = doc.ReadFromMemory( buffer.c_str(), size);//, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING);
    Poco::XML::DOMParser parser;
    
    // release and null out if we already have a document
    if(document) {
        documentRawPtr->release();
    }
    
    documentRawPtr = parser.parseString(buffer);
    
    if(documentRawPtr) {
        document = ofPtr<ofXmlDocument>(new ofXmlDocument(documentRawPtr));
        element = ofPtr<Element>( (Element*) documentRawPtr->documentElement(), deleter<Element>());
        return true;
    }
    return false;

}

//---------------------------------------------------------
void ofXml::copyXmlToString(string & str)
{
	/*TiXmlPrinter printer;
	doc.Accept(&printer);

	str = printer.CStr();*/
    
    ostringstream stream;
    Poco::XML::DOMWriter writer;
    writer.writeNode( stream, documentRawPtr );

    str = stream.str();
    
}

bool ofXml::hasParent()
{
    if(element->parentNode() && 
       element->parentNode()->nodeType() == Node::ELEMENT_NODE) {
        return true;
    }
    return false;
}

ofPtr<Element> ofXml::getElement(const string& path)
{
    string copy = path;
    // does it have an attribute? just in case
    int ind = copy.find("[@");
    if(ind != string::npos) {
        copy = path.substr(0, ind);
    }
    ofPtr<Element> nestedElement = ofPtr<Element>( (Element*) element->getNodeByPath(copy), deleter<Element>()); // we know this'll always be a node
    return nestedElement;
    
}

ofPtr<Element> ofXml::getElement()
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