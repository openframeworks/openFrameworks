/*
* ofxCvShortImage.h
* by stefanix, zach
*
* This is essentially an IPL_DEPTH_32F with one channel.
* Pixel values are typically from 0-1.0 but this range
* can be set to pretty much any values.
*
*/


#ifndef OFX_CV_FLOAT_IMAGE_H
#define OFX_CV_FLOAT_IMAGE_H


#include "ofxCvImage.h"


class ofxCvFloatImage : public ofxCvImage {

    // Float Images and Value Range
    // Float images are special in that they do not have unequivocal scale.
    // We use a default scale of 0.0 - 1.0
    // In most cases this class does the right conversion when going
    // between different image depth/types
    // In cases you are loading image data to/from float* you might
    // want to pay closer attention to what the scales are.

  public:

    ofxCvFloatImage();
    ofxCvFloatImage( const ofxCvFloatImage& mom );
    virtual void  allocate( int w, int h );
    virtual void  clear();
    // virtual void  setUseTexture( bool bUse );                              //in base class
    virtual void  setNativeScale( float _scaleMin, float _scaleMax );  


    // Set Pixel Data
    //
    virtual void  set( float value );
    // virtual void  operator -= ( float value );                             //in base class 
    // virtual void  operator += ( float value );                             //in base class     
    virtual void  operator *= ( float scalar );
	virtual void  operator /= ( float scalar );
	      
    virtual void  setFromPixels( unsigned char* _pixels, int w, int h);
    virtual void  setFromPixels( float * _pixels, int w, int h );  //no scaling
    virtual void  operator = ( unsigned char* _pixels );
    virtual void  operator = ( float* _pixels );  //no scaling
    virtual void  operator = ( const ofxCvGrayscaleImage& mom );
    virtual void  operator = ( const ofxCvColorImage& mom );
    virtual void  operator = ( const ofxCvFloatImage& mom );
    
    // virtual void  operator -= ( const ofxCvImage& mom );                   //in base class 
    // virtual void  operator += ( const ofxCvImage& mom );                   //in base class     
    virtual void  operator *= ( const ofxCvImage& mom );
    virtual void  operator &= ( const ofxCvImage& mom );  //bit-wise at the moment

	void  addWeighted( ofxCvGrayscaleImage& mom, float f );
	
	
	// Get Pixel Data
	//
    virtual unsigned char*  getPixels();
    virtual float*  getPixelsAsFloats();  //no scaling
    // virtual IplImage*  getCvImage() const { return cvImage; };             //in base class 	

    
    // Draw Image
    //
    virtual void  drawWithoutTexture( float x, float y );
    virtual void  drawWithoutTexture( float x, float y, float w, float h );    


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
    virtual void  scaleIntoMe( const ofxCvImage& mom, int interpolationMethod = CV_INTER_NN);
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
    // virtual void  warpPerspective( const ofPoint& A,
    //                                const ofPoint& B,
    //                                const ofPoint& C,
    //                                const ofPoint& D );                      //in base class
    // virtual void  warpIntoMe( const ofxCvGrayscaleImage& mom,
    //                           ofPoint src[4], ofPoint dst[4] );             //in base class
                             

    // Other Image Operations
    //
    // virtual int  countNonZeroInRegion( int x, int y, int w, int h ) const;  //in base class
    

  private:

    virtual void imageHasChanged();
    virtual void convertFloatToGray( const IplImage* floatImg, IplImage* grayImg );
    virtual void convertGrayToFloat( const IplImage* grayImg, IplImage* floatImg );
    
    float* pixelsAsFloats;          // not width stepped.
    bool bFloatPixelsDirty;
    IplImage*  cvGrayscaleImage;    // internal helper grayscale;
    
    float scaleMin;
    float scaleMax;
    
};



#endif


