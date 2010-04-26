
#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include "ofxCvFloatImage.h"
#include "ofxCvShortImage.h"




//--------------------------------------------------------------------------------
ofxCvFloatImage::ofxCvFloatImage() {
    init();
}

//--------------------------------------------------------------------------------
ofxCvFloatImage::ofxCvFloatImage( const ofxCvFloatImage& _mom ) {
    init();
    if( _mom.bAllocated ) {
        // cast non-const,  to get read access to the mon::cvImage
        ofxCvFloatImage& mom = const_cast<ofxCvFloatImage&>(_mom);
        allocate( (int)mom.getWidth(), (int)mom.getHeight() );
        cvCopy( mom.getCvImage(), cvImage, 0 );
    } else {
        ofLog(OF_LOG_NOTICE, "in ofxCvFloatImage copy constructor, mom not allocated");
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::init() {
    ipldepth = IPL_DEPTH_32F;
    iplchannels = 1;
    gldepth = GL_FLOAT;
    glchannels = GL_LUMINANCE;
    floatPixels = NULL;
    bFloatPixelsDirty = true;
    floatPixelsW = 0;
    floatPixelsH = 0;
    cvGrayscaleImage = NULL;
    scaleMin = 0.0f;
    scaleMax = 1.0f;
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::clear() {
    if (bAllocated == true){
        if( cvGrayscaleImage != NULL ){
            cvReleaseImage( &cvGrayscaleImage );
        }
        if( floatPixels != NULL ) {
            delete floatPixels;
            floatPixels = NULL;
            bFloatPixelsDirty = true;
            floatPixelsW = 0;
            floatPixelsH = 0;
        }
    }
    ofxCvImage::clear();    //call clear in base class
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::setNativeScale( float _scaleMin, float _scaleMax ) {
    scaleMin = _scaleMin;
    scaleMax = _scaleMax;
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::flagImageChanged() {
    bFloatPixelsDirty = true;
    ofxCvImage::flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::convertFloatToGray( IplImage* floatImg, IplImage* grayImg ) {
    // map from scaleMin-scaleMax to 0-255
    float scale = 255.0f/(scaleMax-scaleMin);
    cvConvertScale( floatImg, grayImg, scale, -(scaleMin*scale) );
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::convertGrayToFloat( IplImage* grayImg, IplImage* floatImg ) {
    // map from 0-255 to scaleMin-scaleMax
    cvConvertScale( grayImg, floatImg, (scaleMax-scaleMin)/255.0f, scaleMin );
}




// Set Pixel Data

//-------------------------------------------------------------------------------------
void ofxCvFloatImage::set(float value){
	cvSet(cvImage, cvScalar(value));
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator *= ( float scalar ){
    ofRectangle roi = getROI();
    for( int i=0; i<roi.height; i++ ) {
        float* ptr = (float*)(cvImage->imageData + (int)(i+roi.y)*cvImage->widthStep);
        for( int j=0; j<roi.width; j++ ) {
            ptr[(int)(j+roi.x)] = ptr[(int)(j+roi.x)]<0.00001 ? 0 : ptr[(int)(j+roi.x)]*scalar;
        }
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator /= ( float scalar ){
    scalar = 1.0 / scalar;
    ofRectangle roi = getROI();
    for( int i=0; i<roi.height; i++ ) {
        float* ptr = (float*)(cvImage->imageData + (int)(i+roi.y)*cvImage->widthStep);
        for( int j=0; j<roi.width; j++ ) {
            ptr[(int)(j+roi.x)] *= scalar;
        }
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::setFromPixels( unsigned char* _pixels, int w, int h ) {
    // This sets the internal image ignoring any ROI

    if( w == width && h == height ) {
        ofRectangle lastROI = getROI();
        if( cvGrayscaleImage == NULL ) {
            cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
        }
        resetImageROI(cvGrayscaleImage);
        // copy _pixels into cvGrayscaleImage
        for( int i=0; i < height; i++ ) {
            memcpy( cvGrayscaleImage->imageData + (i*cvGrayscaleImage->widthStep),
                    _pixels + (i*w),
                    width );
        }
        convertGrayToFloat(cvGrayscaleImage, cvImage);
        setROI(lastROI);
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in setFromPixels, size mismatch");
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::setFromPixels( float* _pixels, int w, int h ) {
    // This sets the internal image ignoring any ROI

    if( w == width && h == height ) {
        // copy _pixels into cvImage
        for( int i=0; i < height; i++ ) {
            memcpy( cvImage->imageData + (i*cvImage->widthStep),
                    _pixels + (i*w),
                    width );
        }
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in setFromPixels, size mismatch");
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::setRoiFromPixels( unsigned char* _pixels, int w, int h ) {
    ofRectangle roi = getROI();
    ofRectangle inputROI = ofRectangle( roi.x, roi.y, w, h);
    ofRectangle iRoi = getIntersectionROI( roi, inputROI );

    if( iRoi.width > 0 && iRoi.height > 0 ) {
        if( cvGrayscaleImage == NULL ) {
            cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
        }
        setImageROI(cvGrayscaleImage, roi);  //make sure ROI is in sync
        // copy _pixels into cvGrayscaleImage
        for( int i=0; i < iRoi.height; i++ ) {
            memcpy( cvGrayscaleImage->imageData + ((i+(int)iRoi.y)*cvGrayscaleImage->widthStep) + (int)iRoi.x,
                    _pixels + (i*w),
                    (int)(iRoi.width) );
        }
        convertGrayToFloat(cvGrayscaleImage, cvImage);
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in setRoiFromPixels, ROI mismatch");
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::setRoiFromPixels( float* _pixels, int w, int h ) {
    ofRectangle roi = getROI();
    ofRectangle inputROI = ofRectangle( roi.x, roi.y, w, h);
    ofRectangle iRoi = getIntersectionROI( roi, inputROI );

    if( iRoi.width > 0 && iRoi.height > 0 ) {
        // copy _pixels into cvImage
        for( int i=0; i < iRoi.height; i++ ) {
            memcpy( cvImage->imageData + ((i+(int)iRoi.y)*cvImage->widthStep) + (int)iRoi.x*sizeof(float),
                    _pixels + (i*w),
                    (int)(iRoi.width*sizeof(float)) );
        }
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in setRoiFromPixels, ROI mismatch");
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator = ( unsigned char* _pixels ) {
    setFromPixels( _pixels, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator = ( float* _pixels ) {
    setFromPixels( _pixels, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator = ( const ofxCvGrayscaleImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvGrayscaleImage& mom = const_cast<ofxCvGrayscaleImage&>(_mom);
	if( matchingROI(getROI(), mom.getROI()) ) {
        convertGrayToFloat(mom.getCvImage(), cvImage);
        flagImageChanged();
	} else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
	}
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator = ( const ofxCvColorImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvColorImage& mom = const_cast<ofxCvColorImage&>(_mom);
	if( matchingROI(getROI(), mom.getROI()) ) {
        if( cvGrayscaleImage == NULL ) {
            cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
        }
        ofRectangle roi = getROI();
        setImageROI(cvGrayscaleImage, roi);
		cvCvtColor( mom.getCvImage(), cvGrayscaleImage, CV_RGB2GRAY );
        convertGrayToFloat(cvGrayscaleImage, cvImage);
        flagImageChanged();
	} else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
	}
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator = ( const ofxCvFloatImage& _mom ) {
    if(this != &_mom) {  //check for self-assignment
        // cast non-const,  no worries, we will reverse any chages
        ofxCvFloatImage& mom = const_cast<ofxCvFloatImage&>(_mom);
        if( matchingROI(getROI(), mom.getROI()) ) {
            if( getNativeScaleMin() == mom.getNativeScaleMin() &&
                getNativeScaleMax() == mom.getNativeScaleMax() )
            {
                cvCopy( mom.getCvImage(), cvImage, 0 );
            } else {
                rangeMap( mom.getCvImage(), cvImage,
                          mom.getNativeScaleMin(), mom.getNativeScaleMax(),
                          getNativeScaleMin(), getNativeScaleMax() );
            }
            flagImageChanged();
        } else {
            ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
        }
    } else {
        ofLog(OF_LOG_WARNING, "in =, you are assigning a ofxCvFloatImage to itself");
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator = ( const ofxCvShortImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvShortImage& mom = const_cast<ofxCvShortImage&>(_mom);
    if( matchingROI(getROI(), mom.getROI()) ) {
        rangeMap( mom.getCvImage(), cvImage,
                  0, 65535.0f, getNativeScaleMin(), getNativeScaleMax() );
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator = ( const IplImage* _mom ) {
    ofxCvImage::operator = (_mom);
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator *= ( ofxCvImage& mom ) {
	if( mom.getCvImage()->nChannels == cvImage->nChannels &&
        mom.getCvImage()->depth == cvImage->depth )
    {
        if( matchingROI(getROI(), mom.getROI()) ) {
            cvMul( cvImage, mom.getCvImage(), cvImageTemp );
            swapTemp();
            flagImageChanged();
        } else {
            ofLog(OF_LOG_ERROR, "in *=, ROI mismatch");
        }
	} else {
        ofLog(OF_LOG_ERROR, "in *=, images need to have matching type");
	}
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator &= ( ofxCvImage& mom ) {
	if( mom.getCvImage()->nChannels == cvImage->nChannels &&
        mom.getCvImage()->depth == cvImage->depth )
    {
        if( matchingROI(getROI(), mom.getROI()) ) {
            //this is doing it bit-wise; probably not what we want
            cvAnd( cvImage, mom.getCvImage(), cvImageTemp );
            swapTemp();
            flagImageChanged();
        } else {
            ofLog(OF_LOG_ERROR, "in &=, ROI mismatch");
        }
	} else {
        ofLog(OF_LOG_ERROR, "in &=, images need to have matching type");
	}
}


//--------------------------------------------------------------------------------
void ofxCvFloatImage::addWeighted( ofxCvGrayscaleImage& mom, float f ) {
	if( matchingROI(getROI(), mom.getROI()) ) {
        convertGrayToFloat(mom.getCvImage(), cvImageTemp);
        cvAddWeighted( cvImageTemp, f, cvImage, 1.0f-f,0, cvImage );
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in addWeighted, ROI mismatch");
    }
}



// Get Pixel Data

//--------------------------------------------------------------------------------
unsigned char*  ofxCvFloatImage::getPixels(){
    if(bPixelsDirty) {

        if( cvGrayscaleImage == NULL ) {
            cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
        }

        ofRectangle lastROI = getROI();
        resetImageROI(cvGrayscaleImage);
        convertFloatToGray(cvImage, cvGrayscaleImage);
        setROI(lastROI);

        if(pixels == NULL) {
            // we need pixels, allocate it
            pixels = new unsigned char[width*height];
            pixelsWidth = width;
            pixelsHeight = height;
        } else if(pixelsWidth != width || pixelsHeight != height) {
            // ROI changed, reallocate pixels for new size
            delete pixels;
            pixels = new unsigned char[width*height];
            pixelsWidth = width;
            pixelsHeight = height;
        }

        // copy from ROI to pixels
        for( int i = 0; i < height; i++ ) {
            memcpy( pixels + (i*width),
                    cvGrayscaleImage->imageData + (i*cvGrayscaleImage->widthStep),
                    width );
        }
        bPixelsDirty = false;
    }
	return pixels;
}

//--------------------------------------------------------------------------------
float*  ofxCvFloatImage::getPixelsAsFloats(){
    if(bFloatPixelsDirty) {
        if(floatPixels == NULL) {
            // we need pixels, allocate it
            floatPixels = new float[width*height];
            floatPixelsW = width;
            floatPixelsH = height;
        } else if(floatPixelsW != width || floatPixelsH != height) {
            // ROI changed, reallocate floatPixels for new size
            delete floatPixels;
            floatPixels = new float[width*height];
            floatPixelsW = width;
            floatPixelsH = height;
        }

        // copy from ROI to pixels
        for( int i = 0; i < height; i++ ) {
            memcpy( floatPixels + (i*width),
                    cvImage->imageData + (i*cvImage->widthStep),
                    width*sizeof(float) );
        }
        bFloatPixelsDirty = false;
    }
	return floatPixels;
}

//--------------------------------------------------------------------------------
unsigned char*  ofxCvFloatImage::getRoiPixels(){
    if(bPixelsDirty) {
        if( cvGrayscaleImage == NULL ) {
            cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
        }

        ofRectangle roi = getROI();
        setImageROI(cvGrayscaleImage, roi);  //make sure ROI is in sync
        convertFloatToGray(cvImage, cvGrayscaleImage);

        if(pixels == NULL) {
            // we need pixels, allocate it
            pixels = new unsigned char[(int)(roi.width*roi.height)];
            pixelsWidth = (int)roi.width;
            pixelsHeight = (int)roi.height;
        } else if(pixelsWidth != roi.width || pixelsHeight != roi.height) {
            // ROI changed, reallocate pixels for new size
            delete pixels;
            pixels = new unsigned char[(int)(roi.width*roi.height)];
            pixelsWidth = (int)roi.width;
            pixelsHeight = (int)roi.height;
        }

        // copy from ROI to pixels
        for( int i = 0; i < roi.height; i++ ) {
            memcpy( pixels + (int)(i*roi.width),
                    cvGrayscaleImage->imageData + ((int)(i+roi.y)*cvGrayscaleImage->widthStep) + (int)roi.x,
                    (int) roi.width );
        }
        bPixelsDirty = false;
    }
	return pixels;
}

//--------------------------------------------------------------------------------
float*  ofxCvFloatImage::getRoiPixelsAsFloats(){
    if(bFloatPixelsDirty) {
        ofRectangle roi = getROI();
        if(floatPixels == NULL) {
            // we need pixels, allocate it
            floatPixels = new float[(int)(roi.width*roi.height)];
            floatPixelsW = (int)roi.width;
            floatPixelsH = (int)roi.height;
        } else if(floatPixelsW != roi.width || floatPixelsH != roi.height) {
            // ROI changed, reallocate floatPixels for new size
            delete floatPixels;
            floatPixels = new float[(int)(roi.width*roi.height)];
            floatPixelsW = (int)roi.width;
            floatPixelsH = (int)roi.height;
        }

        // copy from ROI to pixels
        for( int i = 0; i < roi.height; i++ ) {
            memcpy( floatPixels + (int)(i*roi.width),
                    cvImage->imageData + ((int)(i+roi.y)*cvImage->widthStep) + (int)roi.x*sizeof(float),
                    (int)(roi.width * sizeof(float)) );
        }
        bFloatPixelsDirty = false;
    }
	return floatPixels;
}



// Draw Image



// Image Filter Operations

//--------------------------------------------------------------------------------
void ofxCvFloatImage::contrastStretch() {
	double minVal, maxVal;
	cvMinMaxLoc( cvImage, &minVal, &maxVal, NULL, NULL, 0 );
    rangeMap( cvImage, minVal,maxVal, scaleMin,scaleMax );
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::convertToRange(float min, float max ){
    rangeMap( cvImage, scaleMin,scaleMax, min,max);
    flagImageChanged();
}



// Image Transformation Operations

//--------------------------------------------------------------------------------
void ofxCvFloatImage::resize( int w, int h ) {

    // note, one image copy operation could be ommitted by
    // reusing the temporal image storage

    IplImage* temp = cvCreateImage( cvSize(w,h), IPL_DEPTH_32F, 1 );
    cvResize( cvImage, temp );
    clear();
    allocate( w, h );
    cvCopy( temp, cvImage );
    cvReleaseImage( &temp );
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::scaleIntoMe( ofxCvImage& mom, int interpolationMethod ){
    //for interpolation you can pass in:
    //CV_INTER_NN - nearest-neigbor interpolation,
    //CV_INTER_LINEAR - bilinear interpolation (used by default)
    //CV_INTER_AREA - resampling using pixel area relation. It is preferred method
    //                for image decimation that gives moire-free results. In case of
    //                zooming it is similar to CV_INTER_NN method.
    //CV_INTER_CUBIC - bicubic interpolation.

    if( mom.getCvImage()->nChannels == cvImage->nChannels &&
        mom.getCvImage()->depth == cvImage->depth ) {

        if ((interpolationMethod != CV_INTER_NN) &&
            (interpolationMethod != CV_INTER_LINEAR) &&
            (interpolationMethod != CV_INTER_AREA) &&
            (interpolationMethod != CV_INTER_CUBIC) ){
            ofLog(OF_LOG_WARNING, "in scaleIntoMe, setting interpolationMethod to CV_INTER_NN");
    		interpolationMethod = CV_INTER_NN;
    	}
        cvResize( mom.getCvImage(), cvImage, interpolationMethod );
        flagImageChanged();

    } else {
        ofLog(OF_LOG_ERROR, "in scaleIntoMe, mom image type has to match");
    }
}
