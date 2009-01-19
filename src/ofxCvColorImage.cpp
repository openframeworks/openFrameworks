

#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include "ofxCvFloatImage.h"




//--------------------------------------------------------------------------------
ofxCvColorImage::ofxCvColorImage() {
    cvGrayscaleImage = NULL;
}

//--------------------------------------------------------------------------------
ofxCvColorImage::ofxCvColorImage( const ofxCvColorImage& mom ) {
    allocate(mom.width, mom.height);    
    cvCopy( mom.getCvImage(), cvImage, 0 );
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::allocate( int w, int h ) {
	if (bAllocated == true){
		ofLog(OF_WARNING, "in allocate, reallocating a ofxCvColorImage");
		clear();
	}
    
	cvImage = cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 3 );
	cvImageTemp	= cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 3 );

	width = w;
	height = h;
	bAllocated = true;

    if( bUseTexture ) {
        tex.allocate( width, height, GL_RGB );
        bTextureDirty = true;
    }
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
    imageHasChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::set(int valueR, int valueG, int valueB){
    cvSet(cvImage, cvScalar(valueR, valueG, valueB));
    imageHasChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator -= ( float value ) {
	cvSubS( cvImage, cvScalar(value, value, value), cvImageTemp );
	swapTemp();
    imageHasChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator += ( float value ) {
	cvAddS( cvImage, cvScalar(value, value, value), cvImageTemp );
	swapTemp();
    imageHasChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::setFromPixels( unsigned char* _pixels, int w, int h ) {
	for( int i = 0; i < h; i++ ) {
		memcpy( cvImage->imageData+(i*cvImage->widthStep), _pixels+(i*width*3), width*3 );
	}
    imageHasChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::setFromGrayscalePlanarImages( const ofxCvGrayscaleImage& red, const ofxCvGrayscaleImage& green, const ofxCvGrayscaleImage& blue){
     cvCvtPlaneToPix(red.getCvImage(), green.getCvImage(), blue.getCvImage(),NULL, cvImage);
     imageHasChanged();
}


//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( unsigned char* _pixels ) {
    setFromPixels( _pixels, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( const ofxCvGrayscaleImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvCvtColor( mom.getCvImage(), cvImage, CV_GRAY2RGB );
        imageHasChanged();
	} else {
        ofLog(OF_ERROR, "in =, images are different sizes");
	}
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( const ofxCvColorImage& mom ) {
    if(this != &mom) {  //check for self-assignment
        if( mom.width == width && mom.height == height ) {
            cvCopy( mom.getCvImage(), cvImage, 0 );
            imageHasChanged();
        } else {
            ofLog(OF_ERROR, "in =, images are different sizes");
        }
    } else {
        ofLog(OF_WARNING, "in =, you are assigning a ofxCvColorImage to itself");
    }
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( const ofxCvFloatImage& mom ) {
	if( mom.width == width && mom.height == height ) {
        if( cvGrayscaleImage == NULL ) {
            cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
        }
		cvConvertScale( mom.getCvImage(), cvGrayscaleImage, 1, 0 );
		cvCvtColor( cvGrayscaleImage, cvImage, CV_GRAY2RGB );
        imageHasChanged();
	} else {
        ofLog(OF_ERROR, "in =, images are different sizes");
	}
}



// Get Pixel Data

//--------------------------------------------------------------------------------
unsigned char* ofxCvColorImage::getPixels() {
	if(bPixelsDirty) {
        if(pixels == NULL) { pixels = new unsigned char[width*height*3]; };
        for( int i=0; i<height; i++ ) {
            memcpy( pixels+(i*width*3),
                    cvImage->imageData+(i*cvImage->widthStep), width*3 );
        }
        bPixelsDirty = false;
    }
	return pixels;
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertToGrayscalePlanarImages(ofxCvGrayscaleImage& red, ofxCvGrayscaleImage& green, ofxCvGrayscaleImage& blue){
    cvCvtPixToPlane(cvImage, red.getCvImage(), green.getCvImage(), blue.getCvImage(), NULL);
}



// Draw Image

//--------------------------------------------------------------------------------
void ofxCvColorImage::drawWithoutTexture( float x, float y ) {
    drawWithoutTexture( x,y, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::drawWithoutTexture( float x, float y, float w, float h ) {
    // this is slower than the typical draw method based on textures
    // but useful when dealing with threads GL textures often don't work
    
    if( x == 0) {
        x += 0.01;
        ofLog( OF_NOTICE, "BUG: can't draw at x==0 in texture-less mode.");
    }
    
    glRasterPos3f( x, y+h, 0.0 );
    IplImage* tempImg;
    tempImg = cvCreateImage( cvSize((int)w, (int)h), IPL_DEPTH_8U, 3 );
    cvResize( cvImage, tempImg, CV_INTER_NN );
    cvFlip( tempImg, tempImg, 0 );
    glDrawPixels( tempImg->width, tempImg->height ,
                 GL_RGB, GL_UNSIGNED_BYTE, tempImg->imageData );
    cvReleaseImage( &tempImg );
    glRasterPos3f( -x, -(y+h), 0.0 );
}



// Image Filter Operations

//--------------------------------------------------------------------------------
void ofxCvColorImage::contrastStretch() {
	ofLog(OF_WARNING, "in contrastStratch, not implemented for ofxCvColorImage");
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertToRange(float min, float max ){
    rangeMap( cvImage, 0,255, min,max);
    imageHasChanged();
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
void ofxCvColorImage::scaleIntoMe( const ofxCvImage& mom, int interpolationMethod ){
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
            ofLog(OF_WARNING, "in scaleIntoMe, setting interpolationMethod to CV_INTER_NN");
    		interpolationMethod = CV_INTER_NN;
    	}
        cvResize( mom.getCvImage(), cvImage, interpolationMethod );
        imageHasChanged();

    } else {
        ofLog(OF_ERROR, "in scaleIntoMe, mom image type has to match");
    }
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertRgbToHsv(){
    cvCvtColor( cvImage, cvImageTemp, CV_RGB2HSV);
    swapTemp();
    imageHasChanged();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertHsvToRgb(){
    cvCvtColor( cvImage, cvImageTemp, CV_HSV2RGB);
    swapTemp();
    imageHasChanged();
}

