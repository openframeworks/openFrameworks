#ifndef OFX_CV_GRAYSCALE_IMAGE_H
#define OFX_CV_GRAYSCALE_IMAGE_H

#include "ofxCvImage.h"
#include "ofxCvBlob.h"



class ofxCvGrayscaleImage : public ofxCvImage {


  public:

    ofxCvGrayscaleImage() {};
    ofxCvGrayscaleImage( const ofxCvGrayscaleImage& mom );
    void allocate( int w, int h );
    // virtual void clear();                        //in base class
    // virtual void setUseTexture( bool bUse );     //in base class   


    // Set Pixel Data
    //
    void set( float value );
    // virtual void operator -= ( float value );    //in base class 
    // virtual void operator += ( float value );    //in base class
    
    void setFromPixels( unsigned char* _pixels, int w, int h );
    void operator = ( unsigned char* _pixels );
    void operator = ( const ofxCvGrayscaleImage& mom );
    void operator = ( const ofxCvColorImage& mom );
    void operator = ( const ofxCvFloatImage& mom );

    // virtual void operator -= ( const ofxCvImage& mom );    //in base class 
    // virtual void operator += ( const ofxCvImage& mom );    //in base class 
    // virtual void operator *= ( const ofxCvImage& mom );    //in base class 
    // virtual void operator &= ( const ofxCvImage& mom );    //in base class 

    void absDiff( ofxCvGrayscaleImage& mom );
    void absDiff( ofxCvGrayscaleImage& mom, ofxCvGrayscaleImage& dad );


    // Get Pixel Data
    //
    unsigned char*  getPixels();
    // virtual IplImage*  getCvImage() const { return cvImage; };    //in base class


    // Draw Image
    //
    void draw( float x, float y );
    void draw( float x, float y, float w, float h );

    void drawBlobIntoMe( ofxCvBlob & blob, int color );


    // Image Filter Operations
    //
    void contrastStretch();
    void threshold( int value, bool invert = false);
    // virtual void erode( );                                //in base class
    // virtual void dilate( );                               //in base class
    // virtual void blur( int value=3 );                     //in base class
    // virtual void blurGaussian( int value=3 );             //in base class
    // virtual void invert();                                //in base class
    // virtual void convertToRange(float min, float max);    //in base class    


    // Image Transformation Operations
    //
    void resize( int w, int h );
    void scaleIntoMe( const ofxCvImage& mom, int interpolationMethod = CV_INTER_NN);
    // virtual void mirror( bool bFlipVertically, bool bFlipHorizontally );  //in base class
    // virtual void translate( float x, float y );                           //in base class
    // virtual void rotate( float angle, float centerX, float centerY );     //in base class
    // virtual void scale( float scaleX, float sclaeY );                     //in base class
    // virtual void transform( float angle, float centerX, float centerY,
    //                        float scaleX, float scaleY,
    //                        float moveX, float moveY );                    //in base class
    // virtual void undistort( float radialDistX, float radialDistY,
    //                         float tangentDistX, float tangentDistY,
    //                         float focalX, float focalY,
    //                         float centerX, float centerY );               //in base class
    // virtual void remap( IplImage* mapX, IplImage* mapY );                 //in base class
    // virtual void warpPerspective( const ofPoint& A,
    //                               const ofPoint& B,
    //                               const ofPoint& C,
    //                               const ofPoint& D );                     //in base class
    // virtual void warpIntoMe( const ofxCvGrayscaleImage& mom,
    //                          ofPoint src[4], ofPoint dst[4] );            //in base class
                             

    // Other Image Operations
    //
    // virtual int countNonZeroInRegion( int x, int y, int w, int h ) const; //in base class
    
    
    
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

};



#endif

