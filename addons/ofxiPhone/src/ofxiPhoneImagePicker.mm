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
	[imagePicker setMaxDimension: 640];
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
bool ofxiPhoneImagePicker::openCamera()
{
	return [imagePicker openCamera];
}
#ifdef __IPHONE_3_1
//----------------------------------------------------------------
bool ofxiPhoneImagePicker::showCameraOverlay()
{
	return [imagePicker showCameraOverlay];
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
	
	// Get the width and height of the image
	width = (int) CGImageGetWidth(photo);
	height = (int) CGImageGetHeight(photo);
	
	if(photo)
	{
		photoData = (GLubyte *) malloc(width * height * bpp);
		
		pixels = (unsigned char *) malloc(width * height * bpp);
		
		photoContext = CGBitmapContextCreate(photoData, width, height, 8, width * bpp, CGImageGetColorSpace(photo), kCGImageAlphaPremultipliedLast);
		
		CGContextDrawImage(photoContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), photo);
		
		int numBytesPerRow = width * bpp;
		for(int y=0; y<height; y++) {
			memcpy(pixels + (numBytesPerRow * y), photoData + (numBytesPerRow * (height - 1 - y)), numBytesPerRow);
		}
		
		
		CGContextRelease(photoContext);
		CGImageRelease(photo);
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
		photoLibraryIsAvailable = false;
		savedPhotosIsAvailable = false;
		cameraIsAvailable = false;
		
		_imagePicker = [[UIImagePickerController alloc] init];
	
		CGSize s = [[[UIApplication sharedApplication] keyWindow] bounds].size;		
										
		overlay = [[OverlayView alloc]
				   initWithFrame:CGRectMake(0, 0, s.width, s.height) andDelegate:_imagePicker];
		
		_imagePicker.delegate = self;
		
		cppPixelLoader = _picker;
		
		
		if( [ UIImagePickerController isSourceTypeAvailable: UIImagePickerControllerSourceTypePhotoLibrary ]== YES )
		{
			photoLibraryIsAvailable = true;
		}
		
		if( [ UIImagePickerController isSourceTypeAvailable: UIImagePickerControllerSourceTypeCamera ]== YES )
		{
			cameraIsAvailable = true;
		}
		
		if( [ UIImagePickerController isSourceTypeAvailable: UIImagePickerControllerSourceTypeSavedPhotosAlbum ]== YES )
		{
			savedPhotosIsAvailable = true;
		}
		
		_image = [UIImage alloc];
	}
	return self;
}
//--------------------------------------------------------------

- (void)dealloc 
{ 
	[_imagePicker release];
	
	[_image	release];
	
	[super dealloc];
}

//----------------------------------------------------------------


- (void) imagePickerController:(UIImagePickerController *)picker didFinishPickingImage:(UIImage *)image editingInfo:(NSDictionary *)editingInfo
{	
	[_image initWithCGImage: [self scaleAndRotateImage:image].CGImage];
	[_imagePicker.view removeFromSuperview];
	
	cppPixelLoader->loadPixels();
}


#ifdef __IPHONE_3_1
- (void) imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
	[_image initWithCGImage: [self scaleAndRotateImage:[info objectForKey:UIImagePickerControllerOriginalImage]].CGImage];
	[_imagePicker.view removeFromSuperview];
	
	cppPixelLoader->loadPixels();
}


//--------------------------------------------------------------
- (void) takePicture
{
	[_imagePicker takePicture];
}
#endif
//--------------------------------------------------------------
- (void) imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
	[_imagePicker.view removeFromSuperview];
}
//--------------------------------------------------------------
- (void) saveImageToPhotoAlbum
{
	UIImageWriteToSavedPhotosAlbum(_image, nil, nil, nil);
	printf("saved!");
}
//--------------------------------------------------------------
- (bool) openCamera
{
	if(cameraIsAvailable)
	{
		[_imagePicker init]; //needs this to refresh the camera.
		_imagePicker.sourceType = UIImagePickerControllerSourceTypeCamera;
		[[[UIApplication sharedApplication] keyWindow] addSubview:_imagePicker.view];
		
		return true;
	}
	else
	{
		return false;
	}
}
#ifdef __IPHONE_3_1
//--------------------------------------------------------------
- (bool) showCameraOverlay
{
	if(cameraIsAvailable)
	{
		[_imagePicker init]; //needs this to refresh the camera.
		_imagePicker.sourceType = UIImagePickerControllerSourceTypeCamera;
		_imagePicker.showsCameraControls = NO;
		_imagePicker.wantsFullScreenLayout = YES;
		_imagePicker.cameraViewTransform = CGAffineTransformScale(_imagePicker.cameraViewTransform,1,1.12412);
		_imagePicker.cameraOverlayView = overlay;
		//[self presentModalViewController:_imagePicker animated:NO];
		[[[UIApplication sharedApplication] keyWindow] addSubview:_imagePicker.view];
		
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------
- (void) hideCameraOverlay
{
	[_imagePicker.view removeFromSuperview];
	_imagePicker.showsCameraControls = YES;
	_imagePicker.wantsFullScreenLayout = NO;
	_imagePicker.cameraViewTransform = CGAffineTransformScale(_imagePicker.cameraViewTransform, 1, 1);
	_imagePicker.cameraOverlayView = nil;
}
#endif
//--------------------------------------------------------------
- (bool) openLibrary
{
	if(photoLibraryIsAvailable)
	{
		_imagePicker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
		[[[UIApplication sharedApplication] keyWindow] addSubview:_imagePicker.view];
		
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------
- (bool) openSavedPhotos
{
	if(savedPhotosIsAvailable)
	{
		_imagePicker.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
		[[[UIApplication sharedApplication] keyWindow] addSubview:_imagePicker.view];
		
		return true;
	}
	else
	{
		return false;
	}
}

//--------------------------------------------------------------
- (UIImage *) getUIImage
{
	return _image;
}
//--------------------------------------------------------------
- (bool) isCameraAvailable
{
	return cameraIsAvailable;
}
//--------------------------------------------------------------
- (bool) isPhotoLibraryAvailable
{
	return photoLibraryIsAvailable;
}
//--------------------------------------------------------------
- (bool) isSavedPhotosAvailable
{
	return savedPhotosIsAvailable;
}

//--------------------------------------------------------------
- (CGImageRef) getCGImage;
{
	return _image.CGImage;
}
//--------------------------------------------------------------
- (UIImageOrientation) getImageOrientation;
{
	return _image.imageOrientation;
}
//--------------------------------------------------------------
- (void) setMaxDimension:(int)mDimension 
{
	maxDimension = mDimension;
}

//--------------------------------------------------------------

- (UIImage *)scaleAndRotateImage:(UIImage *)image 
{	
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

@implementation OverlayView
- (id)initWithFrame:(CGRect)frame andDelegate:(UIImagePickerController *)del{
    if (self = [super initWithFrame:frame]) {
        //clear the background color of the overlay
        self.opaque = NO;
        self.backgroundColor = [UIColor clearColor];
		
        //load an image to show in the overlay. uncomment this if you want to show an image like crosshairs, etc
        //UIImage *searcher = [UIImage imageNamed:@"searcher.png"];
        //UIImageView *searcherView = [[UIImageView alloc] 
		//							 initWithImage:searcher];
        //searcherView.frame = CGRectMake(30, 100, 260, 200);
        //[self addSubview:searcherView];
        //[searcherView release];
		
        UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
        [button setTitle:@"" forState:UIControlStateNormal];
		
		CGSize s = [[[UIApplication sharedApplication] keyWindow] bounds].size;		
				
        button.frame = CGRectMake(0, 0, s.height, s.width); // change these numbers to move the location of the button.
		button.opaque = NO;
		
		[button addTarget:self action:@selector(takePhoto:) forControlEvents:UIControlEventTouchUpInside];
		
        [self addSubview:button];
		_del = del;
    }
    return self;
}

- (void) takePhoto:(id) sender
{
	[_del takePicture];
}
@end