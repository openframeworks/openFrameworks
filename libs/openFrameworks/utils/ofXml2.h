#pragma once

#include "ofConstants.h"
#include "pugixml.hpp"
#include "ofFileUtils.h"
#include "ofUtils.h"

template<class It>
class ofXmlIterator;
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
		ofXml first() const;

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
		std::string value() const;

		void setName(const std::string & name);

		int intValue() const;
		unsigned int uintValue() const;
		float floatValue() const;
		double doubleValue() const;
		bool boolValue() const;
		operator bool() const;

		Attribute nextAttribute() const;
		Attribute previousAttribute() const;

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
		It begin() const { return It(ofXml(doc, *range.begin())); }
		It end() const { return It(ofXml(doc, pugi::xml_node())); }

	private:
		Range(std::shared_ptr<pugi::xml_document> doc, pugi::xml_object_range<typename It::Base> range)
			:doc(doc), range(range){}
		std::shared_ptr<pugi::xml_document> doc;
		pugi::xml_object_range<typename It::Base> range;
		friend class ofXml;
	};

	ofXml();

	bool load(const std::filesystem::path & file);
	bool parse(const std::string & xmlStr);
	bool save(const std::filesystem::path & file) const;
	std::string toString(const std::string & indent = "\t") const;

	ofXml child(const std::string & name) const;
	Range<ofXmlIterator<pugi::xml_node_iterator>> children() const;
	Range<ofXmlIterator<pugi::xml_named_node_iterator>> children(const std::string & name) const;

	ofXml appendChild(const ofXml & xml);
	ofXml appendChild(ofXml && xml);
	ofXml prependChild(const ofXml & xml);
	ofXml prependChild(ofXml && xml);
	ofXml appendChild(const std::string & name);
	ofXml prependChild(const std::string & name);
	bool removeChild(const std::string & name);

	ofXml insertChildAfter(const std::string & name, const ofXml & after);
	ofXml insertChildBefore(const std::string & name, const ofXml & after);

	ofXml nextSibling() const;
	ofXml previousSibling() const;
	ofXml nextSibling(const std::string & name) const;
	ofXml previousSibling(const std::string & name) const;

	ofXml firstChild() const;
	ofXml lastChild() const;


	Attribute attribute(const std::string & name) const;
	Range<ofXmlIterator<pugi::xml_attribute_iterator>> attributes() const;
	Attribute firstAttribute() const;
	Attribute lastAttribute() const;
	Attribute appendAttribute(const std::string & name);
	Attribute prependAttribute(const std::string & name);

	template<typename T>
	Attribute setAttribute(const std::string & name, const T & value){
		auto attr = attribute(name);
		if(!attr){
			attr = appendAttribute(name);
		}
		attr.set(value);
		return std::move(attr);
	}

	ofXml findFirst(const std::string & path) const;
	Search find(const std::string & path) const;

	template<typename T>
	T value() const{
		return ofFromString<T>(this->xml.text().as_string());
	}

	std::string value() const;

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

	int intValue() const;
	unsigned int uintValue() const;
	float floatValue() const;
	double doubleValue() const;
	bool boolValue() const;

	operator bool() const;

private:
	ofXml(std::shared_ptr<pugi::xml_document> doc, const pugi::xml_node & xml);
	std::shared_ptr<pugi::xml_document> doc;
	pugi::xml_node xml;

	template<class It>
	friend class ofXmlIterator;
	friend class ofXmlSearchIterator;
};

template<class It>
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

	ofXml& operator*() const{
		return this->xml;
	}

	ofXml* operator->() const{
		return &this->xml;
	}

	const ofXmlIterator& operator++(){
		this->xml = xml.nextSibling();
		return *this;
	}

	ofXmlIterator operator++(int){
		auto now = xml;
		this->xml = xml.nextSibling();
		return now;
	}

	const ofXmlIterator& operator--(){
		this->xml = xml.previousSibling();
		return *this;
	}

	ofXmlIterator operator--(int){
		auto now = xml;
		this->xml = xml.previousSibling();
		return now;
	}
	typedef It Base;
private:

	// Construct an iterator which points to the specified node
	ofXmlIterator(ofXml xml)
	:xml(xml){

	}
	mutable ofXml xml;
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
