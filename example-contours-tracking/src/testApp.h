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

typedef Pair<int, float> LabelPair;

// better, a (little) slower, global minimization would be:
// 1 get the distance matrix
// 2 sort the columns in each row by dist
// 3 sort the rows by the min dist
// 4 match the first row with the first col
// 5 next row: if the first col is matched, remove and resort remaining rows
// 6 repeat 5 until the first col is unmatched

// one more that is O(n^2 + n^2 log n^2)
// relies on sort being fast
// 1 get distance matrix O(n^2)
// 2 sort all the results by distance
// 3 step through and match each pair that isn't already matched
// 4 when done, label any new pairs with new labels
			
template <class T>
class Tracker_ {
protected:
	vector<T> previous;
	vector<int> labels, previousLabels;
	int curLabel;
	int getNewLabel() {
		return curLabel++;
	}
public:
	Tracker_<T>()
	:curLabel(0) {
	}
	vector<int>& track(vector<T>& objects) {
		int m = previous.size();
		int n = objects.size();
		
		labels.clear();
		labels.resize(n, 0);
		
		if(m == 0) {
			// generate labels if there aren't any previous tracked objects
			for(int i = 0; i < n; i++) {
				labels[i] = getNewLabel();
			}
		} else if(n > 0) {
			// build dist between new and old objects
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
				
			cout << "sorted: ";
			for(int i = 0; i < n; i++) {
				cout << "(" << diff[i].key << "," << diff[i].value << ")";
			}	
			cout << endl;
			
			vector<bool> labeled(m, false);
			for(int i = 0; i < n; i++) {
				float bestDist;
				int bestPosition = -1;
				for(int j = 0; j < m; j++) {
					if(!labeled[j] && (bestPosition == -1 || dist(i, j) < bestDist)) {
						bestDist = dist(i, j);
						bestPosition = j;
					}
				}
				if(bestPosition == -1) {
					labels[i] = getNewLabel();
				} else {
					labels[i] = previousLabels[bestPosition];
					labeled[bestPosition] = true;
				}
			}
		}
		for(int i = 0; i < n; i++) {
			cout << "(" << objects[i].x << "," << objects[i].y << " " << labels[i] << ")";
		}
		cout << endl;
		previous = objects;
		previousLabels = labels;
		return labels;
	}
	bool exists(int label) {
		
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
