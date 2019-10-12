#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	//If you want a landscape oreintation
    ofSetOrientation(OF_ORIENTATION_90_RIGHT);
	ofSetFrameRate(60);
	ofBackground(20);
    
    // GL_REPEAT for texture wrap only works with NON-ARB textures //
    ofDisableArbTex();
    texture.load("of.png");
    texture.getTexture().setTextureWrap( GL_REPEAT, GL_REPEAT );
    
    bFill       = true;
    bWireframe  = true;
    bDrawNormals= true;
    bDrawLights = false;
    bInfoText   = true;
    
    float width     = ofGetWidth() * .12;
    float height    = ofGetHeight() * .12;
    
    
    plane.set( width*1.5, height*1.5 );
    box.set( width*1.25 );
    sphere.setRadius( width );
    icoSphere.setRadius( width );
    cylinder.set(width*.7, height*2.2);
    cone.set( width*.75, height*2.2 );
    
    mode        = 0;
    numTouches  = 0;
    
    //ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    pointLight2.setDiffuseColor( ofFloatColor( 238.f/255.f, 57.f/255.f, 135.f/255.f ));
    pointLight2.setSpecularColor(ofFloatColor(.8f, .8f, .9f));
    
    pointLight3.setDiffuseColor( ofFloatColor(19.f/255.f,94.f/255.f,77.f/255.f) );
    pointLight3.setSpecularColor( ofFloatColor(18.f/255.f,150.f/255.f,135.f/255.f) );
    
    // shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess( 120 );
    // the light highlight of the material //
	material.setSpecularColor(ofColor(255, 255, 255, 255));
    
    ofSetSphereResolution(24);
}

//--------------------------------------------------------------
void ofApp::update(){
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    pointLight2.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()), -300);
    
    pointLight3.setPosition(
                            cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.2) * ofGetWidth()
                            );
    
	ofSetWindowTitle("Framerate: "+ofToString(ofGetFrameRate(), 0));
}

//--------------------------------------------------------------
void ofApp::draw(){
	float spinX = sin(ofGetElapsedTimef()*.35f);
    float spinY = cos(ofGetElapsedTimef()*.075f);
    
    ofEnableDepthTest();
    
    ofEnableLighting();
    pointLight.enable();
    pointLight2.enable();
    pointLight3.enable();
    
	material.begin();
    
    
    ofSetColor(180);
    ofNoFill();
    ofDrawSphere(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth());
    
    if(mode == 1 || mode == 2) texture.getTexture().bind();
    
    
    // Plane //
    plane.setPosition(ofGetWidth()*.2, ofGetHeight()*.25, 0);
    plane.rotate(spinX, 1.0, 0.0, 0.0);
    plane.rotate(spinY, 0, 1.0, 0.0);
    
    
    ofMesh deformPlane;
    if(mode == 2) {
        deformPlane = plane.getMesh();
        // x = columns, y = rows //
        glm::vec3 planeDims = plane.getResolution();
        float planeAngleX = ofGetElapsedTimef()*3.6;
        float planeAngleInc = 3.f/(float)planeDims.x;
        glm::vec3 vert;
        for(int i = 0; i < deformPlane.getNumIndices(); i++ ) {
            planeAngleX += planeAngleInc;
            int ii = deformPlane.getIndex( i );
            vert = deformPlane.getVertex( ii );
            vert.z += cos(planeAngleX) * 50;
            deformPlane.setVertex( ii, vert );
        }
    }
    
    if(bFill) {
        ofFill();
        ofSetColor(255);
        if(mode == 2) {
            plane.transformGL();
            deformPlane.draw();
            plane.restoreTransformGL();
        } else {
            plane.draw();
        }
    }
    if(bWireframe) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        if(!bFill) ofSetColor(255);
        plane.setPosition(plane.getPosition().x, plane.getPosition().y, plane.getPosition().z+1);
        //if(bFill) {
        if( mode == 2 ) {
            ofSetColor(255);
        }
        plane.drawWireframe();
        //}
        plane.setPosition(plane.getPosition().x, plane.getPosition().y, plane.getPosition().z-2);
        
    }
    
    
    // Box //
    box.setPosition(ofGetWidth()*.5, ofGetHeight()*.25, 0);
    box.rotate(spinX, 1.0, 0.0, 0.0);
    box.rotate(spinY, 0, 1.0, 0.0);
    
    if(bFill) {
        ofFill();
        ofSetColor(255);
        if(mode == 2) {
            box.transformGL();
            for(int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++ ) {
                glPushMatrix();
                ofTranslate( boxSides[i].getNormal(0) * sin(ofGetElapsedTimef()) * 50  );
                boxSides[i].draw();
                glPopMatrix();
            }
            box.restoreTransformGL();
        } else {
            box.draw();
        }
    }
    if(bWireframe) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        if(!bFill) ofSetColor(255);
        box.setScale(1.01f);
        if(mode == 2) {
            ofSetColor(255);
        }
        box.drawWireframe();
        box.setScale(1.f);
    }
    
    
    // Sphere //
    sphere.setPosition(ofGetWidth()*.8f, ofGetHeight()*.25, 0);
    sphere.rotate(spinX, 1.0, 0.0, 0.0);
    sphere.rotate(spinY, 0, 1.0, 0.0);
    
    
    vector<ofMeshFace> triangles;
    if(mode == 2) {
        // to get unique triangle, you have to use triangles mode //
        sphere.setMode( OF_PRIMITIVE_TRIANGLES );
        triangles = sphere.getMesh().getUniqueFaces();
    }
    
    if(bFill) {
        ofFill();
        ofSetColor(255);
        if(mode == 2) {
            float angle = ofGetElapsedTimef()*3.2;
            float strength = (sin( angle )) * .25f * 2.f;
            glm::vec3 faceNormal;
            for(int i = 0; i < triangles.size(); i++ ) {
                // store the face normal here.
                // we change the vertices, which makes the face normal change
                // every time that we call getFaceNormal //
                faceNormal = triangles[i].getFaceNormal();
                for(int j = 0; j < 3; j++ ) {
                    triangles[i].setVertex( j, triangles[i].getVertex(j) + faceNormal * strength);
                }
            }
            sphere.getMesh().setFromTriangles( triangles );
        }
        sphere.draw();
        
    }
    if(bWireframe) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        if(!bFill) ofSetColor(255);
        sphere.setScale(1.01f);
        sphere.drawWireframe();
        sphere.setScale(1.f);
    }
    
    
    // ICO Sphere //
    icoSphere.setPosition(ofGetWidth()*.2, ofGetHeight()*.75, 0);
    icoSphere.rotate(spinX, 1.0, 0.0, 0.0);
    icoSphere.rotate(spinY, 0, 1.0, 0.0);
    
    if(mode == 2) {
        triangles = icoSphere.getMesh().getUniqueFaces();
    }
    
    if(bFill) {
        ofFill();
        ofSetColor(255);
        
        if(mode == 2) {
            float angle = (ofGetElapsedTimef() * 1.4);
            glm::vec3 faceNormal;
            for(int i = 0; i < triangles.size(); i++ ) {
                float frc = ofSignedNoise(angle* (float)i * .1, angle*.05) * 4;
                faceNormal = triangles[i].getFaceNormal();
                for(int j = 0; j < 3; j++ ) {
                    triangles[i].setVertex(j, triangles[i].getVertex(j) + faceNormal * frc );
                }
            }
            icoSphere.getMesh().setFromTriangles( triangles );
        }
        
        icoSphere.draw();
    }
    if(bWireframe) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        if(!bFill) ofSetColor(255);
        icoSphere.setScale(1.01f);
        icoSphere.drawWireframe();
        icoSphere.setScale(1.f);
    }
    
    
    // Cylinder //
    
    ofMesh topCap, bottomCap, body;
    
    if(mode == 2) {
        topCap      = cylinder.getTopCapMesh();
        bottomCap   = cylinder.getBottomCapMesh();
        body        = cylinder.getCylinderMesh();
    }
    
    cylinder.setPosition(ofGetWidth()*.5, ofGetHeight()*.75, 0);
    cylinder.rotate(spinX, 1.0, 0.0, 0.0);
    cylinder.rotate(spinY, 0, 1.0, 0.0);
    if(bFill) {
        ofFill();
        ofSetColor(255);
        if(mode == 2) {
            cylinder.transformGL();
            glPushMatrix(); {
                if(topCap.getNumNormals() > 0) {
                    ofTranslate( topCap.getNormal(0) * (cos(ofGetElapsedTimef()*5)+1)*.5f * 100 );
                    topCap.draw();
                }
            } glPopMatrix();
            glPushMatrix(); {
                if(bottomCap.getNumNormals() > 0) {
                    ofTranslate( bottomCap.getNormal(0) * (cos(ofGetElapsedTimef()*4)+1)*.5f * 100 );
                    bottomCap.draw();
                }
            } glPopMatrix();
            glPushMatrix(); {
                float scale = (cos(ofGetElapsedTimef()*3)+1)*.5f + .2;
                ofScale( scale, scale, scale );
                body.draw();
            } glPopMatrix();
            cylinder.restoreTransformGL();
        } else {
            cylinder.draw();
        }
    }
    if(bWireframe) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        if(!bFill || mode == 2) ofSetColor(255);
        cylinder.setScale(1.01f);
        cylinder.drawWireframe();
        cylinder.setScale(1.0f);
    }
    
    
    // Cone //
    cone.setPosition(ofGetWidth()*.8, ofGetHeight()*.75, 0);
    cone.rotate(spinX, 1.0, 0.0, 0.0);
    cone.rotate(spinY, 0, 1.0, 0.0);
    
    if(mode == 2) {
        bottomCap   = cone.getCapMesh();
        body        = cone.getConeMesh();
    }
    if(bFill) {
        ofFill();
        ofSetColor(255);
        if(mode == 2) {
            cone.transformGL();
            glPushMatrix();
            if(bottomCap.getNumNormals() > 0 ) {
                ofTranslate( bottomCap.getNormal(0) * cone.getHeight()*.5 );
                glRotatef( sin(ofGetElapsedTimef()*5) * RAD_TO_DEG, 1, 0, 0);
                bottomCap.draw();
            }
            glPopMatrix();
            
            glPushMatrix();
            glRotatef(90, 1, 0, 0);
            glRotatef( (cos(ofGetElapsedTimef()*6) +1)*.5 * 360 , 1, 0, 0 );
            body.draw();
            glPopMatrix();
            cone.restoreTransformGL();
        } else {
            cone.draw();
        }
    }
    if(bWireframe) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        if(!bFill || mode == 2) ofSetColor(255);
        cone.setScale(1.01f);
        cone.drawWireframe();
        cone.setScale(1.0f);
    }
    
    if(mode == 1 || mode == 2) texture.getTexture().unbind();
    
    material.end();
    ofDisableLighting();
    
    if(bDrawLights) {
        ofFill();
        ofSetColor(pointLight.getDiffuseColor());
        pointLight.draw();
        ofSetColor(pointLight2.getDiffuseColor());
        pointLight2.draw();
        ofSetColor(pointLight3.getDiffuseColor());
        pointLight3.draw();
    }
    
    if(bDrawNormals) {
        ofSetColor(225, 0, 255);
        plane.drawNormals(20);
        box.drawNormals(20);
        sphere.drawNormals(20);
        icoSphere.drawNormals(20);
        cylinder.drawNormals(20);
        cone.drawNormals(20);
    }
    
    ofDisableDepthTest();
    
    ofFill();
    ofSetColor(255, 255, 255);
    
    if(bInfoText) {
        stringstream ss;
        ss << "Framerate: " << ofToString(ofGetFrameRate(),0) << "\n";
        ss << "(touch id 2): Draw Normals"<<endl;
        ss << "(tap): Set Resolutions = "<<numTouches <<endl;
        ss << "(DoubleTap): Set Mode "<<ofToString(mode,0)<<endl;
        
        ofDrawBitmapString(ss.str().c_str(), 20, 20);
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch) {
    if(touch.id==0) {
        numTouches++;
        if(numTouches > 2) numTouches=0;
        if(numTouches== 0) {
            sphere.setResolution(4);
            // icosahedron //
            icoSphere.setResolution(0); // number of subdivides //
            plane.setResolution(3, 2);
            cylinder.setResolution(4,2,2);
            cone.setResolution(5, 2, 2);
            box.setResolution(2);
        } else if (numTouches == 1) {
            sphere.setResolution(8);
            icoSphere.setResolution(1);
            plane.setResolution(6, 4);
            cylinder.setResolution(8,5);
            cone.setResolution(7, 3, 2);
            box.setResolution(4);
        } else if(numTouches == 2) {
            sphere.setResolution(12);
            icoSphere.setResolution(2);
            plane.setResolution(8,5);
            cylinder.setResolution(10, 9, 3);
            cone.setResolution(8, 5, 3);
            box.setResolution(6);
        }
    } else if (touch.id == 1) {
        bDrawNormals = !bDrawNormals;
    } else if (touch.id == 2) {
        bInfoText = !bInfoText;
    }
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
    mode++;
    if(mode > 2) mode = 0;
    
    if(mode == 1) {
        // resize the plane to the size of the texture //
        float tscale = 1.0f;
        if(ofGetWidth() < 1024) tscale = 0.4f;
        plane.resizeToTexture( texture.getTexture(), tscale );
        // setTexCoordsFromTexture sets normalized or non-normalized tex coords based on an ofTexture passed in.
        box.mapTexCoordsFromTexture( texture.getTexture() );
        sphere.mapTexCoordsFromTexture( texture.getTexture() );
        icoSphere.mapTexCoordsFromTexture( texture.getTexture() );
        cylinder.mapTexCoordsFromTexture( texture.getTexture() );
        cone.mapTexCoordsFromTexture( texture.getTexture() );
    }
    
    //
    if( mode == 2 ) {
        
        // if the faces were split, we can get some weird results, since we
        // might not know what the new strides were,
        // so reset the primitives by calling their setMode function
        // which recreates the mesh with the proper indicies //
        sphere.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        icoSphere.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        cylinder.setMode( OF_PRIMITIVE_TRIANGLE_STRIP );
        cone.setMode( OF_PRIMITIVE_TRIANGLE_STRIP );
        // box only supports triangles //
        box.setMode( OF_PRIMITIVE_TRIANGLES );
        
        plane.setMode( OF_PRIMITIVE_TRIANGLE_STRIP );
        plane.mapTexCoords(0, 0, 5, 5);
        
        // rebuild the box,
        box.mapTexCoords(0, 0, 5, 5);
        sphere.mapTexCoords(0, 0, 5, 5);
        icoSphere.mapTexCoords(0, 0, 5, 5);
        cylinder.mapTexCoords(0, 0, 5, 5);
        cone.mapTexCoords(0, 0, 5, 5);
        
        // store the box sides so that we can manipulate them later //
        for(int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++ ) {
            boxSides[i] = box.getSideMesh( i );
        }
    }
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){

}

//--------------------------------------------------------------
void ofApp::gotFocus(){

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){

}

