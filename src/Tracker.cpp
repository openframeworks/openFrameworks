#include "Tracker.h"

namespace ofxCv {
	
	float trackingDistance(const cv::Rect& a, const cv::Rect& b) {
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		float dw = a.width - b.width;
		float dh = a.height - b.height;
		float pd = sqrtf(dx * dx + dy * dy);
		float sd = sqrtf(dw * dw + dh * dh);
		return pd;// + sd;
	}
	
	float trackingDistance(const cv::Point2f& a, const cv::Point2f& b) {
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		return sqrtf(dx * dx + dy * dy);
	}
	
}