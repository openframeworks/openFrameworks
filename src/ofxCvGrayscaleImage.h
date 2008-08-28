#ifndef OFX_CV_GRAYSCALE_IMAGE_H
#define OFX_CV_GRAYSCALE_IMAGE_H

#include "ofxCvImage.h"
#include "ofxCvBlob.h"

// for all the things ofxCvGrayscaleImage can do, check:
// ofxCvImage.h


class ofxCvGrayscaleImage : public ofxCvImage {


  public:

    ofxCvGrayscaleImage() {};
    ofxCvGrayscaleImage( const ofxCvGrayscaleImage& mom );
    void allocate( int w, int h );

    // Set Pixel Data - Arrays
    void setFromPixels( unsigned char* _pixels, int w, int h );
    void set(int value);
	void invert( );


    void operator = ( unsigned char* _pixels );
    void operator = ( ofxCvGrayscaleImage& mom );
    void operator = ( ofxCvColorImage& mom );
    void operator = ( ofxCvFloatImage& mom );
    void operator -= ( ofxCvGrayscaleImage& mom );
    void operator += ( ofxCvGrayscaleImage& mom );
    void operator *= ( ofxCvGrayscaleImage& mom );
    void operator &= ( ofxCvGrayscaleImage& mom );
    void operator -=	( float scalar );
	void operator +=	( float scalar );

    void absDiff( ofxCvGrayscaleImage& mom );
    void absDiff( ofxCvGrayscaleImage& mom, ofxCvGrayscaleImage& dad );

    // Get Pixel Data
    unsigned char*  getPixels();

    // Draw Image
    void draw( float x, float y );
    void draw( float x, float y, float w, float h );

    void drawBlobIntoMe( ofxCvBlob & blob, int color );

    // Image Filter Operations
    void contrastStretch();
    void threshold( int value, bool invert = false);

    // Image Transformation Operations
    void resize( int w, int h );
    void scaleIntoMe( ofxCvGrayscaleImage& mom, int interpolationMethod = CV_INTER_NN);

   //---------------------------------------------
    //---------------- legacy ---------------------
	// this code is kept here to allow older ofxCv code to work
    // you should use the newer functions (ie, blur(33) instead of blurHeavily)
    // since we might not always keep old code around

    void blurHeavily() { blur(33); };   // for legacy code
	void erode_3x3() { erode(); };
	void dilate_3x3() { dilate(); };
	void setFromColorImage(ofxCvColorImage& mom) { *this = mom; };
	void setFromFloatImage(ofxCvFloatImage& mom) { *this = mom; };


    //--------------------------------------------
    //---------------- in ofImage ----------------
    // the code below is inside of ofImage, meaning we can call it
    // on this object (since it inherits ofImage)
    // kept here as a reference....

    /*
    virtual void draw( float x, float y ) = 0;
    virtual void draw( float x, float y, float w, float h ) = 0;
    virtual void erode( );  // based on 3x3 shape
    virtual void dilate( );  // based on 3x3 shape
    virtual void blur( int value=3 );  // value = x*2+1, where x is an integer
    virtual void blurGaussian( int value=3 );  // value = x*2+1, where x is an integer
    virtual void mirror( bool bFlipVertically, bool bFlipHorizontally );

    virtual void translate( float x, float y );
    virtual void rotate( float angle, float centerX, float centerY );
    virtual void scale( float scaleX, float sclaeY );
    virtual void transform( float angle, float centerX, float centerY,
                            float scaleX, float scaleY,
                            float moveX, float moveY );
    virtual void undistort( float radialDistX, float radialDistY,
                            float tangentDistX, float tangentDistY,
                            float focalX, float focalY,
                            float centerX, float centerY );

    virtual void remap( IplImage* mapX, IplImage* mapY );
    virtual void warpPerspective( const ofPoint& A,
                                  const ofPoint& B,
                                  const ofPoint& C,
                                  const ofPoint& D );
    virtual void warpIntoMe( const ofxCvGrayscaleImage& mom,
                             ofPoint src[4], ofPoint dst[4] );
    virtual int countNonZeroInRegion( int x, int y, int w, int h ) const;
    */

};



#endif

