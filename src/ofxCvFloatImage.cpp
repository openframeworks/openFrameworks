
#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include "ofxCvFloatImage.h"




//--------------------------------------------------------------------------------
ofxCvFloatImage::ofxCvFloatImage() {
    pixelsAsFloats = NULL;
    bFloatPixelsDirty = true;
    cvGrayscaleImage = NULL;
    scaleMin = 0.0f;
    scaleMax = 1.0f;
}

//--------------------------------------------------------------------------------
ofxCvFloatImage::ofxCvFloatImage( const ofxCvFloatImage& mom ) {
    allocate(mom.width, mom.height);    
    cvCopy( mom.getCvImage(), cvImage, 0 );
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::allocate( int w, int h ) {
	if (bAllocated == true){
		ofLog(OF_WARNING, "in allocate, reallocating a ofxCvFloatImage");
		clear();
	}
	cvImage = cvCreateImage( cvSize(w,h), IPL_DEPTH_32F, 1 );
	cvImageTemp	= cvCreateImage( cvSize(w,h), IPL_DEPTH_32F, 1 );
    width = w;
	height = h;
	bAllocated = true;
    if( bUseTexture ) {
        tex.allocate(width, height, GL_LUMINANCE);
        bTextureDirty = true;
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::clear() {
    if (bAllocated == true){
        if( cvGrayscaleImage != NULL ){
            cvReleaseImage( &cvGrayscaleImage );
        }
        if( pixelsAsFloats != NULL ) {
            delete pixelsAsFloats;
            pixelsAsFloats = NULL;
            bFloatPixelsDirty = true;
        }
    }
    ofxCvImage::clear();    //call clear in base class    
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::setNativeScale( float _scaleMin, float _scaleMax ) {
    scaleMin = _scaleMin;
    scaleMax = _scaleMax;
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::imageHasChanged() {
    bFloatPixelsDirty = true;
    ofxCvImage::imageHasChanged();
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::convertFloatToGray( const IplImage* floatImg, IplImage* grayImg ) {
    // map from scaleMin-scaleMax to 0-255
    float scale = 255.0f/(scaleMax-scaleMin);
    cvConvertScale( floatImg, grayImg, scale, -(scaleMin*scale) );
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::convertGrayToFloat( const IplImage* grayImg, IplImage* floatImg ) {
    // map from 0-255 to scaleMin-scaleMax
    cvConvertScale( grayImg, floatImg, (scaleMax-scaleMin)/255.0f, scaleMin );
}




// Set Pixel Data

//-------------------------------------------------------------------------------------
void ofxCvFloatImage::set(float value){  
	cvSet(cvImage, cvScalar(value));
    imageHasChanged();
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator *= ( float scalar ){
    // does this exist in opencv?
    int totalPixels = cvImage->width*cvImage->height;
    if(totalPixels <= 0)return;

    float * ptr = ((float *)cvImage->imageData);

    for (int i = 0; i < totalPixels; i++){
       ( *ptr ) *= scalar;
        if( ( *ptr ) < 0.00001 ) (*ptr) = 0.0;
        ptr++;
    }
    
    imageHasChanged();
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator /= ( float scalar ){
    // does this exist in opencv?
    int totalPixels = cvImage->width*cvImage->height;
    if(totalPixels == 0 || scalar == 0)return;

    //inverse and then multiply
    scalar = 1.0 / scalar;

    float * ptr = ((float *)cvImage->imageData);

    for (int i = 0; i < totalPixels; i++){
       ( *ptr ) *= scalar;
        if( ( *ptr ) < 0.00001 ) (*ptr) = 0.0;
        ptr++;
    }
    
    imageHasChanged();
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::setFromPixels( unsigned char* _pixels, int w, int h ) {
    if( cvGrayscaleImage == NULL ) {
        cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
    }
    for( int i = 0; i < h; i++ ) {
		memcpy( cvGrayscaleImage->imageData+(i*cvGrayscaleImage->widthStep), _pixels+(i*w), w );
	}
    
    convertGrayToFloat(cvGrayscaleImage, cvImage);
    imageHasChanged();
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::setFromPixels( float* _pixels, int w, int h ) {
	for( int i = 0; i < h; i++ ) {
		memcpy( cvImage->imageData+(i*cvImage->widthStep), _pixels+(i*w), w*sizeof(float));
	}
    imageHasChanged();
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
void ofxCvFloatImage::operator = ( const ofxCvGrayscaleImage& mom ) {
	if( mom.width == width && mom.height == height ) {
        convertGrayToFloat(mom.getCvImage(), cvImage);       
        imageHasChanged();
	} else {
        ofLog(OF_ERROR, "in =, images are different sizes");
	}
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator = ( const ofxCvColorImage& mom ) {
	if( mom.width == width && mom.height == height ) {
        if( cvGrayscaleImage == NULL ) {
            cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );
        }    
		cvCvtColor( mom.getCvImage(), cvGrayscaleImage, CV_RGB2GRAY );
        convertGrayToFloat(cvGrayscaleImage, cvImage);                
        imageHasChanged();
	} else {
        ofLog(OF_ERROR, "in =, images are different sizes");
	}
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator = ( const ofxCvFloatImage& mom ) {
    if(this != &mom) {  //check for self-assignment
        if( mom.width == width && mom.height == height ) {
            cvCopy( mom.getCvImage(), cvImage, 0 );
            imageHasChanged();
        } else {
            ofLog(OF_ERROR, "in =, images are different sizes");
        }
    } else {
        ofLog(OF_WARNING, "in =, you are assigning a ofxCvFloatImage to itself");
    }
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator *= ( const ofxCvImage& mom ) {
	if( mom.width == width && mom.height == height &&
	    mom.getCvImage()->nChannels == cvImage->nChannels && 
        mom.getCvImage()->depth == cvImage->depth )
    {
		cvMul( cvImage, mom.getCvImage(), cvImageTemp );
		swapTemp();
        imageHasChanged();
	} else {
        ofLog(OF_ERROR, "in *=, images need to match in size and type");
	}
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::operator &= ( const ofxCvImage& mom ) {
	if( mom.width == width && mom.height == height &&
	    mom.getCvImage()->nChannels == cvImage->nChannels && 
        mom.getCvImage()->depth == cvImage->depth )
    {
	    //this is doing it bit-wise; probably not what we want
		cvAnd( cvImage, mom.getCvImage(), cvImageTemp );
		swapTemp();
        imageHasChanged();
	} else {
        ofLog(OF_ERROR, "in &=, images need to match in size and type");
	}
}


//--------------------------------------------------------------------------------
void ofxCvFloatImage::addWeighted( ofxCvGrayscaleImage& mom, float f ) {
	if( mom.width == width && mom.height == height ) {
         convertGrayToFloat(mom.getCvImage(), cvImageTemp);
         cvAddWeighted( cvImageTemp, f, cvImage, 1.0f-f,0, cvImage );
         imageHasChanged();
         
    } else {
        ofLog(OF_ERROR, "in addWeighted, images are different sizes");
    }
}



// Get Pixel Data

//--------------------------------------------------------------------------------
unsigned char*  ofxCvFloatImage::getPixels(){
    if(bPixelsDirty) {
        if( cvGrayscaleImage == NULL ) { cvGrayscaleImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 ); } 
        convertFloatToGray(cvImage, cvGrayscaleImage); 
        if(pixels == NULL) { pixels = new unsigned char[width*height]; };
        for( int i = 0; i < height; i++ ) {
            memcpy( pixels+(i*width),
                    cvGrayscaleImage->imageData+(i*cvGrayscaleImage->widthStep), width );
        }
        bPixelsDirty = false;
    }
	return pixels;
}

//--------------------------------------------------------------------------------
float*  ofxCvFloatImage::getPixelsAsFloats(){
    if(bFloatPixelsDirty) {
        if( pixelsAsFloats == NULL ) { pixelsAsFloats = new float[width*height]; }
        for( int i = 0; i < height; i++ ) {
            memcpy( pixelsAsFloats+(i*width),
                    cvImage->imageData+(i*cvImage->widthStep), width*4 );
        }
        bFloatPixelsDirty = false;
    }
	return pixelsAsFloats;
}



// Draw Image

//--------------------------------------------------------------------------------
void ofxCvFloatImage::drawWithoutTexture( float x, float y ) {
    drawWithoutTexture( x,y, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::drawWithoutTexture( float x, float y, float w, float h ) {
    // this is slower than the typical draw method based on textures
    // but useful when dealing with threads GL textures often don't work
    
    if( x == 0) {
        x += 0.01;
        ofLog(OF_NOTICE, "BUG: can't draw at x==0 in texture-less mode.");
    }
    
    glRasterPos3f( x, y+h, 0.0 );
    IplImage* tempImg;
    tempImg = cvCreateImage( cvSize((int)w, (int)h), IPL_DEPTH_32F, 1 );
    cvResize( cvImage, tempImg, CV_INTER_NN );
    
    // map from scaleMin-scaleMax to 0-1
    float scale = 1.0f/(scaleMax-scaleMin);
    cvConvertScale( tempImg, tempImg, scale, -(scaleMin*scale) );    
    cvFlip( tempImg, tempImg, 0 );
    glDrawPixels( tempImg->width, tempImg->height ,
                 GL_LUMINANCE, GL_FLOAT, tempImg->imageData );
    cvReleaseImage( &tempImg );
    glRasterPos3f( -x, -(y+h), 0.0 );
}



// Image Filter Operations

//--------------------------------------------------------------------------------
void ofxCvFloatImage::contrastStretch() {
	double minVal, maxVal;
	cvMinMaxLoc( cvImage, &minVal, &maxVal, NULL, NULL, 0 );
    rangeMap( cvImage, minVal,maxVal, scaleMin,scaleMax );
    imageHasChanged();
}

//--------------------------------------------------------------------------------
void ofxCvFloatImage::convertToRange(float min, float max ){
    rangeMap( cvImage, scaleMin,scaleMax, min,max);
    imageHasChanged();
}



// Image Transformation Operations

//--------------------------------------------------------------------------------
void ofxCvFloatImage::resize( int w, int h ) {

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
void ofxCvFloatImage::scaleIntoMe( const ofxCvImage& mom, int interpolationMethod ){
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
