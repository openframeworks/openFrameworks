/*
* ofxCvShortImage.h
* by stefanix
*
* This is essentially an IPL_DEPTH_16U with one channel.
* Pixel values are typically from 0-65535.
*
*/


#pragma once


#include "ofxCvImage.h"


class ofxCvShortImage : public ofxCvImage {


  public:

    ofxCvShortImage();
    ofxCvShortImage( const ofxCvShortImage& mom );
    // virtual void  allocate( int w, int h );                                //in base class
    virtual void  clear();
    virtual void  flagImageChanged();
	//virtual float getWidth();                                               //in base class
	//virtual float getHeight();                                              //in base class    
    // virtual void  setUseTexture( bool bUse );                              //in base class 


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
	      
    virtual void  setFromPixels( const unsigned char* _pixels, int w, int h);
    virtual void  setRoiFromPixels( const unsigned char* _pixels, int w, int h);
    virtual void  operator = ( unsigned char* _pixels );
    virtual void  operator = ( const ofxCvGrayscaleImage& mom );
    virtual void  operator = ( const ofxCvColorImage& mom );
    virtual void  operator = ( const ofxCvFloatImage& mom );
    virtual void  operator = ( const ofxCvShortImage& mom );
    virtual void  operator = ( const IplImage* mom );        
    
    // virtual void  operator -= ( ofxCvImage& mom );                         //in base class 
    // virtual void  operator += ( ofxCvImage& mom );                         //in base class     

	void  addWeighted( ofxCvGrayscaleImage& mom, float f );
	
	
	// Get Pixel Data
	//
    //virtual unsigned char*  getPixels();                                     //in base class
	//virtual ofPixelsRef		getPixelsRef();                                //in base class
    //virtual unsigned char*  getRoiPixels();                                  //in base class
    // virtual IplImage*  getCvImage();                                        //in base class

    
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
    // virtual int  countNonZeroInRegion( int x, int y, int w, int h );  //in base class
    
    ofShortPixels & getShortPixelsRef();
    ofShortPixels & getRoiShortPixelsRef();

  protected:
    
    void init();
    virtual void convertShortToGray( IplImage* floatImg, IplImage* grayImg );
    virtual void convertGrayToShort( IplImage* grayImg, IplImage* floatImg );
    virtual IplImage*  getCv8BitsImage();
    virtual IplImage*  getCv8BitsRoiImage();
    
    IplImage*  cvGrayscaleImage;    // internal helper grayscale, allocated on demand
    bool cvGrayscaleDirty;
    bool bShortPixelsDirty;

    ofShortPixels shortPixels;
};
