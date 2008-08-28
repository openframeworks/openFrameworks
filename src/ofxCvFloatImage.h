
#ifndef OFX_CV_FLOAT_IMAGE_H
#define OFX_CV_FLOAT_IMAGE_H


#include "ofxCvImage.h"


class ofxCvFloatImage : public ofxCvImage {


    public:

        ofxCvFloatImage() {};
        ofxCvFloatImage( const ofxCvFloatImage& mom );
        void allocate( int w, int h );
        void setFromPixels( float * _pixels, int w, int h );

        // Set Pixel Data - Arrays
        void operator = ( ofxCvGrayscaleImage& mom );
        void operator = ( ofxCvColorImage& mom );
        void operator = ( ofxCvFloatImage& mom );
        void operator -= ( ofxCvFloatImage& mom );
        void operator += ( ofxCvFloatImage& mom );
        void operator *= ( ofxCvFloatImage& mom );

        void operator -=	( float scalar );
    	void operator +=	( float scalar );
        void operator *=	( float scalar );
    	void operator /=    ( float scalar );

        void addWeighted( ofxCvGrayscaleImage& mom, float f );

        //------------------------------------------------------
        // scale min and scale max are used to convert from float to unsigned char...
        // are needed, since we need to convert floating point values into pixel values (0-255)
        // default values convert from 0-255, but if you has floating point data between 0 and 1,
        // you could pass in 0, 1, etc...

		void 	draw( float x, float y ) { drawWithScale(x,y,0.0f,255.0f); };
		void 	draw( float x, float y, float w, float h ) { drawWithScale(x,y,w,h,0.0f,255.0f); };
        void 	drawWithScale( float x, float y, float scaleMin, float scaleMax);
        void 	drawWithScale( float x, float y, float w, float h, float scaleMin, float scaleMax);

        unsigned char*  	getPixels(float scaleMin = 0, float scaleMax = 255);
        float * 			getPixelsAsFloats();
        //------------------------------------------------------

        void resize( int w, int h );
        void scaleIntoMe( ofxCvFloatImage& mom, int interpolationMethod = CV_INTER_NN);


    private :

        float * pixelsAsFloats; // not width stepped.
        IplImage*  cvGrayscaleImage;     // internal IPL grayscale;
};



#endif


