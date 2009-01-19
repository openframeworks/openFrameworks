/*
* ofxCvContourFinder.h
*
* Finds white blobs in binary images and identifies
* centroid, bounding box, area, length and polygonal contour
* The result is placed in a vector of ofxCvBlob objects.
*
*/

#ifndef OFX_CV_CONTOUR_FINDER
#define OFX_CV_CONTOUR_FINDER



#include "ofxCvConstants.h"
#include "ofxCvBlob.h"
#include "ofxCvGrayscaleImage.h"
#include <algorithm>

class ofxCvContourFinder {

  public:
  
    vector<ofxCvBlob>  blobs;
    int                nBlobs;    // how many did we find
      

    ofxCvContourFinder();
    virtual  ~ofxCvContourFinder();
    virtual int  findContours( ofxCvGrayscaleImage& input,
                               int minArea, int maxArea,
                               int nConsidered, bool bFindHoles,
                               bool bUseApproximation = true);
                               // approximation = don't do points for all points 
                               // of the contour, if the contour runs
                               // along a straight line, for example...

    virtual void  draw() { draw (0,0); };
    virtual void  draw( float x, float y );
    //virtual ofxCvBlob  getBlob(int num);



  protected:

    ofxCvGrayscaleImage     inputCopy;
    CvMemStorage*           contour_storage;
    CvMemStorage*           storage;
    CvMoments*              myMoments;
    vector<CvSeq*>          cvSeqBlobs;  //these will become blobs

    virtual void reset();

};



#endif
