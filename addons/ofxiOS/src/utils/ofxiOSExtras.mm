/***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 


#import "ofxiOSExtras.h"

//--------------------------------------------------------------
ofxiOSDeviceType ofxiOSGetDeviceType() {
    NSString * dev = [[[UIDevice currentDevice] model] lowercaseString];    
    if( [dev hasPrefix:@"iphone"] ) return ofxiOS_DEVICE_IPHONE;
    if( [dev hasPrefix:@"ipad"] ) return ofxiOS_DEVICE_IPAD;
    if( [dev hasPrefix:@"ipod"] ) return ofxiOS_DEVICE_IPODTOUCH;
    return ofxiOS_UNKNOWN_DEVICE;   //this would need to be declared 
}


//--------------------------------------------------------------
string ofxiOSGetDeviceRevision() {
	size_t size;
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
	
	if( size > 0 ){
    	char * machine;
	machine = new char[size];
	sysctlbyname("hw.machine", machine, &size, NULL, 0);
	
	string device(machine);
	
		delete[] machine;
			
	return device;
	}else{
		return "";
	}
}

//--------------------------------------------------------------
UIWindow *ofxiOSGetUIWindow() {
	return [[UIApplication sharedApplication] keyWindow];
}


//--------------------------------------------------------------
ofxiOSEAGLView *ofxiOSGetGLView() {
	return [ofxiOSEAGLView getInstance];
}

//--------------------------------------------------------------
UIView * ofxiOSGetGLParentView() {
	return ofxiOSGetGLView().superview;
}

//--------------------------------------------------------------
ofAppiOSWindow* ofxiOSGetOFWindow() {
	return ofAppiOSWindow::getInstance();
}


//--------------------------------------------------------------
ofxiOSAppDelegate *ofxiOSGetAppDelegate() {
	return [[UIApplication sharedApplication] delegate];
}

//--------------------------------------------------------------
ofxiOSViewController *ofxiOSGetViewController() {
	return [ofxiOSGetAppDelegate() glViewController];
}

//--------------------------------------------------------------
void ofxiOSSendGLViewToFront() {
	[ofxiOSGetGLView().superview bringSubviewToFront:ofxiOSGetGLView()];
}


//--------------------------------------------------------------
void ofxiOSSendGLViewToBack() {
	[ofxiOSGetGLView().superview sendSubviewToBack:ofxiOSGetGLView()];
}


//--------------------------------------------------------------
void ofxiOSSetGLViewTransparent(bool b) {
	ofxiOSGetGLView().layer.opaque = !b;
}


//--------------------------------------------------------------
void ofxiOSSetGLViewUserInteraction(bool b) {
	ofxiOSGetGLView().userInteractionEnabled = b;
}



//--------------------------------------------------------------
void ofxiOSEnableIdleTimer() {
	[UIApplication sharedApplication].idleTimerDisabled = false;
}


//--------------------------------------------------------------
void ofxiOSDisableIdleTimer() {
	[UIApplication sharedApplication].idleTimerDisabled = true;
}


//--------------------------------------------------------------
void ofxiOSLockGLContext() {
	[ofxiOSGetGLView() lockGL];
}


//--------------------------------------------------------------
void ofxiOSUnlockGLContext() {
	[ofxiOSGetGLView() unlockGL];
}


//--------------------------------------------------------------
void ofxiOSEnableLoopInThread() {
//	[ofxiOSGetAppDelegate() enableLoopInThread];
	ofLogWarning("ofxiOSExtras") << "ofxiOSEnableLoopInThread() is deprecated, CADisplayLink provides better animation loop";
}


//--------------------------------------------------------------
void ofxiOSSetOrientation(ofOrientation orientation) {
    ofSetOrientation(orientation);
}


//--------------------------------------------------------------
UIDeviceOrientation ofxiOSGetOrientation() {
    return (UIDeviceOrientation)ofGetOrientation();
}


//--------------------------------------------------------------
bool ofxiOSBundleImageToGLTexture(NSString *filename, GLuint *spriteTexture) {
	return ofxiOSUIImageToGLTexture([UIImage imageNamed:filename], spriteTexture);
}


//--------------------------------------------------------------
bool ofxiOSUIImageToGLTexture(UIImage *uiImage, GLuint *spriteTexture) {
	if(!uiImage) return false;
	
	CGImageRef cgImage;
	CGContextRef spriteContext;
	GLubyte *pixels;
	size_t	width, height;
	
	// Creates a Core Graphics image from an image file
	cgImage = uiImage.CGImage;
	
	// Get the width and height of the image
	width = CGImageGetWidth(cgImage);
	height = CGImageGetHeight(cgImage);
	
	// Allocated memory needed for the bitmap context
	pixels = (GLubyte *) malloc(width * height * 4);
	// Uses the bitmatp creation function provided by the Core Graphics framework. 
	spriteContext = CGBitmapContextCreate(pixels, width, height, 8, width * 4, CGImageGetColorSpace(cgImage), kCGImageAlphaPremultipliedLast);
	// After you create the context, you can draw the sprite image to the context.
	CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), cgImage);
	// You don't need the context at this point, so you need to release it to avoid memory leaks.
	CGContextRelease(spriteContext);
	
	// Use OpenGL ES to generate a name for the texture.
	glGenTextures(1, spriteTexture);
	// Bind the texture name. 
	glBindTexture(GL_TEXTURE_2D, *spriteTexture);
	// Speidfy a 2D texture image, provideing the a pointer to the image data in memory
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	// Release the image data
	free(pixels);
	
	// Set the texture parameters to use a minifying filter and a linear filer (weighted average)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	return true;
}


//--------------------------------------------------------------
bool ofxiOSUIImageToOFImage(UIImage *uiImage, ofImage &outImage, int targetWidth, int targetHeight) {
	if(uiImage == nil) {
        return false;
    }
	
	CGContextRef spriteContext;
	CGImageRef cgImage = uiImage.CGImage;
	
	int bytesPerPixel = CGImageGetBitsPerPixel(cgImage)/8;
	if(bytesPerPixel == 3) bytesPerPixel = 4;
	
	int width = targetWidth > 0 ? targetWidth : CGImageGetWidth(cgImage);
	int height = targetHeight > 0 ? targetHeight : CGImageGetHeight(cgImage);
	
	// Allocated memory needed for the bitmap context
	GLubyte * pixels = (GLubyte *)malloc(width * height * bytesPerPixel);
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	
	// Uses the bitmatp creation function provided by the Core Graphics framework.
	spriteContext = CGBitmapContextCreate(pixels,
                                          width,
                                          height,
                                          CGImageGetBitsPerComponent(cgImage),
                                          width * bytesPerPixel,
                                          colorSpace,
                                          bytesPerPixel == 4 ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNone);
    CGColorSpaceRelease(colorSpace);
	
	if(spriteContext == NULL) {
		ofLogError("ofxiOSExtras") << "ofxiOSUIImageToOFImage(): CGBitmapContextCreate returned NULL";
		free(pixels);
		return false;
	}
	
    CGContextSetBlendMode(spriteContext, kCGBlendModeCopy);
    
	// After you create the context, you can draw the sprite image to the context.
	ofLogVerbose("ofxiOSExtras") << "ofxiOSUIImageToOFImage(): about to CGContextDrawImage";
	CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), cgImage);
	
	// You don't need the context at this point, so you need to release it to avoid memory leaks.
	ofLogVerbose("ofxiOSExtras") << "ofxiOSUIImageToOFImage(): about to CGContextRelease";
	CGContextRelease(spriteContext);
	
	ofImageType ofImageMode;
	
	switch(bytesPerPixel) {
		case 1:
			ofImageMode = OF_IMAGE_GRAYSCALE;
			break;
		case 3: 
			ofImageMode = OF_IMAGE_COLOR;
			break;
		case 4: 
		default:
			ofImageMode = OF_IMAGE_COLOR_ALPHA; break;
	}
			
	ofLogVerbose("ofxiOSExtras") << "ofxiOSUIImageToOFImage(): about to setFromPixels";
	outImage.setFromPixels(pixels, width, height, ofImageMode, true);

	free(pixels);
	
	return true;
}

//--------------------------------------------------------------
bool ofxiOSUIImageToOFTexture(UIImage *uiImage, ofTexture &outTexture, int targetWidth, int targetHeight) {
	if(!uiImage) return false;
	
	CGContextRef spriteContext;
	CGImageRef	cgImage = uiImage.CGImage;

	int bytesPerPixel	= CGImageGetBitsPerPixel(cgImage)/8;
	if(bytesPerPixel == 3) bytesPerPixel = 4;
	
	int width			= targetWidth > 0 ? targetWidth : CGImageGetWidth(cgImage);
	int height			= targetHeight > 0 ? targetHeight : CGImageGetHeight(cgImage);
	
	// Allocated memory needed for the bitmap context
	GLubyte *pixels		= (GLubyte *) malloc(width * height * bytesPerPixel);
	
	// Uses the bitmap creation function provided by the Core Graphics framework. 
	spriteContext = CGBitmapContextCreate(pixels, width, height, CGImageGetBitsPerComponent(cgImage), width * bytesPerPixel, CGImageGetColorSpace(cgImage), bytesPerPixel == 4 ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNone);
	
	if(spriteContext == NULL) {
		ofLogError("ofxiOSExtras") << "ofxiOSUIImageToOFImage(): CGBitmapContextCreate returned NULL";
		free(pixels);
		return false;
	}

    CGContextSetBlendMode(spriteContext, kCGBlendModeCopy);
    
	// After you create the context, you can draw the sprite image to the context.
	ofLogVerbose("ofxiOSExtras") << "ofxiOSUIImageToOFImage(): about to CGContextDrawImage";
	CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), cgImage);
	
	// You don't need the context at this point, so you need to release it to avoid memory leaks.
	ofLogVerbose("ofxiOSExtras") << "ofxiOSUIImageToOFImage(): about to CGContextRelease";
	CGContextRelease(spriteContext);
		
	int glMode;
	
	switch(bytesPerPixel) {
		case 1:
			glMode = GL_LUMINANCE;
			break;
		case 3: 
			glMode = GL_RGB;
			break;
		case 4: 
		default:
			glMode = GL_RGBA; break;
	}
			
	outTexture.allocate(width, height, glMode);
	outTexture.loadData(pixels, width, height, glMode);

	free(pixels);
	
	return true;
}

//--------------------------------------------------------------

bool ofxiOSCGImageToPixels(CGImageRef & ref, unsigned char * pixels){
	
	CGContextRef spriteContext;
	
	int bytesPerPixel	= CGImageGetBitsPerPixel(ref)/8;
	if(bytesPerPixel == 3) bytesPerPixel = 4;
	
	int w			= CGImageGetWidth(ref);
	int h			= CGImageGetHeight(ref);
	
	// Allocated memory needed for the bitmap context
	GLubyte *pixelsTmp	= (GLubyte *) malloc(w * h * bytesPerPixel);
	
	// Uses the bitmap creation function provided by the Core Graphics framework. 
	spriteContext = CGBitmapContextCreate(pixelsTmp, w, h, CGImageGetBitsPerComponent(ref), w * bytesPerPixel, CGImageGetColorSpace(ref), bytesPerPixel == 4 ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNone);
	
	if(spriteContext == NULL) {
		ofLogError("ofxiOSExtras") << "ofxiOSCGImageToPixels(): CGBitmapContextCreate returned NULL";
		free(pixelsTmp);
		return false;
	}
	
	CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)w, (CGFloat)h), ref);
	CGContextRelease(spriteContext);
	
	// int totalSrcBytes = w*h*bytesPerPixel;  
	
	/*
	// averaging around 3300ms per 100reps
	long then = ofGetElapsedTimeMillis();
	for (int N=0; N<100; N++){
		int j = 0;
		for(int k = 0; k < totalSrcBytes; k+= bytesPerPixel ){
			pixels[j  ] = pixelsTmp[k  ];
			pixels[j+1] = pixelsTmp[k+1];
			pixels[j+2] = pixelsTmp[k+2];
			j+=3;
		}
		
	}
	long now = ofGetElapsedTimeMillis();
	ofLogNotice("ofxiOSExtras") << "ofxiOSCGImageToPixels(): elapsed " << now-then;
	*/
	
	// Step through both source and destination 4 bytes at a time.
	// But reset the destination pointer by shifting it backwards 1 byte each time. 
	// Effectively: 4 steps forward, 1 step back each time through the loop. 
	// on average, around 1900ms for 100 reps // GOOD
	unsigned int *isrc4 = (unsigned int *)pixelsTmp;
	unsigned int *idst3 = (unsigned int *)pixels;
	unsigned int *ilast4 = &isrc4[w*h-1];
	while (isrc4 < ilast4){
		*(idst3++) = *(isrc4++);
		idst3 = (unsigned int *) (((unsigned char *) idst3) - 1);
	}
	
	free(pixelsTmp);
	return true;
	 
}

//--------------------------------------------------------------

string ofxiOSNSStringToString(NSString * s) {
	return string([s UTF8String]);
}

//--------------------------------------------------------------

NSString * ofxiOSStringToNSString(string s) {
	return [NSString stringWithUTF8String:s.c_str()];
}

//--------------------------------------------------------------

string ofxiOSGetDocumentsDirectory()
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	return ofxNSStringToString([paths objectAtIndex:0]) + "/";
}

//--------------------------------------------------------------

void ofxiOSLaunchBrowser(string url) {
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:ofxStringToNSString(url)]];
}

//--------------------------------------------------------------
void ofxiOSSetClipboardString(string clipboardString) {
    [UIPasteboard generalPasteboard].string = [NSString stringWithUTF8String:clipboardString.c_str()];
}

string ofxiOSGetClipboardString() {
    return [[UIPasteboard generalPasteboard].string UTF8String];
}

/******************** ofxiOSScreenGrab *********************/

@interface SaveDelegate : NSObject {
	id delegate;
}
@property (retain, nonatomic) id delegate;
@end


@implementation SaveDelegate
@synthesize delegate;

// callback for UIImageWriteToSavedPhotosAlbum
- (void)image:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo {
	ofLogVerbose("ofxiOSExtras") << "didFinishSavingWithError: save finished";

	if([delegate respondsToSelector: @selector(saveComplete)]) {
        [delegate performSelector:@selector(saveComplete)];
    }
	
	[self release];
}

-(void)dealloc {
	[delegate release];
	[super dealloc];
}

@end



// callback for CGDataProviderCreateWithData
void releaseData(void *info, const void *data, size_t dataSize) {
	//	NSLog(@"releaseData\n");
	free((void*)data);		// free the 
}


void ofxiOSScreenGrab(id delegate) {
	CGRect rect = [[UIScreen mainScreen] bounds];
	
	//fix from: http://forum.openframeworks.cc/index.php/topic,6092.15.html
	//TODO: look and see if we need to take rotation into account 
    if(ofxiOSGetOFWindow()->isRetinaEnabled()) {
        float f_scale = [[UIScreen mainScreen] scale];
        rect.size.width *= f_scale;
        rect.size.height *= f_scale;
    }

	int width  = rect.size.width;
	int height = rect.size.height;	
	
	NSInteger myDataLength = width * height * 4;
	GLubyte *buffer = (GLubyte *) malloc(myDataLength);
	GLubyte *bufferFlipped = (GLubyte *) malloc(myDataLength);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	for(int y = 0; y <height; y++) {
		for(int x = 0; x <width * 4; x++) {
			bufferFlipped[int((height - 1 - y) * width * 4 + x)] = buffer[int(y * 4 * width + x)];
		}
	}
	free(buffer);	// free original buffer
	
	CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, bufferFlipped, myDataLength, releaseData);
	CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
	CGImageRef imageRef = CGImageCreate(width, height, 8, 32, 4 * width, colorSpaceRef, kCGBitmapByteOrderDefault, provider, NULL, NO, kCGRenderingIntentDefault);		
	
	CGColorSpaceRelease(colorSpaceRef);
	CGDataProviderRelease(provider);
    
    UIImage * image = [UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
    
    NSData * imageData = UIImagePNGRepresentation(image);
    UIImage * imageLossless = [UIImage imageWithData:imageData];
    
	SaveDelegate *saveDelegate = [SaveDelegate new];
	saveDelegate.delegate = delegate;
	UIImageWriteToSavedPhotosAlbum(imageLossless, saveDelegate, @selector(image:didFinishSavingWithError:contextInfo:), nil);
}


