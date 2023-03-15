#pragma once

#include "ofConstants.h"
#include "ofParameter.h"
#include "ofParameterGroup.h"

#include <numeric>

#define byte _priv_byte

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

#undef byte

class ofxXmlPoco{
    
public:
    
    ofxXmlPoco();
    ~ofxXmlPoco();

    ofxXmlPoco( const std::string & path );
    ofxXmlPoco( const ofxXmlPoco& rhs );
    const ofxXmlPoco& operator =( const ofxXmlPoco& rhs );

    bool load(const of::filesystem::path & filePath);
    bool save(const of::filesystem::path & filePath);

    bool            addChild( const std::string& path );
    void            addXml( ofxXmlPoco& xml, bool copyAll = false);

    std::string          getValue() const;
    std::string          getValue(const std::string & path) const;
    int				getIntValue() const;
    int				getIntValue(const std::string & path) const;
    int64_t getInt64Value() const;
    int64_t getInt64Value(const std::string& path) const;
    float			getFloatValue() const;
    float			getFloatValue(const std::string & path) const;
    bool			getBoolValue() const;
    bool			getBoolValue(const std::string & path) const;

    bool            setValue(const std::string& path, const std::string& value);
    
    std::string          getAttribute(const std::string& path) const;
    bool            setAttribute(const std::string& path, const std::string& value);
    std::map<std::string, std::string> getAttributes() const;
    int             getNumChildren() const;
    int             getNumChildren(const std::string& path) const;

    bool            removeAttribute(const std::string& path);
    bool            removeAttributes(const std::string& path); // removes attributes for the passed path
    bool            removeAttributes(); // removes attributes for the element ofxXmlPoco is pointing to
    bool            removeContents(const std::string& path); // removes the path passed as parameter
    bool            removeContents(); // removes the childs of the current element
    bool            remove(const std::string& path); // removes both attributes and tags for the passed path
    void            remove(); // removes the current element and all its children,
    						  // the current element will point to it's parent afterwards
    						  // if the current element is the document root this will act as clear()

    bool            exists(const std::string& path) const; // works for both attributes and tags
    
    void			clear();  // clears the full document and points the current element to the root

    std::string          getName() const;
    bool            reset();

    bool            setToChild(unsigned long index);
    bool            setTo(const std::string& path);
    bool            setToParent();
    bool            setToParent(int numLevelsUp);
    bool            setToSibling();
    bool            setToPrevSibling();
    
    bool            loadFromBuffer( const std::string& buffer );
    
    std::string          toString() const;


    //////////////////////////////////////////////////////////////////
    // please excuse our mess: templated get/set
    //////////////////////////////////////////////////////////////////
    
    // a pretty useful tokenization system:
    static std::vector<std::string> tokenize(const std::string & str, const std::string & delim){
        std::vector<std::string> tokens;

        size_t p0 = 0, p1 = std::string::npos;
        while(p0 != std::string::npos){
            p1 = str.find_first_of(delim, p0);
            if(p1 != p0){
                std::string token = str.substr(p0, p1 - p0);
                tokens.push_back(token);
            }
            p0 = str.find_first_not_of(delim, p1);
        }
        return tokens;
    }
    
    // templated to be anything
    template <class T> bool addValue(const std::string& path, T data=T(), bool createEntirePath = false){
        std::string value = ofToString(data);
        std::vector<std::string> tokens;
        
        if(path.find('/') != std::string::npos){
            tokens = tokenize(path, "/");
        }

        // is this a tokenized tag?
        if(tokens.size() > 1){
            // don't 'push' down into the new nodes
            Poco::XML::Element* firstElement=nullptr, *lastElement=nullptr;
            if(element){
                lastElement = element;
            }

            if(!firstElement){
                firstElement = lastElement;
            }
            
			for(std::size_t i = 0; i < tokens.size(); i++){
                Poco::XML::Element* newElement = getPocoDocument()->createElement(tokens.at(i));

                //ofLogVerbose("ofxXml") << "addValue(): creating " << newElement->nodeName();

                if(lastElement){
                    lastElement->appendChild(newElement);
                }

                lastElement = newElement;
            }
            
            if(value != ""){
                Poco::XML::Text *text = getPocoDocument()->createTextNode(value);
                try {
                    lastElement->appendChild( text );
                } catch ( Poco::XML::DOMException &e ){
                    ofLogError("ofxXml") << "addValue(): couldn't set node value: " << DOMErrorMessage(e.code());
                    return false;
                }
            }

            if(!element){
                element = firstElement;
                document->appendChild(element);
            }
            
            return true;
            
        }else{
            Poco::XML::Element *newElement = getPocoDocument()->createElement(path);

            if(value != ""){
                Poco::XML::Text *text = getPocoDocument()->createTextNode(value);
                try {
                    newElement->appendChild(text);
                    text->release();
                    
                } catch ( Poco::XML::DOMException &e ){
                    ofLogError("ofxXml") << "addValue(): couldn't set node value: " << DOMErrorMessage(e.code());
                    return false;
                }
            }
            
            if(element){
                element->appendChild(newElement);
            }else{
                element = newElement;
            }
        }
        return true;
    }

    
    // templated to be anything
    template <class T> T getValue(const std::string& path, T returnVal=T()) const{
    	if(element){
			if(path == ""){
				if(element->firstChild() && element->firstChild()->nodeType() == Poco::XML::Node::TEXT_NODE) {
					return ofFromString<T>(element->innerText());
				}else{
					ofLogWarning("ofxXmlPoco") << "getValue(): path \"" << path<< "\" not found when getting value";
					return returnVal; // hmm. this could be a problem
				}
			}else{
				Poco::XML::Element *e = (Poco::XML::Element*) element->getNodeByPath(path);
				if(e){
					return ofFromString<T>(e->innerText());
				}
			}
    	}

        return T();
    }
    
    // these are advanced, you probably don't want to use them
    
    Poco::XML::Element*        getPocoElement();
    Poco::XML::Element*        getPocoElement(const std::string& path);
    const Poco::XML::Element*  getPocoElement() const;
    const Poco::XML::Element*  getPocoElement(const std::string& path) const;

    Poco::XML::Document*       getPocoDocument();
    const Poco::XML::Document* getPocoDocument() const;


protected:
    void releaseAll();
    std::string DOMErrorMessage(short msg);

    Poco::XML::Document *document;
    Poco::XML::Element *element;
};

// serializer
void ofSerialize(ofxXmlPoco & xml, const ofAbstractParameter & parameter);
void ofDeserialize(const ofxXmlPoco & xml, ofAbstractParameter & parameter);
