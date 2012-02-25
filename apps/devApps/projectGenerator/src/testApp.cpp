#include "testApp.h"
#include "Utils.h"

//--------------------------------------------------------------
void testApp::setup(){
	
    
    setOFRoot("../../../../../");

    
    ofDirectory dir;
    dir.listDir("../../../../../examples/basicExamples");
    for (int i = 0; i < dir.size(); i++){
        xcProject.setup();
        cout << "making xcode project at " << dir.getPath(i) << endl;
        xcProject.create(dir.getPath(i));
        vector < string > addons;
        parseAddonsDotMake(xcProject.getPath() + "addons.make", addons);
        for (int i = 0; i < addons.size(); i++){
            ofAddon addon;
            addon.fromFS(getOFRoot()+ "/addons/" + addons[i],"osx");
            xcProject.addAddon(addon);
        }
    }
    
    dir.listDir("../../../../../examples/addonsExamples");
    for (int i = 0; i < dir.size(); i++){
        xcProject.setup();
        xcProject.create(dir.getPath(i));
        vector < string > addons;
        parseAddonsDotMake(xcProject.getPath() + "addons.make", addons);
        for (int i = 0; i < addons.size(); i++){
            
            printf("---> adding addon %s \n", addons[i].c_str());
                   
            ofAddon addon;
            addon.fromFS(getOFRoot()+ "/addons/" + addons[i],"osx");
            xcProject.addAddon(addon);
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
