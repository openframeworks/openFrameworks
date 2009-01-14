#ifndef _IMAGE_TEXTURE_H_
#define _IMAGE_TEXTURE_H_

#include "ofConstants.h"
#include "ofGraphics.h"


class ofTexture : public ofBaseDraw{

	public :
		
		ofTexture();
		~ofTexture();
		
		// -----------------------------------------------------------------------
		// the pass by copy stuff was corrupting textures
		// so these things try to prevent that corruption by just
		// not doing any copies in pass by copy or =, ie: textureA = textureB
		ofTexture(const ofTexture& mom); 				// don't allow pass by copy				
		ofTexture& operator=(const ofTexture& mom); 	// don't allow use of =
		// -----------------------------------------------------------------------
		
		void allocate(int w, int h, int internalGlDataType, bool bUseARBExtention = true);	 
		void clear();
		void loadData(unsigned char * data, int w, int h, int glDataType); 
		void loadScreenData(int x, int y, int w, int h);
		void draw(float x, float y, float w, float h);
		void draw(float x, float y);
		
	protected:		
		
		int 			textureTarget;		// normal texture or ARB version of nonpow2 texture
		float 			tex_t, tex_u;   	// internal t,u coords (ie, 0.8 x 0.7)
		int 			tex_w, tex_h;		// internal w,h coords (ie, 512 x 256)
		int 			width, height;		// pixel width	(ie, 320 x 240)
		unsigned int 	textureName[1];		// the texture's "name"
		bool 			bFlipTexture;  		// if we copy in screen data, we need to flip
		
}; 

#endif
