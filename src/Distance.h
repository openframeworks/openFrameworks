#pragma once

#include <string>
#include <vector>
#include <cstdlib>

namespace ofxCv {

	using namespace std;
	
	// edit distance is the number of transformations required to turn one string into another
	int editDistance(const string& a, const string& b);
	
	// cross correlation using edit distance gives the most representative string from a set
	const string& mostRepresentative(const vector<string>& strs);
}
