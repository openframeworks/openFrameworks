#include "testApp.h"
#include "Utils.h"

#include <Poco/Path.h>

//--------------------------------------------------------------
void testApp::setup(){
	
    
    setOFRoot("../../../../../");

        
    
    
        
    

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

void testApp::makeNewProjectViaDialog(){
    
    Poco::Path path(true);
    path.parse(Poco::Path::current() + "/" + "data/" + getOFRoot());
    
    ofFileDialogResult res = ofSystemSaveDialog("newProjectName", "choose a folder for a new OF project :)");
    if (res.fileName == "" || res.filePath == "") return;
    Poco::Path base(true);
    base.parse(res.filePath);
    base.pushDirectory(res.fileName);   // somehow an extra things here helps?

    
    
    string relPath;
    if (path.toString() == base.toString()){
        // do something.
    }

    int maxx = MAX(base.depth(), path.depth());
    for (int i = 0; i < maxx; i++){
        
        bool bRunOut = false;
        bool bChanged = false;
        if (i < base.depth() && i < path.depth()){
            if (base.directory(i) == path.directory(i)){
                
            } else {
                bChanged = true;
            }
        } else {
            bRunOut = true;
        }
        if (bRunOut == true || bChanged == true){
            for (int j = i; j < base.depth(); j++){
                relPath += "../";
            }
            for (int j = i; j < path.depth()-1; j++){
                relPath += path.directory(j) + "/";
            }
            break;
        }
    }
    
    xcProject.setup();
    xcProject.create(res.filePath);
    
    if (relPath != "../../../"){
        findandreplaceInTexfile(res.filePath + "/" + res.fileName + ".xcodeproj/project.pbxproj", "../../../", relPath);
        findandreplaceInTexfile(res.filePath + "/" + "Project.xcconfig", "../../../", relPath);
        string relPath2 = relPath;
        relPath2.erase(relPath2.end()-1);
        findandreplaceInTexfile(res.filePath + "/" + "Project.xcconfig", "../../..", relPath2);
        
    }
    
}




//--------------------------------------------------------------
void testApp::update(){

    
   
}

//--------------------------------------------------------------
void testApp::draw(){

    ofDrawBitmapString("press 'm' to make all files\npress ' ' to make a specific file", ofPoint(30,30));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if (key == 'm'){
        
        printf("generating examples \n");
        generateExamples();
        printf("finished generating examples \n");
        
    }
    
    if (key == ' '){
        makeNewProjectViaDialog();
    }
    
    
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
