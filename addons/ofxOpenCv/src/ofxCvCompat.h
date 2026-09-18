#pragma once

// Compatibility layer so ofxOpenCv keeps compiling against OpenCV 4 (legacy C
// API still present via imgproc_c.h) and OpenCV 5 (C API removed).
//
// OpenCV 4: thin aliases around cv::cvarrToMat.
// OpenCV 5: IplImage / CvMat headers over cv::Mat plus the cv* helpers this
// addon actually calls. Public ofxOpenCv types (getCvImage, CV_INTER_NN, …)
// stay the same.

#include "opencv2/core/version.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/objdetect.hpp"
#if CV_MAJOR_VERSION >= 5 && defined(__has_include)
	#if __has_include("opencv2/xobjdetect.hpp")
		// Haar CascadeClassifier moved from objdetect to contrib xobjdetect.
		#include "opencv2/xobjdetect.hpp"
	#endif
#endif

#if CV_MAJOR_VERSION >= 4 && CV_MAJOR_VERSION < 5
	#include "opencv2/imgproc/imgproc_c.h"
#endif

#include <vector>

#if CV_MAJOR_VERSION >= 5

#ifndef IPL_DEPTH_8U
#define IPL_DEPTH_8U 8
#define IPL_DEPTH_16U 16
#define IPL_DEPTH_32F 32
#endif

#ifndef CV_INTER_NN
#define CV_INTER_NN cv::INTER_NEAREST
#define CV_INTER_LINEAR cv::INTER_LINEAR
#define CV_INTER_AREA cv::INTER_AREA
#define CV_INTER_CUBIC cv::INTER_CUBIC
#endif

#ifndef CV_RGB2GRAY
#define CV_RGB2GRAY cv::COLOR_RGB2GRAY
#define CV_GRAY2RGB cv::COLOR_GRAY2RGB
#define CV_RGB2HSV cv::COLOR_RGB2HSV
#define CV_HSV2RGB cv::COLOR_HSV2RGB
#endif

#ifndef CV_THRESH_BINARY
#define CV_THRESH_BINARY cv::THRESH_BINARY
#define CV_THRESH_BINARY_INV cv::THRESH_BINARY_INV
#define CV_ADAPTIVE_THRESH_MEAN_C cv::ADAPTIVE_THRESH_MEAN_C
#define CV_ADAPTIVE_THRESH_GAUSSIAN_C cv::ADAPTIVE_THRESH_GAUSSIAN_C
#endif

#ifndef CV_BLUR
#define CV_BLUR 1
#define CV_GAUSSIAN 2
#endif

#ifndef CV_32F
#define CV_32F CV_32FC1
#endif

struct IplROI {
	int coi = 0;
	int xOffset = 0;
	int yOffset = 0;
	int width = 0;
	int height = 0;
};

struct IplImage {
	int nChannels = 0;
	int depth = 0;
	int origin = 0;
	int width = 0;
	int height = 0;
	int widthStep = 0;
	char * imageData = nullptr;
	IplROI * roi = nullptr;
	IplROI roiStorage {};
	cv::Mat mat;
};

struct CvMat {
	cv::Mat mat;
	union {
		unsigned char * ptr;
		float * fl;
	} data { nullptr };
	void syncPtr() { data.ptr = mat.data; }
};

using CvSize = cv::Size;
using CvRect = cv::Rect;
using CvPoint = cv::Point;
using CvPoint2D32f = cv::Point2f;
using CvScalar = cv::Scalar;

inline CvSize cvSize(int w, int h) { return cv::Size(w, h); }
inline CvRect cvRect(int x, int y, int w, int h) { return cv::Rect(x, y, w, h); }
inline CvScalar cvScalar(double a, double b = 0, double c = 0, double d = 0) {
	return cv::Scalar(a, b, c, d);
}

inline int ofxCvIplToCvType(int iplDepth, int nChannels) {
	int depth = CV_8U;
	switch(iplDepth) {
		case IPL_DEPTH_8U: depth = CV_8U; break;
		case IPL_DEPTH_16U: depth = CV_16U; break;
		case IPL_DEPTH_32F: depth = CV_32F; break;
		default: depth = CV_8U; break;
	}
	return CV_MAKETYPE(depth, nChannels);
}

inline void ofxCvSyncHeader(IplImage * img) {
	if(!img) {
		return;
	}
	img->width = img->mat.cols;
	img->height = img->mat.rows;
	img->nChannels = img->mat.channels();
	img->widthStep = (int)img->mat.step;
	img->imageData = img->mat.data ? (char *)img->mat.data : nullptr;
}

inline cv::Mat ofxCvAsMat(IplImage * img) {
	if(!img) {
		return cv::Mat();
	}
	if(img->roi) {
		return img->mat(cv::Rect(img->roi->xOffset, img->roi->yOffset, img->roi->width, img->roi->height));
	}
	return img->mat;
}

inline cv::Mat ofxCvAsMat(const IplImage * img) {
	return ofxCvAsMat(const_cast<IplImage *>(img));
}

inline cv::Mat ofxCvToMat(IplImage * img) { return ofxCvAsMat(img); }
inline cv::Mat ofxCvToMat(const IplImage * img) { return ofxCvAsMat(img); }
inline cv::Mat ofxCvToMat(CvMat * m) { return m ? m->mat : cv::Mat(); }
inline cv::Mat ofxCvToMat(const CvMat * m) { return m ? m->mat : cv::Mat(); }
inline cv::Mat ofxCvToMat(const CvMat & m) { return m.mat; }

// copyTo / OutputArray may reallocate a temporary Mat header; write back if so.
inline void ofxCvWrite(IplImage * dst, cv::Mat & result) {
	if(!dst) {
		return;
	}
	cv::Mat target = ofxCvAsMat(dst);
	if(result.data != target.data) {
		result.copyTo(target);
	}
	ofxCvSyncHeader(dst);
}

inline IplImage * cvCreateImage(CvSize size, int depth, int channels) {
	auto * img = new IplImage();
	img->depth = depth;
	img->nChannels = channels;
	img->origin = 0;
	img->roi = nullptr;
	img->mat = cv::Mat::zeros(size.height, size.width, ofxCvIplToCvType(depth, channels));
	ofxCvSyncHeader(img);
	return img;
}

inline void cvReleaseImage(IplImage ** p) {
	if(p && *p) {
		delete *p;
		*p = nullptr;
	}
}

inline CvMat * cvCreateMat(int rows, int cols, int type) {
	auto * m = new CvMat();
	m->mat = cv::Mat::zeros(rows, cols, type);
	m->syncPtr();
	return m;
}

inline void cvReleaseMat(CvMat ** p) {
	if(p && *p) {
		delete *p;
		*p = nullptr;
	}
}

inline CvMat cvMat(int rows, int cols, int type, void * data) {
	CvMat m;
	m.mat = cv::Mat(rows, cols, type, data);
	m.syncPtr();
	return m;
}

inline void cvmSet(CvMat * m, int row, int col, double value) {
	if(!m) {
		return;
	}
	if(m->mat.type() == CV_32FC1 || m->mat.depth() == CV_32F) {
		m->mat.at<float>(row, col) = (float)value;
	} else {
		m->mat.at<double>(row, col) = value;
	}
}

inline void cvSetZero(CvMat * m) {
	if(m) {
		m->mat.setTo(0);
	}
}

inline void cvSetImageROI(IplImage * img, CvRect rect) {
	if(!img) {
		return;
	}
	img->roiStorage.xOffset = rect.x;
	img->roiStorage.yOffset = rect.y;
	img->roiStorage.width = rect.width;
	img->roiStorage.height = rect.height;
	img->roiStorage.coi = 0;
	img->roi = &img->roiStorage;
}

inline void cvResetImageROI(IplImage * img) {
	if(img) {
		img->roi = nullptr;
	}
}

inline CvRect cvGetImageROI(const IplImage * img) {
	if(!img) {
		return cv::Rect();
	}
	if(img->roi) {
		return cv::Rect(img->roi->xOffset, img->roi->yOffset, img->roi->width, img->roi->height);
	}
	return cv::Rect(0, 0, img->width, img->height);
}

inline void cvCopy(const IplImage * src, IplImage * dst, const IplImage * mask = nullptr) {
	cv::Mat d = ofxCvAsMat(dst);
	if(mask) {
		ofxCvAsMat(src).copyTo(d, ofxCvAsMat(mask));
	} else {
		ofxCvAsMat(src).copyTo(d);
	}
	ofxCvWrite(dst, d);
}

inline void cvSet(IplImage * img, CvScalar value) { ofxCvAsMat(img).setTo(value); }

inline void cvResize(const IplImage * src, IplImage * dst, int interpolation = CV_INTER_LINEAR) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::resize(ofxCvAsMat(src), d, d.size(), 0, 0, interpolation);
	ofxCvWrite(dst, d);
}

inline void cvCvtColor(const IplImage * src, IplImage * dst, int code) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::cvtColor(ofxCvAsMat(src), d, code);
	ofxCvWrite(dst, d);
}

inline void cvConvertScale(const IplImage * src, IplImage * dst, double scale = 1, double shift = 0) {
	cv::Mat d = ofxCvAsMat(dst);
	ofxCvAsMat(src).convertTo(d, d.type(), scale, shift);
	ofxCvWrite(dst, d);
}

inline void cvAbsDiff(const IplImage * src1, const IplImage * src2, IplImage * dst) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::absdiff(ofxCvAsMat(src1), ofxCvAsMat(src2), d);
	ofxCvWrite(dst, d);
}

inline void cvAdd(const IplImage * src1, const IplImage * src2, IplImage * dst) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::add(ofxCvAsMat(src1), ofxCvAsMat(src2), d);
	ofxCvWrite(dst, d);
}

inline void cvSub(const IplImage * src1, const IplImage * src2, IplImage * dst) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::subtract(ofxCvAsMat(src1), ofxCvAsMat(src2), d);
	ofxCvWrite(dst, d);
}

inline void cvMul(const IplImage * src1, const IplImage * src2, IplImage * dst, double scale = 1) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::multiply(ofxCvAsMat(src1), ofxCvAsMat(src2), d, scale);
	ofxCvWrite(dst, d);
}

inline void cvAnd(const IplImage * src1, const IplImage * src2, IplImage * dst) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::bitwise_and(ofxCvAsMat(src1), ofxCvAsMat(src2), d);
	ofxCvWrite(dst, d);
}

inline void cvNot(const IplImage * src, IplImage * dst) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::bitwise_not(ofxCvAsMat(src), d);
	ofxCvWrite(dst, d);
}

inline void cvAddS(const IplImage * src, CvScalar value, IplImage * dst) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::add(ofxCvAsMat(src), value, d);
	ofxCvWrite(dst, d);
}

inline void cvSubS(const IplImage * src, CvScalar value, IplImage * dst) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::subtract(ofxCvAsMat(src), value, d);
	ofxCvWrite(dst, d);
}

inline void cvAddWeighted(const IplImage * src1, double alpha, const IplImage * src2, double beta, double gamma, IplImage * dst) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::addWeighted(ofxCvAsMat(src1), alpha, ofxCvAsMat(src2), beta, gamma, d);
	ofxCvWrite(dst, d);
}

inline void cvDilate(const IplImage * src, IplImage * dst, void * = nullptr, int iterations = 1) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::dilate(ofxCvAsMat(src), d, cv::Mat(), cv::Point(-1, -1), iterations);
	ofxCvWrite(dst, d);
}

inline void cvErode(const IplImage * src, IplImage * dst, void * = nullptr, int iterations = 1) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::erode(ofxCvAsMat(src), d, cv::Mat(), cv::Point(-1, -1), iterations);
	ofxCvWrite(dst, d);
}

inline void cvSmooth(const IplImage * src, IplImage * dst, int smoothtype = CV_GAUSSIAN, int size1 = 3, int = 0, double = 0, double = 0) {
	cv::Mat d = ofxCvAsMat(dst);
	if(smoothtype == CV_BLUR) {
		cv::blur(ofxCvAsMat(src), d, cv::Size(size1, size1));
	} else {
		cv::GaussianBlur(ofxCvAsMat(src), d, cv::Size(size1, size1), 0);
	}
	ofxCvWrite(dst, d);
}

inline void cvFlip(const IplImage * src, IplImage * dst, int flipCode) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::flip(ofxCvAsMat(src), d, flipCode);
	ofxCvWrite(dst, d);
}

inline void cvThreshold(const IplImage * src, IplImage * dst, double thresh, double maxval, int type) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::threshold(ofxCvAsMat(src), d, thresh, maxval, type);
	ofxCvWrite(dst, d);
}

inline void cvAdaptiveThreshold(const IplImage * src, IplImage * dst, double maxValue, int adaptiveMethod, int thresholdType, int blockSize, double C) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::adaptiveThreshold(ofxCvAsMat(src), d, maxValue, adaptiveMethod, thresholdType, blockSize, C);
	ofxCvWrite(dst, d);
}

inline void cvMinMaxLoc(const IplImage * img, double * minVal, double * maxVal, CvPoint * minLoc = nullptr, CvPoint * maxLoc = nullptr, const IplImage * mask = nullptr) {
	cv::Point minP, maxP;
	cv::minMaxLoc(ofxCvAsMat(img), minVal, maxVal, minLoc ? &minP : nullptr, maxLoc ? &maxP : nullptr, mask ? ofxCvAsMat(mask) : cv::Mat());
	if(minLoc) {
		*minLoc = minP;
	}
	if(maxLoc) {
		*maxLoc = maxP;
	}
}

inline int cvCountNonZero(const IplImage * img) { return cv::countNonZero(ofxCvAsMat(img)); }

inline void cvEqualizeHist(const IplImage * src, IplImage * dst) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::equalizeHist(ofxCvAsMat(src), d);
	ofxCvWrite(dst, d);
}

inline void cvLUT(const IplImage * src, IplImage * dst, const CvMat * lut) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::LUT(ofxCvAsMat(src), lut->mat, d);
	ofxCvWrite(dst, d);
}

inline void cvFillPoly(IplImage * img, CvPoint ** pts, int * npts, int contours, CvScalar color) {
	std::vector<std::vector<cv::Point>> polys(contours);
	for(int c = 0; c < contours; ++c) {
		polys[c].resize(npts[c]);
		for(int i = 0; i < npts[c]; ++i) {
			polys[c][i] = cv::Point(pts[c][i].x, pts[c][i].y);
		}
	}
	cv::fillPoly(ofxCvAsMat(img), polys, color);
}

inline void cvSplit(const IplImage * src, IplImage * c0, IplImage * c1, IplImage * c2, IplImage * c3) {
	std::vector<cv::Mat> ch;
	cv::split(ofxCvAsMat(src), ch);
	if(c0 && ch.size() > 0) {
		ch[0].copyTo(ofxCvAsMat(c0));
	}
	if(c1 && ch.size() > 1) {
		ch[1].copyTo(ofxCvAsMat(c1));
	}
	if(c2 && ch.size() > 2) {
		ch[2].copyTo(ofxCvAsMat(c2));
	}
	if(c3 && ch.size() > 3) {
		ch[3].copyTo(ofxCvAsMat(c3));
	}
}

inline void cvMerge(const IplImage * c0, const IplImage * c1, const IplImage * c2, const IplImage * c3, IplImage * dst) {
	std::vector<cv::Mat> ch;
	if(c0) {
		ch.push_back(ofxCvAsMat(c0));
	}
	if(c1) {
		ch.push_back(ofxCvAsMat(c1));
	}
	if(c2) {
		ch.push_back(ofxCvAsMat(c2));
	}
	if(c3) {
		ch.push_back(ofxCvAsMat(c3));
	}
	cv::Mat d = ofxCvAsMat(dst);
	cv::merge(ch, d);
	ofxCvWrite(dst, d);
}

inline void cvWarpAffine(const IplImage * src, IplImage * dst, const CvMat * map) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::warpAffine(ofxCvAsMat(src), d, map->mat, d.size());
	ofxCvWrite(dst, d);
}

inline void cvWarpPerspective(const IplImage * src, IplImage * dst, const CvMat * map) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::warpPerspective(ofxCvAsMat(src), d, map->mat, d.size());
	ofxCvWrite(dst, d);
}

inline void cvRemap(const IplImage * src, IplImage * dst, const IplImage * mapx, const IplImage * mapy) {
	cv::Mat d = ofxCvAsMat(dst);
	cv::remap(ofxCvAsMat(src), d, ofxCvAsMat(mapx), ofxCvAsMat(mapy), cv::INTER_LINEAR);
	ofxCvWrite(dst, d);
}

inline void cvGetPerspectiveTransform(const CvPoint2D32f * src, const CvPoint2D32f * dst, CvMat * map) {
	cv::Point2f s[4], d[4];
	for(int i = 0; i < 4; ++i) {
		s[i] = src[i];
		d[i] = dst[i];
	}
	cv::Mat H = cv::getPerspectiveTransform(s, d);
	H.convertTo(map->mat, map->mat.type());
	map->syncPtr();
}

#else // OpenCV 4 and older: C API is still there

inline cv::Mat ofxCvToMat(const IplImage * img) {
	return cv::cvarrToMat(const_cast<IplImage *>(img));
}
inline cv::Mat ofxCvToMat(IplImage * img) { return cv::cvarrToMat(img); }
inline cv::Mat ofxCvToMat(const CvMat * m) { return cv::cvarrToMat(const_cast<CvMat *>(m)); }
inline cv::Mat ofxCvToMat(CvMat * m) { return cv::cvarrToMat(m); }
inline cv::Mat ofxCvToMat(const CvMat & m) { return cv::cvarrToMat(const_cast<CvMat *>(&m)); }

#endif // CV_MAJOR_VERSION >= 5
