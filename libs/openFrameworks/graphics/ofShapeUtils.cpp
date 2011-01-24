#include "ofShapeUtils.h"

// helper functions for accessing points in polylines without worrying about wraparound indexing
inline int loopMod(int i, int n) {
	i = i % n;
	return i < 0 ? i + n : i;
}

template <class T>
inline T& loopGet(vector<T>& vec, int i) {
	return vec[loopMod(i, vec.size())];
}

ofPolyline ofSmooth(const ofPolyline& polyline, int smoothingSize, float smoothingAmount) {
	ofPolyline result = polyline;
	
	if(!polyline.getClosed()) {
		ofLogError() << "ofSmooth() currently only supports closed ofPolylines.";
		return polyline;
	}
	
	// precompute weights and normalization
	vector<float> weights;
	float weightSum = 0;
	weights.push_back(1); // center weight
	// side weights
	for(int i = 1; i <= smoothingSize; i++) {
		float curWeight = ofMap(i, 0, smoothingSize, 1, smoothingAmount);
		weights.push_back(curWeight);
		weightSum += curWeight;
	}
	float weightNormalization = 1 / (1 + 2 * weightSum);
	
	// use weights to make weighted averages of neighbors
	int n = polyline.size();
	for(int i = 0; i < n; i++) {
		for(int j = 1; j <= smoothingSize; j++) {
			int leftPosition = (n + i - j) % n;
			int rightPosition = (i + j) % n;
			const ofPoint& left = polyline[leftPosition];
			const ofPoint& right = polyline[rightPosition];
			result[i] += (left + right) * weights[j];
		}
		result[i] *= weightNormalization;
	}
	
	return result;
}

ofPolyline ofResampleSpacing(const ofPolyline& polyline, float spacing) {
	ofPolyline result;
	// if more properties are added to ofPolyline, we need to copy them here
	result.setClosed(polyline.getClosed());

	float totalLength = 0;
	int curStep = 0;
	int lastPosition = polyline.size() - 1;
	if(polyline.getClosed()) {
		lastPosition++;
	}
	for(int i = 0; i < lastPosition; i++) {
		bool repeatNext = i == (int) (polyline.size() - 1);
	
		const ofPoint& cur = polyline[i];
		const ofPoint& next = repeatNext ? polyline[0] : polyline[i + 1];
		ofPoint diff = next - cur;
		
		float curSegmentLength = diff.length();
		totalLength += curSegmentLength;
		
		while(curStep * spacing <= totalLength) {
			float curSample = curStep * spacing;
			float curLength = curSample - (totalLength - curSegmentLength);
			float relativeSample = curLength / curSegmentLength;
			result.addVertex(cur.getInterpolated(next, relativeSample));
			curStep++;
		}
	}
	
	return result;
}

ofPolyline ofResampleCount(const ofPolyline& polyline, int count) {
	float perimeter = polyline.getPerimeter();
	return ofResampleSpacing(polyline, perimeter / count);
}

/*

ofPoint ofGetClosestPoint(const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, float* uptr) {
	float u = (p3.x - p1.x) * (p2.x - p1.x);
	u += (p3.y - p1.y) * (p2.y - p1.y);
	u /= (p2 - p1).length();
	// clamp u
	if(u > 1) {
		u = 1;
	} else if(u < 0) {
		u = 0;
	}
	if(uptr != NULL) {
		*uptr = u;
	}
	return p1.interpolated(p2, u);
}

ofPoint ofGetClosestPoint(ofPolyline& polyline, const ofPoint& target, int& nearest) {
	vector<ofPoint>& pts = polyline.pts;
	
	if(pts.size() == 0) {
		return target;
	}
	
	float distance = 0;
	for(int i = 0; i < pts.size(); i++) {
		float curDistance = target.distance(pts[i]);
		if(i == 0 || curDistance < distance) {
			distance = curDistance;
			nearest = i;
		}
	}
	
	ofPoint left = loopGet(pts, nearest - 1);
	ofPoint center = pts[nearest];
	ofPoint right = loopGet(pts, nearest + 1);
	
	ofPoint leftClosest = closestPoint(left, center, target);
	ofPoint rightClosest = closestPoint(center, right, target);
	
	if(leftClosest.distance(target) < rightClosest.distance(target)) {
		return leftClosest;
	} else {
		return rightClosest;
	}
}

ofPoint ofGetClosestPoint(vector<ofPolyline>& polylines, const ofPoint& target, ofPolyline*& matchedPolyline, int& matchedIndex) {
	matchedPolyline = NULL;
	ofPoint closest;
	float distance;
	bool hasPoints = false;
	for(int i = 0; i < polylines.size(); i++) {
		ofPolyline& cur = polylines[i];
		if(cur.pts.size() > 0) {
			hasPoints = true;
			int curIndex;
			ofPoint curClosest = closestPoint(cur, target, curIndex);
			float curDistance = curClosest.distance(target);
			if(i == 0 || curDistance < distance) {
				distance = curDistance;
				closest = curClosest;
				matchedPolyline = &cur;
				matchedIndex = curIndex;
			}
		}
	}
	if(hasPoints)
		return closest;
	else
		return target;
}
*/
