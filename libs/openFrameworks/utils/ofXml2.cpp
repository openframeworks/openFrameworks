#include "ofXml2.h"
#include "ofUtils.h"

ofXml::ofXml()
:doc(new pugi::xml_document){
	xml = doc->root();
}

ofXml::ofXml(std::shared_ptr<pugi::xml_document> doc, const pugi::xml_node & xml)
:doc(doc)
,xml(xml){

}

bool ofXml::load(const std::filesystem::path & file){
	auto auxDoc = std::make_shared<pugi::xml_document>();
	if(auxDoc->load_file(ofToDataPath(file).c_str())){
		doc = auxDoc;
		xml = doc->root();
		return true;
	}else{
		return false;
	}
}

bool ofXml::parse(const std::string & xmlStr){
	auto auxDoc = std::make_shared<pugi::xml_document>();
	if(auxDoc->load(xmlStr.c_str())){
		doc = auxDoc;
		xml = doc->root();
		return true;
	}else{
		return false;
	}
}

bool ofXml::save(const std::filesystem::path & file) const{
	if(xml == doc->root()){
		return doc->save_file(ofToDataPath(file).c_str());
	}else{
		pugi::xml_document doc;
		if(doc.append_copy(xml.root())){
			return doc.save_file(ofToDataPath(file).c_str());
		}
	}
	return false;
}

std::string ofXml::toString(const std::string & indent) const{
	ostringstream stream;
	if(xml == doc->root()){
		doc->print(stream, indent.c_str());
	}else{
		pugi::xml_document doc;
		doc.append_copy(xml.root());
	}
	return stream.str();
}

ofXml ofXml::child(const std::string & name) const{
	return ofXml(doc, xml.child(name.c_str()));
}

ofXml::Range<ofXmlIterator<pugi::xml_node_iterator>> ofXml::children() const{
	return ofXml::Range<ofXmlIterator<pugi::xml_node_iterator>>(doc, xml.children());
}

ofXml::Range<ofXmlIterator<pugi::xml_named_node_iterator>> ofXml::children(const std::string & name) const{
	return ofXml::Range<ofXmlIterator<pugi::xml_named_node_iterator>>(doc, xml.children(name.c_str()));
}

ofXml ofXml::appendChild(const ofXml & xml){
	return ofXml(doc, this->xml.append_copy(xml.xml));
}

ofXml ofXml::appendChild(ofXml && xml){
	return ofXml(doc, this->xml.append_move(xml.xml));
}

ofXml ofXml::prependChild(const ofXml & xml){
	return ofXml(doc, this->xml.prepend_copy(xml.xml));
}

ofXml ofXml::prependChild(ofXml && xml){
	return ofXml(doc, this->xml.prepend_move(xml.xml));
}

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

ofXml ofXml::nextSibling() const{
	return ofXml(doc, this->xml.next_sibling());
}

ofXml ofXml::previousSibling() const{
	return ofXml(doc, this->xml.previous_sibling());
}

ofXml ofXml::nextSibling(const std::string & name) const{
	return ofXml(doc, this->xml.next_sibling(name.c_str()));
}

ofXml ofXml::previousSibling(const std::string & name) const{
	return ofXml(doc, this->xml.previous_sibling(name.c_str()));
}

ofXml ofXml::firstChild() const{
	return ofXml(doc, this->xml.first_child());
}

ofXml ofXml::lastChild() const{
	return ofXml(doc, this->xml.last_child());
}


ofXml::Attribute ofXml::attribute(const std::string & name) const{
	return this->xml.attribute(name.c_str());
}

ofXml::Range<ofXmlIterator<pugi::xml_attribute_iterator>> ofXml::attributes() const{
	return ofXml::Range<ofXmlIterator<pugi::xml_attribute_iterator>>(doc, this->xml.attributes());
}

ofXml::Attribute ofXml::firstAttribute() const{
	return this->xml.first_attribute();
}

ofXml::Attribute ofXml::lastAttribute() const{
	return this->xml.last_attribute();
}

ofXml::Attribute ofXml::appendAttribute(const std::string & name){
	if(xml==doc->document_element()){
		xml = doc->append_child(pugi::node_element);
	}
	return this->xml.append_attribute(name.c_str());
}

ofXml::Attribute ofXml::prependAttribute(const std::string & name){
	if(xml==doc->document_element()){
		xml = doc->append_child(pugi::node_element);
	}
	return this->xml.prepend_attribute(name.c_str());
}

ofXml ofXml::findFirst(const std::string & path) const{
	try{
		return ofXml(doc, this->xml.select_single_node(path.c_str()).node());
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

std::string ofXml::value() const{
	return this->xml.text().as_string();
}

void ofXml::setName(const std::string & name){
	if(xml==doc->document_element()){
		xml = doc->append_child(pugi::node_element);
	}
	this->xml.set_name(name.c_str());
}

int ofXml::intValue() const{
	return this->xml.text().as_int();
}

unsigned int ofXml::uintValue() const{
	return this->xml.text().as_uint();
}

float ofXml::floatValue() const{
	return this->xml.text().as_float();
}

double ofXml::doubleValue() const{
	return this->xml.text().as_double();
}

bool ofXml::boolValue() const{
	return this->xml.text().as_bool();
}

ofXml::operator bool() const{
	return this->xml;
}



//--------------------------------------------------------
// Attribute

ofXml::Attribute::Attribute(const pugi::xml_attribute & attr)
:attr(attr){}

std::string ofXml::Attribute::value() const{
	return this->attr.as_string();
}

void ofXml::Attribute::setName(const std::string & name){
	this->attr.set_name(name.c_str());
}

int ofXml::Attribute::intValue() const{
	return this->attr.as_int();
}

unsigned int ofXml::Attribute::uintValue() const{
	return this->attr.as_uint();
}

float ofXml::Attribute::floatValue() const{
	return this->attr.as_float();
}

double ofXml::Attribute::doubleValue() const{
	return this->attr.as_double();
}

bool ofXml::Attribute::boolValue() const{
	return this->attr.as_bool();
}

ofXml::Attribute::operator bool() const{
	return this->attr;
}

ofXml::Attribute ofXml::Attribute::nextAttribute() const{
	return this->attr.next_attribute();
}

ofXml::Attribute ofXml::Attribute::previousAttribute() const{
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
ofXml ofXml::Search::first() const{
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
