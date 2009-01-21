/*
* ofxCvFloatImage.h
* by stefanix, zach
*
* This is essentially an IPL_DEPTH_8U with three channels.
* Pixel values are typically from 0-255 for each channel.
*
*/


#ifndef OFX_CV_COLOR_IMAGE_H
#define OFX_CV_COLOR_IMAGE_H

#include "ofxCvImage.h"


class ofxCvColorImage : public ofxCvImage {


  public:

    ofxCvColorImage();
    ofxCvColorImage( const ofxCvColorImage& mom );
    // virtual void  allocate( int w, int h );                                //in base class
    virtual void  clear();
    // virtual void  setUseTexture( bool bUse );                              //in base class    
    // virtual ofTexture&  getTextureReference();                             //in base class    


    // ROI - region of interest
    //
    // virtual void  pushROI();                                               //in base class
    // virtual void  popROI();                                                //in base class
    // virtual void  setROI( int x, int y, int w, int h );                    //in base class
    // virtual void  setROI( const ofRectangle& rect );                       //in base class
    // virtual ofRectangle  getROI();                                         //in base class
    // virtual void  resetROI();                                              //in base class
    // virtual ofRectangle  getIntersectionROI( const ofRectangle& rec1,      
    //                                  const ofRectangle& rec2 ) const;      // inbase class
    

    // Set Pixel Data
    //
    virtual void  set( float value );
    virtual void  set( int valueR, int valueG, int valueB);
    virtual void  operator -= ( float value );
    virtual void  operator += ( float value ); 
        
    virtual void  setFromPixels( unsigned char * _pixels, int w, int h );
    virtual void  setFromGrayscalePlanarImages( const ofxCvGrayscaleImage& red, const ofxCvGrayscaleImage& green, const ofxCvGrayscaleImage& blue );
    virtual void  operator = ( unsigned char* _pixels );
    virtual void  operator = ( const ofxCvGrayscaleImage& mom );
    virtual void  operator = ( const ofxCvColorImage& mom );
    virtual void  operator = ( const ofxCvFloatImage& mom );
    
    // virtual void  operator -= ( const ofxCvImage& mom );                   //in base class 
    // virtual void  operator += ( const ofxCvImage& mom );                   //in base class 
    // virtual void  operator *= ( const ofxCvImage& mom );                   //in base class 
    // virtual void  operator &= ( const ofxCvImage& mom );                   //in base class     


    // Get Pixel Data
    //
    virtual unsigned char*  getPixels();
    virtual void  convertToGrayscalePlanarImages( ofxCvGrayscaleImage& red, ofxCvGrayscaleImage& green, ofxCvGrayscaleImage& blue );
    // virtual IplImage*  getCvImage() const { return cvImage; };             //in base class
    

    // Draw Image
    //
    //virtual void  draw( float x, float y );                                 //in base class
    //virtual void  draw( float x, float y, float w, float h );               //in base class
    

    // Image Filter Operations
    //
    virtual void  contrastStretch();    // not yet implemented for this type
    virtual void  convertToRange( float min, float max );   
    // virtual void  erode( );                                                //in base class
    // virtual void  dilate( );                                               //in base class
    // virtual void  blur( int value=3 );                                     //in base class
    // virtual void  blurGaussian( int value=3 );                             //in base class
    // virtual void  invert();                                                //in base class
    

    // Image Transformation Operations
    //
    virtual void  resize( int w, int h );
    virtual void  scaleIntoMe( const ofxCvImage& mom, int interpolationMethod = CV_INTER_NN);
    virtual void  convertRgbToHsv();
    virtual void  convertHsvToRgb();
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
  
    IplImage*  cvGrayscaleImage;    // internal helper grayscale, allocated on demand
      
};



#endif
