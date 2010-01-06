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
void ofxiPhoneImagePicker::setMaxDimension(int _maxDimension)
{
	maxDimension = _maxDimension;
	[imagePicker setMaxDimension: maxDimension];
}

//--------------------------------------------------------------
ofxiPhoneImagePicker::~ofxiPhoneImagePicker()
{
	[imagePicker release];
}

//----------------------------------------------------------------
bool ofxiPhoneImagePicker::openCamera()
{
	return [imagePicker openCamera];
}
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