#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include "ofxCvFloatImage.h"
#include "ofxCvShortImage.h"


//--------------------------------------------------------------------------------
ofxCvColorImage::ofxCvColorImage() {
    init();
}

//--------------------------------------------------------------------------------
ofxCvColorImage::ofxCvColorImage( const ofxCvColorImage& _mom ) {
    init();
    if( _mom.bAllocated ) {
        // cast non-const,  to get read access to the mon::cvImage
        ofxCvColorImage& mom = const_cast<ofxCvColorImage&>(_mom);
        allocate( (int)mom.getWidth(), (int)mom.getHeight() );
        cvCopy( mom.getCvImage(), cvImage, 0 );
    } else {
        ofLogWarning("ofxCvColorImage") << "copy constructor: source image not allocated";
    }
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::init() {
    ipldepth = IPL_DEPTH_8U;
    iplchannels = 3;
    gldepth = GL_UNSIGNED_BYTE;
    glchannels = GL_RGB;
    cvGrayscaleImage = NULL;
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::clear() {
    if (bAllocated == true && cvGrayscaleImage != NULL){
        cvReleaseImage( &cvGrayscaleImage );
    }
    ofxCvImage::clear();    //call clear in base class
}




// Set Pixel Data

//--------------------------------------------------------------------------------
void ofxCvColorImage::set( float value ){
	if( !bAllocated ){
        ofLogError("ofxCvColorImage") << "set(): image not allocated";
		return;
    }
    cvSet(cvImage, cvScalar(value, value, value));
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::set(int valueR, int valueG, int valueB){
	if( !bAllocated ){
        ofLogError("ofxCvColorImage") << "set(): image not allocated";
		return;		
    }
    cvSet(cvImage, cvScalar(valueR, valueG, valueB));
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator -= ( float value ) {
	if( !bAllocated ){
        ofLogError("ofxCvColorImage") << "set(): image not allocated";
		return;		
    }
	cvSubS( cvImage, cvScalar(value, value, value), cvImageTemp );
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator += ( float value ) {
	if( !bAllocated ){
        ofLogError("ofxCvColorImage") << "set(): image not allocated";
		return;		
    }
	cvAddS( cvImage, cvScalar(value, value, value), cvImageTemp );
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::setFromPixels( const unsigned char* _pixels, int w, int h ) {
    // copy pixels ignoring any ROI
	if( w == 0 || h == 0 ){
		ofLogError("ofxCvColorImage") << "setFromPixels(): width and height are zero";
		return;
	}

    if( !bAllocated || w != width || h != height ) {
		if ( !bAllocated ){
			ofLogNotice("ofxCvColorImage") << "setFromPixels(): allocating to match dimensions: "
				<< width << " " << height;
		}else{
			ofLogNotice("ofxCvColorImage") << "setFromPixels(): reallocating to match dimensions: "
				<< width << " " << height;
		}
		allocate(w,h);
	}
	
	if( cvImage->width*cvImage->nChannels == cvImage->widthStep ){
		memcpy( cvImage->imageData,  _pixels, w*h*3);
	}else{		
		for( int i=0; i < height; i++ ) {
			memcpy( cvImage->imageData + (i*cvImage->widthStep), _pixels + (i*w*3),width*3 );
		}
	}
	
	flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::setRoiFromPixels( const unsigned char* _pixels, int w, int h ) {
   	if( w == 0 || h == 0 ){
		ofLogError("ofxCvColorImage") << "setRoiFromPixels(): width and height are zero";
		return;
	}
   	if(!bAllocated){
		ofLogError("ofxCvColorImage") << "setRoiFromPixels(): image not allocated";
		return;
	}
		 
	ofRectangle roi = getROI();
    ofRectangle inputROI = ofRectangle( roi.x, roi.y, w, h);
    ofRectangle iRoi = getIntersectionROI( roi, inputROI );

    if( iRoi.width > 0 && iRoi.height > 0 ) {
        // copy pixels from _pixels, however many we have or will fit in cvImage
        for( int i=0; i < iRoi.height; i++ ) {
            memcpy( cvImage->imageData + ((i+(int)iRoi.y)*cvImage->widthStep) + (int)iRoi.x*3,
                    _pixels + (i*w*3),
                    (int)(iRoi.width*3) );
        }
        flagImageChanged();
    } else {
        ofLogError("ofxCvColorImage") << "setRoiFromPixels(): region of interest width and/or height are zero: "
			<< iRoi.width << " " << iRoi.height;
    }
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::setFromGrayscalePlanarImages( ofxCvGrayscaleImage& red, ofxCvGrayscaleImage& green, ofxCvGrayscaleImage& blue){
	ofRectangle roi = getROI();
    ofRectangle redRoi = red.getROI();
    ofRectangle greenRoi = green.getROI();
    ofRectangle blueRoi = blue.getROI();
	
	if( !bAllocated ){
		ofLogNotice("ofxCvColorImage") << "setFromGrayscalePlanarImages(): allocating to match dimensions";		
		allocate(red.getWidth(), red.getHeight());
	}
	
    if( redRoi.width == roi.width && redRoi.height == roi.height &&
        greenRoi.width == roi.width && greenRoi.height == roi.height &&
        blueRoi.width == roi.width && blueRoi.height == roi.height )
    {
         cvCvtPlaneToPix(red.getCvImage(), green.getCvImage(), blue.getCvImage(),NULL, cvImage);
         flagImageChanged();
	} else {
        ofLogError("ofxCvColorImage") << "setFromGrayscalePlanarImages(): image size or region of interest mismatch";
	}
}


//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( unsigned char* _pixels ) {
    setFromPixels( _pixels, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( const ofxCvGrayscaleImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvGrayscaleImage& mom = const_cast<ofxCvGrayscaleImage&>(_mom);
	
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLogError("ofxCvColorImage") << "operator=: source width and/or height are zero: "
			<< mom.getWidth() << " " << mom.getHeight();
		return;	
	}
	if( !bAllocated ){
		ofLogNotice("ofxCvColorImage") << "operator=: allocating to match dimensions: "
			<< mom.getWidth() << " " << mom.getHeight();
		allocate(mom.getWidth(), mom.getHeight());
	}
		
	if( matchingROI(getROI(), mom.getROI()) ) {
		cvCvtColor( mom.getCvImage(), cvImage, CV_GRAY2RGB );
        flagImageChanged();
	} else {
        ofLogError("ofxCvColorImage") << "operator=: region of interest mismatch";
	}
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( const ofxCvColorImage& _mom ) {
    if(this != &_mom) {  //check for self-assignment
        // cast non-const,  no worries, we will reverse any changes
        ofxCvColorImage& mom = const_cast<ofxCvColorImage&>(_mom);
        
		if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
			ofLogError("ofxCvColorImage") << "operator=: source width and/or height are zero:"
				<< mom.getWidth() << " " << mom.getHeight();
			return;	
		}
		if( !bAllocated ){
			ofLogNotice("ofxCvColorImage") << "operator=: allocating to match dimensions: "
				<< mom.getWidth() << " " << mom.getHeight();		
			allocate(mom.getWidth(), mom.getHeight());
		}		
					
		if( matchingROI(getROI(), mom.getROI()) ) {
            cvCopy( mom.getCvImage(), cvImage, 0 );
            flagImageChanged();
        } else {
            ofLogError("ofxCvColorImage") << "operator=: region of interest mismatch";
        }
    } else {
        ofLogWarning("ofxCvColorImage") << "operator=: assigning image to itself, not copying";
    }
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( const ofxCvFloatImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvFloatImage& mom = const_cast<ofxCvFloatImage&>(_mom);
	
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLogError("ofxCvColorImage") << "operator=: source width and/or height are zero:"
			<< mom.getWidth() << " " << mom.getHeight();	
		return;	
	}
	if( !bAllocated ){
		ofLogNotice("ofxCvColorImage") << "operator=: allocating to match dimensions: "
			<< mom.getWidth() << " " << mom.getHeight();	
		allocate(mom.getWidth(), mom.getHeight());
	}	
	
	if( matchingROI(getROI(), mom.getROI()) ) {
        if( cvGrayscaleImage == NULL ) {
            cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
        }
        ofRectangle roi = getROI();
        setImageROI(cvGrayscaleImage, roi);
        rangeMap( mom.getCvImage(), cvGrayscaleImage,
                  mom.getNativeScaleMin(), mom.getNativeScaleMax(), 0, 255.0f );
		cvCvtColor( cvGrayscaleImage, cvImage, CV_GRAY2RGB );
        flagImageChanged();
	} else {
        ofLogError("ofxCvColorImage") << "operator=: region of interest mismatch";
	}
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( const ofxCvShortImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvShortImage& mom = const_cast<ofxCvShortImage&>(_mom);
	
	if( mom.getWidth() == 0 || mom.getHeight() == 0 ){
		ofLogError("ofxCvColorImage") << "operator=: source width and/or height are zero:"
			<< mom.getWidth() << " " << mom.getHeight();
		return;	
	}
	if( !bAllocated ){
		ofLogNotice("ofxCvColorImage") << "operator=: allocating to match dimensions: "
			<< mom.getWidth() << " " << mom.getHeight();		
		allocate(mom.getWidth(), mom.getHeight());
	}	
	
    if( matchingROI(getROI(), mom.getROI()) ) {
        if( cvGrayscaleImage == NULL ) {
            cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
        }
        ofRectangle roi = getROI();
        setImageROI(cvGrayscaleImage, roi);
        rangeMap( mom.getCvImage(), cvGrayscaleImage, 0, 65535.0f, 0, 255.0f );
		cvCvtColor( cvGrayscaleImage, cvImage, CV_GRAY2RGB );
        flagImageChanged();
    } else {
        ofLogError("ofxCvColorImage") << "operator=: region of interest mismatch";
    }
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( const IplImage* _mom ) {
    ofxCvImage::operator = (_mom);
}


// Get Pixel Data


//--------------------------------------------------------------------------------
void ofxCvColorImage::convertToGrayscalePlanarImages(ofxCvGrayscaleImage& red, ofxCvGrayscaleImage& green, ofxCvGrayscaleImage& blue){
	if( !bAllocated ){
		ofLogError("ofxCvColorImage") << "convertToGrayscalePlanarImages(): image not allocated";	
		return;	
	}
	
    ofRectangle roi = getROI();
    ofRectangle redRoi = red.getROI();
    ofRectangle greenRoi = green.getROI();
    ofRectangle blueRoi = blue.getROI();
	
	if( !red.bAllocated ){
		red.allocate(width, height);
	} 
	if( !green.bAllocated ){
		green.allocate(width, height);
	} 
	if( !blue.bAllocated ){
		blue.allocate(width, height);
	} 
			
	if( redRoi.width == roi.width && redRoi.height == roi.height &&
        greenRoi.width == roi.width && greenRoi.height == roi.height &&
        blueRoi.width == roi.width && blueRoi.height == roi.height )
    {
        cvCvtPixToPlane(cvImage, red.getCvImage(), green.getCvImage(), blue.getCvImage(), NULL);
        red.flagImageChanged();
        green.flagImageChanged();
        blue.flagImageChanged();
	} else {
        ofLogError("ofxCvColorImage") << "convertToGrayscalePlanarImages(): image size or region of interest mismatch";
	}
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertToGrayscalePlanarImage (ofxCvGrayscaleImage& grayImage, int whichPlane){
	if( !bAllocated ){
		ofLogError("ofxCvColorImage") << "convertToGrayscalePlanarImage(): image not allocated";	
		return;	
	}
	
	if( !grayImage.bAllocated ){
		grayImage.allocate(width, height);
	} 
		
	ofRectangle roi = getROI();
    ofRectangle grayRoi = grayImage.getROI();
   
	if( grayRoi.width == roi.width && grayRoi.height == roi.height ){

		switch (whichPlane){
				
			case 0:
				cvCvtPixToPlane(cvImage, grayImage.getCvImage(), NULL, NULL, NULL);
				grayImage.flagImageChanged();
				break;
			case 1:
				cvCvtPixToPlane(cvImage, NULL, grayImage.getCvImage(), NULL, NULL);
				grayImage.flagImageChanged();
				break;
			case 2:
				cvCvtPixToPlane(cvImage, NULL, NULL, grayImage.getCvImage(), NULL);
				grayImage.flagImageChanged();
				break;
		}
			
	} else {
		ofLogError("ofxCvColorImage") << "convertToGrayscalePlanarImages(): image size or region of interest mismatch";
	}
}




// Draw Image



// Image Filter Operations

//--------------------------------------------------------------------------------
void ofxCvColorImage::contrastStretch() {
	ofLogWarning("ofxCvColorImage") << "contrastStretch(): not implemented";
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertToRange(float min, float max ){
	if( !bAllocated ){
		ofLogError("ofxCvColorImage") << "convertToRange(): image not allocated";	
		return;	
	}
    rangeMap( cvImage, 0,255, min,max);
    flagImageChanged();
}



// Image Transformation Operations

//--------------------------------------------------------------------------------
void ofxCvColorImage::resize( int w, int h ) {
	if( !bAllocated ){
		ofLogError("ofxCvColorImage") << "resize(): image not allocated";	
		return;	
	}
    // note, one image copy operation could be ommitted by
    // reusing the temporal image storage

    IplImage* temp = cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 3 );
    cvResize( cvImage, temp );
    clear();
    allocate( w, h );
    cvCopy( temp, cvImage );
    cvReleaseImage( &temp );
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::scaleIntoMe( ofxCvImage& mom, int interpolationMethod ){
	if( !bAllocated ){
		ofLogError("ofxCvColorImage") << "resize(): image not allocated";	
		return;	
	}
	if( !mom.bAllocated ){
		ofLogError("ofxCvColorImage") << "resize(): source image not allocated";	
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
            ofLogWarning("ofxCvColorImage") << "scaleIntoMe(): setting interpolationMethod to CV_INTER_NN";
    		interpolationMethod = CV_INTER_NN;
    	}
        cvResize( mom.getCvImage(), cvImage, interpolationMethod );
        flagImageChanged();

    } else {
        ofLogError("ofxCvColorImage") << "scaleIntoMe(): type mismatch with source image";
    }
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertRgbToHsv(){
	if( !bAllocated ){
		ofLogError("ofxCvColorImage") << "convertRgbToHsv(): image not allocated";	
		return;	
	}
    cvCvtColor( cvImage, cvImageTemp, CV_RGB2HSV);
    swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertHsvToRgb(){
	if( !bAllocated ){
		ofLogError("ofxCvColorImage") << "convertHsvToRgb(): image not allocated";
		return;	
	}
    cvCvtColor( cvImage, cvImageTemp, CV_HSV2RGB);
    swapTemp();
    flagImageChanged();
}

