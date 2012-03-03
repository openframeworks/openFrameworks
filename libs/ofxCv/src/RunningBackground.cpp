#include "ofxCv/RunningBackground.h"

namespace ofxCv {
	RunningBackground::RunningBackground()
	:learningRate(.0001)
	,learningTime(900.0)
	,useLearningTime(false)
	,thresholdValue(26)
	,ignoreForeground(false)
	,needToReset(false)
	,differenceMode(ABSDIFF) {
	}
	void RunningBackground::update(cv::Mat frame, cv::Mat& thresholded) {
		if(needToReset || accumulator.empty()) {
			needToReset = false;
			frame.convertTo(accumulator, CV_32F);
		}

		accumulator.convertTo(background, CV_8U);
		switch(differenceMode) {
			case ABSDIFF: cv::absdiff(background, frame, foreground); break;
			case BRIGHTER: cv::subtract(frame, background, foreground); break;
			case DARKER: cv::subtract(background, frame, foreground); break;
		}
		cv::cvtColor(foreground, foregroundGray, CV_RGB2GRAY);
		int thresholdMode = ignoreForeground ? cv::THRESH_BINARY_INV : cv::THRESH_BINARY;
		cv::threshold(foregroundGray, thresholded, thresholdValue, 255, thresholdMode);

		float curLearningRate = learningRate;
		if(useLearningTime) {
			curLearningRate = 1. - powf(1. - (thresholdValue / 255.), 1. / learningTime);
		}
		if(ignoreForeground) {
			cv::accumulateWeighted(frame, accumulator, curLearningRate, thresholded);
			cv::bitwise_not(thresholded, thresholded);
		} else {
			cv::accumulateWeighted(frame, accumulator, curLearningRate);
		}
	}
	cv::Mat& RunningBackground::getBackground() {
		return background;
	}
	cv::Mat& RunningBackground::getForeground() {
		return foreground;
	}
	float RunningBackground::getPresence() const {
		// this could be memoized to improve speed
		return cv::mean(foreground)[0] / 255.;
	}
	void RunningBackground::setThresholdValue(unsigned int thresholdValue) {
		this->thresholdValue = thresholdValue;
	}
	void RunningBackground::setLearningRate(double learningRate) {
		this->learningRate = learningRate;
		useLearningTime = false;
	}
	void RunningBackground::setLearningTime(double learningTime) {
		this->learningTime = learningTime;
		useLearningTime = true;
	}
	void RunningBackground::setIgnoreForeground(bool ignoreForeground) {
		this->ignoreForeground = ignoreForeground;
	}
	void RunningBackground::setDifferenceMode(DifferenceMode differenceMode) {
		this->differenceMode = differenceMode;
	}
	void RunningBackground::reset() {
		needToReset = true;
	}
}
