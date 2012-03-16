/*
 the tracker is used for tracking the identities of a collection of objects that
 change slightly over time. example applications are in contour tracking and
 face tracking. when using a tracker, the two most important things to know are
 the persistence and maximumDistance. persistence determines how many frames an
 object can last without being seen until the tracker forgets about it.
 maximumDistance determines how far an object can move until the tracker
 considers it a new object.
 
 the default trackers are for cv::Rect and cv::Point2f (RectTracker and
 PointTracker). to create a new kind of tracker, you need to add a
 trackingDistance() function that returns the distance between two tracked
 objects.
 
 the tracking algorithm calls the distance function approximately n^2 times.
 it then filters the distances using maximumDistance, which can significantly
 reduce the possible matches. then it sorts the distance using std::sort, which
 runs in nlogn time. the primary bottleneck for most data is the distance
 function. in practical terms, the tracker can become non-realtime when
 tracking more than a few hundred objects. to optimize the tracker, consider
 avoiding usage of sqrt() in the trackingDistance() function.
 
 this tracker doesn't find a global minimum, but a local minimum. for example:
 when a dense set of points moves farther than the average point-to-point radius
 (like a line of points 5 pixels apart moving up and to the right 5 pixels). it
 also fails to model the data, so two objects might be swapped if they cross
 paths quickly.
 */

#pragma once

#include "opencv2/opencv.hpp"
#include <utility>
#include <map>

namespace ofxCv {
	
	using namespace cv;
	
	float trackingDistance(const cv::Rect& a, const cv::Rect& b);
	float trackingDistance(const cv::Point2f& a, const cv::Point2f& b);
	
	template <class T>
	class TrackedObject {
	protected:
		unsigned int lastSeen, label, age;
		int index;
	public:
		T object;
		
		TrackedObject(const T& object, unsigned int label, int index)
		:object(object)
		,lastSeen(0)
		,age(0)
		,label(label)
		,index(index) {
		}
		TrackedObject(const T& object, const TrackedObject<T>& previous, int index)
		:object(object)
		,label(previous.label)
		,index(index)
		,lastSeen(0)
		,age(previous.age) {
		}
		TrackedObject(const TrackedObject<T>& old)
		:object(old.object)
		,label(old.label)
		,index(-1)
		,lastSeen(old.lastSeen)
		,age(old.age) {
		}
		void timeStep(bool visible) {
			age++;
			if(!visible) {
				lastSeen++;
			}
		}
		unsigned int getLastSeen() const {
			return lastSeen;
		}
		unsigned long getAge() const {
			return age;
		}
		unsigned int getLabel() const {
			return label;
		}
		int getIndex() const {
			return index;
		}
	};
	
	struct bySecond {
		template <class First, class Second>
		bool operator()(std::pair<First, Second> const &a, std::pair<First, Second> const &b) { 
			return a.second < b.second;
		}
	};
	
	template <class T>
	class Tracker {
	protected:		
		vector<TrackedObject<T> > previous, current;
		vector<unsigned int> currentLabels, previousLabels, newLabels, deadLabels;
		std::map<unsigned int, TrackedObject<T>*> previousLabelMap, currentLabelMap;
		
		float maximumDistance;
		unsigned int persistence, curLabel;
		unsigned int getNewLabel() {
			return curLabel++;
		}
		
	public:
		Tracker<T>()
		:curLabel(0)
		,persistence(15)
		,maximumDistance(64) {
		}
		void setPersistence(unsigned int persistence);
		void setMaximumDistance(float maximumDistance);
		vector<unsigned int>& track(const vector<T>& objects);
		
		// organized in the order received by track()
		vector<unsigned int>& getCurrentLabels();
		vector<unsigned int>& getPreviousLabels();
		vector<unsigned int>& getNewLabels();
		vector<unsigned int>& getDeadLabels();
		unsigned int getLabelFromIndex(unsigned int i) const;
		
		// organized by label
		int getIndexFromLabel(unsigned int label) const;
		T& getPrevious(unsigned int label) const;
		T& getCurrent(unsigned int label) const;
		bool existsCurrent(unsigned int label) const;
		bool existsPrevious(unsigned int label) const;
		int getAge(unsigned int label) const;
	};
	
	template <class T>
	void Tracker<T>::setPersistence(unsigned int persistence) {
		this->persistence = persistence;
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
		
		// build NxM distance matrix
		typedef std::pair<int, int> MatchPair;
		typedef std::pair<MatchPair, float> MatchDistancePair;
		vector<MatchDistancePair> all;
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < m; j++) {
				float curDistance = trackingDistance(objects[i], previous[j].object);
				if(curDistance < maximumDistance) {
					all.push_back(MatchDistancePair(MatchPair(i, j), curDistance));
				}
			}
		}
		
		// sort all possible matches by distance
		sort(all.begin(), all.end(), bySecond());
		
		previousLabels = currentLabels;
		currentLabels.clear();
		currentLabels.resize(n);
		current.clear();
		vector<bool> matchedObjects(n, false);
		vector<bool> matchedPrevious(m, false);
		// walk through matches in order
		for(int k = 0; k < all.size(); k++) {
			MatchPair& match = all[k].first;
			int i = match.first;
			int j = match.second;
			// only use match if both objects are unmatched, lastSeen is set to 0
			if(!matchedObjects[i] && !matchedPrevious[j]) {
				matchedObjects[i] = true;
				matchedPrevious[j] = true;
				int index = current.size();
				current.push_back(TrackedObject<T>(objects[i], previous[j], index));
				current.back().timeStep(true);
				currentLabels[i] = current.back().getLabel();
			}
		}
		
		// create new labels for new unmatched objects, lastSeen is set to 0
		newLabels.clear();
		for(int i = 0; i < n; i++) {
			if(!matchedObjects[i]) {
				int curLabel = getNewLabel();
				int index = current.size();
				current.push_back(TrackedObject<T>(objects[i], curLabel, index));
				current.back().timeStep(true);
				currentLabels[i] = curLabel;
				newLabels.push_back(curLabel);
			}
		}
		
		// copy old unmatched objects if young enough, lastSeen is increased
		deadLabels.clear();
		for(int j = 0; j < m; j++) {
			if(!matchedPrevious[j]) {
				if(previous[j].getLastSeen() < persistence) {
					current.push_back(previous[j]);
					current.back().timeStep(false);
				}
				deadLabels.push_back(previous[j].getLabel());
			}
		}
		
		// build label maps
		currentLabelMap.clear();
		for(int i = 0; i < current.size(); i++) {
			unsigned int label = current[i].getLabel();
			currentLabelMap[label] = &(current[i]);
		}
		previousLabelMap.clear();
		for(int i = 0; i < previous.size(); i++) {
			unsigned int label = previous[i].getLabel();
			previousLabelMap[label] = &(previous[i]);
		}
		
		return currentLabels;
	}
	
	template <class T>
	vector<unsigned int>& Tracker<T>::getCurrentLabels() {
		return currentLabels;
	}
	
	template <class T>
	vector<unsigned int>& Tracker<T>::getPreviousLabels() {
		return previousLabels;
	}
	
	template <class T>
	vector<unsigned int>& Tracker<T>::getNewLabels() {
		return newLabels;
	}
	
	template <class T>
	vector<unsigned int>& Tracker<T>::getDeadLabels() {
		return deadLabels;
	}

	template <class T>
	unsigned int Tracker<T>::getLabelFromIndex(unsigned int i) const {
		return currentLabels[i];
	}
	
	template <class T>
	int Tracker<T>::getIndexFromLabel(unsigned int label) const {
		return getCurrent(label).getIndex();
	}
	
	template <class T>
	T& Tracker<T>::getPrevious(unsigned int label) const {
		return previousLabelMap.find(label)->second->object;
	}
	
	template <class T>
	T& Tracker<T>::getCurrent(unsigned int label) const {
		return currentLabelMap.find(label)->second->object;
	}
	
	template <class T>
	bool Tracker<T>::existsCurrent(unsigned int label) const {
		return currentLabelMap.count(label) > 0;
	}
	
	template <class T>
	bool Tracker<T>::existsPrevious(unsigned int label) const {
		return previousLabelMap.count(label) > 0;
	}

	template <class T>
	int Tracker<T>::getAge(unsigned int label) const{
		return currentLabelMap.find(label)->second->getAge();
	}
	
	typedef Tracker<cv::Rect> RectTracker;
	typedef Tracker<cv::Point2f> PointTracker;
}
