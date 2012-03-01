#include "ofx3DModelLoader.h"

//-------------------------------------------
static string getExtension( string filename ){
	std::string::size_type idx;
	idx = filename.rfind('.');

	if(idx != std::string::npos){
		return filename.substr(idx+1);
	}
	else{
		return "";
	}
}

//-------------------------------------------
ofx3DModelLoader::ofx3DModelLoader(){
    numRotations = 0;
    rotAngle.clear();
    rotAxis.clear();
    scale = ofPoint(1, 1, 1);
	
	model = NULL;
}

//------------------------------------------
void ofx3DModelLoader::loadModel(string modelName, float scale){
	if( model != NULL)delete model;
	
	string ext = getExtension( modelName );
	if( ext == "3ds" || ext == "3DS" ){	
		model = new model3DS();
        ofLog(OF_LOG_VERBOSE, "ofx3DModelLoader: Loading Model " + modelName);
		model->loadModel( modelName.c_str(), scale);
	}
	
}

//-------------------------------------------
ofx3DModelLoader::~ofx3DModelLoader(){
	if( model != NULL )delete model;
}

//-------------------------------------------
void ofx3DModelLoader::setPosition(float x, float y, float z){
    pos.x = x;
    pos.y = y;
    pos.z = z;
}

void ofx3DModelLoader::setScale(float x, float y, float z){
    scale.x = x;
    scale.y = y;
    scale.z = z;
}

//-------------------------------------------
void ofx3DModelLoader::setRotation(int which, float angle, float rot_x, float rot_y, float rot_z){

    if(which + 1 > numRotations){
        int diff = 1 + (which - numRotations);
        for(int i = 0; i < diff; i++){
            rotAngle.push_back(0);
            rotAxis.push_back(ofPoint());
        }
        numRotations = rotAngle.size();
    }

    rotAngle[which]  = angle;
    rotAxis[which].x = rot_x;
    rotAxis[which].y = rot_y;
    rotAxis[which].z = rot_z;
}

//-------------------------------------------
void ofx3DModelLoader::draw(){
    glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glScalef(scale.x, scale.y, scale.z);
        for(int i = 0; i < numRotations; i++){
            glRotatef(rotAngle[i], rotAxis[i].x, rotAxis[i].y, rotAxis[i].z);
        }

        glShadeModel(GL_SMOOTH);
		if( model->hasTexture)
			glEnable(GL_TEXTURE_2D);
        model->draw();
        if( model->hasTexture)
			glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
