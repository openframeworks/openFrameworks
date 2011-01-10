#pragma once

#include "ofBaseTypes.h"

class ofPixels : public ofBaseHasPixels{
	public:
		ofPixels(){
			//TODO: smart pointer here?
			pixels			= NULL;
			bitsPerPixel	= 0;
			bytesPerPixel	= 0;
			width			= 0;
			height			= 0;
			glDataType		= GL_LUMINANCE;
			ofImageType		= OF_IMAGE_UNDEFINED;
			bAllocated		= false;
		}
		
		unsigned char * getPixels(){
			return pixels;
		}

		int getPixelIndex(int x, int y){
			if( !bAllocated ){
				return 0;
			}else{
				return ( x + y * width ) * bytesPerPixel; 
			}
		}
		
		ofColor getPixel(int x, int y){
			ofColor c;
			int index = getPixelIndex(x, y);
			
			if( bytesPerPixel == 1 ){			
				c.set( pixels[index] );
			}else if( bytesPerPixel == 3 ){
				c.set( pixels[index], pixels[index+1], pixels[index+2] );
			}else if( bytesPerPixel == 4 ){
				c.set( pixels[index], pixels[index+1], pixels[index+2], pixels[index+3] );
			}
			
			return c;
		}
		
		int getWidth(){
			return width;
		}
		
		int getHeight(){
			return height;
		}
		
		//TODO: make protected? ofPixels feels like it should have easy access / low level 
		
		//TODO: template? allows for other pixel types
		//TODO: smart pointer for pixels??
		unsigned char * pixels;
		int width;
		int height;

		int		bitsPerPixel;		// 8 = gray, 24 = rgb, 32 = rgba
		int		bytesPerPixel;		// 1, 3, 4 bytes per pixels
		
		//TODO:  should we have GL info here???
		GLint	glDataType;			// GL_LUMINANCE, GL_RGB, GL_RGBA
		int		ofImageType;		// OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, OF_IMAGE_COLOR_ALPHA
		
		bool	bAllocated;

};