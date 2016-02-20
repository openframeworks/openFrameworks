#include "ofApp.h"

/*
Loads in the same 3D penguin model in various file types. Demonstrates how to load in a model using both ofMesh and ofxAssimpModelLoader.
*/
 
//--------------------------------------------------------------
void ofApp::setup(){	
	ofBackground(255,255,255);
		
	ofSetVerticalSync(true);
    
    // so the model isn't see through.
    ofEnableDepthTest();
    
    // load the first model
    model.loadModel("penguin.dae", 20);
    // model info
    curFileInfo = ".dae";
    
    // this loads in the model directly into a mesh
    // ofMesh can only read in .plys that are in the ascii and not in the binary format.
    bUsingMesh = false;
    mesh.load("penguin.ply");
    
    //you can create as many rotations as you want
    //choose which axis you want it to effect
    //you can update these rotations later on
    // these rotation set the model so it is oriented correctly
    model.setRotation(0, 90, 1, 0, 0);
    model.setRotation(0, 90, 0, 1, 0);
    model.setScale(0.9, 0.9, 0.9);
    model.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    light.setPosition(0, 0, 500);
    
    //set help text to display by default
    bHelpText  = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    // rotates the model in a circle
    model.setRotation(1, 270 + ofGetElapsedTimef() * 60, 0, 1, 0);    
}

//--------------------------------------------------------------
void ofApp::draw(){
	
    light.enable();
    
    // draws the ply file loaded into the mesh is you pressed 6
    if (bUsingMesh){
        cam.begin();
        mesh.draw();
        cam.end();
    }
    // draws all the other file types which are loaded into model.
    else{
        ofColor(255,255);
        model.drawFaces();
    }
    
    light.disable();
    
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
                model.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
                curFileInfo = ".dae";
                break;
            case '2':
                bUsingMesh = false;
                model.loadModel("penguin.3ds");
                model.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
                curFileInfo = ".3ds";
                glShadeModel(GL_SMOOTH);
                break;
            case '3':
                bUsingMesh = false;
                model.loadModel("penguin.ply");
                model.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
                curFileInfo = ".ply";
                break;
            case '4':
                bUsingMesh = false;
                model.loadModel("penguin.obj");
                model.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
                curFileInfo = ".obj";
                break;
            case '5':
                bUsingMesh = false;
            
                model.loadModel("penguin.stl");
                model.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
                curFileInfo = ".stl";
                break;
            case '6':
                bUsingMesh = true;
                cam.setTarget(mesh.getCentroid());
                cam.setPosition(mesh.getCentroid().x, mesh.getCentroid().y, 300);
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
