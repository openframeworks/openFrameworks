#pragma once

#include "ofxCv.h"

namespace ofxCv {
	
	using namespace cv;
	
	class Flow {
	public:
        // should constructor be protected?
		Flow();
		virtual ~Flow();
		
		//call these functions to calculate flow on sequential images.
		//After this call the flow field will be populated and
		//subsequent calls to getFlow() will be updated
		
		//call with two contiguous images
        template <class T>
		void calcOpticalFlow(T& lastImage, T& currentImage) {
            calcOpticalFlow(toCv(lastImage), toCv(currentImage));
        }
		void calcOpticalFlow(Mat lastImage, Mat currentImage);
		
		//call with subsequent images to do running optical flow. 
		//the Flow class internally stores the last image for convenience
        template <class T>
		void calcOpticalFlow(T& currentImage) {
            calcOpticalFlow(toCv(currentImage));
        }
		void calcOpticalFlow(Mat nextImage);
		
		void draw();
		void draw(float x, float y);
		void draw(float x, float y, float width, float height);
		void draw(ofRectangle rect);
		int getWidth();
		int getHeight();
        
        virtual void resetFlow();
        
    private:
		Mat last, curr;
        
    protected:
		bool hasFlow;
		
		//specific flow implementation
		virtual void calcFlow(Mat prev, Mat next) = 0;
		//specific drawing implementation
		virtual void drawFlow(ofRectangle r) = 0;
	};
	
	//there are two implementations of Flow
	//use Farneback for a dense flow field, 
	//use PyrLK for specific features
	
	//see http://opencv.willowgarage.com/documentation/cpp/motion_analysis_and_object_tracking.html
	//for more info on the meaning of these parameters
	
	class FlowPyrLK : public Flow {
	public:
		FlowPyrLK();
		virtual ~FlowPyrLK();
		
		//flow parameters
		void setMinDistance(int minDistance);		
		void setWindowSize(int winsize);
		
		//feature finding parameters
		void setMaxLevel(int maxLevel);
		void setMaxFeatures(int maxFeatures);
		void setQualityLevel(float qualityLevel);
		void setPyramidLevels(int levels);
		
		//returns tracking features for this image
		vector<ofPoint> getFeatures();
		vector<ofPoint> getCurrent();
		vector<ofVec2f> getMotion();
		
		// recalculates features to track
		void resetFeaturesToTrack();
		void setFeaturesToTrack(const vector<ofVec2f> & features);
		void setFeaturesToTrack(const vector<cv::Point2f> & features);
        void resetFlow();
	protected:
		
		void drawFlow(ofRectangle r);
		void calcFlow(Mat prev, Mat next);
		void calcFeaturesToTrack(vector<cv::Point2f> & features, Mat next);
		
		vector<cv::Point2f> prevPts, nextPts;
		
		//LK feature finding parameters
		int windowSize;
		int maxLevel;
		int maxFeatures;
		float qualityLevel;
		
		//min distance for PyrLK 
		int minDistance;
		
		//pyramid levels
		int pyramidLevels;
		
		bool calcFeaturesNextFrame;
		
		//pyramid + err/status data
		vector<cv::Mat> pyramid;
		vector<cv::Mat> prevPyramid;
		vector<uchar> status;
		vector<float> err;
	};
	
	class FlowFarneback : public Flow {
	public:
		
		FlowFarneback();
		virtual ~FlowFarneback();
		
		//see http://opencv.willowgarage.com/documentation/cpp/motion_analysis_and_object_tracking.html
		//for a description of these parameters
		
		void setPyramidScale(float scale);
		void setNumLevels(int levels);
		void setWindowSize(int winsize);
		void setNumIterations(int interations);
		void setPolyN(int polyN);
		void setPolySigma(float polySigma);
		void setUseGaussian(bool gaussian);
		
        Mat& getFlow();
		ofVec2f getTotalFlow();
		ofVec2f getAverageFlow();		
		ofVec2f getFlowOffset(int x, int y);
		ofVec2f getFlowPosition(int x, int y);
		ofVec2f getTotalFlowInRegion(ofRectangle region);
		ofVec2f getAverageFlowInRegion(ofRectangle region);
		
        //call this if you switch to a new video file to reset internal caches
        void resetFlow();
    
	protected:
		cv::Mat flow;

		void drawFlow(ofRectangle rect);
		void calcFlow(Mat prev, Mat next);
		
		float pyramidScale;
		int numLevels;
		int windowSize;
		int numIterations;
		int polyN;
		float polySigma;
		bool farnebackGaussian;
	};
	
}
