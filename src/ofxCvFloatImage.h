
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
        void allocate( int w, int h );


        // Set Pixel Data
        //
        virtual void set( float value );
        void setFromPixels( float * _pixels, int w, int h );
        
        void operator = ( const ofxCvGrayscaleImage& mom );
        void operator = ( const ofxCvColorImage& mom );
        void operator = ( const ofxCvFloatImage& mom );
        
        void operator -= ( const ofxCvFloatImage& mom );
        void operator += ( const ofxCvFloatImage& mom );
        void operator *= ( const ofxCvFloatImage& mom );
        void operator &= ( const ofxCvFloatImage& mom );

        void operator -=	( float scalar );
    	void operator +=	( float scalar );
        void operator *=	( float scalar );
    	void operator /=    ( float scalar );
    	
    	void addWeighted( ofxCvGrayscaleImage& mom, float f );
    	
    	
    	// Get Pixel Data
    	//
        unsigned char*      getPixels() { return getPixels( 0.0f, 255.0f); };
        unsigned char*  	getPixels(float scaleMin, float scaleMax);
        float * 			getPixelsAsFloats();    	

        
        // Draw Image
        //
		void 	draw( float x, float y ) { drawWithScale(x,y,0.0f,255.0f); };
		void 	draw( float x, float y, float w, float h ) { drawWithScale(x,y,w,h,0.0f,255.0f); };
        void 	drawWithScale( float x, float y, float scaleMin, float scaleMax);
        void 	drawWithScale( float x, float y, float w, float h, float scaleMin, float scaleMax);


        // Image Transformation Operations
        //
        void resize( int w, int h );
        void scaleIntoMe( const ofxCvImage& mom, int interpolationMethod = CV_INTER_NN);


    private :

        float * pixelsAsFloats; // not width stepped.
        IplImage*  cvGrayscaleImage;     // internal IPL grayscale;
};



#endif


