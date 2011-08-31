#pragma once

#include "ofxCv.h"

namespace ofxCv {
	
	using namespace cv;
	
	template <class T>
	class TrackedObject {
	protected:
		unsigned int age;
		unsigned int label;
	public:
		T object;
		
		TrackedObject(const T& object, unsigned int label)
		:object(object)
		,label(label)
		,age(0) {
		}
		TrackedObject(const T& object, const TrackedObject<T>& previous)
		:object(object)
		,label(previous.label)
		,age(0) {
		}
		void timeStep() {
			age++;
		}
		unsigned int getAge() const {
			return age;
		}
		unsigned int getLabel() const {
			return label;
		}
	};
	
	struct bySecond {
		template <class First, class Second>
		bool operator()(pair<First, Second> const &a, pair<First, Second> const &b) { 
			return a.second < b.second;
		}
	};
	
	template <class T>
	class Tracker {
	protected:
	
		typedef pair<int, int> MatchPair;
		typedef pair<MatchPair, float> MatchDistancePair;
		
		vector<TrackedObject<T> > previous, current;
		vector<unsigned int> labels;
		map<unsigned int, T*> previousLabelMap, currentLabelMap;
		
		float maximumDistance;
		unsigned int maximumAge;
		unsigned int curLabel;
		unsigned int getNewLabel() {
			return curLabel++;
		}
				
	public:
		Tracker<T>()
		:curLabel(0)
		,maximumAge(4)
		,maximumDistance(32) {
		}
		void setMaximumAge(unsigned int maximumAge);
		void setMaximumDistance(float maximumDistance);
		vector<unsigned int>& track(const vector<T>& objects);
		vector<unsigned int>& getLabels();
		T& getPrevious(unsigned int label);
		T& getCurrent(unsigned int label);
		bool existsCurrent(unsigned int label) const;
		bool existsPrevious(unsigned int label) const;
	};
	
	template <class T>
	void Tracker<T>::setMaximumAge(unsigned int maximumAge) {
		this->maximumAge = maximumAge;
	}
	
	template <class T>
	void Tracker<T>::setMaximumDistance(float maximumDistance) {
		this->maximumDistance = maximumDistance;
	}
	
	template <class T>
	vector<unsigned int>& Tracker<T>::track(const vector<T>& objects) {
		previous = current;
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
		
		labels.clear();
		labels.resize(n);
		current.clear();
		vector<bool> matchedObjects(n, false);
		vector<bool> matchedPrevious(m, false);
		// walk through matches in order
		for(k = 0; k < nm && all[k].second < maximumDistance; k++) {
			MatchPair& match = all[k].first;
			int i = match.first;
			int j = match.second;
			// only use match if both objects are unmatched, age is reset to 0
			if(!matchedObjects[i] && !matchedPrevious[j]) {
				matchedObjects[i] = true;
				matchedPrevious[j] = true;
				current.push_back(TrackedObject<T>(objects[i], previous[j]));
				labels[i] = current.back().getLabel();
			}
		}
		
		// create new labels for new unmatched objects, age is set to 0
		for(int i = 0; i < n; i++) {
			if(!matchedObjects[i]) {
				current.push_back(TrackedObject<T>(objects[i], getNewLabel()));
				labels[i] = current.back().getLabel();
			}
		}
		
		// copy old unmatched objects if young enough, age is increased
		for(int j = 0; j < m; j++) {
			if(!matchedPrevious[j] && previous[j].getAge() < maximumAge) {
				current.push_back(previous[j]);
				current.back().timeStep();
			}
		}
		
		// build label maps
		currentLabelMap.clear();
		for(int i = 0; i < current.size(); i++) {
			unsigned int label = current[i].getLabel();
			currentLabelMap[label] = &(current[i].object);
		}
		previousLabelMap.clear();
		for(int i = 0; i < previous.size(); i++) {
			unsigned int label = previous[i].getLabel();
			previousLabelMap[label] = &(previous[i].object);
		}
				
		return labels;
	}
		
	template <class T>
	vector<unsigned int>& Tracker<T>::getLabels() {
		return labels;
	}
	
	template <class T>
	T& Tracker<T>::getPrevious(unsigned int label) {
		return *(previousLabelMap[label]);
	}
	
	template <class T>
	T& Tracker<T>::getCurrent(unsigned int label) {
		return *(currentLabelMap[label]);
	}
	
	template <class T>
	bool Tracker<T>::existsCurrent(unsigned int label) const {
		return currentLabelMap.count(label) > 0;
	}
	
	template <class T>
	bool Tracker<T>::existsPrevious(unsigned int label) const {
		return previousLabelMap.count(label) > 0;
	}
	
	float trackingDistance(const cv::Rect& a, const cv::Rect& b);
	float trackingDistance(const cv::Point2f& a, const cv::Point2f& b);
	
	typedef Tracker<cv::Point2f> PointTracker;
	typedef Tracker<cv::Rect> RectTracker;
	
}