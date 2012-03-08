#include "testApp.h"
#include "Utils.h"
#include <stdio.h>

//--------------------------------------------------------------
void testApp::setup(){
	project = NULL;
	
	while(!checkConfigExists()){
		askOFRoot();
	}

	setOFRoot(getOFRootFromConfig());

	int plat = ofGetTargetPlatform();
	//plat = OF_TARGET_IPHONE;
	
    setupForPlatform(plat);

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
        project->save(projectPath);
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

    panelOptions.setup("","settings.xml",ofGetWidth()-panelAddons.getWidth()-10,120);
    panelOptions.add(createProject.setup("create project",300));
    panelOptions.add(updateProject.setup("update project",300));
    panelOptions.add(createAndOpen.setup("create and open project",300));
    panelOptions.add(changeOFRoot.setup("change OF path",300));

    createProject.addListener(this,&testApp::createProjectPressed);
    updateProject.addListener(this,&testApp::updateProjectPressed);
    createAndOpen.addListener(this,&testApp::createAndOpenPressed);
    changeOFRoot.addListener(this,&testApp::changeOFRootPressed);
	
	examplesPanel.setup("generate examples", "examples.xml", 400, 10);
	examplesPanel.add(generateButton.setup("<--Generate"));
	examplesPanel.add(wincbToggle.setup("win CB projects",false));
	examplesPanel.add(winvsToggle.setup("win VS projects", false));
	examplesPanel.add(linuxcbToggle.setup("linux CB projects",false));
	examplesPanel.add(osxToggle.setup("osx projects",false));
	examplesPanel.add(iosToggle.setup("ios projects",false));
	
	generateButton.addListener(this,&testApp::generateExamplesCB);

    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
	ofSetFrameRate(60);
}

void testApp::setupForPlatform(int plat){
	if(project){
		delete project;
	}
	
	switch(plat){
    case OF_TARGET_OSX:
    	project = new xcodeProject;
    	platform = "osx";
		((xcodeProject *)project)->setupForPlatform(platform);
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
		project = new xcodeProject();
    	platform = "ios";		
		((xcodeProject *)project)->setupForPlatform(platform);		
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
}

void testApp::generateExamplesCB(bool & pressed){

	vector <int> platformsToMake;
	if( osxToggle )		platformsToMake.push_back(OF_TARGET_OSX);
	if( iosToggle )		platformsToMake.push_back(OF_TARGET_IPHONE);
	if( wincbToggle )	platformsToMake.push_back(OF_TARGET_WINGCC);
	if( winvsToggle )	platformsToMake.push_back(OF_TARGET_WINVS);
	if( linuxcbToggle )	platformsToMake.push_back(OF_TARGET_LINUX);
	
	if( platformsToMake.size() == 0 ){
		cout << "Error: generateExamplesCB - must specifiy a project to generate " <<endl;
	}

	for(int i = 0; i < platformsToMake.size(); i++){
		setupForPlatform(platformsToMake[i]);
		generateExamples();
	}
	
	int plat = ofGetTargetPlatform();	
    setupForPlatform(plat);
}

void testApp::generateExamples(){
    ofDirectory dir;
    
    dir.listDir(ofFilePath::join(getOFRoot(),"examples"));

    for (int i = 0; i < dir.size(); i++){
        
		if( platform == "ios" ){
			if( dir.getName(i) != "ios" ) continue;
		}else{
			if (dir.getName(i) == "android" || dir.getName(i) == "ios") continue;
        }
		
        ofDirectory subdir;
        subdir.listDir(dir.getPath(i));
        
        for (int j = 0; j < subdir.size(); j++){
            project->setup();
            project->create(subdir.getPath(j));
            vector < string > addons;
            parseAddonsDotMake(project->getPath() + "addons.make", addons);
            for (int i = 0; i < addons.size(); i++){
                ofAddon addon;
                addon.pathToOF = getOFRelPath(subdir.getPath(j));
                addon.fromFS(ofFilePath::join(ofFilePath::join(getOFRoot(), "addons"), addons[i]),platform);
                project->addAddon(addon);
            }
            project->save(subdir.getPath(j));
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
            addon.pathToOF = getOFRelPath(res.filePath);
			addon.fromFS(ofFilePath::join(ofFilePath::join(getOFRoot(), "addons"), addonsToggles[i]),platform);
			printf("adding %s addons \n", addonsToggles[i].c_str());
            project->addAddon(addon);
            
		}
	}
    project->save(res.filePath);
    
    return res;
}

ofFileDialogResult testApp::updateProjectViaDialog(){
    ofFileDialogResult res = ofSystemLoadDialog("choose a folder to update an OF project :)",true);
    if (res.fileName == "" || res.filePath == "") return res;
    //base.pushDirectory(res.fileName);   // somehow an extra things here helps?

    project->setup(getOFRelPath(res.filePath));
	project->create(res.filePath);
	vector<string> addonsToggles = panelAddons.getControlNames();
	for (int i = 0; i < addonsToggles.size(); i++){
		ofxToggle toggle = panelAddons.getToggle(addonsToggles[i]);
		if(toggle){
			ofAddon addon;
			addon.pathToOF = getOFRelPath(res.filePath);
			addon.fromFS(ofFilePath::join(ofFilePath::join(getOFRoot(), "addons"), addonsToggles[i]),platform);
			printf("adding %s addons \n", addonsToggles[i].c_str());
			project->addAddon(addon);

		}
	}
	project->save(res.filePath);

	return res;
}

void testApp::createProjectPressed(bool & pressed){
	if(!pressed) makeNewProjectViaDialog();
}

void testApp::updateProjectPressed(bool & pressed){
	if(!pressed) updateProjectViaDialog();
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
	
    //ofBackgroundGradient(ofColor::gray,ofColor::black);
	
    panelAddons.draw();
	panelOptions.draw();
	examplesPanel.draw();
	
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
