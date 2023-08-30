#pragma once

#include "ofMain.h"

#define USE_MATERIAL

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
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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
	
		bool rayIntersectsTriangle( glm::vec3& aP, glm::vec3& aDirection,
							   glm::vec3& aV0, glm::vec3& aV1, glm::vec3& aV2, glm::vec3& aIntersectionPt );
		float calcTriArea( glm::vec3& aA, glm::vec3& aB, glm::vec3& aC );
		void calcTriArea( glm::vec3& aA, glm::vec3& aB, glm::vec3& aC, glm::vec3& aP, float& a1, float& a2, float& a3 );

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
		
	
		#ifdef USE_MATERIAL
		vector< shared_ptr<ofMaterial> > materials;
		int materialIndex = 0;
		#else
		vector< shared_ptr<TexturePack> > texturePacks;
		int texturePackIndex = 0;
		#endif
		
		
		ofTexture textureBrush;
		
		ofVboMesh meshSphere;
		ofMesh colliderMesh;
		ofMesh mHitMeshTri;
		
		ofEasyCam camera;
		
		#ifndef USE_MATERIAL
		shared_ptr<ofShader> shader;
		#endif
		
		ofFbo fboInfluence;
	
		bool bDrawIntoFbo = false;
		bool bDebug = false;
		bool bBrushDrawing = false;
	
		float mBrushSize = 150.f;
		float mBrushStrength = 12.f;
	
		glm::vec3 mHitSpherePos = {0,0,0};
		glm::vec3 mHitNormal = {1,0,0};
		glm::vec2 mHitCoord = {0,0};
		bool mBHitATri = false;
	
		glm::vec2 mInputPos = {0,0};	
};
