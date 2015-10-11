/*
* ofxCvFloatImage.h
* by stefanix, zach
*
* This is essentially an IPL_DEPTH_32F with one channel.
* Pixel values are typically from 0-1.0 but this range
* can be set to pretty much any values.
*
*/

#pragma once


#include "ofxCvImage.h"


class ofxCvFloatImage : public ofxCvImage {

    // Float Images and Value Range:
    // Float images are special in that they do not have unequivocal scale.
    // We use a default scale of 0.0 - 1.0
    // In most cases this class does the right conversion when going
    // between different image depth/types
    // In cases you are loading image data to/from float* you might
    // want to pay closer attention to what the scales are.

  public:
	using ofxCvImage::setFromPixels;

    ofxCvFloatImage();
    ofxCvFloatImage( const ofxCvFloatImage& mom );
    // virtual void  allocate( int w, int h );                                //in base class
    virtual void  clear();
	// virtual float getWidth();                                              //in base class
	// virtual float getHeight();                                             //in base class    
    // virtual void  setUseTexture( bool bUse );                              //in base class
    // virtual ofTexture&  getTextureReference();                             //in base class
    virtual void flagImageChanged();
    virtual void  setNativeScale( float _scaleMin, float _scaleMax ); 
    virtual float  getNativeScaleMin() {return scaleMin;}; 
    virtual float  getNativeScaleMax() {return scaleMax;}; 


    // ROI - region of interest
    //
    // virtual void  setROI( int x, int y, int w, int h );                    //in base class
    // virtual void  setROI( ofRectangle& rect );                             //in base class
    // virtual ofRectangle  getROI();                                         //in base class
    // virtual void  resetROI();                                              //in base class
    // virtual ofRectangle  getIntersectionROI( ofRectangle& rec1,      
    //                                          ofRectangle& rec2 );          // inbase class
    

    // Set Pixel Data
    //
    virtual void  set( float value );
    // virtual void  operator -= ( float value );                             //in base class 
    // virtual void  operator += ( float value );                             //in base class     
    virtual void  operator *= ( float scalar );
	virtual void  operator /= ( float scalar );
	      
    virtual void  setFromPixels( const unsigned char* _pixels, int w, int h);
    virtual void  setFromPixels( float * _pixels, int w, int h );  //no scaling
    virtual void  setRoiFromPixels( const unsigned char* _pixels, int w, int h);
    virtual void  setRoiFromPixels( float * _pixels, int w, int h );  //no scaling
    virtual void  operator = ( unsigned char* _pixels );
    virtual void  operator = ( float* _pixels );  //no scaling
    virtual void  operator = ( const ofxCvGrayscaleImage& mom );
    virtual void  operator = ( const ofxCvColorImage& mom );
    virtual void  operator = ( const ofxCvFloatImage& mom );
    virtual void  operator = ( const ofxCvShortImage& mom );
    virtual void  operator = ( const IplImage* mom );     
    
    // virtual void  operator -= ( ofxCvImage& mom );                         //in base class 
    // virtual void  operator += ( ofxCvImage& mom );                         //in base class     
    virtual void  operator *= ( ofxCvImage& mom );
    virtual void  operator &= ( ofxCvImage& mom );  //bit-wise at the moment

	void  addWeighted( ofxCvGrayscaleImage& mom, float f );
	
	
	// Get Pixel Data
	//
    //virtual unsigned char*  getPixels();									   //in base class
    virtual float*  getPixelsAsFloats();  //no scaling                                   //in base class
    virtual ofFloatPixels &		getFloatPixelsRef();
    //virtual unsigned char*  getRoiPixels();								   //in base class
    virtual float*  getRoiPixelsAsFloats();  //no scaling
    virtual ofFloatPixels &		getRoiFloatPixelsRef();
    // virtual IplImage*  getCvImage();

    
    // Draw Image
    //
    //virtual void  draw( float x, float y );                                  //in base class
    //virtual void  draw( float x, float y, float w, float h );                //in base class
    //virtual void  drawROI( float x, float y );                               //in base class
    //virtual void  drawROI( float x, float y, float w, float h );             //in base class
    //virtual void setAnchorPercent( float xPct, float yPct );                 //in base class
    //virtual void setAnchorPoint( int x, int y );                             //in base class
    //virtual void resetAnchor();                                              //in base class    


    // Image Filter Operations
    //
    virtual void  contrastStretch();
    virtual void  convertToRange( float min, float max );    
    // virtual void  erode( );                                                 //in base class
    // virtual void  dilate( );                                                //in base class
    // virtual void  blur( int value=3 );                                      //in base class
    // virtual void  blurGaussian( int value=3 );                              //in base class
    // virtual void  invert();                                                 //in base class
        

    // Image Transformation Operations
    //
    virtual void  resize( int w, int h );
    virtual void  scaleIntoMe( ofxCvImage& mom, int interpolationMethod = CV_INTER_NN);
    // virtual void  mirror( bool bFlipVertically, bool bFlipHorizontally );   //in base class
    // virtual void  translate( float x, float y );                            //in base class
    // virtual void  rotate( float angle, float centerX, float centerY );      //in base class
    // virtual void  scale( float scaleX, float sclaeY );                      //in base class
    // virtual void  transform( float angle, float centerX, float centerY,
    //                          float scaleX, float scaleY,
    //                          float moveX, float moveY );                     //in base class
    // virtual void  undistort( float radialDistX, float radialDistY,
    //                          float tangentDistX, float tangentDistY,
    //                          float focalX, float focalY,
    //                          float centerX, float centerY );                //in base class
    // virtual void  remap( IplImage* mapX, IplImage* mapY );                  //in base class
    // virtual void  warpPerspective( ofPoint& A, ofPoint& B,
    //                                ofPoint& C, ofPoint& D );                //in base class
    // virtual void  warpIntoMe( ofxCvImage& mom,
    //                           ofPoint src[4], ofPoint dst[4] );             //in base class
                             

    // Other Image Operations
    //
    // virtual int  countNonZeroInRegion( int x, int y, int w, int h );        //in base class
    

  protected:
    void allocateTexture();
    void allocatePixels(int w, int h);
    void init();
    virtual void convertFloatToGray( IplImage* floatImg, IplImage* grayImg );
    virtual void convertGrayToFloat( IplImage* grayImg, IplImage* floatImg );
    virtual IplImage* getCv8BitsImage();
    virtual IplImage* getCv8BitsRoiImage();
    
    ofFloatPixels floatPixels;             // not width stepped for getPixelsAsFloats()

    
    bool bFloatPixelsDirty;
    IplImage*  cvGrayscaleImage;    // internal helper grayscale, allocated on demand
    
    float scaleMin;
    float scaleMax;
    
};
