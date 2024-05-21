#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"


class PeriodicMotionDetector {
private:
	struct FrequencyComponent {
		double frequency;
		double magnitude;
	};
		
	ofxCv::ContourFinder  contourFinder;

	ofPixels prevFrame;
	vector<double> motionMagnitudeHistory;
	const int windowSize = 100; // Size of the sliding window

public:
	void update(const ofPixels& frame) {
		
		if (!prevFrame.isAllocated()) {
			prevFrame = frame;
			return;
		}
		
		// Convert to grayscale
		ofPixels currFrame = frame;
		if (currFrame.getImageType() != OF_IMAGE_GRAYSCALE) {
			currFrame.setImageType(OF_IMAGE_GRAYSCALE);
		}
		
		// Calculate absolute difference between frames
		ofPixels diff;
		ofxCv::absdiff(prevFrame, currFrame, diff);
		
		// Threshold the difference image
		ofxCv::threshold(diff, 30);
		
		// Find contours
		vector<vector<cv::Point>> contours;
		contourFinder.findContours(diff);

		// Calculate motion magnitude
		double frameMotion = 0.0;
		for (const auto& contour : contourFinder.getContours()) {
			double area = contourArea(contour);
			if (area > 100) { // Ignore small contours
				frameMotion += area;
			}
		}
		
		// Store motion magnitude in history
		motionMagnitudeHistory.push_back(frameMotion);
		
		// Update previous frame
		prevFrame = currFrame;
		
		// Apply Fourier Transform to the last window of motion magnitude history
		if (motionMagnitudeHistory.size() >= windowSize) {
			// Get the last window
			cv::Mat motionMagnitudeMat(windowSize, 1, CV_64F);
			for (int i = 0; i < windowSize; ++i) {
				motionMagnitudeMat.at<double>(i, 0) = motionMagnitudeHistory[motionMagnitudeHistory.size() - windowSize + i];
			}
			
			// Apply Fourier Transform to the window
			cv::Mat fourierTransform;
			dft(motionMagnitudeMat, fourierTransform, cv::DFT_REAL_OUTPUT);
			
			// Extract frequency components
			vector<FrequencyComponent> frequencyComponents;
			for (int i = 0; i < fourierTransform.rows; ++i) {
				FrequencyComponent component;
				component.frequency = i;
				component.magnitude = fourierTransform.at<float>(i);
				frequencyComponents.push_back(component);
			}
			
			// Sort frequency components by magnitude
			std::sort(frequencyComponents.begin(), frequencyComponents.end(), [](auto & a, auto & b) {
				return a.magnitude > b.magnitude;
			});
			
			// Display the strongest frequency component of the window
			double frequency = frequencyComponents[0].frequency / windowSize;
			double magnitude = frequencyComponents[0].magnitude;
			cout << "Dominant Frequency: " << frequency << " Hz, Magnitude: " << magnitude << endl;
		}
	}
	void draw(int x, int y) const {
		
	}
	
	
};




class ofApp : public ofBaseApp{
	
	PeriodicMotionDetector detector_;
	ofVideoPlayer player_;
public:
	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;
	
	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void mouseMoved(int x, int y ) override;
	void mouseDragged(int x, int y, int button) override;
	void mousePressed(int x, int y, int button) override;
	void mouseReleased(int x, int y, int button) override;
	void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
	void mouseEntered(int x, int y) override;
	void mouseExited(int x, int y) override;
	void windowResized(int w, int h) override;
	void dragEvent(ofDragInfo dragInfo) override;
	void gotMessage(ofMessage msg) override;
	
};
