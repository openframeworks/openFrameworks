
#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include "ofxCvFloatImage.h"
#include "ofxCvShortImage.h"




//--------------------------------------------------------------------------------
ofxCvShortImage::ofxCvShortImage() {
    cvGrayscaleImage = NULL;
}

//--------------------------------------------------------------------------------
ofxCvShortImage::ofxCvShortImage( const ofxCvShortImage& mom ) {
    allocate(mom.width, mom.height);    
    cvCopy( mom.getCvImage(), cvImage, 0 );
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::allocate( int w, int h ) {
	if (bAllocated == true){
		ofLog(OF_WARNING, "in allocate, reallocating a ofxCvShortImage");
		clear();
	}
	cvImage = cvCreateImage( cvSize(w,h), IPL_DEPTH_16U, 1 );
	cvImageTemp	= cvCreateImage( cvSize(w,h), IPL_DEPTH_16U, 1 );
    width = w;
	height = h;
	bAllocated = true;
    if( bUseTexture ) {
        tex.allocate(width, height, GL_LUMINANCE);
        bTextureDirty = true;
    }
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
void ofxCvShortImage::convertShortToGray( const IplImage* floatImg, IplImage* grayImg ) {
    // map from 0-65535 to 0-255
    float scale = 255.0f/65535.0f;
    cvConvertScale( floatImg, grayImg, scale, 0 );
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::convertGrayToShort( const IplImage* grayImg, IplImage* floatImg ) {
    // map from 0-255 to 0-65535
    cvConvertScale( grayImg, floatImg, 65535.0f/255.0f, 0 );
}




// Set Pixel Data

//-------------------------------------------------------------------------------------
void ofxCvShortImage::set(float value){  
	cvSet(cvImage, cvScalar(value));
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::setFromPixels( unsigned char* _pixels, int w, int h ) {
    if( cvGrayscaleImage == NULL ) {
        cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
    }
    for( int i = 0; i < h; i++ ) {
		memcpy( cvGrayscaleImage->imageData+(i*cvGrayscaleImage->widthStep), _pixels+(i*w), w );
	}
    
    convertGrayToShort(cvGrayscaleImage, cvImage);
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::setFromPixels( unsigned short int* _pixels, int w, int h ) {
	for( int i = 0; i < h; i++ ) {
		memcpy( cvImage->imageData+(i*cvImage->widthStep), _pixels+(i*w), w*sizeof(float));
	}
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( unsigned char* _pixels ) {
    setFromPixels( _pixels, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( unsigned short int* _pixels ) {
    setFromPixels( _pixels, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const ofxCvGrayscaleImage& mom ) {
	if( mom.width == width && mom.height == height ) {
        convertGrayToShort(mom.getCvImage(), cvImage);       
        flagImageChanged();
	} else {
        ofLog(OF_ERROR, "in =, images are different sizes");
	}
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const ofxCvColorImage& mom ) {
	if( mom.width == width && mom.height == height ) {
        if( cvGrayscaleImage == NULL ) {
            cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
        }    
		cvCvtColor( mom.getCvImage(), cvGrayscaleImage, CV_RGB2GRAY );
        convertGrayToShort(cvGrayscaleImage, cvImage);                
        flagImageChanged();
	} else {
        ofLog(OF_ERROR, "in =, images are different sizes");
	}
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const ofxCvFloatImage& mom ) {
    if( mom.width == width && mom.height == height ) {
    
        // map from 0-1 to 0-65535
        cvConvertScale( mom.getCvImage(), cvImage, 65535, 0 ); 
        
        flagImageChanged();
    } else {
        ofLog(OF_ERROR, "in =, images are different sizes");
    }
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::operator = ( const ofxCvShortImage& mom ) {
    if(this != &mom) {  //check for self-assignment
        if( mom.width == width && mom.height == height ) {
            cvCopy( mom.getCvImage(), cvImage, 0 ); 
            flagImageChanged();
        } else {
            ofLog(OF_ERROR, "in =, images are different sizes");
        }
    } else {
        ofLog(OF_WARNING, "in =, you are assigning a ofxCvShortImage to itself");
    }
}


//--------------------------------------------------------------------------------
void ofxCvShortImage::addWeighted( ofxCvGrayscaleImage& mom, float f ) {
	if( mom.width == width && mom.height == height ) {
         convertGrayToShort(mom.getCvImage(), cvImageTemp);
         cvAddWeighted( cvImageTemp, f, cvImage, 1.0f-f,0, cvImage );
         flagImageChanged();
         
    } else {
        ofLog(OF_ERROR, "in addWeighted, images are different sizes");
    }
}



// Get Pixel Data

//--------------------------------------------------------------------------------
unsigned char*  ofxCvShortImage::getPixels(){
    if(bPixelsDirty) {
        if( cvGrayscaleImage == NULL ) { cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 ); } 
        convertShortToGray(cvImage, cvGrayscaleImage); 
        if(pixels == NULL) { pixels = new unsigned char[width*height]; };
        for( int i = 0; i < height; i++ ) {
            memcpy( pixels+(i*width),
                    cvGrayscaleImage->imageData+(i*cvGrayscaleImage->widthStep), width );
        }
        bPixelsDirty = false;
    }
	return pixels;
}



// Draw Image

//--------------------------------------------------------------------------------
void ofxCvShortImage::drawWithoutTexture( float x, float y ) {
    drawWithoutTexture( x,y, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvShortImage::drawWithoutTexture( float x, float y, float w, float h ) {
    // this is slower than the typical draw method based on textures
    // but useful when dealing with threads GL textures often don't work
    
    if( x == 0) {
        x += 0.01;
        ofLog( OF_NOTICE, "BUG: can't draw at x==0 in texture-less mode.");
    }
    
    glRasterPos3f( x, y+h, 0.0 );
    IplImage* tempImg;
    tempImg = cvCreateImage( cvSize((int)w, (int)h), IPL_DEPTH_16U, 1 );
    cvResize( cvImage, tempImg, CV_INTER_NN );
    
    cvFlip( tempImg, tempImg, 0 );
    glDrawPixels( tempImg->width, tempImg->height ,
                 GL_LUMINANCE, GL_UNSIGNED_SHORT, tempImg->imageData );
    cvReleaseImage( &tempImg );
    glRasterPos3f( -x, -(y+h), 0.0 );
}



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
void ofxCvShortImage::scaleIntoMe( const ofxCvImage& mom, int interpolationMethod ){
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
        flagImageChanged();

    } else {
        ofLog(OF_ERROR, "in scaleIntoMe, mom image type has to match");
    }
}
