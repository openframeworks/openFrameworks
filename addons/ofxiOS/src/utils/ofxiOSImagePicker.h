/*
 *  ofxiOSImagePicker.h
 *
 *  Created by Zach Gage on 3/1/09.
 *  Copyright 2009 stfj. All rights reserved.
 *
 */

#pragma once

#include <TargetConditionals.h>
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)

#import <UIKit/UIKit.h>
#include "ofPixels.h"

#define OFX_IMG_PICKER_UP 1
#define OFX_IMG_PICKER_DOWN 2
#define OFX_IMG_PICKER_LEFT 3
#define OFX_IMG_PICKER_RIGHT 4

class canLoadPixels
{
public:
	virtual void loadPixels() = 0;
};

//----------------------------------------------------------- overlay.
@interface ofxiOSImagePickerOverlayView : UIView
/// TODO: give protocol explicitly.
@property (nonatomic, strong) id delegate;
- (void)initUI;
- (void)takePhoto:(id)sender;
@end

//-----------------------------------------------------------
@interface ofxiOSImagePickerDelegate : NSObject <UINavigationControllerDelegate, UIImagePickerControllerDelegate>
{

	UIImagePickerController*			_imagePicker;
	ofxiOSImagePickerOverlayView*		overlay;
	UIImage*							_image;
	bool								cameraIsAvailable;
	bool								photoLibraryIsAvailable;
	bool								savedPhotosIsAvailable;
	int									maxDimension;
	
	canLoadPixels *						cppPixelLoader;
}

- (instancetype) initWithPicker:(canLoadPixels *) _picker;

- (void) imagePickerController:(UIImagePickerController *)picker didFinishPickingImage:(UIImage *)image editingInfo:(NSDictionary *)editingInfo;

- (void) takePicture;
- (void) imagePickerControllerDidCancel:(UIImagePickerController *)picker;

- (void) imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info;

- (CGImageRef) getCGImage;

- (UIImageOrientation) getImageOrientation;

- (BOOL) openLibrary;
- (BOOL) openCamera:(int)camera;
- (BOOL) showCameraOverlay;
- (BOOL) showCameraOverlayWithCustomView:(UIView *)overlayView;
- (void) hideCameraOverlay;
- (BOOL) openSavedPhotos;
- (void) close;

- (BOOL) isCameraAvailable;
- (BOOL) isPhotoLibraryAvailable;
- (BOOL) isSavedPhotosAvailable;
//- (bool) wasNewImagePicked;

- (void) saveImageToPhotoAlbum;

- (void) setMaxDimension:(int)mDimension;

- (UIImage *) getUIImage;
- (UIImage *)scaleAndRotateImage:(UIImage *)image;

@end

#define ofxiPhoneImagePickerDelegate ofxiOSImagePickerDelegate


class ofxiOSImagePicker : public canLoadPixels
{
	
public:
	
	ofxiOSImagePicker();
	~ofxiOSImagePicker();
	
	bool openCamera(int camera=0); // 0 for rear, 1 for front
	bool openLibrary();
    
	bool showCameraOverlay();
    bool showCameraOverlayWithCustomView(UIView * view);
	void hideCameraOverlay();

	bool openSavedPhotos();
    void close(); //closes the image picker interface
    void clear(); //clears the internal ofPixels - useful if you need to free the memory without deleting the object. 
	
	bool cameraIsAvailable; //variables to see if specific functions are available for a specific device.
	bool photoLibraryIsAvailable;
	bool savedPhotosIsAvailable;
	
	void setMaxDimension(int _maxDimension); //images that you take with the camera will be too big to properly get into an ofImage (im not sure why). It's good to set a maximum dimension for the images under 1000
	int getOrientation();
	
	void saveImage(); //this doesn't quite work right now and i'm not sure why.
	
	void takePicture();
	
	void loadPixels(); //never call this. this is called by the obj-c class.
	
    bool getImageUpdated();
    unsigned char * 	getPixels();
    ofPixels &			getPixelsRef();
    int getWidth();
    int getHeight();
    
    protected:
    
    ofPixels pixels;
    int maxDimension;
	
//	unsigned char * pixels;
	
protected:
	bool imageUpdated; //when a new image is loaded in, this is set to true
	ofxiOSImagePickerDelegate *	imagePicker;
};

#define ofxiPhoneImagePicker ofxiOSImagePicker; 

#endif
