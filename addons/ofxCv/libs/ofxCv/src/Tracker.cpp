#include "ofxCv/Tracker.h"

#include "ofxCv/Utilities.h"
#include "ofRectangle.h"
#include "ofVec2f.h"

namespace ofxCv {
	
	float trackingDistance(const cv::Rect& a, const cv::Rect& b) {
		float dx = (a.x + a.width / 2.) - (b.x + b.width / 2.);
		float dy = (a.y + a.height / 2.) - (b.y + b.height / 2.);
		float dw = a.width - b.width;
		float dh = a.height - b.height;
		float pd = sqrtf(dx * dx + dy * dy);
		float sd = sqrtf(dw * dw + dh * dh);
		return pd + sd;
	}
	
	float trackingDistance(const cv::Point2f& a, const cv::Point2f& b) {
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		return sqrtf(dx * dx + dy * dy);
	}
	
	float trackingDistance(const ofRectangle& a, const ofRectangle& b) {
		return trackingDistance(toCv(a), toCv(b));
	}
	
	float trackingDistance(const ofVec2f& a, const ofVec2f& b) {
		return trackingDistance(toCv(a), toCv(b));
	}
	
}