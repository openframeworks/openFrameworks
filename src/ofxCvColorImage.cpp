

#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include "ofxCvFloatImage.h"




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
        allocate(mom.width, mom.height);    
        cvCopy( mom.getCvImage(), cvImage, 0 );
    } else {
        ofLog(OF_LOG_NOTICE, "in ofxCvColorImage copy constructor, mom not allocated");
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
    cvSet(cvImage, cvScalar(value, value, value));
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::set(int valueR, int valueG, int valueB){
    cvSet(cvImage, cvScalar(valueR, valueG, valueB));
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator -= ( float value ) {
	cvSubS( cvImage, cvScalar(value, value, value), cvImageTemp );
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator += ( float value ) {
	cvAddS( cvImage, cvScalar(value, value, value), cvImageTemp );
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::setFromPixels( unsigned char* _pixels, int w, int h ) {
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
        ofLog(OF_LOG_ERROR, "in setFromPixels, ROI mismatch");
    }    
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::setFromGrayscalePlanarImages( ofxCvGrayscaleImage& red, ofxCvGrayscaleImage& green, ofxCvGrayscaleImage& blue){     
	if( red.width == width && red.height == height &&
        green.width == width && green.height == height &&
        blue.width == width && blue.height == height )
    {
         cvCvtPlaneToPix(red.getCvImage(), green.getCvImage(), blue.getCvImage(),NULL, cvImage);
         flagImageChanged();
	} else {
        ofLog(OF_LOG_ERROR, "in setFromGrayscalePlanarImages, images are different sizes");
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
	if( pushSetBothToTheirIntersectionROI(*this,mom) ) {
		cvCvtColor( mom.getCvImage(), cvImage, CV_GRAY2RGB );
        popROI();       //restore prevoius ROI
        mom.popROI();   //restore prevoius ROI         
        flagImageChanged();
	} else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
	}
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( const ofxCvColorImage& _mom ) {
    if(this != &_mom) {  //check for self-assignment
        // cast non-const,  no worries, we will reverse any chages
        ofxCvColorImage& mom = const_cast<ofxCvColorImage&>(_mom);    
        if( pushSetBothToTheirIntersectionROI(*this,mom) ) {
            cvCopy( mom.getCvImage(), cvImage, 0 );
            popROI();       //restore prevoius ROI
            mom.popROI();   //restore prevoius ROI              
            flagImageChanged();
        } else {
            ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
        }
    } else {
        ofLog(OF_LOG_WARNING, "in =, you are assigning a ofxCvColorImage to itself");
    }
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( const ofxCvFloatImage& _mom ) {
    // cast non-const,  no worries, we will reverse any chages
    ofxCvFloatImage& mom = const_cast<ofxCvFloatImage&>(_mom); 
	if( pushSetBothToTheirIntersectionROI(*this,mom) ) {
        if( cvGrayscaleImage == NULL ) {
            cvGrayscaleImage = cvCreateImage( cvSize(cvImage->width,cvImage->height), IPL_DEPTH_8U, 1 );
        }
        cvSetImageROI(cvGrayscaleImage, cvRect(roiX,roiY,width,height));
		cvConvertScale( mom.getCvImage(), cvGrayscaleImage, 1, 0 );
		cvCvtColor( cvGrayscaleImage, cvImage, CV_GRAY2RGB );
        popROI();       //restore prevoius ROI
        mom.popROI();   //restore prevoius ROI          
        cvSetImageROI(cvGrayscaleImage, cvRect(roiX,roiY,width,height));
        flagImageChanged();
	} else {
        ofLog(OF_LOG_ERROR, "in =, ROI mismatch");
	}
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( const IplImage* _mom ) {
    ofxCvImage::operator = (_mom);
}


// Get Pixel Data

//--------------------------------------------------------------------------------
unsigned char* ofxCvColorImage::getPixels() {
    if(bPixelsDirty) {
        if(pixels == NULL) {
            // we need pixels, allocate it
            pixels = new unsigned char[width*height*3];
            pixelsWidth = width;
            pixelsHeight = height;            
        } else if(pixelsWidth != width || pixelsHeight != height) {
            // ROI changed, reallocate pixels for new size
            delete pixels;
            pixels = new unsigned char[width*height*3];
            pixelsWidth = width;
            pixelsHeight = height;
        }
        
        // copy from ROI to pixels
        for( int i = 0; i < height; i++ ) {
            memcpy( pixels + (i*width*3),
                    cvImage->imageData + ((i+roiY)*cvImage->widthStep) + roiX*3,
                    width*3 );
        }
        bPixelsDirty = false;
    }
	return pixels;
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertToGrayscalePlanarImages(ofxCvGrayscaleImage& red, ofxCvGrayscaleImage& green, ofxCvGrayscaleImage& blue){
	if( red.width == width && red.height == height &&
        green.width == width && green.height == height &&
        blue.width == width && blue.height == height )
    {
        cvCvtPixToPlane(cvImage, red.getCvImage(), green.getCvImage(), blue.getCvImage(), NULL);
	} else {
        ofLog(OF_LOG_ERROR, "in convertToGrayscalePlanarImages, images are different sizes");
	}     
}



// Draw Image



// Image Filter Operations

//--------------------------------------------------------------------------------
void ofxCvColorImage::contrastStretch() {
	ofLog(OF_LOG_WARNING, "in contrastStratch, not implemented for ofxCvColorImage");
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertToRange(float min, float max ){
    rangeMap( cvImage, 0,255, min,max);
    flagImageChanged();
}



// Image Transformation Operations

//--------------------------------------------------------------------------------
void ofxCvColorImage::resize( int w, int h ) {

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

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertRgbToHsv(){
    cvCvtColor( cvImage, cvImageTemp, CV_RGB2HSV);
    swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertHsvToRgb(){
    cvCvtColor( cvImage, cvImageTemp, CV_HSV2RGB);
    swapTemp();
    flagImageChanged();
}

