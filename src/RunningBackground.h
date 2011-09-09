/*
 this class is designed to handle the common task of detecting foreground
 objects in a complex scene by subtracting a known background. a good summary
 of different background subtraction techniques is available at:
 
 http://www-staff.it.uts.edu.au/~massimo/BackgroundSubtractionReview-Piccardi.pdf
 
 this class only implements the running average technique. this technique is
 also described in the opencv 2 cookbook in chapter 10, under "extracting the
 foreground objects in video". this class does not replicate the example, which
 ignores foreground objects during accumulation, as this can lead to counter-
 intuitive artifacts.
 
 learningRate determines how quickly the background is learned. a smaller value
 means the background takes longer to learn. default the learningRate is .0001
 and you can use setLearningRate() to change this.
 
 guessing a learningRate can be hard, because it's related to the threshold and
 your camera framerate. setLearningTime() will let you set the learningRate
 in terms of frames. larger values meaning the background takes longer to learn.
 a learning time of 900 means a 30 fps camera would take 30 seconds before
 a foreground object could leave a "shadow", or "trace" in the background
 that appears as foreground after thresholding. in practice, it will only take
 this amount of time exactly when the background is completely black and the
 foreground is completely white. most of the time it will take longer than 
 learningTime, so it's safe to under-shoot.
 
 to do:
 - use hsb, or sb, space for differencing (like ContourFinder)
 */

#pragma once

#include "opencv2/opencv.hpp"

namespace ofxCv {
	class RunningBackground {
	protected:
		cv::Mat accumulator, background, foreground, foregroundGray;
		double learningRate, learningTime;
		unsigned int thresholdValue;
		bool useLearningTime, needToReset;
	public:
		RunningBackground();
		void update(cv::Mat frame, cv::Mat& thresholded);
		cv::Mat& getBackground();
		cv::Mat& getForeground();
		void setThresholdValue(unsigned int thresholdValue);
		void setLearningRate(double learningRate);
		void setLearningTime(double learningTime);
		void reset();
	};	
}