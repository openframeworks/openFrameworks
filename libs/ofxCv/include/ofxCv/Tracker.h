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
 
 usually you don't just want to know the labels of tracked objects, but you also
 want to maintain a collection of your own objects that are paired with those
 tracked/labeled objects. use the TrackerFollower extension for this: create
 your own MyFollower extending Follower, and create TrackerFollower<MyFollower>.

 for example:
  class MyFollower : public ofxCv::PointFollower { ... }
	ofxCv::PointTrackerFollower<MyFollower> tracker;
	
 then whenever you call tracker.track(), the tracker will maintain a list of
 MyFollower objects internally: when a new label is created, it will call
 MyFollower::setup(), when an old label is updated MyFollower::update(),
 then when a label has been lost it will switch to MyFollower::kill(). when
 MyFollower::getDead() is true, the MyFollower object will be removed. 
 */

#pragma once

#include "opencv2/opencv.hpp"
#include <utility>
#include <map>
#include "ofMath.h"

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
		:lastSeen(0)
		,label(label)
		,age(0)
		,index(index)
		,object(object){
		}
		TrackedObject(const T& object, const TrackedObject<T>& previous, int index)
		:lastSeen(0)
		,label(previous.label)
		,age(previous.age)
		,index(index)
		,object(object){
		}
		TrackedObject(const TrackedObject<T>& old)
		:lastSeen(old.lastSeen)
		,label(old.label)
		,age(old.age)
		,index(-1)
		,object(old.object){
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
		
		unsigned int persistence, curLabel;
		float maximumDistance;
		unsigned int getNewLabel() {
			return curLabel++;
		}
		
	public:
		Tracker<T>()
		:persistence(15)
		,curLabel(0)
		,maximumDistance(64) {
		}
		virtual ~Tracker(){};
		void setPersistence(unsigned int persistence);
		void setMaximumDistance(float maximumDistance);
		virtual const vector<unsigned int>& track(const vector<T>& objects);
		
		// organized in the order received by track()
		const vector<unsigned int>& getCurrentLabels() const;
		const vector<unsigned int>& getPreviousLabels() const;
		const vector<unsigned int>& getNewLabels() const;
		const vector<unsigned int>& getDeadLabels() const;
		unsigned int getLabelFromIndex(unsigned int i) const;
		
		// organized by label
		int getIndexFromLabel(unsigned int label) const;
		const T& getPrevious(unsigned int label) const;
		const T& getCurrent(unsigned int label) const;
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
	const vector<unsigned int>& Tracker<T>::track(const vector<T>& objects) {
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
		for(int k = 0; k < (int)all.size(); k++) {
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
		for(int i = 0; i < (int)current.size(); i++) {
			unsigned int label = current[i].getLabel();
			currentLabelMap[label] = &(current[i]);
		}
		previousLabelMap.clear();
		for(int i = 0; i < (int)previous.size(); i++) {
			unsigned int label = previous[i].getLabel();
			previousLabelMap[label] = &(previous[i]);
		}
		
		return currentLabels;
	}
	
	template <class T>
	const vector<unsigned int>& Tracker<T>::getCurrentLabels() const {
		return currentLabels;
	}
	
	template <class T>
	const vector<unsigned int>& Tracker<T>::getPreviousLabels() const {
		return previousLabels;
	}
	
	template <class T>
	const vector<unsigned int>& Tracker<T>::getNewLabels() const {
		return newLabels;
	}
	
	template <class T>
	const vector<unsigned int>& Tracker<T>::getDeadLabels() const {
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
	const T& Tracker<T>::getPrevious(unsigned int label) const {
		return previousLabelMap.find(label)->second->object;
	}
	
	template <class T>
	const T& Tracker<T>::getCurrent(unsigned int label) const {
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
	
	class RectTracker : public Tracker<cv::Rect> {
	protected:
		float smoothingRate;
		std::map<unsigned int, cv::Rect> smoothed;
	public:
		RectTracker()
		:smoothingRate(.5) {
		}
		void setSmoothingRate(float smoothingRate) {
			this->smoothingRate = smoothingRate;
		}
		float getSmoothingRate() const {
			return smoothingRate;
		}
		const vector<unsigned int>& track(const vector<cv::Rect>& objects) {
			const vector<unsigned int>& labels = Tracker<cv::Rect>::track(objects);
			// add new objects, update old objects
			for(int i = 0; i < labels.size(); i++) {
				unsigned int label = labels[i];
				const cv::Rect& cur = getCurrent(label);
				if(smoothed.count(label) > 0) {
					cv::Rect& smooth = smoothed[label];
					smooth.x = ofLerp(smooth.x, cur.x, smoothingRate);
					smooth.y = ofLerp(smooth.y, cur.y, smoothingRate);
					smooth.width = ofLerp(smooth.width, cur.width, smoothingRate);
					smooth.height = ofLerp(smooth.height, cur.height, smoothingRate);
				} else {
					smoothed[label] = cur;
				}
			}
			std::map<unsigned int, cv::Rect>::iterator smoothedItr;
			for(smoothedItr = smoothed.begin(); smoothedItr != smoothed.end(); smoothedItr++) {
				unsigned int label = smoothedItr->first;
				if(!existsCurrent(label)) {
					smoothed.erase(smoothed.find(label));
				}
			}
			return labels;
		}
		const cv::Rect& getSmoothed(unsigned int label) const {
			return smoothed.find(label)->second;
		}
		cv::Vec2f getVelocity(unsigned int i) const {
			unsigned int label = getLabelFromIndex(i);
			if(existsPrevious(label)) {
				const cv::Rect& previous = getPrevious(label);
				const cv::Rect& current = getCurrent(label);
				cv::Vec2f previousPosition(previous.x + previous.width / 2, previous.y + previous.height / 2);
				cv::Vec2f currentPosition(current.x + current.width / 2, current.y + current.height / 2);
				return currentPosition - previousPosition;
			} else {
				return cv::Vec2f(0, 0);
			}
		}
	};
	
	typedef Tracker<cv::Point2f> PointTracker;
	
	template <class T>
	class Follower {
	protected:
		bool dead;
		unsigned int label;
	public:
		Follower()
		:dead(false)
		,label(0) {}
		
		virtual ~Follower(){};
		virtual void setup(const T& track) {}
		virtual void update(const T& track) {}
		virtual void kill() {
			dead = true;
		}
		
		void setLabel(unsigned int label) {
			this->label = label;
		}
		unsigned int getLabel() const {
			return label;
		}
		bool getDead() const {
			return dead;
		}
	};
	
	typedef Follower<cv::Rect> RectFollower;
	typedef Follower<cv::Point2f> PointFollower;
	
	template <class T, class F>
	class TrackerFollower : public Tracker<T> {
	protected:
		vector<unsigned int> labels;
		vector<F> followers;
	public:
		const vector<unsigned int>& track(const vector<T>& objects) {
			Tracker<T>::track(objects);
			// kill missing, update old
			for(int i = 0; i < labels.size(); i++) {
				unsigned int curLabel = labels[i];
				F& curFollower = followers[i];
				if(!Tracker<T>::existsCurrent(curLabel)) {
					curFollower.kill();
				} else {
					curFollower.update(Tracker<T>::getCurrent(curLabel));
				}
			}
			// add new
			for(int i = 0; i < Tracker<T>::newLabels.size(); i++) {
				unsigned int curLabel = Tracker<T>::newLabels[i];
				labels.push_back(curLabel);
				followers.push_back(F());
				followers.back().setup(Tracker<T>::getCurrent(curLabel));
				followers.back().setLabel(curLabel);
			}
			// remove dead
			for(int i = labels.size() - 1; i >= 0; i--) {
				if(followers[i].getDead()) {
					followers.erase(followers.begin() + i);
					labels.erase(labels.begin() + i);
				}
			}
			return labels;
		}
		vector<F>& getFollowers() {
			return followers;
		}
	};
	
	template <class F> class RectTrackerFollower : public TrackerFollower<cv::Rect, F> {};
	template <class F> class PointTrackerFollower : public TrackerFollower<cv::Point2f, F> {};
}
