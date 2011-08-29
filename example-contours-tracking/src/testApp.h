#pragma once

#include "ofMain.h"
#include "ofxCv.h"
using namespace ofxCv;
using namespace cv;

float trackingDistance(const cv::Rect& a, const cv::Rect& b);
float trackingDistance(const cv::Point2f& a, const cv::Point2f& b);

template <class Key, class Value>
class Pair {
public:
	Key key;
	Value value;
	Pair(Key key, Value value) : key(key), value(value) {}
	struct byKey {
		bool operator()(Pair<Key, Value> const &a, Pair<Key, Value> const &b) { 
			return a.key < b.key;
		}
	};
	struct byValue {
		bool operator()(Pair<Key, Value> const &a, Pair<Key, Value> const &b) { 
			return a.value < b.value;
		}
	};
};

typedef Pair<unsigned int, float> LabelPair;

template <class T>
class Tracker_ {
protected:
	vector<T> previous;
	vector<unsigned int> labels;
public:
	vector<unsigned int>& track(vector<T>& objects) {
		int m = previous.size();
		int n = objects.size();
		if(m > 0 && n > 0) {
			Mat_<float> dist(n, m, CV_32FC1);
			for(int i = 0; i < n; i++) { // objects
				for(int j = 0; j < m; j++) { // previous
					dist(i, j) = trackingDistance(objects[i], previous[j]);
				}
			}
			
			Mat_<float> minDistance = minRows(dist);
			vector<LabelPair> diff;
			for(int i = 0; i < n; i++) {
				diff.push_back(LabelPair(i, minDistance(i)));
			}
			sort(diff.begin(), diff.end(), LabelPair::byValue());
			/*
			for(int i = 0; i < n; i++) {
				cout << "(" << labelDifficulty[i].key << "," << labelDifficulty[i].value << ")";
			}
			cout << endl;
			*/
			
			// sorted.
			// now create a vector of bools for the ones that are taken.
			// clear labels.
			// start with the first one in diff and find it's min.
			// mark that one as 'taken', add to labels, and move on to the next one.
			
			// then go back through the code and add conditions for age: adding and removing labels
		}
		previous = objects;
		return labels;
	}
	bool exists(unsigned int label) {
		
	}
};

typedef Tracker_<cv::Point2f> PointTracker;
typedef Tracker_<cv::Rect> RectTracker;

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	float threshold;
	ofVideoPlayer movie;	
	ContourFinder contourFinder;
	RectTracker rectTracker;
};
