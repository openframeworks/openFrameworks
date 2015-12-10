#pragma once

#include <string>
#include <vector>
#include <cstdlib>

namespace ofxCv {
	
	// edit distance is the number of transformations required to turn one string into another
	int editDistance(const std::string& a, const std::string& b);
	
	// cross correlation using edit distance gives the most representative string from a set
	const std::string& mostRepresentative(const std::vector<std::string>& strs);
}
