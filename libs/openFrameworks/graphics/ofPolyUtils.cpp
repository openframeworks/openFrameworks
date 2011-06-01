#include "ofPolyUtils.h"
#include "ofMath.h"
#include "ofUtils.h"

ofPolyline ofGetSmoothed(const ofPolyline& polyline, int smoothingSize, float smoothingShape) {
	ofPolyline result = polyline;
	
	if(!polyline.isClosed()) {
		ofLog( OF_LOG_ERROR, "ofSmooth() currently only supports closed ofPolylines." );
		return polyline;
	}
	
	// precompute weights and normalization
	vector<float> weights;
	weights.resize(smoothingSize+1);
	float weightSum = 0;
	weights[0]=1; // center weight
	// side weights
	for(int i = 1; i <= smoothingSize; i++) {
		float curWeight = ofMap(i, 0, smoothingSize, 1, smoothingShape);
		weights[i]=curWeight;
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

ofPolyline ofGetResampledSpacing(const ofPolyline& polyline, float spacing) {
	ofPolyline result;
	// if more properties are added to ofPolyline, we need to copy them here
	result.setClosed(polyline.isClosed());

	float totalLength = 0;
	int curStep = 0;
	int lastPosition = polyline.size() - 1;
	if(polyline.isClosed()) {
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

ofPolyline ofGetResampledCount(const ofPolyline& polyline, int count) {
	float perimeter = polyline.getPerimeter();
	return ofGetResampledSpacing(polyline, perimeter / count);
}

ofRectangle ofGetBoundingBox(const ofPolyline& polyline) {
	ofRectangle box;
	int n = polyline.size();
	if(n > 0) {
		const ofPoint& first = polyline[0];
		// inititally, use width and height as max x and max y
		box.set(first.x, first.y, first.x, first.y);
		for(int i = 0; i < n; i++) {
			const ofPoint& cur = polyline[i];
			if(cur.x < box.x) {
				box.x = cur.x;
			}
			if(cur.x > box.width) {
				box.width = cur.x;
			}
			if(cur.y < box.y) {
				box.y = cur.y;
			}
			if(cur.y > box.height) {
				box.height = cur.y;
			}
		}
		// later, we make width and height relative
		box.width -= box.x;
		box.height -= box.y;
	}
	return box;
}

// http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/
ofPoint ofGetClosestPoint(const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, float* normalizedPosition) {
	// if p1 is coincident with p2, there is no line
	if(p1 == p2) {
		if(normalizedPosition != NULL) {
			*normalizedPosition = 0;
		}
		return p1;
	}
	
	float u = (p3.x - p1.x) * (p2.x - p1.x);
	u += (p3.y - p1.y) * (p2.y - p1.y);
	// perfect place for fast inverse sqrt...
	float len = (p2 - p1).length();
	u /= (len * len);
	
	// clamp u
	if(u > 1) {
		u = 1;
	} else if(u < 0) {
		u = 0;
	}
	if(normalizedPosition != NULL) {
		*normalizedPosition = u;
	}
	return p1.getInterpolated(p2, u);
}

// a much faster but less accurate version would check distances to vertices first,
// which assumes vertices are evenly spaced
ofPoint ofGetClosestPoint(const ofPolyline& polyline, const ofPoint& target, unsigned int* nearestIndex) {
	if(polyline.size() < 2) {
		if(nearestIndex != NULL) {
			nearestIndex = 0;
		}
		return target;
	}
	
	float distance = 0;
	ofPoint nearestPoint;
	unsigned int nearest = 0;
	float normalizedPosition = 0;
	unsigned int lastPosition = polyline.size() - 1;
	if(polyline.isClosed()) {
		lastPosition++;
	}
	for(int i = 0; i < (int) lastPosition; i++) {
		bool repeatNext = i == (int) (polyline.size() - 1);
		
		const ofPoint& cur = polyline[i];
		const ofPoint& next = repeatNext ? polyline[0] : polyline[i + 1];
		
		float curNormalizedPosition = 0;
		ofPoint curNearestPoint = ofGetClosestPoint(cur, next, target, &curNormalizedPosition);
		float curDistance = curNearestPoint.distance(target);
		if(i == 0 || curDistance < distance) {
			distance = curDistance;
			nearest = i;
			nearestPoint = curNearestPoint;
			normalizedPosition = curNormalizedPosition;
		}
	}
	
	if(nearestIndex != NULL) {
		if(normalizedPosition > .5) {
			nearest++;
			if(nearest == polyline.size()) {
				nearest = 0;
			}
		}
		*nearestIndex = nearest;
	}
	
	return nearestPoint;
}
