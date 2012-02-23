#include "testApp.h"
#include "Utils.h"

//--------------------------------------------------------------
void testApp::setup(){
	
    
    setOFRoot("../../../../../");

    printf("generating examples \n");
    generateExamples();
    printf("finished generating examples \n");
    

}

void testApp::generateExamples(){
    
    ofDirectory dir;
    dir.listDir("../../../../../examples");
    
    for (int i = 0; i < dir.size(); i++){
        
        if (dir.getName(i) == "android" || dir.getName(i) == "ios") continue;
        
        ofDirectory subdir;
        subdir.listDir(dir.getPath(i));
        
        for (int j = 0; j < subdir.size(); j++){
            xcProject.setup();
            xcProject.create(subdir.getPath(j));
            vector < string > addons;
            parseAddonsDotMake(xcProject.getPath() + "addons.make", addons);
            for (int i = 0; i < addons.size(); i++){
                ofAddon addon;
                addon.fromFS(getOFRoot()+ "/addons/" + addons[i],"osx");
                xcProject.addAddon(addon);
            }
        }
    }
    
}




//--------------------------------------------------------------
void testApp::update(){

    
   
}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
