#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofBackground(20);

    // GL_REPEAT for texture wrap only works with NON-ARB textures //
    ofDisableArbTex();
    texture.load("of.png");
    texture.getTexture().setTextureWrap( GL_REPEAT, GL_REPEAT );
    vidGrabber.setup(640, 480, true);

    bFill       = true;
    bWireframe  = true;
    bDrawNormals= false;
    bDrawAxes   = false;
    bDrawLights = false;
    bHelpText   = true;
    bMousePressed   = false;
    bSplitFaces = false;

    float width     = ofGetWidth() * .12;
    float height    = ofGetHeight() * .12;


    plane.set( width*1.5, height*1.5 );
    box.set( width*1.25 );
    sphere.setRadius( width );
    icoSphere.setRadius( width );
    cylinder.set(width*.7, height*2.2);
    cone.set( width*.75, height*2.2 );

    mode = 0;

    ofSetSmoothLighting(true);
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
void ofApp::update() {
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    pointLight2.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()), -300);

    pointLight3.setPosition(
                            cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.2) * ofGetWidth()
    );

	//ofSetWindowTitle("Framerate: "+ofToString(ofGetFrameRate(), 0));
    if(mode == 2 || ofGetElapsedTimef() < 10) {
        vidGrabber.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw() {

	float spinX = sin(ofGetElapsedTimef()*.35f);
	float spinY = cos(ofGetElapsedTimef()*.075f);

	if (bMousePressed) {
		spinX = spinY = 0.0f;
	}

	cam.setGlobalPosition({ 0,0,cam.getImagePlaneDistance(ofGetCurrentViewport()) });
	cam.begin();

	ofEnableDepthTest();

	ofEnableLighting();
	pointLight.enable();
	pointLight2.enable();
	pointLight3.enable();



	// draw the outer sphere
	material.begin();
	ofNoFill();
	ofDrawSphere(0, 0, max(ofGetWidth(),ofGetHeight()));
	material.end();

	if (mode == 1 || mode == 3) texture.getTexture().bind();
	if (mode == 2) vidGrabber.getTexture().bind();

	float screenWidth = ofGetWidth();
	float screenHeight = ofGetHeight();

	plane.setPosition(     -screenWidth * .5 + screenWidth *  1/4.f, screenHeight *  1.1/6.f, 0);
	box.setPosition(       -screenWidth * .5 + screenWidth *  2/4.f, screenHeight *  1.1/6.f, 0);
	sphere.setPosition(    -screenWidth * .5 + screenWidth *  3/4.f, screenHeight *  1.1/6.f, 0);
	icoSphere.setPosition( -screenWidth * .5 + screenWidth *  1/4.f, screenHeight * -1.1/6.f, 0);
	cylinder.setPosition(  -screenWidth * .5 + screenWidth *  2/4.f, screenHeight * -1.1/6.f, 0);
	cone.setPosition(      -screenWidth * .5 + screenWidth *  3/4.f, screenHeight * -1.1/6.f, 0);

	// Plane //

	plane.rotateDeg(spinX, 1.0, 0.0, 0.0);
	plane.rotateDeg(spinY, 0, 1.0, 0.0);



	if (mode == 3) {
		deformPlane = plane.getMesh();
		// x = columns, y = rows //
		ofVec3f planeDims = plane.getResolution();
		float planeAngleX = ofGetElapsedTimef()*3.6;
		float planeAngleInc = 3.f / (float)planeDims.x;
		ofVec3f vert;
		for (size_t i = 0; i < deformPlane.getNumIndices(); i++) {
			planeAngleX += planeAngleInc;
			int ii = deformPlane.getIndex(i);
			vert = deformPlane.getVertex(ii);
			vert.z += cos(planeAngleX) * 50;
			deformPlane.setVertex(ii, vert);
		}
	}

	if (!bFill && bWireframe) {
		// if we are only drawing the wireframe, use
		// the material to draw it, otherwise the material
		// will be bound and unbound for every geometry
		// and the wireframe will be drawn in black
		material.begin();
	}

	if (bFill) {
		material.begin();
		ofFill();
		if (mode == 3) {
			plane.transformGL();
			deformPlane.draw();
			plane.restoreTransformGL();
		}
		else {
			plane.draw();
		}
		material.end();
	}

	if (bWireframe) {
		ofNoFill();
		ofSetColor(0, 0, 0);
		plane.setPosition(plane.getPosition().x, plane.getPosition().y, plane.getPosition().z + 1);
		plane.drawWireframe();
		plane.setPosition(plane.getPosition().x, plane.getPosition().y, plane.getPosition().z - 1);

	}

	// Box //

	box.rotateDeg(spinX, 1.0, 0.0, 0.0);
	box.rotateDeg(spinY, 0, 1.0, 0.0);

	if (bFill) {
		material.begin();
		ofFill();
		if (mode == 3) {
			box.transformGL();
			for (int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++) {
				ofPushMatrix();
				ofTranslate(boxSides[i].getNormal(0) * sin(ofGetElapsedTimef()) * 50);
				boxSides[i].draw();
				ofPopMatrix();
			}
			box.restoreTransformGL();
		}
		else {
			box.draw();
		}
		material.end();
	}

	if (bWireframe) {
		ofNoFill();
		ofSetColor(0, 0, 0);
		box.setScale(1.01f);
		box.drawWireframe();
		box.setScale(1.f);
	}


	// Sphere //
	sphere.rotateDeg(spinX, 1.0, 0.0, 0.0);
	sphere.rotateDeg(spinY, 0, 1.0, 0.0);

	if (mode == 3) {
		sphere.setMode(OF_PRIMITIVE_TRIANGLES);
		triangles = sphere.getMesh().getUniqueFaces();
	}

	if (bFill) {
		material.begin();
		ofFill();
		if (mode == 3) {
			float angle = ofGetElapsedTimef()*3.2;
			float strength = (sin(angle + .25)) * .5f * 5.f;
			ofVec3f faceNormal;
			for (size_t i = 0; i < triangles.size(); i++) {
				// store the face normal here.
				// we change the vertices, which makes the face normal change
				// every time that we call getFaceNormal //
				faceNormal = triangles[i].getFaceNormal();
				for (int j = 0; j < 3; j++) {
					triangles[i].setVertex(j, triangles[i].getVertex(j) + faceNormal * strength);
				}
			}
			sphere.getMesh().setFromTriangles(triangles);
		}
		sphere.draw();
		material.end();
	}

	if (bWireframe) {
		ofNoFill();
		ofSetColor(0, 0, 0);
		sphere.setScale(1.01f);
		sphere.drawWireframe();
		sphere.setScale(1.f);
	}


	// ICO Sphere //
	
	icoSphere.rotateDeg(spinX, 1.0, 0.0, 0.0);
	icoSphere.rotateDeg(spinY, 0, 1.0, 0.0);

	if (mode == 3) {
		triangles = icoSphere.getMesh().getUniqueFaces();
	}

	if (bFill) {
		material.begin();
		ofFill();

		if (mode == 3) {
			float angle = (ofGetElapsedTimef() * 1.4);
			ofVec3f faceNormal;
			for (size_t i = 0; i < triangles.size(); i++) {
				float frc = ofSignedNoise(angle* (float)i * .1, angle*.05) * 4;
				faceNormal = triangles[i].getFaceNormal();
				for (int j = 0; j < 3; j++) {
					triangles[i].setVertex(j, triangles[i].getVertex(j) + faceNormal * frc);
				}
			}
			icoSphere.getMesh().setFromTriangles(triangles);
		}

		icoSphere.draw();
		material.end();
	}

	if (bWireframe) {
		ofNoFill();
		ofSetColor(0, 0, 0);
		icoSphere.setScale(1.01f);
		icoSphere.drawWireframe();
		icoSphere.setScale(1.f);
	}


	// Cylinder //
	if (mode == 3) {
		topCap = cylinder.getTopCapMesh();
		bottomCap = cylinder.getBottomCapMesh();
		body = cylinder.getCylinderMesh();
	}

	cylinder.rotateDeg(spinX, 1.0, 0.0, 0.0);
	cylinder.rotateDeg(spinY, 0, 1.0, 0.0);
	if (bFill) {
		material.begin();
		ofFill();
		if (mode == 3) {
			cylinder.transformGL();
			ofPushMatrix(); {
				if (topCap.getNumNormals() > 0) {
					ofTranslate(topCap.getNormal(0) * (cos(ofGetElapsedTimef() * 5) + 1)*.5f * 100);
					topCap.draw();
				}
			} ofPopMatrix();
			ofPushMatrix(); {
				if (bottomCap.getNumNormals() > 0) {
					ofTranslate(bottomCap.getNormal(0) * (cos(ofGetElapsedTimef() * 4) + 1)*.5f * 100);
					bottomCap.draw();
				}
			} ofPopMatrix();
			ofPushMatrix(); {
				float scale = (cos(ofGetElapsedTimef() * 3) + 1)*.5f + .2;
				ofScale(scale, scale, scale);
				body.draw();
			} ofPopMatrix();
			cylinder.restoreTransformGL();
		}
		else {
			cylinder.draw();
		}
		material.end();
	}

	if (bWireframe) {
		ofNoFill();
		ofSetColor(0, 0, 0);
		cylinder.setScale(1.01f);
		cylinder.drawWireframe();
		cylinder.setScale(1.0f);
	}


	// Cone //
	cone.rotateDeg(spinX, 1.0, 0.0, 0.0);
	cone.rotateDeg(spinY, 0, 1.0, 0.0);

	if (mode == 3) {
		bottomCap = cone.getCapMesh();
		body = cone.getConeMesh();
	}
	if (bFill) {
		material.begin();
		ofFill();
		if (mode == 3) {
			cone.transformGL();
			ofPushMatrix();
			if (bottomCap.getNumNormals() > 0) {
				ofTranslate(bottomCap.getNormal(0) * cone.getHeight()*.5);
				ofRotateDeg(sin(ofGetElapsedTimef() * 5) * RAD_TO_DEG, 1, 0, 0);
				bottomCap.draw();
			}
			ofPopMatrix();

			ofPushMatrix();
			ofRotateDeg(90, 1, 0, 0);
			ofRotateDeg((cos(ofGetElapsedTimef() * 6) + 1)*.5 * 360, 1, 0, 0);
			body.draw();
			ofPopMatrix();
			cone.restoreTransformGL();
		}
		else {
			cone.draw();
		}
		material.end();
	}

	if (bWireframe) {
		ofNoFill();
		ofSetColor(0, 0, 0);
		cone.setScale(1.01f);
		cone.drawWireframe();
		cone.setScale(1.0f);
	}

	if (!bFill && bWireframe) {
		material.end();
	}

	if (mode == 1 || mode == 3) texture.getTexture().unbind();
	if (mode == 2) vidGrabber.getTexture().unbind();

	material.end();
	ofDisableLighting();

	if (bDrawLights) {
		ofFill();
		ofSetColor(pointLight.getDiffuseColor());
		pointLight.draw();
		ofSetColor(pointLight2.getDiffuseColor());
		pointLight2.draw();
		ofSetColor(pointLight3.getDiffuseColor());
		pointLight3.draw();
	}

	if (bDrawNormals) {
		ofSetColor(225, 0, 255);
		plane.drawNormals(20, bSplitFaces);
		box.drawNormals(20, bSplitFaces);
		sphere.drawNormals(20, bSplitFaces);
		icoSphere.drawNormals(20, bSplitFaces);
		cylinder.drawNormals(20, bSplitFaces);
		cone.drawNormals(20, bSplitFaces);
	}
	if (bDrawAxes) {
		plane.drawAxes(plane.getWidth()*.5 + 30);
		box.drawAxes(box.getWidth() + 30);
		sphere.drawAxes(sphere.getRadius() + 30);
		icoSphere.drawAxes(icoSphere.getRadius() + 30);
		cylinder.drawAxes(cylinder.getHeight() + 30);
		cone.drawAxes(cone.getHeight() + 30);
	}

	ofDisableDepthTest();

	ofFill();

	cam.end();
	
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);

	ofDrawBitmapStringHighlight("ofPlanePrimitive"    , glm::vec3(-90,-160,0) + cam.worldToScreen(plane.getGlobalPosition()) );
	ofDrawBitmapStringHighlight("ofBoxPrimitive"      , glm::vec3(-90,-160,0) + cam.worldToScreen( box.getPosition()));
    ofDrawBitmapStringHighlight("ofSpherePrimitive"   , glm::vec3(-90,-160,0) + cam.worldToScreen( sphere.getPosition()));
	ofDrawBitmapStringHighlight("ofIcoSpherePrimitive", glm::vec3(-90,+160,0) + cam.worldToScreen( icoSphere.getPosition()));
	ofDrawBitmapStringHighlight("ofCylinderPrimitive" , glm::vec3(-90,+160,0) + cam.worldToScreen( cylinder.getPosition()));
	ofDrawBitmapStringHighlight("ofConePrimitive"     , glm::vec3(-90,+160,0) + cam.worldToScreen( cone.getPosition()));

    if(bHelpText) {
        stringstream ss;
        ss << "FPS: " << ofToString(ofGetFrameRate(),0) << endl << endl;
        ss << "(f): Toggle Fullscreen"<<endl<<"(s): Draw Solid Shapes"<<endl<<"(w): Draw Wireframes"<<endl;
        ss <<"(1/2/3/4): Set Resolutions" <<endl<<"(n): Draw Normals"<<"\n(LEFT/RIGHT): Set Mode "<<ofToString(mode,0)<<endl;
        ss <<"(z): Split Faces " <<bSplitFaces<<endl;
        ss <<"(a): Draw Axes"<<endl<<"(l): Render lights" << endl <<"(h): Toggle help."<<endl;
        ofDrawBitmapStringHighlight(ss.str().c_str(), 20, 20);
    }


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch(key) {
		case 'f':
			ofToggleFullscreen();
			break;
        case 's':
            bFill = !bFill;
            break;
        case 'w':
            bWireframe = !bWireframe;
            break;
        case '1':
            bSplitFaces=false;
            sphere.setResolution(4);
            // icosahedron //
            icoSphere.setResolution(0); // number of subdivides //
            plane.setResolution(3, 2);
            cylinder.setResolution(4,2,0);
            cone.setResolution(4, 1, 0);
            box.setResolution(1);
            break;
        case '2':
            bSplitFaces=false;
            sphere.setResolution(8);
            icoSphere.setResolution(1);
            plane.setResolution(6, 4);
            cylinder.setResolution(8,4,1);
            cone.setResolution(7, 2, 1);
            box.setResolution(2);
            break;
        case '3':
            bSplitFaces=false;
            sphere.setResolution(16);
            icoSphere.setResolution(2);
            plane.setResolution(8,5);
            cylinder.setResolution(12, 9, 2);
            cone.setResolution(10, 5, 2);
            box.setResolution(6);
            break;
        case '4':
            bSplitFaces=false;
            sphere.setResolution(48);
            icoSphere.setResolution(4);
            plane.setResolution(12, 9);
            cylinder.setResolution(20, 13, 4);
            cone.setResolution(20, 9, 3);
            box.setResolution(10);
            break;
        case 'n':
            bDrawNormals = !bDrawNormals;
            break;
        case OF_KEY_RIGHT:
            mode++;
            if(mode > 3) mode = 0;
            if(mode==3){
                // to get unique triangles, you have to use triangles mode //
                sphere.setMode( OF_PRIMITIVE_TRIANGLES );
            }
            break;
        case OF_KEY_LEFT:
            mode--;
            if(mode < 0) mode = 3;
            if(mode==3){
                // to get unique triangles, you have to use triangles mode //
                sphere.setMode( OF_PRIMITIVE_TRIANGLES );
            }
            break;
        case 'a':
            bDrawAxes = !bDrawAxes;
            break;
        case 'l':
            bDrawLights = !bDrawLights;
            break;
        case 'h':
            bHelpText=!bHelpText;
            break;
        case 'z':
            bSplitFaces = !bSplitFaces;

            if(mode == 3) bSplitFaces = false;

            if(bSplitFaces) {
                sphere.setMode( OF_PRIMITIVE_TRIANGLES );
                vector<ofMeshFace> triangles = sphere.getMesh().getUniqueFaces();
                sphere.getMesh().setFromTriangles( triangles, true );

                icoSphere.setMode( OF_PRIMITIVE_TRIANGLES );
                triangles = icoSphere.getMesh().getUniqueFaces();
                icoSphere.getMesh().setFromTriangles(triangles, true);

                plane.setMode( OF_PRIMITIVE_TRIANGLES );
                triangles = plane.getMesh().getUniqueFaces();
                plane.getMesh().setFromTriangles(triangles, true);

                cylinder.setMode( OF_PRIMITIVE_TRIANGLES );
                triangles = cylinder.getMesh().getUniqueFaces();
                cylinder.getMesh().setFromTriangles(triangles, true);

                cone.setMode( OF_PRIMITIVE_TRIANGLES );
                triangles = cone.getMesh().getUniqueFaces();
                cone.getMesh().setFromTriangles(triangles, true);

                box.setMode( OF_PRIMITIVE_TRIANGLES );
                triangles = box.getMesh().getUniqueFaces();
                box.getMesh().setFromTriangles(triangles, true);

            } else {
                // vertex normals are calculated with creation, set resolution //
                sphere.setResolution( sphere.getResolution() );

                icoSphere.setResolution( icoSphere.getResolution() );
                plane.setResolution( plane.getNumColumns(), plane.getNumRows() );

                cylinder.setResolution( cylinder.getResolutionRadius(), cylinder.getResolutionHeight(), cylinder.getResolutionCap() );
                cone.setResolution( cone.getResolutionRadius(), cone.getResolutionHeight(), cone.getResolutionCap() );
                box.setResolution( box.getResolutionWidth() );
            }
            break;
	}

    if(mode == 1) {
        // resize the plane to the size of the texture //
        plane.resizeToTexture( texture.getTexture() );
        // setTexCoordsFromTexture sets normalized or non-normalized tex coords based on an ofTexture passed in.
        box.mapTexCoordsFromTexture( texture.getTexture() );
        sphere.mapTexCoordsFromTexture( texture.getTexture() );
        icoSphere.mapTexCoordsFromTexture( texture.getTexture() );
        cylinder.mapTexCoordsFromTexture( texture.getTexture() );
        cone.mapTexCoordsFromTexture( texture.getTexture() );
    }

    if(mode == 2) {
        plane.resizeToTexture( vidGrabber.getTexture(), .5 );
        box.mapTexCoordsFromTexture( vidGrabber.getTexture() );
        sphere.mapTexCoordsFromTexture( vidGrabber.getTexture() );
        icoSphere.mapTexCoordsFromTexture( vidGrabber.getTexture() );
        cylinder.mapTexCoordsFromTexture( vidGrabber.getTexture() );
        cone.mapTexCoordsFromTexture( vidGrabber.getTexture() );
    }

    //
    if( mode == 3 ) {

        bSplitFaces = false;

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
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){


}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    bMousePressed = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    bMousePressed = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
