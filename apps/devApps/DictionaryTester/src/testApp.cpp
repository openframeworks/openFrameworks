#include "testApp.h"
#include "ofDictionary.h"

ofDictionary dict;

class MyData {
public:
	int a, b;
	string s;
};

void testApp::setup() {
	ofSetVerticalSync(true);
	
	int year = 1975;
	// adding known types to dictionary:
	dict.addInt("year", year);
	dict.addInt("month", 8);
	dict.addBool("married", false);
	dict.addBool("driver", true);
	dict.addFloat("age", 35.5);
	dict.addChar("initial", 'm');
	dict.addString("name", "memo");
	dict.addPoint("size", ofPoint(70.5, 175, 30.7));
	
	
	// adding new types to dictionary
	MyData d;
	d.a = 1;
	d.b = 2;
	d.s = "This is custom data";
	dict.add("custom data", d);
	
	
	// you can even add another dictionary!
	ofDictionary dict2;
	dict2.addInt("year", 2001);
	dict2.addString("make", "Peugeot");
	dict2.addString("model", "406");
	dict.addDictionary("car", dict2);
	
	year++;
	
}

void testApp::update() {
	
	// reading known types from dictinary:
	int year = dict.getInt("year");
	int month =  dict.getInt("month");
	bool married = dict.getBool("married");
	bool driver = dict.getBool("driver");
	float age = dict.getFloat("age");
	char initial = dict.getChar("initial");
	string name = dict.getString("name");
	ofPoint size = dict.getPoint("size");
	
	// reading new types from dictionary
	MyData d = dict.get("custom data", MyData());
	
	
	// you can even read an embedded dictionary!
	// in this case just adding itself b
	ofDictionary car = dict.getDictionary("car");
	

	
	cout << "Dumping contents manually: " << endl
	<< "year: " << year << endl
	<< "month: " << month << endl
	<< "age: " << age << endl 
	<< "initial: " << initial << endl
	<< "name: " << name << endl
	<< "size: " << size << endl
	<< "custom data: " << d.a << ", " << d.b << ", " << d.s << endl
	<< "car: " <<  car;//car.getString("make") << ", " << car.getString("model") << ", " << car.getInt("year") << endl;
	
	cout << endl << "Dumping contents via << operator directly: " << dict;
	
	std::exit(0);
	
}

void testApp::draw() {
}

void testApp::exit() {
}

void testApp::keyPressed(int key) {
	switch(key) {
		case 'f':
			ofToggleFullscreen();
			break;
			
	}
}

void testApp::keyReleased(int key) {
}

void testApp::mouseMoved(int x, int y) {
}

void testApp::mouseDragged(int x, int y, int button) {
}

void testApp::mousePressed(int x, int y, int button) {
}

void testApp::mouseReleased(int x, int y, int button) {
}

void testApp::windowResized(int w, int h) {
}