#pragma once

#import "aiColor4D.h"
#import "aiVector3D.h"
#import "aiVector2D.h"
#import "aiMatrix4x4.h"


class ofx3DBaseLoader{

	public:
		ofx3DBaseLoader(){
			hasTexture = false;
		}
		virtual void loadModel(string model, float scale = 1.0){}
		virtual void loadModel(string model){}
		virtual ~ofx3DBaseLoader(){}
		virtual void draw(){ printf("hello\n"); }

		bool hasTexture;

};
