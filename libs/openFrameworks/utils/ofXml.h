#pragma once

#include "ofConstants.h"
#include "pugixml.hpp"
#include "ofParameter.h"

template<typename It>
class ofXmlIterator;
class ofXmlAttributeIterator;
class ofXmlSearchIterator;

class ofXml{
public:
	class Search{
	public:
		Search(){}

		// Get collection type
		pugi::xpath_node_set::type_t type() const;

		// Get collection size
		size_t size() const;

		// Indexing operator
		ofXml operator[](size_t index) const;

		// Collection iterators
		ofXmlSearchIterator begin() const;
		ofXmlSearchIterator end() const;

		// Sort the collection in ascending/descending order by document order
		void sort(bool reverse = false);

		// Get first node in the collection by document order
		ofXml getFirst() const;

		// Check if collection is empty
		bool empty() const;
	private:
		Search(std::shared_ptr<pugi::xml_document> doc, pugi::xpath_node_set set);
		std::shared_ptr<pugi::xml_document> doc;
		pugi::xpath_node_set search;
		friend class ofXml;
	};

	class Attribute{
	public:
		Attribute(){}
		std::string getValue() const;

		void setName(const std::string & name);
		std::string getName() const;

		int getIntValue() const;
		unsigned int getUintValue() const;
		float getFloatValue() const;
		double getDoubleValue() const;
		bool getBoolValue() const;
		operator bool() const;

		Attribute getNextAttribute() const;
		Attribute getPreviousAttribute() const;

		template<typename T>
		ofXml::Attribute & operator=(const T & value){
			this->attr = ofToString(value);
			return *this;
		}

		template<typename T>
		Attribute & set(const T & value){
			this->attr.set_value(ofToString(value).c_str());
			return *this;
		}
	private:
		Attribute(const pugi::xml_attribute & attr);
		pugi::xml_attribute attr;
		friend class ofXml;
	};

	template<class It>
	class Range{
	public:
		It begin() const {
			if(range.begin() != range.end()){
				return It(doc, *range.begin());
			}else{
				return It(doc, typename It::Node());
			}
		}
		It end() const { return It(doc, typename It::Node()); }

	private:
		Range(std::shared_ptr<pugi::xml_document> doc, pugi::xml_object_range<typename It::Base> range)
			:doc(doc), range(range){}
		std::shared_ptr<pugi::xml_document> doc;
		pugi::xml_object_range<typename It::Base> range;
		friend class ofXml;
	};

	ofXml();

	bool load(const std::filesystem::path & file);
	bool load(const ofBuffer & buffer);
	bool parse(const std::string & xmlStr);
	bool save(const std::filesystem::path & file) const;
	void clear();
	std::string toString(const std::string & indent = "\t") const;

	ofXml getChild(const std::string & name) const;
	Range<ofXmlIterator<pugi::xml_node_iterator>> getChildren() const;
	Range<ofXmlIterator<pugi::xml_named_node_iterator>> getChildren(const std::string & name) const;

	ofXml appendChild(const ofXml & xml);
	ofXml prependChild(const ofXml & xml);
	bool removeChild(const ofXml & node);

#if PUGIXML_VERSION>=170
	ofXml appendChild(ofXml && xml);
	ofXml prependChild(ofXml && xml);
	bool removeChild(ofXml && node);
#endif

	ofXml appendChild(const std::string & name);
	ofXml prependChild(const std::string & name);
	bool removeChild(const std::string & name);

	ofXml insertChildAfter(const std::string & name, const ofXml & after);
	ofXml insertChildBefore(const std::string & name, const ofXml & after);

	ofXml getNextSibling() const;
	ofXml getPreviousSibling() const;
	ofXml getNextSibling(const std::string & name) const;
	ofXml getPreviousSibling(const std::string & name) const;

	ofXml getFirstChild() const;
	ofXml getLastChild() const;
	
	ofXml getParent() const;


	Attribute getAttribute(const std::string & name) const;
	Range<ofXmlAttributeIterator> getAttributes() const;
	Attribute getFirstAttribute() const;
	Attribute getLastAttribute() const;
	Attribute appendAttribute(const std::string & name);
	Attribute prependAttribute(const std::string & name);
	bool removeAttribute(const std::string & name);
	bool removeAttribute(const Attribute & attr);
	bool removeAttribute(Attribute && attr);

	template<typename T>
	Attribute setAttribute(const std::string & name, const T & value){
		auto attr = getAttribute(name);
		if(!attr){
			attr = appendAttribute(name);
		}
		attr.set(value);
		return attr;
	}

	ofXml findFirst(const std::string & path) const;
	Search find(const std::string & path) const;

	template<typename T>
	T getValue() const{
		return ofFromString<T>(this->xml.text().as_string());
	}

	std::string getValue() const;
	std::string getName() const;

	template<typename T>
	void set(const T & value){
		if(!xml){
			xml = doc->append_child(pugi::node_element);
		}
		auto child = this->xml.first_child();
		if(!child){
			child = this->xml.append_child(pugi::node_pcdata);
		}
		if(child.type() == pugi::node_pcdata || child.type() == pugi::node_cdata){
			child.set_value(ofToString(value).c_str());
		}
	}

	void set(const unsigned char & value){
		if(!xml){
			xml = doc->append_child(pugi::node_element);
		}
		auto child = this->xml.first_child();
		if(!child){
			child = this->xml.append_child(pugi::node_pcdata);
		}
		if(child.type() == pugi::node_pcdata || child.type() == pugi::node_cdata){
			child.set_value(ofToString(int(value)).c_str());
		}
	}


	void setName(const std::string & name);

	int getIntValue() const;
	unsigned int getUintValue() const;
	float getFloatValue() const;
	double getDoubleValue() const;
	bool getBoolValue() const;

	operator bool() const;

private:
	ofXml(std::shared_ptr<pugi::xml_document> doc, const pugi::xml_node & xml);
	std::shared_ptr<pugi::xml_document> doc;
	pugi::xml_node xml;

	template<typename It>
	friend class ofXmlIterator;
	friend class ofXmlAttributeIterator;
	friend class ofXmlSearchIterator;
};

template<typename It>
class ofXmlIterator{
public:
	ofXmlIterator(){}

	// Iterator operators
	bool operator==(const ofXmlIterator& rhs) const{
		return this->xml.xml == rhs.xml.xml;
	}

	bool operator!=(const ofXmlIterator& rhs) const{
		return this->xml.xml != rhs.xml.xml;
	}

	const ofXml& operator*() const{
		return this->xml;
	}

	const ofXml* operator->() const{
		return &this->xml;
	}

	ofXml& operator*(){
		return this->xml;
	}

	ofXml* operator->(){
		return &this->xml;
	}

	const ofXmlIterator& operator++(){
		this->xml = xml.getNextSibling();
		return *this;
	}

	ofXmlIterator operator++(int){
		auto now = xml;
		this->xml = xml.getNextSibling();
		return now;
	}

	const ofXmlIterator& operator--(){
		this->xml = xml.getPreviousSibling();
		return *this;
	}

	ofXmlIterator operator--(int){
		auto now = xml;
		this->xml = xml.getPreviousSibling();
		return now;
	}
	typedef It Base;
	typedef pugi::xml_node Node;
private:

	// Construct an iterator which points to the specified node
	ofXmlIterator(std::shared_ptr<pugi::xml_document> doc, const pugi::xml_node & xml)
	:xml(doc, xml){

	}

	// Construct an iterator which points to the specified node
	ofXmlIterator(ofXml && xml)
	:xml(xml){

	}
	mutable ofXml xml;
	friend class ofXml;
};

class ofXmlAttributeIterator{
public:
	ofXmlAttributeIterator(){}

	// Iterator operators
	bool operator==(const ofXmlAttributeIterator& rhs) const{
		return this->attr == rhs.attr;
	}

	bool operator!=(const ofXmlAttributeIterator& rhs) const{
		return this->attr != rhs.attr;
	}

	const ofXml::Attribute & operator*() const{
		return this->attr;
	}

	const ofXml::Attribute* operator->() const{
		return &this->attr;
	}

	ofXml::Attribute & operator*(){
		return this->attr;
	}

	ofXml::Attribute* operator->(){
		return &this->attr;
	}

	const ofXmlAttributeIterator& operator++(){
		this->attr = attr.getNextAttribute();
		return *this;
	}

	ofXmlAttributeIterator operator++(int){
		auto now = attr;
		this->attr = attr.getNextAttribute();
		return now;
	}

	const ofXmlAttributeIterator& operator--(){
		this->attr = attr.getPreviousAttribute();
		return *this;
	}

	ofXmlAttributeIterator operator--(int){
		auto now = attr;
		this->attr = attr.getPreviousAttribute();
		return now;
	}

	typedef pugi::xml_attribute_iterator Base;
	typedef pugi::xml_attribute Node;
private:

	// Construct an iterator which points to the specified node
	ofXmlAttributeIterator(std::shared_ptr<pugi::xml_document>, const ofXml::Attribute & attr)
	:attr(attr){

	}

	ofXmlAttributeIterator(const ofXml::Attribute & attr)
	:attr(attr){

	}
	ofXml::Attribute attr;
	friend class ofXml;
};


class ofXmlSearchIterator{
public:
	ofXmlSearchIterator();

	// Iterator operators
	bool operator==(const ofXmlSearchIterator& rhs) const;
	bool operator!=(const ofXmlSearchIterator& rhs) const;

	ofXml & operator*() const;
	ofXml * operator->() const;

	const ofXmlSearchIterator& operator++();
	ofXmlSearchIterator operator++(int);

	const ofXmlSearchIterator& operator--();
	ofXmlSearchIterator operator--(int);

private:
	ofXmlSearchIterator(std::shared_ptr<pugi::xml_document> doc, const pugi::xpath_node * node)
		:node(node)
	{
		if(node){
			xml = ofXml(doc, node->node());
		}
	}
	const pugi::xpath_node * node = nullptr;
	mutable ofXml xml;
	friend ofXml::Search;
};
// serializer
void ofSerialize(ofXml & xml, const ofAbstractParameter & parameter);
void ofDeserialize(const ofXml & xml, ofAbstractParameter & parameter);
