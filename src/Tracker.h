#pragma once

#include "ofxCv.h"

// if an object has an age > a certain amount, remove it
// if an object is > distance than a certain amount, add new id anyway

namespace ofxCv {
	
	using namespace cv;
	
	struct bySecond {
		template <class First, class Second>
		bool operator()(pair<First, Second> const &a, pair<First, Second> const &b) { 
			return a.second < b.second;
		}
	};
	
	template <class T>
	class TrackedObject {
	public:
		T object;
		unsigned int age;
		int label;
		
		TrackedObject(const T& object, int label)
		:object(object)
		,label(label)
		,age(0) {
		}
		TrackedObject(const T& object, const TrackedObject<T>& previous)
		:object(object)
		,label(previous.label)
		,age(previous.age) {
		}
	};
	
	template <class T>
	class Tracker {
	protected:
	
		typedef pair<int, int> MatchPair;
		typedef pair<MatchPair, float> MatchDistancePair;
		
		vector<TrackedObject<T> > previous;
		vector<int> labels;
		
		int curLabel;
		int getNewLabel() {
			return curLabel++;
		}
		
	public:
		Tracker<T>()
		:curLabel(0) {
		}
		vector<int>& track(const vector<T>& objects);
		vector<int>& getLabels() {
			return labels;
		}
	};
	
	template <class T>
	vector<int>& Tracker<T>::track(const vector<T>& objects) {
		int n = objects.size();
		int m = previous.size();
		int nm = n * m;
		
		// build NxM distance matrix
		vector<MatchDistancePair> all(nm);
		int k = 0;
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < m; j++) {
				all[k].first = MatchPair(i, j);
				all[k].second = trackingDistance(objects[i], previous[j].object);
				k++;
			}
		}
		
		// sort all possible matches by distance
		sort(all.begin(), all.end(), bySecond());
		
		vector<TrackedObject<T> > current;
		vector<bool> matchedObjects(n, false);
		vector<bool> matchedPrevious(m, false);
		// walk through matches in order
		for(int i = 0; i < nm; i++) {
			MatchPair& cur = all[i].first;
			int i = cur.first;
			int j = cur.second;
			// only use match if both objects are unmatched
			if(!matchedObjects[i] && !matchedPrevious[j]) {
				matchedObjects[i] = true;
				matchedPrevious[j] = true;
				current.push_back(TrackedObject<T>(objects[i], previous[j]));
			}
		}
		
		// create new labels for new unmatched objects
		for(int i = 0; i < n; i++) {
			if(!matchedObjects[i]) {
				current.push_back(TrackedObject<T>(objects[i], getNewLabel()));
			}
		}
		
		// copy old labels for old unmatched objects
		for(int j = 0; j < m; j++) {
			if(!matchedPrevious[j]) {
				current.push_back(previous[j]);
			}
		}
		
		// save labels only
		labels.clear();
		labels.resize(n);
		for(int i = 0; i < n; i++) {
			labels[i] = current[i].label;
		}
		
		previous = current;
		return labels;
	}
	
	float trackingDistance(const cv::Rect& a, const cv::Rect& b);
	float trackingDistance(const cv::Point2f& a, const cv::Point2f& b);
	
	typedef Tracker<cv::Point2f> PointTracker;
	typedef Tracker<cv::Rect> RectTracker;
	
}