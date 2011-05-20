
#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include "ofxCvFloatImage.h"
#include "ofxCvShortImage.h"




//--------------------------------------------------------------------------------
ofxCvShortImage::ofxCvShortImage() {
    init();
}

//--------------------------------------------------------------------------------
ofxCvShortImage::ofxCvShortImage( const ofxCvShortImage& _mom ) {
    init();
    if( _mom.bAllocated ) {
        // cast non-const,  to get read access to the mon::cvImage
        ofxCvShortImage& mom = const_cast<ofxCvShortImage&>(_mom);
        allocate( (int)mom.getWidth(), (int)mom.getHeight() );
        cvCopy( mom.getCvImage(), cvImage, 0 );
    } else {
        ofLog(OF_LOG_NOTICE, "in ofxCvShortImage copy constructor, mom not allocated");
    }
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::init() {
    ipldepth = IPL_DEPTH_16U;
    iplchannels = 1;
    gldepth = GL_UNSIGNED_SHORT;
    glchannels = GL_LUMINANCE;
    cvGrayscaleImage = NULL;
    bShortPixelsDirty = true;
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::clear() {
    if (bAllocated == true){
        if( cvGrayscaleImage != NULL ){
            cvReleaseImage( &cvGrayscaleImage );
        }
    }
    ofxCvImage::clear();    //call clear in base class
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::flagImageChanged() {
    bShortPixelsDirty = true;
    ofxCvImage::flagImageChanged();
}


//--------------------------------------------------------------------------------
void ofxCvShortImage::convertShortToGray( IplImage* shortImg, IplImage* grayImg ) {
    // map from 0-65535 to 0-255
    float scale = 255.0f/65535.0f;
    cvConvertScale( shortImg, grayImg, scale, 0 );
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::convertGrayToShort( IplImage* grayImg, IplImage* shortImg ) {
    // map from 0-255 to 0-65535
    cvConvertScale( grayImg, shortImg, 65535.0f/255.0f, 0 );
}




// Set Pixel Data

//-------------------------------------------------------------------------------------
void ofxCvShortImage::set(float value){
	cvSet(cvImage, cvScalar(value));
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::setFromPixels( const unsigned char* _pixels, int w, int h ) {
    // This sets the internal image ignoring any ROI

    if( w == width &&  h == height ) {
        ofRectangle lastROI = getROI();
        if(cvGrayscaleImage == NULL) {
            cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
        }
        resetImageROI(cvGrayscaleImage);
        // copy _pixels into cvGrayscaleImage
        for( int i=0; i < height; i++ ) {
            memcpy( cvGrayscaleImage->imageData + (i*cvGrayscaleImage->widthStep),
                    _pixels + (i*w),
                    width );
        }
        convertGrayToShort(cvGrayscaleImage, cvImage);
        setROI(lastROI);
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in setFromPixels, size mismatch");
    }

}

//--------------------------------------------------------------------------------
void ofxCvShortImage::setRoiFromPixels( const unsigned char* _pixels, int w, int h ) {
    ofRectangle roi = getROI();
    ofRectangle inputROI = ofRectangle( roi.x, roi.y, w, h );
    ofRectangle iRoi = getIntersectionROI( roi, inputROI );

    if( iRoi.width > 0 && iRoi.height > 0 ) {
        if(cvGrayscaleImage == NULL) {
            cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
        }
        setImageROI(cvGrayscaleImage, roi);  //make sure ROI is in sync
        // copy pixels from _pixels, however many we have or will fit in cvGrayscaleImage
        for( int i=0; i < iRoi.height; i++ ) {
            memcpy( cvGrayscaleImage->imageData + ((i+(int)iRoi.y)*cvGrayscaleImage->widthStep) + (int)iRoi.x,
                    _pixels + (i*w),
                    (int)iRoi.width );
        }
        convertGrayToShort(cvGrayscaleImage, cvImage);
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in setRoiFromPixels, ROI mismatch");
    }

}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( unsigned char* _pixels ) {
    setFromPixels( _pixels, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const ofxCvGrayscaleImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvGrayscaleImage& mom = const_cast<ofxCvGrayscaleImage&>(_mom);
	if( matchingROI(getROI(), mom.getROI()) ) {
        convertGrayToShort(mom.getCvImage(), cvImage);
        flagImageChanged();
	} else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
	}
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const ofxCvColorImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvColorImage& mom = const_cast<ofxCvColorImage&>(_mom);
	if( matchingROI(getROI(), mom.getROI()) ) {
        if( cvGrayscaleImage == NULL ) {
            cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
        }
        ofRectangle roi = getROI();
        setImageROI(cvGrayscaleImage, roi);
		cvCvtColor( mom.getCvImage(), cvGrayscaleImage, CV_RGB2GRAY );
        convertGrayToShort(cvGrayscaleImage, cvImage);
        setImageROI(cvGrayscaleImage, roi);
        flagImageChanged();
	} else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
	}
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const ofxCvFloatImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvFloatImage& mom = const_cast<ofxCvFloatImage&>(_mom);
    if( matchingROI(getROI(), mom.getROI()) ) {
        rangeMap( mom.getCvImage(), cvImage,
                  mom.getNativeScaleMin(), mom.getNativeScaleMax(), 0, 65535.0f );
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
    }
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const ofxCvShortImage& _mom ) {
    if(this != &_mom) {  //check for self-assignment
        // cast non-const,  no worries, we will reverse any chages
        ofxCvShortImage& mom = const_cast<ofxCvShortImage&>(_mom);
        if( matchingROI(getROI(), mom.getROI()) ) {
            cvCopy( mom.getCvImage(), cvImage, 0 );
            flagImageChanged();
        } else {
            ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
        }
    } else {
        ofLog(OF_LOG_WARNING, "in =, you are assigning a ofxCvShortImage to itself");
    }
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const IplImage* _mom ) {
    ofxCvImage::operator = (_mom);
}


//--------------------------------------------------------------------------------
void ofxCvShortImage::addWeighted( ofxCvGrayscaleImage& mom, float f ) {
	if( matchingROI(getROI(), mom.getROI()) ) {
        convertGrayToShort(mom.getCvImage(), cvImageTemp);
        cvAddWeighted( cvImageTemp, f, cvImage, 1.0f-f,0, cvImage );
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in addWeighted, ROI mismatch");
    }
}



// Get Pixel Data

//--------------------------------------------------------------------------------
IplImage*  ofxCvShortImage::getCv8BitsImage() {
	if(bPixelsDirty) {
		if( cvGrayscaleImage == NULL ) {
			cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
		}

		ofRectangle lastROI = getROI();

		resetImageROI(cvGrayscaleImage);
		convertShortToGray(cvImage, cvGrayscaleImage);
		setROI(lastROI);
	}
	return cvGrayscaleImage;
}

//--------------------------------------------------------------------------------
IplImage*  ofxCvShortImage::getCv8BitsRoiImage() {
	if(bPixelsDirty) {
		if( cvGrayscaleImage == NULL ) {
			cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
		}

		ofRectangle roi = getROI();
		setImageROI(cvGrayscaleImage, roi);  //make sure ROI is in sync
		convertShortToGray(cvImage, cvGrayscaleImage);
	}
	return cvGrayscaleImage;
}

// Draw Image


// Image Filter Operations

//--------------------------------------------------------------------------------
void ofxCvShortImage::contrastStretch() {
	double minVal, maxVal;
	cvMinMaxLoc( cvImage, &minVal, &maxVal, NULL, NULL, 0 );
    rangeMap( cvImage, minVal,maxVal, 0,65535 );
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::convertToRange(float min, float max ){
    rangeMap( cvImage, 0,65535, min,max);
    flagImageChanged();
}



// Image Transformation Operations

//--------------------------------------------------------------------------------
void ofxCvShortImage::resize( int w, int h ) {

    // note, one image copy operation could be ommitted by
    // reusing the temporal image storage

    IplImage* temp = cvCreateImage( cvSize(w,h), IPL_DEPTH_16U, 1 );
    cvResize( cvImage, temp );
    clear();
    allocate( w, h );
    cvCopy( temp, cvImage );
    cvReleaseImage( &temp );
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::scaleIntoMe( ofxCvImage& mom, int interpolationMethod ){
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

ofShortPixels & ofxCvShortImage::getShortPixelsRef(){
	if(bShortPixelsDirty) {

		//Note this possible introduces a bug where pixels doesn't contain the current image.
		//Also it means that modifying the pointer return by get pixels - affects the internal cvImage
		//Where as with the slower way below modifying the pointer doesn't change the image.
		if(  cvImage->width*cvImage->depth/8 == cvImage->widthStep ){
			shortPixels.setFromExternalPixels((unsigned short*)cvImage->imageData,width,height,cvImage->nChannels);
		}else{
			shortPixels.setFromAlignedPixels((unsigned short*)cvImage->imageData,width,height,cvImage->nChannels,cvImage->widthStep);
		}
		bShortPixelsDirty = false;
	}
	return shortPixels;

}

ofShortPixels & ofxCvShortImage::getRoiShortPixelsRef(){
	if(bShortPixelsDirty) {
		ofRectangle roi = getROI();
		unsigned short * roi_ptr = (unsigned short*)cvImage->imageData + ((int)(roi.y)*cvImage->widthStep/(cvImage->depth/8) + (int)roi.x * cvImage->nChannels);
		shortPixels.setFromAlignedPixels(roi_ptr,roi.width,roi.height,cvImage->nChannels,cvImage->widthStep);
		bShortPixelsDirty = false;
	}
	return shortPixels;

}
