
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
        ofLogNotice("ofxCvShortImage") << "copy constructor: source image not allocated";
    }
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::init() {
    ipldepth = IPL_DEPTH_16U;
    iplchannels = 1;
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
void ofxCvShortImage::allocateTexture(){
	tex.allocate(shortPixels);
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::allocatePixels(int w, int h){
	pixels.allocate(w,h,OF_PIXELS_GRAY);
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
	if( !bAllocated ){
		ofLogError("ofxCvShortImage") << "set(): image not allocated";	
		return;	
	}
	cvSet(cvImage, cvScalar(value));
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::setFromPixels( const unsigned char* _pixels, int w, int h ) {
    // This sets the internal image ignoring any ROI
	if( w == 0 || h == 0 ){
		ofLogError("ofxCvShortImage") << "setFromPixels(): width and height are zero";
		return;
	}
    if( !bAllocated || w != width || h != height ) {
		if ( !bAllocated ){
			ofLogNotice("ofxCvShortImage") << "setFromPixels(): allocating to match dimensions: "
				<< width << " " << height;
		}else{
			ofLogNotice("ofxCvShortImage") << "setFromPixels(): reallocating to match dimensions: "
				<< width << " " << height;
		}
		allocate(w,h);
	}
	
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
        ofLogError("ofxCvShortImage") << "setFromPixels(): image size mismatch";
    }

}

//--------------------------------------------------------------------------------
void ofxCvShortImage::setRoiFromPixels( const unsigned char* _pixels, int w, int h ) {
	if( w == 0 || h == 0 ){
		ofLogError("ofxCvShortImage") << "setFromPixels(): width and height are zero";
		return;
	}
   	if(!bAllocated){
		ofLogError("ofxCvShortImage") << "setRoiFromPixels(): image not allocated";
		return;
	} 
	
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
        ofLogError("ofxCvShortImage") << "setRoiFromPixels(): region of interest mismatch";
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
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLogError("ofxCvShortImage") << "operator=: source width and/or height are zero:"
			<< mom.getWidth() << " " << mom.getHeight();	
		return;	
	}
	if( !bAllocated ){
		ofLogNotice("ofxCvShortImage") << "operator=: allocating to match dimensions: "
			<< mom.getWidth() << " " << mom.getHeight();		
		allocate(mom.getWidth(), mom.getHeight());
	}
	
	if( matchingROI(getROI(), mom.getROI()) ) {
        convertGrayToShort(mom.getCvImage(), cvImage);
        flagImageChanged();
	} else {
        ofLogError("ofxCvShortImage") << "operator=: region of interest mismatch";
	}
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const ofxCvColorImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvColorImage& mom = const_cast<ofxCvColorImage&>(_mom);
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLogError("ofxCvShortImage") << "operator=: source width and/or height are zero:"
			<< mom.getWidth() << " " << mom.getHeight();	
		return;	
	}
	if( !bAllocated ){
		ofLogNotice("ofxCvShortImage") << "operator=: allocating to match dimensions: "
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
        convertGrayToShort(cvGrayscaleImage, cvImage);
        setImageROI(cvGrayscaleImage, roi);
        flagImageChanged();
	} else {
        ofLogError("ofxCvShortImage") << "operator=: region of interest mismatch";
	}
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const ofxCvFloatImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvFloatImage& mom = const_cast<ofxCvFloatImage&>(_mom);
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLogError("ofxCvShortImage") << "operator=: source width and/or height are zero:"
			<< mom.getWidth() << " " << mom.getHeight();
		return;	
	}
	if( !bAllocated ){
		ofLogNotice("ofxCvShortImage") << "operator=: allocating to match dimensions: "
			<< mom.getWidth() << " " << mom.getHeight();
		allocate(mom.getWidth(), mom.getHeight());
	}
	
	if( matchingROI(getROI(), mom.getROI()) ) {
        rangeMap( mom.getCvImage(), cvImage,
                  mom.getNativeScaleMin(), mom.getNativeScaleMax(), 0, 65535.0f );
        flagImageChanged();
    } else {
        ofLogError("ofxCvShortImage") << "operator=: region of interest mismatch";
    }
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const ofxCvShortImage& _mom ) {
    if(this != &_mom) {  //check for self-assignment
        // cast non-const,  no worries, we will reverse any changes
        ofxCvShortImage& mom = const_cast<ofxCvShortImage&>(_mom);
		if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
			ofLogError("ofxCvShortImage") << "operator=: source width and/or height are zero:"
				<< mom.getWidth() << " " << mom.getHeight();	
			return;	
		}
		if( !bAllocated ){
			ofLogNotice("ofxCvShortImage") << "operator=: allocating to match dimensions: "
				<< mom.getWidth() << " " << mom.getHeight();
			allocate(mom.getWidth(), mom.getHeight());
		}
	
        if( matchingROI(getROI(), mom.getROI()) ) {
            cvCopy( mom.getCvImage(), cvImage, 0 );
            flagImageChanged();
        } else {
            ofLogError("ofxCvShortImage") << "operator=: region of interest mismatch";
        }
    } else {
        ofLogWarning("ofxCvShortImage") << "operator=: assigning image to itself, not copying";
    }
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const IplImage* _mom ) {
    ofxCvImage::operator = (_mom);
}


//--------------------------------------------------------------------------------
void ofxCvShortImage::addWeighted( ofxCvGrayscaleImage& mom, float f ) {
	if( !bAllocated ){
		ofLogError("ofxCvShortImage") << "addWeighted(): image not allocated";
		return;	
	}
	
	if( matchingROI(getROI(), mom.getROI()) ) {
        convertGrayToShort(mom.getCvImage(), cvImageTemp);
        cvAddWeighted( cvImageTemp, f, cvImage, 1.0f-f,0, cvImage );
        flagImageChanged();
    } else {
        ofLogError("ofxCvShortImage") << "addWeighted(): region of interest mismatch";
    }
}



// Get Pixel Data

//--------------------------------------------------------------------------------
IplImage*  ofxCvShortImage::getCv8BitsImage() {
	if( !bAllocated ){
		ofLogWarning("ofxCvShortImage") << "getCv8BitsImage(): image not allocated";
	}
	
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
	if( !bAllocated ){
		ofLogWarning("ofxCvShortImage") << "getCv8BitsRoiImage(): image not allocated";
	}
	
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
	if( !bAllocated ){
		ofLogError("ofxCvShortImage") << "contrastStretch(): image not allocated";
		return;	
	}
	
	double minVal, maxVal;
	cvMinMaxLoc( cvImage, &minVal, &maxVal, NULL, NULL, 0 );
    rangeMap( cvImage, minVal,maxVal, 0,65535 );
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::convertToRange(float min, float max ){
	if( !bAllocated ){
		ofLogError("ofxCvShortImage") << "convertToRange(): image not allocated";
		return;	
	}
	
    rangeMap( cvImage, 0,65535, min,max);
    flagImageChanged();
}



// Image Transformation Operations

//--------------------------------------------------------------------------------
void ofxCvShortImage::resize( int w, int h ) {
	if( !bAllocated ){
		ofLogError("ofxCvShortImage") << "resize(): image not allocated";		
		return;	
	}

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
	if( !bAllocated ){
		ofLogError("ofxCvShortImage") << "scaleIntoMe(): image not allocated";
		return;	
	}
	
	if( !mom.bAllocated ){
		ofLogError("ofxCvShortImage") << "scaleIntoMe(): source image not allocated";
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
            ofLogWarning("ofxCvShortImage") << "scaleIntoMe(): setting interpolationMethod to CV_INTER_NN";
    		interpolationMethod = CV_INTER_NN;
    	}
        cvResize( mom.getCvImage(), cvImage, interpolationMethod );
        flagImageChanged();

    } else {
        ofLogError("ofxCvShortImage") << "scaleIntoMe(): type mismatch with source image";
    }
}

ofShortPixels & ofxCvShortImage::getShortPixelsRef(){
	if( !bAllocated ){
		ofLogWarning("ofxCvShortImage") << "getShortPixelsRef(): image not allocated";		
	} else if(bShortPixelsDirty) {

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
	if( !bAllocated ){
		ofLogWarning("ofxCvShortImage") << "getRoiShortPixelsRef(): image not allocated";	
	} else if(bShortPixelsDirty) {
		ofRectangle roi = getROI();
		unsigned short * roi_ptr = (unsigned short*)cvImage->imageData + ((int)(roi.y)*cvImage->widthStep/(cvImage->depth/8) + (int)roi.x * cvImage->nChannels);
		shortPixels.setFromAlignedPixels(roi_ptr,roi.width,roi.height,cvImage->nChannels,cvImage->widthStep);
		bShortPixelsDirty = false;
	}
	return shortPixels;

}
