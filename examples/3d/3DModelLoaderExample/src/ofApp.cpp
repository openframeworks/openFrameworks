#include "ofApp.h"

/*
Loads in the same 3D penguin model in various file types. Demonstrates how to load in a model using both ofMesh and ofxAssimpModelLoader.
*/
 
//--------------------------------------------------------------
void ofApp::setup(){	
	ofBackground(255,255,255);
		
	ofSetVerticalSync(true);
    
    // load the first model
    model.loadModel("penguin.dae", 20);
    // model info
    curFileInfo = ".dae";
    
    // this loads in the model directly into a mesh
    // ofMesh can only read in .plys that are in the ascii and not in the binary format.
    bUsingMesh = false;
    mesh.load("penguin.ply");
	meshNode.setOrientation(glm::angleAxis(ofDegToRad(-90.f), glm::vec3{1.f, 0.f, 0.f}));
    
	// you can create as many rotations as you want
	// choose which axis you want it to effect
	// you can update these rotations later on
	// these rotation set the model so it is oriented correctly
	model.setRotation(0, 180, 1, 0, 0);
	model.setScale(0.9, 0.9, 0.9);
    light.setPosition(0, 0, 500);
    
	cameraOrbit = 0;
	cam.setDistance(700);

    //set help text to display by default
    bHelpText  = true;
}

//--------------------------------------------------------------
void ofApp::update(){
	cameraOrbit += ofGetLastFrameTime() * 20.; // 20 degrees per second;
	cam.orbitDeg(cameraOrbit, 0., cam.getDistance(), {0., 0., 0.});
}

//--------------------------------------------------------------
void ofApp::draw(){
	// so the model isn't see through.
	ofEnableDepthTest();
	
    light.enable();

	cam.begin();
	ofColor(255,255);
	if (bUsingMesh){
		// draws the ply file loaded into the mesh is you pressed 6
		meshNode.transformGL();
		mesh.draw();
		meshNode.restoreTransformGL();
	} else {
		// draws all the other file types which are loaded into model.
		model.drawFaces();
    }
	cam.end();
    
    light.disable();

	ofDisableDepthTest();
    
    // display help text if it is enable
    if(bHelpText) {
        stringstream ss;
        ss << "FPS: " << ofToString(ofGetFrameRate(),0) << endl << endl;
        ss << "(1/2/3/4/5/6): load different file types"<<endl;
        ss << "Current file info: " + curFileInfo <<endl;
        if(bUsingMesh){
            ss << "Use ofEasyCam mouse and key controls to navigate."<< endl <<endl;
        }
        ss <<"(h): Toggle help."<<endl;
        ofDrawBitmapString(ss.str().c_str(), 20, 20);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
            case '1':
                bUsingMesh = false;
				model.loadModel("penguin.dae");
				model.setRotation(0, 180, 1, 0, 0);
				model.setScale(0.9, 0.9, 0.9);
				cam.setDistance(700);
                curFileInfo = ".dae";
                break;
            case '2':
                bUsingMesh = false;
				model.loadModel("penguin.3ds");
				model.setRotation(0, 180, 1, 0, 0);
				model.setScale(0.9, 0.9, 0.9);
				cam.setDistance(700);
				curFileInfo = ".3ds";
                break;
            case '3':
                bUsingMesh = false;
				model.loadModel("penguin.ply");
				model.setRotation(0, 90, 1, 0, 0);
				model.setScale(0.9, 0.9, 0.9);
				cam.setDistance(700);
                curFileInfo = ".ply";
                break;
            case '4':
                bUsingMesh = false;
				model.loadModel("penguin.obj");
				model.setRotation(0, 90, 1, 0, 0);
				model.setScale(0.9, 0.9, 0.9);
				cam.setDistance(700);
                curFileInfo = ".obj";
                break;
            case '5':
                bUsingMesh = false;
				model.loadModel("penguin.stl");
				model.setRotation(0, 90, 1, 0, 0);
				model.setScale(0.9, 0.9, 0.9);
				cam.setDistance(700);
                curFileInfo = ".stl";
                break;
            case '6':
				bUsingMesh = true;
				cam.setDistance(40);
                curFileInfo = ".ply loaded directly into ofmesh";
                break;
            case 'h':
                //toggle help text
                bHelpText = !bHelpText;
                break;
            default:
                break;
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

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
