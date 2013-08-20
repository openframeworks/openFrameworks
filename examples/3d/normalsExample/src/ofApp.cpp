#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
		
    radius = 400;
    max = 60;
    
    ofEnableDepthTest(); //make sure we test depth for 3d
    
	ofSetVerticalSync(true);
    ofEnableLighting();
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    mesh.addVertex(ofPoint(0,0,0)); // add center vertex
    mesh.addColor(ofColor(137,137,140,255)); // center is same as bg
    mesh.addNormal(ofVec3f(0,0,1)); // center normal points up
    
    zfreq = 3.;
    zamt = .3;
    
    //loop around and make verts in a circle, with a bit of a z-wave
    for (int i = 0; i < max; i++){
        float step = 2*PI/max; // step size around circle
        float theta = ofMap(i, 0, max-1, 0, 2*PI - step); //map i as circle divisions to actual radian values around the circle (note we don't go quite all the way around by one step, because it will be the same as where we started, so we'll just index that starting vertex when we make faces)
 
        float prevTheta = theta - step; //one step back
        float nextTheta = theta + step; // one step forward
        
        // create vertices in polar coordinates, plus a sine wave for z
        ofVec3f p(radius*cos(theta),radius*sin(theta), radius*zamt*sin(zfreq*theta) );
        // add this vertex
        mesh.addVertex(p);
        
        // we need these for calculating normals
        ofVec3f prev(radius*cos(prevTheta),radius*sin(prevTheta),radius*zamt*sin(zfreq*prevTheta) );        
        ofVec3f next(radius*cos(nextTheta),radius*sin(nextTheta),radius*zamt*sin(zfreq*nextTheta) );
        
        // our normals for each triangle face is the cross product of the two vectors making up that sliver 
        ofVec3f previousFaceNormal = prev.crossed(p);
        ofVec3f nextFaceNormal = p.crossed(next);
        
        /* notice here we go in the same direction: previous->current,current->next;
           we could similarly go next->current,current-prev, which would flip all of our normals;
           this might not be the best idea, but its certainly better than going previous->current,next->current, which would end up being quite awful.
           This is the concept of an "orientable mesh" or "face winding order", to be googled for more information.
         */
        
        // since we want smooth normals, we'll sum the two adjacent face normals, then normalize (since usually, only the direction and not the magnitude of the normal is what matters)
        mesh.addNormal((previousFaceNormal + nextFaceNormal).normalize());
        
        //add a color too
        ofColor c;
        c.setHsb(40 + 30*sin(2*theta+PI),255,255,255);        
        mesh.addColor(c);
    }
    
    //index our verts/normals/colors as a triangle fan
    for (int i=0, j = max-1; i < max; j=i++){
        mesh.addIndex(0);
        mesh.addIndex(i+1);
        mesh.addIndex(j+1);
    }
    
    // light the scene to show off why normals are important
    light.enable();
    light.setPointLight();
    light.setPosition(0,0,300);
		
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableLighting();
    ofBackgroundGradient(ofColor(65,62,50),ofColor(25,22,10) );
    
    // disable normals if a key is pressed
    if(ofGetKeyPressed() || ofGetMousePressed()){
        mesh.disableNormals();
    }else{
        mesh.enableNormals();
    }
    
    cam.begin();
    mesh.enableColors();
    mesh.drawWireframe();
    mesh.disableColors();
    ofSetColor(137,137,140);
    ofFill();

    #ifndef TARGET_OPENGLES
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1,-1);
    #endif

    mesh.drawFaces();
    ofSetColor(255,255,255);
    light.draw();
    
    
    // draw our normals, and show that they are perpendicular to the vector from the center to the vertex
    vector<ofVec3f> n = mesh.getNormals();
    vector<ofVec3f> v = mesh.getVertices();
    float normalLength = 50.;
    
    if(!ofGetKeyPressed()){
        ofDisableLighting();
        ofSetColor(255,255,255,70);         
        for(unsigned int i=0; i < n.size() ;i++){
            ofLine(v[i].x,v[i].y,v[i].z,
                   v[i].x+n[i].x*normalLength,v[i].y+n[i].y*normalLength,v[i].z+n[i].z*normalLength);

            ofLine(.98*v[i].x,.98*v[i].y,.98*v[i].z,
                   .98*v[i].x+n[i].x*normalLength*.2,.98*v[i].y+n[i].y*normalLength*.2,.98*v[i].z+n[i].z*normalLength*.2);
            ofLine(.98*v[i].x+n[i].x*normalLength*.2,.98*v[i].y+n[i].y*normalLength*.2,.98*v[i].z+n[i].z*normalLength*.2,
                   v[i].x+n[i].x*normalLength*.2,v[i].y+n[i].y*normalLength*.2,v[i].z+n[i].z*normalLength*.2);
        }
    }
               

    cam.end();

    ofSetColor(255,255,255);
    ofDrawBitmapString("press any key or mouse button to disable mesh normals", 20,20);
    ofDrawBitmapString("light", cam.worldToScreen(light.getGlobalPosition()) + ofPoint(10,0));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
