#pragma once

class ofx3DBaseLoader{

	public:
		ofx3DBaseLoader(){
			hasTexture = false;
		}
		virtual void loadModel(string model, float scale = 1.0){}
		virtual void loadModel(string model){}
		virtual ~ofx3DBaseLoader(){}
		virtual void draw(){ printf("hello\n"); }		
		virtual void draw( GLint paramModelTexture  ){ printf("hello\n"); }

		bool hasTexture;

};