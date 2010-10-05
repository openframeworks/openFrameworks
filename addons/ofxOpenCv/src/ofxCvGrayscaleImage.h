/*
* ofxCvGrayscaleImage.h
* by stefanix, zach
*
* This is essentially an IPL_DEPTH_8U with one channel.
* Pixel values are typically from 0-255.
*
*/


#ifndef OFX_CV_GRAYSCALE_IMAGE_H
#define OFX_CV_GRAYSCALE_IMAGE_H

#include "ofxCvImage.h"
#include "ofxCvBlob.h"



class ofxCvGrayscaleImage : public ofxCvImage {


  public:

    ofxCvGrayscaleImage();
    ofxCvGrayscaleImage( const ofxCvGrayscaleImage& mom );
    // virtual void  allocate( int w, int h );                                //in base class
    // virtual void  clear();                                                 //in base class
	//virtual float getWidth();                                               //in base class
	//virtual float getHeight();                                              //in base class
    // virtual void  setUseTexture( bool bUse );                              //in base class
    // virtual ofTexture&  getTextureReference();                             //in base class


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
    // virtual void  operator -= ( float value );                              //in base class 
    // virtual void  operator += ( float value );                              //in base class
    
    virtual void  setFromPixels( unsigned char* _pixels, int w, int h );
    virtual void  setRoiFromPixels( unsigned char* _pixels, int w, int h );
    virtual void  operator = ( unsigned char* _pixels );
    virtual void  operator = ( const ofxCvGrayscaleImage& mom );
    virtual void  operator = ( const ofxCvColorImage& mom );
    virtual void  operator = ( const ofxCvFloatImage& mom );
    virtual void  operator = ( const ofxCvShortImage& mom );
    virtual void  operator = ( const IplImage* mom );    

    // virtual void  operator -= ( ofxCvImage& mom );                          //in base class 
    // virtual void  operator += ( ofxCvImage& mom );                          //in base class 
    // virtual void  operator *= ( ofxCvImage& mom );                          //in base class 
    // virtual void  operator &= ( ofxCvImage& mom );                          //in base class 

    virtual void  absDiff( ofxCvGrayscaleImage& mom );
    virtual void  absDiff( ofxCvGrayscaleImage& mom, ofxCvGrayscaleImage& dad );


    // Get Pixel Data
    //
    virtual unsigned char*  getPixels();
    virtual unsigned char*  getRoiPixels();
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
    virtual void  threshold( int value, bool invert=false);
    virtual void  adaptiveThreshold( int blockSize, int offset=0,
                                     bool invert=false, bool gauss=false);
	
	virtual void  brightnessContrast(float brightness, float contrast);		   // (uses values between -1 and 1)
	
    // virtual void  erode( );                                                 //in base class
    // virtual void  dilate( );                                                //in base class
    // virtual void  blur( int value=3 );                                      //in base class
    // virtual void  blurGaussian( int value=3 );                              //in base class
    // virtual void  invert();                                                 //in base class


    // Image Transformation Operations
    //
    virtual void  resize( int w, int h );
    virtual void  scaleIntoMe( ofxCvImage& mom, int interpolationMethod=CV_INTER_NN);
    // virtual void  mirror( bool bFlipVertically, bool bFlipHorizontally );   //in base class
    // virtual void  translate( float x, float y );                            //in base class
    // virtual void  rotate( float angle, float centerX, float centerY );      //in base class
    // virtual void  scale( float scaleX, float sclaeY );                      //in base class
    // virtual void  transform( float angle, float centerX, float centerY,
    //                          float scaleX, float scaleY,
    //                          float moveX, float moveY );                    //in base class
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



  protected:
	
	// 
	// for brightness contrast:
	//
	
	CvMat*		briConLutMatrix;
	
    void init();
    
};



#endif

