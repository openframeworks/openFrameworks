
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
        allocate(mom.getWidth(), mom.getHeight());    
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
void ofxCvShortImage::setFromPixels( unsigned char* _pixels, int w, int h ) {
    // This sets the internal image ignoring any ROI
        
    if( w == cvImage->width &&  h == cvImage->height ) {
        pushROI();
        if(cvGrayscaleImage == NULL) {
            cvGrayscaleImage = cvCreateImage( cvSize(cvImage->width,cvImage->height), IPL_DEPTH_8U, 1 );
        }        
        cvResetImageROI(cvGrayscaleImage);
        // copy _pixels into cvGrayscaleImage
        for( int i=0; i < cvImage->height; i++ ) {
            memcpy( cvGrayscaleImage->imageData + (i*cvGrayscaleImage->widthStep),
                    _pixels + (i*w), 
                    cvImage->width );
        }
        convertGrayToShort(cvGrayscaleImage, cvImage);
        popROI();
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in setFromPixels, size mismatch");
    }
    
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::setRoiFromPixels( unsigned char* _pixels, int w, int h ) {    
    ofRectangle roi = getROI();
    ofRectangle inputROI = ofRectangle( roi.x, roi.y, w, h );
    ofRectangle iRoi = getIntersectionROI( roi, inputROI );
    
    if( iRoi.width > 0 && iRoi.height > 0 ) {
        if(cvGrayscaleImage == NULL) {
            cvGrayscaleImage = cvCreateImage( cvSize(cvImage->width,cvImage->height), IPL_DEPTH_8U, 1 );
        }
        cvSetImageROI(cvGrayscaleImage, cvRect(roi.x,roi.y,roi.width,roi.height));  //make sure ROI is in sync
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
    setFromPixels( _pixels, cvImage->width, cvImage->height );
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const ofxCvGrayscaleImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvGrayscaleImage& mom = const_cast<ofxCvGrayscaleImage&>(_mom); 
	if( matchROI(*this,mom) ) {
        convertGrayToShort(mom.getCvImage(), cvImage);
        unmatchROI(*this,mom);           
        flagImageChanged();
	} else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
	}
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const ofxCvColorImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvColorImage& mom = const_cast<ofxCvColorImage&>(_mom); 
	if( matchROI(*this,mom) ) {
        if( cvGrayscaleImage == NULL ) {
            cvGrayscaleImage = cvCreateImage( cvSize(cvImage->width,cvImage->height), IPL_DEPTH_8U, 1 );
        }
        ofRectangle roi = getROI();
        cvSetImageROI(cvGrayscaleImage, cvRect(roi.x,roi.y,roi.width,roi.height));
		cvCvtColor( mom.getCvImage(), cvGrayscaleImage, CV_RGB2GRAY );
        convertGrayToShort(cvGrayscaleImage, cvImage);
        unmatchROI(*this,mom);
        cvSetImageROI(cvGrayscaleImage, cvRect(roi.x,roi.y,roi.width,roi.height));                      
        flagImageChanged();
	} else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
	}
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const ofxCvFloatImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvFloatImage& mom = const_cast<ofxCvFloatImage&>(_mom); 
    if( matchROI(*this,mom) ) {
        rangeMap( mom.getCvImage(), cvImage, 
                  mom.getNativeScaleMin(), mom.getNativeScaleMax(), 0, 65535.0f );        
        unmatchROI(*this,mom);       
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
        if( matchROI(*this,mom) ) {
            cvCopy( mom.getCvImage(), cvImage, 0 ); 
            unmatchROI(*this,mom);       
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
	if( matchROI(*this,mom) ) {
        convertGrayToShort(mom.getCvImage(), cvImageTemp);
        cvAddWeighted( cvImageTemp, f, cvImage, 1.0f-f,0, cvImage );
        unmatchROI(*this,mom);        
        flagImageChanged();
    } else {
        ofLog(OF_LOG_ERROR, "in addWeighted, ROI mismatch");
    }
}



// Get Pixel Data

//--------------------------------------------------------------------------------
unsigned char*  ofxCvShortImage::getPixels(){
    // get pixels ignoring any ROI
    
    if(bPixelsDirty) {
        if( cvGrayscaleImage == NULL ) {
            cvGrayscaleImage = cvCreateImage( cvSize(cvImage->width,cvImage->height), IPL_DEPTH_8U, 1 );
        }
         
        pushROI();
        cvResetImageROI(cvGrayscaleImage);
        convertShortToGray(cvImage, cvGrayscaleImage);
        popROI();   
    
        if(pixels == NULL) {
            // we need pixels, allocate it
            pixels = new unsigned char[cvImage->width*cvImage->height];
            pixelsWidth = cvImage->width;
            pixelsHeight = cvImage->height;            
        } else if(pixelsWidth != cvImage->width || pixelsHeight != cvImage->height) {
            // ROI changed, reallocate pixels for new size
            // this is needed because getRoiPixels() might change size of pixels
            delete pixels;
            pixels = new unsigned char[cvImage->width*cvImage->height];
            pixelsWidth = cvImage->width;
            pixelsHeight = cvImage->height;
        }
        
        // copy from ROI to pixels
        for( int i = 0; i < cvImage->height; i++ ) {
            memcpy( pixels + (i*cvImage->width),
                    cvGrayscaleImage->imageData + (i*cvGrayscaleImage->widthStep),
                    cvImage->width );
        }
        bPixelsDirty = false;
    }
	return pixels;    
    
}

//--------------------------------------------------------------------------------
unsigned char*  ofxCvShortImage::getRoiPixels(){    
    if(bPixelsDirty) {
        if( cvGrayscaleImage == NULL ) {
            cvGrayscaleImage = cvCreateImage( cvSize(cvImage->width,cvImage->height), IPL_DEPTH_8U, 1 );
        }
        
        ofRectangle roi = getROI();
        cvSetImageROI(cvGrayscaleImage, cvRect(roi.x,roi.y,roi.width,roi.height));  //make sure ROI is in sync
        convertShortToGray(cvImage, cvGrayscaleImage);    
    
        if(pixels == NULL) {
            // we need pixels, allocate it
            pixels = new unsigned char[(int)(roi.width*roi.height)];
            pixelsWidth = roi.width;
            pixelsHeight = roi.height;            
        } else if(pixelsWidth != roi.width || pixelsHeight != roi.height) {
            // ROI changed, reallocate pixels for new size
            delete pixels;
            pixels = new unsigned char[(int)(roi.width*roi.height)];
            pixelsWidth = roi.width;
            pixelsHeight = roi.height;
        }
        
        // copy from ROI to pixels
        for( int i = 0; i < roi.height; i++ ) {
            memcpy( pixels + (int)(i*roi.width),
                    cvGrayscaleImage->imageData + ((int)(i+roi.y)*cvGrayscaleImage->widthStep) + (int)roi.x,
                    roi.width );
        }
        bPixelsDirty = false;
    }
	return pixels;
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
