#include "testApp.h"
#include "Utils.h"
#include <stdio.h>



#ifdef TARGET_WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#elif defined(TARGET_LINUX)
    #include <unistd.h>
    #define GetCurrentDir getcwd
#else
    #include <mach-o/dyld.h>	/* _NSGetExecutablePath */
    #include <limits.h>		/* PATH_MAX */
#endif




//--------------------------------------------------------------
void testApp::setup(){

	while(!checkConfigExists()){
		askOFRoot();
	}

	setOFRoot(getOFRootFromConfig());

    switch(ofGetTargetPlatform()){
    case OF_TARGET_OSX:
    	project = new xcodeProject;
    	platform = "osx";
    	break;
    case OF_TARGET_WINGCC:
    	project = new CBWinProject;
    	platform = "wincb";
    	break;
    case OF_TARGET_WINVS:
    	project = new visualStudioProject;
    	platform = "vs2010";
    	break;
    case OF_TARGET_IPHONE:
    	break;
    case OF_TARGET_ANDROID:
    	break;
    case OF_TARGET_LINUX:
    	project = new CBLinuxProject;
    	platform = "linux";
    	break;
    case OF_TARGET_LINUX64:
    	project = new CBLinuxProject;
    	platform = "linux64";
    	break;
    }

    if(projectPath!=""){
        project->setup(getOFRelPath(projectPath));
        project->create(projectPath);
        vector < string > addons;
        parseAddonsDotMake(project->getPath() + "addons.make", addons);
        for (int i = 0; i < addons.size(); i++){
            ofAddon addon;
            addon.fromFS(ofFilePath::join(ofFilePath::join(getOFRoot(), "addons"), addons[i]),platform);
            project->addAddon(addon);
        }
        std::exit(0);
    }

    panelAddons.setup("addons");
    ofDirectory addons(ofFilePath::join(getOFRoot(),"addons"));
    addons.listDir();
    for(int i=0;i<(int)addons.size();i++){
    	string addon = addons.getName(i);
    	if(addon.find("ofx")==0){
    		ofxToggle * toggle = new ofxToggle();
    		panelAddons.add(toggle->setup(addon,false,300));
    	}
    }

    panelOptions.setup("","settings.xml",ofGetWidth()-panelAddons.b.width-10,120);
    panelOptions.add(createProject.setup("create/update project",300));
    panelOptions.add(createAndOpen.setup("create and open project",300));
    panelOptions.add(changeOFRoot.setup("change OF path",300));

    createProject.addListener(this,&testApp::createProjectPressed);
    createAndOpen.addListener(this,&testApp::createAndOpenPressed);
    changeOFRoot.addListener(this,&testApp::changeOFRootPressed);

    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
}

void testApp::generateExamples(){
    
    ofDirectory dir;
    //../../../../../
    dir.listDir(ofFilePath::join(getOFRoot(),"apps/examples"));
    
    for (int i = 0; i < dir.size(); i++){
        
        if (dir.getName(i) == "android" || dir.getName(i) == "ios") continue;
        
        ofDirectory subdir;
        subdir.listDir(dir.getPath(i));
        
        for (int j = 0; j < subdir.size(); j++){
            project->setup();
            project->create(subdir.getPath(j));
            vector < string > addons;
            parseAddonsDotMake(project->getPath() + "addons.make", addons);
            for (int i = 0; i < addons.size(); i++){
                ofAddon addon;
                addon.fromFS(ofFilePath::join(ofFilePath::join(getOFRoot(), "addons"), addons[i]),platform);
                project->addAddon(addon);
            }
        }
    }
}

ofFileDialogResult testApp::makeNewProjectViaDialog(){
    ofFileDialogResult res = ofSystemSaveDialog("newProjectName", "choose a folder for a new OF project :)");
    if (res.fileName == "" || res.filePath == "") return res;
    //base.pushDirectory(res.fileName);   // somehow an extra things here helps?
    
    project->setup(getOFRelPath(res.filePath));
    project->create(res.filePath);
    vector<string> addonsToggles = panelAddons.getControlNames();
	for (int i = 0; i < addonsToggles.size(); i++){
		ofxToggle toggle = panelAddons.getToggle(addonsToggles[i]);
		if(toggle){
			ofAddon addon;
			addon.fromFS(ofFilePath::join(ofFilePath::join(getOFRoot(), "addons"), addonsToggles[i]),platform);
			project->addAddon(addon);
		}
	}
    return res;
}

void testApp::createProjectPressed(bool & pressed){
	if(!pressed) makeNewProjectViaDialog();
}

void testApp::createAndOpenPressed(bool & pressed){
	if(!pressed){
		ofFileDialogResult res = makeNewProjectViaDialog();
		if(res.bSuccess){
			#ifdef TARGET_LINUX
				system(("/usr/bin/codeblocks " + ofFilePath::join(res.filePath, res.fileName+".workspace ") + "&").c_str());
			#elif defined(TARGET_OSX)
				system(("open " + ofFilePath::join(res.filePath, res.fileName+".xcodeproj ") + "&").c_str());
			#elif defined(TARGET_WIN32)
				system(("open " + ofFilePath::join(res.filePath, res.fileName+".workspace ") + "&").c_str());
			#endif
		}
	}
}

void testApp::changeOFRootPressed(bool & pressed){
	if(!pressed) askOFRoot();
}



//--------------------------------------------------------------
void testApp::update(){

    
   
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor::gray,ofColor::black);
	panelAddons.draw();
	panelOptions.draw();

	ofSetColor(0,0,0,100);
	ofRect(ofGetWidth()-410,10,400,100);

    /*ofDrawBitmapString("press 'm' to make all files\npress ' ' to make a specific file", ofPoint(30,30));*/
	ofSetColor(255);
    ofDrawBitmapString("OF path: " + getOFRoot(), ofPoint(ofGetWidth() - 390,30));
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
