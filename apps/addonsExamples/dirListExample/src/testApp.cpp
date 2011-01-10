#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){


    DIR.setVerbose(false);
    nImages = DIR.listDir("images/of_logos");
 	images = new ofImage[nImages];
    //you can now iterate through the files as you like
    for(int i = 0; i < nImages; i++){
            images[i].loadImage(DIR.getPath(i));
    }
    currentImage = 0;

    ofBackground(255,255,255);

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){



    if (nImages > 0){
        ofSetHexColor(0xffffff);
        images[currentImage].draw(300,50);
        ofSetHexColor(0x999999);
        string pathInfo = DIR.getPath(currentImage)
                          + "\n\n" + "press any key to advance current image"
                           + "\n\n" + "many thanks to hikaru furuhashi for the OFs" ;
        ofDrawBitmapString(pathInfo, 300,images[currentImage].height + 80);
    }

    ofSetHexColor(0x000000);
    for(int i = 0; i < nImages; i++){
            if (currentImage == i) ofSetHexColor(0xff0000);
            else ofSetHexColor(0x000000);
            string fileInfo = "file " + ofToString(i+1) + " = " + DIR.getName(i); // +  "path is " + DIR.getPath(i);
            ofDrawBitmapString(fileInfo, 50,i*20 + 50);
    }


}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    if (nImages > 0){
        currentImage++;
        currentImage %= nImages;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

