
#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include "ofxCvFloatImage.h"
#include "ofxCvShortImage.h"



//--------------------------------------------------------------------------------
ofxCvGrayscaleImage::ofxCvGrayscaleImage() {
    init();
}

//--------------------------------------------------------------------------------
ofxCvGrayscaleImage::ofxCvGrayscaleImage( const ofxCvGrayscaleImage& _mom ) {
    init();
    if( _mom.bAllocated ) {
        // cast non-const,  to get read access to the mon::cvImage
        ofxCvGrayscaleImage& mom = const_cast<ofxCvGrayscaleImage&>(_mom);
        allocate( (int)mom.getWidth(), (int)mom.getHeight() );
        cvCopy( mom.getCvImage(), cvImage, 0 );
    } else {
        ofLog(OF_LOG_NOTICE, "in ofxCvGrayscaleImage copy constructor, mom not allocated");
    }
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::init() {
    ipldepth = IPL_DEPTH_8U;
    iplchannels = 1;
    gldepth = GL_UNSIGNED_BYTE;
    glchannels = GL_LUMINANCE;
    briConLutMatrix = cvCreateMat(1,256,CV_8UC1);
}


// Set Pixel Data - Arrays
//-------------------------------------------------------------------------------------
void ofxCvGrayscaleImage::set( float value ){
	cvSet(cvImage, cvScalar(value));
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::setFromPixels( unsigned char* _pixels, int w, int h ) {
    // This sets the internal image ignoring any ROI

    if( w == width && h == height ) {
		
		if( cvImage->width == cvImage->widthStep ){
			memcpy( cvImage->imageData,  _pixels, w*h);
		}else{
		
        // copy pixels from _pixels, however many we have or will fit in cvImage
        for( int i=0; i < height; i++ ) {
            memcpy( cvImage->imageData + (i*cvImage->widthStep),
                    _pixels + (i*w),
                    width);
        }
			
		}
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in setFromPixels, size mismatch");
    }
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::setRoiFromPixels( unsigned char* _pixels, int w, int h ) {
    ofRectangle roi = getROI();
    ofRectangle inputROI = ofRectangle( roi.x, roi.y, w, h);
    ofRectangle iRoi = getIntersectionROI( roi, inputROI );

    if( iRoi.width > 0 && iRoi.height > 0 ) {
        // copy pixels from _pixels, however many we have or will fit in cvImage
        for( int i=0; i < iRoi.height; i++ ) {
            memcpy( cvImage->imageData + ((i+(int)iRoi.y)*cvImage->widthStep) + (int)iRoi.x,
                    _pixels + (i*w),
                    (int)(iRoi.width));
        }
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in setRoiFromPixels, ROI mismatch");
    }
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator = ( unsigned char* _pixels ) {
    setFromPixels( _pixels, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator = ( const ofxCvGrayscaleImage& _mom ) {
    if(this != &_mom) {  //check for self-assignment
        // cast non-const,  no worries, we will reverse any chages
        ofxCvGrayscaleImage& mom = const_cast<ofxCvGrayscaleImage&>(_mom);
        if( matchingROI(getROI(), mom.getROI()) ) {
            cvCopy( mom.getCvImage(), cvImage, 0 );
            flagImageChanged();
        } else {
            ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
        }
    } else {
        ofLog(OF_LOG_WARNING, "in =, you are assigning a ofxCvGrayscaleImage to itself");
    }
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator = ( const ofxCvColorImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvColorImage& mom = const_cast<ofxCvColorImage&>(_mom);
	if( matchingROI(getROI(), mom.getROI()) ) {
		cvCvtColor( mom.getCvImage(), cvImage, CV_RGB2GRAY );
        flagImageChanged();
	} else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
	}
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator = ( const ofxCvFloatImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvFloatImage& mom = const_cast<ofxCvFloatImage&>(_mom);
	if( matchingROI(getROI(), mom.getROI()) ) {
        rangeMap( mom.getCvImage(), cvImage,
                  mom.getNativeScaleMin(), mom.getNativeScaleMax(), 0, 255.0f );
        flagImageChanged();
	} else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
	}
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator = ( const ofxCvShortImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvShortImage& mom = const_cast<ofxCvShortImage&>(_mom);
    if( matchingROI(getROI(), mom.getROI()) ) {
        rangeMap( mom.getCvImage(), cvImage, 0, 65535.0f, 0, 255.0f );
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
    }
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator = ( const IplImage* _mom ) {
    ofxCvImage::operator = (_mom);
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::absDiff( ofxCvGrayscaleImage& mom ) {
    if( matchingROI(getROI(), mom.getROI()) ) {
        cvAbsDiff( cvImage, mom.getCvImage(), cvImageTemp );
        swapTemp();
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in *=, ROI mismatch");
    }
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::absDiff( ofxCvGrayscaleImage& mom,
                                   ofxCvGrayscaleImage& dad ) {
    ofRectangle roi = getROI();
    ofRectangle momRoi = mom.getROI();
    ofRectangle dadRoi = dad.getROI();
    if( (momRoi.width == roi.width && momRoi.height == roi.height ) &&
        (dadRoi.width == roi.width && dadRoi.height == roi.height ) )
    {
        cvAbsDiff( mom.getCvImage(), dad.getCvImage(), cvImage );
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in absDiff, images are different sizes");
    }

}


// Get Pixel Data
//--------------------------------------------------------------------------------
unsigned char* ofxCvGrayscaleImage::getPixels() {
	//Note this possible introduces a bug where pixels doesn't contain the current image.
	//Also it means that modifying the pointer return by get pixels - affects the internal cvImage
	//Where as with the slower way below modifying the pointer doesn't change the image. 
	if(  cvImage->width == cvImage->widthStep ){
		return (unsigned char *)cvImage->imageData;
	}

    if(bPixelsDirty) {
        if(pixels == NULL) {
            // we need pixels, allocate it
            pixels = new unsigned char[width*height];
            pixelsWidth = width;
            pixelsHeight = height;
        } else if(pixelsWidth != width || pixelsHeight != height) {
            // ROI changed, reallocate pixels for new size
            // this is needed because getRoiPixels() might change size of pixels
            delete pixels;
            pixels = new unsigned char[width*height];
            pixelsWidth = width;
            pixelsHeight = height;
        }

        // copy from ROI to pixels
        for( int i = 0; i < height; i++ ) {
            memcpy( pixels + (i*width),
                    cvImage->imageData + (i*cvImage->widthStep),
                    width );
        }
        bPixelsDirty = false;
    }
	return pixels;
}

//--------------------------------------------------------------------------------
unsigned char* ofxCvGrayscaleImage::getRoiPixels() {
    if(bPixelsDirty) {
        ofRectangle roi = getROI();
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
                    cvImage->imageData + ((int)(i+roi.y)*cvImage->widthStep) + (int)roi.x,
                    (int)roi.width );
        }
        bPixelsDirty = false;
    }
	return pixels;
}


// Draw Image



// Image Filter Operations

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::contrastStretch() {
	double minVal, maxVal;
	cvMinMaxLoc( cvImage, &minVal, &maxVal, NULL, NULL, 0 );
    rangeMap( cvImage, minVal,maxVal, 0,255 );
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::convertToRange(float min, float max ){
    rangeMap( cvImage, 0, 255, min, max);
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::threshold( int value, bool invert) {
	//http://lush.sourceforge.net/lush-manual/01a8321b.html
	if(invert) cvThreshold( cvImage, cvImageTemp, value, 255, CV_THRESH_BINARY_INV );
	else cvThreshold( cvImage, cvImageTemp, value, 255, CV_THRESH_BINARY );
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::adaptiveThreshold( int blockSize, int offset,
                                             bool invert, bool gauss) {
    if( blockSize < 2 ) {
        ofLog(OF_LOG_NOTICE, "in adaptiveThreshold, value < 2, will make it 3");
        blockSize = 3;
    }

    if( blockSize % 2 == 0 ) {
        ofLog(OF_LOG_NOTICE, "in adaptiveThreshold, value not odd -> will add 1 to cover your back");
        blockSize++;
    }

    int threshold_type = CV_THRESH_BINARY;
    if(invert) threshold_type = CV_THRESH_BINARY_INV;

    int adaptive_method = CV_ADAPTIVE_THRESH_MEAN_C;
    if(gauss) adaptive_method = CV_ADAPTIVE_THRESH_GAUSSIAN_C;

    cvAdaptiveThreshold( cvImage, cvImageTemp, 255, adaptive_method,
                         threshold_type, blockSize, offset);
   swapTemp();
   flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::brightnessContrast(float brightness, float contrast){

	int i;
	
	/*
	 * The algorithm is by Werner D. Streidt
	 * (http://visca.com/ffactory/archives/5-99/msg00021.html)
	 * (note: uses values between -1 and 1)
	 */

	double delta;
	double a;
	double b;
	if( contrast > 0 )
	{
		delta = 127.*contrast;
		a = 255./(255. - delta*2);
		b = a*(brightness*100 - delta);
	}else{
		delta = -128.*contrast;
		a = (256.-delta*2)/255.;
		b = a*brightness*100. + delta;
	}

	for( i = 0; i < 256; i++ )
	{
		int v = cvRound(a*i + b);
		if( v < 0 )
			v = 0;
		if( v > 255 )
			v = 255;
		briConLutMatrix->data.ptr[i] = (uchar)v;
		//briConLut[i] = (uchar)v;
	}

	//cvSetData( briConLutMatrix, briConLut, 0 );

	cvLUT( cvImage, cvImageTemp, briConLutMatrix); 
	swapTemp();
	flagImageChanged();
}


// Image Transformation Operations
//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::resize( int w, int h ) {

    // note, one image copy operation could be ommitted by
    // reusing the temporal image storage

    IplImage* temp = cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 1 );
    cvResize( cvImage, temp );
    clear();
    allocate( w, h );
    cvCopy( temp, cvImage );
    cvReleaseImage( &temp );
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::scaleIntoMe( ofxCvImage& mom, int interpolationMethod ){
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
        ofLog(OF_LOG_ERROR, "in scaleIntoMe: mom image type has to match");
    }
}

