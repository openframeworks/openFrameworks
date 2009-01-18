
#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include "ofxCvFloatImage.h"


//--------------------------------------------------------------------------------
ofxCvGrayscaleImage::ofxCvGrayscaleImage( const ofxCvGrayscaleImage& mom ) {
    allocate(mom.width, mom.height);
    cvCopy( mom.getCvImage(), cvImage, 0 );
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::allocate( int w, int h ) {
	if (bAllocated == true){
		cout << "warning: reallocating cvImage in ofxCvGrayscaleImage" << endl;
		clear();
	}
    
	cvImage = cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 1 );
	cvImageTemp	= cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 1 );
    
	width = w;
	height = h;
	bAllocated = true;

    if( bUseTexture ) {
        tex.allocate( width, height, GL_LUMINANCE );
        bTextureDirty = true;
    }
}




// Set Pixel Data - Arrays
//-------------------------------------------------------------------------------------
void ofxCvGrayscaleImage::set( float value ){
	cvSet(cvImage, cvScalar(value));
    imageHasChanged();
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::setFromPixels( unsigned char* _pixels, int w, int h ) {
	for( int i = 0; i < h; i++ ) {
		memcpy( cvImage->imageData+(i*cvImage->widthStep), _pixels+(i*w), w );
	}
    imageHasChanged();
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator = ( unsigned char* _pixels ) {
    setFromPixels( _pixels, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator = ( const ofxCvGrayscaleImage& mom ) {
    if(this != &mom) {  //check for self-assignment
        if( mom.width == width && mom.height == height ) {
            cvCopy( mom.getCvImage(), cvImage, 0 );
            imageHasChanged();
        } else {
            cout << "error in =, images are different sizes" << endl;
        }
    } else {
        cout << "warning: ignoring self-assignment in ofxCvGrayscaleImage::operator =" << endl;
    }
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator = ( const ofxCvColorImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvCvtColor( mom.getCvImage(), cvImage, CV_RGB2GRAY );
        imageHasChanged();
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator = ( const ofxCvFloatImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		//cvConvertScale( mom.getCvImage(), cvImage, 1.0f, 0);
        cvConvert( mom.getCvImage(), cvImage );
        imageHasChanged();
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}


//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::absDiff( ofxCvGrayscaleImage& mom ) {
    if( mom.width == width && mom.height == height ) {
        cvAbsDiff( cvImage, mom.getCvImage(), cvImageTemp );
        swapTemp();
        imageHasChanged();
    } else {
        cout << "error in absDiff, images are different sizes" << endl;
    }
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::absDiff( ofxCvGrayscaleImage& mom,
                                   ofxCvGrayscaleImage& dad ) {
    if( (mom.width == width && mom.height == height ) &&
        (dad.width == width && dad.height == height ) )
    {
        cvAbsDiff( mom.getCvImage(), dad.getCvImage(), cvImage );
        imageHasChanged();
    } else {
        cout << "error in absDiff, images are different sizes" << endl;
    }

}


// Get Pixel Data
//--------------------------------------------------------------------------------
unsigned char* ofxCvGrayscaleImage::getPixels() {
    if(bPixelsDirty) {
        if(pixels == NULL) { pixels = new unsigned char[width*height]; };
        for( int i = 0; i < height; i++ ) {
            memcpy( pixels+(i*width),
                    cvImage->imageData+(i*cvImage->widthStep), width );
        }
        bPixelsDirty = false;
    }
	return pixels;
}


// Draw Image

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::drawWithoutTexture( float x, float y ) {
    drawWithoutTexture( x,y, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::drawWithoutTexture( float x, float y, float w, float h ) {
    // this is slower than the typical draw method based on textures
    // but useful when dealing with threads GL textures often don't work
    
    if( x == 0) {
        x += 0.01;
        //ofLog( OF_NOTICE, "BUG: can't draw at x==0 in texture-less mode.\n");
    }
    
    glRasterPos3f( x, y+h, 0.0 );
    IplImage* tempImg;
    tempImg = cvCreateImage( cvSize((int)w, (int)h), IPL_DEPTH_8U, 1 );
    cvResize( cvImage, tempImg, CV_INTER_NN );
    cvFlip( tempImg, tempImg, 0 );
    glDrawPixels( tempImg->width, tempImg->height ,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, tempImg->imageData );
    cvReleaseImage( &tempImg );
    glRasterPos3f( -x, -(y+h), 0.0 );
}

//--------------------------------------------------------------------------------
void  ofxCvGrayscaleImage::drawBlobIntoMe( ofxCvBlob &blob, int color ) {
       if( blob.pts.size() > 0 ) {
           CvPoint* pts = new CvPoint[blob.nPts];
           for( int i=0; i < blob.nPts ; i++ ) {
               pts[i].x = (int)blob.pts[i].x;
               pts[i].y = (int)blob.pts[i].y;
           }
           int nPts = blob.nPts;
           cvFillPoly( cvImage, &pts, &nPts, 1,
                       CV_RGB(color,color,color) );
           delete pts;
           imageHasChanged();
       }
}


// Image Filter Operations

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::contrastStretch() {
	double minVal, maxVal;
	cvMinMaxLoc( cvImage, &minVal, &maxVal, NULL, NULL, 0 );
	double scale=1.0f;
	double shift=0;
	if( (maxVal-minVal) != 0 ) {
		scale=255.0/(maxVal-minVal);
    	shift=-minVal*scale;
	}
	cvConvertScale( cvImage, cvImageTemp, scale, shift );
	swapTemp();
    imageHasChanged();
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::threshold( int value, bool invert) {
	//http://lush.sourceforge.net/lush-manual/01a8321b.html
	if(invert) cvThreshold( cvImage, cvImageTemp, value, 255, CV_THRESH_BINARY_INV );
	else cvThreshold( cvImage, cvImageTemp, value, 255, CV_THRESH_BINARY );
	swapTemp();
    imageHasChanged();
}


// Image Transformation Operations
//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::resize( int w, int h ) {

    // note, one image copy operation could be ommitted by
    // reusing the temporal image storage

    IplImage* temp = cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 1 );
    cvResize( cvImage, temp );
    clear();
    allocate( w, h );
    cvCopy( temp, cvImage );
    cvReleaseImage( &temp );
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::scaleIntoMe( const ofxCvImage& mom, int interpolationMethod ){
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
        imageHasChanged();

    } else {
        cout << "error in scaleIntoMe: mom image type has to match" << endl;
    }
}

