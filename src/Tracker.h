#pragma once

#include "ofxCv.h"

namespace ofxCv {
	
	using namespace cv;
	
	template <class Key, class Value>
	class Pair {
	public:
		Key key;
		Value value;
		Pair() {}
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
	
	typedef Pair<int, int> LabelMatch;
	typedef Pair<LabelMatch, float> MatchDistance;
	
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
			int nm = n * m;
			
			vector<MatchDistance> all(nm);
			int k = 0;
			for(int i = 0; i < n; i++) {
				for(int j = 0; j < m; j++) {
					all[k].key = LabelMatch(i, j);
					all[k].value = trackingDistance(objects[i], previous[j]);
					k++;
				}
			}
			
			sort(all.begin(), all.end(), MatchDistance::byValue());
			
			labels.clear();
			labels.resize(n);
			vector<bool> labeledObjects(n, false);
			vector<bool> labeledPrevious(m, false);
			for(int i = 0; i < nm; i++) {
				LabelMatch& cur = all[i].key;
				int i = cur.key;
				int j = cur.value;
				if(!labeledObjects[i] && !labeledPrevious[j]) {
					labeledObjects[i] = true;
					labeledPrevious[j] = true;
					labels[i] = previousLabels[j];
				}
			}
			
			for(int i = 0; i < n; i++) {
				if(!labeledObjects[i]) {
					labels[i] = getNewLabel();
				}
			}
			/*
			 for(int i = 0; i < n; i++) {
			 cout << "(" << objects[i].x << "," << objects[i].y << " " << labels[i] << ")";
			 }
			 cout << endl;
			 */
			previous = objects;
			previousLabels = labels;
			return labels;
		}
		const vector<int>& getLabels() const {
			return labels;
		}
		bool exists(int label) {
			
		}
	};
	
	float trackingDistance(const cv::Rect& a, const cv::Rect& b);
	float trackingDistance(const cv::Point2f& a, const cv::Point2f& b);
	
	typedef Tracker_<cv::Point2f> PointTracker;
	typedef Tracker_<cv::Rect> RectTracker;
	
}