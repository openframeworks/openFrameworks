
#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include "ofxCvFloatImage.h"


//--------------------------------------------------------------------------------
ofxCvGrayscaleImage::ofxCvGrayscaleImage( const ofxCvGrayscaleImage& mom ) {
    cvCopy( mom.getCvImage(), cvImage, 0 );
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::allocate( int w, int h ) {
	
	if (bAllocated == true){
		printf ("warning: reallocating cvImage in ofxCvGrayscaleImage\n");
		clear();
	}

	cvImage = cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 1 );
	cvImageTemp	= cvCreateImage( cvSize(w,h), IPL_DEPTH_8U, 1 );
	pixels = new unsigned char[w*h];
	width = w;
	height = h;
	bAllocated = true;

    if( bUseTexture ) {
        tex.allocate( width, height, GL_LUMINANCE );
    }
}




// Set Pixel Data - Arrays
//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::setFromPixels( unsigned char* _pixels, int w, int h ) {
	for( int i = 0; i < h; i++ ) {
		memcpy( cvImage->imageData+(i*cvImage->widthStep), _pixels+(i*w), w );
	}
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator =	( unsigned char* _pixels ) {
    setFromPixels( _pixels, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator =	( ofxCvGrayscaleImage& mom ) {
	if( mom.width == width && mom.height == height ) {
        cvCopy( mom.getCvImage(), cvImage, 0 );
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator =	( ofxCvColorImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvCvtColor( mom.getCvImage(), cvImage, CV_RGB2GRAY );
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator =	( ofxCvFloatImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvConvertScale( mom.getCvImage(), cvImage, 1.0f/255.0f, 0);
	} else {
        cout << "error in =, images are different sizes" << endl;
	}
}


//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator -= ( ofxCvGrayscaleImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvSub( cvImage, mom.getCvImage(), cvImageTemp );
		swapTemp();
	} else {
        cout << "error in -=, images are different sizes" << endl;
	}
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator += ( ofxCvGrayscaleImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvAdd( cvImage, mom.getCvImage(), cvImageTemp );
		swapTemp();
	} else {
        cout << "error in +=, images are different sizes" << endl;
	}
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator *= ( ofxCvGrayscaleImage& mom ) {
    float scalef = 1.0f / 255.0f;
	if( mom.width == width && mom.height == height ) {
		cvMul( cvImage, mom.getCvImage(), cvImageTemp, scalef );
		swapTemp();
	} else {
        cout << "error in *=, images are different sizes" << endl;
	}
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator &= ( ofxCvGrayscaleImage& mom ) {
	if( mom.width == width && mom.height == height ) {
		cvAnd( cvImage, mom.getCvImage(), cvImageTemp );
		swapTemp();
	} else {
        cout << "error in &=, images are different sizes" << endl;
	}
}


//-------------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator -=	( float scalar ){
	cvSubS(cvImage, cvScalar(scalar), cvImageTemp);
	swapTemp();
}

//-------------------------------------------------------------------------------------
void ofxCvGrayscaleImage::operator +=	( float scalar ){
	cvAddS(cvImage, cvScalar(scalar), cvImageTemp);
	swapTemp();
}


//-------------------------------------------------------------------------------------
void ofxCvGrayscaleImage::set(int value){
	cvSet(cvImage, cvScalar(value));
}


//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::absDiff( ofxCvGrayscaleImage& mom ) {
    if( mom.width == width && mom.height == height ) {
        cvAbsDiff( cvImage, mom.getCvImage(), cvImageTemp );
        swapTemp();
    } else {
        cout << "error in absDiff, images are different sizes" << endl;
    }
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::absDiff( ofxCvGrayscaleImage& mom,
                                  ofxCvGrayscaleImage& dad ) {
    if(( mom.width == width && mom.height == height ) &&
       ( dad.width == width && dad.height == height )){
        cvAbsDiff( mom.getCvImage(), dad.getCvImage(), cvImage );
    } else {
        cout << "error in absDiff, images are different sizes" << endl;
    }

}


// Get Pixel Data
//--------------------------------------------------------------------------------
unsigned char* ofxCvGrayscaleImage::getPixels() {
	for( int i = 0; i < height; i++ ) {
		memcpy( pixels+(i*width),
                cvImage->imageData+(i*cvImage->widthStep), width );
	}
	return pixels;
}


// Draw Image
//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::draw( float x, float y ) {
    if( bUseTexture ) {
        // note, this is a bit ineficient, as we have to
        // copy the data out of the cvImage into the pixel array
        // and then upload to texture.  We should add
        // to the texture class an override for pixelstorei
        // that allows stepped-width image upload:
        tex.loadData(getPixels(), width, height, GL_LUMINANCE);
        tex.draw(x,y,width, height);

    } else {
        IplImage* o;
        o = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 1 );
        cvResize( cvImage, o, CV_INTER_NN );
        cvFlip( o, o, 0 );
        glRasterPos3f( x, y+height, 0.0 );
        glDrawPixels( o->width, o->height ,
                     GL_LUMINANCE, GL_UNSIGNED_BYTE, o->imageData );
        cvReleaseImage( &o );
        glRasterPos3f( -x, -(y+height), 0.0 );
    }
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::draw( float x, float y, float w, float h ) {
    if( bUseTexture ) {
        tex.loadData(getPixels(), width, height, GL_LUMINANCE);
        tex.draw(x,y, w,h);

    } else {
        // warning this is gonna be slow !
        IplImage* tempImg;
        tempImg = cvCreateImage( cvSize((int)w, (int)h), IPL_DEPTH_8U, 1 );
        cvResize( cvImage, tempImg, CV_INTER_NN );
        cvFlip( tempImg, tempImg, 0 );
        glRasterPos3f( x, y+h, 0.0 );
        glDrawPixels( tempImg->width, tempImg->height ,
                     GL_LUMINANCE, GL_UNSIGNED_BYTE, tempImg->imageData );
        cvReleaseImage( &tempImg );
        glRasterPos3f( -x, -(y+h), 0.0 );
    }
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
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::threshold( int value, bool invert) {
	//http://lush.sourceforge.net/lush-manual/01a8321b.html
	if(invert) cvThreshold( cvImage, cvImageTemp, value, 255, CV_THRESH_BINARY_INV );
	else cvThreshold( cvImage, cvImageTemp, value, 255, CV_THRESH_BINARY );
	swapTemp();
}

//--------------------------------------------------------------------------------
void ofxCvGrayscaleImage::invert(){
    cvNot(cvImage, cvImage);
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
void ofxCvGrayscaleImage::scaleIntoMe( ofxCvGrayscaleImage& mom, int interpolationMethod){

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

