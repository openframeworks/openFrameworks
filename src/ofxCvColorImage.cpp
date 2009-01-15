

#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include "ofxCvFloatImage.h"




//--------------------------------------------------------------------------------
ofxCvColorImage::ofxCvColorImage( const ofxCvColorImage& mom ) {
    allocate(mom.width, mom.height);    
    cvCopy( mom.getCvImage(), cvImage, 0 );
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::allocate( int w, int h ) {
	if (bAllocated == true){
		cout << "warning: reallocating cvImage in ofxCvColorImage" << endl;
		clear();
	}
	cvImage = cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 3 );
	cvImageTemp	= cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 3 );
    cvGrayscaleImage = NULL;  //only allocated when needed
	pixels = new unsigned char[w*h*3];
	width = w;
	height = h;
	bAllocated = true;
    if( bUseTexture ) {
        tex.allocate( width, height, GL_RGB );
    }
}

//--------------------------------------------------------------------------------
ofxCvColorImage::~ofxCvColorImage() {
    if (bAllocated == true && cvGrayscaleImage != NULL){
        cvReleaseImage( &cvGrayscaleImage );
    }
}



// Set Pixel Data

//--------------------------------------------------------------------------------
void ofxCvColorImage::set( float value ){
    cvSet(cvImage, cvScalar(value, value, value));
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::set(int valueR, int valueG, int valueB){
    cvSet(cvImage, cvScalar(valueR, valueG, valueB));
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator -= ( float value ) {
	cvSubS( cvImage, cvScalar(value, value, value), cvImageTemp );
	swapTemp();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator += ( float value ) {
	cvAddS( cvImage, cvScalar(value, value, value), cvImageTemp );
	swapTemp();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::setFromPixels( unsigned char* _pixels, int w, int h ) {
	for( int i = 0; i < h; i++ ) {
		memcpy( cvImage->imageData+(i*cvImage->widthStep), _pixels+(i*width*3), width*3 );
	}
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::setFromGrayscalePlanarImages( const ofxCvGrayscaleImage& red, const ofxCvGrayscaleImage& green, const ofxCvGrayscaleImage& blue){
     cvCvtPlaneToPix(red.getCvImage(), green.getCvImage(), blue.getCvImage(),NULL, cvImage);
}


//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( unsigned char* _pixels ) {
    setFromPixels( _pixels, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( const ofxCvGrayscaleImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvCvtColor( mom.getCvImage(), cvImage, CV_GRAY2RGB );
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator = ( const ofxCvColorImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvCopy( mom.getCvImage(), cvImage, 0 );
	} else {
        cout << "error in =, images are different sizes" << endl;
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
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}



// Get Pixel Data

//--------------------------------------------------------------------------------
unsigned char* ofxCvColorImage::getPixels() {
	// copy each line of pixels:
	for( int i=0; i<height; i++ ) {
		memcpy( pixels+(i*width*3),
                cvImage->imageData+(i*cvImage->widthStep), width*3 );
	}
	return pixels;
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertToGrayscalePlanarImages(ofxCvGrayscaleImage& red, ofxCvGrayscaleImage& green, ofxCvGrayscaleImage& blue){
    cvCvtPixToPlane(cvImage, red.getCvImage(), green.getCvImage(), blue.getCvImage(), NULL);
}



// Draw Image

//--------------------------------------------------------------------------------
void ofxCvColorImage::draw( float x, float y ) {

	// note, this is a bit ineficient, as we have to
	// copy the data out of the cvImage into the pixel array
	// and then upload to texture.  We should add
	// to the texture class an override for pixelstorei
	// that allows stepped-width image upload:

    if( bUseTexture ) {
        tex.loadData( getPixels(), width, height, GL_RGB );
        tex.draw( x, y, width, height );

    } else {
        IplImage* o;
        o = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 3 );
        cvResize( cvImage, o, CV_INTER_NN );
        cvFlip( o, o, 0 );
        glRasterPos3f( x, y+height, 0.0 );
        glDrawPixels( o->width, o->height ,
                     GL_BGR, GL_UNSIGNED_BYTE, o->imageData );
        cvReleaseImage( &o );
        glRasterPos3f( -x, -(y+height), 0.0 );
    }
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::draw( float x, float y, float w, float h ) {
    if( bUseTexture ) {
        tex.loadData( getPixels(), width, height, GL_RGB );
        tex.draw( x, y, w, h );

    } else {
        IplImage* o;
        o = cvCreateImage( cvSize((int)w, (int)h), IPL_DEPTH_8U, 3 );
        cvResize( cvImage, o, CV_INTER_NN );
        cvFlip( o, o, 0 );
        glRasterPos3f( x, y+h, 0.0 );
        glDrawPixels( o->width, o->height ,
                     GL_BGR, GL_UNSIGNED_BYTE, o->imageData );
        cvReleaseImage( &o );
        glRasterPos3f( -x, -(y+h), 0.0 );
    }
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
            cout << "error in scaleIntoMe / interpolationMethod, setting to CV_INTER_NN" << endl;
    		interpolationMethod = CV_INTER_NN;
    	}
        cvResize( mom.getCvImage(), cvImage, interpolationMethod );

    } else {
        cout << "error in scaleIntoMe: mom image type has to match" << endl;
    }
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertRgbToHsv(){
    cvCvtColor( cvImage, cvImageTemp, CV_RGB2HSV);
    swapTemp();
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertHsvToRgb(){
    cvCvtColor( cvImage, cvImageTemp, CV_HSV2RGB);
    swapTemp();
}

