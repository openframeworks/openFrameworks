#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(50, 0);

    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.

    bAnimate = true;
    bAnimateMouse = false;
    animationPosition = 0;

    loadModel("Fox/Fox_05.fbx");

    bHelpText = true;
}

//--------------------------------------------------------------
void ofApp::loadModel(string filename){
    
    if( model.load(filename, ofxAssimpModelLoader::OPTIMIZE_DEFAULT) ){
        if( model.hasAnimations() ){
            animationIndex = 0;
            model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
            model.getAnimation(animationIndex).play();
        }
    }else{
        ofLogError() << " can't load model: " << filename << endl;
    }
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
    float sceneHeight = fabs((model.getSceneMaxModelSpace()-model.getSceneMinModelSpace()).y);

    if(bUseCamera){
        model.setScale(1, -1, 1);
        model.setRotation(0, 180, 0, 1, 0);
        model.setPosition(0, -sceneHeight * 0.5, 0);
        light.setPosition(model.getPosition() + glm::vec3(-300, 300, 1200));
    }else{
        model.setScale(1, 1, 1);
        model.setRotation(0, 180, 0, 1, 0);
        model.setPosition(ofGetWidth()/2, ofGetHeight()/2 + sceneHeight * 0.5, 0);
        light.setPosition(model.getPosition() + glm::vec3(-300, -300, 1200));
    }
        
    model.update();

    if( model.hasAnimations() ){
        if(bAnimateMouse) {
            model.setPositionForAllAnimations(animationPosition);
        }
        mesh = model.getCurrentAnimatedMesh(0);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);

    ofEnableDepthTest();
    
    if(bUseCamera)cam.begin();
    
        ofEnableLighting();
        light.enable();
        ofEnableSeparateSpecularLight();

        ofPushMatrix();
            ofTranslate(model.getPosition().x, model.getPosition().y, 0);
            ofRotateDeg(mouseX+30, 0, 1, 0);
            ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
            
            model.drawFaces();
        ofPopMatrix();

        light.disable();
        ofDisableLighting();
        ofDisableSeparateSpecularLight();
 
    if( bUseCamera ){
        ofDrawSphere(light.getPosition(), 10);
    }
 
    if(bUseCamera)cam.end();
    ofDisableDepthTest();

    if(bHelpText){
        ofSetColor(255, 255, 255 );
        string str;
        str += "FPS: " + ofToString(ofGetFrameRate(),0) + "\n\n";
        str +="(keys 1-5): load models\n";
        str += "num of animations in this model: " + ofToString(model.getAnimationCount()) + " <- -> to change\n";
        str +="(Spacebar): toggle animation\n";
        str +="(LEFT MOUSE BUTTON DRAG in y-axis): control animation.\n";
        str += "(c): toggle camera: " + (bUseCamera ? string(" using ofEasyCam with (0,0) as screen center. \n") : string(" default view (0,0) is top left \n"));
        str += "(h): toggle help.\n";
        ofDrawBitmapString(str, 20, 20);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch (key) {
        case '1':
            loadModel("Fox/Fox_05.fbx");
            break;
        case '2':
            loadModel("FlightHelmet/FlightHelmet.gltf");
            break;
        case '3':
            loadModel("Druid/druid.gltf");
            break;
        case '4':
            loadModel("Astroboy/astroBoy_walk.dae");
            break;
        case '5':
            loadModel("Payphone/korean_public_payphone_01_1k.gltf");
            break;
        case 'c':
            bUseCamera = !bUseCamera;
            break;
        case ' ':
            bAnimate = !bAnimate;
            if( model.hasAnimations() ){
                if( bAnimate ){
                    model.getAnimation(animationIndex).play();
                }else{
                    model.stopAllAnimations();
                }
            }
            break;
        case 'h':
            bHelpText = !bHelpText;
            break;
        case OF_KEY_RIGHT:
            if( model.hasAnimations() ){
                model.stopAllAnimations();
                animationIndex++;
                animationIndex %= model.getAnimationCount();
                model.getAnimation(animationIndex).play();
            }
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	// scrub through aninations manually.
	animationPosition = y / (float)ofGetHeight();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	// pause all animations, so we can scrub through them manually.
	model.setPausedForAllAnimations(true);
	animationPosition = y / (float)ofGetHeight();
	bAnimateMouse = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	// unpause animations when finished scrubbing.
	if(bAnimate) {
		model.setPausedForAllAnimations(false);
	}
	bAnimateMouse = false;
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
    loadModel(dragInfo.files[0]);
}

