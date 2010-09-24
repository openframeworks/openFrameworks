
#include "ofxCvImage.h"
#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include "ofxCvFloatImage.h"
#include "ofxCvBlob.h"





//--------------------------------------------------------------------------------
ofxCvImage::ofxCvImage() {
    width			= 0;
    height			= 0;
    bUseTexture		= true;
    bTextureDirty   = true;
	bAllocated		= false;
	pixels			= NULL;
    bPixelsDirty    = true;
    pixelsWidth     = 0;
    pixelsHeight    = 0;
}

//--------------------------------------------------------------------------------
ofxCvImage::~ofxCvImage() {
    clear();
}

//--------------------------------------------------------------------------------
void ofxCvImage::allocate( int w, int h ) {
	if (bAllocated == true){
		ofLog(OF_LOG_WARNING, "in allocate, reallocating a ofxCvImage");
		clear();
	}

	cvImage = cvCreateImage( cvSize(w,h), ipldepth, iplchannels );
	cvImageTemp	= cvCreateImage( cvSize(w,h), ipldepth, iplchannels );

	width = w;
	height = h;
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

		if( bUseTexture ) {
			tex.clear();
            bTextureDirty = true;
		}

		bAllocated = false;
	}
}

//--------------------------------------------------------------------------------
float ofxCvImage::getWidth(){
	return width;
}

//--------------------------------------------------------------------------------
float ofxCvImage::getHeight(){
	return height;
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
void ofxCvImage::flagImageChanged() {
    bTextureDirty = true;
    bPixelsDirty = true;
}



// ROI - region of interest


//--------------------------------------------------------------------------------
void ofxCvImage::setROI( int x, int y, int w, int h ) {

    x = (int)ofClamp(x, 0, (int)width-1);
    y = (int)ofClamp(y, 0, (int)height-1);
    w = (int)ofClamp(w, 0, (int)width - x);
    h = (int)ofClamp(h, 0, (int)height - y);

    cvSetImageROI( cvImage, cvRect(x,y, w,h) );
    cvSetImageROI( cvImageTemp, cvRect(x,y, w,h) );
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::setROI( const ofRectangle& rect ) {
    setROI( (int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height );
}

//--------------------------------------------------------------------------------
ofRectangle ofxCvImage::getROI() {
    CvRect rect = cvGetImageROI(cvImage);
    return ofRectangle((float)rect.x, (float)rect.y, (float)rect.width, (float)rect.height);
}

//--------------------------------------------------------------------------------
void ofxCvImage::resetROI() {
    cvResetImageROI( cvImage );
    cvResetImageROI( cvImageTemp );
}

//--------------------------------------------------------------------------------
ofRectangle ofxCvImage::getIntersectionROI( const ofRectangle& r1, const ofRectangle& r2 ) {
    // Calculates the intersection rectangle of two overlapping rectangles.
    // The following pattern can be used to do image operation
    // on the intersection (overlapping part) of two ROIs
    //
    // ofRectangle iRoi = img1.getIntersectionROI( img1.getROI(), img2.getROI() );
    // if( iRoi.width > 0 && iRoi.height > 0 ) {
    //     img1.pushROI();
    //     img1.setROI( iRoi );
    //     img2.pushROI();
    //     img2.setROI( iRoi );
    //
    //     // do image operation here
    //
    //     img1.popROI();
    //     img2.popROI();
    // }
    //

    int r1x1 = (int)r1.x;
    int r1y1 = (int)r1.y;
    int r1x2 = (int)(r1.x+r1.width);
    int r1y2 = (int)(r1.y+r1.height);

    int r2x1 = (int)r2.x;
    int r2y1 = (int)r2.y;
    int r2x2 = (int)(r2.x+r2.width);
    int r2y2 = (int)(r2.y+r2.height);

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
        ofLog(OF_LOG_ERROR, "in =, images need to match in size, channels, and depth");
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
        if( matchingROI(getROI(), mom.getROI()) ) {
            cvSub( cvImage, mom.getCvImage(), cvImageTemp );
            swapTemp();
            flagImageChanged();
        } else {
            ofLog(OF_LOG_ERROR, "in -=, ROI mismatch");
        }
	} else {
        ofLog(OF_LOG_ERROR, "in -=, images need to have matching type");
	}
}

//--------------------------------------------------------------------------------
void ofxCvImage::operator += ( ofxCvImage& mom ) {
	if( mom.getCvImage()->nChannels == cvImage->nChannels &&
        mom.getCvImage()->depth == cvImage->depth )
    {
        if( matchingROI(getROI(), mom.getROI()) ) {
            cvAdd( cvImage, mom.getCvImage(), cvImageTemp );
            swapTemp();
            flagImageChanged();
        } else {
            ofLog(OF_LOG_ERROR, "in +=, ROI mismatch");
        }
	} else {
        ofLog(OF_LOG_ERROR, "in +=, images need to have matching type");
	}
}

//--------------------------------------------------------------------------------
void ofxCvImage::operator *= ( ofxCvImage& mom ) {
	if( mom.getCvImage()->nChannels == cvImage->nChannels &&
        mom.getCvImage()->depth == cvImage->depth )
    {
        if( matchingROI(getROI(), mom.getROI()) ) {
            float scalef = 1.0f / 255.0f;
            cvMul( cvImage, mom.getCvImage(), cvImageTemp, scalef );
            swapTemp();
            flagImageChanged();
        } else {
            ofLog(OF_LOG_ERROR, "in *=, ROI mismatch");
        }
	} else {
        ofLog(OF_LOG_ERROR, "in *=, images need to have matching type");
	}
}

//--------------------------------------------------------------------------------
void ofxCvImage::operator &= ( ofxCvImage& mom ) {
	if( mom.getCvImage()->nChannels == cvImage->nChannels &&
        mom.getCvImage()->depth == cvImage->depth )
    {
        if( matchingROI(getROI(), mom.getROI()) ) {
            cvAnd( cvImage, mom.getCvImage(), cvImageTemp );
            swapTemp();
            flagImageChanged();
        } else {
            ofLog(OF_LOG_ERROR, "in &=, ROI mismatch");
        }
	} else {
        ofLog(OF_LOG_ERROR, "in &=, images need to have matching type");
	}
}

//--------------------------------------------------------------------------------
void  ofxCvImage::drawBlobIntoMe( ofxCvBlob& blob, int color ) {
       if( blob.pts.size() > 0 ) {
           CvPoint* pts = new CvPoint[blob.nPts];
           for( int i=0; i < blob.nPts ; i++ ) {
               pts[i].x = (int)blob.pts[i].x;
               pts[i].y = (int)blob.pts[i].y;
           }
           int nPts = blob.nPts;
           cvFillPoly( cvImage, &pts, &nPts, 1,
                       CV_RGB(color,color,color) );
           delete pts;
       }
}


// Draw Image

//--------------------------------------------------------------------------------
void ofxCvImage::draw( float x, float y ) {
    draw( x,y, width, height );
}

//--------------------------------------------------------------------------------
void ofxCvImage::updateTexture(){
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
	}
}

//--------------------------------------------------------------------------------
void ofxCvImage::draw( float x, float y, float w, float h ) {
    if( bUseTexture ) {
    	updateTexture();
        tex.draw(x,y, w,h);
    } else {
        #ifdef TARGET_OPENGLES
            ofLog(OF_LOG_ERROR, "texture-less drawing not supported in OpenGL ES");
        #else
            // this is slower than the typical draw method based on textures
            // but useful when dealing with threads GL textures often don't work
            ofLog(OF_LOG_NOTICE, "in draw, using slow texture-less drawing");
            ofLog(OF_LOG_NOTICE, "texture-less drawing - be aware, unlike texture drawing, \
                              this always draws window aligned, rotation not supported");

            if( x == 0) {
                x += 0.01;
                ofLog(OF_LOG_NOTICE, "BUG: can't draw at x==0 in texture-less mode.");
            }

            if(bAnchorIsPct){
                x -= anchor.x * w;
                y -= anchor.y * h;
            }else{
                x -= anchor.x;
                y -= anchor.y;
            }

            glRasterPos2f( x, y+h );

            IplImage* tempImg;
            tempImg = cvCreateImage( cvSize((int)w, (int)h), ipldepth, iplchannels );
            cvResize( cvImage, tempImg, CV_INTER_NN );
            cvFlip( tempImg, tempImg, 0 );
            glDrawPixels( tempImg->width, tempImg->height ,
                          glchannels, gldepth, tempImg->imageData );
            cvReleaseImage( &tempImg );
        #endif
    }
}

//--------------------------------------------------------------------------------
void ofxCvImage::drawROI( float x, float y ) {
    ofRectangle roi = getROI();
    drawROI( x,y, roi.width, roi.height );
}

//--------------------------------------------------------------------------------
void ofxCvImage::drawROI( float x, float y, float w, float h ) {
    if( bUseTexture ) {
        ofRectangle roi = getROI();
        if( bTextureDirty ) {
            if(tex.getWidth() != roi.width || tex.getHeight() != roi.height) {
                //ROI was changed
                // reallocating texture - this could be faster with ROI support
                tex.clear();
                tex.allocate( (int)roi.width, (int)roi.height, glchannels );
            }
            tex.loadData( getRoiPixels(), (int)roi.width, (int)roi.height, glchannels );
            bTextureDirty = false;
        }

        tex.draw(x,y, w,h);

    } else {
        ofLog(OF_LOG_ERROR, "texture-less drawing not implemented for drawROI");
    }
}

//--------------------------------------------------------------------------------
void ofxCvImage::setAnchorPercent( float xPct, float yPct ){
    if( bUseTexture ) {
    	tex.setAnchorPercent(xPct,yPct);
    } else {
        anchor.x = xPct;
        anchor.y = yPct;

        bAnchorIsPct = true;
    }
}

//--------------------------------------------------------------------------------
void ofxCvImage::setAnchorPoint( float x, float y ){
    if( bUseTexture ) {
    	tex.setAnchorPoint(x,y);
    }else{
        anchor.x = x;
        anchor.y = y;

        bAnchorIsPct = false;
    }
}

//--------------------------------------------------------------------------------
void ofxCvImage::resetAnchor(){
    if( bUseTexture ) {
    	tex.resetAnchor();
    }else{
        anchor.set(0,0);
        bAnchorIsPct = false;
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
        ofLog(OF_LOG_NOTICE, "in blur, value not odd -> will add 1 to cover your back");
        value++;
    }
	cvSmooth( cvImage, cvImageTemp, CV_BLUR , value);
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::blurGaussian( int value ) {
    if( value % 2 == 0 ) {
        ofLog(OF_LOG_NOTICE, "in blurGaussian, value not odd -> will add 1 to cover your back");
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
void ofxCvImage::warpIntoMe( ofxCvImage& mom, const ofPoint src[4], const ofPoint dst[4] ){
    if( mom.getCvImage()->nChannels == cvImage->nChannels &&
        mom.getCvImage()->depth == cvImage->depth ) {

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

    } else {
        ofLog(OF_LOG_ERROR, "in warpIntoMe: mom image type has to match");
    }
}




// Other Image Operations

//--------------------------------------------------------------------------------
int ofxCvImage::countNonZeroInRegion( int x, int y, int w, int h ) {
    //TODO: test this method

	if (w == 0 || h == 0) return 0;
    int count = 0;

    // intersect the global ROI with the region to check
    ofRectangle iRoi = getIntersectionROI( getROI(), ofRectangle(x,y,w,h) );

    ofRectangle lastROI = getROI();
    setROI(iRoi);
	count = cvCountNonZero( cvImage );
    setROI(lastROI);

	return count;
}




// private methods

//--------------------------------------------------------------------------------
void ofxCvImage::swapTemp() {
	if (getROI().x != 0 ||
		getROI().y != 0 ||
		getROI().width != width ||
		getROI().height != height )
    {
		cvCopy( cvImageTemp, cvImage );
	} else {
		IplImage*  temp;
		temp = cvImage;
		cvImage	= cvImageTemp;
		cvImageTemp	= temp;
	}
}

//--------------------------------------------------------------------------------
void ofxCvImage::rangeMap( IplImage* img, float min1, float max1, float min2, float max2 ) {
    // map from min1-max1 to min2-max2
    float scale = (max2-min2)/(max1-min1);
    cvConvertScale( img, img, scale, -(min1*scale)+min2 );
}

//--------------------------------------------------------------------------------
void ofxCvImage::rangeMap( IplImage* mom, IplImage* kid, float min1, float max1, float min2, float max2 ) {
    // map from min1-max1 to min2-max2
    float scale = (max2-min2)/(max1-min1);
    cvConvertScale( mom, kid, scale, -(min1*scale)+min2 );
}

//--------------------------------------------------------------------------------
bool ofxCvImage::matchingROI( const ofRectangle& rec1, const ofRectangle& rec2 ) {
    if( rec1.width == rec2.width && rec1.height == rec2.height ) {
        return true;
    } else {
        return false;
    }
}

//--------------------------------------------------------------------------------
void  ofxCvImage::setImageROI( IplImage* img, const ofRectangle& roi ) {
    cvSetImageROI(img, cvRect((int)roi.x,(int)roi.y,(int)roi.width,(int)roi.height));
}

//--------------------------------------------------------------------------------
void  ofxCvImage::resetImageROI( IplImage* img ) {
    cvResetImageROI(img);
}





