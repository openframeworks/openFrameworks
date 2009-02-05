
#include "ofxCvImage.h"
#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include "ofxCvFloatImage.h"



//--------------------------------------------------------------------------------
ofxCvImage::ofxCvImage() {
    width			= 0;
    height			= 0;
    roiX            = 0;
    roiY            = 0;
    bUseTexture		= true;
    bTextureDirty   = true;
	bAllocated		= false;
	pixels			= NULL;
    bPixelsDirty    = true;
    pixelsWidth     = 0;
    pixelsHeight    = 0;
    bUseRoiOffsetWhenDrawing = false;
}

//--------------------------------------------------------------------------------
ofxCvImage::~ofxCvImage() {
    clear();
}

//--------------------------------------------------------------------------------
void ofxCvImage::allocate( int w, int h ) {
	if (bAllocated == true){
		ofLog(OF_WARNING, "in allocate, reallocating a ofxCvImage");
		clear();
	}
    
	cvImage = cvCreateImage( cvSize(w,h), ipldepth, iplchannels );
	cvImageTemp	= cvCreateImage( cvSize(w,h), ipldepth, iplchannels );

	width = w;
	height = h;
    roiStack.push_back( ofRectangle(0,0,w,h) );   // set ROI to full image
	bAllocated = true;

    if( bUseTexture ) {
        tex.allocate( width, height, glchannels );
        bTextureDirty = true;
    }
}

//--------------------------------------------------------------------------------
void ofxCvImage::clear() {

	if (bAllocated == true){
		if (width > 0 && height > 0){
			cvReleaseImage( &cvImage );
			cvReleaseImage( &cvImageTemp );
		}
        if( pixels != NULL ) {
            delete pixels;
            pixels = NULL;
            bPixelsDirty = true;
            pixelsWidth = 0;
            pixelsHeight = 0;            
        }
		width = 0;
		height = 0;
        roiX = 0;    
        roiY = 0;
        roiStack.clear(); 

		if( bUseTexture ) {
			tex.clear();
            bTextureDirty = true;
		}
		
		bAllocated = false;
	}
}

//--------------------------------------------------------------------------------
void ofxCvImage::setUseTexture( bool bUse ) {
	bUseTexture = bUse;
}

//--------------------------------------------------------------------------------
ofTexture& ofxCvImage::getTextureReference() {
	return tex;
}

//--------------------------------------------------------------------------------
void ofxCvImage::swapTemp() {
	IplImage*  temp;
	temp = cvImage;
	cvImage	= cvImageTemp;
	cvImageTemp	= temp;
}

//--------------------------------------------------------------------------------
void ofxCvImage::flagImageChanged() {
    bTextureDirty = true;
    bPixelsDirty = true;
}

//--------------------------------------------------------------------------------
void ofxCvImage::setUseRoiOffsetWhenDrawing( bool bUse ) {
    bUseRoiOffsetWhenDrawing = bUse;
}

//--------------------------------------------------------------------------------
void ofxCvImage::rangeMap( IplImage* img, float min1, float max1, float min2, float max2 ) {
    // map from min1-max1 to min2-max2
    float scale = (max2-min2)/(max1-min1);
    cvConvertScale( img, img, scale, -(min1*scale)+min2 );
}

//--------------------------------------------------------------------------------
bool ofxCvImage::pushSetBothToTheirIntersectionROI( ofxCvImage& img1, ofxCvImage& img2 ) {
    // calculates intersection ROI
    // pushes the intersection ROI on both the images' roiStack
    // use popROI() to restore previous ROI
    ofRectangle iRoi = getIntersectionROI( img1.getROI(), img2.getROI() );
    if( iRoi.width > 0 && iRoi.height > 0 ) { 
        img1.pushROI();
        img1.setROI( iRoi );
        img2.pushROI();
        img2.setROI( iRoi );            
        return true;
    } else {
        return false;
    }
}


// ROI - region of interest

//--------------------------------------------------------------------------------
void ofxCvImage::pushROI() {
    roiStack.push_back( ofRectangle() );
    setROI(0,0,cvImage->width,cvImage->height);
    
}

//--------------------------------------------------------------------------------
void ofxCvImage::popROI() {
    if(roiStack.size() > 1) {
        roiStack.pop_back();
    } else {
        ofLog(OF_WARNING, "in popROI, not popping since there is only one element left.");
    }
    setROI( roiStack.back() );
}

//--------------------------------------------------------------------------------
void ofxCvImage::setROI( int x, int y, int w, int h ) {
    
    x = ofClamp(x, 0, cvImage->width-1);
    y = ofClamp(y, 0, cvImage->height-1);
    w = ofClamp(w, 0, cvImage->width - x);
    h = ofClamp(h, 0, cvImage->height - y);
    
    cvSetImageROI( cvImage, cvRect(x,y, w,h) );
    cvSetImageROI( cvImageTemp, cvRect(x,y, w,h) );
    width = w;
    height = h;
    roiX = x;
    roiY = y;
    roiStack.back().x = x;
    roiStack.back().y = y;
    roiStack.back().width = w;
    roiStack.back().height = h;    
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::setROI( const ofRectangle& rect ) {
    setROI( rect.x, rect.y, rect.width, rect.height );
}

//--------------------------------------------------------------------------------
ofRectangle ofxCvImage::getROI() {
    return roiStack.back();
}

//--------------------------------------------------------------------------------
void ofxCvImage::resetROI() {
    cvResetImageROI( cvImage );
    cvResetImageROI( cvImageTemp );
    width = cvImage->width;
    height = cvImage->height;
    roiX = 0;
    roiY = 0;
    setROI( 0,0, cvImage->width, cvImage->height );
}

//--------------------------------------------------------------------------------
ofRectangle ofxCvImage::getIntersectionROI( const ofRectangle& r1, const ofRectangle& r2 ) {
    int r1x1 = r1.x;
    int r1y1 = r1.y;
    int r1x2 = r1.x+r1.width;
    int r1y2 = r1.y+r1.height;

    int r2x1 = r2.x;
    int r2y1 = r2.y;
    int r2x2 = r2.x+r2.width;
    int r2y2 = r2.y+r2.height;

    int r3x1 = 0;
    int r3y1 = 0;
    int r3x2 = 0;
    int r3y2 = 0;
    
    bool bIntersect =  ( ( ofInRange(r2x1, r1x1,r1x2) || ofInRange(r1x1, r2x1,r2x2) ) &&
                         ( ofInRange(r2y1, r1y1,r1y2) || ofInRange(r1y1, r2y1,r2y2) ) );

    if( bIntersect ){
        r3x1 = MAX( r1x1, r2x1 );
        r3y1 = MAX( r1y1, r2y1 );

        r3x2 = MIN( r1x2, r2x2 );
        r3y2 = MIN( r1y2, r2y2 );
    
        return ofRectangle( r3x1,r3y1, r3x2-r3x1,r3y2-r3y1 );
        
    } else {
        return ofRectangle( 0,0, 0,0 );
    }
}




// Set Pixel Data

//--------------------------------------------------------------------------------
void  ofxCvImage::operator = ( const IplImage* mom ) {
	if( mom->width == width && mom->height == height &&
	    mom->nChannels == cvImage->nChannels && 
        mom->depth == cvImage->depth )
    {
		cvCopy( mom, cvImage );
        flagImageChanged();
	} else {
        ofLog(OF_ERROR, "in =, images need to match in size, channels, and depth");
	}
}

//--------------------------------------------------------------------------------
void ofxCvImage::operator -= ( float value ) {
	cvSubS( cvImage, cvScalar(value), cvImageTemp );
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::operator += ( float value ) {
	cvAddS( cvImage, cvScalar(value), cvImageTemp );
	swapTemp();
    flagImageChanged();
}


//--------------------------------------------------------------------------------
void ofxCvImage::operator -= ( ofxCvImage& mom ) {
	if( mom.getCvImage()->nChannels == cvImage->nChannels && 
        mom.getCvImage()->depth == cvImage->depth )
    {
        if( pushSetBothToTheirIntersectionROI(*this,mom) ) {
            cvSub( cvImage, mom.getCvImage(), cvImageTemp );
            swapTemp();
            popROI();       //restore prevoius ROI
            mom.popROI();   //restore prevoius ROI              
            flagImageChanged();
        } else {
            ofLog(OF_ERROR, "in -=, ROI mismatch");
        }
	} else {
        ofLog(OF_ERROR, "in -=, images need to have matching type");
	}
}

//--------------------------------------------------------------------------------
void ofxCvImage::operator += ( ofxCvImage& mom ) {
	if( mom.getCvImage()->nChannels == cvImage->nChannels && 
        mom.getCvImage()->depth == cvImage->depth )
    {
        if( pushSetBothToTheirIntersectionROI(*this,mom) ) {
            cvAdd( cvImage, mom.getCvImage(), cvImageTemp );
            swapTemp();
            popROI();       //restore prevoius ROI
            mom.popROI();   //restore prevoius ROI              
            flagImageChanged();
        } else {
            ofLog(OF_ERROR, "in +=, ROI mismatch");
        }
	} else {
        ofLog(OF_ERROR, "in +=, images need to have matching type");
	}
}

//--------------------------------------------------------------------------------
void ofxCvImage::operator *= ( ofxCvImage& mom ) {
	if( mom.getCvImage()->nChannels == cvImage->nChannels && 
        mom.getCvImage()->depth == cvImage->depth )
    {
        if( pushSetBothToTheirIntersectionROI(*this,mom) ) {
            float scalef = 1.0f / 255.0f;
            cvMul( cvImage, mom.getCvImage(), cvImageTemp, scalef );
            swapTemp();
            popROI();       //restore prevoius ROI
            mom.popROI();   //restore prevoius ROI              
            flagImageChanged();
        } else {
            ofLog(OF_ERROR, "in *=, ROI mismatch");
        }
	} else {
        ofLog(OF_ERROR, "in *=, images need to have matching type");
	}
}

//--------------------------------------------------------------------------------
void ofxCvImage::operator &= ( ofxCvImage& mom ) {
	if( mom.getCvImage()->nChannels == cvImage->nChannels && 
        mom.getCvImage()->depth == cvImage->depth )
    {
        if( pushSetBothToTheirIntersectionROI(*this,mom) ) {
            cvAnd( cvImage, mom.getCvImage(), cvImageTemp );
            swapTemp();
            popROI();       //restore prevoius ROI
            mom.popROI();   //restore prevoius ROI              
            flagImageChanged();
        } else {
            ofLog(OF_ERROR, "in &=, ROI mismatch");
        }
	} else {
        ofLog(OF_ERROR, "in &=, images need to have matching type");
	}
}



// Draw Image

//--------------------------------------------------------------------------------
void ofxCvImage::draw( float x, float y ) {
    draw( x,y, width,height );
}

//--------------------------------------------------------------------------------
void ofxCvImage::draw( float x, float y, float w, float h ) {    
    if( bUseTexture ) {
        if( bTextureDirty ) {
            if(tex.getWidth() != width || tex.getHeight() != height) {
                //ROI was changed
                // reallocating texture - this could be faster with ROI support
                tex.clear();
                tex.allocate( width, height, glchannels );
            }
            tex.loadData( getPixels(), width, height, glchannels );
            bTextureDirty = false;
        }
        if( bUseRoiOffsetWhenDrawing ){
            tex.draw(x+roiX, y+roiY, w,h);
        } else {
            tex.draw(x,y, w,h);
        }

    } else {
        // this is slower than the typical draw method based on textures
        // but useful when dealing with threads GL textures often don't work
        ofLog(OF_NOTICE, "in draw, using slow texture-less drawing");
        
        if( x == 0) {
            x += 0.01;
            ofLog(OF_NOTICE, "BUG: can't draw at x==0 in texture-less mode.");
        }
        
        glRasterPos3f( x+roiX, y+h+roiY, 0.0 );
        if( bUseRoiOffsetWhenDrawing ){
            glRasterPos3f( x+roiX, y+h+roiY, 0.0 );
        } else {
            glRasterPos3f( x, y+h, 0.0 );
        }        
        IplImage* tempImg;
        tempImg = cvCreateImage( cvSize((int)w, (int)h), ipldepth, iplchannels );        
        cvResize( cvImage, tempImg, CV_INTER_NN );
        cvFlip( tempImg, tempImg, 0 );
        glDrawPixels( tempImg->width, tempImg->height ,
                      glchannels, gldepth, tempImg->imageData );
        cvReleaseImage( &tempImg );
        if( bUseRoiOffsetWhenDrawing ){
            glRasterPos3f( -(x+roiX), -(y+h+roiY), 0.0 ); 
        } else {
            glRasterPos3f( -x, -(y+h), 0.0 ); 
        }         
    }
}




// Image Filter Operations
//--------------------------------------------------------------------------------
void ofxCvImage::dilate() {
	cvDilate( cvImage, cvImageTemp, 0, 1 );
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::erode() {
	cvErode( cvImage, cvImageTemp, 0, 1 );
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::blur( int value ) {
    if( value % 2 == 0 ) {
        ofLog(OF_NOTICE, "in blur, value not odd -> will add 1 to cover your back");
        value++;
    }
	cvSmooth( cvImage, cvImageTemp, CV_BLUR , value);
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::blurGaussian( int value ) {
    if( value % 2 == 0 ) {
        ofLog(OF_NOTICE, "in blurGaussian, value not odd -> will add 1 to cover your back");
        value++;
    }
	cvSmooth( cvImage, cvImageTemp, CV_GAUSSIAN ,value );
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::invert(){
    cvNot(cvImage, cvImage);
    flagImageChanged();
}




// Image Transformation Operations

//--------------------------------------------------------------------------------
void ofxCvImage::mirror( bool bFlipVertically, bool bFlipHorizontally ) {
	int flipMode = 0;

	if( bFlipVertically && !bFlipHorizontally ) flipMode = 0;
	else if( !bFlipVertically && bFlipHorizontally ) flipMode = 1;
	else if( bFlipVertically && bFlipHorizontally ) flipMode = -1;
	else return;

	cvFlip( cvImage, cvImageTemp, flipMode );
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::translate( float x, float y ) {
    transform( 0, 0,0, 1,1, x,y );
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::rotate( float angle, float centerX, float centerY ) {
    transform( angle, centerX, centerY, 1,1, 0,0 );
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::scale( float scaleX, float scaleY ) {
    transform( 0, 0,0, scaleX,scaleY, 0,0 );
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::transform( float angle, float centerX, float centerY,
                            float scaleX, float scaleY,
                            float moveX, float moveY ){
    float sina = sin(angle * DEG_TO_RAD);
    float cosa = cos(angle * DEG_TO_RAD);
    CvMat*  transmat = cvCreateMat( 2,3, CV_32F );
    cvmSet( transmat, 0,0, scaleX*cosa );
    cvmSet( transmat, 0,1, scaleY*sina );
    cvmSet( transmat, 0,2, -centerX*scaleX*cosa - centerY*scaleY*sina + moveX + centerX );
    cvmSet( transmat, 1,0, -1.0*scaleX*sina );
    cvmSet( transmat, 1,1, scaleY*cosa );
    cvmSet( transmat, 1,2, -centerY*scaleY*cosa + centerX*scaleX*sina + moveY + centerY);

    cvWarpAffine( cvImage, cvImageTemp, transmat );
	swapTemp();
    flagImageChanged();

    cvReleaseMat( &transmat );
}

//--------------------------------------------------------------------------------
void ofxCvImage::undistort( float radialDistX, float radialDistY,
                            float tangentDistX, float tangentDistY,
                            float focalX, float focalY,
                            float centerX, float centerY ){
    float camIntrinsics[] = { focalX, 0, centerX, 0, focalY, centerY, 0, 0, 1 };
    float distortionCoeffs[] = { radialDistX, radialDistY, tangentDistX, tangentDistY };
    cvUnDistortOnce( cvImage, cvImageTemp, camIntrinsics, distortionCoeffs, 1 );
	swapTemp();
    flagImageChanged();
}


//--------------------------------------------------------------------------------
void ofxCvImage::remap( IplImage* mapX, IplImage* mapY ) {
    cvRemap( cvImage, cvImageTemp, mapX, mapY );
	swapTemp();
    flagImageChanged();
}




/**
*    A  +-------------+  B
*      /               \
*     /                 \
*    /                   \
* D +-------------------- +  C
*/

//--------------------------------------------------------------------------------
void ofxCvImage::warpPerspective( const ofPoint& A, const ofPoint& B, const ofPoint& C, const ofPoint& D ) { 
    // compute matrix for perspectival warping (homography) 
    CvPoint2D32f cvsrc[4]; 
    CvPoint2D32f cvdst[4]; 
    CvMat* translate = cvCreateMat( 3,3, CV_32FC1 ); 
    cvSetZero( translate ); 

    cvdst[0].x = 0; 
    cvdst[0].y = 0; 
    cvdst[1].x = width; 
    cvdst[1].y = 0; 
    cvdst[2].x = width; 
    cvdst[2].y = height; 
    cvdst[3].x = 0; 
    cvdst[3].y = height; 

    cvsrc[0].x = A.x; 
    cvsrc[0].y = A.y; 
    cvsrc[1].x = B.x; 
    cvsrc[1].y = B.y; 
    cvsrc[2].x = C.x; 
    cvsrc[2].y = C.y; 
    cvsrc[3].x = D.x; 
    cvsrc[3].y = D.y; 

    cvWarpPerspectiveQMatrix( cvsrc, cvdst, translate );  // calculate homography 
    cvWarpPerspective( cvImage, cvImageTemp, translate ); 
    swapTemp();
    flagImageChanged();
    cvReleaseMat( &translate ); 
} 



//--------------------------------------------------------------------------------
void ofxCvImage::warpIntoMe( ofxCvGrayscaleImage& mom, const ofPoint src[4], const ofPoint dst[4] ){
	// compute matrix for perspectival warping (homography)
	CvPoint2D32f cvsrc[4];
	CvPoint2D32f cvdst[4];
	CvMat* translate = cvCreateMat( 3, 3, CV_32FC1 );
	cvSetZero( translate );
	for (int i = 0; i < 4; i++ ) {
		cvsrc[i].x = src[i].x;
		cvsrc[i].y = src[i].y;
		cvdst[i].x = dst[i].x;
		cvdst[i].y = dst[i].y;
	}
	cvWarpPerspectiveQMatrix( cvsrc, cvdst, translate );  // calculate homography
	cvWarpPerspective( mom.getCvImage(), cvImage, translate);
    flagImageChanged();
	cvReleaseMat( &translate );
}




// Other Image Operations

//--------------------------------------------------------------------------------
int ofxCvImage::countNonZeroInRegion( int x, int y, int w, int h ) {
    //TODO: test this method
    
	if (w == 0 || h == 0) return 0;
    int count = 0;
    
    // intersect the global ROI with the region to check
    ofRectangle iRoi = getIntersectionROI( getROI(), ofRectangle(x,y,w,h) );
    
    pushROI();
    setROI(iRoi);

	count = cvCountNonZero( cvImage );
    
    popROI();
        
	return count;
}




