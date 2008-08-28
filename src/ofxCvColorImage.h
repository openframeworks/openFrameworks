
#ifndef OFX_CV_COLOR_IMAGE_H
#define OFX_CV_COLOR_IMAGE_H

#include "ofxCvImage.h"


class ofxCvColorImage : public ofxCvImage {


  public:

    ofxCvColorImage() {};
    ofxCvColorImage( const ofxCvColorImage& mom );
    void allocate( int w, int h );

    void set(int value);
    void set(int valueR, int valueG, int valueB);

    void convertRgbToHsv();
    void convertHsvToRgb();
    void setFromGrayscalePlanarImages(ofxCvGrayscaleImage& red, ofxCvGrayscaleImage& green, ofxCvGrayscaleImage& blue);
    void convertToGrayscalePlanarImages(ofxCvGrayscaleImage& red, ofxCvGrayscaleImage& green, ofxCvGrayscaleImage& blue);

    void setFromPixels( unsigned char * _pixels, int w, int h );
    void operator = ( unsigned char* _pixels );
    void operator = ( ofxCvGrayscaleImage& mom );
    void operator = ( ofxCvColorImage& mom );
    void operator = ( ofxCvFloatImage& mom );
    void operator -= ( ofxCvColorImage& mom );
    void operator += ( ofxCvColorImage& mom );
    void operator *= ( ofxCvColorImage& mom );

    unsigned char*  getPixels();

    void draw( float x, float y );
    void draw( float x, float y, float w, float h );

    void resize( int w, int h );
    void scaleIntoMe( ofxCvColorImage& mom, int interpolationMethod = CV_INTER_NN);

};



#endif
