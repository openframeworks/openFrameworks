#pragma once

#include "ofConstants.h"
#include "ofParameter.h"
#include "ofParameterGroup.h"
#include "ofBaseTypes.h"

#include <numeric>
#include "pugixml.hpp"

class ofXml{
    
public:
    
	ofXml();

    ofXml( const string & path );
    ofXml( const ofXml& rhs );
    const ofXml& operator =( const ofXml& rhs );

    bool load(const std::filesystem::path & path);
    bool save(const std::filesystem::path & path);

    bool            addChild( const string& path );
    void            addXml( ofXml& xml, bool copyAll = false);

    string          getValue() const;
    string          getValue(const string & path) const;
    int				getIntValue() const;
    int				getIntValue(const string & path) const;
	int64_t			getInt64Value() const;
	int64_t			getInt64Value(const string& path) const;
    float			getFloatValue() const;
    float			getFloatValue(const string & path) const;
    bool			getBoolValue() const;
    bool			getBoolValue(const string & path) const;

    bool            setValue(const string& path, const string& value);
    
    string          getAttribute(const string& path) const;
    bool            setAttribute(const string& path, const string& value);
    map<string, string> getAttributes() const;
    int             getNumChildren() const;
    int             getNumChildren(const string& path) const;

    bool            removeAttribute(const string& path);
    bool            removeAttributes(const string& path); // removes attributes for the passed path
    bool            removeAttributes(); // removes attributes for the element ofXml is pointing to
    bool            removeContents(const string& path); // removes the path passed as parameter
    bool            removeContents(); // removes the childs of the current element
    bool            remove(const string& path); // removes both attributes and tags for the passed path
    void            remove(); // removes the current element and all its children,
    						  // the current element will point to it's parent afterwards
    						  // if the current element is the document root this will act as clear()

    bool            exists(const string& path) const; // works for both attributes and tags
    
    void			clear();  // clears the full document and points the current element to the root

    string          getName() const;
    bool            reset();

    bool            setToChild(unsigned long index);
    bool            setTo(const string& path);
    bool            setToParent();
    bool            setToParent(int numLevelsUp);
    bool            setToSibling();
    bool            setToPrevSibling();
    
    bool            loadFromBuffer( const string& buffer );
    
	string          toString(const std::string & indent = "\t") const;

    //////////////////////////////////////////////////////////////////
    // please excuse our mess: templated get/set
    //////////////////////////////////////////////////////////////////
    
    // a pretty useful tokenization system:
    static vector<string> tokenize(const string & str, const string & delim){
        vector<string> tokens;

        size_t p0 = 0, p1 = string::npos;
        while(p0 != string::npos){
            p1 = str.find_first_of(delim, p0);
            if(p1 != p0){
                string token = str.substr(p0, p1 - p0);
                tokens.push_back(token);
            }
            p0 = str.find_first_not_of(delim, p1);
        }
        return tokens;
    }
    
    // templated to be anything
    template <class T> bool addValue(const string& path, T data=T(), bool createEntirePath = false){
        string value = ofToString(data);
        vector<string> tokens;
        
        if(path.find('/') != string::npos){
            tokens = tokenize(path, "/");
        }

        // is this a tokenized tag?
        if(tokens.size() > 1){
            // don't 'push' down into the new nodes
			pugi::xml_node firstElement, lastElement;
			if(element){
				lastElement = element;
				firstElement = element;
            }

            
			for(const auto & token: tokens){
				pugi::xml_node newElement;

                //ofLogVerbose("ofxXml") << "addValue(): creating " << newElement->nodeName();

                if(lastElement){
					newElement = lastElement.append_child(token.c_str());
				}else{
					newElement = document.append_child(token.c_str());
					firstElement = newElement;
				}

                lastElement = newElement;
            }
            
            if(value != ""){
				lastElement.append_child(pugi::node_pcdata).set_value(value.c_str());
            }

            if(!element){
				element = firstElement;
            }
            
            return true;
            
		}else{
			pugi::xml_node newElement;
			if(element){
				newElement = element.append_child(path.c_str());
			}else{
				element = newElement = document.append_child(path.c_str());
			}

			if(value != ""){
				newElement.append_child(pugi::node_pcdata).set_value(value.c_str());
			}
        }
        return true;
    }

    
    // templated to be anything
    template <class T> T getValue(const string& path, T returnVal=T()) const{
    	if(element){
			if(path == ""){
				const auto & first_child = element.first_child();
				if(first_child && (first_child.type() == pugi::node_pcdata || first_child.type() == pugi::node_cdata)) {
					return ofFromString<T>(element.text().as_string());
				}else{
					ofLogWarning("ofXml") << "getValue(): path \"" << path<< "\" not found when getting value";
					return returnVal; // hmm. this could be a problem
				}
			}else{
				const auto & e = element.select_node(path.c_str()).node();
				if(!e.empty()){
					return ofFromString<T>(e.first_child().text().as_string());
				}
			}
    	}

        return T();
    }
    
    // these are advanced, you probably don't want to use them
    
	pugi::xml_node&				getPugiElement();
	const pugi::xml_node&		getPugiElement() const;
	pugi::xml_node				getPugiElement(const string& path);

	pugi::xml_document &		getPugiDocument();
	const pugi::xml_document &	getPugiDocument() const;


protected:
    string DOMErrorMessage(short msg);

	pugi::xml_document document;
	pugi::xml_node element;
};

// serializer
void ofSerialize(ofXml & xml, const ofAbstractParameter & parameter);
void ofDeserialize(const ofXml & xml, ofAbstractParameter & parameter);
