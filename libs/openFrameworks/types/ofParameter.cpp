#include "ofParameter.h"
#include "ofParameterGroup.h"
using namespace std;

string ofAbstractParameter::getEscapedName() const{
	return escape(getName());
}


string ofAbstractParameter::escape(const string& _str) const{

	std::string str(_str);

	ofStringReplace(str, " ", "_");
	ofStringReplace(str, "<", "_");
	ofStringReplace(str, ">", "_");
	ofStringReplace(str, "{", "_");
	ofStringReplace(str, "}", "_");
	ofStringReplace(str, "[", "_");
	ofStringReplace(str, "]", "_");
	ofStringReplace(str, ",", "_");
	ofStringReplace(str, "(", "_");
	ofStringReplace(str, ")", "_");
	ofStringReplace(str, "/", "_");
	ofStringReplace(str, "\\", "_");
	ofStringReplace(str, ".", "_");
	
	return str;
}


string ofAbstractParameter::type() const{
	return typeid(*this).name();
}

vector<string> ofAbstractParameter::getGroupHierarchyNames() const{
	vector<string> hierarchy;
	auto p = getFirstParent();
	if(p){
		hierarchy = p.getGroupHierarchyNames();
	}
	hierarchy.push_back(getEscapedName());
	return hierarchy;
}

ostream& operator<<(ostream& os, const ofAbstractParameter& p){
	os << p.toString();
	return os;
}

istream& operator>>(istream& is, ofAbstractParameter& p){
	string str;
	is >> str;
	p.fromString(str);
	return is;
}
