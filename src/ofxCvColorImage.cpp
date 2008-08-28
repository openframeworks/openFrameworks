

#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include "ofxCvFloatImage.h"




//--------------------------------------------------------------------------------
ofxCvColorImage::ofxCvColorImage( const ofxCvColorImage& mom ) {
    cvCopy( mom.getCvImage(), cvImage, 0 );
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::allocate( int w, int h ) {
	
	if (bAllocated == true){
		printf ("warning: reallocating cvImage in ofxCvColorImage\n");
		clear();
	}
	
	cvImage = cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 3 );
	cvImageTemp	= cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 3 );
	pixels = new unsigned char[w*h*3];
	width = w;
	height = h;
	bAllocated = true;
    if( bUseTexture ) {
        tex.allocate( width, height, GL_RGB );
    }
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::set(int value){
    cvSet(cvImage, cvScalar(value, value, value));
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::set(int valueR, int valueG, int valueB){
    cvSet(cvImage, cvScalar(valueR, valueG, valueB));
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

//--------------------------------------------------------------------------------
void ofxCvColorImage::setFromGrayscalePlanarImages(ofxCvGrayscaleImage& red, ofxCvGrayscaleImage& green, ofxCvGrayscaleImage& blue){
     cvCvtPlaneToPix(red.getCvImage(), green.getCvImage(), blue.getCvImage(),NULL, cvImage);
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::convertToGrayscalePlanarImages(ofxCvGrayscaleImage& red, ofxCvGrayscaleImage& green, ofxCvGrayscaleImage& blue){
    cvCvtPixToPlane(cvImage, red.getCvImage(), green.getCvImage(), blue.getCvImage(), NULL);
}


// Set Pixel Data - Arrays
//--------------------------------------------------------------------------------
void ofxCvColorImage::setFromPixels( unsigned char* _pixels, int w, int h ) {
	for( int i = 0; i < h; i++ ) {
		memcpy( cvImage->imageData+(i*cvImage->widthStep), _pixels+(i*width*3), width*3 );
	}
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator =	( unsigned char* _pixels ) {
    setFromPixels( _pixels, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator =	( ofxCvGrayscaleImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvCvtColor( mom.getCvImage(), cvImage, CV_GRAY2RGB );
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator =	( ofxCvColorImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvCopy( mom.getCvImage(), cvImage, 0 );
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator =	( ofxCvFloatImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		//cvCopy(mom.getCvImage(), cvImage, 0);
		//cvConvertScale( mom.getCvImage(), cvImage, 1, 0 );
		cvConvert( mom.getCvImage(), cvImage ); // same as above but optimized
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator -= ( ofxCvColorImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvSub( cvImage, mom.getCvImage(), cvImageTemp );
		swapTemp();
	} else {
        cout << "error in -=, images are different sizes" << endl;
	}
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator += ( ofxCvColorImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvAdd( cvImage, mom.getCvImage(), cvImageTemp );
		swapTemp();
	} else {
        cout << "error in +=, images are different sizes" << endl;
	}
}

//--------------------------------------------------------------------------------
void ofxCvColorImage::operator *= ( ofxCvColorImage& mom ) {
    float scalef = 1.0f / 255.0f;
	if( mom.width == width && mom.height == height ) {
		cvMul( cvImage, mom.getCvImage(), cvImageTemp, scalef );
		swapTemp();
	} else {
        cout << "error in *=, images are different sizes" << endl;
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
void ofxCvColorImage::scaleIntoMe( ofxCvColorImage& mom, int interpolationMethod){

    if ((interpolationMethod != CV_INTER_NN) ||
        (interpolationMethod != CV_INTER_LINEAR) ||
        (interpolationMethod != CV_INTER_AREA) ||
        (interpolationMethod != CV_INTER_CUBIC) ){
        printf("error in scaleIntoMe / interpolationMethod, setting to CV_INTER_NN \n");
		interpolationMethod = CV_INTER_NN;
	}
    cvResize( mom.getCvImage(), cvImage, interpolationMethod );

    /*
    you can pass in:
    CV_INTER_NN - nearest-neigbor interpolation,
    CV_INTER_LINEAR - bilinear interpolation (used by default)
    CV_INTER_AREA - resampling using pixel area relation. It is preferred method for image decimation that gives moire-free results. In case of zooming it is similar to CV_INTER_NN method.
    CV_INTER_CUBIC - bicubic interpolation.
    ----> http://opencvlibrary.sourceforge.net/CvReference
    */

}
