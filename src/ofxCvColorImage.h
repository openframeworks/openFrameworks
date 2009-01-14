
#ifndef OFX_CV_COLOR_IMAGE_H
#define OFX_CV_COLOR_IMAGE_H

#include "ofxCvImage.h"


class ofxCvColorImage : public ofxCvImage {


  public:

    ofxCvColorImage() {};
    ofxCvColorImage( const ofxCvColorImage& mom );
    void allocate( int w, int h );
    // virtual void clear();                        //in base class
    // virtual void setUseTexture( bool bUse );     //in base class    


    // Set Pixel Data
    //
    void set( float value );
    void set( int valueR, int valueG, int valueB);
    virtual void operator -= ( float value );
    virtual void operator += ( float value ); 
        
    void setFromPixels( unsigned char * _pixels, int w, int h );
    void setFromGrayscalePlanarImages( const ofxCvGrayscaleImage& red, const ofxCvGrayscaleImage& green, const ofxCvGrayscaleImage& blue );
    void operator = ( unsigned char* _pixels );
    void operator = ( const ofxCvGrayscaleImage& mom );
    void operator = ( const ofxCvColorImage& mom );
    void operator = ( const ofxCvFloatImage& mom );
    
    // virtual void operator -= ( const ofxCvImage& mom );    //in base class 
    // virtual void operator += ( const ofxCvImage& mom );    //in base class 
    // virtual void operator *= ( const ofxCvImage& mom );    //in base class 
    // virtual void operator &= ( const ofxCvImage& mom );    //in base class     


    // Get Pixel Data
    //
    unsigned char*  getPixels();
    void convertToGrayscalePlanarImages( ofxCvGrayscaleImage& red, ofxCvGrayscaleImage& green, ofxCvGrayscaleImage& blue );
    // virtual IplImage*  getCvImage() const { return cvImage; };    //in base class
    

    // Draw Image
    //
    void draw( float x, float y );
    void draw( float x, float y, float w, float h );


    // Image Filter Operations
    //
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
    void convertRgbToHsv();
    void convertHsvToRgb();
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

};



#endif
