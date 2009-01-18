/*
* ofxCvImage.h
* by stefanix
*
* This is the common abstract base class of all the different
* image types used in this addon. Methods with "= 0" at the end
* are pure virtual and need to be implemented by the inheriting
* image calss.
*
*/


#ifndef OFX_CV_IMAGE_H
#define OFX_CV_IMAGE_H


#include "ofxCvConstants.h"

class ofxCvGrayscaleImage;
class ofxCvColorImage;
class ofxCvFloatImage;



class ofxCvImage {
    
  // the "= 0" after a method means "pure virtual"
  // it also means they have to be implemented in
  // ofxCvGrayscaleImage, ofxCvColorImage, ...


  public:

    int width;
    int height;
	bool bAllocated;

    ofxCvImage();
    virtual ~ofxCvImage();
    virtual void allocate( int w, int h ) = 0;
    virtual void clear();
    virtual void setUseTexture( bool bUse );


    // Set Pixel Data
    //
    virtual void set( float value ) = 0;
    virtual void operator -= ( float value );
    virtual void operator += ( float value );

    virtual void setFromPixels( unsigned char* _pixels, int w, int h ) = 0;
    virtual void operator = ( const ofxCvGrayscaleImage& mom ) = 0;
    virtual void operator = ( const ofxCvColorImage& mom ) = 0;
    virtual void operator = ( const ofxCvFloatImage& mom ) = 0;
    
    virtual void operator -= ( const ofxCvImage& mom );
    virtual void operator += ( const ofxCvImage& mom );
    virtual void operator *= ( const ofxCvImage& mom );
    virtual void operator &= ( const ofxCvImage& mom );
    

    // Get Pixel Data
    //
    virtual unsigned char*  getPixels() = 0;
    virtual IplImage*  getCvImage() const { return cvImage; };


    // Draw Image
    //
    virtual void draw( float x, float y );
    virtual void draw( float x, float y, float w, float h );
    virtual void drawWithoutTexture( float x, float y ) {};
    virtual void drawWithoutTexture( float x, float y, float w, float h ) {};    


    // Image Filter Operations
    //
    virtual void erode( );  // based on 3x3 shape
    virtual void dilate( );  // based on 3x3 shape
    virtual void blur( int value=3 );  // value = x*2+1, where x is an integer
    virtual void blurGaussian( int value=3 );  // value = x*2+1, where x is an integer
    virtual void invert();
    virtual void convertToRange(float min, float max);


    // Image Transformation Operations
    //
    //virtual void setROI( int w, int h );  //TODO, support image regions
    virtual void resize( int w, int h ) = 0;
    virtual void scaleIntoMe( const ofxCvImage& mom, int interpolationMethod = CV_INTER_NN ) = 0;
    virtual void mirror( bool bFlipVertically, bool bFlipHorizontally );

    virtual void translate( float x, float y );
    virtual void rotate( float angle, float centerX, float centerY );
    virtual void scale( float scaleX, float sclaeY );
    virtual void transform( float angle, float centerX, float centerY,
                            float scaleX, float scaleY,
                            float moveX, float moveY );
    /**
    * undistort Usage Example:
    * undistort( 0, 1, 0, 0, 200, 200, cwidth/2, cheight/2 );
    * creates kind of an old TV monitor distortion.
    */
    virtual void undistort( float radialDistX, float radialDistY,
                            float tangentDistX, float tangentDistY,
                            float focalX, float focalY,
                            float centerX, float centerY );

    virtual void remap( const IplImage* mapX, const IplImage* mapY );

    virtual void warpPerspective( const ofPoint& A,
                                  const ofPoint& B,
                                  const ofPoint& C,
                                  const ofPoint& D );
    virtual void warpIntoMe( const ofxCvGrayscaleImage& mom,
                             ofPoint src[4], ofPoint dst[4] );



    // Other Image Operations
    //
    virtual int countNonZeroInRegion( int x, int y, int w, int h ) const;




  protected:

    virtual void imageHasChanged();  // call this whenever cvImage is changed
                                     // this flags all the helper images dirty
                                     
    virtual void swapTemp();  // swap cvImageTemp back
                              // to cvImage after an image operation

    IplImage*  cvImage;
    IplImage*  cvImageTemp;   // this is typically swapped back into cvImage
                              // after an image operation with swapImage()

    unsigned char* 	pixels;	  // not width stepped
    bool bPixelsDirty;        // pixels need to be reloaded
    ofTexture  tex;		      // internal tex
    bool bUseTexture;
    bool bTextureDirty;       // texture needs to be reloaded before drawing 

};


#endif
