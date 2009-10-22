#pragma once

#include "ofMain.h"
#include "model3DS.h"

class ofx3DModelLoader{

    public:
    ofx3DModelLoader();
	~ofx3DModelLoader();

    void loadModel(string modelName, float scale = 1.0);
    void setPosition(float x, float y, float z);
    void setScale(float x, float y, float z);
    void setRotation(int which, float angle, float rot_x, float rot_y, float r_z);
    void draw();

    vector <float> rotAngle;
    vector <ofPoint> rotAxis;
    ofPoint scale;
    ofPoint pos;

    int numRotations;
    ofx3DBaseLoader * model;

};
