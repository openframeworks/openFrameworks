#pragma once

struct ofxLocation{
	double altitude;
	double latitude;
	double longitude;
	float speed;
	float bearing;
};


inline ostream & operator <<(ostream & ostr, const ofxLocation & location){
	ostr << location.altitude << " " << location.latitude << " " << location.longitude << " " << location.bearing << " " << location.speed;
	return ostr;
}

inline istream & operator >>(istream & istr, ofxLocation & location){
	istr >> location.altitude >> location.latitude >> location.longitude >> location.bearing >> location.speed;
	return istr;
}
