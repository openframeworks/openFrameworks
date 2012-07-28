/*
 *  ofxiPhoneImagePicker.cpp
 *  iPhone UIImagePicker Example
 *
 *  Created by Zach Gage on 3/1/09.
 *  Copyright 2009 stfj. All rights reserved.
 *
 */

#import "ofxiPhoneImagePicker.h"

//C++ class implementations

//--------------------------------------------------------------
ofxiPhoneImagePicker::ofxiPhoneImagePicker()
{
	imagePicker = [[ofxiPhoneImagePickerDelegate alloc] initWithPicker:this];
	cameraIsAvailable = [imagePicker isCameraAvailable];
	photoLibraryIsAvailable = [imagePicker isPhotoLibraryAvailable];
	savedPhotosIsAvailable = [imagePicker isSavedPhotosAvailable];
	pixels = NULL;
	maxDimension = 0;
	imageUpdated = false;
	width = 0;
	height = 0;
	[imagePicker setMaxDimension: 640];
	pixelsAllocated=false;
}

//--------------------------------------------------------------
ofxiPhoneImagePicker::~ofxiPhoneImagePicker(){
	[imagePicker release];
	if( pixels != NULL ){
		delete[] pixels;
		pixels = NULL;
	}
}

//--------------------------------------------------------------
void ofxiPhoneImagePicker::setMaxDimension(int _maxDimension)
{
	maxDimension = _maxDimension;
	[imagePicker setMaxDimension: maxDimension];
}

//----------------------------------------------------------------
bool ofxiPhoneImagePicker::openCamera(int camera)
{
	return [imagePicker openCamera:camera];
}
#ifdef __IPHONE_3_1
//----------------------------------------------------------------
bool ofxiPhoneImagePicker::showCameraOverlay() {
	return [imagePicker showCameraOverlay];
}

//----------------------------------------------------------------
bool ofxiPhoneImagePicker::showCameraOverlayWithCustomView(UIView * view) {
    return [imagePicker showCameraOverlayWithCustomView:view];
}

//----------------------------------------------------------------
void ofxiPhoneImagePicker::hideCameraOverlay()
{
	[imagePicker hideCameraOverlay];
}
#endif
//----------------------------------------------------------------
bool ofxiPhoneImagePicker::openLibrary()
{
	return [imagePicker openLibrary];
}
//----------------------------------------------------------------
bool ofxiPhoneImagePicker::openSavedPhotos()
{
	return [imagePicker openSavedPhotos];
}

//----------------------------------------------------------------
void ofxiPhoneImagePicker::close() {
    [imagePicker close];
}

//----------------------------------------------------------------
int ofxiPhoneImagePicker::getOrientation()
{
	switch( [imagePicker getImageOrientation] )
	{
		case UIImageOrientationUp:
			return OFX_IMG_PICKER_UP;
			break;
		case UIImageOrientationDown:
			return OFX_IMG_PICKER_DOWN;
			break;
		case UIImageOrientationLeft:
			return OFX_IMG_PICKER_LEFT;
			break;
		case UIImageOrientationRight:
			return OFX_IMG_PICKER_RIGHT;
			break;
		default:
			return 0;
			break;
			
	}
}
#ifdef __IPHONE_3_1
void ofxiPhoneImagePicker::takePicture()
{
	[imagePicker takePicture];
}
#endif
//----------------------------------------------------------------
void ofxiPhoneImagePicker::loadPixels()
{	
	// load image texture
	CGImageRef photo;
	CGContextRef photoContext;
	GLubyte *photoData;
	
	photo = [imagePicker getCGImage];
	
	int bpp = 4;
	bool mallocPixels = false;
	
	if(width != (int) CGImageGetWidth(photo) || height != (int) CGImageGetHeight(photo))
		mallocPixels = true;
	
	// Get the width and height of the image
	width = (int) CGImageGetWidth(photo);
	height = (int) CGImageGetHeight(photo);
	
	if(photo)
	{
		photoData = (GLubyte *) malloc(width * height * bpp);
		
		if(mallocPixels) {
			if(pixelsAllocated)
				free(pixels);
				
			pixels = (unsigned char *) malloc(width * height * bpp);
			
			pixelsAllocated = true;
		}
		
		photoContext = CGBitmapContextCreate(photoData, width, height, 8, width * bpp, CGImageGetColorSpace(photo), kCGImageAlphaPremultipliedLast);
		
		CGContextDrawImage(photoContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), photo);
		
		memcpy(pixels, photoData, width*height*4);
		
		CGContextRelease(photoContext);
		free(photoData);
	}
	
	imageUpdated = true;
	
}
//----------------------------------------------------------------
void ofxiPhoneImagePicker::saveImage()
{
	[imagePicker saveImageToPhotoAlbum];
}
//----------------------------------------------------------------


// CLASS IMPLEMENTATIONS--------------objc------------------------
//----------------------------------------------------------------
@implementation ofxiPhoneImagePickerDelegate

- (id) initWithPicker:(canLoadPixels *) _picker
{
	if(self = [super init])
	{
        photoLibraryIsAvailable = [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary];
        cameraIsAvailable       = [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera];
        savedPhotosIsAvailable  = [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeSavedPhotosAlbum];
        
        CGSize screenSize = [UIScreen mainScreen].bounds.size;
		
		_imagePicker = [[UIImagePickerController alloc] init];
        _imagePicker.delegate = self;
        
        if([UIScreen mainScreen].bounds.size.width == 768 &&    // check if its an ipad.
           [UIScreen mainScreen].bounds.size.height == 1024) {  // this is not future proof!
            /**
             *  on the ipad, the image picker view needs to be rotated to match the orientation
             *  of the device, otherwise the camera image is not straight.
             */
            UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
            if(orientation == UIInterfaceOrientationLandscapeLeft) {
                _imagePicker.view.transform = CGAffineTransformMakeRotation(-PI*0.5);
            } else if(orientation == UIInterfaceOrientationLandscapeRight) {
                _imagePicker.view.transform = CGAffineTransformMakeRotation(PI*0.5);
            } else if(orientation == UIInterfaceOrientationPortraitUpsideDown) {
                _imagePicker.view.transform = CGAffineTransformMakeRotation(PI);
            }
        }
	
		cppPixelLoader = _picker;
	}
	return self;
}

//--------------------------------------------------------------
- (void)dealloc { 

    _imagePicker.delegate = nil;
    _imagePicker.cameraOverlayView = nil;
	[_imagePicker.view removeFromSuperview];
	[_imagePicker release];
	
    if(_image) {
        [_image release];
        _image = nil;
    }
    
    if(overlay) {
        overlay.delegate = nil;
        [overlay release];
        overlay = nil;
    }
    
    cppPixelLoader = NULL;
	
	[super dealloc];
}

//----------------------------------------------------------------
- (void) imagePickerController:(UIImagePickerController *)picker 
         didFinishPickingImage:(UIImage *)image 
                   editingInfo:(NSDictionary *)editingInfo {
    
    _image = [[self scaleAndRotateImage:image] retain];
	cppPixelLoader->loadPixels();
}

#ifdef __IPHONE_3_1
- (void) imagePickerController:(UIImagePickerController *)picker 
 didFinishPickingMediaWithInfo:(NSDictionary *)info {

    _image = [[self scaleAndRotateImage:[info objectForKey:UIImagePickerControllerOriginalImage]] retain];
	cppPixelLoader->loadPixels();
}

//--------------------------------------------------------------
- (void) takePicture {
	[_imagePicker takePicture];
}
#endif

//--------------------------------------------------------------
- (void) imagePickerControllerDidCancel:(UIImagePickerController *)picker {
	[self close];
}

//--------------------------------------------------------------
- (void) saveImageToPhotoAlbum {
	UIImageWriteToSavedPhotosAlbum(_image, nil, nil, nil);
	printf("saved!");
}

//--------------------------------------------------------------
- (BOOL) openCamera:(int)camera {
	if(cameraIsAvailable) {		
		_imagePicker.sourceType = UIImagePickerControllerSourceTypeCamera;
		if(camera == 0) {
			_imagePicker.cameraDevice = UIImagePickerControllerCameraDeviceRear;
        } else {
			if([UIImagePickerController isCameraDeviceAvailable:UIImagePickerControllerCameraDeviceFront]) {
				_imagePicker.cameraDevice = UIImagePickerControllerCameraDeviceFront;
            } else {
				_imagePicker.cameraDevice = UIImagePickerControllerCameraDeviceRear;
            }
		}

		[[[UIApplication sharedApplication] keyWindow] addSubview:_imagePicker.view];
		
		return true;
	} else {
		return false;
	}
}

#ifdef __IPHONE_3_1
//-------------------------------------------------------------- overlay
- (BOOL) showCameraOverlay {
	[self showCameraOverlayWithCustomView:nil];
}

//--------------------------------------------------------------
- (BOOL) showCameraOverlayWithCustomView:(UIView *)overlayView {
    
    if(!cameraIsAvailable) {
        return NO;
    }
    
    CGSize screenSize = [UIScreen mainScreen].bounds.size;
    
    BOOL isOverlayView = YES;
    isOverlayView = isOverlayView && (overlayView != nil);
    if(isOverlayView) {
        isOverlayView = isOverlayView && [[overlayView class] isSubclassOfClass:[OverlayView class]];
    }
    
    if(isOverlayView) {
        overlay = (OverlayView *)overlayView;
    } else {
        
        CGSize screenSize = [UIScreen mainScreen].bounds.size;
        int screenW = screenSize.width;
        int screenH = screenSize.height;
        if(UIInterfaceOrientationIsLandscape([UIApplication sharedApplication].statusBarOrientation)) {
            screenW = screenSize.height;
            screenH = screenSize.width;
        }
        
        CGRect overlayFrame = CGRectMake(0, 0, screenW, screenH);
        overlay = [[OverlayView alloc] initWithFrame:overlayFrame];
    }
    overlay.delegate = _imagePicker;
    
    if([UIScreen mainScreen].bounds.size.width != 768 &&    // check if its NOT an ipad.
       [UIScreen mainScreen].bounds.size.height != 1024) {  // this is not future proof!
        /**
         *  only do this for iphone, not ipad.
         */
        overlay.center = CGPointMake(screenSize.width * 0.5, screenSize.height * 0.5);
        UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
        if(orientation == UIInterfaceOrientationLandscapeLeft) {
            overlay.transform = CGAffineTransformMakeRotation(-PI * 0.5);
        } else if(orientation == UIInterfaceOrientationLandscapeRight) {
            overlay.transform = CGAffineTransformMakeRotation(PI * 0.5);
        } else if(orientation == UIInterfaceOrientationPortraitUpsideDown) {
            overlay.transform = CGAffineTransformMakeRotation(PI);
        }
    }
    
    float cameraAspectRatio = 3.0f / 4.0f;  // all iOS devices have more or less an aspect 4:3 aspect ratio.
    float screenAspectRatio = screenSize.width / screenSize.height;
    float scale = cameraAspectRatio / screenAspectRatio;    // 1.12412 on iPhone.
    CGAffineTransform cameraViewTransform = CGAffineTransformMakeScale(scale, scale);
    
    _imagePicker.sourceType = UIImagePickerControllerSourceTypeCamera;
    _imagePicker.showsCameraControls = NO;
    _imagePicker.wantsFullScreenLayout = YES;
    _imagePicker.cameraViewTransform = cameraViewTransform;
    _imagePicker.cameraOverlayView = overlay;
    [[[UIApplication sharedApplication] keyWindow] addSubview:_imagePicker.view];
    
    return YES;
}

//--------------------------------------------------------------
- (void) hideCameraOverlay {
	_imagePicker.showsCameraControls = YES;
	_imagePicker.wantsFullScreenLayout = NO;
	_imagePicker.cameraViewTransform = CGAffineTransformScale(_imagePicker.cameraViewTransform, 1, 1);
	_imagePicker.cameraOverlayView = nil;
    
    if(overlay) {
        overlay.delegate = nil;
        [overlay release];
        overlay = nil;
    }
}
#endif
//--------------------------------------------------------------
- (BOOL) openLibrary {
	if(photoLibraryIsAvailable) {
		_imagePicker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
		[[[UIApplication sharedApplication] keyWindow] addSubview:_imagePicker.view];
	
        return true;
	} 
    return false;
}
//--------------------------------------------------------------
- (BOOL) openSavedPhotos {
	if(savedPhotosIsAvailable) {
		_imagePicker.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
		[[[UIApplication sharedApplication] keyWindow] addSubview:_imagePicker.view];
		
		return true;
	}
	return false;
}

//--------------------------------------------------------------
- (void)close {
	[_imagePicker.view removeFromSuperview];
}

//--------------------------------------------------------------
- (UIImage *) getUIImage {
	return _image;
}
//--------------------------------------------------------------
- (BOOL) isCameraAvailable {
	return cameraIsAvailable;
}
//--------------------------------------------------------------
- (BOOL) isPhotoLibraryAvailable {
	return photoLibraryIsAvailable;
}
//--------------------------------------------------------------
- (BOOL) isSavedPhotosAvailable {
	return savedPhotosIsAvailable;
}

//--------------------------------------------------------------
- (CGImageRef) getCGImage {
	return _image.CGImage;
}
//--------------------------------------------------------------
- (UIImageOrientation) getImageOrientation {
	return _image.imageOrientation;
}
//--------------------------------------------------------------
- (void) setMaxDimension:(int)mDimension {
	maxDimension = mDimension;
}

//--------------------------------------------------------------
- (UIImage *)scaleAndRotateImage:(UIImage *)image {	
	//scaleAndRotateImage code from Yoshimasa Niwa :: http://niw.at/en
	
	CGImageRef imgRef = image.CGImage;
	CGFloat width = CGImageGetWidth(imgRef);
	CGFloat height = CGImageGetHeight(imgRef);
	
	CGAffineTransform transform = CGAffineTransformIdentity;
	CGRect bounds = CGRectMake(0, 0, width, height);
	if (width > maxDimension || height > maxDimension) {
		CGFloat ratio = width/height;
		if (ratio > 1) {
			bounds.size.width = maxDimension;
			bounds.size.height = bounds.size.width / ratio;
		} else {
			bounds.size.height = maxDimension;
			bounds.size.width = bounds.size.height * ratio;
		}
	}
	
	CGFloat scaleRatio = bounds.size.width / width;
	CGSize imageSize = CGSizeMake(CGImageGetWidth(imgRef), CGImageGetHeight(imgRef));
	CGFloat boundHeight;
	
	UIImageOrientation orient = image.imageOrientation;
	switch(orient) {
		case UIImageOrientationUp:
			transform = CGAffineTransformIdentity;
			break;
		case UIImageOrientationUpMirrored:
			transform = CGAffineTransformMakeTranslation(imageSize.width, 0.0);
			transform = CGAffineTransformScale(transform, -1.0, 1.0);
			break;
		case UIImageOrientationDown:
			transform = CGAffineTransformMakeTranslation(imageSize.width, imageSize.height);
			transform = CGAffineTransformRotate(transform, M_PI);
			break;
		case UIImageOrientationDownMirrored:
			transform = CGAffineTransformMakeTranslation(0.0, imageSize.height);
			transform = CGAffineTransformScale(transform, 1.0, -1.0);
			break;
		case UIImageOrientationLeftMirrored:
			boundHeight = bounds.size.height;
			bounds.size.height = bounds.size.width;
			bounds.size.width = boundHeight;
			transform = CGAffineTransformMakeTranslation(imageSize.height, imageSize.width);
			transform = CGAffineTransformScale(transform, -1.0, 1.0);
			transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
			break;
		case UIImageOrientationLeft:
			boundHeight = bounds.size.height;
			bounds.size.height = bounds.size.width;
			bounds.size.width = boundHeight;
			transform = CGAffineTransformMakeTranslation(0.0, imageSize.width);
			transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
			break;
		case UIImageOrientationRightMirrored:
			boundHeight = bounds.size.height;
			bounds.size.height = bounds.size.width;
			bounds.size.width = boundHeight;
			transform = CGAffineTransformMakeScale(-1.0, 1.0);
			transform = CGAffineTransformRotate(transform, M_PI / 2.0);
			break;
		case UIImageOrientationRight:
			boundHeight = bounds.size.height;
			bounds.size.height = bounds.size.width;
			bounds.size.width = boundHeight;
			transform = CGAffineTransformMakeTranslation(imageSize.height, 0.0);
			transform = CGAffineTransformRotate(transform, M_PI / 2.0);
			break;
		default:
			[NSException raise:NSInternalInconsistencyException format:@"Invalid image orientation"];
	}
	
	UIGraphicsBeginImageContext(bounds.size);
	CGContextRef context = UIGraphicsGetCurrentContext();
	if (orient == UIImageOrientationRight || orient == UIImageOrientationLeft) {
		CGContextScaleCTM(context, -scaleRatio, scaleRatio);
		CGContextTranslateCTM(context, -height, 0);
	} else {
		CGContextScaleCTM(context, scaleRatio, -scaleRatio);
		CGContextTranslateCTM(context, 0, -height);
	}
	CGContextConcatCTM(context, transform);
	CGContextDrawImage(UIGraphicsGetCurrentContext(), CGRectMake(0, 0, width, height), imgRef);
	UIImage *imageCopy = UIGraphicsGetImageFromCurrentImageContext();
	UIGraphicsEndImageContext();
	
	return imageCopy;
}
@end

//----------------------------------------------------------- overlay.
@implementation OverlayView

@synthesize delegate;

- (id)initWithFrame:(CGRect)frame{
    if (self = [super initWithFrame:frame]) {
        [self initUI];
    }
    return self;
}

/**
 *
 * override initUI if you'd like to create your own custom UI.
 *
 */
- (void)initUI {

    self.opaque = NO;
    self.backgroundColor = [UIColor clearColor];
    
    int buttonW = 70;
    int buttonH = 40;
    int buttonX = self.frame.size.width - buttonW - 10;
    int buttonY = (self.frame.size.height - buttonH) * 0.5;
    
    UIButton * button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    button.frame = CGRectMake(buttonX, buttonY, buttonW, buttonH);
    button.alpha = 0.7;
    [button setTitle:@"SNAP" forState:UIControlStateNormal];
    [button addTarget:self action:@selector(takePhoto:) forControlEvents:UIControlEventTouchUpInside];
    
    [self addSubview:button];
}

- (void)takePhoto:(id)sender {
    if([delegate respondsToSelector:@selector(takePicture)]) {
        [delegate takePicture];
    }	
}

@end