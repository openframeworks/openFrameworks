#include "testApp.h"

using namespace ofxCv;
using namespace cv;

ofVec2f closestPointOnRay(const ofVec2f& p1, const ofVec2f& p2, const ofVec2f& p3);
ofVec2f closestPointOnLine(const ofVec2f& p1, const ofVec2f& p2, const ofVec2f& p3);
ofVec2f closestPointOnRect(const cv::RotatedRect& rect, const ofVec2f& point);
ofVec2f closestPointOnCircle(const ofVec2f& center, float radius, const ofVec2f& point);
ofVec2f closestPointOnEllipse(const cv::RotatedRect& ellipse, const ofVec2f& point);

float distanceToEllipse(const ofVec2f& point, const cv::RotatedRect& ellipse);
float distanceToRect(const ofVec2f& point, const cv::RotatedRect& rect);
float distanceToRay(const ofVec2f& point, const ofVec2f& start, const ofVec2f& end);
float distanceToLine(const ofVec2f& point, const ofVec2f& start, const ofVec2f& end);