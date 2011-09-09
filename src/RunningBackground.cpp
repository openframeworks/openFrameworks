#include "RunningBackground.h"

namespace ofxCv {
	RunningBackground::RunningBackground()
	:learningRate(.0001)
	,learningTime(900.0)
	,useLearningTime(false)
	,thresholdValue(26)
	,needToReset(false) {
	}
	void RunningBackground::update(cv::Mat frame, cv::Mat& thresholded) {
		if(needToReset || accumulator.empty()) {
			needToReset = false;
			frame.convertTo(accumulator, CV_32F);
		}
		
		accumulator.convertTo(background, CV_8U);
		cv::absdiff(background, frame, foreground);
		cv::cvtColor(foreground, foregroundGray, CV_RGB2GRAY);
		cv::threshold(foregroundGray, thresholded, thresholdValue, 255, cv::THRESH_BINARY);
		
		float curLearningRate = learningRate;
		if(useLearningTime) {
			curLearningRate = 1. - powf(1. - (thresholdValue / 255.), 1. / learningTime);
		}
		cv::accumulateWeighted(frame, accumulator, curLearningRate);
	}
	cv::Mat& RunningBackground::getBackground() {
		return background;
	}
	cv::Mat& RunningBackground::getForeground() {
		return foreground;
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
	void RunningBackground::reset() {
		needToReset = true;
	}
}