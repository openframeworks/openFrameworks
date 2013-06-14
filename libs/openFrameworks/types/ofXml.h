
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
    
    bool            addChild( const string& path );
    void            addXml( ofXml& xml, bool copyAll = false);

    string          getValue();
    bool            setValue(const string& path, const string& value);
    
    string          getAttribute(const string& path);
    bool            setAttribute(const string& path, const string& value);
    map<string, string> getAttributes();
    int             getNumChildren();

    
    bool            clearAttributes(const string& path);
    bool            clearAttributes();
    bool            clearContents(const string& path);
    bool            clearContents();
    
    bool            remove(const string& path); // works for both attributes and tags
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
    
    //////////////////////////////////////////////////////////////////
    // please excuse our mess: templated get/set
    //////////////////////////////////////////////////////////////////
    
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
    
    // templated to be anything
    template <class T = string> void addValue(const string& path, T data, bool createEntirePath = false)
    {
        stringstream str;
        str << ofToString(data);
        string value = str.str();
        //addValue(path, str.str());
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
                Element* newElement = getPocoDocument()->createElement(tokens.at(i));
                
                cout << " creating " << newElement->nodeName() << endl;
                
                if(lastElement) {
                    lastElement->appendChild(newElement);
                }
                
                lastElement = newElement;
            }
            
            if(value != "")
            {
                
                Text *text = getPocoDocument()->createTextNode(value);
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
            
            Element *newElement = getPocoDocument()->createElement(path);
            
            if(value != "") {
                
                Text *text = getPocoDocument()->createTextNode(value);
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

    
    // templated to be anything
    template <class T = string> T getValue(const string& path)
    {
        T data;
        
        if(path == "")
        {
            if(element->firstChild()->nodeType() == Node::TEXT_NODE) {
                stringstream ss;
                ss << element->innerText();
                ss >> data;
                return data;
            }
            return ""; // hmm. this could be a problem
        }
        else
        {
            Element *e = (Element*) element->getNodeByPath(path);
            if(e) {
                stringstream ss;
                ss << e->innerText();
                ss >> data;
                return data;
            }
        }
        
        return data;
    }
    
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
