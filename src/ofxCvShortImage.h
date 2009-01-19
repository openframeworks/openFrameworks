/*
* ofxCvShortImage.h
* by stefanix
*
* This is essentially an IPL_DEPTH_16U with one channel.
* Pixel values are typically from 0-65535.
*
*/



#ifndef OFX_CV_SHORT_IMAGE_H
#define OFX_CV_SHORT_IMAGE_H


#include "ofxCvImage.h"


class ofxCvShortImage : public ofxCvImage {


  public:

    ofxCvShortImage();
    ofxCvShortImage( const ofxCvShortImage& mom );
    virtual void  allocate( int w, int h );
    virtual void  clear();
    // virtual void  setUseTexture( bool bUse );                              //in base class 


    // Set Pixel Data
    //
    virtual void  set( float value );
    // virtual void  operator -= ( float value );                             //in base class 
    // virtual void  operator += ( float value );                             //in base class     
	      
    virtual void  setFromPixels( unsigned char* _pixels, int w, int h);
    virtual void  setFromPixels( unsigned short int* _pixels, int w, int h );
    virtual void  operator = ( unsigned char* _pixels );
    virtual void  operator = ( unsigned short int* _pixels );  //no scaling
    virtual void  operator = ( const ofxCvGrayscaleImage& mom );
    virtual void  operator = ( const ofxCvColorImage& mom );
    virtual void  operator = ( const ofxCvFloatImage& mom );
    virtual void  operator = ( const ofxCvShortImage& mom );
    
    // virtual void  operator -= ( const ofxCvImage& mom );                   //in base class 
    // virtual void  operator += ( const ofxCvImage& mom );                   //in base class     

	void  addWeighted( ofxCvGrayscaleImage& mom, float f );
	
	
	// Get Pixel Data
	//
    virtual unsigned char*  getPixels();
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

    virtual void convertShortToGray( const IplImage* floatImg, IplImage* grayImg );
    virtual void convertGrayToShort( const IplImage* grayImg, IplImage* floatImg );
    
    IplImage*  cvGrayscaleImage;    // internal helper grayscale;
    
};



#endif


