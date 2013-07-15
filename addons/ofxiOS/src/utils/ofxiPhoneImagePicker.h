/*
 *  ofxiPhoneImagePicker.h
 *  iPhone UIImagePicker Example 
 *
 *  Created by Zach Gage on 3/1/09.
 *  Copyright 2009 stfj. All rights reserved.
 *
 */

#import <UIKit/UIKit.h>
#import <OpenGLES/ES1/gl.h>
#import "ofMain.h"
#include <iostream>
#include <Availability.h>

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
@interface OverlayView : UIView
@property (nonatomic, retain) id delegate;
- (void)initUI;
- (void)takePhoto:(id)sender;
@end

//-----------------------------------------------------------
@interface ofxiPhoneImagePickerDelegate : NSObject <UINavigationControllerDelegate, UIImagePickerControllerDelegate>
{

	UIImagePickerController*			_imagePicker;
	OverlayView *						overlay;
	UIImage*							_image;
	bool								cameraIsAvailable;
	bool								photoLibraryIsAvailable;
	bool								savedPhotosIsAvailable;
	int									maxDimension;
	
	canLoadPixels *						cppPixelLoader;
}

- (id) initWithPicker:(canLoadPixels *) _picker;

- (void) imagePickerController:(UIImagePickerController *)picker didFinishPickingImage:(UIImage *)image editingInfo:(NSDictionary *)editingInfo;

#ifdef __IPHONE_3_1
- (void) takePicture;
#endif

- (void) imagePickerControllerDidCancel:(UIImagePickerController *)picker;

#ifdef __IPHONE_3_1
- (void) imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info;
#endif
- (CGImageRef) getCGImage;

- (UIImageOrientation) getImageOrientation;

- (BOOL) openLibrary;
- (BOOL) openCamera:(int)camera;
#ifdef __IPHONE_3_1
- (BOOL) showCameraOverlay;
- (BOOL) showCameraOverlayWithCustomView:(UIView *)overlayView;
- (void) hideCameraOverlay;
#endif
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


class ofxiPhoneImagePicker : public canLoadPixels
{
	
public:
	
	ofxiPhoneImagePicker();
	~ofxiPhoneImagePicker();
	
	bool openCamera(int camera=0); // 0 for rear, 1 for front
	bool openLibrary();
    
#ifdef __IPHONE_3_1
	bool showCameraOverlay();
    bool showCameraOverlayWithCustomView(UIView * view);
	void hideCameraOverlay();
#endif
	bool openSavedPhotos();
    void close();
	
	bool cameraIsAvailable; //variables to see if specific functions are available for a specific device.
	bool photoLibraryIsAvailable;
	bool savedPhotosIsAvailable;
	
	void setMaxDimension(int _maxDimension); //images that you take with the camera will be too big to properly get into an ofImage (im not sure why). It's good to set a maximum dimension for the images under 1000
	int getOrientation();
	
	void saveImage(); //this doesn't quite work right now and i'm not sure why.
	
#ifdef __IPHONE_3_1
	void takePicture();
#endif
	
	void loadPixels(); //never call this. this is called by the obj-c class.
	bool imageUpdated; //when a new image is loaded in, this is set to true
	
	int width;
	int height;
	int type;
	int glType;
	int texType;
	int bpp;
	
	bool pixelsAllocated;
	
	int maxDimension;
	
	unsigned char * pixels;
	
protected:
	
	ofxiPhoneImagePickerDelegate *	imagePicker;
};
