#include "testApp.h"
#include "ofDictionary.h"

// used to demonstrate adding custom classes
// this has the << operator overloaded 
// so will be saved when you call << or toJson() on the dictionary
// THIS IS TOTALLY OPTIONAL. YOU DON'T NEED TO CREATE CUSTOM CLASSES TO ADD DATA
// I GENERALLY JUST CREATE DICTIONARIES INSIDE CLASSES, FOR ALL GROUPS OF SETTINGS, AND ADD THOSE
class ExData1 {
public:
	int a, b;
	string s;
	
	
	friend ostream& operator<<(ostream& os, const ExData1& d) {
		os << " { a: " << d.a << ", " << " b: " << d.b << ", " << d.s << " }";
		return os;
	}
};


// used to demonstrate adding custom classes
// this does NOT the << operator overloaded 
// so will NOT be saved when you call << or toJson() on the dictionary
// but will still be stored in the dictionary
// THIS IS TOTALLY OPTIONAL. YOU DON'T NEED TO CREATE CUSTOM CLASSES TO ADD DATA
// I GENERALLY JUST CREATE DICTIONARIES INSIDE CLASSES, FOR ALL GROUPS OF SETTINGS, AND ADD THOSE
class ExData2 {
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
	
	
	// adding custom types to dictionary
	ExData1 d1;
	d1.a = 1;
	d1.b = 2;
	d1.s = "This is custom data with << overloaded";
	info.add("custom data1", d1);
	
	ExData2 d2;
	d2.a = 3;
	d2.b = 4;
	d2.s = "This is custom data without << overloaded";
	info.add("custom data2", d2);
	
	
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
	ExData1 d1 = info.get("custom data1", ExData1());
	ExData2 d2 = info.get("custom data2", ExData2());
	
	
	// you can even read back a nested dictionary!
	ofDictionary &car = info.getDictionary("car");
	
	
	// dump info to console using overloaded << operator.
	// this could be a filestream or any other stream
	// it works with nested dictionaries!
	cout << endl << endl << "Dumping contents via << operator: " << info;
	
	// you will notice that there was no output for "custom data2"
	// that's because it has no << operator
	// but it is still stored in the dictionary, let's check:
	// (we already read it into the d2 variable above)
	cout << endl << endl << "Reading contents of \"custom data2\" directly: " << endl << " { " << d2.a << ", " << d2.b << ", " << d2.s << " } ";
	
	
	// to directly access a nested dictionaries member you can also do
	cout << endl << endl << "car year: " << info.getDictionary("car").getInt("year");
	
	//or you can modify in place
	info.getDictionary("car").getInt("year") = 2005;
	cout << endl << endl << "new car year: " << info.getDictionary("car").getInt("year");
	
	
	// of course the above doesn't work for only nested dictionaries, but for any members
	info.getString("name") = "Memo Akten";
	cout << endl << endl << "new name: " << info.getString("name");
	
//	ofCreateAlertDialog("Check console");	// crazy memory leaks in this
	std::exit(0);
}
