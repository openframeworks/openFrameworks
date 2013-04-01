
#pragma once

#include "ofMain.h"

#include <numeric>

#include <Poco/DOM/Document.h>
#include <Poco/DOM/DocumentFragment.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/DOMParser.h>

#include <Poco/DOM/DOMException.h>
#include <Poco/SAX/SAXException.h>
#include <Poco/XML/XMLString.h>
#include <Poco/XML/XMLWriter.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/DOMWriter.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Attr.h>
#include <Poco/DOM/Node.h>
#include <Poco/DOM/NodeIterator.h>
#include <Poco/DOM/NodeFilter.h>
#include <Poco/DOM/NamedNodeMap.h>  
#include <Poco/DOM/ChildNodesList.h>

using namespace Poco::XML;

/*class ofXmlElement {
public:
    
    ofXmlElement(const ofXmlElement &rhs);
    
    const ofXmlElement& operator =( const ofXmlElement& rhs );
    
    ofXmlElement(Element*& e) {
        element = e;
    }
    
    ~ofXmlElement() {
        if(element) {
            //element->release();
        }
    }
    
    Element* operator()() const {
        return element;
    }
    
private:
    
    Element *element;
    
};*/

class ofXmlDocument {
public:
    
    ofXmlDocument(Document*& d) {
        document = d;
    }
    
    // Q: what's this? why's it here?
    // A: So we can use auto_ptr with our Element. Element has a protected destructor,
    // which means auto_ptr can't call it. They want you to call release(), but auto_ptr
    // won't do that, so we get a wrapper to do it for us. When we get C++11 we'll use
    // unique_ptr and this won't be a problem.
    ~ofXmlDocument() {
        if(document) {
            //element->release();
        }
    }
    
    Document *getDocument(){
        return document;
    }
    
private:
    
    Document *document;
    
};

template <class T>
class deleter 
{ 
public:
    void operator()(T *p) 
    { 
        //p->release();
    }
    
}; 

class ofXml {
    
public:
    
    void deleteFunc() {
        //p->release();
    }
    
    /*static void deleteFunc(Node* p) {
        p->release();
    }*/
    
    ofXml();
    ~ofXml();
    
    ofXml( const ofXml& rhs );
    const ofXml& operator =( const ofXml& rhs );
    
    bool            addValue(const string& path, const string& value, bool createEntirePath = false);
    string          getValue();
    string          getValue(const string& path);
    bool            setValue(const string& path, const string& value);
    
    string          getName();
    
    string          getAttribute(const string& path);
    bool            addAttribute(const string& path, const string& value);
    bool            setAttribute(const string& path, const string& value);
    
    bool            clearAttributes(const string& path);
    bool            clearContents(const string& path);
    bool            clearContents();
    bool            clearAttributes();
    
    bool            remove(const string& path); // works for both attributes and tags
    bool            removeAll(const string& path); // works for both attributes and tags
    bool            exists(const string& path); // works for both attributes and tags
    
    void copyXmlToString(string & str);
    bool loadFromBuffer( string buffer );
    
    ofXml getParent();
    ofXml getChild(const string &path);
    ofXml getChildAt(int i);
    
    int getNumChildren();
    bool hasParent();
    vector<ofXml> getChildren();
    map<string, string> getAttributes();
    
    string toString();
    
    //Element* getElement();
    //Element* getElement(const string& path);
    
    ofPtr<Element> getElement();
    ofPtr<Element> getElement(const string& path);
    
    Document* getDocument();
    
    bool addChild( ofXml& child );
    void setName( string name );
    
    bool isEmpty() { return _empty; } 
    
       
protected:
    
    // this is how we get parent and child.
    // TODO should currentElement be unique_ptr?
    ofXml(const ofPtr<ofXmlDocument>& pdocument, ofPtr<Element> pcurrentElement);
    
    void addList( Element *addTo, Element *addFrom, bool topLevel = false);
    
    string DOMErrorMessage(short msg);
    
    Poco::XML::Document *documentRawPtr;
    ofPtr<ofXmlDocument> document;
    ofPtr<Element> element;
    
    bool _empty;
    
};
