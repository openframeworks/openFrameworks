/*
 utilities are used internally by ofxCv, and make it easier to write code that
 can work with OpenCv and openFrameworks data.
 
 useful functions from this file:
 - imitate and copy
 - toCv and toOf
 */

#pragma once

#include "ofMain.h"
#include "opencv2/opencv.hpp"

namespace ofxCv {
	
	using namespace cv;
	
	// these functions are for accessing Mat, ofPixels and ofImage consistently.
	// they're very important for imitate().
	
	// width, height
	template <class T> inline int getWidth(T& src) {return src.getWidth();}
	template <class T> inline int getHeight(T& src) {return src.getHeight();}
	inline int getWidth(Mat& src) {return src.cols;}
	inline int getHeight(Mat& src) {return src.rows;}
	template <class T> inline bool getAllocated(T& src) {
		return getWidth(src) > 0 && getHeight(src) > 0;
	}
	
	// depth
    inline int getDepth(int cvImageType) {
        return CV_MAT_DEPTH(cvImageType);
    }
	inline int getDepth(Mat& mat) {
		return mat.depth();
	}
    inline int getDepth(ofTexture& tex) {
        // avoid "texture not allocated" warning
        if(!tex.isAllocated()) {
            return CV_8U;
        }
        int type = tex.getTextureData().glInternalFormat;
        switch(type) {
            case GL_RGBA:
            case GL_RGB:
            case GL_LUMINANCE_ALPHA:
            case GL_LUMINANCE:
                return CV_8U;
                
#ifndef TARGET_OPENGLES
            case GL_RGBA8:
            case GL_RGB8:
            case GL_LUMINANCE8:
            case GL_LUMINANCE8_ALPHA8:
                return CV_8U;
                
            case GL_RGBA32F_ARB:
            case GL_RGB32F_ARB:
            case GL_LUMINANCE32F_ARB:
                return CV_32F;
#endif
            default: return 0;
        }
    }
	template <class T> inline int getDepth(ofPixels_<T>& pixels) {
		switch(pixels.getBytesPerChannel()) {
			case 4: return CV_32F;
			case 2: return CV_16U;
			case 1: default: return CV_8U;
		}
	}
	template <> inline int getDepth(ofPixels_<signed short>& pixels) {
		return CV_16S;
	}
	template <> inline int getDepth(ofPixels_<signed char>& pixels) {
		return CV_8S;
	}
	template <class T> inline int getDepth(ofBaseHasPixels_<T>& img) {
		return getDepth(img.getPixels());
	}
	
	// channels
	inline int getChannels(int cvImageType) {
		return CV_MAT_CN(cvImageType);
	}
	inline int getChannels(ofImageType imageType) {
		switch(imageType) {
			case OF_IMAGE_COLOR_ALPHA: return 4;
			case OF_IMAGE_COLOR: return 3;
			case OF_IMAGE_GRAYSCALE: default: return 1;
		}
	}
	inline int getChannels(Mat& mat) {
		return mat.channels();
	}
    inline int getChannels(ofTexture& tex) {
        // avoid "texture not allocated" warning
        if(!tex.isAllocated()) {
            return GL_RGB;
        }
        int type = tex.getTextureData().glInternalFormat;
        switch(type) {
            case GL_RGBA: return 4;
            case GL_RGB: return 3;
            case GL_LUMINANCE_ALPHA: return 2;
            case GL_LUMINANCE: return 1;
                
#ifndef TARGET_OPENGLES
            case GL_RGBA8: return 4;
            case GL_RGB8: return 3;
            case GL_LUMINANCE8: return 1;
            case GL_LUMINANCE8_ALPHA8: return 2;
                
            case GL_RGBA32F_ARB: return 4;
            case GL_RGB32F_ARB: return 3;
            case GL_LUMINANCE32F_ARB: return 1;
#endif
            default: return 0;
        }
    }
	template <class T> inline int getChannels(ofPixels_<T>& pixels) {
		return pixels.getNumChannels();
	}
	template <class T> inline int getChannels(ofBaseHasPixels_<T>& img) {
		return getChannels(img.getPixels());
	}
	
	// image type
	inline int getCvImageType(int channels, int cvDepth = CV_8U) {
		return CV_MAKETYPE(cvDepth, channels);
	}
	inline int getCvImageType(ofImageType imageType, int cvDepth = CV_8U) {
		return CV_MAKETYPE(cvDepth, getChannels(imageType));
	}
	template <class T> inline int getCvImageType(T& img) {
		return CV_MAKETYPE(getDepth(img), getChannels(img));
	}
	inline ofImageType getOfImageType(int cvImageType) {
		switch(getChannels(cvImageType)) {
			case 4: return OF_IMAGE_COLOR_ALPHA;
			case 3: return OF_IMAGE_COLOR;
			case 1: default: return OF_IMAGE_GRAYSCALE;
		}
	}
    inline int getGlImageType(int cvImageType) {
        int channels = getChannels(cvImageType);
        int depth = getDepth(cvImageType);
        switch(depth) {
            case CV_8U:
                switch(channels) {
                    case 1: return GL_LUMINANCE;
                    case 3: return GL_RGB;
                    case 4: return GL_RGBA;
                }
#ifndef TARGET_OPENGLES
            case CV_32F:
                switch(channels) {
                    case 1: return GL_LUMINANCE32F_ARB;
                    case 3: return GL_RGB32F;
                    case 4: return GL_RGBA32F;
                }
#endif
        }
        return 0;
    }
	
	// allocation
	// only happens when necessary
	template <class T> inline void allocate(T& img, int width, int height, int cvType) {
		int iw = getWidth(img), ih = getHeight(img);
		int it = getCvImageType(img);
		if(iw != width || ih != height || it != cvType) {
			img.allocate(width, height, getOfImageType(cvType));
		}
	}
    inline void allocate(ofTexture& img, int width, int height, int cvType) {
        int iw = getWidth(img), ih = getHeight(img);
        int it = getCvImageType(img);
        if(iw != width || ih != height || it != cvType) {
            img.allocate(width, height, getGlImageType(cvType));
        }
    }
	inline void allocate(Mat& img, int width, int height, int cvType) {
		int iw = getWidth(img), ih = getHeight(img);
		int it = getCvImageType(img);
		if(iw != width || ih != height || it != cvType) {
			img.create(height, width, cvType);
		}
	}
	// ofVideoPlayer/Grabber can't be allocated, so we assume we don't need to do anything
	inline void allocate(ofVideoPlayer& img, int width, int height, int cvType) {}
	inline void allocate(ofVideoGrabber& img, int width, int height, int cvType) {}
	
	// imitate() is good for preparing buffers
	// it's like allocate(), but uses the size and type of the original as a reference
	// like allocate(), the image being allocated is the first argument	
	
	// this version copies size, but manually specifies mirror's image type
	template <class M, class O> void imitate(M& mirror, O& original, int mirrorCvImageType) {
		int ow = getWidth(original), oh = getHeight(original);
		allocate(mirror, ow, oh, mirrorCvImageType);
	}
	
	// this version copies size and image type
	template <class M, class O> void imitate(M& mirror, O& original) {
		imitate(mirror, original, getCvImageType(original));
	}
	
	// maximum possible values for that depth or matrix
	float getMaxVal(int cvDepth);
	float getMaxVal(const Mat& mat);
	int getTargetChannelsFromCode(int conversionCode);
    
	// toCv functions
	// for conversion functions, the signature reveals the behavior:
	// 1       Type& argument // creates a shallow copy of the data
	// 2 const Type& argument // creates a deep copy of the data
	// 3       Type  argument // creates a deep copy of the data
	// style 1 is used when possible (for Mat conversion). style 2 is used when
	// dealing with a lot of data that can't/shouldn't be shallow copied. style 3
	// is used for small objects where the compiler can optimize the copying if
	// necessary. the reference is avoided to make inline toCv/toOf use easier.
	
	Mat toCv(Mat& mat);
	template <class T> inline Mat toCv(ofPixels_<T>& pix) {
		return Mat(pix.getHeight(), pix.getWidth(), getCvImageType(pix), pix.getData(), 0);
	}
	template <class T> inline Mat toCv(ofBaseHasPixels_<T>& img) {
		return toCv(img.getPixels());
	}
	Mat toCv(ofMesh& mesh);
	Point2f toCv(ofVec2f vec);
	Point3f toCv(ofVec3f vec);
	cv::Rect toCv(ofRectangle rect);
	vector<cv::Point2f> toCv(const ofPolyline& polyline);
	vector<cv::Point2f> toCv(const vector<ofVec2f>& points);
	vector<cv::Point3f> toCv(const vector<ofVec3f>& points);
	Scalar toCv(ofColor color);
	
	// cross-toolkit, cross-bitdepth copying
	template <class S, class D>
	void copy(S& src, D& dst, int dstDepth) {
		imitate(dst, src, getCvImageType(getChannels(src), dstDepth));
		Mat srcMat = toCv(src), dstMat = toCv(dst);
		if(srcMat.type() == dstMat.type()) {
			srcMat.copyTo(dstMat);
		} else {
			double alpha = getMaxVal(dstMat) / getMaxVal(srcMat);
			srcMat.convertTo(dstMat, dstMat.depth(), alpha);
		}
	}
	
	// most of the time you want the destination to be the same as the source. but
	// sometimes your destination is a different depth, and copy() will notice and
	// do the conversion for you.
	template <class S, class D>
	void copy(S& src, D& dst) {
		int dstDepth;
		if(getAllocated(dst)) {
			dstDepth = getDepth(dst);
		} else {
			dstDepth = getDepth(src);
		}
		copy(src, dst, dstDepth);
	}
	
	// toOf functions
	ofVec2f toOf(Point2f point);
	ofVec3f toOf(Point3f point);
	ofRectangle toOf(cv::Rect rect);
	ofPolyline toOf(cv::RotatedRect rect);
	template <class T> inline ofPolyline toOf(const vector<cv::Point_<T> >& contour) {
		ofPolyline polyline;
		polyline.resize(contour.size());
		for(int i = 0; i < (int)contour.size(); i++) {
			polyline[i].x = contour[i].x;
			polyline[i].y = contour[i].y;
		}
		polyline.close();
		return polyline;
	}
	template <class T>
	void toOf(Mat mat, ofPixels_<T>& pixels) {
		pixels.setFromExternalPixels(mat.ptr<T>(), mat.cols, mat.rows, mat.channels());
	}
	template <class T>
	void toOf(Mat mat, ofImage_<T>& img) {
		imitate(img, mat);
		toOf(mat, img.getPixels());
	}
}
