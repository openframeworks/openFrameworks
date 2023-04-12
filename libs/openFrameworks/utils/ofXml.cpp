#include "ofXml.h"
#include "ofUtils.h"
#include <clocale>

using std::string;

ofXml::ofXml()
:doc(new pugi::xml_document){
	xml = doc->root();
}

ofXml::ofXml(std::shared_ptr<pugi::xml_document> doc, const pugi::xml_node & xml)
:doc(doc)
,xml(xml){

}

bool ofXml::load(const of::filesystem::path & file){
	auto auxDoc = std::make_shared<pugi::xml_document>();
	auto p = ofToDataPath(file);
	auto res = auxDoc->load_file(ofToDataPath(file).c_str());
	if( res ){
		doc = auxDoc;
		xml = doc->root();
		return true;
	}else{
		ofLogWarning("ofXml") << "Cannot load file " << file << ": " << res.description();
		return false;
	}
}

bool ofXml::load(const ofBuffer & buffer){
	return parse(buffer.getText());
}

bool ofXml::parse(const std::string & xmlStr){
	auto auxDoc = std::make_shared<pugi::xml_document>();
    #if ( defined(PUGIXML_VERSION) && PUGIXML_VERSION >= 150 )
        if(auxDoc->load_string(xmlStr.c_str())){
    #else
        if(auxDoc->load(xmlStr.c_str())){
    #endif
		doc = auxDoc;
		xml = doc->root();
		return true;
	}else{
		return false;
	}
}

bool ofXml::save(const of::filesystem::path & file) const{
	if(xml == doc->root()){
		auto res = doc->save_file(ofToDataPath(file).c_str());
		ofLogVerbose("ofXml")<<"ofXML Save : "<< res;
		ofLogVerbose("ofXml")<<this->toString();
		return res;
	}else{
		pugi::xml_document doc;
		if(doc.append_copy(xml.root())){
			return doc.save_file(ofToDataPath(file).c_str());
		}
	}
	return false;
}

void ofXml::clear(){
	doc.reset(new pugi::xml_document);
	xml = doc->root();
}

std::string ofXml::toString(const std::string & indent) const{
	std::ostringstream stream;
	if(xml == doc->root()){
		doc->print(stream, indent.c_str());
	}else{
		pugi::xml_document doc;
		doc.append_copy(xml.root());
	}
	return stream.str();
}

ofXml ofXml::getChild(const std::string & name) const{
	return ofXml(doc, xml.child(name.c_str()));
}

ofXml::Range<ofXmlIterator<pugi::xml_node_iterator>> ofXml::getChildren() const{
	return ofXml::Range<ofXmlIterator<pugi::xml_node_iterator>>(doc, xml.children());
}

ofXml::Range<ofXmlIterator<pugi::xml_named_node_iterator>> ofXml::getChildren(const std::string & name) const{
	return ofXml::Range<ofXmlIterator<pugi::xml_named_node_iterator>>(doc, xml.children(name.c_str()));
}

ofXml ofXml::appendChild(const ofXml & xml){
	return ofXml(doc, this->xml.append_copy(xml.xml));
}

ofXml ofXml::prependChild(const ofXml & xml){
	return ofXml(doc, this->xml.prepend_copy(xml.xml));
}

#if PUGIXML_VERSION>=170
ofXml ofXml::appendChild(ofXml && xml){
	return ofXml(doc, this->xml.append_move(xml.xml));
}

ofXml ofXml::prependChild(ofXml && xml){
	return ofXml(doc, this->xml.prepend_move(xml.xml));
}

bool ofXml::removeChild(ofXml && node){
	return xml.remove_child(node.xml);
}
#endif

ofXml ofXml::appendChild(const std::string & name){
	return ofXml(doc, this->xml.append_child(name.c_str()));
}

ofXml ofXml::prependChild(const std::string & name){
	return ofXml(doc, this->xml.prepend_child(name.c_str()));
}

ofXml ofXml::insertChildAfter(const std::string & name, const ofXml & after){
	return ofXml(doc, this->xml.insert_child_after(name.c_str(), after.xml));
}

ofXml ofXml::insertChildBefore(const std::string & name, const ofXml & before){
	return ofXml(doc, this->xml.insert_child_before(name.c_str(), before.xml));
}

bool ofXml::removeChild(const std::string & name){
	return xml.remove_child(name.c_str());
}

bool ofXml::removeChild(const ofXml & node){
	return xml.remove_child(node.xml);
}

ofXml ofXml::getNextSibling() const{
	return ofXml(doc, this->xml.next_sibling());
}

ofXml ofXml::getPreviousSibling() const{
	return ofXml(doc, this->xml.previous_sibling());
}

ofXml ofXml::getNextSibling(const std::string & name) const{
	return ofXml(doc, this->xml.next_sibling(name.c_str()));
}

ofXml ofXml::getPreviousSibling(const std::string & name) const{
	return ofXml(doc, this->xml.previous_sibling(name.c_str()));
}

ofXml ofXml::getFirstChild() const{
	return ofXml(doc, this->xml.first_child());
}

ofXml ofXml::getLastChild() const{
	return ofXml(doc, this->xml.last_child());
}

ofXml ofXml::getParent() const {
	return ofXml(doc, this->xml.parent());
}

ofXml::Attribute ofXml::getAttribute(const std::string & name) const{
	return this->xml.attribute(name.c_str());
}

ofXml::Range<ofXmlAttributeIterator> ofXml::getAttributes() const{
	return ofXml::Range<ofXmlAttributeIterator>(doc, this->xml.attributes());
}

ofXml::Attribute ofXml::getFirstAttribute() const{
	return this->xml.first_attribute();
}

ofXml::Attribute ofXml::getLastAttribute() const{
	return this->xml.last_attribute();
}

ofXml::Attribute ofXml::appendAttribute(const std::string & name){
	return this->xml.append_attribute(name.c_str());
}

ofXml::Attribute ofXml::prependAttribute(const std::string & name){
	return this->xml.prepend_attribute(name.c_str());
}

bool ofXml::removeAttribute(const std::string & name){
	auto attr = getAttribute(name);
	if(attr){
		return xml.remove_attribute(attr.attr);
	}else{
		return false;
	}
}

bool ofXml::removeAttribute(const ofXml::Attribute & attr){
	return xml.remove_attribute(attr.attr);
}

bool ofXml::removeAttribute(ofXml::Attribute && attr){
	return xml.remove_attribute(attr.attr);
}

ofXml ofXml::findFirst(const std::string & path) const{
	try{
		return ofXml(doc, this->xml.select_node(path.c_str()).node());
	}catch(pugi::xpath_exception & e){
		return ofXml();
	}
}

ofXml::Search ofXml::find(const std::string & path) const{
	try{
		return ofXml::Search(doc, this->xml.select_nodes(path.c_str()));
	}catch(pugi::xpath_exception & e){
		ofLogError() << e.what();
		return ofXml::Search();
	}
}

std::string ofXml::getValue() const{
	return this->xml.text().as_string();
}

std::string ofXml::getName() const{
	return this->xml.name();
}

void ofXml::setName(const std::string & name){
	if(xml==doc->document_element()){
		xml = doc->append_child(pugi::node_element);
	}
	this->xml.set_name(name.c_str());
}

int ofXml::getIntValue() const{
	return this->xml.text().as_int();
}

unsigned int ofXml::getUintValue() const{
	return this->xml.text().as_uint();
}

float ofXml::getFloatValue() const{
	auto loc = std::setlocale( LC_NUMERIC, NULL );
	std::setlocale( LC_NUMERIC, "C" );
	float f = this->xml.text().as_float();
	std::setlocale( LC_NUMERIC, loc );
	return f;
}

double ofXml::getDoubleValue() const{
	auto loc = std::setlocale( LC_NUMERIC, NULL );
	std::setlocale( LC_NUMERIC, "C" );
	float d = this->xml.text().as_double();
	std::setlocale( LC_NUMERIC, loc );
	return d;
}

bool ofXml::getBoolValue() const{
	return this->xml.text().as_bool();
}

ofXml::operator bool() const{
	return this->xml;
}



//--------------------------------------------------------
// Attribute

ofXml::Attribute::Attribute(const pugi::xml_attribute & attr)
:attr(attr){}

std::string ofXml::Attribute::getValue() const{
	return this->attr.as_string();
}

void ofXml::Attribute::setName(const std::string & name){
	this->attr.set_name(name.c_str());
}

std::string ofXml::Attribute::getName() const{
	return this->attr.name();
}

int ofXml::Attribute::getIntValue() const{
	return this->attr.as_int();
}

unsigned int ofXml::Attribute::getUintValue() const{
	return this->attr.as_uint();
}

float ofXml::Attribute::getFloatValue() const{
	auto loc = std::setlocale( LC_NUMERIC, NULL );
	std::setlocale( LC_NUMERIC, "C" );
	float f = this->attr.as_float();
	std::setlocale( LC_NUMERIC, loc );
	return f;
}

double ofXml::Attribute::getDoubleValue() const{
	auto loc = std::setlocale( LC_NUMERIC, NULL );
	std::setlocale( LC_NUMERIC, "C" );
	float d = this->attr.as_double();
	std::setlocale( LC_NUMERIC, loc );
	return d;
}

bool ofXml::Attribute::getBoolValue() const{
	return this->attr.as_bool();
}

ofXml::Attribute::operator bool() const{
	return this->attr;
}

ofXml::Attribute ofXml::Attribute::getNextAttribute() const{
	return this->attr.next_attribute();
}

ofXml::Attribute ofXml::Attribute::getPreviousAttribute() const{
	return this->attr.previous_attribute();
}


//-----------------------------------------------
// Search

// Get collection type
pugi::xpath_node_set::type_t ofXml::Search::type() const{
	return search.type();
}

// Get collection size
size_t ofXml::Search::size() const{
	return search.size();
}

// Indexing operator
ofXml ofXml::Search::operator[](size_t index) const{
	return ofXml(doc, search[index].node());
}

// Collection iterators
ofXmlSearchIterator ofXml::Search::begin() const{
	return ofXmlSearchIterator(doc, search.begin());
}

ofXmlSearchIterator ofXml::Search::end() const{
	return ofXmlSearchIterator(doc, search.end());
}

// Sort the collection in ascending/descending order by document order
void ofXml::Search::sort(bool reverse){
	search.sort(reverse);
}

// Get first node in the collection by document order
ofXml ofXml::Search::getFirst() const{
	return ofXml(doc, search.first().node());
}

// Check if collection is empty
bool ofXml::Search::empty() const{
	return search.empty();
}


ofXml::Search::Search(std::shared_ptr<pugi::xml_document> doc, pugi::xpath_node_set set)
:doc(doc)
,search(set){}

//----------------------------------------------------
// SearchIterator
ofXmlSearchIterator::ofXmlSearchIterator(){}

// Iterator operators
bool ofXmlSearchIterator::operator==(const ofXmlSearchIterator& rhs) const{
	return this->node == rhs.node;
}

bool ofXmlSearchIterator::operator!=(const ofXmlSearchIterator& rhs) const{
	return this->node != rhs.node;
}

ofXml & ofXmlSearchIterator::operator*() const{
	return xml;
}

ofXml * ofXmlSearchIterator::operator->() const{
	return &xml;
}

const ofXmlSearchIterator& ofXmlSearchIterator::operator++(){
	if(node){
		node = node + 1;
		if(node){
			xml.xml = node->node();
		}
	}
	return *this;
}

ofXmlSearchIterator ofXmlSearchIterator::operator++(int){
	if(node){
		auto now = *this;
		++(*this);
		return now;
	}else{
		return *this;
	}
}

const ofXmlSearchIterator& ofXmlSearchIterator::operator--(){
	if(node){
		node = node - 1;
		if(node){
			xml.xml = node->node();
		}
	}
	return *this;
}

ofXmlSearchIterator ofXmlSearchIterator::operator--(int){
	if(node){
		auto now = *this;
		--(*this);
		return now;
	}else{
		return *this;
	}
}

void ofSerialize(ofXml & xml, const ofAbstractParameter & parameter){
	if(!parameter.isSerializable()){
		return;
	}
	string name = parameter.getEscapedName();
	if(name == ""){
		name = "UnknownName";
	}
	ofXml child	= xml.findFirst(name);
	
	if(!child){
		child = xml.appendChild(name);
		ofLogVerbose("ofXml") << "creating group " << name;
	}
	if(parameter.type() == typeid(ofParameterGroup).name()){
		const ofParameterGroup & group = static_cast <const ofParameterGroup &>(parameter);
		
		ofLogVerbose("ofXml") << "group " << name;
		for(auto & p: group){
			ofSerialize(child, *p);
		}
		ofLogVerbose("ofXml") << "end group " << name;
	}else{
		string value = parameter.toString();
		child.set(value);
	}
}


void ofDeserialize(const ofXml & xml, ofAbstractParameter & parameter){
	if(!parameter.isSerializable()){
		return;
	}
	string name = parameter.getEscapedName();
	
	ofXml child = xml.findFirst(name);
	if(child){
		if(parameter.type() == typeid(ofParameterGroup).name()){
			ofParameterGroup & group = static_cast <ofParameterGroup &>(parameter);
			for(auto & p: group){
				ofDeserialize(child, *p);
			}
		}else{
			if(parameter.type() == typeid(ofParameter <int> ).name()){
				parameter.cast <int>() = child.getIntValue();
			}else if(parameter.type() == typeid(ofParameter <float> ).name()){
				parameter.cast <float>() = child.getFloatValue();
			}else if(parameter.type() == typeid(ofParameter <bool> ).name()){
				parameter.cast <bool>() = child.getBoolValue();
			}else if(parameter.type() == typeid(ofParameter <string> ).name()){
				parameter.cast <string>() = child.getValue();
			}else{
				parameter.fromString(child.getValue());
			}
		}
	}
}
