
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
    bFloatPixelsDirty = true;
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

        floatPixels.clear();
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
	if( !bAllocated ){
		ofLog(OF_LOG_ERROR, "in set, image needs to be allocated");	
		return;	
	}
	cvSet(cvImage, cvScalar(value));
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator *= ( float scalar ){
	if( !bAllocated ){
		ofLog(OF_LOG_ERROR, "in *=, image needs to be allocated");	
		return;	
	}
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
	if( !bAllocated ){
		ofLog(OF_LOG_ERROR, "in /=, image needs to be allocated");	
		return;	
	}
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
void ofxCvFloatImage::setFromPixels( const unsigned char* _pixels, int w, int h ) {
    // This sets the internal image ignoring any ROI
	if( w == 0 || h == 0 ){
		ofLog(OF_LOG_ERROR, "in setFromPixels, w and h cannot = 0");
		return;
	}
    if( !bAllocated || w != width || h != height ) {
		ofLog(OF_LOG_NOTICE, "in setFromPixels, reallocating to match dimensions");	
		allocate(w,h);
	}

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
	if( w == 0 || h == 0 ){
		ofLog(OF_LOG_ERROR, "in setFromPixels, w and h cannot = 0");
		return;
	}
    if( !bAllocated || w != width || h != height ) {
		ofLog(OF_LOG_NOTICE, "in setFromPixels, reallocating to match dimensions");	
		allocate(w,h);
	}

    if( w == width && h == height ) {
        // copy _pixels into cvImage
        for( int i=0; i < height; i++ ) {
            memcpy( cvImage->imageData + (i*cvImage->widthStep),
                    _pixels + (i*w),
                    width * sizeof(float) );
        }
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in setFromPixels, size mismatch");
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::setRoiFromPixels( const unsigned char* _pixels, int w, int h ) {
	if( w == 0 || h == 0 ){
		ofLog(OF_LOG_ERROR, "in setFromPixels, w and h cannot = 0");
		return;
	}
   	if(!bAllocated){
		ofLog(OF_LOG_ERROR, "in setRoiFromPixels, image is not allocated");
		return;
	} 
	
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
   	if( w == 0 || h == 0 ){
		ofLog(OF_LOG_ERROR, "in setFromPixels, w and h cannot = 0");
		return;
	}
   	if(!bAllocated){
		ofLog(OF_LOG_ERROR, "in setRoiFromPixels, image is not allocated");
		return;
	} 
	
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
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLog(OF_LOG_ERROR, "in =, mom width or height is 0");	
		return;	
	}
	if( !bAllocated ){
		ofLog(OF_LOG_NOTICE, "in =, allocating to match dimensions");		
		allocate(mom.getWidth(), mom.getHeight());
	}	
	
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
	
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLog(OF_LOG_ERROR, "in =, mom width or height is 0");	
		return;	
	}
	if( !bAllocated ){
		ofLog(OF_LOG_NOTICE, "in =, allocating to match dimensions");		
		allocate(mom.getWidth(), mom.getHeight());
	}
		
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

		if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
			ofLog(OF_LOG_ERROR, "in =, mom width or height is 0");	
			return;	
		}
		if( !bAllocated ){
			ofLog(OF_LOG_NOTICE, "in =, allocating to match dimensions");		
			allocate(mom.getWidth(), mom.getHeight());
		}
			
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
	
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLog(OF_LOG_ERROR, "in =, mom width or height is 0");	
		return;	
	}
	if( !bAllocated ){
		ofLog(OF_LOG_NOTICE, "in =, allocating to match dimensions");		
		allocate(mom.getWidth(), mom.getHeight());
	}
		
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
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLog(OF_LOG_ERROR, "in *=, mom width or height is 0");	
		return;	
	}
	if( !bAllocated ){
		ofLog(OF_LOG_ERROR, "in *=, image is not allocated");		
		return;	
	}
		
	if( mom.getCvImage()->nChannels == cvImage->nChannels && mom.getCvImage()->depth == cvImage->depth ){
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
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLog(OF_LOG_ERROR, "in &=, mom width or height is 0");	
		return;	
	}
	if( !bAllocated ){
		ofLog(OF_LOG_ERROR, "in &=, image is not allocated");		
		return;	
	}

	if( mom.getCvImage()->nChannels == cvImage->nChannels && mom.getCvImage()->depth == cvImage->depth ){
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
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLog(OF_LOG_ERROR, "in addWeighted, mom width or height is 0");	
		return;	
	}
	if( !bAllocated ){
		ofLog(OF_LOG_ERROR, "in addWeighted, image is not allocated");		
		return;	
	}
	
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
IplImage*  ofxCvFloatImage::getCv8BitsImage() {
	if( !bAllocated ){
		ofLog(OF_LOG_WARNING, "in getCv8BitsImage, image is not allocated");		
	}
		
	if(bPixelsDirty) {
		if( cvGrayscaleImage == NULL ) {
			cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
		}

		ofRectangle lastROI = getROI();

		resetImageROI(cvGrayscaleImage);
		convertFloatToGray(cvImage, cvGrayscaleImage);
		setROI(lastROI);
	}
	return cvGrayscaleImage;
}

//--------------------------------------------------------------------------------
IplImage*  ofxCvFloatImage::getCv8BitsRoiImage() {
	if( !bAllocated ){
		ofLog(OF_LOG_WARNING, "in getCv8BitsRoiImage, image is not allocated");		
	}
	
	if(bPixelsDirty) {
		if( cvGrayscaleImage == NULL ) {
			cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
		}

		ofRectangle roi = getROI();
		setImageROI(cvGrayscaleImage, roi);  //make sure ROI is in sync
		convertFloatToGray(cvImage, cvGrayscaleImage);
	}
	return cvGrayscaleImage;
}


//--------------------------------------------------------------------------------
float*  ofxCvFloatImage::getPixelsAsFloats(){
	if( !bAllocated ){
		ofLog(OF_LOG_WARNING, "in getPixelsAsFloats, image is not allocated");		
	}
	
	return getFloatPixelsRef().getPixels();
}
//--------------------------------------------------------------------------------
ofFloatPixels &	ofxCvFloatImage::getFloatPixelsRef(){
	if( !bAllocated ){
		ofLog(OF_LOG_WARNING, "in getPixelsAsFloats, image is not allocated");		
	}
	
	if(bFloatPixelsDirty) {
        if(  cvImage->width*cvImage->depth/8 == cvImage->widthStep ){
        	floatPixels.setFromExternalPixels((float*)cvImage->imageData,width,height,1);
        }else{
        	floatPixels.setFromAlignedPixels((float*)cvImage->imageData,width,height,1,cvImage->widthStep);
        }
        bFloatPixelsDirty = false;
    }
    return floatPixels;
}

//--------------------------------------------------------------------------------
float*  ofxCvFloatImage::getRoiPixelsAsFloats(){
	if( !bAllocated ){
		ofLog(OF_LOG_WARNING, "in getRoiPixelsAsFloats, image is not allocated");		
	}
	
	return getRoiFloatPixelsRef().getPixels();
}

//--------------------------------------------------------------------------------
ofFloatPixels &	ofxCvFloatImage::getRoiFloatPixelsRef(){
	if( !bAllocated ){
		ofLog(OF_LOG_WARNING, "in getRoiFloatPixelsRef, image is not allocated");		
	}
	
	if(bFloatPixelsDirty) {
		ofRectangle roi = getROI();
		float * roi_ptr = (float*)cvImage->imageData + ((int)(roi.y)*cvImage->widthStep/(cvImage->depth/8)) + (int)roi.x;
		floatPixels.setFromAlignedPixels(roi_ptr,roi.width,roi.height,cvImage->nChannels,cvImage->widthStep);
		bPixelsDirty = false;
	}
	return floatPixels;
}


// Draw Image



// Image Filter Operations

//--------------------------------------------------------------------------------
void ofxCvFloatImage::contrastStretch() {
	if( !bAllocated ){
		ofLog(OF_LOG_ERROR, "in contrastStretch, image is not allocated");		
		return;	
	}
	
	double minVal, maxVal;
	cvMinMaxLoc( cvImage, &minVal, &maxVal, NULL, NULL, 0 );
    rangeMap( cvImage, minVal,maxVal, scaleMin,scaleMax );
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::convertToRange(float min, float max ){
	if( !bAllocated ){
		ofLog(OF_LOG_ERROR, "in convertToRange, image is not allocated");		
		return;	
	}
	
    rangeMap( cvImage, scaleMin,scaleMax, min,max);
    flagImageChanged();
}



// Image Transformation Operations

//--------------------------------------------------------------------------------
void ofxCvFloatImage::resize( int w, int h ) {
	if( !bAllocated ){
		ofLog(OF_LOG_ERROR, "in resize, image is not allocated");		
		return;	
	}
	
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
	if( !bAllocated ){
		ofLog(OF_LOG_ERROR, "in scaleIntoMe, image needs to be allocated");	
		return;	
	}
	
	if( !mom.bAllocated ){
		ofLog(OF_LOG_ERROR, "in scaleIntoMe, mom needs to be allocated");	
		return;	
	}
	
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
