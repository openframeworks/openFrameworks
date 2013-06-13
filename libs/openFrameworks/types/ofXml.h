
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
#include <Poco/DOM/Text.h>
#include <Poco/DOM/NodeIterator.h>
#include <Poco/DOM/NodeFilter.h>
#include <Poco/DOM/NamedNodeMap.h>  
#include <Poco/DOM/ChildNodesList.h>

using namespace Poco::XML;

class ofXml {
    
public:
    
    ofXml();
    ~ofXml();
    
    ofXml( const ofXml& rhs );
    const ofXml& operator =( const ofXml& rhs );
    
    bool            addValue(const string& path, const string& value, bool createEntirePath = false);
    bool            addChild( const string& path );
    void            addXml( ofXml& xml, bool copyAll = false);
    
    // templated to be anything
    template <class T> void addValueT(const string& path, T& data);
    
    string          getValue(const string& path);
    string          getValue();
    bool            setValue(const string& path, const string& value);
    
    // templated to be anything
    template <class T> T getValueT(const string& path);
    
    string          getAttribute(const string& path);
    bool            setAttribute(const string& path, const string& value);
    map<string, string> getAttributes();
    int             getNumChildren();

    
    bool            clearAttributes(const string& path);
    bool            clearAttributes();
    bool            clearContents(const string& path);
    bool            clearContents();
    
    bool            remove(const string& path); // works for both attributes and tags
    //bool            removeAll(const string& path); // works for both attributes and tags
    bool            exists(const string& path); // works for both attributes and tags
    
    string          getName();
    bool            reset();

    bool            setToChild(int index);
    bool            setTo(const string& path);
    bool            setToParent();
    bool            setToParent(int numLevelsUp);
    bool            setToSibling();
    bool            setToPrevSibling();
    
    bool            loadFromBuffer( string buffer );

    
    string          toString();
    
    // these are advanced, you probably don't want to use them
    
    Element*        getPocoElement();
    Element*        getPocoElement(const string& path);
    
    Document*       getPocoDocument();
    Document*       getPocoDocument() const;

       
protected:
    
    string DOMErrorMessage(short msg);
    
    Poco::XML::Document *document;
    Element *element;
    
};
