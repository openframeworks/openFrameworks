
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
        ofLogNotice("ofxCvFloatImage") << "copy constructor: source image not allocated";
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
		ofLogError("ofxCvFloatImage") << "set(): image not allocated";
		return;	
	}
	cvSet(cvImage, cvScalar(value));
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator *= ( float scalar ){
	if( !bAllocated ){
		ofLogError("ofxCvFloatImage") << "operator*=: image not allocated";	
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
		ofLogError("ofxCvFloatImage") << "operator/=: image not allocated";	
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
		ofLogError("ofxCvFloatImage") << "setFromPixels(): width and height are zero";
		return;
	}
    if( !bAllocated || w != width || h != height ) {
		if ( !bAllocated ){
			ofLogNotice("ofxCvFloatImage") << "setFromPixels(): allocating to match dimensions: "
				<< width << " " << height;
		}else{
			ofLogNotice("ofxCvFloatImage") << "setFromPixels(): reallocating to match dimensions: "
				<< width << " " << height;
		}
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
        ofLogError("ofxCvFloatImage") << "setFromPixels(): image size mismatch";
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::setFromPixels( float* _pixels, int w, int h ) {
    // This sets the internal image ignoring any ROI
	if( w == 0 || h == 0 ){
		ofLogError("ofxCvFloatImage") << "setFromPixels(): width and height are zero";
		return;
	}
    if( !bAllocated || w != width || h != height ) {
		if ( !bAllocated ){
			ofLogNotice("ofxCvFloatImage") << "setFromPixels(): allocating to match dimensions: "
				<< width << " " << height;
		}else{
			ofLogNotice("ofxCvFloatImage") << "setFromPixels(): reallocating to match dimensions: "
				<< width << " " << height;
		}
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
        ofLogError("ofxCvFloatImage") << "setFromPixels(): image size mismatch";
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::setRoiFromPixels( const unsigned char* _pixels, int w, int h ) {
	if( w == 0 || h == 0 ){
		ofLogError("ofxCvFloatImage") << "setRoiFromPixels(): width and height are zero";
		return;
	}
   	if(!bAllocated){
		ofLogError("ofxCvFloatImage") << "setRoiFromPixels(): image not allocated";
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
        ofLogError("ofxCvFloatImage") << "setRoiFromPixels(): region of interest width and/or height are zero: "
			<< iRoi.width << " " << iRoi.height;
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::setRoiFromPixels( float* _pixels, int w, int h ) {
   	if( w == 0 || h == 0 ){
		ofLogError("ofxCvFloatImage") << "setRoiFromPixels(): width and height are zero";
		return;
	}
   	if(!bAllocated){
		ofLogError("ofxCvFloatImage") << "setRoiFromPixels(): image not allocated";
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
        ofLogError("ofxCvFloatImage") << "setRoiFromPixels(): region of interest width and/or height are zero: "
			<< iRoi.width << " " << iRoi.height;
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
		ofLogError("ofxCvFloatImage") << "operator=: source width and/or height are zero: "
			<< mom.getWidth() << " " << mom.getHeight();
		return;	
	}
	if( !bAllocated ){
		ofLogNotice("ofxCvFloatImage") << "operator=: allocating to match dimensions: "
			<< mom.getWidth() << " " << mom.getHeight();
		allocate(mom.getWidth(), mom.getHeight());
	}	
	
	if( matchingROI(getROI(), mom.getROI()) ) {
        convertGrayToFloat(mom.getCvImage(), cvImage);
        flagImageChanged();
	} else {
        ofLogError("ofxCvFloatImage") << "operator=: region of interest mismatch";
	}
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator = ( const ofxCvColorImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvColorImage& mom = const_cast<ofxCvColorImage&>(_mom);
	
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLogError("ofxCvFloatImage") << "operator=: source width and/or height are zero: "
			<< mom.getWidth() << " " << mom.getHeight();	
		return;	
	}
	if( !bAllocated ){
		ofLogNotice("ofxCvFloatImage") << "operator=: allocating to match dimensions: "
			<< mom.getWidth() << " " << mom.getHeight();
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
        ofLogError("ofxCvFloatImage") << "operator=: region of interest mismatch";
	}
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator = ( const ofxCvFloatImage& _mom ) {
    if(this != &_mom) {  //check for self-assignment
        // cast non-const,  no worries, we will reverse any changes
        ofxCvFloatImage& mom = const_cast<ofxCvFloatImage&>(_mom);

		if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
			ofLogError("ofxCvFloatImage") << "operator=: source width and/or height are zero:"
				<< mom.getWidth() << " " << mom.getHeight();	
			return;	
		}
		if( !bAllocated ){
			ofLogNotice("ofxCvFloatImage") << "operator=: allocating to match dimensions: "
				<< mom.getWidth() << " " << mom.getHeight();
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
            ofLogError("ofxCvFloatImage") << "operator=: region of interest mismatch";
        }
    } else {
        ofLogWarning("ofxCvFloatImage") << "operator=: assigning image to itself, not copying";
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator = ( const ofxCvShortImage& _mom ) {
    // cast non-const,  no worries, we will reverse any changes
    ofxCvShortImage& mom = const_cast<ofxCvShortImage&>(_mom);
	
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLogError("ofxCvFloatImage") << "operator=: source width and/or height are zero:"
			<< mom.getWidth() << " " << mom.getHeight();
		return;	
	}
	if( !bAllocated ){
		ofLogNotice("ofxCvFloatImage") << "operator=: allocating to match dimensions: "
			<< mom.getWidth() << " " << mom.getHeight();
		allocate(mom.getWidth(), mom.getHeight());
	}
		
    if( matchingROI(getROI(), mom.getROI()) ) {
        rangeMap( mom.getCvImage(), cvImage,
                  0, 65535.0f, getNativeScaleMin(), getNativeScaleMax() );
        flagImageChanged();
    } else {
        ofLogError("ofxCvFloatImage") << "operator=: region of interest mismatch";
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator = ( const IplImage* _mom ) {
    ofxCvImage::operator = (_mom);
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator *= ( ofxCvImage& mom ) {
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLogError("ofxCvFloatImage") << "operator*=: source width and/or height are zero:"
			<< mom.getWidth() << " " << mom.getHeight();	
		return;	
	}
	if( !bAllocated ){
		ofLogError("ofxCvFloatImage") << "operator*=: image not allocated";		
		return;	
	}
		
	if( mom.getCvImage()->nChannels == cvImage->nChannels && mom.getCvImage()->depth == cvImage->depth ){
        if( matchingROI(getROI(), mom.getROI()) ) {
            cvMul( cvImage, mom.getCvImage(), cvImageTemp );
            swapTemp();
            flagImageChanged();
        } else {
            ofLogError("ofxCvFloatImage") << "operator*=: region of interest mismatch";
        }
	} else {
        ofLogError("ofxCvFloatImage") << "operator*=: type mismatch";
	}
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator &= ( ofxCvImage& mom ) {
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLogError("ofxCvFloatImage") << "operator &=: source width and/or height are zero:"
			<< mom.getWidth() << " " << mom.getHeight();
		return;	
	}
	if( !bAllocated ){
		ofLogError("ofxCvFloatImage") << "operator&=: image not allocated";
		return;	
	}

	if( mom.getCvImage()->nChannels == cvImage->nChannels && mom.getCvImage()->depth == cvImage->depth ){
        if( matchingROI(getROI(), mom.getROI()) ) {
            //this is doing it bit-wise; probably not what we want
            cvAnd( cvImage, mom.getCvImage(), cvImageTemp );
            swapTemp();
            flagImageChanged();
        } else {
            ofLogError("ofxCvFloatImage") << "operator&=: region of interest mismatch";
        }
	} else {
        ofLogError("ofxCvFloatImage") << "operator&=: images type mismatch";
	}
}


//--------------------------------------------------------------------------------
void ofxCvFloatImage::addWeighted( ofxCvGrayscaleImage& mom, float f ) {
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLogError("ofxCvFloatImage") << "addWeighted(): source width and/or height are zero:"
			<< mom.getWidth() << " " << mom.getHeight();
		return;	
	}
	if( !bAllocated ){
		ofLogError("ofxCvFloatImage") << "addWeighted(): image not allocated";
		return;	
	}
	
	if( matchingROI(getROI(), mom.getROI()) ) {
        convertGrayToFloat(mom.getCvImage(), cvImageTemp);
        cvAddWeighted( cvImageTemp, f, cvImage, 1.0f-f,0, cvImage );
        flagImageChanged();
    } else {
        ofLogError("ofxCvFloatImage") << "addWeighted(): region of interest mismatch";
    }
}



// Get Pixel Data

//--------------------------------------------------------------------------------
IplImage*  ofxCvFloatImage::getCv8BitsImage() {
	if( !bAllocated ){
		ofLogWarning("ofxCvFloatImage") << "getCv8BitsImage(): image not allocated";
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
		ofLogWarning("ofxCvFloatImage") << "getCv8BitsRoiImage(): image not allocated";
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
		ofLogWarning("ofxCvFloatImage") << "getPixelsAsFloats(): image not allocated";
	}
	
	return getFloatPixelsRef().getPixels();
}
//--------------------------------------------------------------------------------
ofFloatPixels &	ofxCvFloatImage::getFloatPixelsRef(){
	if( !bAllocated ){
		ofLogWarning("ofxCvFloatImage") << "getPixelsAsFloats(): image not allocated";
	} else if(bFloatPixelsDirty) {
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
		ofLogWarning("ofxCvFloatImage") << "getRoiPixelsAsFloats(): image not allocated";
	}
	
	return getRoiFloatPixelsRef().getPixels();
}

//--------------------------------------------------------------------------------
ofFloatPixels &	ofxCvFloatImage::getRoiFloatPixelsRef(){
	if( !bAllocated ){
		ofLogWarning("ofxCvFloatImage") << "getRoiFloatPixelsRef(): image not allocated";
	} else if(bFloatPixelsDirty) {
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
		ofLogError("ofxCvFloatImage") << "contrastStretch(): image not allocated";		
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
		ofLogError("ofxCvFloatImage") << "convertToRange(): image not allocated";		
		return;	
	}
	
    rangeMap( cvImage, scaleMin,scaleMax, min,max);
    flagImageChanged();
}



// Image Transformation Operations

//--------------------------------------------------------------------------------
void ofxCvFloatImage::resize( int w, int h ) {
	if( !bAllocated ){
		ofLogError("ofxCvFloatImage") << "resize(): image not allocated";		
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
		ofLogError("ofxCvFloatImage") << "scaleIntoMe(): image not allocated";	
		return;	
	}
	
	if( !mom.bAllocated ){
		ofLogError("ofxCvFloatImage") << "scaleIntoMe(): source image not allocated";
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
            ofLogWarning("ofxCvFloatImage") << "scaleIntoMe(): setting interpolationMethod to CV_INTER_NN";
    		interpolationMethod = CV_INTER_NN;
    	}
        cvResize( mom.getCvImage(), cvImage, interpolationMethod );
        flagImageChanged();

    } else {
        ofLogError("ofxCvFloatImage") << "scaleIntoMe(): type mismatch with source image";
    }
}
