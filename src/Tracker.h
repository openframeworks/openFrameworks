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
	
	typedef pair<int, int> LabelMatch;
	typedef pair<LabelMatch, float> MatchDistance;
	
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
		vector<int>& track(vector<T>& objects);
		const vector<int>& getLabels() const {
			return labels;
		}
		bool exists(int label) {
		}
	};
	
	template <class T>
	vector<int>& Tracker_<T>::track(vector<T>& objects) {
		int m = previous.size();
		int n = objects.size();	
		int nm = n * m;
		
		vector<MatchDistance> all(nm);
		int k = 0;
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < m; j++) {
				all[k].first = LabelMatch(i, j);
				all[k].second = trackingDistance(objects[i], previous[j]);
				k++;
			}
		}
		
		sort(all.begin(), all.end(), bySecond());
		
		labels.clear();
		labels.resize(n); 
		vector<bool> labeledObjects(n, false);
		vector<bool> labeledPrevious(m, false);
		for(int i = 0; i < nm; i++) {
			LabelMatch& cur = all[i].first;
			int i = cur.first;
			int j = cur.second;
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
	
	float trackingDistance(const cv::Rect& a, const cv::Rect& b);
	float trackingDistance(const cv::Point2f& a, const cv::Point2f& b);
	
	typedef Tracker_<cv::Point2f> PointTracker;
	typedef Tracker_<cv::Rect> RectTracker;
	
}