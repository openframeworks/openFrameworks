
#include "ofxCvImage.h"
#include "ofxCvGrayscaleImage.h"
#include "ofxCvColorImage.h"
#include "ofxCvFloatImage.h"
#include "ofxCvBlob.h"
#include "ofConstants.h"




//--------------------------------------------------------------------------------
ofxCvImage::ofxCvImage() {
    width			= 0;
    height			= 0;
    bUseTexture		= true;
    bTextureDirty   = true;
	bAllocated		= false;
    bPixelsDirty    = true;
    bRoiPixelsDirty = true;
    cvImageTemp = nullptr;
    bAnchorIsPct = false;
    cvImage = nullptr;
    ipldepth = 0;
    iplchannels = 0;
}

//--------------------------------------------------------------------------------
ofxCvImage::~ofxCvImage() {
    clear();
}

//--------------------------------------------------------------------------------
void ofxCvImage::allocate( int w, int h ) {
	if (bAllocated == true){
		ofLogVerbose("ofxCvImage") << "allocate(): reallocating";
		clear();
	}
	
	if( w == 0 || h == 0 ){
		ofLogError("ofxCvImage") << "allocate(): width and height are zero";
		return;
	}

	cvImage = cvCreateImage( cvSize(w,h), ipldepth, iplchannels );
	cvImageTemp	= cvCreateImage( cvSize(w,h), ipldepth, iplchannels );

	width = w;
	height = h;
	bAllocated = true;

    if( bUseTexture ) {
    	allocatePixels(w,h);
        allocateTexture();
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
        pixels.clear();
        bPixelsDirty = true;
        bRoiPixelsDirty = true;

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
float ofxCvImage::getWidth() const{
	return width;
}

//--------------------------------------------------------------------------------
float ofxCvImage::getHeight() const{
	return height;
}

//--------------------------------------------------------------------------------
void ofxCvImage::setUseTexture( bool bUse ) {
	bUseTexture = bUse;
}

bool ofxCvImage::isUsingTexture() const{
	return bUseTexture;
}

//--------------------------------------------------------------------------------
ofTexture& ofxCvImage::getTexture() {
	return tex;
}

//--------------------------------------------------------------------------------
const ofTexture & ofxCvImage::getTexture() const{
	return tex;
}

//--------------------------------------------------------------------------------
ofTexture& ofxCvImage::getTextureReference() {
	return tex;
}

//--------------------------------------------------------------------------------
const ofTexture & ofxCvImage::getTextureReference() const{
	return tex;
}

//--------------------------------------------------------------------------------
void ofxCvImage::flagImageChanged() {
    bTextureDirty = true;
    bPixelsDirty = true;
    bRoiPixelsDirty = true;
}



// ROI - region of interest


//--------------------------------------------------------------------------------
void ofxCvImage::setROI( int x, int y, int w, int h ) {
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "setROI(): image not allocated";	
		return;
	}
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
ofRectangle ofxCvImage::getROI() const {
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
	if( mom->width == 0 || mom->height == 0 ){
		ofLogError("ofxCvImage") << "operator=: source width and/or height are zero: "
			<< mom->width << " " << mom->height;
		return;
	}
			
	if( !bAllocated || (mom->nChannels == cvImage->nChannels && mom->depth == cvImage->depth) ){
		if( !bAllocated ){ 	//lets allocate if needed
			allocate(mom->width, mom->height);
		}else if( mom->width != width || mom->height != height ){
            ofLogError("ofxCvImage") << "operator=: image size mismatch";
			return;
		}
		cvCopy( mom, cvImage );
        flagImageChanged();
	} else {
        ofLogError("ofxCvImage") << "operator=: image channels and/or depth mismatch";
	}
}

//--------------------------------------------------------------------------------
void ofxCvImage::operator -= ( float value ) {
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "operator-=: image not allocated";
		return;		
	}
	cvSubS( cvImage, cvScalar(value), cvImageTemp );
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::operator += ( float value ) {
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "operator-=: image not allocated";
		return;		
	}
	cvAddS( cvImage, cvScalar(value), cvImageTemp );
	swapTemp();
    flagImageChanged();
}


//--------------------------------------------------------------------------------
void ofxCvImage::operator -= ( ofxCvImage& mom ) {
	if( !mom.bAllocated ){
		ofLogError("ofxCvImage") << "operator-=: source image not allocated";	
		return;	
	}
	if( !bAllocated ){
		ofLogNotice("ofxCvImage") << "operator-=: allocating to match dimensions: "
			<< mom.getWidth() << " " << mom.getHeight();
		allocate(mom.getWidth(), mom.getHeight());
	}

	if( mom.getCvImage()->nChannels == cvImage->nChannels &&
        mom.getCvImage()->depth == cvImage->depth )
    {
        if( matchingROI(getROI(), mom.getROI()) ) {
            cvSub( cvImage, mom.getCvImage(), cvImageTemp );
            swapTemp();
            flagImageChanged();
        } else {
            ofLogError("ofxCvImage") << "operator-=: region of interest mismatch";
        }
	} else {
        ofLogError("ofxCvImage") << "operator-=: image type mismatch";
	}
}

//--------------------------------------------------------------------------------
void ofxCvImage::operator += ( ofxCvImage& mom ) {
	if( !mom.bAllocated ){
		ofLogError("ofxCvImage") << "operator+=: source image not allocated";	
		return;	
	}
	if( !bAllocated ){
		ofLogNotice("ofxCvImage") << "operator+=: allocating to match dimensions: "
			<< mom.getWidth() << " " << mom.getHeight();
		allocate(mom.getWidth(), mom.getHeight());
	}

	if( mom.getCvImage()->nChannels == cvImage->nChannels &&
        mom.getCvImage()->depth == cvImage->depth )
    {
        if( matchingROI(getROI(), mom.getROI()) ) {
            cvAdd( cvImage, mom.getCvImage(), cvImageTemp );
            swapTemp();
            flagImageChanged();
        } else {
            ofLogError("ofxCvImage") << "operator+=: region of interest mismatch";
        }
	} else {
        ofLogError("ofxCvImage") << "operator+=: image type mismatch";
	}
}

//--------------------------------------------------------------------------------
void ofxCvImage::operator *= ( ofxCvImage& mom ) {
	if( !mom.bAllocated ){
		ofLogError("ofxCvImage") << "operator*=: mom needs to be allocated";	
		return;	
	}
	if( !bAllocated ){
		ofLogNotice("ofxCvImage") << "operator*=: allocating to match dimensions: "
			<< mom.getWidth() << " " << mom.getHeight();
		allocate(mom.getWidth(), mom.getHeight());
	}

	if( mom.getCvImage()->nChannels == cvImage->nChannels &&
        mom.getCvImage()->depth == cvImage->depth )
    {
        if( matchingROI(getROI(), mom.getROI()) ) {
            float scalef = 1.0f / 255.0f;
            cvMul( cvImage, mom.getCvImage(), cvImageTemp, scalef );
            swapTemp();
            flagImageChanged();
        } else {
            ofLogError("ofxCvImage") << "operator*=: region of interest mismatch";
        }
	} else {
        ofLogError("ofxCvImage") << "operator*=: images type mismatch";
	}
}

//--------------------------------------------------------------------------------
void ofxCvImage::operator &= ( ofxCvImage& mom ) {
	if( !mom.bAllocated ){
		ofLogError("ofxCvImage") << "operator&=: source image not allocated";	
		return;	
	}
	if( !bAllocated ){
		ofLogNotice("ofxCvImage") << "operator&=: allocating to match dimensions: "
			<< mom.getWidth() << " " << mom.getHeight();
		allocate(mom.getWidth(), mom.getHeight());
	}

	if( mom.getCvImage()->nChannels == cvImage->nChannels &&
        mom.getCvImage()->depth == cvImage->depth )
    {
        if( matchingROI(getROI(), mom.getROI()) ) {
            cvAnd( cvImage, mom.getCvImage(), cvImageTemp );
            swapTemp();
            flagImageChanged();
        } else {
            ofLogError("ofxCvImage") << "operator&=: region of interest mismatch";
        }
	} else {
        ofLogError("ofxCvImage") << "operator&=: images need to have matching type";
	}
}

//--------------------------------------------------------------------------------
void  ofxCvImage::drawBlobIntoMe( ofxCvBlob& blob, int color ) {
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "drawBlobIntoMe(): image not allocated";
		return;
	}
	if( blob.pts.size() > 0 ) {
	   CvPoint* pts = new CvPoint[blob.nPts];
	   for( int i=0; i < blob.nPts ; i++ ) {
		   pts[i].x = (int)blob.pts[i].x;
		   pts[i].y = (int)blob.pts[i].y;
	   }
	   int nPts = blob.nPts;
	   cvFillPoly( cvImage, &pts, &nPts, 1,
	               cvScalar(color,color,color) );
	   delete[] pts;
	}
}


// Draw Image

//--------------------------------------------------------------------------------
void ofxCvImage::draw( float x, float y ) const {
    draw( x,y, width, height );
}

//----------------------------------------------------------
void ofxCvImage::draw(const glm::vec2 & point) const{
	draw(point.x, point.y);
}


//----------------------------------------------------------
void ofxCvImage::draw(const ofRectangle & rect) const{
	draw(rect.x, rect.y, rect.width, rect.height);
}


//--------------------------------------------------------------------------------
void ofxCvImage::updateTexture(){
	if(!bAllocated) {
		ofLogWarning("ofxCvImage") << "updateTexture(): image not allocated";	
	} else if(bUseTexture ) {
		if( bTextureDirty ) {
			tex.loadData( getPixels() );
			bTextureDirty = false;
		}
	}
}

//--------------------------------------------------------------------------------
void ofxCvImage::draw( float x, float y, float w, float h ) const {
    if( bUseTexture ) {
    	ofxCvImage* mutImage = const_cast<ofxCvImage*>(this);
    	mutImage->updateTexture();
        tex.draw(x,y, w,h);
    }
}

//--------------------------------------------------------------------------------
void ofxCvImage::drawROI( float x, float y ) const {
    ofRectangle roi = getROI();
    drawROI( x,y, roi.width, roi.height );
}

//--------------------------------------------------------------------------------
void ofxCvImage::drawROI( float x, float y, float w, float h ) const {
    if( bUseTexture ) {
        ofRectangle roi = getROI();
        if( bTextureDirty ) {
            tex.loadData( getRoiPixels() );
            bTextureDirty = false;
        }

        tex.drawSubsection(x,y, w,h,0,0,roi.width,roi.height);

    } else {
        ofLogError("ofxCvImage") << "drawROI(): textureless drawing mode not implemented";
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
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "dilate(): image not allocated";
		return;		
	}
	cvDilate( cvImage, cvImageTemp, 0, 1 );
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::erode() {
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "erode(): image not allocated";
		return;		
	}
	cvErode( cvImage, cvImageTemp, 0, 1 );
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::blur( int value ) {
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "blur(): image not allocated";
		return;		
	}
    if( value % 2 == 0 ) {
        ofLogNotice("ofxCvImage") << "blur(): value " << value << " not odd, adding 1";
        value++;
    }
	cvSmooth( cvImage, cvImageTemp, CV_BLUR , value);
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::blurGaussian( int value ) {
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "blurGaussian(): image not allocated";
		return;		
	}
    if( value % 2 == 0 ) {
        ofLogNotice("ofxCvImage") << "blurGaussian(): value " << value << " not odd, adding 1";
        value++;
    }
	cvSmooth( cvImage, cvImageTemp, CV_GAUSSIAN ,value );
	swapTemp();
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::invert(){
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "invert(): image not allocated";
		return;		
	}
    cvNot(cvImage, cvImage);
    flagImageChanged();
}




// Image Transformation Operations

//--------------------------------------------------------------------------------
void ofxCvImage::mirror( bool bFlipVertically, bool bFlipHorizontally ) {
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "mirror(): image not allocated";
		return;		
	}	
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
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "translate(): image not allocated";
		return;		
	}	
    transform( 0, 0,0, 1,1, x,y );
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::rotate( float angle, float centerX, float centerY ) {
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "rotate(): image not allocated";
		return;		
	}
    transform( angle, centerX, centerY, 1,1, 0,0 );
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::scale( float scaleX, float scaleY ) {
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "scale(): image not allocated";
		return;		
	}
    transform( 0, 0,0, scaleX,scaleY, 0,0 );
    flagImageChanged();
}

//--------------------------------------------------------------------------------
void ofxCvImage::transform( float angle, float centerX, float centerY,
                            float scaleX, float scaleY,
                            float moveX, float moveY ){

	if( !bAllocated ){
		ofLogError("ofxCvImage") << "transform(): image not allocated";
		return;		
	}	
													
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
							
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "undistort(): image not allocated";
		return;		
	}							
    float camIntrinsics[] = { focalX, 0, centerX, 0, focalY, centerY, 0, 0, 1 };
    float distortionCoeffs[] = { radialDistX, radialDistY, tangentDistX, tangentDistY };
	CvMat _a = cvMat( 3, 3, CV_32F, (void*)camIntrinsics );
    CvMat _k = cvMat( 4, 1, CV_32F, (void*)distortionCoeffs );
#ifdef USE_OLD_CV
    cvUndistort2( cvImage, cvImageTemp, &_a, &_k, 0 );
#else
    cv::Mat src = cv::cvarrToMat(cvImage), dst = cv::cvarrToMat(cvImageTemp);
    cv::Mat A = cv::cvarrToMat(&_a), distCoeffs = cv::cvarrToMat(&_k);

    CV_Assert( src.size() == dst.size() && src.type() == dst.type() );
    cv::undistort( src, dst, A, distCoeffs );
#endif // USE_OLD_CV
	swapTemp();
    flagImageChanged();
}


//--------------------------------------------------------------------------------
void ofxCvImage::remap( IplImage* mapX, IplImage* mapY ) {
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "remap(): image not allocated";
		return;		
	}	
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
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "warpPerspective(): image not allocated";
		return;		
	}
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

    cvGetPerspectiveTransform( cvsrc, cvdst, translate );  // calculate homography
    cvWarpPerspective( cvImage, cvImageTemp, translate );
    swapTemp();
    flagImageChanged();
    cvReleaseMat( &translate );
}



//--------------------------------------------------------------------------------
void ofxCvImage::warpIntoMe( ofxCvImage& mom, const ofPoint src[4], const ofPoint dst[4] ){
    
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "warpIntoMe(): image not allocated";
		return;		
	}
	if( !mom.bAllocated ){
		ofLogError("ofxCvImage") << "warpIntoMe(): source image not allocated";
		return;		
	}
		
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
    	cvGetPerspectiveTransform( cvsrc, cvdst, translate );  // calculate homography
    	cvWarpPerspective( mom.getCvImage(), cvImage, translate);
        flagImageChanged();
    	cvReleaseMat( &translate );

    } else {
        ofLogError("ofxCvImage") << "warpIntoMe(): image type mismatch";
    }
}




// Other Image Operations

//--------------------------------------------------------------------------------
int ofxCvImage::countNonZeroInRegion( int x, int y, int w, int h ) {
	if( !bAllocated ){
		ofLogError("ofxCvImage") << "countNonZeroInRegion(): image not allocated";
		return 0;		
	}
	
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
    if( rec1.width == rec2.width && rec1.height == rec2.height && rec1.width != 0 && rec1.height != 0 ) {
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

//--------------------------------------------------------------------------------
void ofxCvImage::setFromPixels( const ofPixels & pixels ){
	setFromPixels(pixels.getData(),pixels.getWidth(),pixels.getHeight());
}

//--------------------------------------------------------------------------------
void ofxCvImage::setRoiFromPixels( const ofPixels & pixels ){
	setRoiFromPixels(pixels.getData(),pixels.getWidth(),pixels.getHeight());
}

//--------------------------------------------------------------------------------
ofPixels& ofxCvImage::getPixels(){
	if(!bAllocated) {
		ofLogWarning("ofxCvImage") << "getPixels(): image not allocated";
	} else if(bPixelsDirty) {
		IplImage * cv8bit= getCv8BitsImage();

		//Note this possible introduces a bug where pixels doesn't contain the current image.
		//Also it means that modifying the pointer return by get pixels - affects the internal cvImage
		//Where as with the slower way below modifying the pointer doesn't change the image.
		if(  cv8bit->width*cv8bit->nChannels == cvImage->widthStep ){
			pixels.setFromExternalPixels((unsigned char*)cv8bit->imageData,width,height,cv8bit->nChannels);
		}else{
			pixels.setFromAlignedPixels((unsigned char*)cv8bit->imageData,width,height,cv8bit->nChannels,cv8bit->widthStep);
		}
		bPixelsDirty = false;
	}
	return pixels;
}

//--------------------------------------------------------------------------------
ofPixels& ofxCvImage::getRoiPixels(){
	if(!bAllocated) {
		ofLogWarning("ofxCvImage") << "getRoiPixels(): image not allocated";
	} else if(bRoiPixelsDirty) {
		IplImage * cv8bit= getCv8BitsRoiImage();
		ofRectangle roi = getROI();
		unsigned char * roi_ptr = (unsigned char*)cv8bit->imageData + ((int)(roi.y)*cv8bit->widthStep + (int)roi.x * cv8bit->nChannels);
		roiPixels.setFromAlignedPixels(roi_ptr,roi.width,roi.height,cv8bit->nChannels,cv8bit->widthStep);
		bRoiPixelsDirty = false;
	}
	return roiPixels;
}

//--------------------------------------------------------------------------------
const ofPixels& ofxCvImage::getPixels() const{
	ofxCvImage* mutImage = const_cast<ofxCvImage*>(this);
	return mutImage->getPixels();
}

//--------------------------------------------------------------------------------
const ofPixels& ofxCvImage::getRoiPixels() const{
	ofxCvImage* mutImage = const_cast<ofxCvImage*>(this);
	return mutImage->getRoiPixels();
}

//--------------------------------------------------------------------------------
