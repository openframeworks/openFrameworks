#include "ofxCv/Helpers.h"
#include "ofxCv/Utilities.h"

namespace ofxCv {
	
	using namespace cv;
	
	ofMatrix4x4 makeMatrix(Mat rotation, Mat translation) {
		Mat rot3x3;
		if(rotation.rows == 3 && rotation.cols == 3) {
			rot3x3 = rotation;
		} else {
			Rodrigues(rotation, rot3x3);
		}
		double* rm = rot3x3.ptr<double>(0);
		double* tm = translation.ptr<double>(0);
		return ofMatrix4x4(rm[0], rm[3], rm[6], 0.0f,
											 rm[1], rm[4], rm[7], 0.0f,
											 rm[2], rm[5], rm[8], 0.0f,
											 tm[0], tm[1], tm[2], 1.0f);
	}
	
	void drawMat(Mat& mat, float x, float y) {
		drawMat(mat, x, y, mat.cols, mat.rows);
	}
	
	void drawMat(Mat& mat, float x, float y, float width, float height) {
		int glType;
		Mat buffer;
		if(mat.depth() != CV_8U) {
			copy(mat, buffer, CV_8U);
		} else {
			buffer = mat;
		}
		if(mat.channels() == 1) {
			glType = GL_LUMINANCE;
		} else {
			glType = GL_RGB;
		}
		ofTexture tex;
		int w = buffer.cols;
		int h = buffer.rows;
		tex.allocate(w, h, glType);
		tex.loadData(buffer.ptr(), w, h, glType);
		tex.draw(x, y, width, height);
	}
	
	void applyMatrix(const ofMatrix4x4& matrix) {
		glMultMatrixf((GLfloat*) matrix.getPtr());
	}
	
	int forceOdd(int x) {
		return (x / 2) * 2 + 1;
	}
	
	int findFirst(const Mat& arr, unsigned char target) {
		for(int i = 0; i < arr.rows; i++) {
			if(arr.at<unsigned char>(i) == target) {
				return i;
			}
		}
		return 0;
	}
	
	int findLast(const Mat& arr, unsigned char target) {
		for(int i = arr.rows - 1; i >= 0; i--) {
			if(arr.at<unsigned char>(i) == target) {
				return i;
			}
		}
		return 0;
	}
	
	float weightedAverageAngle(const vector<Vec4i>& lines) {
		float angleSum = 0;
		ofVec2f start, end;
		float weights = 0;
		for(int i = 0; i < lines.size(); i++) {
			start.set(lines[i][0], lines[i][1]);
			end.set(lines[i][2], lines[i][3]);
			ofVec2f diff = end - start;
			float length = diff.length();
			float weight = length * length;
			float angle = atan2f(diff.y, diff.x);
			angleSum += angle * weight;
			weights += weight;
		}
		return angleSum / weights;
	}
	
	vector<cv::Point2f> getConvexPolygon(const vector<cv::Point2f>& convexHull, int targetPoints) {
		vector<cv::Point2f> result = convexHull;
		
		static const unsigned int maxIterations = 16;
		static const double infinity = numeric_limits<double>::infinity();
		double minEpsilon = 0;
		double maxEpsilon = infinity;
		double curEpsilon = 16; // good initial guess
		
		// unbounded binary search to simplify the convex hull until it's targetPoints
		if(result.size() > targetPoints) {
			for(int i = 0; i < maxIterations; i++) {
				approxPolyDP(Mat(convexHull), result, curEpsilon, true);
				if(result.size() == targetPoints) {
					break;
				}
				if(result.size() > targetPoints) {
					minEpsilon = curEpsilon;
					if(maxEpsilon == infinity) {
						curEpsilon = curEpsilon *  2;
					} else {
						curEpsilon = (maxEpsilon + minEpsilon) / 2;
					}
				}
				if(result.size() < targetPoints) {
					maxEpsilon = curEpsilon;
					curEpsilon = (maxEpsilon + minEpsilon) / 2;
				}
			}
		}
		
		return result;
	}
	
	void drawHighlightString(string text, ofPoint position, ofColor background, ofColor foreground) {
		drawHighlightString(text, position.x, position.y, background, foreground);
	}
	
	void drawHighlightString(string text, int x, int y, ofColor background, ofColor foreground) {
		vector<string> lines = ofSplitString(text, "\n");
		int textLength = 0;
		for(int i = 0; i < lines.size(); i++) {
			// tabs are not rendered
			int tabs = count(lines[i].begin(), lines[i].end(), '\t');
			int curLength = lines[i].length() - tabs;
			// after the first line, everything is indented with one space
			if(i > 0) {
				curLength++;
			}
			if(curLength > textLength) {
				textLength = curLength;
			}
		}
		
		int padding = 4;
		int fontSize = 8;
		float leading = 1.7;
		int height = lines.size() * fontSize * leading - 1;
		int width = textLength * fontSize;
		
#ifdef TARGET_OPENGLES
		// This needs to be refactored to support OpenGLES
		// Else it will work correctly
#else
		glPushAttrib(GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		ofPushStyle();
		ofSetColor(background);
		ofFill();
		ofRect(x, y, width + 2 * padding, height + 2 * padding);
		ofSetColor(foreground);
		ofNoFill();
		ofPushMatrix();
		ofTranslate(padding, padding);
		ofDrawBitmapString(text, x + 1, y + fontSize + 2);
		ofPopMatrix();
		ofPopStyle();
		glPopAttrib();
#endif         
         
	}
}