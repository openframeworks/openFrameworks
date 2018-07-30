#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

class ofApp: public ofxUnitTestsApp{

	bool hasImageFormat(ofPixelFormat pixelFormat){
		switch(pixelFormat){
			case OF_PIXELS_RGB:
			case OF_PIXELS_BGR:
			case OF_PIXELS_RGB565:
			case OF_PIXELS_RGBA:
			case OF_PIXELS_BGRA:
			case OF_PIXELS_GRAY:
				return true;
			case OF_PIXELS_GRAY_ALPHA:
			case OF_PIXELS_YUY2:
			case OF_PIXELS_UYVY:
			case OF_PIXELS_Y:
			case OF_PIXELS_U:
			case OF_PIXELS_V:
			case OF_PIXELS_UV:
			case OF_PIXELS_VU:
			case OF_PIXELS_NV12:
			case OF_PIXELS_NV21:
			case OF_PIXELS_YV12:
			case OF_PIXELS_I420:
			case OF_PIXELS_NATIVE:
			case OF_PIXELS_NUM_FORMATS:
			case OF_PIXELS_UNKNOWN:
				return false;
		}
		ofxTest(false,"asked for non checked pixel format");
		return false;
	}

	ofImageType imageType(ofPixelFormat pixelFormat){
		switch(pixelFormat){
			case OF_PIXELS_RGB:
			case OF_PIXELS_BGR:
			case OF_PIXELS_RGB565:
				return OF_IMAGE_COLOR;
			case OF_PIXELS_RGBA:
			case OF_PIXELS_BGRA:
				return OF_IMAGE_COLOR_ALPHA;
			case OF_PIXELS_GRAY:
				return OF_IMAGE_GRAYSCALE;
			case OF_PIXELS_GRAY_ALPHA:
			case OF_PIXELS_YUY2:
			case OF_PIXELS_UYVY:
			case OF_PIXELS_Y:
			case OF_PIXELS_U:
			case OF_PIXELS_V:
			case OF_PIXELS_UV:
			case OF_PIXELS_VU:
			case OF_PIXELS_NV12:
			case OF_PIXELS_NV21:
			case OF_PIXELS_YV12:
			case OF_PIXELS_I420:
			case OF_PIXELS_NATIVE:
			case OF_PIXELS_NUM_FORMATS:
			case OF_PIXELS_UNKNOWN:
				ofxTest(false,"asked for image format for pixel format with no correspondence");
				return OF_IMAGE_UNDEFINED;
		}
		return OF_IMAGE_UNDEFINED;
	}

	string formatName(ofPixelFormat pixelFormat){
		switch(pixelFormat){
			case OF_PIXELS_RGB:
				return "RGB";
			case OF_PIXELS_BGR:
				return "BGR";
			case OF_PIXELS_RGB565:
				return "RGB565";
			case OF_PIXELS_RGBA:
				return "RGBA";
			case OF_PIXELS_BGRA:
				return "BGRA";
			case OF_PIXELS_GRAY:
				return "GRAY";
			case OF_PIXELS_GRAY_ALPHA:
				return "GRAY_ALPHA";
			case OF_PIXELS_YUY2:
				return "YUY2";
			case OF_PIXELS_UYVY:
				return "UYVY";
			case OF_PIXELS_Y:
				return "Y";
			case OF_PIXELS_U:
				return "U";
			case OF_PIXELS_V:
				return "V";
			case OF_PIXELS_UV:
				return "UV";
			case OF_PIXELS_VU:
				return "VU";
			case OF_PIXELS_NV12:
				return "NV12";
			case OF_PIXELS_NV21:
				return "NV21";
			case OF_PIXELS_YV12:
				return "YV12";
			case OF_PIXELS_I420:
				return "I420";
			case OF_PIXELS_NATIVE:
				return "NATIVE";
			case OF_PIXELS_NUM_FORMATS:
			case OF_PIXELS_UNKNOWN:
				return "UNKOWN";
		}
		ofxTest(false,"asked for format name for pixel format with no correspondence");
		return "UNKOWN";
	}

	int bitsPerPixel(ofPixelFormat pixelFormat){
		switch(pixelFormat){
			case OF_PIXELS_RGB:
			case OF_PIXELS_BGR:
				return 8*3;
			case OF_PIXELS_RGBA:
			case OF_PIXELS_BGRA:
				return 8*4;
			case OF_PIXELS_GRAY:
			case OF_PIXELS_Y:
			case OF_PIXELS_U:
			case OF_PIXELS_V:
				return 8;
			case OF_PIXELS_GRAY_ALPHA:
			case OF_PIXELS_UV:
			case OF_PIXELS_VU:
				return 8*2;
			case OF_PIXELS_NV12:
			case OF_PIXELS_NV21:
			case OF_PIXELS_YV12:
			case OF_PIXELS_I420:
				return 12;
			case OF_PIXELS_YUY2:
			case OF_PIXELS_UYVY:
			case OF_PIXELS_RGB565:
				return 8*2;
			case OF_PIXELS_NUM_FORMATS:
			case OF_PIXELS_NATIVE:
			case OF_PIXELS_UNKNOWN:
				return 0;
		}
		ofxTest(false,"asked for bits per pixel for pixel format with no correspondence");
		return 0;
	}

	bool hasWorkingIterators(ofPixelFormat pixelFormat){
		switch(pixelFormat){
			case OF_PIXELS_RGB:
			case OF_PIXELS_BGR:
			case OF_PIXELS_RGBA:
			case OF_PIXELS_BGRA:
			case OF_PIXELS_GRAY:
			case OF_PIXELS_GRAY_ALPHA:
			case OF_PIXELS_Y:
			case OF_PIXELS_U:
			case OF_PIXELS_V:
			case OF_PIXELS_UV:
			case OF_PIXELS_VU:
				return true;
			case OF_PIXELS_NV12:
			case OF_PIXELS_NV21:
			case OF_PIXELS_YV12:
			case OF_PIXELS_I420:
			case OF_PIXELS_YUY2:
			case OF_PIXELS_UYVY:
			case OF_PIXELS_RGB565:
			case OF_PIXELS_NUM_FORMATS:
			case OF_PIXELS_NATIVE:
			case OF_PIXELS_UNKNOWN:
				return false;
		}
		ofxTest(false,"asked for working iterators for pixel format with no correspondence");
		return 0;
	}

	int getNumPlanes(ofPixelFormat pixelFormat){
		switch(pixelFormat){
			case OF_PIXELS_RGB:
			case OF_PIXELS_BGR:
			case OF_PIXELS_RGB565:
			case OF_PIXELS_RGBA:
			case OF_PIXELS_BGRA:
			case OF_PIXELS_GRAY:
			case OF_PIXELS_GRAY_ALPHA:
			case OF_PIXELS_YUY2:
			case OF_PIXELS_UYVY:
			case OF_PIXELS_Y:
			case OF_PIXELS_U:
			case OF_PIXELS_V:
			case OF_PIXELS_UV:
			case OF_PIXELS_VU:
				return 1;
			case OF_PIXELS_NV12:
			case OF_PIXELS_NV21:
				return 2;
			case OF_PIXELS_YV12:
			case OF_PIXELS_I420:
				return 3;
			case OF_PIXELS_NUM_FORMATS:
			case OF_PIXELS_NATIVE:
			case OF_PIXELS_UNKNOWN:
				return 0;
		}
		ofxTest(false,"asked for num planes for pixel format with no correspondence");
		return 0;
	}

	bool hasChannels(ofPixelFormat pixelFormat){
		switch(pixelFormat){
			case OF_PIXELS_RGB:
			case OF_PIXELS_BGR:
			case OF_PIXELS_RGB565:
			case OF_PIXELS_RGBA:
			case OF_PIXELS_BGRA:
			case OF_PIXELS_GRAY:
			case OF_PIXELS_GRAY_ALPHA:
			case OF_PIXELS_YUY2:
			case OF_PIXELS_UYVY:
			case OF_PIXELS_Y:
			case OF_PIXELS_U:
			case OF_PIXELS_V:
			case OF_PIXELS_UV:
			case OF_PIXELS_VU:
				return true;
			case OF_PIXELS_NV12:
			case OF_PIXELS_NV21:
			case OF_PIXELS_YV12:
			case OF_PIXELS_I420:
			case OF_PIXELS_NUM_FORMATS:
			case OF_PIXELS_NATIVE:
			case OF_PIXELS_UNKNOWN:
				return false;
		}
		ofxTest(false,"asked for num channels for pixel format with no correspondence");
		return false;
	}

	int numChannels(ofPixelFormat pixelFormat){
		switch(pixelFormat){
			case OF_PIXELS_RGB:
			case OF_PIXELS_BGR:
				return 3;
			case OF_PIXELS_RGB565:
			case OF_PIXELS_GRAY_ALPHA:
			case OF_PIXELS_YUY2:
			case OF_PIXELS_UYVY:
			case OF_PIXELS_UV:
			case OF_PIXELS_VU:
				return 2;
			case OF_PIXELS_RGBA:
			case OF_PIXELS_BGRA:
				return 4;
			case OF_PIXELS_GRAY:
			case OF_PIXELS_Y:
			case OF_PIXELS_U:
			case OF_PIXELS_V:
				return 1;
			case OF_PIXELS_NV12:
			case OF_PIXELS_NV21:
			case OF_PIXELS_YV12:
			case OF_PIXELS_I420:
			case OF_PIXELS_NUM_FORMATS:
			case OF_PIXELS_NATIVE:
			case OF_PIXELS_UNKNOWN:
				return 0;
		}
		ofxTest(false,"asked for num channels for pixel format with no correspondence");
		return 0;
	}

	void run(){
		ofPixels pixels;
		const int w = 320;
		const int h = 240;

		for(ofPixelFormat pixelFormat=OF_PIXELS_GRAY;pixelFormat<OF_PIXELS_NUM_FORMATS;pixelFormat = (ofPixelFormat)(pixelFormat+1)){
			pixels.allocate(w,h,pixelFormat);
			int bpp = bitsPerPixel(pixelFormat);
			string format = formatName(pixelFormat);
			ofxTestEq(pixels.getBitsPerChannel(),8,"getBitsPerChannel() " + format);
			ofxTestEq(pixels.getBitsPerPixel(),bpp,"getBitsPerPixel() " + format);
			ofxTestEq(pixels.getBytesPerChannel(),1,"getBytesPerChannel() " + format);
			ofxTestEq(pixels.getBytesPerPixel(),bpp/8,"getBytesPerPixel() " + format);
			ofxTestEq(pixels.getBytesStride(),w*bpp/8,"getBytesStride() " + format);
			ofxTestEq(pixels.getWidth(),w,"getWidth() " + format);
			ofxTestEq(pixels.getHeight(),h,"getHeight() " + format);
			if(hasImageFormat(pixelFormat)){
				ofxTestEq(pixels.getImageType(),imageType(pixelFormat),"getImageType() " + format);
			}
			if(hasChannels(pixelFormat)){
				ofxTestEq(pixels.getNumChannels(),numChannels(pixelFormat),"getNumChannels() " + format);
			}
			ofxTestEq(pixels.getNumPlanes(),getNumPlanes(pixelFormat),"getNumPlanes() " + format);
			ofxTestEq(pixels.getPixelFormat(),pixelFormat,"getPixelFormat() " + format);
            ofxTestEq((uint64_t)pixels.getPlane(0).getData(), (uint64_t)pixels.getData(),"getPlane(0)==getData() " + format);
			ofxTestEq(pixels.getTotalBytes(),w*h*bpp/8,"getTotalBytes() " + format);

			if(hasWorkingIterators(pixelFormat)){
                ofxTestEq((uint64_t)pixels.getLine(0).begin(), (uint64_t)pixels.getData(),"getLine(0).begin()==getData() " + format);
                ofxTestEq((uint64_t)pixels.getLine(0).end(), (uint64_t)pixels.getData()+(w*bpp/8),"getLine(0).end()==getData()+(w*3) " + format);
                ofxTestEq((uint64_t)pixels.getLine(h-1).begin(), (uint64_t)pixels.getData()+(w*bpp/8*(h-1)),"getLine(h-1).begin()==getData()+(w*bpp/8*(h-1)) " + format);
                ofxTestEq((uint64_t)pixels.getLine(h-1).end(), (uint64_t)pixels.end(),"getLine(h-1).end()==end() " + format);
                ofxTestEq((uint64_t)&pixels.getLine(0).getPixel(10)[0], (uint64_t)pixels.getData()+(10*bpp/8),"getLine(0).getPixel(10)[0]==pixels.getData()+(10*bpp/8)");
			}
		}
	}
};

//========================================================================
int main( ){
	ofInit();
	auto window = make_shared<ofAppNoWindow>();
	auto app = make_shared<ofApp>();
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(window, app);
	return ofRunMainLoop();

}
