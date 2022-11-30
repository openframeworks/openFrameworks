#pragma once

#include "ofMain.h"


class TexturePack {
	public:
		TexturePack() {};
		TexturePack(string aPackname ){
			load(aPackname);
		}
		
		bool load( string aPackName ){
			string spath = ofToLower(aPackName);
					
			bool bok = ofLoadImage(textureDiffuseMap, "textures/"+aPackName+"/"+spath+"_diff.jpg");
			bok |= ofLoadImage(textureNormalMap, "textures/"+aPackName+"/"+spath+"_nor_gl.jpg");
			bok |= ofLoadImage(textureDisplacementMap, "textures/"+aPackName+"/"+spath+"_disp.png");
			bok |= ofLoadImage(textureAORoughMetal, "textures/"+aPackName+"/"+spath+"_arm.jpg");
			
			if( textureDiffuseMap.isAllocated() ) {
				configureTexture(textureDiffuseMap);
			}
			if( textureNormalMap.isAllocated() ) {
				configureTexture(textureNormalMap);
			}
			if(textureDisplacementMap.isAllocated() ) {
				configureTexture(textureDisplacementMap);
			}
			if(textureAORoughMetal.isAllocated()) {
				configureTexture(textureAORoughMetal);
			}
			
			return bok;
		};
		
		void configureTexture( ofTexture& atex ){
			atex.setTextureWrap(GL_REPEAT, GL_REPEAT);
			atex.generateMipmap();
			
			GLfloat fLargest = 1.0;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
			
			atex.bind();
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
			#ifndef TARGET_EMSCRIPTEN
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			#endif
			atex.unbind();
			
			atex.setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		}
		// diffuse texture map holds the color information
		ofTexture textureDiffuseMap;
		// texture holding the normal values for the mesh that will be used for lighting in the frag shader
		ofTexture textureNormalMap;
		// texture holding the values for how far to offset the mesh vertices in vertex shader
		ofTexture textureDisplacementMap;
		// texture holding ambient occlusion, roughness and metal properties
		// we will only be using the ambient occlusion value stored in the red channel
		ofTexture textureAORoughMetal;
		
		ofMaterial material;
		
		ofFloatColor baseColor;
} ;

class ofApp : public ofBaseApp{
	public:
		
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		vector< shared_ptr<ofLight> > lights;
		
		vector< shared_ptr<TexturePack> > texturePacks;
		int texturePackIndex = 0;
		
		ofTexture textureBrush;
		
		ofVboMesh meshSphere;
		
		ofEasyCam camera;
		
		shared_ptr<ofShader> shader;
		
		ofFbo fboInfluence;
		ofRectangle fboRect;
		
		bool bDrawIntoFbo = false;
		bool bDebug = false;
	
};
