#include "testApp.h"
#include "ofDictionary.h"

// used to demonstrate adding custom classes
class ExampleCustomData {
public:
	int a, b;
	string s;
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
	
	
	// adding new types to dictionary
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
	
	// reading new types from dictionary
	// for this you need to pass in a second parameter for default
	ExampleCustomData d = info.get("custom data", ExampleCustomData());
	
	
	// you can even read an embedded dictionary!
	ofDictionary car = info.getDictionary("car");
	

	
	cout << "Dumping contents individually: " << endl
	<< "year: " << year << endl
	<< "month: " << month << endl
	<< "age: " << age << endl 
	<< "initial: " << initial << endl
	<< "name: " << name << endl
	<< "size: " << size << endl
	<< "custom data: " << d.a << ", " << d.b << ", " << d.s << endl
	<< "car: " <<  car;//car.getString("make") << ", " << car.getString("model") << ", " << car.getInt("year") << endl;
	
	cout << endl << endl << "Dumping contents via << operator: " << info;
	
	ofCreateAlertDialog("Check console");
	std::exit(0);
}
