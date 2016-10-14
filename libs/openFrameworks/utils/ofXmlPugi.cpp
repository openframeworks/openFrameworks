#include "ofConstants.h"
#include "ofXmlPugi.h"

ofXml::ofXml(const string & path){
	document.load_file(path.c_str());
	element = document.document_element();
}

ofXml::ofXml(const ofXml & rhs){
	document.reset(rhs.document);
	element = document.document_element();
}

const ofXml & ofXml::operator=(const ofXml & rhs){
	if(&rhs == this){
		return *this;
	}

	document.reset(rhs.document);
	element = document.document_element();
	return *this;
}


ofXml::ofXml(){}

bool ofXml::load(const std::filesystem::path & path){
	if(document.load_file(ofToDataPath(path).c_str()).status == pugi::status_ok){
		element = document.document_element();
		return true;
	}else{
		return false;
	}
}


bool ofXml::save(const std::filesystem::path & path){
	return document.save_file(ofToDataPath(path).c_str());
}

int ofXml::getNumChildren() const{
	if(!element){
		return 0;
	}

	auto list = element.children();
	return std::count_if(list.begin(), list.end(), [](const pugi::xml_node & n){
		return n.type() == pugi::node_element;
	});
}

int ofXml::getNumChildren(const string& path) const{
	if(!element){
		return 0;
	}

	auto list = element.children(path.c_str());
	return std::count_if(list.begin(), list.end(), [](const pugi::xml_node & n){
		return n.type() == pugi::node_element;
	});
}

string ofXml::toString(const std::string & indent) const{
    ostringstream stream;
	document.print(stream, indent.c_str());
	return stream.str();
}

void ofXml::addXml(ofXml& xml, bool copyAll){
	if(copyAll || !xml.element){
		element.append_copy(xml.document.document_element());
	}else{
		element.append_copy(xml.element);
	}
}

bool ofXml::addChild(const string& path){
    vector<string> tokens;

    if(path.find('/') != string::npos){
        tokens = tokenize(path, "/");
    }
    
    // is this a tokenized tag?
	if(tokens.size() > 1){
		pugi::xml_node el = element;
		if(element){
			el = element;
        }else{
			el = document;
        }
		for(auto & token: tokens){
			el = el.append_child(token.c_str());
		}
		if(!element){
			element = el;
		}

        return true;

    }else{
        if(element){
			element.append_child(path.c_str());
        }else{
			element = document.append_child(path.c_str());
        }
    }
    return true;
}

string ofXml::getValue() const{
	return element.text().as_string();
}

string ofXml::getValue(const string & path) const{
	return getValue <string>(path, "");
}


int ofXml::getIntValue() const {
	return ofToInt(getValue());
}


int ofXml::getIntValue(const string & path) const {
	return getValue <int>(path, 0);
}


float ofXml::getFloatValue() const {
	return ofToFloat(getValue());
}


float ofXml::getFloatValue(const string & path) const {
	return getValue <float>(path, 0.0);
}


bool ofXml::getBoolValue() const {
	return ofToBool(getValue());
}


bool ofXml::getBoolValue(const string & path) const {
	return getValue <bool>(path, false);
}


int64_t ofXml::getInt64Value() const {
	return ofToInt64(getValue());
}


int64_t ofXml::getInt64Value(const string & path) const {
	return getValue <int64_t>(path, 0);
}


bool ofXml::reset(){
    if(element){
		element = document.document_element();
        return true;
    }
    ofLogWarning("ofXml") << "reset(): no element set yet";
    return false;
}

bool ofXml::setToChild(unsigned long index){
    if(!element){
		if(document.document_element().first_child()){
			element = document.document_element().first_child();
        }else{
            ofLogWarning("ofXml") << "setToChild(): no element created yet";
            return false;
        }
    }
    
	unsigned long numberOfChildren = 0;
	auto list = element.children();
	auto child = std::find_if(list.begin(), list.end(), [&](const pugi::xml_node & child){
		return child.type() == pugi::node_element && numberOfChildren++ == index;
	});

	if(child != list.end()){
		element = *child;
		return true;
	}else{
		return false;
	}
}

bool ofXml::setToParent(){
	if(element.parent()){
		element = element.parent();
    }else{
        ofLogWarning("ofXml") << "setToParent(): current element has no parent";
        return false;
    }
    return true;
}

bool ofXml::setToParent(int numLevelsUp){
    if(element){
        int i = 0;
		for( ;i < numLevelsUp && setToParent(); i++){}
		return i == numLevelsUp;
    }

    ofLogWarning("ofXml") << "setToParent(): no element set yet";
    return false;    
}

bool ofXml::setToSibling(){
	pugi::xml_node node;
    if(element){
		node = element.next_sibling();
    }else{
        ofLogWarning("ofXml") << "setToSibling() << no element set yet";
        return false;
    }

	/* If we get NULL for node, then we do not have a sibling.
	   We can only savely check the type on a non-Null node (thus
	   avoiding NULL-pointer dereferences). Empty space is treated
	   as a text node and we do not want that. We are also not
	   interessted in comments. If we find a non-TEXT_NODE or
	   non-COMMENT_NODE, we do not look further for a sibling. */
	while(node){
		if(node.type() == pugi::node_cdata || node.type() == pugi::node_pcdata || node.type() ==pugi::node_comment){
			node = node.next_sibling();
		}else{
			break;
		}
	}
	// make sure we actually got a sibling
	if(!node){
		return false;
	}

    // we're cool now
    element = node;
    return true;
}

bool ofXml::setToPrevSibling(){
	pugi::xml_node node;
    if(element){
		node = element.previous_sibling();
    }else{
        ofLogWarning("ofXml") << "setToPrevSibling(): no element set yet";
        return false;
    }

	while(node){
		if(node.type() == pugi::node_cdata || node.type() == pugi::node_pcdata || node.type() ==pugi::node_comment){
			node = node.previous_sibling();
		}else{
			break;
		}
	}
	// make sure we actually got a sibling
	if(!node){
		return false;
	}

	// we're cool now
	element = node;
	return true;
}

bool ofXml::setValue(const string& path, const string& value){
	pugi::xml_node e;
    if(element){
		e = element.select_single_node(path.c_str()).node();
    }else{
        ofLogWarning("ofXml") << "setValue(): no element set yet";
        return false;
    }

    if(!e){
        ofLogWarning("ofXml") <<  "setValue(): path \"" + path + "\" doesn't exist";
        return false;
    }
    
	if(!e.first_child()){
		e.append_child(pugi::node_pcdata).set_value(value.c_str());
        return true;
    }

	if(e.first_child().type() == pugi::node_cdata || e.first_child().type() == pugi::node_pcdata){
		e.first_child().set_value(value.c_str());
        return true;
    }else{
    	return false;
    }
}

string ofXml::getAttribute(const string& path) const{
    if(element){
		if(path.find("[@") == string::npos){
			return element.attribute(path.c_str()).as_string();
		}else{
			return element.select_single_node(path.c_str()).node().text().as_string();
        }
    }else{
        ofLogWarning("ofXml") << "getAttribute(): no element set yet";
        return "";
	}
    return "";
}

bool ofXml::removeAttribute(const string& path){
	if(element){
        if(path.find("[@") != string::npos){
			auto node = element.select_single_node(path.c_str()).node();
			element.remove_child(node);
		}else{
			element.remove_attribute(path.c_str());
		}

    }else{
        ofLogWarning("ofXml") << "clearAttributes(): no element set yet";
        return false;
	}
    return false;
}

bool ofXml::removeAttributes(const string& path){
	if(element){
		if(path.find("[@") != string::npos){
			auto nodes = element.select_nodes(path.c_str());
			for(auto & node: nodes){
				element.remove_child(node.node());
			}
		}else{
			element.remove_attribute(path.c_str());
		}

	}else{
		ofLogWarning("ofXml") << "clearAttributes(): no element set yet";
		return false;
	}
	return false;
}

bool ofXml::removeAttributes(){
    if(element){
		auto attrs = element.attributes();

		for(auto & attr: attrs){
			element.remove_attribute(attr);
		}
        return true;
    }
    ofLogWarning("ofXml") << "clearAttributes(): no element set yet";
    return false;
}

bool ofXml::removeContents(){
	if(element){
		auto list = element.children();
		for(auto & child: list) {
			element.remove_child(child);
		}
		return true;
	}
	return false;
}

bool ofXml::removeContents(const string& path){
	pugi::xml_node e;
    if(element){
		e = element.select_single_node(path.c_str()).node();
    }else{
        ofLogWarning("ofXml") << "clearContents(): no element set yet";
        return false;
    }

    if(e){
		auto list = e.children();
		for(auto & child: list) {
			element.remove_child(child);
		}
        return true;
    }
    return false;
}


void ofXml::clear(){
	document.reset();
	element = document.document_element();
}

bool ofXml::remove(const string & path){ // works for both attributes and tags
	pugi::xml_node node;
	if(element){
		node = element.select_single_node(path.c_str()).node();
	}else{
		ofLogWarning("ofXml") << "remove(): no element set yet";
		return false;
	}

	if(node){
		node.parent().remove_child(node);
		return true;
	}
	return false;
}


void ofXml::remove(){
	auto parent = element.parent();
	if(parent){
		parent.remove_child(element);
		element = parent;
	}else{
		clear();
	}
}


bool ofXml::exists(const string & path) const{ // works for both attributes and tags
	if(element){
		return element.select_single_node(path.c_str()).node();
	}else{
		return false;
	}
}

map<string, string> ofXml::getAttributes() const{ // works for both attributes and tags
    map<string, string> attrMap;

    if(element){
		auto attrs = element.attributes();
		for(auto & attr: attrs){
			attrMap[attr.name()] = attr.as_string();
        }
    }else{
        ofLogWarning("ofXml") << "getAttribute(): no element set";
    }

    return attrMap;
}


bool ofXml::setAttribute(const string& path, const string& value){
    // you can pass either /node[@attr] or just attr
    if(path.find("[@") != string::npos){
		element.select_single_node(path.c_str()).attribute().set_value(value.c_str());
    }else{
		auto attr = element.attribute(path.c_str());
		if(attr){
			attr.set_value(value.c_str());
		}else{
			element.append_attribute(path.c_str()).set_value(value.c_str());
		}
	}
	return true;
}


bool ofXml::loadFromBuffer(const string & buffer){
	if(document.load_buffer(buffer.c_str(), buffer.size()).status == pugi::status_ok){
		element = document.document_element();
		return true;
	}else{
		return false;
	}
}


string ofXml::getName() const {
	if(element){
		return element.name();
	}
	return "";
}

bool ofXml::setTo(const string& path){
	if(path.find("//") != string::npos){
		auto node = document.select_single_node(path.c_str()).node();
		if(node){
			element = node;
			return true;
		}else{
			return false;
		}
	}else{
		auto node = element.select_single_node(path.c_str()).node();
		if(node){
			element = node;
			return true;
		}else{
			return false;
		}
	}
}

pugi::xml_node & ofXml::getPugiElement(){
	return element;
}

const pugi::xml_node & ofXml::getPugiElement() const{
	return element;
}

pugi::xml_node ofXml::getPugiElement(const string& path){
	return element.select_single_node(path.c_str()).node();
}


pugi::xml_document & ofXml::getPugiDocument(){
	return document;
}


const pugi::xml_document & ofXml::getPugiDocument() const{
	return document;
}


string ofXml::DOMErrorMessage(short msg){
	switch(msg){
	 case 1:
		 return "INDEX_SIZE_ERR";
		 break;   /// index or size is negative or greater than allowed value
	 case 2:
		 return "DOMSTRING_SIZE_ERR";             /// the specified range of text does not fit into a DOMString (not used)
		 break;
	 case 3:
		 return "HIERARCHY_REQUEST_ERR";          /// a node is inserted somewhere it doesn't belong
		 break;
	 case 4:
		 return "WRONG_DOCUMENT_ERR";             /// a node is used in a different document than the one that created it
		 break;
	 case 5:
		 return "INVALID_CHARACTER_ERR";          /// an invalid character is specified (not used)
		 break;
	 case 6:
		 return "NO_DATA_ALLOWED_ERR";            /// data is specified for a node which does not support data
		 break;
	 case 7:
		 return "NO_MODIFICATION_ALLOWED_ERR";    /// an attempt is made to modify an object where modifications are not allowed
		 break;
	 case 8:
		 return "NOT_FOUND_ERR";                  /// an attempt was made to reference a node in a context where it does not exist
		 break;
	 case 9:
		 return "NOT_SUPPORTED_ERR";              /// the implementation does not support the type of object requested
		 break;
	 case 10:
		 return "INUSE_ATTRIBUTE_ERR";            /// an attempt is made to add an attribute that is already in use elsewhere
		 break;
	 case 11:
		 return "INVALID_STATE_ERR";              /// a parameter or an operation is not supported by the underlying object
		 break;
	 case 12:
		 return "SYNTAX_ERR";                     /// an invalid or illegal string is specified
		 break;
	 case 13:
		 return "INVALID_MODIFICATION_ERR";       /// an attempt is made to modify the type of the underlying object
		 break;
	 case 14:
		 return "NAMESPACE_ERR";                  /// an attempt is made to create or change an object in a way which is incorrect with regard to namespaces
		 break;
	 case 15:
		 return "INVALID_ACCESS_ERR";             /// an attempt is made to use an object that is not or is no longer usable
		 break;
	}

	return "DOM ERROR";
}


void ofSerialize(ofXml & xml, const ofAbstractParameter & parameter){
	if(!parameter.isSerializable()){
		return;
	}
	string name = parameter.getEscapedName();
	if(name == ""){
		name = "UnknownName";
	}
	if(parameter.type() == typeid(ofParameterGroup).name()){
		const ofParameterGroup & group = static_cast <const ofParameterGroup &>(parameter);
		if(!xml.exists(name)){
			xml.addChild(name);
			ofLogVerbose("ofXml") << "creating group " << name;
		}
		xml.setTo(name);
		ofLogVerbose("ofXml") << "group " << name;
		for(auto & p: group){
			ofSerialize(xml, *p);
		}
		ofLogVerbose("ofXml") << "end group " << name;
		xml.setToParent();
	}else{
		string value = parameter.toString();
		if(!xml.exists(name)){
			xml.addChild(name);
			ofLogVerbose("ofXml") << "creating tag " << name;
		}
		ofLogVerbose("ofXml") << "setting tag " << name << ": " << value;
		xml.setValue(name, value);
	}
}


void ofDeserialize(const ofXml & xml, ofAbstractParameter & parameter){
	if(!parameter.isSerializable()){
		return;
	}
	string name = parameter.getEscapedName();
	if(parameter.type() == typeid(ofParameterGroup).name()){
		ofParameterGroup & group = static_cast <ofParameterGroup &>(parameter);
		if(const_cast<ofXml&>(xml).setTo(name)){
			for(auto & p: group){
				ofDeserialize(xml, *p);
			}
			const_cast<ofXml&>(xml).setToParent();
		}
	}else{
		if(xml.exists(name)){
			if(parameter.type() == typeid(ofParameter <int> ).name()){
				parameter.cast <int>() = xml.getIntValue(name);
			}else if(parameter.type() == typeid(ofParameter <float> ).name()){
				parameter.cast <float>() = xml.getFloatValue(name);
			}else if(parameter.type() == typeid(ofParameter <bool> ).name()){
				parameter.cast <bool>() = xml.getBoolValue(name);
			}else if(parameter.type() == typeid(ofParameter <int64_t> ).name()){
				parameter.cast <int64_t>() = xml.getInt64Value(name);
			}else if(parameter.type() == typeid(ofParameter <string> ).name()){
				parameter.cast <string>() = xml.getValue(name);
			}else{
				parameter.fromString(xml.getValue(name));
			}
		}
	}

}
