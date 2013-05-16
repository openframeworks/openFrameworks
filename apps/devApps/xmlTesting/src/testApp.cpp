#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);

    //load a monospaced font
	//which we will use to show part of the xml structure
	//TTF.loadFont("mono.ttf", 7);
    
    ofFile file;
    file.open("mySettings1.xml");
    ofBuffer buffer = file.readToBuffer();
    XML.loadFromBuffer(buffer.getText());
    
    
    
    ofXml newXML;
    // check here
    newXML.addChild("bar");
    newXML.setCurrentElement("bar");
    newXML.setAttribute("Giroud", "France");
    //cout << newXML.toString() << endl;
    newXML.addChild("baz");
    newXML.setAttribute("/test1/test2[@Attr]", "Value");
    
    map<string, string> attrMap = newXML.getAttributes();
    
    cout << attrMap.size() << endl;
    
    newXML.addValue("flim", "flam");
    newXML.addValue("flim2", "flam2");
    newXML.setCurrentElement("../");
    
    cout << newXML.toString() << endl;
    
    XML.resetCurrentElement();
    XML.addXml(newXML);
    
    //cout << XML.toString() << endl;
    
    ofXml tmp = XML;
    
    //tmp.setCurrentElement("food[0]/serving/@units");

    cout << " TMP XML VALUE IS " << tmp.getValue("food[0]/serving[@units]") << endl;
    
    
}

//--------------------------------------------------------------
void testApp::update(){
	//we change the background color here based on the values
	//affected by the mouse position
	/*ofBackground((int)red,(int)green,(int)blue);
    
    if( ofGetFrameNum() % 30 == 0 )
    {
    
        ofXml aNewXML;
        aNewXML.addChild("root");
        aNewXML.setCurrentElement("root");
        aNewXML.addChild("fooy");
        aNewXML.setCurrentElement("fooy");
        aNewXML.addValue("bar/eggs/spam", "quux");
        aNewXML.resetCurrentElement();
        aNewXML.addValue("foox/bar/eggs/ham", "qux");
        
        cout << aNewXML.toString() << endl;
        
    }*/

}

//--------------------------------------------------------------
void testApp::draw(){
    ofDrawBitmapString(ofToString(ofGetFrameNum()), ofPoint(50, 50));
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

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

    /*cout << XML.toString() << endl;
    
    cout << " number before remove " <<  XML.getNumChildren() << endl;
    XML.remove("food[0]");
    cout << " number after remove " << XML.getNumChildren() << endl;
    
    ofXml name = XML.getChild("food[2]/name");
    cout << name.getValue() << endl;
    ofXml child = XML.getChildAt(0);
    
    cout << child.getNumChildren() << endl;
    
    XML.addValue("food[2]/blah", "barf");
    ofXml blah = XML.getChild("food[2]/blah");
    
    XML.addValue("foo", "blaf");
    
    cout << XML.toString() << endl;
    
    //cout << " number after add " << XML.getNumChildren() << endl;*/
    
    /*ofXml aNewXML;
    aNewXML.addChild("root");
    
    aNewXML.getChild("root").addValue("foox/bar/eggs/ham", "qux");
    
    //cout << aNewXML.toString() << endl;
    
    aNewXML.getChildAt(0).addValue("fooy/bar/eggs/spam", "quux");
    aNewXML.getChildAt(0).addValue("fooz/bar/green_eggs/butter/yoyo/spam/He/will/miss/the/second/leg", "quuux");
    
    //cout << aNewXML.toString() << endl;
    
    for( int i = 0; i < 100; i++) {
        ofXml tmp;
        tmp.addValue("number", ofToString(i));
        aNewXML.getChild("root").addChild(tmp);
    }*/
    
    //cout << aNewXML.toString() << endl;
    
    cout << XML.toString() << endl;
    
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

