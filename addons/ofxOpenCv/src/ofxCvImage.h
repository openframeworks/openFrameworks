/*
* ofxCvImage.h
* by stefanix
*
* This is the common abstract base class of all the different
* image types used in this addon. Methods with "= 0" at the end
* are pure virtual and need to be implemented by the inheriting
* image class.
*
*/

#pragma once


#include "ofxCvConstants.h"

class ofxCvGrayscaleImage;
class ofxCvColorImage;
class ofxCvFloatImage;
class ofxCvShortImage;
class ofxCvBlob;



class ofxCvImage : public ofBaseImage {
    
  public:

    int width;
    int height;
	bool bAllocated;

    ofxCvImage();
    virtual  ~ofxCvImage();
    virtual void  allocate( int w, int h );
    virtual void  clear();
	virtual float getWidth();        // get width of this image or its ROI width
	virtual float getHeight();       // get height of this image or its ROI height
    virtual void  setUseTexture( bool bUse );
    virtual ofTexture&  getTextureReference();
    virtual void flagImageChanged();  //mostly used internally

    
    // ROI - region of interest
    //
    virtual void  setROI( int x, int y, int w, int h );
    virtual void  setROI( const ofRectangle& rect );
    virtual ofRectangle  getROI();
    virtual void  resetROI();
    virtual ofRectangle  getIntersectionROI( const ofRectangle& rec1,
                                             const ofRectangle& rec2 );
    
    
    // Set Pixel Data
    //
    virtual void  set( float value ) = 0;
    virtual void  operator -= ( float value );
    virtual void  operator += ( float value );

    virtual void  setFromPixels( const unsigned char* _pixels, int w, int h ) = 0;
    virtual void  setFromPixels( const ofPixels & pixels );
    virtual void  setRoiFromPixels( const unsigned char* _pixels, int w, int h ) = 0;
    virtual void  setRoiFromPixels( const ofPixels & pixels );
    virtual void  operator = ( const ofxCvGrayscaleImage& mom ) = 0;
    virtual void  operator = ( const ofxCvColorImage& mom ) = 0;
    virtual void  operator = ( const ofxCvFloatImage& mom ) = 0;
    virtual void  operator = ( const ofxCvShortImage& mom ) = 0;
    virtual void  operator = ( const IplImage* mom );
    
    virtual void  operator -= ( ofxCvImage& mom );
    virtual void  operator += ( ofxCvImage& mom );
    virtual void  operator *= ( ofxCvImage& mom );
    virtual void  operator &= ( ofxCvImage& mom );
    
    virtual void  drawBlobIntoMe( ofxCvBlob& blob, int color );
        

    // Get Pixel Data
    //
    virtual unsigned char*  getPixels();
    virtual ofPixelsRef		getPixelsRef();
    virtual unsigned char*  getRoiPixels();
    virtual ofPixelsRef		getRoiPixelsRef();
    virtual IplImage*  getCvImage() { return cvImage; };


    // Draw Image
    //
    virtual void updateTexture();
    virtual void draw( float x, float y );
    virtual void draw( float x, float y, float w, float h );
	virtual void draw(const ofPoint & point);
	virtual void draw(const ofRectangle & rect);
    virtual void drawROI( float x, float y );
    virtual void drawROI( float x, float y, float w, float h );
    virtual void setAnchorPercent( float xPct, float yPct );
    virtual void setAnchorPoint( float x, float y );
    virtual void resetAnchor();
    
    
    // Image Filter Operations
    //
    virtual void  erode( );                     // based on 3x3 shape
    virtual void  dilate( );                    // based on 3x3 shape
    virtual void  blur( int value=3 );          // value = x*2+1, where x is an integer
    virtual void  blurGaussian( int value=3 );  // value = x*2+1, where x is an integer
    virtual void  invert();
    virtual void  contrastStretch() = 0;
    virtual void  convertToRange(float min, float max) = 0;


    // Image Transformation Operations
    //
    virtual void  resize( int w, int h ) = 0;
    virtual void  scaleIntoMe( ofxCvImage& mom, int interpolationMethod = CV_INTER_NN ) = 0;
    virtual void  mirror( bool bFlipVertically, bool bFlipHorizontally );

    virtual void  translate( float x, float y );
    virtual void  rotate( float angle, float centerX, float centerY );
    virtual void  scale( float scaleX, float sclaeY );
    virtual void  transform( float angle, float centerX, float centerY,
                             float scaleX, float scaleY,
                             float moveX, float moveY );
    /**
    * undistort Usage Example:
    * undistort( 0, 1, 0, 0, 200, 200, cwidth/2, cheight/2 );
    * creates kind of an old TV monitor distortion.
    */
    virtual void  undistort( float radialDistX, float radialDistY,
                             float tangentDistX, float tangentDistY,
                             float focalX, float focalY,
                             float centerX, float centerY );

    virtual void  remap( IplImage* mapX, IplImage* mapY );

    virtual void  warpPerspective( const ofPoint& A, const ofPoint& B,
                                   const ofPoint& C, const ofPoint& D );
    virtual void  warpIntoMe( ofxCvImage& mom,
                              const ofPoint src[4], const ofPoint dst[4] );



    // Other Image Operations
    //
    virtual int  countNonZeroInRegion( int x, int y, int w, int h );




  protected:

    bool matchingROI( const ofRectangle& rec1, const ofRectangle& rec2 );
    virtual void  setImageROI( IplImage* img, const ofRectangle& rect );
    virtual void  resetImageROI( IplImage* img );

    virtual void  rangeMap( IplImage* img, float min1, float max1, float min2, float max2 );
    virtual void  rangeMap( IplImage* mom, IplImage* kid, float min1, float max1, float min2, float max2 );
                                     
    virtual void swapTemp();  // swap cvImageTemp back
                              // to cvImage after an image operation
    virtual IplImage*  getCv8BitsImage() { return cvImage; }
    virtual IplImage*  getCv8BitsRoiImage() { return cvImage; }
                          
    IplImage*  cvImage;
    IplImage*  cvImageTemp;   // this is typically swapped back into cvImage
                              // after an image operation with swapImage()
                              
    int ipldepth;             // IPL_DEPTH_8U, IPL_DEPTH_16U, IPL_DEPTH_32F, ...
    int iplchannels;          // 1, 3, 4, ...

    int gldepth;              // GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_FLOAT, ...
    int glchannels;           // GL_LUMINANCE, GL_RGB, GL_RGBA, ...
    
    ofPixels pixels;	  // not width stepped for getPixels(), allocated on demand
    ofPixels roiPixels;
    bool bPixelsDirty;        // pixels need to be reloaded
    bool bRoiPixelsDirty;        // pixels need to be reloaded
    
    ofTexture  tex;		      // internal tex
    bool bUseTexture;
    bool bTextureDirty;       // texture needs to be reloaded before drawing
    
    ofPoint  anchor;
    bool  bAnchorIsPct;    

};
