
#ifndef OFX_CV_FLOAT_IMAGE_H
#define OFX_CV_FLOAT_IMAGE_H


#include "ofxCvImage.h"


class ofxCvFloatImage : public ofxCvImage {

    // some methods use scaleMin and scaleMax
    // they are used to convert from float to unsigned char...
    // to get pixel values (0-255) as it's for example necessary for drawing
    // default values convert from 0-255, but if you has floating point data between 0 and 1,
    // you could pass in 0, 1, etc...

  public:

    ofxCvFloatImage() {};
    ofxCvFloatImage( const ofxCvFloatImage& mom );
    virtual void allocate( int w, int h );
    virtual void clear();
    // virtual void setUseTexture( bool bUse );     //in base class          


    // Set Pixel Data
    //
    virtual void set( float value );
    // virtual void operator -= ( float value );    //in base class 
    // virtual void operator += ( float value );    //in base class     
    virtual void operator *= ( float scalar );
	virtual void operator /= ( float scalar );
	      
    virtual void setFromPixels( unsigned char* _pixels, int w, int h ) { setFromPixels(_pixels,w,h,0.0f,255.0f); };
    virtual void setFromPixels( unsigned char* _pixels, int w, int h, float scaleMin, float scaleMax );
    virtual void setFromPixels( float * _pixels, int w, int h );
    virtual void operator = ( unsigned char* _pixels );
    virtual void operator = ( float* _pixels );
    virtual void operator = ( const ofxCvGrayscaleImage& mom );
    virtual void operator = ( const ofxCvColorImage& mom );
    virtual void operator = ( const ofxCvFloatImage& mom );
    
    // virtual void operator -= ( const ofxCvImage& mom );    //in base class 
    // virtual void operator += ( const ofxCvImage& mom );    //in base class     
    virtual void operator *= ( const ofxCvImage& mom );
    virtual void operator &= ( const ofxCvImage& mom );  //bit-wise at the moment

	void addWeighted( ofxCvGrayscaleImage& mom, float f );
	
	
	// Get Pixel Data
	//
    virtual unsigned char*      getPixels() { return getPixels( 0.0f, 255.0f); };
    virtual unsigned char*  	getPixels(float scaleMin, float scaleMax);
    virtual float*  			getPixelsAsFloats();
    // virtual IplImage*  getCvImage() const { return cvImage; };    //in base class 	

    
    // Draw Image
    //
	//virtual void draw( float x, float y ) { drawWithScale(x,y,0.0f,255.0f); };
	//virtual void draw( float x, float y, float w, float h ) { drawWithScale(x,y,w,h,0.0f,255.0f); };
    //virtual void drawWithScale( float x, float y, float scaleMin, float scaleMax);
    //virtual void drawWithScale( float x, float y, float w, float h, float scaleMin, float scaleMax);
    virtual void drawWithoutTexture( float x, float y );
    virtual void drawWithoutTexture( float x, float y, float w, float h );    


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
    virtual void resize( int w, int h );
    virtual void scaleIntoMe( const ofxCvImage& mom, int interpolationMethod = CV_INTER_NN);
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
    

  private:

    virtual void imageHasChanged();
    
    float * pixelsAsFloats; // not width stepped.
    bool bFloatPixelsDirty;
    IplImage*  cvGrayscaleImage;     // internal IPL grayscale;
};



#endif


