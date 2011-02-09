#include "testApp.h"
#include "ofDictionary.h"

// used to demonstrate adding custom classes
class ExampleCustomData {
public:
	int a, b;
	string s;
	
	
	friend ostream& operator<<(ostream& os, const ExampleCustomData& d) {
		os << " { a: " << d.a << ", " << " b: " << d.b << ", " << d.s << " }";
		return os;
	}
};


ofDictionary info;


void testApp::setup() {
	ofSetVerticalSync(true);
	
	// adding known types to dictionary:
	info.addInt("year", 1975);
	info.addInt("month", 8);
	info.addBool("married", false);
	info.addBool("driver", true);
	info.addFloat("age", 35.5);
	info.addChar("initial", 'm');
	info.addString("name", "memo");
	info.addPoint("size", ofPoint(70.5, 175, 30.7));
	
	
	// adding custom types to dictionary
	ExampleCustomData d;
	d.a = 1;
	d.b = 2;
	d.s = "This is custom data";
	info.add("custom data", d);
	
	
	// you can even add another dictionary!
	ofDictionary carInfo;
	carInfo.addInt("year", 2001);
	carInfo.addString("make", "Peugeot");
	carInfo.addString("model", "406");
	info.addDictionary("car", carInfo);
	
}

void testApp::update() {
	
	// reading known types from dictinary:
	int year = info.getInt("year");
	int month =  info.getInt("month");
	bool married = info.getBool("married");
	bool driver = info.getBool("driver");
	float age = info.getFloat("age");
	char initial = info.getChar("initial");
	string name = info.getString("name");
	ofPoint size = info.getPoint("size");
	
	
	// reading custom types from dictionary
	// for this you need to pass in a second parameter for defaults
	ExampleCustomData d = info.get("custom data", ExampleCustomData());
	
	
	// you can even read back a nested dictionary!
	ofDictionary &car = info.getDictionary("car");
	
	
	// dump info to console using overloaded << operator.
	// this could be a filestream or any other stream
	// it works with nested dictionaries!
	cout << endl << endl << "Dumping contents via << operator: " << info;
	
	
	// to directly access a nested dictionaries member you can do
	cout << endl << endl << "car year: " << info.getDictionary("car").getInt("year");
	
	//or you can modify in place
	info.getDictionary("car").getInt("year") = 2005;
	cout << endl << endl << "new car year: " << info.getDictionary("car").getInt("year");

	
//	ofCreateAlertDialog("Check console");	// crazy memory leaks in this
	std::exit(0);
}
