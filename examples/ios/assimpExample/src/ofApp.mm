#include "ofApp.h"
#include "MyGuiView.h"

MyGuiView * myGuiViewController;
//--------------------------------------------------------------
void ofApp::setup(){
    //Our Gui setup
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"MyGuiView" bundle:nil];
    myGuiViewController    =  [storyboard instantiateViewControllerWithIdentifier:@"MyGuiView"];
    [ofxiOSGetGLParentView() addSubview:myGuiViewController.view];
    
    modelPaths = {
        "Fox/Fox_05.fbx",
        "FlightHelmet/FlightHelmet.gltf",
        "Druid/druid.gltf",
        "Astroboy/astroBoy_walk.dae"
    };
    
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(50, 0);
	
	ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.
	modelIndex = 0;
	loadModel(modelIndex);
    bUseCamera = true;
    bAnimate = true;
}

//--------------------------------------------------------------
void ofApp::loadModel(int aindex) {
    loadModel( modelPaths[modelIndex] );
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
    myGuiViewController.view.hidden = NO;
}

//--------------------------------------------------------------
void ofApp::loadModel(bool decrease){
    modelIndex += decrease ? -1 : 1;
    if(modelIndex < 0){
        modelIndex = modelPaths.size() - 1;
    }else if(modelIndex >= modelPaths.size()){
        modelIndex = 0;
    }
    loadModel(modelIndex);
}

//--------------------------------------------------------------
void ofApp::showNextAnimation(){
    if( model.hasAnimations() ){
        cout<<"stop animation"<<endl;
        model.stopAllAnimations();
        animationIndex++;
        cout<<"update animation"<<animationIndex<<endl;
        animationIndex %= model.getAnimationCount();
        model.getAnimation(animationIndex).play();
    }
}

//--------------------------------------------------------------
void ofApp::updateAnimation(float val){
    animationPosition = val;
}

//--------------------------------------------------------------
void ofApp::toggleAnimation(bool t){
    bAnimate = t;
}

//--------------------------------------------------------------
void ofApp::toggleCamera(bool t){
    bUseCamera = t;
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
    
    if(model.hasAnimations()){
        if(!bAnimate){
            model.setPositionForAllAnimations(animationPosition);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
	//note we have to enable depth buffer in main.mm
	//see: window->enableDepthBuffer(); in main.mm
    ofSetColor(255);
    
    ofEnableDepthTest();
    
    if(bUseCamera)cam.begin();
    
    ofEnableLighting();
    light.enable();
    ofEnableSeparateSpecularLight();
    
    ofPushMatrix();
    if( !bUseCamera ) {
        ofTranslate(model.getPosition().x, model.getPosition().y, 0);
        ofRotateDeg(mouseX+30, 0, 1, 0);
        ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
    }
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
    string status = "fps: " + ofToString(ofGetFrameRate(), 2)
    + "\n" + "num animations for this model: " + ofToString(model.getAnimationCount()) + "\n"
    + " <- -> to change models" + "\n"
    + "current animation number: " + ofToString(animationIndex);
    [myGuiViewController setStatusString:ofxStringToNSString(status)];
}

//--------------------------------------------------------------
void ofApp::exit(){
	
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
	
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
    if( myGuiViewController.view.hidden ){
        myGuiViewController.view.hidden = NO;
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

