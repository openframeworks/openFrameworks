#pragma once


#include <numeric>

#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofParameter.h"
#include "ofParameterGroup.h"

#include <Poco/DOM/Attr.h>
#include <Poco/DOM/ChildNodesList.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/DocumentFragment.h>
#include <Poco/DOM/DOMException.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/DOMWriter.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/NamedNodeMap.h>
#include <Poco/DOM/Node.h>
#include <Poco/DOM/NodeFilter.h>
#include <Poco/DOM/NodeIterator.h>
#include <Poco/DOM/Text.h>
#include <Poco/SAX/SAXException.h>
#include <Poco/XML/XMLString.h>
#include <Poco/XML/XMLWriter.h>


class ofXml: public ofBaseFileSerializer {
public:
    ofXml();
    ~ofXml();

    ofXml(const std::string& path);
    ofXml(const ofXml& rhs);

    const ofXml& operator = (const ofXml& rhs);

    bool load(const std::string& path);
    bool save(const std::string& path);

    bool addChild(const std::string& path);
    void addXml(ofXml& xml, bool copyAll = false);

    std::string getValue() const;
    std::string getValue(const std::string& path) const;
    int getIntValue() const;
    int getIntValue(const std::string& path) const;
    float getFloatValue() const;
    float getFloatValue(const std::string& path) const;
    bool getBoolValue() const;
    bool getBoolValue(const std::string& path) const;

    bool setValue(const std::string& path, const std::string& value);

    std::string getAttribute(const std::string& path) const;
    bool setAttribute(const std::string& path, const std::string& value);

    std::map<std::string, std::string> getAttributes() const;
    std::size_t getNumChildren() const;
    std::size_t getNumChildren(const std::string& path) const;

    bool removeAttribute(const std::string& path);
    bool removeAttributes(const std::string& path); // removes attributes for the passed path
    bool removeAttributes(); // removes attributes for the element ofXml is pointing to
    bool removeContents(const std::string& path); // removes the path passed as parameter
    bool removeContents(); // removes the childs of the current element
    bool remove(const std::string& path); // removes both attributes and tags for the passed path
    void remove(); // removes the current element and all its children,
                   // the current element will point to it's parent afterwards
                   // if the current element is the document root this will act as clear()

    bool exists(const std::string& path) const; // works for both attributes and tags

    void clear();  // clears the full document and points the current element to the root

    std::string getName() const;
    bool reset();

    bool setToChild(int index);
    bool setTo(const std::string& path);
    bool setToParent();
    bool setToParent(int numLevelsUp);
    bool setToSibling();
    bool setToPrevSibling();

    bool loadFromBuffer(const std::string& buffer);

    std::string toString() const;

    // serializer
    void serialize(const ofAbstractParameter& parameter);
    void deserialize(ofAbstractParameter& parameter);

    //////////////////////////////////////////////////////////////////
    // please excuse our mess: templated get/set
    //////////////////////////////////////////////////////////////////

    // a pretty useful tokenization system:
    static std::vector<std::string> tokenize(const std::string& str, const std::string& delim);

    // templated to be anything
    template <class T>
    bool addValue(const std::string& path, T data=T(), bool createEntirePath = false);

    // templated to be anything
    template <class T>
    T getValue(const std::string& path, T returnVal=T()) const;

    // these are advanced, you probably don't want to use them

    Poco::XML::Element* getPocoElement();
    Poco::XML::Element* getPocoElement(const std::string& path);
    const Poco::XML::Element* getPocoElement() const;
    const Poco::XML::Element* getPocoElement(const std::string& path) const;

    Poco::XML::Document* getPocoDocument();
    const Poco::XML::Document* getPocoDocument() const;

protected:
    void releaseAll();
    std::string DOMErrorMessage(short msg);

    Poco::XML::Document *document;
    Poco::XML::Element *element;

};


template<class T>
bool ofXml::addValue(const std::string& path, T data, bool createEntirePath)
{
    std::string value = ofToString(data);
    std::vector<std::string> tokens;

    if(path.find('/') != std::string::npos) {
        tokens = tokenize(path, "/");
    }

    // is this a tokenized tag?
    if(tokens.size() > 1)
    {
        // don't 'push' down into the new nodes
        Poco::XML::Element* firstElement = 0;
        Poco::XML::Element* lastElement = 0;
        if(element) {
            lastElement = element;
        }

        if(!firstElement) {
            firstElement = lastElement;
        }

        for(std::size_t i = 0; i < tokens.size(); i++)
        {
            Poco::XML::Element* newElement = getPocoDocument()->createElement(tokens.at(i));

            //ofLogVerbose("ofxXml") << "addValue(): creating " << newElement->nodeName();

            if(lastElement) {
                lastElement->appendChild(newElement);
            }

            lastElement = newElement;
        }

        if(value != "")
        {

            Poco::XML::Text *text = getPocoDocument()->createTextNode(value);
            try {

                lastElement->appendChild( text );

            } catch (const Poco::XML::DOMException& e ) {
                ofLogError("ofxXml") << "addValue(): couldn't set node value: " << DOMErrorMessage(e.code());
                return false;
            }
        }

        if(!element) {
            element = firstElement;
            document->appendChild(element);
        }

        return true;

    } else {

        Poco::XML::Element *newElement = getPocoDocument()->createElement(path);

        if(value != "") {

            Poco::XML::Text *text = getPocoDocument()->createTextNode(value);
            try {
                newElement->appendChild(text);
                text->release();

            } catch (const Poco::XML::DOMException& e ) {
                ofLogError("ofxXml") << "addValue(): couldn't set node value: " << DOMErrorMessage(e.code());
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
template <class T>
T ofXml::getValue(const std::string& path, T returnVal) const
{
    if(element){
        if(path == ""){
            if(element->firstChild()->nodeType() == Poco::XML::Node::TEXT_NODE) {
                return ofFromString<T>(element->innerText());
            } else {
                ofLogWarning("ofXml") << "getValue(): path \"" << path<< "\" not found when getting value";
                return returnVal; // hmm. this could be a problem
            }
        } else {
            Poco::XML::Element *e = (Poco::XML::Element*) element->getNodeByPath(path);
            if(e) {
                return ofFromString<T>(e->innerText());
            }
        }
    }

    return T();
}
